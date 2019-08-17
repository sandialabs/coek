#pragma once

#include "coek_model.hpp"
#include "solver.hpp"


namespace coek {

class IpoptSolver : public NLPSolverRepn
{
public:

    IpoptSolver() : NLPSolverRepn() {}

    void load(NLPModel& model);

    int resolve();

    int solve(NLPModel& model);

};

}
