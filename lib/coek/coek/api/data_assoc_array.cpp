#include <cmath>
#include <variant>
#include "coek/ast/value_terms.hpp"
#include "coek/api/data_assoc_array.hpp"

namespace coek {

size_t DataAssocArray::size() const { return get_repn()->size(); }

size_t DataAssocArray::dim() const { return get_repn()->dim(); }

std::vector<std::shared_ptr<ConstantTerm>>::iterator DataAssocArray::begin() { return get_repn()->values.begin(); }

std::vector<std::shared_ptr<ConstantTerm>>::iterator DataAssocArray::end() { return get_repn()->values.end(); }

void DataAssocArray::expand() { return get_repn()->expand(); }

#ifdef COEK_WITH_COMPACT_MODEL
expr_pointer_t create_ref(const std::vector<refarg_types>& indices, const std::string& name,
                          std::shared_ptr<AssocArrayBase<ConstantTerm>> var);

Expression DataAssocArray::create_ref(const std::vector<refarg_types>& args)
{
    auto repn = get_repn();
    return coek::create_ref(args, repn->value_template.name(), repn);
}
#endif

}  // namespace coek
