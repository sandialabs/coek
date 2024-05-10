// #define _GLIBCXX_USE_CxX11_ABI 0
#include <iostream>
#include <sstream>
#include <unordered_map>

#include "../../ast/value_terms.hpp"
#include "coek/api/expression.hpp"
#include "coek/api/constraint.hpp"
#include "coek/api/objective.hpp"
#include "coek/compact/constraint_sequence.hpp"
#include "coek/compact/expression_sequence.hpp"
#include "coek/compact/objective_sequence.hpp"
#include "coek/model/model.hpp"
#include "coek/model/model_repn.hpp"
#include "coek_gurobi.hpp"
#include "gurobi_c++.h"

namespace coek {

SolverRepn* create_gurobi_solver() { return new GurobiSolver(); }

namespace {

auto add_gurobi_variable(GRBModel* gmodel, double lb, double ub, const Variable& eval)
{
    if (eval.is_binary())
        return gmodel->addVar(lb, ub, 0, GRB_BINARY);
    else if (eval.is_integer())
        return gmodel->addVar(lb, ub, 0, GRB_INTEGER);
    else {
        if (ub >= 1e19) {
            if (lb <= -1e19)
                return gmodel->addVar(-GRB_INFINITY, GRB_INFINITY, 0, GRB_CONTINUOUS);
            else
                return gmodel->addVar(lb, GRB_INFINITY, 0, GRB_CONTINUOUS);
        }
        else {
            if (lb <= -1e19)
                return gmodel->addVar(-GRB_INFINITY, ub, 0, GRB_CONTINUOUS);
            else
                return gmodel->addVar(lb, ub, 0, GRB_CONTINUOUS);
        }
    }
}

void add_gurobi_objective(GRBModel* gmodel, Expression& expr, bool sense,
                          std::unordered_map<size_t, GRBVar>& x, coek::QuadraticExpr& orepn)
{
    orepn.reset();
    orepn.collect_terms(expr);

    if (orepn.linear_coefs.size() + orepn.quadratic_coefs.size() > 0) {
        GRBLinExpr term1;
        auto iv = orepn.linear_vars.begin();
        for (auto it = orepn.linear_coefs.begin(); it != orepn.linear_coefs.end(); ++it, ++iv)
            term1 += *it * x[(*iv)->index];
        term1 += orepn.constval;

        if (orepn.quadratic_coefs.size() == 0)
            gmodel->setObjective(term1);
        else {
            GRBQuadExpr quadexpr;
            for (size_t i = 0; i < orepn.quadratic_coefs.size(); i++)
                quadexpr.addTerm(orepn.quadratic_coefs[i], x[orepn.quadratic_lvars[i]->index],
                                 x[orepn.quadratic_rvars[i]->index]);
            quadexpr.add(term1);
            gmodel->setObjective(quadexpr);
        }
    }

    if (sense)
        gmodel->set(GRB_IntAttr_ModelSense, GRB_MINIMIZE);
    else
        gmodel->set(GRB_IntAttr_ModelSense, GRB_MAXIMIZE);
}

void add_gurobi_constraint(GRBModel* gmodel, Constraint& con, std::unordered_map<size_t, GRBVar>& x,
                           coek::QuadraticExpr& repn)
{
    repn.reset();
    // coek::QuadraticExpr repn;
    repn.collect_terms(con);

    if (repn.linear_coefs.size() + repn.quadratic_coefs.size() > 0) {
        GRBLinExpr term1;
        for (size_t i = 0; i < repn.linear_coefs.size(); i++)
            term1 += repn.linear_coefs[i] * x[repn.linear_vars[i]->index];
        if (repn.quadratic_coefs.size() > 0) {
            GRBQuadExpr term2;
            for (size_t i = 0; i < repn.quadratic_coefs.size(); i++)
                term2.addTerm(repn.quadratic_coefs[i], x[repn.quadratic_lvars[i]->index],
                              x[repn.quadratic_rvars[i]->index]);

            if (con.is_inequality()) {
                if (con.lower().repn) {
                    double lower = con.lower().value();
                    if (lower > -COEK_INFINITY)
                        gmodel->addQConstr(term1 + term2, GRB_GREATER_EQUAL,
                                           -repn.constval + lower);
                }
                if (con.upper().repn) {
                    double upper = con.upper().value();
                    if (upper < COEK_INFINITY)
                        gmodel->addQConstr(term1 + term2, GRB_LESS_EQUAL, -repn.constval + upper);
                }
            }
            else
                gmodel->addQConstr(term1 + term2, GRB_EQUAL, -repn.constval + con.lower().value());
        }
        else {
            if (con.is_inequality()) {
                if (con.lower().repn) {
                    double lower = con.lower().value();
                    if (lower > -COEK_INFINITY)
                        gmodel->addConstr(term1, GRB_GREATER_EQUAL, -repn.constval + lower);
                }
                if (con.upper().repn) {
                    double upper = con.upper().value();
                    if (upper < COEK_INFINITY)
                        gmodel->addConstr(term1, GRB_LESS_EQUAL, -repn.constval + upper);
                }
            }
            else
                gmodel->addConstr(term1, GRB_EQUAL, -repn.constval + con.lower().value());
        }
    }
}
}  // namespace

GurobiSolver::~GurobiSolver()
{
    if (gmodel)
        delete gmodel;
    if (env)
        delete env;
}

void GurobiSolver::set_gurobi_options()
{
    // All options are converted to strings for Gurobi
    for (auto& it : string_options())
        gmodel->set(it.first, it.second);
    for (auto& it : boolean_options())
        gmodel->set(it.first, std::to_string(it.second));
    for (auto& it : integer_options())
        gmodel->set(it.first, std::to_string(it.second));
    for (auto& it : double_options())
        gmodel->set(it.first, std::to_string(it.second));
}

void GurobiSolver::pre_solve()
{
    results = std::make_shared<SolverResults>();
    results->solver_name = "gurobi";
    results->termination_condition = TerminationCondition::error;
    results->tic();

    if (initial_solve()) {
        env = new GRBEnv(true);
        auto it = integer_options().find("OutputFlag");
        if (it != integer_options().end())
            env->set(GRB_IntParam_OutputFlag, it->second);
        env->start();
        gmodel = new GRBModel(*env);
    }
}

void GurobiSolver::post_solve()
{
    delete gmodel;
    gmodel = 0;
    delete env;
    env = 0;

    results->toc();
}

std::shared_ptr<SolverResults> GurobiSolver::solve(Model& model)
{
    pre_solve();
    auto _model = model.repn.get();
    results->model_name = _model->name;

    // Add Gurobi variables
    size_t nv = 0;
    for (auto& var : _model->variables) {
        if (not var.fixed()) {
            x[var.id()] = add_gurobi_variable(gmodel, var.lower(), var.upper(), var);
            nv++;
        }
    }
    if (nv == 0) {
        results->termination_condition = TerminationCondition::empty_model;
        post_solve();
        return results;
    }

    gmodel->update();

    // Add Gurobi objective
    int nobj = 0;
    try {
        coek::QuadraticExpr orepn;
        for (auto& obj : model.repn->objectives) {
            if (obj.active()) {
                Expression tmp = obj.expr();
                add_gurobi_objective(gmodel, tmp, obj.sense(), x, orepn);
                nobj++;
            }
        }
    }
    catch (GRBException e) {
        results->error_message
            = "Gurobi Error: Caught gurobi exception while creating objectives " + e.getMessage();
        post_solve();
        return results;
    }
    if (nobj > 1) {
        // TODO - is this an error?
        results->termination_condition = TerminationCondition::invalid_model_for_solver;
        results->error_message = "Error initializing Gurobi: More than one objective defined!";
        post_solve();
        return results;
    }

    // Add Gurobi constraints
    try {
        coek::QuadraticExpr repn;
        for (auto& con : _model->constraints) {
            if (con.active())
                add_gurobi_constraint(gmodel, con, x, repn);
        }
    }
    catch (GRBException e) {
        results->error_message
            = "Gurobi Error: Caught gurobi exception while creating constraints " + e.getMessage();
        post_solve();
        return results;
    }

    try {
        set_gurobi_options();
        gmodel->optimize();
    }
    catch (GRBException e) {
        results->error_message
            = "Gurobi Error: Caught gurobi exception while optimizing " + e.getMessage();
        post_solve();
        return results;
    }

    collect_results(model, results);

    post_solve();
    return results;
}

#ifdef COEK_WITH_COMPACT_MODEL
std::shared_ptr<SolverResults> GurobiSolver::solve(CompactModel& compact_model)
{
    pre_solve();
    results->model_name = compact_model.name();

    // Add Gurobi variables
    size_t nv = 0;
    for (auto& val : compact_model.repn->variables) {
        if (auto eval = std::get_if<Variable>(&val)) {
            Expression lb = eval->lower_expression().expand();
            auto lb_ = lb.value();
            Expression ub = eval->upper_expression().expand();
            auto ub_ = ub.value();
            x[eval->id()] = add_gurobi_variable(gmodel, lb_, ub_, *eval);
            nv++;
        }
        else {
            auto& seq = std::get<VariableSequence>(val);
            for (auto jt = seq.begin(); jt != seq.end(); ++jt) {
                x[jt->id()] = add_gurobi_variable(gmodel, jt->lower(), jt->upper(), *jt);
                nv++;
            }
        }
    }
    if (nv == 0) {
        results->termination_condition = TerminationCondition::empty_model;
        post_solve();
        return results;
    }

    gmodel->update();

    // Add Gurobi objective
    int nobj = 0;
    try {
        coek::QuadraticExpr orepn;
        for (auto& val : compact_model.repn->objectives) {
            if (auto eval = std::get_if<Objective>(&val)) {
                if (eval.active()) {
                    Expression tmp = eval->expr().expand();
                    add_gurobi_objective(gmodel, tmp, eval->sense(), x, orepn);
                    nobj++;
                }
            }
            else {
                auto& seq = std::get<ObjectiveSequence>(val);
                // TODO - active()
                for (auto jt = seq.begin(); jt != seq.end(); ++jt) {
                    Expression tmp = jt->expr();
                    add_gurobi_objective(gmodel, tmp, jt->sense(), x, orepn);
                    nobj++;
                }
            }
        }
    }
    catch (GRBException e) {
        results->error_message
            = "Gurobi Error: Caught gurobi exception while creating objectives " + e.getMessage();
        post_solve();
        return results;
    }
    if (nobj > 1) {
        // TODO - is this an error?
        results->termination_condition = TerminationCondition::invalid_model_for_solver;
        results->error_message = "Error initializing Gurobi: More than one objective defined!";
        post_solve();
        return results;
    }

    // Add Gurobi constraints
    try {
        coek::QuadraticExpr repn;
        for (auto& val : compact_model.repn->constraints) {
            if (auto cval = std::get_if<Constraint>(&val)) {
                if (cval.active()) {
                    Constraint c = cval->expand();
                    add_gurobi_constraint(gmodel, c, x, repn);
                }
            }
            else {
                auto& seq = std::get<ConstraintSequence>(val);
                // TODO - active()
                for (auto jt = seq.begin(); jt != seq.end(); ++jt) {
                    add_gurobi_constraint(gmodel, *jt, x, repn);
                }
            }
        }
    }
    catch (GRBException e) {
        results->error_message
            = "Gurobi Error: Caught gurobi exception while creating constraints " + e.getMessage();
        post_solve();
        return results;
    }

    try {
        set_gurobi_options();
        gmodel->optimize();
    }
    catch (GRBException e) {
        results->error_message
            = "Gurobi Error: Caught gurobi exception while optimizing " + e.getMessage();
        post_solve();
        return results;
    }

    collect_results(model, results);

    post_solve();
    return results;
}
#endif

std::shared_ptr<SolverResults> GurobiSolver::resolve()
{
    pre_solve();

    auto _model = model.repn.get();
    results->model_name = _model->name;

    if (initial_solve()) {
        env = new GRBEnv(true);
        auto it = integer_options().find("OutputFlag");
        if (it != integer_options().end())
            env->set(GRB_IntParam_OutputFlag, it->second);
        env->start();
        gmodel = new GRBModel(*env);

        // Add Gurobi variables
        for (auto& var : _model->variables) {
            std::shared_ptr<coek::VariableTerm> v = var.repn;
            if (not v->fixed) {
                double lb = v->lb->eval();
                double ub = v->ub->eval();
                if (v->binary)
                    x[v->index] = gmodel->addVar(lb, ub, 0, GRB_BINARY);
                else if (v->integer)
                    x[v->index] = gmodel->addVar(lb, ub, 0, GRB_INTEGER);
                else {
                    if (ub >= 1e19) {
                        if (lb <= -1e19)
                            x[v->index]
                                = gmodel->addVar(-GRB_INFINITY, GRB_INFINITY, 0, GRB_CONTINUOUS);
                        else
                            x[v->index] = gmodel->addVar(lb, GRB_INFINITY, 0, GRB_CONTINUOUS);
                    }
                    else {
                        if (lb <= -1e19)
                            x[v->index] = gmodel->addVar(-GRB_INFINITY, ub, 0, GRB_CONTINUOUS);
                        else
                            x[v->index] = gmodel->addVar(lb, ub, 0, GRB_CONTINUOUS);
                    }
                }
            }
        }

        gmodel->update();

        // Add Gurobi objective
        int nobj = 0;
        try {
            coek::QuadraticExpr orepn;
            for (auto& obj : model.repn->objectives) {
                if (obj.active()) {
                    Expression tmp = obj.expr();
                    add_gurobi_objective(gmodel, tmp, obj.sense(), x, orepn);
                    nobj++;
                }
            }
        }
        catch (GRBException e) {
            results->error_message
                = "Gurobi Error: Caught gurobi exception while creating objectives "
                  + e.getMessage();
            return results;
        }
        if (nobj > 1) {
            // TODO - is this an error?
            results->termination_condition = TerminationCondition::invalid_model_for_solver;
            results->error_message = "Error initializing Gurobi: More than one objective defined!";
            return results;
        }

        // Add Gurobi constraints
        try {
            coek::QuadraticExpr repn;
            for (auto& con : model.repn->constraints) {
                if (con.active())
                    add_gurobi_constraint(gmodel, con, x, repn);
            }
        }
        catch (GRBException e) {
            results->error_message
                = "Gurobi Error: Caught gurobi exception while creating constraints "
                  + e.getMessage();
            return results;
        }

        set_gurobi_options();
        try {
            gmodel->optimize();
        }
        catch (GRBException e) {
            results->error_message
                = "Gurobi Error: Caught gurobi exception while optimizing " + e.getMessage();
            return results;
        }
    }

    else {
        for (auto& it : updated_coefs) {
            auto [i, where, j] = it;

            switch (where) {
                case 0:  // Constant Value
                    if (i > 0) {
                        int prev = static_cast<int>(i) - 1;
                        gmodel->getConstr(prev).set(GRB_DoubleAttr_RHS, -repn[i].constval->eval());
                    }
                    else
                        gmodel->set(GRB_DoubleAttr_ObjCon, repn[0].constval->eval());
                    break;

                case 1:  // Linear Coef
                    if (i > 0) {
                        int prev = static_cast<int>(i) - 1;
                        gmodel->chgCoeff(gmodel->getConstr(prev), x[repn[i].linear_vars[j]->index],
                                         repn[i].linear_coefs[j]->eval());
                    }
                    else
                        x[repn[0].linear_vars[j]->index].set(GRB_DoubleAttr_Obj,
                                                             repn[0].linear_coefs[j]->eval());
                    break;

                case 2:  // Quadratic Coef
                    break;

                case 3:  // Nonlinear terms
                    results->termination_condition = TerminationCondition::invalid_model_for_solver;
                    results->error_message
                        = "Error initializing Gurobi: Cannot optimize models with nonlinear terms.";
                    return results;
                    break;
            };
        }

        try {
            gmodel->optimize();
        }
        catch (GRBException e) {
            results->error_message
                = "Gurobi Error: Caught gurobi exception while optimizing " + e.getMessage();
            return results;
        }
    }

    // Collect values of Gurobi variables
    collect_results(model, results);

    results->toc();
    return results;
}

void GurobiSolver::collect_results(Model& model, std::shared_ptr<SolverResults>& results)
{
    try {
        int status = gmodel->get(GRB_IntAttr_Status);
        if (status == GRB_OPTIMAL) {
            results->termination_condition = TerminationCondition::convergence_criteria_satisfied;
            results->solution_status = SolutionStatus::optimal;
            results->objective_value = gmodel->getObjective().getValue();
            try {
                double value = gmodel->get(GRB_DoubleAttr_ObjBound);
                results->objective_bound = value;
            }
            catch (GRBException) {
            }
            if (not results->objective_bound.has_value()) {
                try {
                    double value = gmodel->get(GRB_DoubleAttr_ObjBoundC);
                    results->objective_bound = value;
                }
                catch (GRBException) {
                }
            }

            // Collect values of Gurobi variables
            for (auto& var : model.repn->variables) {
                std::shared_ptr<coek::VariableTerm>& v = var.repn;
                if (not v->fixed) {
                    v->set_value(x[v->index].get(GRB_DoubleAttr_X));
                }
            }
        }
        else if (status == GRB_SUBOPTIMAL) {
            results->termination_condition = TerminationCondition::other_termination_limit;
            results->solution_status = SolutionStatus::feasible;
            results->objective_value = gmodel->getObjective().getValue();
            try {
                double value = gmodel->get(GRB_DoubleAttr_ObjBound);
                results->objective_bound = value;
            }
            catch (GRBException) {
            }
            if (not results->objective_bound.has_value()) {
                try {
                    double value = gmodel->get(GRB_DoubleAttr_ObjBoundC);
                    results->objective_bound = value;
                }
                catch (GRBException) {
                }
            }
            results->error_message
                = "Unable to satisfy optimality tolerances; a sub-optimal solution is available";

            // Collect values of Gurobi variables
            for (auto& var : model.repn->variables) {
                std::shared_ptr<coek::VariableTerm>& v = var.repn;
                if (not v->fixed) {
                    v->set_value(x[v->index].get(GRB_DoubleAttr_X));
                }
            }
        }
        else if (status == GRB_INFEASIBLE) {
            results->termination_condition = TerminationCondition::proven_infeasible;
            results->solution_status = SolutionStatus::infeasible;
        }
        else if (status == GRB_UNBOUNDED) {
            results->termination_condition = TerminationCondition::unbounded;
        }
        else if (status == GRB_ITERATION_LIMIT) {
            results->termination_condition = TerminationCondition::iteration_limit;
        }
        else if (status == GRB_TIME_LIMIT) {
            results->termination_condition = TerminationCondition::time_limit;
        }
        else if (status == GRB_INTERRUPTED) {
            results->termination_condition = TerminationCondition::interrupted;
        }
        else if (status == GRB_USER_OBJ_LIMIT) {
            results->termination_condition = TerminationCondition::objective_limit;
        }
        else if (status == GRB_WORK_LIMIT) {
            results->termination_condition = TerminationCondition::other_termination_limit;
            results->error_message
                = "Gurobi terminated because the work expended exceeded the value specified in the "
                  "WorkLimit parameter.";
        }
        else if (status == GRB_MEM_LIMIT) {
            results->termination_condition = TerminationCondition::other_termination_limit;
            results->error_message
                = "Gurobi terminated because the total amount of allocated memory exceeded the "
                  "value specified in the SoftMemLimit parameter.";
        }
        else if (status == GRB_NODE_LIMIT) {
            results->termination_condition = TerminationCondition::other_termination_limit;
            results->error_message
                = "Gurobi terminated because the total number of branch-and-cut nodes explored "
                  "exceeded the value specified in the NodeLimit parameter.";
        }
        else if (status == GRB_SOLUTION_LIMIT) {
            results->termination_condition = TerminationCondition::other_termination_limit;
            results->error_message
                = "Gurobi terminated because the number of solutions found reached the value "
                  "specified in the SolutionLimit parameter.";
        }
        else if (status == GRB_CUTOFF) {
            results->termination_condition = TerminationCondition::other_termination_limit;
            results->error_message
                = "Gurobi terminated because optimal objective for model was proven to be worse "
                  "than the value specified in the Cutoff parameter.";
        }
        else if (status == GRB_NUMERIC) {
            results->termination_condition = TerminationCondition::unknown;
            results->error_message
                = "Gurobi Error: Optimization was terminated due to unrecoverable numerical "
                  "difficulties.";
        }
    }
    catch (GRBException e) {
        results->termination_condition = TerminationCondition::unknown;
        results->error_message = "GUROBI Exception: (results) " + e.getMessage();
    }
}

}  // namespace coek
