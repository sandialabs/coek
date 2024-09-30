#pragma once

#include "coek/api/variable_assoc_array_repn.hpp"

namespace coek {

class VariableAssocArray {
   public:
    virtual std::shared_ptr<VariableAssocArrayRepn> get_repn() = 0;
    virtual const std::shared_ptr<VariableAssocArrayRepn> get_repn() const = 0;
    IndexVector tmp;
    std::vector<refarg_types> reftmp;

   public:
    std::vector<Variable>::iterator begin();
    std::vector<Variable>::iterator end();

    size_t size() const;
    size_t dim() const;

    void expand();

    virtual Variable index(const IndexVector& args) = 0;
#ifdef COEK_WITH_COMPACT_MODEL
    Expression create_varref(const std::vector<refarg_types>& indices);
#endif
};

}  // namespace coek
