#include "coek/solvers/solver.hpp"
#include "coek/api/constraint.hpp"
#include "coek/api/expression.hpp"
#include "coek/api/objective.hpp"
#include "coek/solvers/solver_repn.hpp"

namespace coek {

//
// Solver
//

void Solver::initialize(std::string name)
{
    SolverRepn* tmp = create_solver(name);
    if (tmp) repn = std::shared_ptr<SolverRepn>(tmp);
}

bool Solver::available() const { return repn.get(); }

int Solver::solve(Model& model) { return repn->solve(model); }

void Solver::load(Model& model) { repn->load(model); }

#ifdef COEK_WITH_COMPACT_MODEL
int Solver::solve(CompactModel& model) { return repn->solve(model); }

void Solver::load(CompactModel& model) { repn->load(model); }
#endif

int Solver::resolve() { return repn->resolve(); }

void Solver::reset() { repn->reset(); }

bool Solver::error_status() const
{
    if (not repn.get()) return true;
    return repn->error_occurred;
}
int Solver::error_code() const
{
    if (not repn.get()) return -1;
    return repn->error_code;
}
std::string Solver::error_message() const
{
    if (not repn.get()) return "Error constructing solver.";
    return repn->error_message;
}

//
// NLPSolver
//

void NLPSolver::initialize(std::string name)
{
    std::shared_ptr<NLPSolverRepn> tmp(create_nlpsolver(name, *this));
    repn = tmp;
}

bool NLPSolver::available() const { return repn.get() && repn->available(); }

int NLPSolver::solve(NLPModel& model) { return repn->solve(model); }

void NLPSolver::load(NLPModel& model) { repn->load(model); }

int NLPSolver::resolve(bool reset_nlpmodel) { return repn->resolve(reset_nlpmodel); }

void NLPSolver::reset() { repn->reset(); }

bool NLPSolver::error_status() const
{
    if (not repn.get()) return true;
    return repn->error_occurred;
}
int NLPSolver::error_code() const
{
    if (not repn.get()) return -1;
    return repn->error_code;
}
std::string NLPSolver::error_message() const
{
    if (not repn.get()) return "Error constructing solver.";
    return repn->error_message;
}

}  // namespace coek
