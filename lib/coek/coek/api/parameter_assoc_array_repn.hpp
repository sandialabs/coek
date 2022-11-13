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
    IndexVectorCache cache;
    std::vector<Parameter> values;
    Parameter parameter_template;
    bool call_setup;

   public:
    ParameterAssocArrayRepn();

    virtual void setup();

    virtual size_t dim() = 0;
    virtual size_t size() = 0;

    void resize_index_vectors(IndexVector& tmp, std::vector<refarg_types>& reftmp);

    virtual std::string get_name(size_t index) = 0;

    /** Set the initial variable value. \returns the variable object. */
    void value(double value);
    /** Set the initial variable value. \returns the variable object. */
    void value(const Expression& value);

    /** Set the name of the variable. \returns the variable object */
    void name(const std::string& name);
};

}  // namespace coek
