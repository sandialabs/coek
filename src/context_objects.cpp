#include "expr_types.hpp"
#include "context_objects.hpp"


ExpressionContext_Objects::~ExpressionContext_Objects(void)
{
for (std::list<NumericValue*>::iterator it=owned.begin(); it != owned.end(); it++)
    delete *it;
}

numval_t ExpressionContext_Objects::negate(numval_t expr)
{
NumericValue* e = static_cast<NumericValue*>(expr);
Expression* tmp = new NegExpression(e->context, e);
owned.push_back(tmp);
return tmp;
}

numval_t ExpressionContext_Objects::plus(numval_t lhs, numval_t rhs)
{
if (lhs == &ZeroParameter)
    return rhs;
if (rhs == &ZeroParameter)
    return lhs;

assert(lhs->context == rhs->context);

NumericValue* tmp;
if (lhs->is_parameter() and rhs->is_parameter())
    tmp = new TypedParameter<double>(lhs->context, lhs->_value + rhs->_value, false);
else
    tmp = new AddExpression<NumericValue*,NumericValue*>(lhs->context, lhs,rhs);
owned.push_back(tmp);
return tmp;
}

numval_t ExpressionContext_Objects::minus(numval_t lhs, numval_t rhs)
{
if (lhs == &ZeroParameter)
    return this->negate(rhs);
if (rhs == &ZeroParameter)
    return lhs;

assert(lhs->context == rhs->context);

NumericValue* tmp;
if (lhs->is_parameter() and rhs->is_parameter())
    tmp = new TypedParameter<double>(lhs->context, lhs->_value - rhs->_value, false);
else
    tmp = new SubExpression(lhs->context, lhs, rhs);
owned.push_back(tmp);
return tmp;
}

numval_t ExpressionContext_Objects::times(numval_t lhs, numval_t rhs)
{
if (lhs == &ZeroParameter or rhs == &ZeroParameter)
    return &ZeroParameter;
if (lhs == &OneParameter)
    return rhs;
if (rhs == &OneParameter)
    return lhs;

assert(lhs->context == rhs->context);

NumericValue* tmp;
if (lhs->is_parameter() and rhs->is_parameter())
    tmp = new TypedParameter<double>(lhs->context, lhs->_value * rhs->_value, false);
else
    tmp = new MulExpression<NumericValue*,NumericValue*>(lhs->context, lhs, rhs);
owned.push_back(tmp);
return tmp;
}

numval_t ExpressionContext_Objects::divide(numval_t lhs, numval_t rhs)
{
if (lhs == &ZeroParameter)
    return &ZeroParameter;
if (rhs == &OneParameter)
    return lhs;
if (rhs == &NegativeOneParameter)
    return this->negate(lhs);

assert(lhs->context == rhs->context);

NumericValue* tmp;
if (lhs->is_parameter() and rhs->is_parameter())
    tmp = new TypedParameter<double>(lhs->context, lhs->_value / rhs->_value, false);
else
    tmp = new DivExpression(lhs->context, lhs, rhs);
owned.push_back(tmp);
return tmp;
}

numval_t ExpressionContext_Objects::abs(numval_t expr)
{
Expression* tmp = new AbsExpression(expr->context, expr);
owned.push_back(tmp);
return tmp;
}

numval_t ExpressionContext_Objects::pow(numval_t lhs, numval_t rhs)
{
assert(lhs->context == rhs->context);

Expression* tmp = new PowExpression(lhs->context, lhs, rhs);
owned.push_back(tmp);
return tmp;
}

numval_t ExpressionContext_Objects::ceil(numval_t expr)
{
Expression* tmp = new CeilExpression(expr->context, expr);
owned.push_back(tmp);
return tmp;
}

numval_t ExpressionContext_Objects::floor(numval_t expr)
{
Expression* tmp = new FloorExpression(expr->context, expr);
owned.push_back(tmp);
return tmp;
}

numval_t ExpressionContext_Objects::exp(numval_t expr)
{
Expression* tmp = new ExpExpression(expr->context, expr);
owned.push_back(tmp);
return tmp;
}

numval_t ExpressionContext_Objects::log(numval_t expr)
{
Expression* tmp = new LogExpression(expr->context, expr);
owned.push_back(tmp);
return tmp;
}

numval_t ExpressionContext_Objects::log10(numval_t expr)
{
Expression* tmp = new Log10Expression(expr->context, expr);
owned.push_back(tmp);
return tmp;
}

numval_t ExpressionContext_Objects::sqrt(numval_t expr)
{
Expression* tmp = new SqrtExpression(expr->context, expr);
owned.push_back(tmp);
return tmp;
}

numval_t ExpressionContext_Objects::sin(numval_t expr)
{
Expression* tmp = new SinExpression(expr->context, expr);
owned.push_back(tmp);
return tmp;
}

numval_t ExpressionContext_Objects::cos(numval_t expr)
{
Expression* tmp = new CosExpression(expr->context, expr);
owned.push_back(tmp);
return tmp;
}

numval_t ExpressionContext_Objects::tan(numval_t expr)
{
Expression* tmp = new TanExpression(expr->context, expr);
owned.push_back(tmp);
return tmp;
}

numval_t ExpressionContext_Objects::sinh(numval_t expr)
{
Expression* tmp = new SinhExpression(expr->context, expr);
owned.push_back(tmp);
return tmp;
}

numval_t ExpressionContext_Objects::cosh(numval_t expr)
{
Expression* tmp = new CoshExpression(expr->context, expr);
owned.push_back(tmp);
return tmp;
}

numval_t ExpressionContext_Objects::tanh(numval_t expr)
{
Expression* tmp = new TanhExpression(expr->context, expr);
owned.push_back(tmp);
return tmp;
}

numval_t ExpressionContext_Objects::asin(numval_t expr)
{
Expression* tmp = new AsinExpression(expr->context, expr);
owned.push_back(tmp);
return tmp;
}

numval_t ExpressionContext_Objects::acos(numval_t expr)
{
Expression* tmp = new AcosExpression(expr->context, expr);
owned.push_back(tmp);
return tmp;
}

numval_t ExpressionContext_Objects::atan(numval_t expr)
{
Expression* tmp = new AtanExpression(expr->context, expr);
owned.push_back(tmp);
return tmp;
}

numval_t ExpressionContext_Objects::asinh(numval_t expr)
{
Expression* tmp = new AsinhExpression(expr->context, expr);
owned.push_back(tmp);
return tmp;
}

numval_t ExpressionContext_Objects::acosh(numval_t expr)
{
Expression* tmp = new AcoshExpression(expr->context, expr);
owned.push_back(tmp);
return tmp;
}

numval_t ExpressionContext_Objects::atanh(numval_t expr)
{
Expression* tmp = new AtanhExpression(expr->context, expr);
owned.push_back(tmp);
return tmp;
}

numval_t ExpressionContext_Objects::inequality(numval_t expr, bool strict)
{
Expression* tmp = new InequalityExpression(expr->context, expr, strict);
owned.push_back(tmp);
return tmp;
}

numval_t ExpressionContext_Objects::equality(numval_t expr)
{
Expression* tmp = new EqualityExpression(expr->context, expr);
owned.push_back(tmp);
return tmp;
}

numval_t ExpressionContext_Objects::_var(bool _binary, bool _integer, double lb, double ub, double init, const char* name)
{
Variable* tmp = new Variable(this, _binary, _integer, lb, ub, init, name);
owned.push_back(tmp);
return tmp;
}

numval_t ExpressionContext_Objects::_param(int _value, bool _mutable, const char* name)
{
Parameter* tmp = new TypedParameter<int>(this, _value, _mutable, name);
owned.push_back(tmp);
return tmp;
}

numval_t ExpressionContext_Objects::_param(double _value, bool _mutable, const char* name)
{
Parameter* tmp = new TypedParameter<double>(this, _value, _mutable, name);
owned.push_back(tmp);
return tmp;
}

