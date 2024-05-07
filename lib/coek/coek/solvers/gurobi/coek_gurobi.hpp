#pragma once

#include "gurobi_c++.h"
#include "coek/solvers/solver_repn.hpp"

namespace coek {

class GurobiSolver : public SolverRepn {
   public:
    GRBEnv* env;
    GRBModel* gmodel;
    std::unordered_map<int, GRBVar> x;
    std::shared_ptr<SolverResults> results;

   public:
    GurobiSolver() : SolverRepn() {}
    ~GurobiSolver();

    std::shared_ptr<SolverResults> resolve();
    std::shared_ptr<SolverResults> solve(Model&);
#ifdef COEK_WITH_COMPACT_MODEL
    std::shared_ptr<SolverResults> solve(CompactModel&);
#endif

   public:
    void set_gurobi_options();

   protected:
    void collect_results(Model& model, std::shared_ptr<SolverResults>& results);
    void pre_solve();
    void post_solve();
};

}  // namespace coek
