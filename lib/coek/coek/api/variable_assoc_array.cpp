#include <cmath>
#include <variant>
#include "coek/ast/value_terms.hpp"
#include "coek/api/variable_assoc_array.hpp"

namespace coek {

size_t VariableAssocArray::size() const { return get_repn()->size(); }

size_t VariableAssocArray::dim() const { return get_repn()->dim(); }

void VariableAssocArray::expand() { return get_repn()->expand(); }

std::vector<Variable>::iterator VariableAssocArray::begin() { return get_repn()->values.begin(); }

std::vector<Variable>::iterator VariableAssocArray::end() { return get_repn()->values.end(); }

#ifdef COEK_WITH_COMPACT_MODEL
expr_pointer_t create_varref(const std::vector<refarg_types>& indices, const std::string& name,
                             std::shared_ptr<AssocArrayBase<VariableTerm>> var);

Expression VariableAssocArray::create_varref(const std::vector<refarg_types>& args)
{
    auto repn = get_repn();
    return coek::create_varref(args, get_repn()->value_template.name(), repn);
}
#endif

}  // namespace coek
