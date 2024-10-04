#include "coek/ast/value_terms.hpp"
#include "coek/api/variable_assoc_array_repn.hpp"

namespace coek {

VariableAssocArrayRepn::VariableAssocArrayRepn() {}

void VariableAssocArrayRepn::expand()
{
    if (first_expand) {
        auto vtype = value_template.within();
        bool binary = (vtype == Boolean) or (vtype == Binary);
        bool integer = vtype == Integers;
        auto lower
            = std::make_shared<ConstantTerm>(value_template.lower_expression().expand().value());
        auto upper
            = std::make_shared<ConstantTerm>(value_template.upper_expression().expand().value());
        auto value
            = std::make_shared<ConstantTerm>(value_template.value_expression().expand().value());
        auto size_ = size();
        for (size_t i = 0; i < size_; i++) {
            values.emplace_back(CREATE_POINTER(VariableTerm, lower, upper, value, binary, integer));
        }
        first_expand = false;
    }
}

void VariableAssocArrayRepn::value(double value)
{
    value_template.value(value);
    if (values.size() > 0) {
        coek::Expression e(value);
        for (auto& var : values)
            var.value(e);
    }
}

void VariableAssocArrayRepn::value(const Expression& value)
{
    value_template.value(value);
    if (values.size() > 0) {
        for (auto& var : values)
            var.value(value);
    }
}

void VariableAssocArrayRepn::lower(double value)
{
    value_template.lower(value);
    if (values.size() > 0) {
        coek::Expression e(value);
        for (auto& var : values)
            var.lower(e);
    }
}

void VariableAssocArrayRepn::lower(const Expression& value)
{
    value_template.lower(value);
    if (values.size() > 0) {
        for (auto& var : values)
            var.lower(value);
    }
}

void VariableAssocArrayRepn::upper(double value)
{
    value_template.upper(value);
    if (values.size() > 0) {
        coek::Expression e(value);
        for (auto& var : values)
            var.upper(e);
    }
}

void VariableAssocArrayRepn::upper(const Expression& value)
{
    value_template.upper(value);
    if (values.size() > 0) {
        for (auto& var : values)
            var.upper(value);
    }
}

void VariableAssocArrayRepn::bounds(double lb, double ub)
{
    value_template.bounds(lb, ub);
    if (values.size() > 0) {
        coek::Expression lower(lb);
        coek::Expression upper(ub);
        for (auto& var : values)
            var.bounds(lower, upper);
    }
}

void VariableAssocArrayRepn::bounds(const Expression& lb, double ub)
{
    value_template.bounds(lb, ub);
    if (values.size() > 0) {
        coek::Expression upper(ub);
        for (auto& var : values)
            var.bounds(lb, upper);
    }
}

void VariableAssocArrayRepn::bounds(double lb, const Expression& ub)
{
    value_template.bounds(lb, ub);
    if (values.size() > 0) {
        coek::Expression lower(lb);
        for (auto& var : values)
            var.bounds(lower, ub);
    }
}

void VariableAssocArrayRepn::bounds(const Expression& lb, const Expression& ub)
{
    value_template.bounds(lb, ub);
    if (values.size() > 0) {
        for (auto& var : values)
            var.bounds(lb, ub);
    }
}

void VariableAssocArrayRepn::fixed(bool value)
{
    value_template.fixed(value);
    if (values.size() > 0) {
        for (auto& var : values)
            var.fixed(value);
    }
}

void VariableAssocArrayRepn::name(const std::string& name)
{
    value_template.name(name);
    if (values.size() > 0) {
        // If the string is empty, then we reset the names of all variables
        if (name.size() == 0) {
            for (auto& var : values)
                var.name(name);
        }
        // Otherwise, we re-generate the names
        else
            generate_names();
    }
}

std::string VariableAssocArrayRepn::name() { return value_template.name(); }

void VariableAssocArrayRepn::within(VariableTypes vtype)
{
    value_template.within(vtype);
    if (values.size() > 0) {
        for (auto& var : values)
            var.within(vtype);
    }
}

}  // namespace coek
