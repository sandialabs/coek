
#include <string>
#include "solver.h"

Solver* create_solver(const char* _name)
{
std::string name = _name;

if (name == "ipopt")
    return new IpoptSolver();

return 0;
}


