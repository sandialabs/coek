#include <cstdio>
#include <memory>
#include <sstream>
#include <fstream>
#include <map>
#include "expr/ast_term.hpp"
#include "expr/varray.hpp"
#include "coek_expr.hpp"
#include "coek_model.hpp"
#include "solvers/solver.hpp"
#include "autograd/autograd.hpp"

namespace coek {


//
// Model
//

bool Model::minimize = true;
bool Model::maximize = false;
double Model::inf = COEK_INFINITY;

std::ostream& operator<<(std::ostream& ostr, const Model& arg)
{
ostr << "MODEL" << std::endl;
ostr << "  Objectives" << std::endl;
for (std::vector<Expression>::const_iterator it=arg.objectives.begin(); it != arg.objectives.end(); ++it) {
    ostr << "    " << *it << std::endl;
    }
ostr << "  Constraints" << std::endl;
for (std::vector<Constraint>::const_iterator it=arg.constraints.begin(); it != arg.constraints.end(); ++it) {
    ostr << "    " << *it << std::endl;
    }
return ostr;
}

Variable& Model::getVariable(double lb, double ub, const std::string& name)
{
Variable tmp(lb,ub,COEK_NAN,name);
tmp.repn->index = ++VariableTerm::count;
variables.push_back(tmp);
return variables.back();
}

Variable& Model::getVariable(double lb, double ub, double value)
{
Variable tmp(lb,ub,value);
tmp.repn->index = ++VariableTerm::count;
variables.push_back(tmp);
return variables.back();
}

Variable& Model::getVariable(double lb, double ub, double value, const std::string& name)
{
Variable tmp(lb,ub,value,name);
tmp.repn->index = ++VariableTerm::count;
variables.push_back(tmp);
return variables.back();
}

Variable& Model::getVariable(double lb, double ub, double value, bool binary, bool integer)
{
Variable tmp(lb,ub,value,binary,integer);
tmp.repn->index = ++VariableTerm::count;
variables.push_back(tmp);
return variables.back();
}

Variable& Model::getVariable(double lb, double ub, double value, bool binary, bool integer, const std::string& name)
{
Variable tmp(lb,ub,value,binary,integer,name);
tmp.repn->index = ++VariableTerm::count;
variables.push_back(tmp);
return variables.back();
}

void Model::addVariable(Variable& var)
{
var.repn->index = ++VariableTerm::count;
variables.push_back(var);
}

void Model::addVariable(VariableArray& varray)
{
for (auto it=varray.variables.begin(); it != varray.variables.end(); it++) {
    it->repn->index = ++VariableTerm::count;
    variables.push_back(*it);
    }
}

Expression Model::get_objective(unsigned int i)
{
if (i > objectives.size())
    throw std::out_of_range("Objective index " + std::to_string(i) + " is too large: " + std::to_string(objectives.size()) + "       objectives available.");
return objectives[i];
}

Constraint Model::get_constraint(unsigned int i)
{
if (i > constraints.size())
    throw std::out_of_range("Constraint index " + std::to_string(i) + " is too large: " + std::to_string(constraints.size()) + "      constraints available.");
return constraints[i];
}


static bool endsWith(const std::string& str, const std::string& suffix)
{
    return str.size() >= suffix.size() && 0 == str.compare(str.size()-suffix.size(), suffix.size(), suffix);
}

void write_lp_problem(Model& model, std::ostream& ostr);
void write_nl_problem(Model& model, std::ostream& ostr);


void Model::write(std::string fname)
{
if (endsWith(fname, ".lp")) {
    std::ofstream ofstr(fname);
    write_lp_problem(*this, ofstr);
    ofstr.close();
    return;
    }

else if (endsWith(fname, ".nl")) {
    std::ofstream ofstr(fname);
    write_nl_problem(*this, ofstr);
    ofstr.close();
    return;
    }

throw std::runtime_error("Unknown problem type");
}


//
// NLPModel
//

NLPModel::NLPModel()
{}

NLPModel::NLPModel(Model& model, std::string type, bool sparse_JH)
{
initialize(model, type, sparse_JH);
}

void NLPModel::initialize(Model& model, std::string type, bool sparse_JH)
{
std::shared_ptr<NLPModelRepn> tmp(create_NLPModelRepn(model, type));
repn = tmp;
repn->initialize(sparse_JH);
}

void NLPModel::reset()
{
if (repn == 0)
    return;
repn->reset();
}

size_t NLPModel::num_variables() const
{
if (repn == 0)
    return 0;
return repn->num_variables();
}

size_t NLPModel::num_objectives() const
{
if (repn == 0)
    return 0;
return repn->num_objectives();
}

size_t NLPModel::num_constraints() const
{
if (repn == 0)
    return 0;
return repn->num_constraints();
}

size_t NLPModel::num_nonzeros_Jacobian() const
{
if (repn == 0)
    return 0;
return repn->num_nonzeros_Jacobian();
}

size_t NLPModel::num_nonzeros_Hessian_Lagrangian() const
{
if (repn == 0)
    return 0;
return repn->num_nonzeros_Hessian_Lagrangian();
}

Variable NLPModel::get_variable(size_t i)
{
if (repn == 0)
    throw std::runtime_error("Calling get_variables() for uninitialized NLPModel.");
return repn->get_variable(i);
}

void NLPModel::set_variable(size_t i, const Variable& v)
{
if (repn == 0)
    throw std::runtime_error("Calling set_variables() for uninitialized NLPModel.");
repn->set_variable(i, v.repn);
}

void NLPModel::set_variables(std::vector<double>& x)
{
if (repn == 0)
    throw std::runtime_error("Calling set_variables() for uninitialized NLPModel.");
repn->set_variables(x);
}

void NLPModel::set_variables(const double* x, size_t n)
{
if (repn == 0)
    throw std::runtime_error("Calling set_variables() for uninitialized NLPModel.");
repn->set_variables(x, n);
}

void NLPModel::get_J_nonzeros(std::vector<size_t>& jrow, std::vector<size_t>& jcol)
{
if (repn == 0)
    throw std::runtime_error("Calling get_J_nonzeros() for uninitialized NLPModel.");
repn->get_J_nonzeros(jrow, jcol);
}

void NLPModel::get_H_nonzeros(std::vector<size_t>& hrow, std::vector<size_t>& hcol)
{
if (repn == 0)
    throw std::runtime_error("Calling get_H_nonzeros() for uninitialized NLPModel.");
repn->get_H_nonzeros(hrow, hcol);
}

void NLPModel::write(std::ostream& ostr) const
{
if (repn == 0)
    throw std::runtime_error("Calling write() for uninitialized NLPModel.");
repn->write(ostr);
}

double NLPModel::compute_f(unsigned int i)
{
if (repn == 0)
    throw std::runtime_error("Calling compute_f() for uninitialized NLPModel.");
return repn->compute_f(i);
}

void NLPModel::compute_df(double& f, std::vector<double>& df, unsigned int i)
{
if (repn == 0)
    throw std::runtime_error("Calling compute_df() for uninitialized NLPModel.");
repn->compute_df(f, df, i);
}

void NLPModel::compute_H(std::vector<double>& w, std::vector<double>& H)
{
if (repn == 0)
    throw std::runtime_error("Calling compute_H() for uninitialized NLPModel.");
repn->compute_H(w, H);
}

void NLPModel::compute_c(std::vector<double>& c)
{
if (repn == 0)
    throw std::runtime_error("Calling compute_c() for uninitialized NLPModel.");
repn->compute_c(c);
}

void NLPModel::compute_dc(std::vector<double>& dc, unsigned int i)
{
if (repn == 0)
    throw std::runtime_error("Calling compute_dc() for uninitialized NLPModel.");
repn->compute_dc(dc, i);
}

void NLPModel::compute_J(std::vector<double>& J)
{
if (repn == 0)
    throw std::runtime_error("Calling compute_J() for uninitialized NLPModel.");
repn->compute_J(J);
}

std::ostream& operator<<(std::ostream& ostr, const NLPModel& arg)
{
arg.write(ostr);
return ostr;
}


//
// Solver
//

void Solver::initialize(std::string name)
{
std::shared_ptr<SolverRepn> tmp(create_solver(name));
repn = tmp;
}

int Solver::solve(Model& model)
{
return repn->solve(model);
}

void Solver::load(Model& model)
{
repn->load(model);
}

int Solver::resolve()
{
return repn->resolve();
}

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

int NLPSolver::solve(NLPModel& model)
{
return repn->solve(model);
}

void NLPSolver::load(NLPModel& model)
{
repn->load(model);
}

int NLPSolver::resolve()
{
return repn->resolve();
}

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
