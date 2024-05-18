// #include <memory>
#include <chrono>

#include "coek/api/expression.hpp"
#include "coek/api/constraint.hpp"
#include "coek/api/objective.hpp"
#include "coek/autograd/autograd.hpp"
#include "ipopt_solver.hpp"

namespace coek {

NLPSolverRepn* create_ipopt_solver() { return new IpoptSolver(); }

std::shared_ptr<SolverResults> IpoptSolver::solve(NLPModel& _model)
{
    if (not available_) {
        auto res = std::make_shared<SolverResults>();
        res->model_name = _model.name();
        res->solver_name = "ipopt";
        res->termination_condition = TerminationCondition::solver_not_available;
        return res;
    }

    if (_model.num_variables() == 0) {
        auto res = std::make_shared<SolverResults>();
        res->model_name = _model.name();
        res->solver_name = "ipopt";
        res->termination_condition = TerminationCondition::empty_model;
        return res;
    }

    load(_model);
    if (not initial_solve()) {
        auto res = std::make_shared<SolverResults>();
        res->model_name = _model.name();
        res->solver_name = "ipopt";
        res->termination_condition = TerminationCondition::error;
        res->error_message
            = "Ipopt Error: must call reset() to reset ipopt state before calling solve() a second "
              "time.";
        return res;
    }
    repn->set_options(string_options(), boolean_options(), integer_options(), double_options());
    return repn->perform_solve();
}

std::shared_ptr<SolverResults> IpoptSolver::resolve_exec()
{
    if (not available_) {
        auto res = std::make_shared<SolverResults>();
        res->model_name = model->name();
        res->solver_name = "ipopt";
        res->termination_condition = TerminationCondition::solver_not_available;
        res->error_message = error_message;
        return res;
    }
    if (model->num_variables() == 0) {
        auto res = std::make_shared<SolverResults>();
        res->model_name = model->name();
        res->solver_name = "ipopt";
        res->termination_condition = TerminationCondition::empty_model;
        return res;
    }

    if (not initial_solve())
        model->reset();

    repn->set_options(string_options(), boolean_options(), integer_options(), double_options());
    auto it = string_options().find("warm_start_init_point");
    if ((it != string_options().end()) and (it->second == "yes"))
        repn->set_start_from_last_x(true);
    else
        repn->set_start_from_last_x(false);

    return repn->perform_solve();
}

}  // namespace coek
