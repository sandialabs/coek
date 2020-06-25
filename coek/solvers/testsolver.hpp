#pragma once

#include "solver.hpp"


namespace coek {

class TestSolver : public SolverRepn
{
public:

    TestSolver() : SolverRepn() {}

    int resolve();
    int solve(Model& );
    int solve(CompactModel& );
};

}
