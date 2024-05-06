#include "coek/solvers/solver_repn.hpp"

namespace coek {

SolverRepn* create_coektest_solver();
NLPSolverRepn* create_ipopt_solver();
#ifdef WITH_GUROBI
SolverRepn* create_gurobi_solver();
#endif
#ifdef WITH_HIGHS
SolverRepn* create_highs_solver();
#endif

SolverRepn* create_solver(std::string& name, OptionCache& options)
{
    if (name == "test")
        return create_coektest_solver();

#ifdef WITH_GUROBI
    if (name == "gurobi") {
        auto tmp = create_gurobi_solver();
        tmp->set_options(options);
        return tmp;
    }
#endif

#ifdef WITH_HIGHS
    if (name == "highs") {
        auto tmp = create_highs_solver();
        tmp->set_options(options);
        return tmp;
    }
#endif

    return 0;
}

NLPSolverRepn* create_nlpsolver(std::string& name, OptionCache& options)
{
    if (name == "ipopt") {
        auto tmp = create_ipopt_solver();
        tmp->set_options(options);
        return tmp;
    }

    return 0;
}

}  // namespace coek
