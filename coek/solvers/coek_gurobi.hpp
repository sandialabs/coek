#pragma once

#include "gurobi_c++.h"
#include "solver.hpp"


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
    int solve(CompactModel& );

public:

    bool get_option(const std::string& option, int& value) const;
    bool get_option(const std::string& option, double& value) const;
    void set_option(const std::string& option, int value);
    void set_option(const std::string& option, double value);

};

}
