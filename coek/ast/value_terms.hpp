#pragma once

#include <variant>
#include "base_terms.hpp"


namespace coek {

#if 0
//
// ConstantTerm
//

class ConstantTerm : public BaseExpressionTerm
{
public:

    double value;

    ConstantTerm(double _value, int refcount=0)
        : BaseExpressionTerm(refcount), value(_value)
        {non_variable=true;}

    double eval() const
        { return value; }

    bool is_constant() const
        {return true;}
    bool is_parameter() const
        {return true;}

    expr_pointer_t negate(const expr_pointer_t& repn);

    void accept(Visitor& v)
        { v.visit(*this); }
    term_id id()
        {return ConstantTerm_id;}
};
#endif

//
// ParameterTerm
//

class ParameterTerm : public ConstantTerm
{
public:

    std::string name;

    ParameterTerm(double _value)
        : ConstantTerm(_value)
        {}

    bool is_constant() const
        {return false;}

    expr_pointer_t negate(const expr_pointer_t& repn);

    void accept(Visitor& v)
        { v.visit(*this); }
    term_id id()
        {return ParameterTerm_id;}
};

//
// IndexParameterTerm
//

class IndexParameterTerm : public BaseExpressionTerm
{
public:

    std::string name;
    int type;
    double double_value;
    int int_value;
    std::string string_value;

    IndexParameterTerm(const std::string& _name, int refcount=0)
        : BaseExpressionTerm(refcount), name(_name)
        {
        non_variable=true;
        type = 0;
        }

    bool is_abstract_parameter() const
        {return true;}

    double eval() const;

    void set_value(double value);
    void set_value(int value);
    void set_value(const std::string& value);
    void get_value(double& value);
    void get_value(int& value);
    void get_value(std::string& value);

    expr_pointer_t negate(const expr_pointer_t& repn);

    void accept(Visitor& v)
        { v.visit(*this); }
    term_id id()
        {return IndexParameterTerm_id;}
};

//
// BaseVariableTerm
//

class BaseVariableTerm : public BaseExpressionTerm
{
};

//
// VariableTerm
//

class VariableTerm : public BaseVariableTerm
{
public:

    static unsigned int count;

public:

    unsigned int index;
    double value;
    double lb;
    double ub;
    bool binary;
    bool integer;
    bool fixed;
    bool indexed;
    std::string name;

    VariableTerm(double _lb, double _ub, double _value, bool _binary, bool _integer, bool _indexed=false);

    double eval() const
        { return value; }

    bool is_variable() const
        {return true;}

    expr_pointer_t const_mult(double coef, const expr_pointer_t& repn);
    expr_pointer_t negate(const expr_pointer_t& repn);

    void accept(Visitor& v)
        { v.visit(*this); }
    term_id id()
        {return VariableTerm_id;}

    virtual std::string get_simple_name()
        {
            return "x(" + std::to_string(index) + ")";
        }
    virtual std::string get_name()
        {
        if (name == "")
            return get_simple_name();
        else if (indexed)
            return name + "(" + std::to_string(index) + ")";
        else
            return name;
        }
};

class IndexedVariableTerm : public VariableTerm
{
public:

    void* var;      // ConcreteIndexedVariableRepn
    unsigned int vindex;

    IndexedVariableTerm(double _lb, double _ub, double _value, bool _binary, bool _integer, unsigned int _vindex, void* _var)
        : VariableTerm(_lb, _ub, _value, _binary, _integer), var(_var), vindex(_vindex) {}

    #if COEK_WITH_COMPACT_MODEL
    // See coek_indexed.cpp
    virtual std::string get_name();
    #else
    virtual std::string get_name()
        {return "Unknown";}
    #endif

    void accept(Visitor& v)
        { v.visit(*this); }
    term_id id()
        {return IndexedVariableTerm_id;}
};

//
// MonomialTerm
//

class MonomialTerm : public BaseExpressionTerm
{
public:

    double coef;
    VariableTerm* var;

    MonomialTerm(double lhs, VariableTerm* rhs);
    ~MonomialTerm();

    double eval() const
        { return coef * var->value; }

    bool is_monomial() const
        {return true;}

    expr_pointer_t negate(const expr_pointer_t& repn);

    void accept(Visitor& v)
        { v.visit(*this); }
    term_id id()
        {return MonomialTerm_id;}
};

}
