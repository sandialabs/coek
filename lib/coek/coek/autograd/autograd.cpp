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
#ifdef WITH_ASL
#    include "asl_repn.hpp"
#endif
#include "unknownad_repn.hpp"

namespace coek {

void check_that_expression_variables_are_declared(
    Model& model, const std::unordered_set<std::shared_ptr<VariableTerm>>& vars);

NLPModelRepn* create_NLPModelRepn(Model& model, const std::string& name)
{
#ifdef WITH_CPPAD
    if (name == "cppad")
        return new CppAD_Repn(model);
#endif
#ifdef WITH_ASL
    if (name == "asl")
        return new ASL_Repn(model);
#endif

    throw std::runtime_error("Unexpected NLP model type: " + name);
}

NLPModelRepn* create_NLPModelRepn(const std::string& name) { return new UnknownAD_Repn(name); }

void NLPModelRepn::find_used_variables()
{
    std::unordered_set<VariableRepn> vars;
    std::set<VariableRepn> fixed_vars;
    std::set<ParameterRepn> params;
    std::set<std::shared_ptr<SubExpressionTerm>> visited_subexpressions;

    for (auto& it : model.repn->objectives)
        find_vars_and_params(it.repn, vars, fixed_vars, params, visited_subexpressions);
    for (auto& it : model.repn->constraints)
        find_vars_and_params(it.repn, vars, fixed_vars, params, visited_subexpressions);

    check_that_expression_variables_are_declared(model, vars);

    std::map<size_t, std::shared_ptr<VariableTerm>> tmp;
    for (auto& it : vars)
        tmp[it->index] = it;

    used_variables.clear();
    size_t i = 0;
    for (auto& it : tmp) {
        // std::cout << "DEBUG USED VARS  " << i << " " << it.first << " " << it.second->get_name()
        // << std::endl;
        used_variables[i++] = it.second;
    }

    fixed_variables.clear();
    parameters.clear();
    size_t j = 0;
    for (auto& it : fixed_vars)
        fixed_variables[it] = j++;
    for (auto& it : params)
        parameters[it] = j++;
}

VariableRepn NLPModelRepn::get_variable(size_t i) { return used_variables[i]; }

void NLPModelRepn::set_variable(size_t i, const VariableRepn _v)
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

bool NLPModelRepn::has_constraint_lower(size_t i) { return model.get_constraint(i).has_lower(); }

bool NLPModelRepn::has_constraint_upper(size_t i) { return model.get_constraint(i).has_upper(); }

double NLPModelRepn::get_constraint_lower(size_t i)
{
    return model.get_constraint(i).lower().value();
}

double NLPModelRepn::get_constraint_upper(size_t i)
{
    return model.get_constraint(i).upper().value();
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
{
    ostr << "Model Variables: " << num_variables() << "\n";
    ostr << "   (<Index>: <Name> <Value> <LB> <UB> <Fixed>)\n";
    size_t ctr = 0;
    for (auto const& var : used_variables) {
        double val = var.second->eval();
        ostr << "   " << ctr << ": " << var.second->get_name() << " " << val << " "
             << var.second->lb->eval() << " " << var.second->ub->eval() << " " << var.second->fixed
             << "\n";
        ctr++;
    }
}

}  // namespace coek
