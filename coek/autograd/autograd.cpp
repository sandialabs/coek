#include <unordered_set>
#include "../ast/visitor_fns.hpp"
#include "../ast/objective_terms.hpp"
#include "../ast/constraint_terms.hpp"
#include "../ast/value_terms.hpp"
#include "coek/api/objective.hpp"
#include "coek/api/constraint.hpp"
#include "autograd.hpp"

#ifdef WITH_CPPAD
#include "cppad_repn.hpp"
#endif

namespace coek {

void check_that_expression_variables_are_declared(Model& model, const std::set<unsigned int>& var_ids);


NLPModelRepn* create_NLPModelRepn(Model& model, std::string& name)
{
#ifdef WITH_CPPAD
if (name == "cppad")
    return new CppAD_Repn(model);
#endif

throw std::runtime_error("Unknown autograd type.");
return 0;
}


void NLPModelRepn::find_used_variables()
{
std::unordered_set<VariableTerm*> vars;
std::set<VariableTerm*> fixed_vars;
std::set<ParameterTerm*> params;

for (auto it=model.repn->objectives.begin(); it != model.repn->objectives.end(); ++it)
    find_vars_and_params(it->repn, vars, fixed_vars, params);
for (auto it=model.repn->constraints.begin(); it != model.repn->constraints.end(); ++it)
    find_vars_and_params(it->repn, vars, fixed_vars, params);

//std::cout << "HERE x " << vars.size() << std::endl;
std::set<unsigned int> used_ids;
for (auto it=vars.begin(); it != vars.end(); ++it)
    used_ids.insert((*it)->index);
check_that_expression_variables_are_declared(model, used_ids);

std::map<int,VariableTerm*> tmp;
for (auto it=vars.begin(); it != vars.end(); ++it)
    tmp[(*it)->index] = *it;

used_variables.clear();
int i=0;
for (auto it=tmp.begin(); it != tmp.end(); ++it)
    used_variables[i++] = it->second;


i = 0;
fixed_variables.clear();
for (auto it=fixed_vars.begin(); it != fixed_vars.end(); ++it)
    fixed_variables[*it] = i++;
parameters.clear();
for (auto it=params.begin(); it != params.end(); ++it)
    parameters[*it] = i++;
}

VariableTerm* NLPModelRepn::get_variable(int i)
{
return used_variables[i];
}

void NLPModelRepn::set_variable(int i, const VariableTerm* _v)
{
auto v = used_variables[i];

v->binary = _v->binary;
v->integer = _v->integer;
v->lb = _v->lb;
v->ub = _v->ub;
v->fixed = _v->fixed;
v->value = _v->value;
}

Constraint NLPModelRepn::get_constraint(int i)
{
return model.repn->constraints[i];
}

void NLPModelRepn::print_equations(std::ostream& ostr) const
{
ostr << "NLPModel:" << std::endl;
ostr << "  variables:         " << num_variables() << std::endl;
ostr << "  all variables:     " << used_variables.size() << std::endl;
ostr << "  fixed variables:   " << fixed_variables.size() << std::endl;
ostr << "  parameters:        " << parameters.size() << std::endl;
ostr << "  objectives:        " << num_objectives() << std::endl;
ostr << "  constraints:       " << num_constraints() << std::endl;
ostr << "  nonzeros Jacobian: " << num_nonzeros_Jacobian() << std::endl;
ostr << "  nonzeros Hessian:  " << num_nonzeros_Hessian_Lagrangian() << std::endl;
ostr << std::endl;
ostr << model;
ostr << std::endl;
}

void NLPModelRepn::print_values(std::ostream& ostr) const
{}

}