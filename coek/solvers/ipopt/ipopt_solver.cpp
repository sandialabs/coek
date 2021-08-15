//#include <memory>
#include <chrono>
#include "coek/api/expression.hpp"
#include "coek/api/objective.hpp"
#include "coek/api/constraint.hpp"
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
return nlp->perform_solve();
}


int IpoptSolver::resolve()
{
auto start = std::chrono::high_resolution_clock::now();

if (not initial_solve())
    model->reset();

auto it = string_options.find("warm_start_init_point");
if ((it != string_options.end()) and (it->second == "yes"))
    nlp->set_start_from_last_x(true);
else    
    nlp->set_start_from_last_x(false);
int status = nlp->perform_solve();

auto curr = std::chrono::high_resolution_clock::now();
std::chrono::duration<double> diff = curr-start;
std::cout << "Time to solve: " << diff.count() << " s\n";

return status;
}

}
