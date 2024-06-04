#pragma once

#include "solver_repn.hpp"

namespace coek {

class TestSolver : public SolverRepn {
   public:
    TestSolver() : SolverRepn() {}

    std::shared_ptr<SolverResults> resolve();
    std::shared_ptr<SolverResults> solve(Model&);
#ifdef COEK_WITH_COMPACT_MODEL
    std::shared_ptr<SolverResults> solve(CompactModel&);
#endif
};

}  // namespace coek
