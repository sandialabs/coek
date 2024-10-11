#pragma once

#include <cassert>
#include <string>
#include <variant>
#include <vector>
#include <memory>

// #include "coek/api/expression.hpp"
#include "coek/util/index_vector.hpp"

namespace coek {

class BaseExpressionTerm;

typedef std::shared_ptr<BaseExpressionTerm> expr_pointer_t;
typedef std::variant<int, expr_pointer_t> refarg_types;

template <class TYPE>
class AssocArrayBase {
   public:
#ifdef CUSTOM_INDEXVECTOR
    IndexVectorCache cache;
#endif
    // std::vector<std::shared_ptr<TYPE>> values;
    // std::shared_ptr<TYPE> value_template;

    IndexVector tmp;
    std::vector<refarg_types> reftmp;

    bool first_expand = true;

   public:
    virtual void expand() = 0;
    virtual void generate_names() = 0;

    virtual size_t dim() = 0;
    virtual size_t size() = 0;

    void resize_index_vectors(IndexVector& tmp_, std::vector<refarg_types>& reftmp_)
    {
        auto dim_ = dim();
#ifdef CUSTOM_INDEXVECTOR
        tmp_ = cache.alloc(dim_);
        tmp = cache.alloc(dim_);
#else
        tmp_.resize(dim_);
        tmp.resize(dim_);
#endif
        reftmp_.resize(dim_);
        reftmp.resize(dim_);
    }

    virtual std::shared_ptr<TYPE> index(const IndexVector& args) = 0;
};

}  // namespace coek
