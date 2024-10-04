#include <cmath>
#include <variant>
#include "coek/ast/value_terms.hpp"
#include "coek/api/parameter_assoc_array.hpp"

namespace coek {

size_t ParameterAssocArray::size() const { return get_repn()->size(); }

size_t ParameterAssocArray::dim() const { return get_repn()->dim(); }

std::vector<Parameter>::iterator ParameterAssocArray::begin() { return get_repn()->values.begin(); }

std::vector<Parameter>::iterator ParameterAssocArray::end() { return get_repn()->values.end(); }

#ifdef COEK_WITH_COMPACT_MODEL
expr_pointer_t create_ref(const std::vector<refarg_types>& indices, const std::string& name,
                          std::shared_ptr<AssocArrayBase<ParameterTerm>> var);

Expression ParameterAssocArray::create_ref(const std::vector<refarg_types>& args)
{
    auto repn = get_repn();
    return coek::create_ref(args, repn->value_template.name(), repn);
}
#endif

}  // namespace coek
