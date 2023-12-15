#pragma once

namespace coek {

class Expression;
class Constraint;

/**
 * Create a conditional expression.
 *
 * The function if_else(cond,x,y) returns the value x if cond is true,
 * and the value y otherwise.
 */
Expression if_else(const Expression&, const Expression&, const Expression&);
Expression if_else(const Constraint&, const Expression&, const Expression&);

template <typename CondType, typename ThenType>
inline Expression if_else(const CondType& cond_, const ThenType& then_)
{
    return if_else(cond_, then_, 0);
}

template <typename CondType, typename ThenType, typename ElseType,
          typename = typename std::enable_if<std::is_arithmetic<ThenType>::value, ThenType>::type>
inline Expression if_else(const CondType& cond_, ThenType _then, const ElseType& else_)
{
    Expression then_(_then);
    return if_else(cond_, then_, else_);
}

template <typename CondType, typename ElseType,
          typename = typename std::enable_if<std::is_arithmetic<ElseType>::value, ElseType>::type>
inline Expression if_else(const CondType& cond_, const Expression& then_, ElseType _else)
{
    Expression else_(_else);
    return if_else(cond_, then_, else_);
}

template <typename CondType,
          typename = typename std::enable_if<std::is_arithmetic<CondType>::value, CondType>::type>
inline Expression if_else(CondType _cond, const Expression& then_, const Expression& else_)
{
    if (_cond)
        return then_;
    else
        return else_;
}

/**
 * Create an expression calling the abs() function.
 *
 * The function abs(x) computes the absolute value of x.
 */
Expression abs(const Expression&);
/**
 * Create an expression calling the ceil() function.
 *
 * The function ceil(x) computes the nearest integer not less than x.
 */
Expression ceil(const Expression&);
/**
 * Create an expression calling the floor() function.
 *
 * The function floor(x) computes the nearest integer not greater than x.
 */
Expression floor(const Expression&);
/**
 * Create an expression calling the exp() function.
 *
 * The function exp(x) computes the value of e raised to the xth power.
 */
Expression exp(const Expression&);
/**
 * Create an expression calling the log() function.
 *
 * The function log(x) computes the natural logarithm of x (base e).
 */
Expression log(const Expression&);
/**
 * Create an expression calling the log10() function.
 *
 * The function log10(x) computes the common logarithm of x (base 10).
 */
Expression log10(const Expression&);
/**
 * Create an expression calling the sqrt() function.
 *
 * The function sqrt(x) computes the square root of x.
 */
Expression sqrt(const Expression&);
/**
 * Create an expression calling the sin() function.
 *
 * The function sin(x) computes the sine of x.
 */
Expression sin(const Expression&);
/**
 * Create an expression calling the cos() function.
 *
 * The function cos(x) computes the cosine of x.
 */
Expression cos(const Expression&);
/**
 * Create an expression calling the tan() function.
 *
 * The function tan(x) computes the tangent of x.
 */
Expression tan(const Expression&);
/**
 * Create an expression calling the sinh() function.
 *
 * The function sinh(x) computes the hyperbolic sine of x.
 */
Expression sinh(const Expression&);
/**
 * Create an expression calling the cosh() function.
 *
 * The function cosh(x) computes the hyperbolic cosine of x.
 */
Expression cosh(const Expression&);
/**
 * Create an expression calling the tanh() function.
 *
 * The function tanh(x) computes the hyperbolic tangent of x.
 */
Expression tanh(const Expression&);
/**
 * Create an expression calling the asin() function.
 *
 * The function asin(x) computes the arc sine of x.
 */
Expression asin(const Expression&);
/**
 * Create an expression calling the acos() function.
 *
 * The function acos(x) computes the arc cosine of x.
 */
Expression acos(const Expression&);
/**
 * Create an expression calling the atan() function.
 *
 * The function atan(x) computes the arc tangent of x.
 */
Expression atan(const Expression&);
/**
 * Create an expression calling the asinh() function.
 *
 * The function asinh(x) computes the hyperbolic arc sine of x.
 */
Expression asinh(const Expression&);
/**
 * Create an expression calling the acosh() function.
 *
 * The function acosh(x) computes the hyperbolic arc cosine of x.
 */
Expression acosh(const Expression&);
/**
 * Create an expression calling the atanh() function.
 *
 * The function atanh(x) computes the hyperbolic arc tangent of x.
 */
Expression atanh(const Expression&);
/**
 * Create an expression calling the pow() function with two expression arguments.
 *
 * The function pow(x,y) computes the value of x raised to the power y.
 */
Expression pow(const Expression&, const Expression&);
/**
 * Create an expression calling the pow() function with a double exponent value.
 *
 * The function pow(x,y) computes the value of x raised to the power y.
 */
Expression pow(const Expression&, double);
/**
 * Create an expression calling the pow() function with a double base value.
 *
 * The function pow(x,y) computes the value of x raised to the power y.
 */
Expression pow(double, const Expression&);

}  // namespace coek
