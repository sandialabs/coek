#include "coek/ast/value_terms.hpp"
#include "coek/api/data_assoc_array_repn.hpp"

namespace coek {

DataAssocArrayRepn::DataAssocArrayRepn() { value_template.name("d"); }

void DataAssocArrayRepn::expand()
{
    if (first_expand) {
        for (size_t i = 0; i < size(); i++) {
            values.emplace_back(
                CREATE_POINTER(ConstantTerm, value_template.value_expression().expand().value()));
        }
        first_expand = false;
    }
}

void DataAssocArrayRepn::value(double value)
{
    value_template.value(value);
    if (values.size() > 0) {
        for (auto& var : values)
            var->value = value;
    }
}

void DataAssocArrayRepn::value(const Expression& expr)
{
    value_template.value(expr);
    if (values.size() > 0) {
        for (auto& var : values)
            var->value = expr.value();
    }
}

void DataAssocArrayRepn::name(const std::string& name) { value_template.name(name); }

std::string DataAssocArrayRepn::name() { return value_template.name(); }

}  // namespace coek
