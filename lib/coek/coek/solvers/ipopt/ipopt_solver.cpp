// #include <memory>
#include <chrono>

#include "coek/api/constraint.hpp"
#include "coek/api/expression.hpp"
#include "coek/api/objective.hpp"
#include "coek/autograd/autograd.hpp"
#include "ipopt_solver.hpp"

namespace coek {

int IpoptSolver::solve(NLPModel& _model)
{
    load(_model);
    if (not initial_solve()) {
        std::cout << "ERROR: must reset the model before solving" << std::endl;
        return -1;
    }
    repn->set_options(string_options(), integer_options(), double_options());
    return repn->perform_solve();
}

int IpoptSolver::resolve()
{
#ifdef DEBUG
    auto start = std::chrono::high_resolution_clock::now();
#endif

    if (not initial_solve()) model->reset();

    repn->set_options(string_options(), integer_options(), double_options());
    auto it = string_options().find("warm_start_init_point");
    if ((it != string_options().end()) and (it->second == "yes"))
        repn->set_start_from_last_x(true);
    else
        repn->set_start_from_last_x(false);
    int status = repn->perform_solve();

#ifdef DEBUG
    auto curr = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = curr - start;
    std::cout << "Time to solve: " << diff.count() << " s\n";
#endif

    return status;
}

}  // namespace coek
