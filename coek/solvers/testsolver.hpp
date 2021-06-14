#pragma once

#include "solver.hpp"


namespace coek {

class TestSolver : public SolverRepn
{
public:

    TestSolver() : SolverRepn() {}

    int resolve();
    int solve(Model& );
#ifdef COEK_WITH_COMPACT_MODEL
    int solve(CompactModel& );
#endif
};

}
