//#include <cstdio>
#include <memory>
#include <sstream>
#include <iostream>
#include <map>
#include <cmath>
#include <unordered_set>

#include "../util/map_utils.hpp"
#include "../util/endswith.hpp"
#include "../ast/varray.hpp"
#include "../ast/value_terms.hpp"
#include "coek/api/objective.hpp"
#include "coek/api/constraint.hpp"
#include "coek/model/model.hpp"
#ifdef COEK_WITH_COMPACT_MODEL
#include "coek/compact/coek_indexed.hpp"
#endif
#include "model_repn.hpp"


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
{
ostr << "Model Variables: " << repn->variables_by_name.size() << "\n";
ostr << "Nonzero Variables\n";
for (auto const& var: repn->variables_by_name) {
    double val = var.second.get_value();
    if (::fabs(val) > 1e-7) {
        ostr << "   " << var.first << " " << val << " " << var.second.get_fixed() << "\n";
        }
    }
}

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
}

Variable Model::add_variable(const Expression& lb, const Expression& ub, const Expression& value, bool binary, bool integer)
{
Variable tmp(lb,ub,value,binary,integer);
repn->variables.push_back(tmp);
return repn->variables.back();
}

Variable Model::add_variable(const std::string& name, const Expression& lb, const Expression& ub, const Expression& value, bool binary, bool integer)
{
Variable tmp(name,lb,ub,value,binary,integer);
repn->variables.push_back(tmp);
if (name != "")
    repn->variables_by_name.emplace(name, tmp);
return repn->variables.back();
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
if (name != "")
    repn->objectives_by_name.emplace(name, tmp);
return repn->objectives.back();
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

size_t Model::num_variables() const
{ return repn->variables.size(); }

size_t Model::num_objectives() const
{ return repn->objectives.size(); }

size_t Model::num_constraints() const
{ return repn->constraints.size(); }

Variable Model::get_variable(unsigned int i)
{
if (i >= repn->variables.size())
    throw std::out_of_range("Variable index " + std::to_string(i) + " is too large: " + std::to_string(repn->variables.size()) + " variables available.");
return repn->variables[i];
}

std::vector<Variable>& Model::get_variables()
{ return repn->variables; }

Objective Model::get_objective(unsigned int i)
{
if (i >= repn->objectives.size())
    throw std::out_of_range("Objective index " + std::to_string(i) + " is too large: " + std::to_string(repn->objectives.size()) + " objectives available.");
return repn->objectives[i];
}

Constraint Model::get_constraint(unsigned int i)
{
if (i >= repn->constraints.size())
    throw std::out_of_range("Constraint index " + std::to_string(i) + " is too large: " + std::to_string(repn->constraints.size()) + " constraints available.");
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
{ return repn->osuffix[name][obj.id()]; }

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

void write_lp_problem(Model& model, std::string& fname, std::map<int,int>& varmap, std::map<int,int>& conmap);
void write_nl_problem(Model& model, std::string& fname, std::map<int,int>& varmap, std::map<int,int>& conmap);

void write_lp_problem_ostream(Model& model, std::string& fname, std::map<int,int>& varmap, std::map<int,int>& conmap);
void write_nl_problem_ostream(Model& model, std::string& fname, std::map<int,int>& varmap, std::map<int,int>& conmap);
#ifdef WITH_FMTLIB
void write_lp_problem_fmtlib(Model& model, std::string& fname, std::map<int,int>& varmap, std::map<int,int>& conmap);
void write_nl_problem_fmtlib(Model& model, std::string& fname, std::map<int,int>& varmap, std::map<int,int>& conmap);
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

throw std::runtime_error("Unknown problem type: "+fname);
}


void check_that_expression_variables_are_declared(Model& model, const std::map<unsigned int,Variable>& varobj)
{
std::unordered_set<unsigned int> model_ids;

auto end = model.repn->variables.end();
for (auto it=model.repn->variables.begin(); it != end; ++it)
    model_ids.insert( (*it).id() );

/*
for (unsigned int i: model_ids)
  std::cout << i << std::endl;
std::cout << std::endl;
for (const auto& i: varobj)
  std::cout << i.first << " " << i.second << std::endl;
std::cout << std::endl;
*/

// TODO - Make this faster because both sets are ordered
for (auto it=varobj.begin(); it != varobj.end(); it++) {
    auto tmp = model_ids.find(it->first);
    if (tmp == model_ids.end()) {
        throw std::runtime_error("Model expressions contain variable '" + it->second.get_name() + "' that is not declared in the model.");
        }
    }
}

void check_that_expression_variables_are_declared(Model& model, const std::unordered_set<VariableTerm*>& vars)
{
std::unordered_set<unsigned int> model_ids;

auto end = model.repn->variables.end();
for (auto it=model.repn->variables.begin(); it != end; ++it)
    model_ids.insert( (*it).id() );

// TODO - Make this faster because both sets are ordered
for (auto it=vars.begin(); it != vars.end(); it++) {
    auto tmp = model_ids.find((*it)->index);
    if (tmp == model_ids.end()) {
        throw std::runtime_error("Model expressions contain variable '" + (*it)->name + "' that is not declared in the model.");
        }
    }
}

}
