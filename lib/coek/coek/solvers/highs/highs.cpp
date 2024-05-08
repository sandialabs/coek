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
#include "coek/util/io_utils.hpp"
#include "coek/util/sequence.hpp"

#include "coek_highs.hpp"

namespace coek {

SolverRepn* create_highs_solver() { return new HighsSolver(); }

namespace {

bool add_variable(HighsModel& model, std::unordered_map<size_t, size_t>& x, double lower,
                  double upper, const Variable& var)
{
    x[var.id()] = model.lp_.col_lower_.size();

    if (var.is_binary() or var.is_integer()) {
        model.lp_.col_lower_.push_back(lower);
        model.lp_.col_upper_.push_back(upper);
        model.lp_.integrality_.push_back(HighsVarType::kInteger);
        return false;
    }

    else {
        if (upper >= 1e19)
            upper = 1.0e30;
        if (lower >= 1e19)
            lower = -1.0e30;
        model.lp_.col_lower_.push_back(lower);
        model.lp_.col_upper_.push_back(upper);
        model.lp_.integrality_.push_back(HighsVarType::kContinuous);
        return true;
    }
}

void add_objective(HighsModel& model, Expression& expr, bool sense,
                   std::unordered_map<size_t, size_t>& x, coek::QuadraticExpr& orepn)
{
    orepn.reset();
    orepn.collect_terms(expr);
    model.lp_.col_cost_.resize(x.size());

    model.lp_.offset_ = orepn.constval;

    if (orepn.linear_coefs.size()) {
        auto it = orepn.linear_coefs.begin();
        for (auto& var : orepn.linear_vars) {
            model.lp_.col_cost_[x[var->index]] = *it;
            ++it;
        }
    }

    if (orepn.quadratic_coefs.size() > 0) {
        std::map<std::tuple<size_t, size_t>, double> value;
        bool nz = false;

        for (size_t i : indices(orepn.quadratic_coefs)) {
            auto& lvar = orepn.quadratic_lvars[i];
            auto& rvar = orepn.quadratic_rvars[i];
            nz = true;
            if (x[rvar->index] >= x[lvar->index])
                value[{x[rvar->index], x[lvar->index]}] += orepn.quadratic_coefs[i];
            else
                value[{x[lvar->index], x[rvar->index]}] += orepn.quadratic_coefs[i];
        }

        if (nz) {
            HighsHessian& hessian = model.hessian_;
            size_t prev = 0;
            for (auto& it : value) {
                auto [i, j] = it.first;
                if (j != prev) {
                    hessian.start_.push_back(static_cast<HighsInt>(hessian.index_.size()));
                    prev = j;
                }
                hessian.index_.push_back(static_cast<HighsInt>(i));
                if (i == j)
                    hessian.value_.push_back(2 * it.second);
                else
                    hessian.value_.push_back(it.second);
            }
            // hessian.dim_ = hessian.index_.size();
            hessian.dim_ = static_cast<HighsInt>(model.lp_.col_lower_.size());
            hessian.start_.push_back(static_cast<HighsInt>(hessian.index_.size()));
            hessian.format_ = HessianFormat::kTriangular;
        }
    }

    if (sense)
        model.lp_.sense_ = ObjSense::kMinimize;
    else
        model.lp_.sense_ = ObjSense::kMaximize;
}

void add_constraint(HighsModel& model, Constraint& con, std::unordered_map<size_t, size_t>& x,
                    coek::QuadraticExpr& repn)
{
    repn.reset();
    repn.collect_terms(con);

    if (repn.quadratic_coefs.size() > 0)
        throw std::runtime_error("Highs cannot handle nonlinear expressions in constraints");

    if (repn.linear_coefs.size() == 0)
        return;

    if (model.lp_.a_matrix_.start_.size() == 1) {
        model.lp_.a_matrix_.start_.push_back(static_cast<HighsInt>(repn.linear_coefs.size()));
    }
    else {
        HighsInt tmp = model.lp_.a_matrix_.start_.back();
        tmp += static_cast<HighsInt>(repn.linear_coefs.size());
        model.lp_.a_matrix_.start_.push_back(tmp);
    }
    //  Row coefs
    auto it = repn.linear_coefs.begin();
    for (auto& var : repn.linear_vars) {
        model.lp_.a_matrix_.index_.push_back(static_cast<HighsInt>(x[var->index]));
        model.lp_.a_matrix_.value_.push_back(*it);
        ++it;
    }

    // Row lower/upper
    double lower = -1.0e30;
    double upper = 1.0e30;
    if (con.is_inequality()) {
        if (con.lower().repn) {
            double tmp = con.lower().value();
            if (tmp > -COEK_INFINITY)
                lower = -repn.constval + tmp;
        }
        if (con.upper().repn) {
            double tmp = con.upper().value();
            if (tmp < COEK_INFINITY)
                upper = -repn.constval + tmp;
        }
    }
    else
        lower = upper = -repn.constval + con.lower().value();
    model.lp_.row_lower_.push_back(lower);
    model.lp_.row_upper_.push_back(upper);
}

}  // namespace

void HighsSolver::set_solver_options()
{
    for (auto& it : string_options())
        highs.setOptionValue(it.first, it.second);
    for (auto& it : boolean_options())
        highs.setOptionValue(it.first, it.second);
    for (auto& it : integer_options())
        highs.setOptionValue(it.first, it.second);
    for (auto& it : double_options())
        highs.setOptionValue(it.first, it.second);
}

void HighsSolver::pre_solve()
{
    results = std::make_shared<SolverResults>();
    results->solver_name = "highs";
    results->termination_condition = TerminationCondition::error;
    results->tic();
}

void HighsSolver::post_solve() { results->toc(); }

std::shared_ptr<SolverResults> HighsSolver::solve(Model& coek_model)
{
    pre_solve();
    auto _coek_model = coek_model.repn.get();

    hmodel.clear();

    // Add variables
    bool continuous = true;
    for (auto& var : _coek_model->variables) {
        if (not var.fixed())
            continuous = continuous and add_variable(hmodel, x, var.lower(), var.upper(), var);
    }
    if (continuous)
        hmodel.lp_.integrality_.resize(0);

    // Add objective
    unsigned int nobj = 0;
    try {
        coek::QuadraticExpr orepn;
        for (auto& obj : _coek_model->objectives) {
            Expression tmp = obj.expr();
            add_objective(hmodel, tmp, obj.sense(), x, orepn);
            nobj++;
        }
    }
    catch (const std::exception& e) {
        results->error_message = "Highs Error: Caught highs exception while creating objectives "
                                 + std::string(e.what());
        post_solve();
        return results;
    }
    if (nobj > 1) {
        // TODO - is this an error?
        results->termination_condition = TerminationCondition::invalid_model_for_solver;
        results->error_message = "Error initializing Highs: More than one objective defined!";
        results->toc();
        return results;
    }

    // Add constraints
    hmodel.lp_.a_matrix_.format_ = MatrixFormat::kRowwise;
    try {
        coek::QuadraticExpr repn;
        for (auto& con : _coek_model->constraints) {
            add_constraint(hmodel, con, x, repn);
        }
    }
    catch (const std::exception& e) {
        results->error_message
            = "Highs Error: Caught exception while creating constraints " + std::string(e.what());
        results->toc();
        return results;
    }

    hmodel.lp_.num_col_ = static_cast<HighsInt>(hmodel.lp_.col_cost_.size());
    hmodel.lp_.num_row_ = static_cast<HighsInt>(hmodel.lp_.row_lower_.size());

#if 0
    std::cout << "Ncol: " << hmodel.lp_.num_col_ << std::endl;
    std::cout << "Nrow: " << hmodel.lp_.num_row_ << std::endl;
    std::cout << "ColCost: " << hmodel.lp_.col_cost_ << std::endl;
    std::cout << "Offset: " << hmodel.lp_.offset_ << std::endl;
    std::cout << "Integrality: ";
    for (auto& val : hmodel.lp_.integrality_)
        std::cout << (int)val << " ";
    std::cout << std::endl;
    std::cout << "Lower: " << hmodel.lp_.row_lower_ << std::endl;
    std::cout << "Upper: " << hmodel.lp_.row_upper_ << std::endl;
    std::cout << "Start: " << hmodel.lp_.a_matrix_.start_ << std::endl;
    std::cout << "Index: " << hmodel.lp_.a_matrix_.index_ << std::endl;
    std::cout << "Value: " << hmodel.lp_.a_matrix_.value_ << std::endl;
    std::cout << "Hessian Dim:   " << hmodel.hessian_.dim_ << std::endl;
    std::cout << "Hessian Start: " << hmodel.hessian_.start_ << std::endl;
    std::cout << "Hessian Row:   " << hmodel.hessian_.index_ << std::endl;
    std::cout << "Hessian Value: " << hmodel.hessian_.value_ << std::endl;
#endif

    set_solver_options();
    return_status = highs.passModel(hmodel);
    if (return_status != HighsStatus::kOk) {
        results->error_message = "Highs Error: Error initializing model";
        results->toc();
        return results;
    }

    try {
        return_status = highs.run();
    }
    catch (const std::exception& e) {
        results->error_message
            = "Highs Error: Caught highs exception while optimizing " + std::string(e.what());
        results->toc();
        return results;
    }

    collect_results(coek_model, results);

    post_solve();
    return results;
}

#ifdef COEK_WITH_COMPACT_MODEL
std::shared_ptr<SolverResults> HighsSolver::solve(CompactModel& compact_model)
{
    auto results = std::make_shared<SolverResults>();
    results->solver_name = "highs";
    results->termination_condition = TerminationCondition::error;
    results->tic();

    // Add variables
    bool continuous = true;
    for (auto& val : compact_model.repn->variables) {
        if (auto eval = std::get_if<Variable>(&val)) {
            Expression lb = eval->lower_expression().expand();
            auto lb_ = lb.value();
            Expression ub = eval->upper_expression().expand();
            auto ub_ = ub.value();
            continuous = continuous and add_variable(hmodel, x, lb_, ub_, *eval);
            model.add(*eval);
        }
        else {
            auto& seq = std::get<VariableSequence>(val);
            for (auto jt = seq.begin(); jt != seq.end(); ++jt) {
                continuous = continuous and add_variable(hmodel, x, jt->lower(), jt->upper(), *jt);
                model.add(*jt);
            }
        }
    }
    if (continuous)
        hmodel.lp_.integrality_.resize(0);

    // Add objective
    unsigned int nobj = 0;
    try {
        coek::QuadraticExpr orepn;
        for (auto& val : compact_model.repn->objectives) {
            if (auto eval = std::get_if<Objective>(&val)) {
                Expression tmp = eval->expr().expand();
                add_objective(hmodel, tmp, eval->sense(), x, orepn);
                nobj++;
            }
            else {
                auto& seq = std::get<ObjectiveSequence>(val);
                for (auto jt = seq.begin(); jt != seq.end(); ++jt) {
                    // model.repn->objectives.push_back(*jt);
                    Expression tmp = jt->expr();
                    add_objective(hmodel, tmp, jt->sense(), x, orepn);
                    nobj++;
                }
            }
        }
    }
    catch (const std::exception& e) {
        results->error_message = "Highs Error: Caught highs exception while creating objectives "
                                 + std::string(e.what());
        results->toc();
        return results;
    }
    if (nobj > 1) {
        // TODO - is this an error?
        results->termination_condition = TerminationCondition::invalid_model_for_solver;
        results->error_message = "Error initializing Highs: More than one objective defined!";
        results->toc();
        return results;
    }

    // Add constraints
    hmodel.lp_.a_matrix_.format_ = MatrixFormat::kRowwise;
    try {
        coek::QuadraticExpr repn;
        for (auto& val : compact_model.repn->constraints) {
            if (auto cval = std::get_if<Constraint>(&val)) {
                Constraint c = cval->expand();
                add_constraint(hmodel, c, x, repn);
            }
            else {
                auto& seq = std::get<ConstraintSequence>(val);
                for (auto jt = seq.begin(); jt != seq.end(); ++jt) {
                    add_constraint(hmodel, *jt, x, repn);
                }
            }
        }
    }
    catch (const std::exception& e) {
        results->error_message
            = "Highs Error: Caught exception while creating constraints " + std::string(e.what());
        results->toc();
        return results;
    }

    hmodel.lp_.num_col_ = static_cast<HighsInt>(hmodel.lp_.col_cost_.size());
    hmodel.lp_.num_row_ = static_cast<HighsInt>(hmodel.lp_.row_lower_.size());

    set_solver_options();
    return_status = highs.passModel(hmodel);
    if (return_status != HighsStatus::kOk) {
        results->error_message = "Highs Error: Error initializing model";
        results->toc();
        return results;
    }

    try {
        return_status = highs.run();
    }
    catch (const std::exception& e) {
        results->error_message
            = "Highs Error: Caught highs exception while optimizing " + std::string(e.what());
        results->toc();
        return results;
    }

    collect_results(model, results);

    post_solve();
    return results;
}
#endif

std::shared_ptr<SolverResults> HighsSolver::resolve()
{
    auto results = std::make_shared<SolverResults>();
    results->solver_name = "highs";
    results->termination_condition = TerminationCondition::error;
    results->tic();

#if 0
    auto _model = model.repn.get();

    if (initial_solve()) {
        env = new GRBEnv(true);
        auto it = integer_options().find("OutputFlag");
        if (it != integer_options().end())
            env->set(GRB_IntParam_OutputFlag, it->second);
        env->start();
        gmodel = new GRBModel(*env);

        // Add variables
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

        // Add objective
        int nobj = 0;
        try {
            coek::QuadraticExpr orepn;
            for (auto& obj : model.repn->objectives) {
                Expression tmp = obj.expr();
                add_objective(gmodel, tmp, obj.sense(), x, orepn);
                nobj++;
            }
        }
        catch (const std::exception& e) {
            results->error_message
                = "Highs Error: Caught highs exception while creating objectives "
                  + std::string(e.what());
            results->toc();
            return results;
        }
        if (nobj > 1) {
            // TODO - is this an error?
            results->termination_condition = TerminationCondition::invalid_model_for_solver;
            results->error_message = "Error initializing Highs: More than one objective defined!";
            results->toc();
            return results;
        }

        // Add constraints
        try {
            coek::QuadraticExpr repn;
            for (auto& con : model.repn->constraints) {
                add_constraint(gmodel, con, x, repn);
            }
        }
        catch (const std::exception& e) {
            results->error_message
                = "Highs Error: Caught highs exception while creating constraints "
                  + std::string(e.what());
            results->toc();
            return results;
        }

        set_solver_options();
        try {
            gmodel->optimize();
        }
        catch (const std::exception& e) {
            results->error_message
                = "Highs Error: Caught highs exception while optimizing " + std::string(e.what());
            results->toc();
            return results;
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
                    results->termination_condition = TerminationCondition::invalid_model_for_solver;
                    results->error_message
                        = "Error initializing Highs: Cannot optimize models with nonlinear terms.";
                    results->toc();
                    return results;
                    break;
            };
        }

        try {
            gmodel->optimize();
        }
        catch (const std::exception& e) {
            results->error_message
                = "Highs Error: Caught highs exception while optimizing " + std::string(e.what());
            results->toc();
            return results;
        }
    }

    // Collect values of solver variables
    collect_results(model, results);

#endif
    post_solve();
    return results;
}

void HighsSolver::collect_results(Model& model, std::shared_ptr<SolverResults>& results)
{
    try {
        const auto& model_status = highs.getModelStatus();

        if (model_status == HighsModelStatus::kOptimal) {
            results->termination_condition = TerminationCondition::convergence_criteria_satisfied;
            results->solution_status = SolutionStatus::optimal;
            const auto& solution = highs.getSolution();
            const auto& info = highs.getInfo();

            // Objective
            results->objective_value = info.objective_function_value;
            if (highs.getModel().lp_.isMip())
                results->objective_value = info.mip_dual_bound;
            else
                results->objective_bound = results->objective_value;
            // highs.getOptionValue("objective_bound", results->objective_value);

            // Collect values of variables
            const bool has_primal = info.primal_solution_status;
            if (has_primal) {
                for (auto& var : model.repn->variables) {
                    if (not var.fixed()) {
                        var.value(solution.col_value[x[var.id()]]);
                    }
                }

                // TODO: collect duals and basis if the user requests this information
            }
        }
        else if (model_status == HighsModelStatus::kNotset) {
            results->termination_condition = TerminationCondition::empty_model;
        }
        else if (model_status == HighsModelStatus::kLoadError) {
            results->termination_condition = TerminationCondition::invalid_model_for_solver;
            results->error_message = "Highs terminated with an error loading the model.";
        }
        else if (model_status == HighsModelStatus::kModelError) {
            results->termination_condition = TerminationCondition::error;
            results->error_message = "Highs terminated with a model error.";
        }
        else if (model_status == HighsModelStatus::kPresolveError) {
            results->termination_condition = TerminationCondition::error;
            results->error_message = "Highs terminated with a pre-solve error.";
        }
        else if (model_status == HighsModelStatus::kSolveError) {
            results->termination_condition = TerminationCondition::error;
            results->error_message = "Highs terminated with a solve error.";
        }
        else if (model_status == HighsModelStatus::kPostsolveError) {
            results->termination_condition = TerminationCondition::error;
            results->error_message = "Highs terminated with a post-solve error.";
        }
        else if (model_status == HighsModelStatus::kModelEmpty) {
            results->termination_condition = TerminationCondition::empty_model;
        }
        else if (model_status == HighsModelStatus::kInfeasible) {
            results->termination_condition = TerminationCondition::proven_infeasible;
            results->solution_status = SolutionStatus::infeasible;
        }
        else if (model_status == HighsModelStatus::kUnboundedOrInfeasible) {
            results->termination_condition = TerminationCondition::infeasible_or_unbounded;
        }
        else if (model_status == HighsModelStatus::kUnbounded) {
            results->termination_condition = TerminationCondition::unbounded;
        }
        else if (model_status == HighsModelStatus::kObjectiveBound) {
            results->termination_condition = TerminationCondition::objective_limit;
            results->error_message
                = "Highs terminated because optimal objective for model was proven to be worse "
                  "than the value specified in the objective_bound option.";
        }
        else if (model_status == HighsModelStatus::kObjectiveTarget) {
            results->termination_condition = TerminationCondition::other_termination_limit;
            results->error_message
                = "Highs terminated because optimal objective for model was proven to be worse "
                  "than the value specified in the objective_target option.";
        }
        else if (model_status == HighsModelStatus::kTimeLimit) {
            results->termination_condition = TerminationCondition::time_limit;
        }
        else if (model_status == HighsModelStatus::kIterationLimit) {
            results->termination_condition = TerminationCondition::iteration_limit;
        }
        else if (model_status == HighsModelStatus::kSolutionLimit) {
            results->termination_condition = TerminationCondition::other_termination_limit;
            results->error_message
                = "Highs terminated because the number of solutions found reached the value "
                  "specified in the SolutionLimit parameter.";
        }
        else if (model_status == HighsModelStatus::kInterrupt) {
            results->termination_condition = TerminationCondition::interrupted;
        }
        else if (model_status == HighsModelStatus::kUnknown) {
            results->termination_condition = TerminationCondition::unknown;
            results->error_message
                = "Highs Error: Optimization was terminated due to unknown difficulties.";
        }
    }
    catch (const std::exception& e) {
        results->termination_condition = TerminationCondition::unknown;
        results->error_message = "HIGHS Exception: (results) " + std::string(e.what());
    }
}

}  // namespace coek
