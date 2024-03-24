
#include <cmath>
#include <variant>

#include "coek/api/variable_assoc_array.hpp"
#include "coek/ast/compact_terms.hpp"

namespace coek {

expr_pointer_t create_varref(const std::vector<refarg_types>& indices, const std::string& name,
                             void* var);

//
// VariableAssocArrayRepn
//

VariableAssocArrayRepn::VariableAssocArrayRepn() {}

void VariableAssocArrayRepn::resize_index_vectors(IndexVector& tmp,
                                                  std::vector<refarg_types>& reftmp)
{
    tmp = cache.alloc(dim());
    reftmp.resize(dim());
}

void VariableAssocArrayRepn::setup()
{
    if (first_setup) {
        auto vtype = variable_template.within();
        bool binary = (vtype == Boolean) or (vtype == Binary);
        bool integer = vtype == Integers;
        auto lower
            = std::make_shared<ConstantTerm>(variable_template.lower_expression().expand().value());
        auto upper
            = std::make_shared<ConstantTerm>(variable_template.upper_expression().expand().value());
        auto value
            = std::make_shared<ConstantTerm>(variable_template.value_expression().expand().value());
        for (size_t i = 0; i < size(); i++) {
            values.emplace_back(CREATE_POINTER(VariableTerm, lower, upper, value, binary, integer));
        }
        first_setup = false;
    }
}

void VariableAssocArrayRepn::value(double value)
{
    variable_template.value(value);
    if (values.size() > 0) {
        for (auto& var : values) var.value(value);
    }
}

void VariableAssocArrayRepn::value(const Expression& value)
{
    variable_template.value(value);
    if (values.size() > 0) {
        for (auto& var : values) var.value(value);
    }
}

void VariableAssocArrayRepn::lower(double value)
{
    variable_template.lower(value);
    if (values.size() > 0) {
        for (auto& var : values) var.lower(value);
    }
}

void VariableAssocArrayRepn::lower(const Expression& value)
{
    variable_template.lower(value);
    if (values.size() > 0) {
        for (auto& var : values) var.lower(value);
    }
}

void VariableAssocArrayRepn::upper(double value)
{
    variable_template.upper(value);
    if (values.size() > 0) {
        for (auto& var : values) var.upper(value);
    }
}

void VariableAssocArrayRepn::upper(const Expression& value)
{
    variable_template.upper(value);
    if (values.size() > 0) {
        for (auto& var : values) var.upper(value);
    }
}

void VariableAssocArrayRepn::bounds(double lb, double ub)
{
    variable_template.bounds(lb, ub);
    if (values.size() > 0) {
        for (auto& var : values) var.bounds(lb, ub);
    }
}

void VariableAssocArrayRepn::bounds(const Expression& lb, double ub)
{
    variable_template.bounds(lb, ub);
    if (values.size() > 0) {
        for (auto& var : values) var.bounds(lb, ub);
    }
}

void VariableAssocArrayRepn::bounds(double lb, const Expression& ub)
{
    variable_template.bounds(lb, ub);
    if (values.size() > 0) {
        for (auto& var : values) var.bounds(lb, ub);
    }
}

void VariableAssocArrayRepn::bounds(const Expression& lb, const Expression& ub)
{
    variable_template.bounds(lb, ub);
    if (values.size() > 0) {
        for (auto& var : values) var.bounds(lb, ub);
    }
}

void VariableAssocArrayRepn::fixed(bool value)
{
    variable_template.fixed(value);
    if (values.size() > 0) {
        for (auto& var : values) var.fixed(value);
    }
}

void VariableAssocArrayRepn::name(const std::string& name)
{
    variable_template.name(name);
    if (values.size() > 0) {
        // If the string is empty, then we reset the names of all variables
        if (name.size() == 0) {
            for (auto& var : values) var.name(name);
        }
        // Otherwise, we re-generate the names
        else
            generate_names();
    }
}

void VariableAssocArrayRepn::within(VariableTypes vtype)
{
    variable_template.within(vtype);
    if (values.size() > 0) {
        for (auto& var : values) var.within(vtype);
    }
}

//
// VariableAssocArray
//

size_t VariableAssocArray::size() const { return get_repn()->size(); }

size_t VariableAssocArray::dim() const { return get_repn()->dim(); }

std::vector<Variable>::iterator VariableAssocArray::begin() { return get_repn()->values.begin(); }

std::vector<Variable>::iterator VariableAssocArray::end() { return get_repn()->values.end(); }

#ifdef COEK_WITH_COMPACT_MODEL
Expression VariableAssocArray::create_varref(const std::vector<refarg_types>& args)
{
    return coek::create_varref(args, get_repn()->variable_template.name(), this);
}
#endif

//
// OTHER
//

#ifdef COEK_WITH_COMPACT_MODEL
expr_pointer_t get_concrete_var(VariableRefTerm& varref)
{
    VariableAssocArray* var = static_cast<VariableAssocArray*>(varref.var);

    std::vector<int> index;
    for (auto& reftmp : varref.indices) {
        if (auto ival = std::get_if<int>(&reftmp))
            index.push_back(*ival);
        else {
            expr_pointer_t eval = std::get<expr_pointer_t>(reftmp);
            double vald = eval->eval();
            long int vali = std::lround(vald);
            assert(fabs(vald - vali) < 1e-7);
            index.push_back(static_cast<int>(vali));
        }
    }

    IndexVector& tmp = var->tmp;
    for (size_t i = 0; i < index.size(); i++) tmp[i] = index[i];

    Expression e = var->index(tmp);
    return e.repn;
}
#endif

}  // namespace coek
