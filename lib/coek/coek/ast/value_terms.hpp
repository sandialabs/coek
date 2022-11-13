#pragma once

#include <string>

#include "base_terms.hpp"

namespace coek {

//
// BaseParameterTerm
//

class BaseParameterTerm : public BaseExpressionTerm {};

//
// ParameterTerm
//

class ParameterTerm : public BaseParameterTerm {
   public:
    expr_pointer_t value;
    unsigned int index;
    bool indexed;
    std::string name;

    ParameterTerm();
    explicit ParameterTerm(const expr_pointer_t& _value, bool _indexed = false);

    double eval() const { return value->eval(); }

    bool is_parameter() const { return true; }

    expr_pointer_t negate(const expr_pointer_t& repn);

    void accept(Visitor& v) { v.visit(*this); }
    term_id id() { return ParameterTerm_id; }

    void set_value(double val);
    void set_value(expr_pointer_t val);

    virtual std::string get_simple_name() { return "x[" + std::to_string(index) + "]"; }
    virtual std::string get_name()
    {
        if (name == "")
            return std::to_string(eval());
        else
            return name;
    }
};

class IndexedParameterTerm : public ParameterTerm {
   public:
    void* param;  // ConcreteIndexedParameterRepn
    size_t pindex;
    bool first = true;

    IndexedParameterTerm(const expr_pointer_t& _value, size_t _pindex, void* _param)
        : ParameterTerm(_value), param(_param), pindex(_pindex)
    {
    }

    std::string get_name();
};

//
// IndexParameterTerm
//

class IndexParameterTerm : public BaseExpressionTerm {
   public:
    std::string name;
    int type;
    double double_value;
    int int_value;
    std::string string_value;

    explicit IndexParameterTerm(const std::string& _name) : name(_name), type(0)
    {
        non_variable = true;
    }

    bool is_abstract_parameter() const { return true; }

    double eval() const;

    void set_value(double value);
    void set_value(int value);
    void set_value(const std::string& value);
    bool get_value(double& value);
    bool get_value(int& value);
    bool get_value(std::string& value);

    expr_pointer_t negate(const expr_pointer_t& repn);

    void accept(Visitor& v) { v.visit(*this); }
    term_id id() { return IndexParameterTerm_id; }
};

//
// BaseVariableTerm
//

class BaseVariableTerm : public BaseExpressionTerm {};

//
// VariableTerm
//

class VariableTerm : public BaseVariableTerm {
   public:
    static unsigned int count;
    static std::shared_ptr<ConstantTerm> negative_infinity;
    static std::shared_ptr<ConstantTerm> positive_infinity;
    static std::shared_ptr<ConstantTerm> nan;

   public:
    unsigned int index;
    expr_pointer_t value;
    expr_pointer_t lb;
    expr_pointer_t ub;
    bool binary;
    bool integer;
    bool fixed;
    bool indexed;
    std::string name;

    VariableTerm(const expr_pointer_t& lb, const expr_pointer_t& ub, const expr_pointer_t& value,
                 bool _binary, bool _integer, bool _indexed = false);

    double eval() const { return value->eval(); }

    bool is_variable() const { return true; }

    expr_pointer_t const_mult(double coef, const expr_pointer_t& repn);
    expr_pointer_t negate(const expr_pointer_t& repn);

    void accept(Visitor& v) { v.visit(*this); }
    term_id id() { return VariableTerm_id; }

    virtual std::string get_simple_name() { return "x[" + std::to_string(index) + "]"; }
    virtual std::string get_name()
    {
        if (name == "")
            return get_simple_name();
        else
            return name;
    }

    void set_lb(double val);
    void set_lb(expr_pointer_t val);

    void set_ub(double val);
    void set_ub(expr_pointer_t val);

    void set_value(double val);
    void set_value(expr_pointer_t val);
};

class IndexedVariableTerm : public VariableTerm {
   public:
    // WEH - TODO: more explicit representation here with shared_ptr
    void* var;  // ConcreteIndexedVariableRepn
    size_t vindex;
    bool first = true;

    IndexedVariableTerm(const expr_pointer_t& _lb, const expr_pointer_t& _ub,
                        const expr_pointer_t& _value, bool _binary, bool _integer, size_t _vindex,
                        void* _var)
        : VariableTerm(_lb, _ub, _value, _binary, _integer), var(_var), vindex(_vindex)
    {
    }

    std::string get_name();

    // void accept(Visitor& v)
    //     { v.visit(*this); }
};

//
// MonomialTerm
//

class MonomialTerm : public BaseExpressionTerm {
   public:
    double coef;
    std::shared_ptr<VariableTerm> var;

    MonomialTerm(double lhs, const std::shared_ptr<VariableTerm>& rhs);

    double eval() const { return coef * var->value->eval(); }

    bool is_monomial() const { return true; }

    expr_pointer_t negate(const expr_pointer_t& repn);

    void accept(Visitor& v) { v.visit(*this); }
    term_id id() { return MonomialTerm_id; }
};

}  // namespace coek
