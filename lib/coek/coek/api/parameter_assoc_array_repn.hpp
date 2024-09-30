#pragma once

#include <cassert>
#include <string>
#include <variant>
#include <vector>

#include "coek/api/expression.hpp"
#include "coek/util/index_vector.hpp"

namespace coek {

class BaseExpressionTerm;

typedef std::shared_ptr<BaseExpressionTerm> expr_pointer_t;
typedef std::variant<int, expr_pointer_t> refarg_types;

class ParameterAssocArrayRepn {
   public:
    #ifdef CUSTOM_INDEXVECTOR
    IndexVectorCache cache;
    #endif
    std::vector<Parameter> values;
    Parameter parameter_template;

    IndexVector tmp;
    std::vector<refarg_types> reftmp;

    bool first_expand = true;

   public:
    ParameterAssocArrayRepn();

    virtual void expand();
    virtual void generate_names() = 0;

    virtual size_t dim() = 0;
    virtual size_t size() = 0;

    void resize_index_vectors(IndexVector& tmp, std::vector<refarg_types>& reftmp);

    /** Set the initial variable value. */
    void value(double value);
    /** Set the initial variable value. */
    void value(const Expression& value);

    /** Set the name of the variable. */
    void name(const std::string& name);

    virtual Parameter index(const IndexVector& args) = 0;

#ifdef COEK_WITH_COMPACT_MODEL
    Expression create_paramref(const std::vector<refarg_types>& indices);
#endif
};

}  // namespace coek
