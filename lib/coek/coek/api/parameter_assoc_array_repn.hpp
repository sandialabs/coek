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

    /** Set the initial parameter value. */
    void value(double value);
    /** Set the initial parameter value. */
    void value(const Expression& value);

    /** Set the name of the parameter. */
    void name(const std::string& name);

    /** Get the name of the parameter. */
    std::string name();

#ifdef COEK_WITH_COMPACT_MODEL
    expr_pointer_t create_ref(const std::vector<refarg_types>& indices);
#endif
};

}  // namespace coek
