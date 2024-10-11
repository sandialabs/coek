#pragma once

#include "coek/api/data_assoc_array_repn.hpp"

namespace coek {

class DataAssocArray {
   public:
    virtual std::shared_ptr<DataAssocArrayRepn> get_repn() = 0;
    virtual const std::shared_ptr<DataAssocArrayRepn> get_repn() const = 0;
    IndexVector tmp;
    std::vector<refarg_types> reftmp;

   public:
    std::vector<std::shared_ptr<ConstantTerm>>::iterator begin();
    std::vector<std::shared_ptr<ConstantTerm>>::iterator end();

    size_t size() const;
    size_t dim() const;

    void expand();

    // virtual data index(const IndexVector& args) = 0;
#ifdef COEK_WITH_COMPACT_MODEL
    Expression create_ref(const std::vector<refarg_types>& indices);
#endif
};

}  // namespace coek
