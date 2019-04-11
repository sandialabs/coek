#include <string>
#include "solver.hpp"

#ifdef WITH_IPOPT
#include "solvers/ipopt.hpp"
#endif

//int i;

namespace coek {

Solver* create_solver(const char* _name)
{
std::string name = _name;

#ifdef WITH_IPOPT
if (name == "ipopt")
    return new IpoptSolver();
#endif

return 0;
}

}

