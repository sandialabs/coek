#pragma once

#include "solver.hpp"


namespace coek {

class TestSolver : public SolverRepn
{
public:

    TestSolver() : SolverRepn() {}

    int resolve(bool reset_initial_point);
    int solve(Model& );
    int solve(CompactModel& );
};

}
