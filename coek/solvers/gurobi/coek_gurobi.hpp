#pragma once

#include "gurobi_c++.h"
//#include "coek/solvers/solver.hpp"
#include "coek/solvers/solver_repn.hpp"


namespace coek {

class GurobiSolver : public SolverRepn
{
public:

    GRBEnv* env;
    GRBModel* gmodel;
    std::unordered_map<int,GRBVar> x;

public:

    GurobiSolver() : SolverRepn() {}
    ~GurobiSolver();

    int resolve();
    int solve(Model& );
#ifdef COEK_WITH_COMPACT_MODEL
    int solve(CompactModel& );
#endif

public:

    bool get_option(const std::string& option, int& value) const;
    bool get_option(const std::string& option, double& value) const;
    void set_option(const std::string& option, int value);
    void set_option(const std::string& option, double value);

};

}
