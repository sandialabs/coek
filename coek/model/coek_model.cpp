#include <cstdio>
#include <memory>
#include <sstream>
#include <iostream>
#include <map>

#include "../util/map_utils.hpp"
#include "../ast/varray.hpp"
#include "coek/api/objective.hpp"
#include "coek/abstract/expr_rule.hpp"
#include "coek/autograd/autograd.hpp"
#include "coek/coek_model.hpp"
#include "coek/solvers/solver.hpp"
#ifdef COEK_WITH_COMPACT_MODEL
#include "coek/compact/objective_sequence.hpp"
#include "coek/compact/constraint_sequence.hpp"
#include "coek/compact/coek_exprterm.hpp"
#endif



namespace coek {

//
// Model
//

bool Model::minimize = true;
bool Model::maximize = false;
double Model::inf = COEK_INFINITY;

std::ostream& operator<<(std::ostream& ostr, const Model& arg)
{
arg.print_equations(ostr);
return ostr;
}

void Model::print_equations() const
{ print_equations(std::cout); }

void Model::print_equations(std::ostream& ostr) const
{
ostr << "MODEL" << std::endl;
ostr << "  Objectives" << std::endl;
for (auto it=repn->objectives.begin(); it != repn->objectives.end(); ++it) {
    ostr << "    " << *it << std::endl;
    }
ostr << "  Constraints" << std::endl;
for (auto it=repn->constraints.begin(); it != repn->constraints.end(); ++it) {
    ostr << "    " << *it << std::endl;
    }
}

void Model::print_values() const
{ print_values(std::cout); }

void Model::print_values(std::ostream& ostr) const
{ ostr << "ERROR - Model::print_values is not implemented yet." << std::endl; }

Model::Model()
{ repn = std::make_shared<ModelRepn>(); }

Model::~Model()
{}

Model::Model(const Model& other)
{ repn = other.repn; }

Model& Model::operator=(const Model& other)
{
repn = other.repn;
return *this;
}

Objective Model::add_objective(const Expression& expr, bool _sense)
{
Objective tmp(expr.repn, _sense);
repn->objectives.push_back(tmp);
return repn->objectives.back();
}

Objective Model::add_objective(const std::string& name, const Expression& expr, bool _sense)
{
Objective tmp(name, expr.repn, _sense);
repn->objectives.push_back(tmp);
repn->objectives_by_name.emplace(name, tmp);
//return repn->objectives.back();
return tmp;
}

Constraint Model::add_constraint(const Constraint& expr)
{
repn->constraints.push_back(expr);
return expr;
}

Constraint Model::add_constraint(const std::string& name, const Constraint& expr)
{
repn->constraints.push_back(expr);
repn->constraints.back().set_name(name);
repn->constraints_by_name.emplace(name, expr);
return expr;
}

Variable Model::add_variable(double lb, double ub, double value, bool binary, bool integer)
{
Variable tmp(lb,ub,value,binary,integer);
repn->variables.push_back(tmp);
return repn->variables.back();
}

Variable Model::add_variable(const std::string& name, double lb, double ub, double value, bool binary, bool integer)
{
Variable tmp(name,lb,ub,value,binary,integer);
repn->variables.push_back(tmp);
if (name != "")
    repn->variables_by_name.emplace(name, tmp);
return repn->variables.back();
//return tmp;
}

Variable Model::add_variable(Variable& var)
{
repn->variables.push_back(var);
auto name = var.get_name();
if (name != "")
    repn->variables_by_name.emplace(name, var);
return var;
}

void Model::add_variable(VariableArray& varray)
{
for (auto it=varray.variables.begin(); it != varray.variables.end(); it++) {
    repn->variables.push_back(*it);
    auto name = it->get_name();
    if (name != "")
        repn->variables_by_name.emplace(name, *it);
    }
}

#ifdef COEK_WITH_COMPACT_MODEL
void Model::add_variable(ConcreteIndexedVariable& vars)
{
auto end = vars.end();
for (auto it=vars.begin(); it != end; ++it) {
    repn->variables.push_back(*it);
    auto name = it->get_name();
    if (name != "")
        repn->variables_by_name.emplace(name, *it);
    }
}
#endif

size_t Model::num_variables() const
{ return repn->variables.size(); }

size_t Model::num_objectives() const
{ return repn->objectives.size(); }

size_t Model::num_constraints() const
{ return repn->constraints.size(); }

Variable Model::get_variable(unsigned int i)
{
if (i > repn->variables.size())
    throw std::out_of_range("Variable index " + std::to_string(i) + " is too large: " + std::to_string(repn->variables.size()) + "       variables available.");
return repn->variables[i];
}

Objective Model::get_objective(unsigned int i)
{
if (i > repn->objectives.size())
    throw std::out_of_range("Objective index " + std::to_string(i) + " is too large: " + std::to_string(repn->objectives.size()) + "       objectives available.");
return repn->objectives[i];
}

Constraint Model::get_constraint(unsigned int i)
{
if (i > repn->constraints.size())
    throw std::out_of_range("Constraint index " + std::to_string(i) + " is too large: " + std::to_string(repn->constraints.size()) + "      constraints available.");
return repn->constraints[i];
}

Variable Model::get_variable(const std::string& name)
{
auto it = repn->variables_by_name.find(name);
if (it == repn->variables_by_name.end()) 
    throw std::runtime_error("Unknown variable name " + name);
return it->second;
}

Objective Model::get_objective(const std::string& name)
{
auto it = repn->objectives_by_name.find(name);
if (it == repn->objectives_by_name.end()) 
    throw std::runtime_error("Unknown objective name " + name);
return it->second;
}

Constraint Model::get_constraint(const std::string& name)
{
auto it = repn->constraints_by_name.find(name);
if (it == repn->constraints_by_name.end()) 
    throw std::runtime_error("Unknown constraint name " + name);
return it->second;
}

std::set<std::string> Model::variable_names() const
{ return map_keys(repn->variables_by_name); }

std::set<std::string> Model::objective_names() const
{ return map_keys(repn->objectives_by_name); }

std::set<std::string> Model::constraint_names() const
{ return map_keys(repn->constraints_by_name); }

void Model::set_suffix(const std::string& name, Variable& var, double value)
{ repn->vsuffix[name].emplace(var.id(), value); }

void Model::set_suffix(const std::string& name, Constraint& con, double value)
{ repn->csuffix[name].emplace(con.id(), value); }

void Model::set_suffix(const std::string& name, Objective& obj, double value)
{ repn->osuffix[name].emplace(obj.id(), value); }

void Model::set_suffix(const std::string& name, double value)
{ repn->msuffix.emplace(name, value); }

double Model::get_suffix(const std::string& name, Variable& var)
{ return repn->vsuffix[name][var.id()]; }

double Model::get_suffix(const std::string& name, Constraint& con)
{ return repn->csuffix[name][con.id()]; }

double Model::get_suffix(const std::string& name, Objective& obj)
{ return repn->csuffix[name][obj.id()]; }

double Model::get_suffix(const std::string& name)
{ return repn->msuffix[name]; }

std::set<std::string> Model::variable_suffix_names() const
{ return map_keys(repn->vsuffix); }

std::set<std::string> Model::objective_suffix_names() const
{ return map_keys(repn->osuffix); }

std::set<std::string> Model::constraint_suffix_names() const
{ return map_keys(repn->csuffix); }

std::set<std::string> Model::model_suffix_names() const
{ return map_keys(repn->msuffix); }

static bool endsWith(const std::string& str, const std::string& suffix)
{ return str.size() >= suffix.size() && 0 == str.compare(str.size()-suffix.size(), suffix.size(), suffix); }

void write_lp_problem(Model& model, std::string& fname, std::map<int,int>& varmap, std::map<int,int>& conmap);
void write_nl_problem(Model& model, std::string& fname, std::map<int,int>& varmap, std::map<int,int>& conmap);

void write_lp_problem_ostream(Model& model, std::string& fname, std::map<int,int>& varmap, std::map<int,int>& conmap);
void write_nl_problem_ostream(Model& model, std::string& fname, std::map<int,int>& varmap, std::map<int,int>& conmap);
#ifdef WITH_FMTLIB
void write_lp_problem_fmtlib(Model& model, std::string& fname, std::map<int,int>& varmap, std::map<int,int>& conmap);
void write_nl_problem_fmtlib(Model& model, std::string& fname, std::map<int,int>& varmap, std::map<int,int>& conmap);
#endif

#ifdef COEK_WITH_COMPACT_MODEL
void write_lp_problem(CompactModel& model, std::string& fname, std::map<int,int>& varmap, std::map<int,int>& conmap);
void write_lp_problem_ostream(CompactModel& model, std::string& fname, std::map<int,int>& varmap, std::map<int,int>& conmap);
#ifdef WITH_FMTLIB
void write_lp_problem_fmtlib(CompactModel& model, std::string& fname, std::map<int,int>& varmap, std::map<int,int>& conmap);
#endif
#endif

void Model::write(std::string fname)
{
std::map<int,int> varmap;
std::map<int,int> conmap;
write(fname, varmap, conmap);
}

void Model::write(std::string fname, std::map<int,int>& varmap, std::map<int,int>& conmap)
{
if (endsWith(fname, ".lp")) {
    write_lp_problem(*this, fname, varmap, conmap);
    return;
    }

else if (endsWith(fname, ".ostrlp")) {
    write_lp_problem_ostream(*this, fname, varmap, conmap);
    return;
    }

#ifdef WITH_FMTLIB
else if (endsWith(fname, ".fmtlp")) {
    write_lp_problem_fmtlib(*this, fname, varmap, conmap);
    return;
    }
#endif

else if (endsWith(fname, ".nl")) {
    write_nl_problem(*this, fname, varmap, conmap);
    return;
    }

else if (endsWith(fname, ".ostrnl")) {
    write_nl_problem_ostream(*this, fname, varmap, conmap);
    return;
    }

#ifdef WITH_FMTLIB
else if (endsWith(fname, ".fmtnl")) {
    write_nl_problem_fmtlib(*this, fname, varmap, conmap);
    return;
    }
#endif

throw std::runtime_error("Unknown problem type");
}


//
// CompactModel
//

#ifdef COEK_WITH_COMPACT_MODEL
Objective CompactModel::add_objective(const Expression& expr, bool _sense)
{
Objective obj(expr, _sense);
objectives.push_back( obj );
return obj;
}

void CompactModel::add_objective(const Expression& expr, const SequenceContext& context, bool _sense)
{
ObjectiveSequence seq(expr, context, _sense);
objectives.push_back( seq );
}

Constraint CompactModel::add_constraint(const Constraint& expr)
{
constraints.push_back(expr);
return expr;
}

void CompactModel::add_constraint(const Constraint& expr, const SequenceContext& context)
{
ConstraintSequence seq(expr, context);
constraints.push_back(seq);
}

Variable CompactModel::add_variable(double lb, double ub, double value, bool binary, bool integer)
{
Variable tmp(lb,ub,value,binary,integer);
variables.push_back(tmp);
return variables.back();
}

Variable CompactModel::add_variable(const std::string& name, double lb, double ub, double value, bool binary, bool integer)
{
Variable tmp(name,lb,ub,value,binary,integer);
variables.push_back(tmp);
return variables.back();
}

Variable CompactModel::add_variable(Variable& var)
{
variables.push_back(var);
return var;
}

void CompactModel::add_variable(VariableArray& varray)
{
for (auto it=varray.variables.begin(); it != varray.variables.end(); it++) {
    variables.push_back(*it);
    }
}

void CompactModel::add_variable(ConcreteIndexedVariable& vars)
{
auto end = vars.end();
for (auto it=vars.begin(); it != end; ++it) {
    variables.push_back(*it);
    }
}

Model CompactModel::expand()
{
Model model;
model.repn->variables = variables;
for (auto it=variables.begin(); it != variables.end(); ++it) {
    auto name = it->get_name();
    if (name != "")
        model.repn->variables_by_name.emplace(name, *it);
    }

//int i=0;
for (auto it=objectives.begin(); it != objectives.end(); ++it) {
    auto& val = *it;
    if (auto eval = std::get_if<Objective>(&val)) {
        Expression e = eval->body().expand();
        model.add_objective(e, eval->sense());
        }
    else {
#if 0
        auto& seq = std::get<ExpressionSequence>(val);
        for (auto jt=seq.begin(); jt != seq.end(); ++jt) {
            //model.repn->objectives.push_back(*jt);
            model.add( *jt, osense); //repn->objectives.push_back(*jt);
            //model.repn->sense.push_back(osense);
            }
#endif
        }
    }

//i=0;
for (auto it=constraints.begin(); it != constraints.end(); ++it) {
    auto& val = *it;
    if (auto cval = std::get_if<Constraint>(&val)) {
        Constraint c = cval->expand();
        model.repn->constraints.push_back(c);
        }
    else {
        auto& seq = std::get<ConstraintSequence>(val);
        for (auto jt=seq.begin(); jt != seq.end(); ++jt) {
            model.repn->constraints.push_back(*jt);
            }
        }
    }
return model;
}

void CompactModel::write(std::string fname)
{
std::map<int,int> varmap;
std::map<int,int> conmap;
write(fname, varmap, conmap);
}

void CompactModel::write(std::string fname, std::map<int,int>& varmap, std::map<int,int>& conmap)
{
if (endsWith(fname, ".lp")) {
    write_lp_problem(*this, fname, varmap, conmap);
    return;
    }

else if (endsWith(fname, ".ostrlp")) {
    write_lp_problem_ostream(*this, fname, varmap, conmap);
    return;
    }

#ifdef WITH_FMTLIB
else if (endsWith(fname, ".fmtlp")) {
    write_lp_problem_fmtlib(*this, fname, varmap, conmap);
    return;
    }
#endif

Model model = expand();
model.write(fname, varmap, conmap);
}
#endif

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

Constraint NLPModel::get_constraint(size_t i)
{
if (repn == 0)
    throw std::runtime_error("Calling get_constraint() for uninitialized NLPModel.");
return repn->get_constraint(i);
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

void NLPModel::write(std::string fname)
{
if (repn == 0)
    throw std::runtime_error("Calling write() for uninitialized NLPModel.");
std::map<int,int> varmap;
std::map<int,int> conmap;
repn->model.write(fname, varmap, conmap);
}

void NLPModel::write(std::string fname, std::map<int,int>& varmap, std::map<int,int>& conmap)
{
if (repn == 0)
    throw std::runtime_error("Calling write() for uninitialized NLPModel.");
repn->model.write(fname, varmap, conmap);
}

void NLPModel::print_equations() const
{ print_equations(std::cout); }

void NLPModel::print_equations(std::ostream& ostr) const
{
if (repn == 0)
    throw std::runtime_error("Calling print_equations() for uninitialized NLPModel.");
repn->print_equations(ostr);
}

void NLPModel::print_values() const
{ print_values(std::cout); }

void NLPModel::print_values(std::ostream& ostr) const
{
if (repn == 0)
    throw std::runtime_error("Calling print_values() for uninitialized NLPModel.");
repn->print_values(ostr);
}

std::ostream& operator<<(std::ostream& ostr, const NLPModel& arg)
{
arg.print_equations(ostr);
return ostr;
}


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
{ return repn.use_count() > 0; }

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


void check_that_expression_variables_are_declared(Model& model, const std::set<unsigned int>& var_ids)
{
std::unordered_set<unsigned int> model_ids;

auto end = model.repn->variables.end();
for (auto it=model.repn->variables.begin(); it != end; ++it)
    model_ids.insert( (*it).id() );

// TODO - Make this faster because both sets are ordered
for (auto it=var_ids.begin(); it != var_ids.end(); it++) {
    auto tmp = model_ids.find(*it);
    if (tmp == model_ids.end()) {
        throw std::runtime_error("Model expressions contain variables that are not declared in the model.");
        }
    }

// TODO - More diagnostic analysis here
}

}
