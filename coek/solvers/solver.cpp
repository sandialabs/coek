#include "coek/api/objective.hpp"
#include "coek/api/constraint.hpp"
#include "coek/solvers/solver.hpp"
#include "coek/solvers/solver_repn.hpp"


namespace coek {

//
// Solver
//

void Solver::initialize(std::string name)
{
SolverRepn* tmp = create_solver(name);
if (tmp)
    repn = std::shared_ptr<SolverRepn>(tmp);
}

bool Solver::available() const
{ return repn.get(); }

int Solver::solve(Model& model)
{ return repn->solve(model); }

void Solver::load(Model& model)
{ repn->load(model); }

#ifdef COEK_WITH_COMPACT_MODEL
int Solver::solve(CompactModel& model)
{ return repn->solve(model); }

void Solver::load(CompactModel& model)
{ repn->load(model); }
#endif

int Solver::resolve()
{ return repn->resolve(); }

void Solver::reset()
{ repn->reset(); }

bool Solver::get_option(const std::string& option, int& value) const
{ return repn->get_option(option, value); }
bool Solver::get_option(const std::string& option, double& value) const
{ return repn->get_option(option, value); }
bool Solver::get_option(const std::string& option, std::string& value) const
{ return repn->get_option(option, value); }
bool Solver::get_option(int option, int& value) const
{ return repn->get_option(option, value); }
bool Solver::get_option(int option, double& value) const
{ return repn->get_option(option, value); }
bool Solver::get_option(int option, std::string& value) const
{ return repn->get_option(option, value); }

void Solver::set_option(const std::string& option, int value)
{ repn->set_option(option, value); }
void Solver::set_option(const std::string& option, double value)
{ repn->set_option(option, value); }
void Solver::set_option(const std::string& option, const std::string value)
{ repn->set_option(option, value); }
void Solver::set_option(int option, int value)
{ repn->set_option(option, value); }
void Solver::set_option(int option, double value)
{ repn->set_option(option, value); }
void Solver::set_option(int option, const std::string value)
{ repn->set_option(option, value); }

//
// NLPSolver
//

void NLPSolver::initialize(std::string name)
{
std::shared_ptr<NLPSolverRepn> tmp(create_nlpsolver(name));
repn = tmp;
}

bool NLPSolver::available() const
{ return repn.get() && repn->available(); }

int NLPSolver::solve(NLPModel& model)
{ return repn->solve(model); }

void NLPSolver::load(NLPModel& model)
{ repn->load(model); }

int NLPSolver::resolve()
{ return repn->resolve(); }

void NLPSolver::reset()
{ repn->reset(); }

bool NLPSolver::get_option(const std::string& option, int& value) const
{ return repn->get_option(option, value); }
bool NLPSolver::get_option(const std::string& option, double& value) const
{ return repn->get_option(option, value); }
bool NLPSolver::get_option(const std::string& option, std::string& value) const
{ return repn->get_option(option, value); }
bool NLPSolver::get_option(int option, int& value) const
{ return repn->get_option(option, value); }
bool NLPSolver::get_option(int option, double& value) const
{ return repn->get_option(option, value); }
bool NLPSolver::get_option(int option, std::string& value) const
{ return repn->get_option(option, value); }

void NLPSolver::set_option(const std::string& option, int value)
{ repn->set_option(option, value); }
void NLPSolver::set_option(const std::string& option, double value)
{ repn->set_option(option, value); }
void NLPSolver::set_option(const std::string& option, const std::string value)
{ repn->set_option(option, value); }
void NLPSolver::set_option(int option, int value)
{ repn->set_option(option, value); }
void NLPSolver::set_option(int option, double value)
{ repn->set_option(option, value); }
void NLPSolver::set_option(int option, const std::string value)
{ repn->set_option(option, value); }

}
