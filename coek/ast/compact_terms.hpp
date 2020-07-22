#pragma once

#include <variant>
#include <vector>
#include "base_terms.hpp"
#include "value_terms.hpp"


namespace coek {

//typedef std::variant<int,double,std::string,expr_pointer_t> refarg_types;
typedef std::variant<int,expr_pointer_t> refarg_types;

#if 0
//
// IndexParameterTerm
//

class IndexParameterTerm : public BaseExpressionTerm
{
public:

    std::string name;
    std::variant<int, double, std::string> value;

    IndexParameterTerm(const std::string& _name, int refcount=0)
        : BaseExpressionTerm(refcount), name(_name)
        {non_variable=true;}

    bool is_abstract_parameter() const
        {return true;}

    double eval() const
        {
        if (auto ival = std::get_if<int>(&value))
            return *ival;
        if (auto dval = std::get_if<double>(&value))
            return *dval;
        throw std::runtime_error("Accessing the value of a non-numeric abstract parameter");
        }

    expr_pointer_t negate(const expr_pointer_t& repn);

    void accept(Visitor& v)
        { v.visit(*this); }
    term_id id()
        {return IndexParameterTerm_id;}
};
#endif

#if 0
class IndexedVariableTerm : public VariableTerm
{
public:

    void* var;      // ConcreteIndexedVariableRepn
    unsigned int vindex;

    IndexedVariableTerm(double _lb, double _ub, double _value, bool _binary, bool _integer, unsigned int _vindex, void* _var)
        : VariableTerm(_lb, _ub, _value, _binary, _integer), var(_var), vindex(_vindex) {}

    virtual std::string get_name();

    void accept(Visitor& v)
        { v.visit(*this); }
    term_id id()
        {return IndexedVariableTerm_id;}
};
#endif


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
