#include "coek/solvers/solver.hpp"
#include "coek/api/expression.hpp"
#include "coek/api/constraint.hpp"
#include "coek/api/objective.hpp"
#include "coek/solvers/solver_repn.hpp"

namespace coek {

//
// Solver
//

void Solver::initialize(std::string name_)
{
    std::shared_ptr<SolverRepn> tmp(create_solver(name_, *this));
    repn = tmp;
    name = name_;
}

bool Solver::available() const { return repn.get(); }

std::shared_ptr<SolverResults> Solver::solve(Model& model)
{
    if (not repn.get()) {
        auto res = std::make_shared<SolverResults>();
        res->solver_name = name;
        res->termination_condition = TerminationCondition::solver_not_available;
        return res;
    }
    return repn->solve(model);
}

void Solver::load(Model& model) { repn->load(model); }

#ifdef COEK_WITH_COMPACT_MODEL
std::shared_ptr<SolverResults> Solver::solve(CompactModel& model)
{
    if (not repn.get()) {
        auto res = std::make_shared<SolverResults>();
        res->solver_name = name;
        res->termination_condition = TerminationCondition::solver_not_available;
        return res;
    }
    return repn->solve(model);
}

void Solver::load(CompactModel& model) { repn->load(model); }
#endif

std::shared_ptr<SolverResults> Solver::resolve()
{
    if (not repn.get()) {
        auto res = std::make_shared<SolverResults>();
        res->solver_name = name;
        res->termination_condition = TerminationCondition::solver_not_available;
        return res;
    }
    return repn->resolve();
}

void Solver::reset() { repn->reset(); }

//
// NLPSolver
//

void NLPSolver::initialize(std::string name_)
{
    std::shared_ptr<NLPSolverRepn> tmp(create_nlpsolver(name_, *this));
    repn = tmp;
    name = name_;
}

bool NLPSolver::available() const { return repn.get() && repn->available(); }

std::shared_ptr<SolverResults> NLPSolver::solve(NLPModel& model)
{
    if (not repn.get() or not repn->available()) {
        auto res = std::make_shared<SolverResults>();
        res->solver_name = name;
        res->termination_condition = TerminationCondition::solver_not_available;
        return res;
    }
    return repn->solve(model);
}

void NLPSolver::load(NLPModel& model) { repn->load(model); }

std::shared_ptr<SolverResults> NLPSolver::resolve(bool reset_nlpmodel)
{
    if (not repn.get() or not repn->available()) {
        auto res = std::make_shared<SolverResults>();
        res->solver_name = name;
        res->termination_condition = TerminationCondition::solver_not_available;
        return res;
    }
    return repn->resolve(reset_nlpmodel);
}

void NLPSolver::reset() { repn->reset(); }

}  // namespace coek
