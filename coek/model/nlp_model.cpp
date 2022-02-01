//#include <cstdio>
//#include <memory>
//#include <sstream>
//#include <iostream>
//#include <map>
//#include <cmath>

//#include "../util/map_utils.hpp"
//#include "../util/endswith.hpp"
//#include "../ast/varray.hpp"
//#include "../ast/value_terms.hpp"
#include "coek/api/objective.hpp"
#include "coek/api/constraint.hpp"
//#include "coek/abstract/expr_rule.hpp"
#include "coek/autograd/autograd.hpp"
#include "coek/coek_model.hpp"
//#include "coek/solvers/solver.hpp"
#include "model_repn.hpp"


namespace coek {

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
if (repn)
    repn->reset();
}

size_t NLPModel::num_variables() const
{
if (repn)
    return repn->num_variables();
return 0;
}

size_t NLPModel::num_objectives() const
{
if (repn)
    return repn->num_objectives();
return 0;
}

size_t NLPModel::num_constraints() const
{
if (repn)
    return repn->num_constraints();
return 0;
}

size_t NLPModel::num_nonzeros_Jacobian() const
{
if (repn)
    return repn->num_nonzeros_Jacobian();
return 0;
}

size_t NLPModel::num_nonzeros_Hessian_Lagrangian() const
{
if (repn)
    return repn->num_nonzeros_Hessian_Lagrangian();
return 0;
}

Variable NLPModel::get_variable(size_t i)
{
if (repn)
    return repn->get_variable(i);
else
    throw std::runtime_error("Calling get_variables() for uninitialized NLPModel.");
}

void NLPModel::set_variable(size_t i, const Variable& v)
{
if (repn) 
    repn->set_variable(i, v.repn);
else
    throw std::runtime_error("Calling set_variables() for uninitialized NLPModel.");
}

void NLPModel::set_variables(std::vector<double>& x)
{
if (repn)
    repn->set_variables(x);
else
    throw std::runtime_error("Calling set_variables() for uninitialized NLPModel.");
}

void NLPModel::set_variables(const double* x, size_t n)
{
if (repn)
    repn->set_variables(x, n);
else
    throw std::runtime_error("Calling set_variables() for uninitialized NLPModel.");
}

Constraint NLPModel::get_constraint(size_t i)
{
if (repn)
    return repn->get_constraint(i);
else
    throw std::runtime_error("Calling get_constraint() for uninitialized NLPModel.");
}

void NLPModel::get_J_nonzeros(std::vector<size_t>& jrow, std::vector<size_t>& jcol)
{
if (repn)
    repn->get_J_nonzeros(jrow, jcol);
else
    throw std::runtime_error("Calling get_J_nonzeros() for uninitialized NLPModel.");
}

void NLPModel::get_H_nonzeros(std::vector<size_t>& hrow, std::vector<size_t>& hcol)
{
if (repn)
    repn->get_H_nonzeros(hrow, hcol);
else
    throw std::runtime_error("Calling get_H_nonzeros() for uninitialized NLPModel.");
}

double NLPModel::compute_f(unsigned int i)
{
if (repn)
    return repn->compute_f(i);
else
    throw std::runtime_error("Calling compute_f() for uninitialized NLPModel.");
}

void NLPModel::compute_df(double& f, std::vector<double>& df, unsigned int i)
{
if (repn)
    repn->compute_df(f, df, i);
else
    throw std::runtime_error("Calling compute_df() for uninitialized NLPModel.");
}

void NLPModel::compute_H(std::vector<double>& w, std::vector<double>& H)
{
if (repn)
    repn->compute_H(w, H);
else
    throw std::runtime_error("Calling compute_H() for uninitialized NLPModel.");
}

void NLPModel::compute_c(std::vector<double>& c)
{
if (repn)
    repn->compute_c(c);
else
    throw std::runtime_error("Calling compute_c() for uninitialized NLPModel.");
}

void NLPModel::compute_dc(std::vector<double>& dc, unsigned int i)
{
if (repn)
    repn->compute_dc(dc, i);
else
    throw std::runtime_error("Calling compute_dc() for uninitialized NLPModel.");
}

void NLPModel::compute_J(std::vector<double>& J)
{
if (repn)
    repn->compute_J(J);
else
    throw std::runtime_error("Calling compute_J() for uninitialized NLPModel.");
}

void NLPModel::write(std::string fname)
{
if (!repn)
    throw std::runtime_error("Calling write() for uninitialized NLPModel.");

std::map<int,int> varmap;
std::map<int,int> conmap;
repn->model.write(fname, varmap, conmap);
}

void NLPModel::write(std::string fname, std::map<int,int>& varmap, std::map<int,int>& conmap)
{
if (!repn)
    throw std::runtime_error("Calling write() for uninitialized NLPModel.");

repn->model.write(fname, varmap, conmap);
}

void NLPModel::print_equations() const
{ print_equations(std::cout); }

void NLPModel::print_equations(std::ostream& ostr) const
{
if (repn)
    repn->print_equations(ostr);
else
    throw std::runtime_error("Calling print_equations() for uninitialized NLPModel.");
}

void NLPModel::print_values() const
{ print_values(std::cout); }

void NLPModel::print_values(std::ostream& ostr) const
{
if (repn)
    repn->print_values(ostr);
else
    throw std::runtime_error("Calling print_values() for uninitialized NLPModel.");
}

std::ostream& operator<<(std::ostream& ostr, const NLPModel& arg)
{
arg.print_equations(ostr);
return ostr;
}


#if 0
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
#endif

}
