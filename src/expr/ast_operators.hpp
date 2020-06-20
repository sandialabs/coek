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
if (lhs == ZEROCONST)
    return rhs;
#if 0
PlusTerm* _lhs = dynamic_cast<PlusTerm*>(lhs);
if (_lhs && (_lhs->data->size() == _lhs->n)) {
    _lhs->push_back(rhs);
    return _lhs;
    }
return CREATE_POINTER(PlusTerm, lhs, rhs, false);
#else
return CREATE_POINTER(PlusTerm, lhs, rhs);
#endif
}

template <typename LHS>
expr_pointer_t plus(const LHS& lhs, double rhs)
{
if (rhs == 0.0)
    return lhs;

expr_pointer_t _rhs = CREATE_POINTER(ConstantTerm, rhs);
#if 0
PlusTerm* _lhs = dynamic_cast<PlusTerm*>(lhs);
if (_lhs && (_lhs->data->size() == _lhs->n)) {
    _lhs->push_back(_rhs);
    return _lhs;
    }
return CREATE_POINTER(PlusTerm, lhs, _rhs, false);
#else
return CREATE_POINTER(PlusTerm, lhs, _rhs);
#endif
}

template <typename RHS>
expr_pointer_t plus(double lhs, const RHS& rhs)
{
if (lhs == 0.0)
    return rhs;

expr_pointer_t _lhs = CREATE_POINTER(ConstantTerm, lhs);
return CREATE_POINTER(PlusTerm, _lhs, rhs, false);
}

template <typename LHS>
expr_pointer_t plus(const LHS& lhs, int rhs)
{
if (rhs == 0)
    return lhs;

expr_pointer_t _rhs = CREATE_POINTER(ConstantTerm, rhs);
#if 0
PlusTerm* _lhs = dynamic_cast<PlusTerm*>(lhs);
if (_lhs && (_lhs->data->size() == _lhs->n)) {
    _lhs->push_back(_rhs);
    return _lhs;
    }
return CREATE_POINTER(PlusTerm, lhs, _rhs, false);
#else
return CREATE_POINTER(PlusTerm, lhs, _rhs);
#endif
}

template <typename RHS>
expr_pointer_t plus(int lhs, const RHS& rhs)
{
if (lhs == 0)
    return rhs;

expr_pointer_t _lhs = CREATE_POINTER(ConstantTerm, lhs);
return CREATE_POINTER(PlusTerm, _lhs, rhs, false);
}

//
// operator-
//
template <typename LHS, typename RHS>
expr_pointer_t minus(const LHS& lhs, const RHS& rhs)
{
#if 0
PlusTerm* _lhs = dynamic_cast<PlusTerm*>(lhs);
if (_lhs && (_lhs->data->size() == _lhs->n)) {
    _lhs->push_back(rhs->negate(rhs));
    return _lhs;
    }
return CREATE_POINTER(PlusTerm, lhs, rhs->negate(rhs), false);
#else
return CREATE_POINTER(PlusTerm, lhs, rhs->negate(rhs));
#endif
}

template <typename RHS>
expr_pointer_t minus(double lhs, const RHS& rhs)
{
if (lhs == 0.0)
    return rhs->negate(rhs);

expr_pointer_t _lhs = CREATE_POINTER(ConstantTerm, lhs);
return CREATE_POINTER(PlusTerm, _lhs, rhs->negate(rhs), false);
}

template <typename RHS>
expr_pointer_t minus(int lhs, const RHS& rhs)
{
if (lhs == 0)
    return rhs->negate(rhs);

expr_pointer_t _lhs = CREATE_POINTER(ConstantTerm, lhs);
return CREATE_POINTER(PlusTerm, _lhs, rhs->negate(rhs), false);
}

template <typename LHS>
expr_pointer_t minus(const LHS& lhs, double rhs)
{
// IGNORE, SINCE THIS CASE IS VERY UNCOMMON
//if (rhs == 0.0)
//    return lhs;

expr_pointer_t _rhs = CREATE_POINTER(ConstantTerm, -rhs);
#if 0
PlusTerm* _lhs = dynamic_cast<PlusTerm*>(lhs);
if (_lhs && (_lhs->data->size() == _lhs->n)) {
    _lhs->push_back(_rhs);
    return _lhs;
    }
return CREATE_POINTER(PlusTerm, lhs, _rhs, false);
#else
return CREATE_POINTER(PlusTerm, lhs, _rhs);
#endif
}

template <typename LHS>
expr_pointer_t minus(const LHS& lhs, int rhs)
{
// IGNORE, SINCE THIS CASE IS VERY UNCOMMON
//if (rhs == 0)
//    return lhs;

expr_pointer_t _rhs = CREATE_POINTER(ConstantTerm, -rhs);
#if 0
PlusTerm* _lhs = dynamic_cast<PlusTerm*>(lhs);
if (_lhs && (_lhs->data->size() == _lhs->n)) {
    _lhs->push_back(_rhs);
    return _lhs;
    }
return CREATE_POINTER(PlusTerm, lhs, _rhs, false);
#else
return CREATE_POINTER(PlusTerm, lhs, _rhs);
#endif
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

inline ConstraintTerm* less_than(expr_pointer_t lhs, expr_pointer_t rhs, bool strict)
{
if (lhs->non_variable)
    // lower=lhs, body=rhs
    return CREATE_POINTER(InequalityTerm, lhs, rhs, 0, strict);
else if (rhs->non_variable)
    // body=lhs, upper=rhs
    return CREATE_POINTER(InequalityTerm, 0, lhs, rhs, strict);
else {
    // body=lhs-rhs, upper=ZERO
    expr_pointer_t e = minus(lhs, rhs);
    return CREATE_POINTER(InequalityTerm, 0, e, ZEROCONST, strict);
    }
}

inline ConstraintTerm* less_than(expr_pointer_t lhs, int rhs, bool strict)
{
if (rhs == 0)
    return CREATE_POINTER(InequalityTerm, 0, lhs, ZEROCONST, strict);
else
    return CREATE_POINTER(InequalityTerm, 0, lhs, CREATE_POINTER(ConstantTerm, rhs), strict);
}

inline ConstraintTerm* less_than(expr_pointer_t lhs, double rhs, bool strict)
{
if (rhs == 0.0)
    return CREATE_POINTER(InequalityTerm, 0, lhs, ZEROCONST, strict);
else
    return CREATE_POINTER(InequalityTerm, 0, lhs, CREATE_POINTER(ConstantTerm, rhs), strict);
}

inline ConstraintTerm* less_than(int lhs, expr_pointer_t rhs, bool strict)
{
if (lhs == 0)
    return CREATE_POINTER(InequalityTerm, ZEROCONST, rhs, 0, strict);
else
    return CREATE_POINTER(InequalityTerm, CREATE_POINTER(ConstantTerm, lhs), rhs, 0, strict);
}

inline ConstraintTerm* less_than(double lhs, expr_pointer_t rhs, bool strict)
{
if (lhs == 0)
    return CREATE_POINTER(InequalityTerm, ZEROCONST, rhs, 0, strict);
else
    return CREATE_POINTER(InequalityTerm, CREATE_POINTER(ConstantTerm, lhs), rhs, 0, strict);
}

inline ConstraintTerm* equal(expr_pointer_t lhs, expr_pointer_t rhs)
{
if (lhs->non_variable)
    return CREATE_POINTER(EqualityTerm, rhs, lhs);
else if (rhs->non_variable)
    return CREATE_POINTER(EqualityTerm, lhs, rhs);
else {
    expr_pointer_t e = minus(lhs, rhs);
    return CREATE_POINTER(EqualityTerm, e, ZEROCONST);
    }
}

inline ConstraintTerm* equal(expr_pointer_t lhs, int rhs)
{
if (rhs == 0)
    return CREATE_POINTER(EqualityTerm, lhs, ZEROCONST);
else
    return CREATE_POINTER(EqualityTerm, lhs, CREATE_POINTER(ConstantTerm, rhs));
}

inline ConstraintTerm* equal(expr_pointer_t lhs, double rhs)
{
if (rhs == 0)
    return CREATE_POINTER(EqualityTerm, lhs, ZEROCONST);
else
    return CREATE_POINTER(EqualityTerm, lhs, CREATE_POINTER(ConstantTerm, rhs));
}

inline ConstraintTerm* equal(int lhs, expr_pointer_t rhs)
{
if (lhs == 0)
    return CREATE_POINTER(EqualityTerm, rhs, ZEROCONST);
else
    return CREATE_POINTER(EqualityTerm, rhs, CREATE_POINTER(ConstantTerm, lhs));
}

inline ConstraintTerm* equal(double lhs, expr_pointer_t rhs)
{
if (lhs == 0)
    return CREATE_POINTER(EqualityTerm, rhs, ZEROCONST);
else
    return CREATE_POINTER(EqualityTerm, rhs, CREATE_POINTER(ConstantTerm, lhs));
}

}
