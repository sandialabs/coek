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
    static unsigned int count;

   public:
    expr_pointer_t value;
    unsigned int index;
    std::string name;

   public:
    ParameterTerm();
    explicit ParameterTerm(const expr_pointer_t& _value);

    double _eval() const { return value->_eval(); }

    bool is_parameter() const { return true; }
    bool is_constant_expression() { return true; }

    expr_pointer_t negate(const expr_pointer_t& repn);

    void accept(Visitor& v) { v.visit(*this); }
    term_id id() { return ParameterTerm_id; }

    void set_value(double val);
    void set_value(expr_pointer_t val);

    virtual std::string get_simple_name() { return "P[" + std::to_string(index) + "]"; }
    virtual std::string get_name()
    {
        if (name == "")
            return get_simple_name();
        else
            return name;
    }
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

   public:
    explicit IndexParameterTerm(const std::string& _name) : name(_name), type(0)
    {
        non_variable = true;
    }

    bool is_abstract_parameter() const { return true; }

    double _eval() const { return as_double_value(); }
    double as_double_value() const;

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
    std::string name;

   public:
    VariableTerm(const expr_pointer_t& lb, const expr_pointer_t& ub, const expr_pointer_t& value,
                 bool _binary, bool _integer);

    double _eval() const { return value->_eval(); }

    bool is_variable() const { return true; }
    bool is_constant_expression() { return fixed; }

    expr_pointer_t const_mult(double coef, const expr_pointer_t& repn);
    expr_pointer_t negate(const expr_pointer_t& repn);

    void accept(Visitor& v) { v.visit(*this); }
    term_id id() { return VariableTerm_id; }

    virtual std::string get_simple_name() { return "X[" + std::to_string(index) + "]"; }
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

//
// MonomialTerm
//

class MonomialTerm : public BaseExpressionTerm {
   public:
    double coef;
    std::shared_ptr<VariableTerm> var;

   public:
    MonomialTerm(double lhs, const std::shared_ptr<VariableTerm>& rhs);

    double _eval() const { return coef * var->value->_eval(); }

    bool is_monomial() const { return true; }
    bool is_constant_expression() { return var->fixed; }

    expr_pointer_t negate(const expr_pointer_t& repn);

    void accept(Visitor& v) { v.visit(*this); }
    term_id id() { return MonomialTerm_id; }
};

}  // namespace coek
