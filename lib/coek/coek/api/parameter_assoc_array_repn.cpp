#include "coek/ast/value_terms.hpp"
#include "coek/api/parameter_assoc_array_repn.hpp"

namespace coek {

ParameterAssocArrayRepn::ParameterAssocArrayRepn() { value_template.name("p"); }

void ParameterAssocArrayRepn::expand()
{
    if (first_expand) {
        auto value
            = std::make_shared<ConstantTerm>(value_template.value_expression().expand().value());
        // expand_expr(value_template.->value)->eval());
        for (size_t i = 0; i < size(); i++) {
            values.emplace_back(CREATE_POINTER(ParameterTerm, value));
        }
        first_expand = false;
    }
}

void ParameterAssocArrayRepn::value(double value)
{
    value_template.value(value);
    if (values.size() > 0) {
        Expression e(value);
        for (auto& var : values)
            var.value(e.repn);
    }
}

void ParameterAssocArrayRepn::value(const Expression& value)
{
    value_template.value(value);
    if (values.size() > 0) {
        for (auto& var : values)
            var.value(value);
    }
}

void ParameterAssocArrayRepn::name(const std::string& name)
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

std::string ParameterAssocArrayRepn::name() { return value_template.name(); }

}  // namespace coek
