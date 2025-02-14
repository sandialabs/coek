#include <optional>
#include <sstream>
#include <coek/solvers/solver_results.hpp>
#include <coek/util/string_utils.hpp>

namespace coek {

std::string to_string(TerminationCondition tc)
{
    switch (tc) {
        case TerminationCondition::convergence_criteria_satisfied:
            return "convergence_criteria_satisfied";

        case TerminationCondition::time_limit:
            return "time_limit";

        case TerminationCondition::iteration_limit:
            return "iteration_limit";

        case TerminationCondition::objective_limit:
            return "objective_limit";

        case TerminationCondition::min_step_length:
            return "min_step_length";

        case TerminationCondition::other_termination_limit:
            return "other_termination_limit";

        case TerminationCondition::unbounded:
            return "unbounded";

        case TerminationCondition::proven_infeasible:
            return "proven_infeasible";

        case TerminationCondition::locally_infeasible:
            return "locally_infeasible";

        case TerminationCondition::infeasible_or_unbounded:
            return "infeasible_or_unbounded";

        case TerminationCondition::error:
            return "error";

        case TerminationCondition::interrupted:
            return "interrupted";

        case TerminationCondition::license_problems:
            return "license_problems";

        case TerminationCondition::solver_not_available:
            return "solver_not_available";

        case TerminationCondition::empty_model:
            return "empty_model";

        case TerminationCondition::invalid_model_for_solver:
            return "invalid_model_for_solver";

        default:
            return "unknown";
    }
}

std::string to_string(SolutionStatus ss)
{
    switch (ss) {
        case SolutionStatus::unknown:
            return "unknown";

        case SolutionStatus::no_solution:
            return "no_solution";

        case SolutionStatus::infeasible:
            return "infeasible";

        case SolutionStatus::feasible:
            return "feasible";

        case SolutionStatus::optimal:
            return "optimal";

        default:
            return "unknown";
    }
}

std::string to_string(const SolverResults& res, unsigned int indent)
{
    std::stringstream ans;

    ans << "model_name: " << res.model_name << std::endl;

    ans << "solver_name: " << res.solver_name << std::endl;

    // ans << "solver_options: "

    // ans << "solver_version: "

    ans << "termination_condition: " << to_string(res.termination_condition) << std::endl;

    ans << "error_message: ";
    if (res.error_message.find("\n") == std::string::npos)
        ans << res.error_message << std::endl;
    else
        ans << "\"" << res.error_message << "\"" << std::endl;

    ans << "solution_status: " << to_string(res.solution_status) << std::endl;

    ans << "objective_value: ";
    if (std::isnan(res.objective_value))
        ans << "nan" << std::endl;
    else
        ans << res.objective_value << std::endl;

    ans << "objective_bound: ";
    if (res.objective_bound.has_value())
        ans << res.objective_bound.value() << std::endl;
    else
        ans << "nan" << std::endl;

    ans << "iteration_count: " << res.iteration_count << std::endl;

    ans << "system_time: ";
    if (std::isnan(res.system_time))
        ans << "nan" << std::endl;
    else
        ans << res.system_time << std::endl;

    return indent_string(ans.str(), ' ', indent);
}

}  // namespace coek
