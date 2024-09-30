#pragma once

#include "coek/ast/assoc_array_base.hpp"
#include "coek/api/expression.hpp"

namespace coek {

class ParameterTerm;

class ParameterAssocArrayRepn : public AssocArrayBase<ParameterTerm> {
   public:

    std::vector<Parameter> values;
    Parameter value_template;

   public:

    ParameterAssocArrayRepn();

    virtual void expand();

    /** Set the initial variable value. */
    void value(double value);
    /** Set the initial variable value. */
    void value(const Expression& value);

    /** Set the name of the variable. */
    void name(const std::string& name);

#ifdef COEK_WITH_COMPACT_MODEL
    expr_pointer_t create_paramref(const std::vector<refarg_types>& indices);
#endif
};

}  // namespace coek
