#include "autograd.hpp"

#include <cmath>
#include <unordered_set>

#include "../ast/base_terms.hpp"
#include "../ast/constraint_terms.hpp"
#include "../ast/value_terms.hpp"
#include "../ast/visitor_fns.hpp"
#include "coek/api/constraint.hpp"
#include "coek/api/objective.hpp"
#include "coek/model/model_repn.hpp"

#ifdef WITH_CPPAD
#    include "cppad_repn.hpp"
#endif
#include "unknownad_repn.hpp"

namespace coek {

void check_that_expression_variables_are_declared(Model& model,
                                                  const std::unordered_set<VariableTerm*>& vars);

NLPModelRepn* create_NLPModelRepn(Model& model, const std::string& name)
{
#ifdef WITH_CPPAD
    if (name == "cppad") return new CppAD_Repn(model);
#endif

    throw std::runtime_error("Unexpected NLP model type: " + name);
}

NLPModelRepn* create_NLPModelRepn(const std::string& name) { return new UnknownAD_Repn(name); }

void NLPModelRepn::find_used_variables()
{
    std::unordered_set<VariableTerm*> vars;
    std::set<VariableTerm*> fixed_vars;
    std::set<ParameterTerm*> params;

    for (auto it = model.repn->objectives.begin(); it != model.repn->objectives.end(); ++it)
        find_vars_and_params(it->repn, vars, fixed_vars, params);
    for (auto it = model.repn->constraints.begin(); it != model.repn->constraints.end(); ++it)
        find_vars_and_params(it->repn, vars, fixed_vars, params);

    check_that_expression_variables_are_declared(model, vars);

    std::map<size_t, VariableTerm*> tmp;
    for (auto it = vars.begin(); it != vars.end(); ++it) tmp[(*it)->index] = *it;

    used_variables.clear();
    size_t i = 0;
    for (auto it = tmp.begin(); it != tmp.end(); ++it) used_variables[i++] = it->second;

    i = 0;
    fixed_variables.clear();
    for (auto it = fixed_vars.begin(); it != fixed_vars.end(); ++it) fixed_variables[*it] = i++;
    parameters.clear();
    for (auto it = params.begin(); it != params.end(); ++it) parameters[*it] = i++;
}

VariableTerm* NLPModelRepn::get_variable(size_t i) { return used_variables[i]; }

void NLPModelRepn::set_variable(size_t i, const VariableTerm* _v)
{
    auto v = used_variables[i];

    v->binary = _v->binary;
    v->integer = _v->integer;
    v->lb = _v->lb;
    v->ub = _v->ub;
    v->fixed = _v->fixed;
    v->value = _v->value;
}

Objective NLPModelRepn::get_objective(size_t i) { return model.get_objective(i); }

Constraint NLPModelRepn::get_constraint(size_t i) { return model.get_constraint(i); }

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
{
    ostr << "Model Variables: " << num_variables() << "\n";
    ostr << "Nonzero Variables\n";
    for (auto const& var : used_variables) {
        double val = var.second->eval();
        if (::fabs(val) > 1e-7) {
            ostr << "   " << var.first << " " << val << " " << var.second->fixed << "\n";
        }
    }
}

}  // namespace coek
