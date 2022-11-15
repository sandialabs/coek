//#define _GLIBCXX_USE_CxX11_ABI 0
#include <cassert>
#include <iostream>
#include <sstream>
#include <unordered_map>

#include "../../ast/value_terms.hpp"
#include "coek/api/constraint.hpp"
#include "coek/api/expression.hpp"
#include "coek/api/objective.hpp"
#include "coek/compact/constraint_sequence.hpp"
#include "coek/compact/expression_sequence.hpp"
#include "coek/compact/objective_sequence.hpp"
#include "coek/model/model.hpp"
#include "coek/model/model_repn.hpp"
#include "coek_gurobi.hpp"
#include "gurobi_c++.h"

namespace coek {

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
                          std::unordered_map<int, GRBVar>& x, coek::QuadraticExpr& orepn)
{
    orepn.reset();
    // coek::QuadraticExpr orepn;
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

void add_gurobi_constraint(GRBModel* gmodel, Constraint& con, std::unordered_map<int, GRBVar>& x,
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
    if (gmodel) delete gmodel;
    if (env) delete env;
}

int GurobiSolver::solve(Model& model)
{
    auto _model = model.repn.get();

    env = new GRBEnv();
    gmodel = new GRBModel(*env);

    assert(_model->objectives.size() == 1);

    // Add Gurobi variables
    for (auto& var : _model->variables) {
        if (not var.fixed()) {
            x[var.id()] = add_gurobi_variable(gmodel, var.lower(), var.upper(), var);
        }
    }

    gmodel->update();

    // Add Gurobi objective
    int nobj = 0;
    try {
        coek::QuadraticExpr orepn;
        for (auto& obj : model.repn->objectives) {
            Expression tmp = obj.expr();
            add_gurobi_objective(gmodel, tmp, obj.sense(), x, orepn);
            nobj++;
        }
    }
    catch (GRBException e) {
        std::cerr << "GUROBI Exception: (objective) " << e.getMessage() << std::endl;
        throw;
    }
    if (nobj > 1) {
        //
        // TODO - is this an error?
        //
        std::cerr << "Error initializing Gurobi: More than one objective defined!" << std::endl;
        return -1;
    }

    // Add Gurobi constraints
    try {
        coek::QuadraticExpr repn;
        for (auto& con : model.repn->constraints) {
            add_gurobi_constraint(gmodel, con, x, repn);
        }
    }
    catch (GRBException e) {
        std::cerr << "GUROBI Exception: (constraint) " << e.getMessage() << std::endl;
        throw;
    }

    // All options are converted to strings for Gurobi
    for (auto it = string_options.begin(); it != string_options.end(); ++it)
        gmodel->set(it->first, it->second);

    try {
        gmodel->optimize();

        int status = gmodel->get(GRB_IntAttr_Status);
        if (status == GRB_OPTIMAL) {
            // TODO: Are there other conditions where the variables have valid values?
            // TODO: If we do not update the COEK variable values, should we set them to NAN?
            // TODO: We need to cache the optimization status in COEK somewhere
            // TODO: Is there a string description of the solver status?

            // Collect values of Gurobi variables
            for (auto& var : _model->variables) {
                std::shared_ptr<coek::VariableTerm> v = var.repn;
                if (not v->fixed) {
                    v->set_value(x[v->index].get(GRB_DoubleAttr_X));
                }
            }
        }
    }
    catch (GRBException e) {
        std::cerr << "GUROBI Exception: (solver) " << e.getMessage() << std::endl;
        // TODO: We should raise a CoekException object, to ensure that COEK can manage exceptions
        // in a uniform manner.
        throw;
    }

    delete gmodel;
    gmodel = 0;
    delete env;
    env = 0;

    return 0;
}

#ifdef COEK_WITH_COMPACT_MODEL
int GurobiSolver::solve(CompactModel& compact_model)
{
    std::cout << "STARTING GUROBI" << std::endl << std::flush;

    env = new GRBEnv();
    gmodel = new GRBModel(*env);

    std::cout << "BUILDING GUROBI MODEL" << std::endl << std::flush;

    // Add Gurobi variables
    for (auto& val : compact_model.repn->variables) {
        if (auto eval = std::get_if<Variable>(&val)) {
            Expression lb = eval->lower_expression().expand();
            auto lb_ = lb.value();
            Expression ub = eval->upper_expression().expand();
            auto ub_ = ub.value();
            x[eval->id()] = add_gurobi_variable(gmodel, lb_, ub_, *eval);
        }
        else {
            auto& seq = std::get<VariableSequence>(val);
            for (auto jt = seq.begin(); jt != seq.end(); ++jt) {
                x[jt->id()] = add_gurobi_variable(gmodel, jt->lower(), jt->upper(), *jt);
            }
        }
    }

    gmodel->update();

    // Add Gurobi objective
    int nobj = 0;
    try {
        coek::QuadraticExpr orepn;
        for (auto& val : compact_model.repn->objectives) {
            if (auto eval = std::get_if<Objective>(&val)) {
                Expression tmp = eval->expr().expand();
                add_gurobi_objective(gmodel, tmp, eval->sense(), x, orepn);
                nobj++;
            }
            else {
                auto& seq = std::get<ObjectiveSequence>(val);
                for (auto jt = seq.begin(); jt != seq.end(); ++jt) {
                    model.repn->objectives.push_back(*jt);
                    Expression tmp = jt->expr();
                    add_gurobi_objective(gmodel, tmp, jt->sense(), x, orepn);
                    nobj++;
                }
            }
        }
    }
    catch (GRBException e) {
        std::cerr << "GUROBI Exception: (objective) " << e.getMessage() << std::endl;
        throw;
    }
    if (nobj > 1) {
        //
        // TODO - is this an error?
        //
        std::cerr << "Error initializing Gurobi: More than one objective defined!" << std::endl;
        return -1;
    }

    // Add Gurobi constraints
    try {
        coek::QuadraticExpr repn;
        for (auto& val : compact_model.repn->constraints) {
            if (auto cval = std::get_if<Constraint>(&val)) {
                Constraint c = cval->expand();
                add_gurobi_constraint(gmodel, c, x, repn);
            }
            else {
                auto& seq = std::get<ConstraintSequence>(val);
                for (auto jt = seq.begin(); jt != seq.end(); ++jt) {
                    add_gurobi_constraint(gmodel, *jt, x, repn);
                }
            }
        }
    }
    catch (GRBException e) {
        std::cerr << "GUROBI Exception: (constraint) " << e.getMessage() << std::endl;
        throw;
    }

    std::cout << "OPTIMIZING GUROBI MODEL" << std::endl << std::flush;
    // All options are converted to strings for Gurobi
    for (auto it = string_options.begin(); it != string_options.end(); ++it)
        gmodel->set(it->first, it->second);
    try {
        gmodel->optimize();

        int status = gmodel->get(GRB_IntAttr_Status);
        if (status == GRB_OPTIMAL) {
            // TODO: Are there other conditions where the variables have valid values?
            // TODO: If we do not update the COEK variable values, should we set them to NAN?
            // TODO: We need to cache the optimization status in COEK somewhere
            // TODO: Is there a string description of the solver status?

#    if 0

WEH - What is the 'results object' for compact models?  This is not defined yet.

            // Collect values of Gurobi variables
            for (auto& var : model.variables) {
                var.set_value(x[var.index].get(GRB_DoubleAttr_X));
            }
#    endif
        }
    }
    catch (GRBException e) {
        std::cerr << "GUROBI Exception: (solver) " << e.getMessage() << std::endl;
        // TODO: We should raise a CoekException object, to ensure that COEK can manage exceptions
        // in a uniform manner.
        throw;
    }

    delete gmodel;
    gmodel = 0;
    delete env;
    env = 0;

    return 0;
}
#endif

int GurobiSolver::resolve()
{
    auto _model = model.repn.get();

    if (initial_solve()) {
        env = new GRBEnv();
        gmodel = new GRBModel(*env);

        assert(_model->objectives.size() == 1);

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
                Expression tmp = obj.expr();
                add_gurobi_objective(gmodel, tmp, obj.sense(), x, orepn);
                nobj++;
            }
        }
        catch (GRBException e) {
            std::cerr << "GUROBI Exception: (objective) " << e.getMessage() << std::endl;
            throw;
        }
        if (nobj > 1) {
            //
            // TODO - is this an error?
            //
            std::cerr << "Error initializing Gurobi: More than one objective defined!" << std::endl;
            return -1;
        }

        // Add Gurobi constraints
        try {
            coek::QuadraticExpr repn;
            for (auto& con : model.repn->constraints) {
                add_gurobi_constraint(gmodel, con, x, repn);
            }
        }
        catch (GRBException e) {
            std::cerr << "GUROBI Exception: (constraint) " << e.getMessage() << std::endl;
            throw;
        }

        // All options are converted to strings for Gurobi
        for (auto it = string_options.begin(); it != string_options.end(); ++it)
            gmodel->set(it->first, it->second);

        try {
            gmodel->optimize();
        }
        catch (GRBException e) {
            std::cerr << "Gurobi Exception: (solve) " << e.getMessage() << std::endl;
            throw;
        }
    }

    else {
        for (auto it = updated_coefs.begin(); it != updated_coefs.end(); ++it) {
            size_t i = std::get<0>(*it);
            size_t where = std::get<1>(*it);
            size_t j = std::get<2>(*it);

            switch (where) {
                case 0:  // Constant Value
                    if (i > 0)
                        gmodel->getConstr(i - 1).set(GRB_DoubleAttr_RHS, -repn[i].constval->eval());
                    else
                        gmodel->set(GRB_DoubleAttr_ObjCon, repn[0].constval->eval());
                    break;

                case 1:  // Linear Coef
                    if (i > 0)
                        gmodel->chgCoeff(gmodel->getConstr(i - 1), x[repn[i].linear_vars[j]->index],
                                         repn[i].linear_coefs[j]->eval());
                    else
                        x[repn[0].linear_vars[j]->index].set(GRB_DoubleAttr_Obj,
                                                             repn[0].linear_coefs[j]->eval());
                    break;

                case 2:  // Quadratic Coef
                    break;

                case 3:  // Nonlinear terms
                    throw std::runtime_error("Gurobi cannot optimize models with nonlinear terms.");
                    break;
            };
        }

        gmodel->optimize();
    }

    // Collect values of Gurobi variables
    try {
        for (auto it = _model->variables.begin(); it != _model->variables.end(); ++it) {
            std::shared_ptr<coek::VariableTerm> v = it->repn;
            if (!(v->fixed)) {
                v->set_value(x[v->index].get(GRB_DoubleAttr_X));
            }
        }
    }
    catch (GRBException e) {
        std::cerr << "Gurobi Exception: (results) " << e.getMessage() << std::endl;
        throw;
    }

    return 0;
}

bool GurobiSolver::get_option(const std::string& option, int& value) const
{
    auto curr = string_options.find(option);
    if (curr == string_options.end()) return false;
    value = atoi(curr->second.c_str());
    return true;
}

bool GurobiSolver::get_option(const std::string& option, double& value) const
{
    auto curr = string_options.find(option);
    if (curr == string_options.end()) return false;
    value = atoi(curr->second.c_str());
    return true;
}

void GurobiSolver::set_option(const std::string& option, int value)
{
    std::stringstream ostr;
    ostr << value;
    string_options[option] = ostr.str();
}

void GurobiSolver::set_option(const std::string& option, double value)
{
    std::stringstream ostr;
    ostr << value;
    string_options[option] = ostr.str();
}

}  // namespace coek
