#pragma once

#include "coek/api/variable_assoc_array_repn.hpp"


namespace coek {

class VariableAssocArray
{
public:

    virtual VariableAssocArrayRepn* get_repn() = 0;
    virtual const VariableAssocArrayRepn* get_repn() const = 0;
    IndexVector tmp;
    std::vector<refarg_types> reftmp;

public:

    std::vector<Variable>::iterator begin();
    std::vector<Variable>::iterator end();

    size_t size() const;
    size_t dim() const;

    virtual Variable index(const IndexVector& args) = 0;
    Expression create_varref(const std::vector<refarg_types>& indices);
};

}
