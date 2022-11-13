#pragma once

#include <cmath>

#include "base_terms.hpp"

namespace coek {

//
// operator+
//
inline expr_pointer_t plus_(const expr_pointer_t& lhs, const expr_pointer_t& rhs)
{
    expr_pointer_t tmp;
    if (lhs == ZeroConstant) tmp = rhs;
    /* WEH - Not seen in practice
    if (rhs == ZEROCONST)
        return lhs;
    if (lhs->is_constant() and (lhs->eval() == 0))
        return rhs;
    if (rhs->is_constant() and (rhs->eval() == 0))
        return lhs;
    */
    else if (lhs->is_constant() and rhs->is_constant()) {
        auto _lhs = std::dynamic_pointer_cast<ConstantTerm>(lhs);
        auto _rhs = std::dynamic_pointer_cast<ConstantTerm>(rhs);
        tmp = CREATE_POINTER(ConstantTerm, _lhs->value + _rhs->value);
    }
    else tmp = CREATE_POINTER(PlusTerm, lhs, rhs);
    return tmp;
}

template <typename LHS, typename RHS>
expr_pointer_t plus(const LHS& lhs, const RHS& rhs)
{
    expr_pointer_t tmp;
    if (lhs == ZEROCONST) tmp = rhs;
    else tmp = CREATE_POINTER(PlusTerm, lhs, rhs);
    return tmp;
}

template <typename LHS>
expr_pointer_t plus(const LHS& lhs, double rhs)
{
    expr_pointer_t tmp;
    if (rhs == 0.0) tmp = lhs;
    else tmp = CREATE_POINTER(PlusTerm, lhs,  CREATE_POINTER(ConstantTerm, rhs));
    return tmp;
}

template <typename RHS>
expr_pointer_t plus(double lhs, const RHS& rhs)
{
    expr_pointer_t tmp;
    if (lhs == 0.0) tmp = rhs;
    else tmp = CREATE_POINTER(PlusTerm, CREATE_POINTER(ConstantTerm, lhs), rhs, false);
    return tmp;
}

template <typename LHS>
expr_pointer_t plus(const LHS& lhs, int rhs)
{
    expr_pointer_t tmp;
    if (rhs == 0) tmp = lhs;
    else tmp = CREATE_POINTER(PlusTerm, lhs, CREATE_POINTER(ConstantTerm, rhs));
    return tmp;
}

template <typename RHS>
expr_pointer_t plus(int lhs, const RHS& rhs)
{
    expr_pointer_t tmp;
    if (lhs == 0) tmp = rhs;
    else tmp = CREATE_POINTER(PlusTerm, CREATE_POINTER(ConstantTerm, lhs), rhs, false);
    return tmp;
}

//
// operator-
//
template <typename LHS, typename RHS>
expr_pointer_t minus(const LHS& lhs, const RHS& rhs)
{
    return CREATE_POINTER(PlusTerm, lhs, rhs->negate(rhs));
}

template <typename RHS>
expr_pointer_t minus(double lhs, const RHS& rhs)
{
    expr_pointer_t tmp;
    if (lhs == 0.0) tmp = rhs->negate(rhs);
    else tmp = CREATE_POINTER(PlusTerm,  CREATE_POINTER(ConstantTerm, lhs), rhs->negate(rhs), false);
    return tmp;
}

template <typename RHS>
expr_pointer_t minus(int lhs, const RHS& rhs)
{
    expr_pointer_t tmp;
    if (lhs == 0) tmp = rhs->negate(rhs);
    else tmp = CREATE_POINTER(PlusTerm, CREATE_POINTER(ConstantTerm, lhs), rhs->negate(rhs), false);
    return tmp;
}

template <typename LHS>
expr_pointer_t minus(const LHS& lhs, double rhs)
{
    // IGNORE, SINCE THIS CASE IS VERY UNCOMMON
    // if (rhs == 0.0)
    //    return lhs;
    return CREATE_POINTER(PlusTerm, lhs, CREATE_POINTER(ConstantTerm, -rhs));
}

template <typename LHS>
expr_pointer_t minus(const LHS& lhs, int rhs)
{
    // IGNORE, SINCE THIS CASE IS VERY UNCOMMON
    // if (rhs == 0)
    //    return lhs;
    return CREATE_POINTER(PlusTerm, lhs, CREATE_POINTER(ConstantTerm, -rhs));
}

//
// operator*
//
inline expr_pointer_t times_(const expr_pointer_t& lhs, const expr_pointer_t& rhs)
{
    expr_pointer_t tmp;
    if (lhs == ONECONST) tmp = rhs;
    else if (rhs == ONECONST) tmp = lhs;
    /* WEH - Not seen in practice
    if (rhs == ZEROCONST)
        return ZEROCONST;
    if (lhs == ZEROCONST)
        return ZEROCONST;
    if (lhs->is_constant()) {
        auto _lhs = dynamic_cast<ConstantTerm*>(lhs);
        if (_lhs->value == 1) {
            return rhs;
            }
        }
    if (rhs->is_constant()) {
        auto _rhs = dynamic_cast<ConstantTerm*>(rhs);
        if (_rhs->value == 1) {
            return lhs;
            }
        }
    */
    else if (lhs->is_constant() and rhs->is_constant()) {
        auto _lhs = std::dynamic_pointer_cast<ConstantTerm>(lhs);
        auto _rhs = std::dynamic_pointer_cast<ConstantTerm>(rhs);
        tmp = CREATE_POINTER(ConstantTerm, _lhs->value * _rhs->value);
    }
    else tmp = CREATE_POINTER(TimesTerm, lhs, rhs);
    return tmp;
}

template <typename LHS, typename RHS>
expr_pointer_t times(const LHS& lhs, const RHS& rhs)
{
    return CREATE_POINTER(TimesTerm, lhs, rhs);
}

template <typename LHS>
expr_pointer_t times(const LHS& lhs, double rhs)
{
    expr_pointer_t tmp;
    if (rhs == 0.0) tmp = ZEROCONST;
    else if (rhs == 1.0) tmp = lhs;
    // if (rhs == -1.0)
    //     return lhs->negate(lhs);

    else tmp = CREATE_POINTER(TimesTerm, lhs, CREATE_POINTER(ConstantTerm, rhs));
    return tmp;
}

template <typename RHS>
expr_pointer_t times(double lhs, const RHS& rhs)
{
    expr_pointer_t tmp;
    if (lhs == 0.0) tmp = ZEROCONST;
    else if (lhs == 1.0) tmp = rhs;
    // if (lhs == -1.0)
    //     return rhs->negate(rhs);

    else tmp = CREATE_POINTER(TimesTerm, CREATE_POINTER(ConstantTerm, lhs), rhs);
    return tmp;
}

template <typename LHS>
expr_pointer_t times(const LHS& lhs, int rhs)
{
    expr_pointer_t tmp;
    if (rhs == 0) tmp = ZEROCONST;
    else if (rhs == 1) tmp = lhs;
    // if (rhs == -1)
    //     return lhs->negate(lhs);

    else tmp = CREATE_POINTER(TimesTerm, lhs, CREATE_POINTER(ConstantTerm, rhs));
    return tmp;
}

template <typename RHS>
expr_pointer_t times(int lhs, const RHS& rhs)
{
    expr_pointer_t tmp;
    if (lhs == 0) tmp = ZEROCONST;
    else if (lhs == 1) tmp = rhs;
    // if (lhs == -1)
    //     return rhs->negate(rhs);

    else tmp = CREATE_POINTER(TimesTerm, CREATE_POINTER(ConstantTerm, lhs), rhs);
    return tmp;
}

//
// operator/
//

inline expr_pointer_t divide_(const expr_pointer_t& lhs, const expr_pointer_t& rhs)
{
    expr_pointer_t tmp;
    if (lhs == ZEROCONST) tmp = ZEROCONST;
    else if (lhs->is_constant() and (std::dynamic_pointer_cast<ConstantTerm>(lhs)->value == 0))
        tmp = ZEROCONST;
    /* WEH - Not used in practice
    if (rhs == ONECONST)
        return lhs;
    if (rhs == NEGATIVEONECONST)
        return lhs->negate(lhs);
    if (rhs == ZEROCONST)
        throw std::domain_error("Division by zero");
    if (rhs->is_constant()) {
        auto _rhs = dynamic_cast<ConstantTerm*>(rhs);
        if (_rhs->value == 1) {
            return lhs;
            }
        }
    if (lhs->is_constant() and rhs->is_constant()) {
        auto _lhs = dynamic_cast<ConstantTerm*>(lhs);
        auto _rhs = dynamic_cast<ConstantTerm*>(rhs);
        auto ans = CREATE_POINTER(ConstantTerm, _lhs->value / _rhs->value);
        return ans;
        }
    */
    else tmp = CREATE_POINTER(DivideTerm, lhs, rhs);
    return tmp;
}

template <typename LHS, typename RHS>
expr_pointer_t divide(const LHS& lhs, const RHS& rhs)
{
    return CREATE_POINTER(DivideTerm, lhs, rhs);
}

template <typename LHS>
expr_pointer_t divide(const LHS& lhs, double rhs)
{
    if (rhs == 0.0) throw std::domain_error("Division by zero.");
    expr_pointer_t tmp;
    if (rhs == 1.0) tmp = lhs;
    else tmp = CREATE_POINTER(DivideTerm, lhs, CREATE_POINTER(ConstantTerm, rhs));
    return tmp;
}

template <typename RHS>
expr_pointer_t divide(double lhs, const RHS& rhs)
{
    expr_pointer_t tmp;
    if (lhs == 0.0) tmp = ZEROCONST;
    else tmp = CREATE_POINTER(DivideTerm, CREATE_POINTER(ConstantTerm, lhs), rhs);
    return tmp;
}

template <typename LHS>
expr_pointer_t divide(const LHS& lhs, int rhs)
{
    if (rhs == 0) throw std::domain_error("Division by zero.");

    expr_pointer_t tmp;
    if (rhs == 1) tmp = lhs;
    else tmp = CREATE_POINTER(DivideTerm, lhs, CREATE_POINTER(ConstantTerm, rhs));
    return tmp;
}

template <typename RHS>
expr_pointer_t divide(int lhs, const RHS& rhs)
{
    expr_pointer_t tmp;
    if (lhs == 0) tmp = ZEROCONST;
    else tmp = CREATE_POINTER(DivideTerm, CREATE_POINTER(ConstantTerm, lhs), rhs);
    return tmp;
}

//
// Unary Function Terms
//
template <typename BODY>
expr_pointer_t intrinsic_abs(const BODY& body)
{
    /* WEH - Doesn't show up
    if (body->is_constant())
       return CREATE_POINTER(ConstantTerm, ::fabs(body->eval()));
    */
    return CREATE_POINTER(AbsTerm, body);
}

#define UNARY_TEMPLATE(FN, TERM)                                                          \
    template <typename BODY>                                                              \
    expr_pointer_t intrinsic_##FN(const BODY& body)                                       \
    {                                                                                     \
        if (body->is_constant()) return CREATE_POINTER(ConstantTerm, ::FN(body->eval())); \
        else return CREATE_POINTER(TERM, body);                                                \
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
#define BINARY_TEMPLATE(FN, TERM)                                                \
    template <typename LHS, typename RHS>                                        \
    expr_pointer_t intrinsic_##FN(const LHS& lhs, const RHS& rhs)                \
    {                                                                            \
        if (lhs->is_constant() and rhs->is_constant())                           \
            return CREATE_POINTER(ConstantTerm, ::FN(lhs->eval(), rhs->eval())); \
        else return CREATE_POINTER(TERM, lhs, rhs);                                   \
    }

template <typename LHS, typename RHS>
expr_pointer_t intrinsic_pow(const LHS& lhs, const RHS& rhs)
{
    if (lhs->is_constant()) {
        double _lhs = lhs->eval();
        if (_lhs == 0)
            return ZEROCONST;
        else if (_lhs == 1)
            return ONECONST;
        /* WEH - Not seen
        else if (rhs->is_constant())
            return CREATE_POINTER(ConstantTerm, ::pow(_lhs, rhs->eval()));
        */
    }
    else if (rhs->is_constant()) {
        double _rhs = rhs->eval();
        if (_rhs == 0)
            return ONECONST;
        else if (_rhs == 1)
            return lhs;
    }
    return CREATE_POINTER(PowTerm, lhs, rhs);
}

// BINARY_TEMPLATE(pow, PowTerm)

//
// operator<
//

inline std::shared_ptr<ConstraintTerm> less_than(const expr_pointer_t& lhs,
                                                 const expr_pointer_t& rhs, bool strict)
{
    if (lhs->non_variable)
        // lower=lhs, body=rhs
        return CREATE_POINTER(InequalityTerm, lhs, rhs, 0, strict);
    else if (rhs->non_variable)
        // body=lhs, upper=rhs
        return CREATE_POINTER(InequalityTerm, 0, lhs, rhs, strict);
    else {
        // body=lhs-rhs, upper=ZERO
        return CREATE_POINTER(InequalityTerm, 0, minus(lhs, rhs), ZEROCONST, strict);
    }
}

inline std::shared_ptr<ConstraintTerm> less_than(const expr_pointer_t& lhs, int rhs, bool strict)
{
    if (rhs == 0)
        return CREATE_POINTER(InequalityTerm, 0, lhs, ZEROCONST, strict);
    else
        return CREATE_POINTER(InequalityTerm, 0, lhs, CREATE_POINTER(ConstantTerm, rhs), strict);
}

inline std::shared_ptr<ConstraintTerm> less_than(const expr_pointer_t& lhs, double rhs, bool strict)
{
    if (rhs == 0.0)
        return CREATE_POINTER(InequalityTerm, 0, lhs, ZEROCONST, strict);
    else
        return CREATE_POINTER(InequalityTerm, 0, lhs, CREATE_POINTER(ConstantTerm, rhs), strict);
}

inline std::shared_ptr<ConstraintTerm> less_than(int lhs, const expr_pointer_t& rhs, bool strict)
{
    if (lhs == 0)
        return CREATE_POINTER(InequalityTerm, ZEROCONST, rhs, 0, strict);
    else
        return CREATE_POINTER(InequalityTerm, CREATE_POINTER(ConstantTerm, lhs), rhs, 0, strict);
}

inline std::shared_ptr<ConstraintTerm> less_than(double lhs, const expr_pointer_t& rhs, bool strict)
{
    if (lhs == 0)
        return CREATE_POINTER(InequalityTerm, ZEROCONST, rhs, 0, strict);
    else
        return CREATE_POINTER(InequalityTerm, CREATE_POINTER(ConstantTerm, lhs), rhs, 0, strict);
}

inline std::shared_ptr<ConstraintTerm> equal(const expr_pointer_t& lhs, const expr_pointer_t& rhs)
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

inline std::shared_ptr<ConstraintTerm> equal(const expr_pointer_t& lhs, int rhs)
{
    if (rhs == 0)
        return CREATE_POINTER(EqualityTerm, lhs, ZEROCONST);
    else
        return CREATE_POINTER(EqualityTerm, lhs, CREATE_POINTER(ConstantTerm, rhs));
}

inline std::shared_ptr<ConstraintTerm> equal(const expr_pointer_t& lhs, double rhs)
{
    if (rhs == 0)
        return CREATE_POINTER(EqualityTerm, lhs, ZEROCONST);
    else
        return CREATE_POINTER(EqualityTerm, lhs, CREATE_POINTER(ConstantTerm, rhs));
}

inline std::shared_ptr<ConstraintTerm> equal(int lhs, const expr_pointer_t& rhs)
{
    if (lhs == 0)
        return CREATE_POINTER(EqualityTerm, rhs, ZEROCONST);
    else
        return CREATE_POINTER(EqualityTerm, rhs, CREATE_POINTER(ConstantTerm, lhs));
}

inline std::shared_ptr<ConstraintTerm> equal(double lhs, const expr_pointer_t& rhs)
{
    if (lhs == 0)
        return CREATE_POINTER(EqualityTerm, rhs, ZEROCONST);
    else
        return CREATE_POINTER(EqualityTerm, rhs, CREATE_POINTER(ConstantTerm, lhs));
}

}  // namespace coek
