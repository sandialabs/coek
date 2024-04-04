#pragma once

#include <cmath>
#include <string>
#include <map>
#include <vector>
#include <variant>
#include <tuple>
#include <memory>
#include <optional>
#include <coek/util/tictoc.hpp>

namespace coek {

//
// A description of the termination state of a solver.
//
enum class TerminationCondition {
    // The solver exited because convergence criteria of the problem were satisfied.
    convergence_criteria_satisfied,

    // The solver exited due to reaching a specified time limit.
    time_limit,

    // The solver exited due to reaching a specified iteration limit.
    iteration_limit,

    // The solver exited due to reaching an objective limit. For example, in Gurobi, the exit
    // message "Optimal objective for model was proven to be worse than the value specified in the
    // Cutoff parameter" would map to objectiveLimit.
    objective_limit,

    // The solver exited due to a minimum step length.  Minimum step length reached may mean that
    // the problem is infeasible or that the problem is feasible but the solver could not converge.
    min_step_length,

    // The solver exited due to some other termination limit that is not captured here.
    other_termination_limit,

    // The solver exited because the problem has been found to be unbounded.
    unbounded,

    // The solver exited because the problem has been proven infeasible.
    proven_infeasible,

    // The solver exited because no feasible solution was found to the submitted problem, but it
    // could not be proven that no such solution exists.
    locally_infeasible,

    // Some solvers do not specify between infeasibility or unboundedness and instead return that
    // one or the other has occurred. For example, in Gurobi, this may occur because there are some
    // steps in presolve that prevent Gurobi from distinguishing between infeasibility and
    // unboundedness.
    infeasible_or_unbounded,

    // The solver exited with some error. The error message will also be captured and returned.
    error,

    // The solver was interrupted while running.
    interrupted,

    // The solver experienced issues with licensing. This could be that no license was found, the
    // license is of the wrong type for the problem (e.g., problem is too big for type of license),
    // or there was an issue contacting a licensing server.
    licensing_problems,

    // The solver cannot be found.  This could be a problem the installation of the solver, the
    // solver link library may not be found, or the solver executable may not be found.
    solver_not_available,

    // The model being solved did not have any unfixed variables.
    empty_model,

    // The model being solved was not suitable for the solver.
    invalid_model_for_solver,

    // All other unrecognized exit statuses fall in this category.
    unknown
};

//
// A description of a solution for a model.
//
enum class SolutionStatus {
    // Unknown error occurred.
    unknown,

    // No solution is available.
    no_solution,

    // A solution that does not satisfy all the constraints in the model.
    infeasible,

    // A solution that satisfies all the constraints in the model.
    feasible,

    // A feasible solution where the objective function is a maximizer or minimizer.
    optimal

};

class Solution {
   public:
    // A description of the solution state.
    SolutionStatus solution_status = SolutionStatus::no_solution;

    // The value of the objective associted with the solution.
    double objective_value = nan("");
};

class SolverResultsBase {
   protected:
    TicTocTimer timer;

   public:
    // The name of the solver
    std::string solver_name;

    // A map of solver options that were set
    std::map<std::string, std::variant<std::string, int, double>> solver_options;

    // The solver version
    std::tuple<unsigned int, unsigned int, unsigned int> solver_version = {0, 0, 0};

    // The termination state of a solver.
    TerminationCondition termination_condition = TerminationCondition::unknown;

    // A bound on the objective computed during optimization
    std::optional<double> objective_bound;

    // The number of solver iterations executed
    unsigned int iteration_count = 0;

    // A message describing the solver execution
    std::string error_message;

    double system_time = nan("");

    // Start timing
    void tic() { timer.tic(); }

    // Stop timing and store the system time
    void toc() { system_time = timer.toc(); }
};

class SolverResults : public SolverResultsBase, public Solution {};

class MOSolverResults : public SolverResultsBase {
    SolutionStatus solution_status = SolutionStatus::no_solution;
    std::vector<double> objective_value;
};

class MultiSolSolverResults : public SolverResultsBase {
   public:
    std::vector<Solution> solutions;
};

inline bool check_optimal_termination(const SolverResults& results)
{
    return results.solution_status == SolutionStatus::optimal
           and results.termination_condition
                   == TerminationCondition::convergence_criteria_satisfied;
}

inline bool check_optimal_termination(const std::shared_ptr<SolverResults>& results)
{
    return check_optimal_termination(*results);
}

inline bool check_optimal_termination(const MultiSolSolverResults& results)
{
    bool ans = false;
    for (auto& sol : results.solutions) {
        if (sol.solution_status == SolutionStatus::optimal) {
            ans = true;
            break;
        }
    }
    return ans and results.solutions.size() == 1
           and results.termination_condition
                   == TerminationCondition::convergence_criteria_satisfied;
}

inline bool check_optimal_termination(const std::shared_ptr<MultiSolSolverResults>& results)
{
    return check_optimal_termination(*results);
}

std::string to_string(TerminationCondition tc);
std::string to_string(SolutionStatus ss);
std::string to_string(const SolverResults& res);

}  // namespace coek
