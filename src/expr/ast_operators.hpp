#pragma once

#include <cmath>
#include "ast_term.hpp"

namespace coek {

//
// operator+
//
inline expr_pointer_t plus_(expr_pointer_t lhs, expr_pointer_t rhs)
{
if (lhs == ZEROCONST)
    return rhs;
if (rhs == ZEROCONST)
    return lhs;
if (lhs->is_constant() and (lhs->eval() == 0))
    return rhs;
if (rhs->is_constant() and (rhs->eval() == 0))
    return lhs;
if (lhs->is_constant() and rhs->is_constant()) {
    auto _lhs = dynamic_cast<ConstantTerm*>(lhs);
    auto _rhs = dynamic_cast<ConstantTerm*>(rhs);
    auto ans = CREATE_POINTER(ConstantTerm, _lhs->value + _rhs->value);
    DISCARD_POINTER(_lhs);
    DISCARD_POINTER(_rhs);
    return ans;
    }
return CREATE_POINTER(PlusTerm, lhs, rhs);
}

template <typename LHS, typename RHS>
expr_pointer_t plus(const LHS& lhs, const RHS& rhs)
{
return CREATE_POINTER(PlusTerm, lhs, rhs);
}

template <typename LHS>
expr_pointer_t plus(const LHS& lhs, double rhs)
{
if (rhs == 0.0)
    return lhs;

expr_pointer_t _rhs = CREATE_POINTER(ConstantTerm, rhs);
return CREATE_POINTER(PlusTerm, lhs, _rhs);
}

template <typename RHS>
expr_pointer_t plus(double lhs, const RHS& rhs)
{
if (lhs == 0.0)
    return rhs;

expr_pointer_t _lhs = CREATE_POINTER(ConstantTerm, lhs);
return CREATE_POINTER(PlusTerm, _lhs, rhs);
}

template <typename LHS>
expr_pointer_t plus(const LHS& lhs, int rhs)
{
if (rhs == 0)
    return lhs;

expr_pointer_t _rhs = CREATE_POINTER(ConstantTerm, rhs);
return CREATE_POINTER(PlusTerm, lhs, _rhs);
}

template <typename RHS>
expr_pointer_t plus(int lhs, const RHS& rhs)
{
if (lhs == 0)
    return rhs;

expr_pointer_t _lhs = CREATE_POINTER(ConstantTerm, lhs);
return CREATE_POINTER(PlusTerm, _lhs, rhs);
}

//
// operator-
//
template <typename LHS, typename RHS>
expr_pointer_t minus(const LHS& lhs, const RHS& rhs)
{
return CREATE_POINTER(PlusTerm, lhs, rhs->negate(rhs) );
}

template <typename RHS>
expr_pointer_t minus(double lhs, const RHS& rhs)
{
if (lhs == 0.0)
    return rhs->negate(rhs);

expr_pointer_t _lhs = CREATE_POINTER(ConstantTerm, lhs);
return CREATE_POINTER(PlusTerm, _lhs, rhs->negate(rhs));
}

template <typename RHS>
expr_pointer_t minus(int lhs, const RHS& rhs)
{
if (lhs == 0)
    return rhs->negate(rhs);

expr_pointer_t _lhs = CREATE_POINTER(ConstantTerm, lhs);
return CREATE_POINTER(PlusTerm, _lhs, rhs->negate(rhs));
}

template <typename LHS>
expr_pointer_t minus(const LHS& lhs, double rhs)
{
// IGNORE, SINCE THIS CASE IS VERY UNCOMMON
//if (rhs == 0.0)
//    return lhs;

expr_pointer_t _rhs = CREATE_POINTER(ConstantTerm, -rhs);
return CREATE_POINTER(PlusTerm, lhs, _rhs);
}

template <typename LHS>
expr_pointer_t minus(const LHS& lhs, int rhs)
{
// IGNORE, SINCE THIS CASE IS VERY UNCOMMON
//if (rhs == 0)
//    return lhs;

expr_pointer_t _rhs = CREATE_POINTER(ConstantTerm, -rhs);
return CREATE_POINTER(PlusTerm, lhs, _rhs);
}

//
// operator*
//
inline expr_pointer_t times_(expr_pointer_t lhs, expr_pointer_t rhs)
{
if (lhs == ZEROCONST)
    return ZEROCONST;
if (lhs == ONECONST)
    return rhs;
if (rhs == ZEROCONST)
    return ZEROCONST;
if (rhs == ONECONST)
    return lhs;
if (lhs->is_constant()) {
    auto _lhs = dynamic_cast<ConstantTerm*>(lhs);
    if (_lhs->value == 1) {
        DISCARD_POINTER(_lhs);
        return rhs;
        }
    }
if (rhs->is_constant()) {
    auto _rhs = dynamic_cast<ConstantTerm*>(rhs);
    if (_rhs->value == 1) {
        DISCARD_POINTER(_rhs);
        return lhs;
        }
    }
if (lhs->is_constant() and rhs->is_constant()) {
    auto _lhs = dynamic_cast<ConstantTerm*>(lhs);
    auto _rhs = dynamic_cast<ConstantTerm*>(rhs);
    auto ans = CREATE_POINTER(ConstantTerm, _lhs->value * _rhs->value);
    DISCARD_POINTER(_lhs);
    DISCARD_POINTER(_rhs);
    return ans;
    }
return CREATE_POINTER(TimesTerm, lhs, rhs);
}

template <typename LHS, typename RHS>
expr_pointer_t times(const LHS& lhs, const RHS& rhs)
{
return CREATE_POINTER(TimesTerm, lhs, rhs);
}

template <typename LHS>
expr_pointer_t times(const LHS& lhs, double rhs)
{
if (rhs == 0.0)
    return ZEROCONST;
//if (rhs == 1.0)
//    return lhs;
//if (rhs == -1.0)
//    return lhs->negate(lhs);

expr_pointer_t _rhs = CREATE_POINTER(ConstantTerm, rhs);
return CREATE_POINTER(TimesTerm, lhs, _rhs);
}

template <typename RHS>
expr_pointer_t times(double lhs, const RHS& rhs)
{
if (lhs == 0.0)
    return ZEROCONST;
//if (lhs == 1.0)
//    return rhs;
//if (lhs == -1.0)
//    return rhs->negate(rhs);

expr_pointer_t _lhs = CREATE_POINTER(ConstantTerm, lhs);
return CREATE_POINTER(TimesTerm, _lhs, rhs);
}

template <typename LHS>
expr_pointer_t times(const LHS& lhs, int rhs)
{
if (rhs == 0)
    return ZEROCONST;
//if (rhs == 1)
//    return lhs;
//if (rhs == -1)
//    return lhs->negate(lhs);

expr_pointer_t _rhs = CREATE_POINTER(ConstantTerm, rhs);
return CREATE_POINTER(TimesTerm, lhs, _rhs);
}

template <typename RHS>
expr_pointer_t times(int lhs, const RHS& rhs)
{
if (lhs == 0)
    return ZEROCONST;
//if (lhs == 1)
//    return rhs;
//if (lhs == -1)
//    return rhs->negate(rhs);

expr_pointer_t _lhs = CREATE_POINTER(ConstantTerm, lhs);
return CREATE_POINTER(TimesTerm, _lhs, rhs);
}

//
// operator/
//

inline expr_pointer_t divide_(expr_pointer_t lhs, expr_pointer_t rhs)
{
if (lhs == ZEROCONST)
    return ZEROCONST;
if (rhs == ONECONST)
    return lhs;
if (rhs == NEGATIVEONECONST)
    return lhs->negate(lhs);
if (rhs == ZEROCONST)
    throw std::domain_error("Division by zero");
if (lhs->is_constant()) {
    auto _lhs = dynamic_cast<ConstantTerm*>(lhs);
    if (_lhs->value == 0) {
        DISCARD_POINTER(_lhs);
        DISCARD_POINTER(rhs);
        return ZEROCONST;
        }
    }
if (rhs->is_constant()) {
    auto _rhs = dynamic_cast<ConstantTerm*>(rhs);
    if (_rhs->value == 1) {
        DISCARD_POINTER(_rhs);
        return lhs;
        }
    }
if (lhs->is_constant() and rhs->is_constant()) {
    auto _lhs = dynamic_cast<ConstantTerm*>(lhs);
    auto _rhs = dynamic_cast<ConstantTerm*>(rhs);
    auto ans = CREATE_POINTER(ConstantTerm, _lhs->value / _rhs->value);
    DISCARD_POINTER(_lhs);
    DISCARD_POINTER(_rhs);
    return ans;
    }
return CREATE_POINTER(DivideTerm, lhs, rhs);
}


template <typename LHS, typename RHS>
expr_pointer_t divide(const LHS& lhs, const RHS& rhs)
{
return CREATE_POINTER(DivideTerm, lhs, rhs);
}

template <typename LHS>
expr_pointer_t divide(const LHS& lhs, double rhs)
{
//if (rhs == 1.0)
//    return lhs;
//if (rhs == -1.0)
//    return lhs->negate(lhs);
if (rhs == 0.0)
    throw std::domain_error("Division by zero");

expr_pointer_t _rhs = CREATE_POINTER(ConstantTerm, rhs);
return CREATE_POINTER(DivideTerm, lhs, _rhs);
}

template <typename RHS>
expr_pointer_t divide(double lhs, const RHS& rhs)
{
if (lhs == 0.0)
    return ZEROCONST;

expr_pointer_t _lhs = CREATE_POINTER(ConstantTerm, lhs);
return CREATE_POINTER(DivideTerm, _lhs, rhs);
}

template <typename LHS>
expr_pointer_t divide(const LHS& lhs, int rhs)
{

//if (rhs == 1)
//    return lhs;
//if (rhs == -1)
//    return lhs->negate(lhs);
if (rhs == 0)
    throw std::domain_error("Division by zero");

expr_pointer_t _rhs = CREATE_POINTER(ConstantTerm, rhs);
return CREATE_POINTER(DivideTerm, lhs, _rhs);
}

template <typename RHS>
expr_pointer_t divide(int lhs, const RHS& rhs)
{
if (lhs == 0)
    return ZEROCONST;

expr_pointer_t _lhs = CREATE_POINTER(ConstantTerm, lhs);
return CREATE_POINTER(DivideTerm, _lhs, rhs);
}


//
// Unary Function Terms
//
template <typename BODY>
expr_pointer_t intrinsic_abs(const BODY& body)
{
if (body->is_constant())
   return CREATE_POINTER(ConstantTerm, ::fabs(body->eval()));
return CREATE_POINTER(AbsTerm, body);
}

#define UNARY_TEMPLATE(FN, TERM)\
template <typename BODY>\
expr_pointer_t intrinsic_ ## FN(const BODY& body)\
{\
if (body->is_constant())\
   return CREATE_POINTER(ConstantTerm, ::FN(body->eval()));\
return CREATE_POINTER(TERM, body);\
}

UNARY_TEMPLATE(ceil, CeilTerm)
UNARY_TEMPLATE(floor, FloorTerm)
UNARY_TEMPLATE(exp, ExpTerm)
UNARY_TEMPLATE(log, LogTerm)
UNARY_TEMPLATE(log10, Log10Term)
UNARY_TEMPLATE(sqrt, SqrtTerm)
UNARY_TEMPLATE(sin, SinTerm)
UNARY_TEMPLATE(cos, CosTerm)
UNARY_TEMPLATE(tan, TanTerm)
UNARY_TEMPLATE(sinh, SinhTerm)
UNARY_TEMPLATE(cosh, CoshTerm)
UNARY_TEMPLATE(tanh, TanhTerm)
UNARY_TEMPLATE(asin, ASinTerm)
UNARY_TEMPLATE(acos, ACosTerm)
UNARY_TEMPLATE(atan, ATanTerm)
UNARY_TEMPLATE(asinh, ASinhTerm)
UNARY_TEMPLATE(acosh, ACoshTerm)
UNARY_TEMPLATE(atanh, ATanhTerm)

//
// Binary Function Terms
//
#define BINARY_TEMPLATE(FN, TERM)\
template <typename LHS, typename RHS>\
expr_pointer_t intrinsic_ ## FN(const LHS& lhs, const RHS& rhs)\
{\
if (lhs->is_constant() and rhs->is_constant())\
   return CREATE_POINTER(ConstantTerm, ::FN(lhs->eval(), rhs->eval()));\
return CREATE_POINTER(TERM, lhs, rhs);\
}

template <typename LHS, typename RHS>
expr_pointer_t intrinsic_pow(const LHS& lhs, const RHS& rhs)
{
if (lhs->is_constant()) {
    double _lhs = lhs->eval();
    if (_lhs == 0)
        return ZEROCONST;
    if (_lhs == 1)
        return ONECONST;
    if (rhs->is_constant())
        return CREATE_POINTER(ConstantTerm, ::pow(lhs->eval(), rhs->eval()));
    }
else if (rhs->is_constant()) {
    double _rhs = rhs->eval();
    if (_rhs == 0)
        return ONECONST;
    if (_rhs == 1)
        return lhs;
    }
return CREATE_POINTER(PowTerm, lhs, rhs);
}

//BINARY_TEMPLATE(pow, PowTerm)

//
// operator<
//

template <typename LHS, typename RHS>
ConstraintTerm* less_than(const LHS& lhs, const RHS& rhs, bool strict)
{
expr_pointer_t e = minus(lhs, rhs);

return CREATE_POINTER(InequalityTerm, e, strict);
}

template <typename LHS>
ConstraintTerm* less_than(const LHS& lhs, int rhs, bool strict)
{
expr_pointer_t e;
if (rhs == 0)
    e = lhs;
else
    e = minus(lhs, rhs);

return CREATE_POINTER(InequalityTerm, e, strict);
}

template <typename LHS>
ConstraintTerm* less_than(const LHS& lhs, double rhs, bool strict)
{
expr_pointer_t e;
if (rhs == 0.0)
    e = lhs;
else
    e = minus(lhs, rhs);

return CREATE_POINTER(InequalityTerm, e, strict);
}

template <typename LHS, typename RHS>
ConstraintTerm* greater_than(const LHS& lhs, const RHS& rhs, bool strict)
{
expr_pointer_t _lhs = lhs->negate(lhs);
expr_pointer_t e = plus(_lhs, rhs);

return CREATE_POINTER(InequalityTerm, e, strict);
}

template <typename RHS>
ConstraintTerm* greater_than(int lhs, const RHS& rhs, bool strict)
{
expr_pointer_t e = plus(-lhs, rhs);

return CREATE_POINTER(InequalityTerm, e, strict);
}

template <typename RHS>
ConstraintTerm* greater_than(double lhs, const RHS& rhs, bool strict)
{
expr_pointer_t e = plus(-lhs, rhs);

return CREATE_POINTER(InequalityTerm, e, strict);
}

template <typename LHS, typename RHS>
ConstraintTerm* equal(const LHS& lhs, const RHS& rhs)
{
expr_pointer_t e = minus(lhs, rhs);

return CREATE_POINTER(EqualityTerm, e);
}

template <typename LHS>
ConstraintTerm* equal(const LHS& lhs, int rhs)
{
expr_pointer_t e;
if (rhs == 0)
    e = lhs;
else
    e = minus(lhs, rhs);

return CREATE_POINTER(EqualityTerm, e);
}

template <typename LHS>
ConstraintTerm* equal(const LHS& lhs, double rhs)
{
expr_pointer_t e;
if (rhs == 0.0)
    e = lhs;
else
    e = minus(lhs, rhs);

return CREATE_POINTER(EqualityTerm, e);
}

}