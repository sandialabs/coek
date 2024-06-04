#pragma once

#include <memory>
#include "Highs.h"
#include "coek/solvers/solver_repn.hpp"

namespace coek {

class HighsSolver : public SolverRepn {
   public:
    Highs highs;
    HighsModel hmodel;
    HighsStatus return_status;
    std::shared_ptr<SolverResults> results;
    std::unordered_map<size_t, size_t> x;

   public:
    HighsSolver() : SolverRepn() {}

    std::shared_ptr<SolverResults> resolve();
    std::shared_ptr<SolverResults> solve(Model&);
#ifdef COEK_WITH_COMPACT_MODEL
    std::shared_ptr<SolverResults> solve(CompactModel&);
#endif

   public:
    void set_solver_options();

   protected:
    void collect_results(Model& model, std::shared_ptr<SolverResults>& results);
    void pre_solve(Model& coek_model);
    void post_solve();
};

}  // namespace coek
