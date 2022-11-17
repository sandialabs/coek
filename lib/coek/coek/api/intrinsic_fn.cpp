#include "../ast/base_terms.hpp"
#include "../ast/constraint_terms.hpp"
#include "../ast/expr_terms.hpp"
#include "../ast/visitor_fns.hpp"
#include "../ast/ast_operators.hpp"
#include "expression.hpp"

namespace coek {

//
// intrinsic functions
//
Expression abs(const Expression& body)
{
    if (body.repn->is_constant()) {
        auto _body = std::dynamic_pointer_cast<ConstantTerm>(body.repn);
        return Expression(::fabs(_body->value));
    }
    return intrinsic_abs(body.repn);
}

#define INTRINSIC_DEF1(FN)                                                   \
    Expression FN(const Expression& body)                                    \
    {                                                                        \
        if (body.repn->is_constant()) {                                      \
            auto _body = std::dynamic_pointer_cast<ConstantTerm>(body.repn); \
            return Expression(::FN(_body->value));                           \
        }                                                                    \
        return intrinsic_##FN(body.repn);                                    \
    }

INTRINSIC_DEF1(ceil)
INTRINSIC_DEF1(floor)
INTRINSIC_DEF1(exp)
INTRINSIC_DEF1(log)
INTRINSIC_DEF1(log10)
INTRINSIC_DEF1(sqrt)
INTRINSIC_DEF1(sin)
INTRINSIC_DEF1(cos)
INTRINSIC_DEF1(tan)
INTRINSIC_DEF1(sinh)
INTRINSIC_DEF1(cosh)
INTRINSIC_DEF1(tanh)
INTRINSIC_DEF1(asin)
INTRINSIC_DEF1(acos)
INTRINSIC_DEF1(atan)
INTRINSIC_DEF1(asinh)
INTRINSIC_DEF1(acosh)
INTRINSIC_DEF1(atanh)

#define INTRINSIC_DEF2(FN)                                               \
    Expression FN(const Expression& lhs, const Expression& rhs)          \
    {                                                                    \
        if (lhs.is_constant() and rhs.is_constant()) {                   \
            return Expression(::FN(lhs.repn->eval(), rhs.repn->eval())); \
        }                                                                \
        return intrinsic_##FN(lhs.repn, rhs.repn);                       \
    }                                                                    \
                                                                         \
    Expression FN(const Expression& lhs, double rhs)                     \
    {                                                                    \
        if (lhs.is_constant()) {                                         \
            return Expression(::FN(lhs.repn->eval(), rhs));              \
        }                                                                \
        Expression _rhs(rhs);                                            \
        return intrinsic_##FN(lhs.repn, _rhs.repn);                      \
    }                                                                    \
                                                                         \
    Expression FN(double lhs, const Expression& rhs)                     \
    {                                                                    \
        if (rhs.is_constant()) {                                         \
            return Expression(::FN(lhs, rhs.repn->eval()));              \
        }                                                                \
        Expression _lhs(lhs);                                            \
        return intrinsic_##FN(_lhs.repn, rhs.repn);                      \
    }

INTRINSIC_DEF2(pow)
}  // namespace coek
