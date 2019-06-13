#include <string>
#include "solver.hpp"

#ifdef WITH_IPOPT
#include "solvers/ipopt.hpp"
#endif

#ifdef WITH_GUROBI
#include "solvers/coek_gurobi.hpp"
#endif


namespace coek {

Solver* create_solver(const char* _name)
{
std::string name = _name;

#ifdef WITH_IPOPT
if (name == "ipopt")
    return new IpoptSolver();
#endif

#ifdef WITH_GUROBI
if (name == "gurobi")
    return new GurobiSolver();
#endif

return 0;
}

}

