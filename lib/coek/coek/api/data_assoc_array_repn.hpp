#pragma once

#include <memory>
#include "coek/ast/assoc_array_base.hpp"
#include "coek/api/expression.hpp"

namespace coek {

class ConstantTerm;

class DataAssocArrayRepn : public AssocArrayBase<ConstantTerm> {
   public:
    std::vector<std::shared_ptr<ConstantTerm>> values;
    Parameter value_template;

    void generate_names() {}

   public:
    DataAssocArrayRepn();

    virtual void expand();

    /** Set the initial data value. */
    void value(double value);
    /** Set the initial data value. */
    void value(const Expression& value);

    /** Set the name of the data. */
    void name(const std::string& name);

    /** Get the name of the data. */
    std::string name();

#ifdef COEK_WITH_COMPACT_MODEL
    expr_pointer_t create_ref(const std::vector<refarg_types>& indices);
#endif
};

}  // namespace coek
