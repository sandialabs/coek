#pragma once

#include <variant>
#include <vector>
#include "base_terms.hpp"
#include "value_terms.hpp"


namespace coek {

//typedef std::variant<int,double,std::string,expr_pointer_t> refarg_types;
typedef std::variant<int,expr_pointer_t> refarg_types;


class VariableRefTerm : public BaseVariableTerm
{
public:

    std::vector<refarg_types> indices;
    std::string name;
    void* var;

public:

    VariableRefTerm(const std::vector<refarg_types>& _indices, const std::string& _name, void* _var);
    ~VariableRefTerm();

    double eval() const
        { throw std::runtime_error("Cannot evaluate a VariableRefTerm"); }
    void accept(Visitor& v)
        { v.visit(*this); }
    term_id id()
        {return VariableRefTerm_id;}

};

class SetRefTerm : public BaseExpressionTerm
{
public:

    expr_pointer_t body;

public:

    SetRefTerm(const expr_pointer_t& repn);
    ~SetRefTerm();
};

}
