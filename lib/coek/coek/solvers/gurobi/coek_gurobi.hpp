#pragma once

#include "gurobi_c++.h"
#include "coek/solvers/solver_repn.hpp"

namespace coek {

class GurobiSolver : public SolverRepn {
   public:
    GRBEnv* env;
    GRBModel* gmodel;
    std::unordered_map<size_t, GRBVar> x;
    std::shared_ptr<SolverResults> results;

    enum class gurobi_license_status { available, error, unknown };
    gurobi_license_status license_status;

   public:
    GurobiSolver() : SolverRepn(), env(0), gmodel(0), license_status(gurobi_license_status::unknown)
    {
    }
    ~GurobiSolver();

    bool available();

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
