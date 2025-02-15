#pragma once

#include "coek/ast/assoc_array_base.hpp"
#include "coek/api/expression.hpp"

namespace coek {

class VariableTerm;

class VariableAssocArrayRepn : public AssocArrayBase<VariableTerm> {
   public:
    std::vector<Variable> values;
    Variable value_template;

   public:
    VariableAssocArrayRepn();

    virtual void expand();

    /** Set the initial variable value. \returns the variable object. */
    void value(double value);
    /** Set the initial variable value. \returns the variable object. */
    void value(const Expression& value);

    /** Set the lower bound. \returns the variable object. */
    void lower(double value);
    /** Set the lower bound. \returns the variable object. */
    void lower(const Expression& value);

    /** Set the upper bound. \returns the variable object. */
    void upper(double value);
    /** Set the upper bound. \returns the variable object. */
    void upper(const Expression& value);

    /** Set the upper and lower bounds. \returns the variable object. */
    void bounds(double lb, double ub);
    /** Set the upper and lower bounds. \returns the variable object. */
    void bounds(const Expression& lb, double ub);
    /** Set the upper and lower bounds. \returns the variable object. */
    void bounds(double lb, const Expression& ub);
    /** Set the upper and lower bounds. \returns the variable object. */
    void bounds(const Expression& lb, const Expression& ub);

    /** Set a flag indicating if the variable is fixed. */
    void fixed(bool flag);

    /** Set the name of the variable. \returns the variable object */
    void name(const std::string& name);
    /** Get the name of the variable. */
    std::string name();

    /** Set the variable type. \returns the variable object */
    void within(VariableTypes vtype);

#ifdef COEK_WITH_COMPACT_MODEL
    expr_pointer_t create_ref(const std::vector<refarg_types>& indices);
#endif
};

}  // namespace coek
