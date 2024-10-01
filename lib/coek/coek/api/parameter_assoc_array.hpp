#pragma once

#include "coek/api/parameter_assoc_array_repn.hpp"

namespace coek {

class ParameterAssocArray {
   public:
    virtual std::shared_ptr<ParameterAssocArrayRepn> get_repn() = 0;
    IndexVector tmp;
    std::vector<refarg_types> reftmp;

   public:
    std::vector<Parameter>::iterator begin();
    std::vector<Parameter>::iterator end();

    size_t size();
    size_t dim();

    // virtual Parameter index(const IndexVector& args) = 0;
#ifdef COEK_WITH_COMPACT_MODEL
    Expression create_paramref(const std::vector<refarg_types>& indices);
#endif
};

}  // namespace coek
