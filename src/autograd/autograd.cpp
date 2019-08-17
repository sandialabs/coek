
#include "expr/ast_visitors.hpp"
#include "autograd/autograd.hpp"

#ifdef WITH_CPPAD
#include "autograd/cppad_repn.hpp"
#endif

namespace coek {


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

for (auto it=model.objectives.begin(); it != model.objectives.end(); ++it)
    find_vars_and_params(it->repn, vars, fixed_vars, params);
for (auto it=model.constraints.begin(); it != model.constraints.end(); ++it)
    find_vars_and_params(it->repn, vars, fixed_vars, params);

//std::cout << "HERE x " << vars.size() << std::endl;
std::map<int,VariableTerm*> tmp;
for (auto it=vars.begin(); it != vars.end(); ++it)
    tmp[(*it)->index] = *it;

used_variables.clear();
int i=0;
for (auto it=tmp.begin(); it != tmp.end(); ++it)
    used_variables[i++] = it->second;
//std::cout << "HERE y " << used_variables.size() << std::endl << std::flush;

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
v->initialize = _v->initialize;
v->value = _v->value;
}

}
