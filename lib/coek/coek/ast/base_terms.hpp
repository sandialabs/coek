#pragma once

#include <list>
#include <string>
#include <memory>
#include <iostream>
#include <unordered_map>

#include "visitor.hpp"

namespace coek {

class BaseExpressionTerm;

// SHARED_PTR
typedef std::shared_ptr<BaseExpressionTerm> expr_pointer_t;
#define CREATE_POINTER(PTR, ...) std::make_shared<PTR>(__VA_ARGS__)

class BaseExpressionTerm {
   public:
    bool non_variable;

    BaseExpressionTerm() : non_variable(false) {}
    virtual ~BaseExpressionTerm() {}

    virtual double _eval() const = 0;
    double eval() const;

    virtual bool is_constant() const { return false; }
    virtual bool is_parameter() const { return false; }
    virtual bool is_abstract_parameter() const { return false; }
    virtual bool is_variable() const { return false; }
    virtual bool is_monomial() const { return false; }
    virtual bool is_expression() const { return false; }

    virtual bool constains_vars() const { return false; }

    virtual expr_pointer_t const_mult(double coef, const expr_pointer_t& repn);
    virtual expr_pointer_t negate(const expr_pointer_t& repn);
    virtual term_id id() = 0;
    std::list<std::string> to_list();
};

//
// ConstantTerm
//

class ConstantTerm : public BaseExpressionTerm {
   public:
    double value;

    explicit ConstantTerm(double _value) : value(_value) { non_variable = true; }

    double _eval() const { return value; }

    bool is_constant() const { return true; }

    expr_pointer_t negate(const expr_pointer_t& repn);

    term_id id() { return ConstantTerm_id; }
};

extern std::shared_ptr<ConstantTerm> ZeroConstant;
extern std::shared_ptr<ConstantTerm> OneConstant;
extern std::shared_ptr<ConstantTerm> NegativeOneConstant;
#define ZEROCONST ZeroConstant
#define ONECONST OneConstant
#define NEGATIVEONECONST NegativeOneConstant

}  // namespace coek
