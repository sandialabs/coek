#pragma once

#include <map>
#include <memory>
#include <string>

#include "coek/model/nlp_model.hpp"
#include "coek/solvers/solver_repn.hpp"

namespace coek {

class IpoptSolverRepn {
   public:
    virtual std::shared_ptr<SolverResults> perform_solve() = 0;
    virtual void set_start_from_last_x(bool flag) = 0;
    virtual void set_options(const std::map<std::string, std::string>& string_options,
                             const std::map<std::string, bool>& boolean_options,
                             const std::map<std::string, int>& integer_options,
                             const std::map<std::string, double>& double_options)
        = 0;
};

class IpoptSolver : public NLPSolverRepn {
   protected:
    bool available_;
    std::string error_message;
    std::shared_ptr<IpoptSolverRepn> repn;

    void initialize();

   public:
    IpoptSolver() : NLPSolverRepn(), available_(true) { initialize(); }

    void load(NLPModel& model);

    std::shared_ptr<SolverResults> resolve_exec();

    std::shared_ptr<SolverResults> solve(NLPModel& model);

    bool available() { return available_; }
};

}  // namespace coek
