#include "expr/expr_types.hpp"
#include "expr/expr_manager_objects.hpp"


ExprManager_Objects::~ExprManager_Objects(void)
{
for (std::list<NumericValue*>::iterator it=owned.begin(); it != owned.end(); it++)
    delete *it;
}

ExprManager_Objects::numval_t ExprManager_Objects::negate(numval_t expr)
{
NumericValue* e = static_cast<NumericValue*>(expr);
Expression* tmp = new NegExpression(e->manager, e);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::plus(numval_t lhs, numval_t rhs)
{
if (lhs == &ZeroParameter)
    return rhs;
if (rhs == &ZeroParameter)
    return lhs;

assert(lhs->manager == rhs->manager);

NumericValue* tmp;
if (lhs->is_parameter() and !lhs->is_mutable_parameter() and rhs->is_parameter() and !rhs->is_mutable_parameter())
    tmp = new TypedParameter<double>(lhs->manager, lhs->_value + rhs->_value, false);
else
    tmp = new AddExpression<NumericValue*,NumericValue*>(lhs->manager, lhs,rhs);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::minus(numval_t lhs, numval_t rhs)
{
if (lhs == &ZeroParameter)
    return this->negate(rhs);
if (rhs == &ZeroParameter)
    return lhs;

assert(lhs->manager == rhs->manager);

NumericValue* tmp;
if (lhs->is_parameter() and !lhs->is_mutable_parameter() and rhs->is_parameter() and !rhs->is_mutable_parameter())
    tmp = new TypedParameter<double>(lhs->manager, lhs->_value - rhs->_value, false);
else
    tmp = new SubExpression(lhs->manager, lhs, rhs);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::times(numval_t lhs, numval_t rhs)
{
if (lhs == &ZeroParameter or rhs == &ZeroParameter)
    return &ZeroParameter;
if (lhs == &OneParameter)
    return rhs;
if (rhs == &OneParameter)
    return lhs;
if (lhs == &NegativeOneParameter)
    return this->negate(rhs);
if (rhs == &NegativeOneParameter)
    return this->negate(lhs);

assert(lhs->manager == rhs->manager);

NumericValue* tmp;
if (lhs->is_parameter() and !lhs->is_mutable_parameter() and rhs->is_parameter() and !rhs->is_mutable_parameter())
    tmp = new TypedParameter<double>(lhs->manager, lhs->_value * rhs->_value, false);
else
    tmp = new MulExpression<NumericValue*,NumericValue*>(lhs->manager, lhs, rhs);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::divide(numval_t lhs, numval_t rhs)
{
if (lhs == &ZeroParameter)
    return &ZeroParameter;
if (rhs == &OneParameter)
    return lhs;
if (rhs == &NegativeOneParameter)
    return this->negate(lhs);

assert(lhs->manager == rhs->manager);

if (rhs->is_parameter() and !rhs->is_mutable_parameter() and (rhs->_value==0.0))
   return 0;

NumericValue* tmp;
if (lhs->is_parameter() and !lhs->is_mutable_parameter() and rhs->is_parameter() and !rhs->is_mutable_parameter()) {
    tmp = new TypedParameter<double>(lhs->manager, lhs->_value / rhs->_value, false);
    }
else {
    tmp = new DivExpression(lhs->manager, lhs, rhs);
    }
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::abs(numval_t expr)
{
Expression* tmp = new AbsExpression(expr->manager, expr);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::pow(numval_t lhs, numval_t rhs)
{
if (lhs == &ZeroParameter)
    return &ZeroParameter;
if (lhs == &OneParameter)
    return &OneParameter;
if (rhs == &ZeroParameter)
    return &OneParameter;
if (rhs == &OneParameter)
    return lhs;
assert(lhs->manager == rhs->manager);

Expression* tmp = new PowExpression(lhs->manager, lhs, rhs);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::ceil(numval_t expr)
{
Expression* tmp = new CeilExpression(expr->manager, expr);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::floor(numval_t expr)
{
Expression* tmp = new FloorExpression(expr->manager, expr);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::exp(numval_t expr)
{
Expression* tmp = new ExpExpression(expr->manager, expr);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::log(numval_t expr)
{
Expression* tmp = new LogExpression(expr->manager, expr);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::log10(numval_t expr)
{
Expression* tmp = new Log10Expression(expr->manager, expr);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::sqrt(numval_t expr)
{
Expression* tmp = new SqrtExpression(expr->manager, expr);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::sin(numval_t expr)
{
Expression* tmp = new SinExpression(expr->manager, expr);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::cos(numval_t expr)
{
Expression* tmp = new CosExpression(expr->manager, expr);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::tan(numval_t expr)
{
Expression* tmp = new TanExpression(expr->manager, expr);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::sinh(numval_t expr)
{
Expression* tmp = new SinhExpression(expr->manager, expr);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::cosh(numval_t expr)
{
Expression* tmp = new CoshExpression(expr->manager, expr);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::tanh(numval_t expr)
{
Expression* tmp = new TanhExpression(expr->manager, expr);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::asin(numval_t expr)
{
Expression* tmp = new AsinExpression(expr->manager, expr);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::acos(numval_t expr)
{
Expression* tmp = new AcosExpression(expr->manager, expr);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::atan(numval_t expr)
{
Expression* tmp = new AtanExpression(expr->manager, expr);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::asinh(numval_t expr)
{
Expression* tmp = new AsinhExpression(expr->manager, expr);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::acosh(numval_t expr)
{
Expression* tmp = new AcoshExpression(expr->manager, expr);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::atanh(numval_t expr)
{
Expression* tmp = new AtanhExpression(expr->manager, expr);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::inequality(numval_t expr, bool strict)
{
Expression* tmp = new InequalityExpression(expr->manager, expr, strict);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::equality(numval_t expr)
{
Expression* tmp = new EqualityExpression(expr->manager, expr);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::_var(bool _binary, bool _integer, double lb, double ub, double init, const char* name)
{
Variable* tmp = new Variable(this, _binary, _integer, lb, ub, init, name);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::_param(int _value, bool _mutable, const char* name)
{
if (! _mutable) {
    if (_value == 0)
       return &ZeroParameter;
    if (_value == 1)
       return &OneParameter;
    if (_value == -1)
       return &NegativeOneParameter;
    }

Parameter* tmp = new TypedParameter<int>(this, _value, _mutable, name);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::_param(double _value, bool _mutable, const char* name)
{
if (! _mutable) {
    if (_value == 0.0)
       return &ZeroParameter;
    if (_value == 1.0)
       return &OneParameter;
    if (_value == -1.0)
       return &NegativeOneParameter;
    }

Parameter* tmp = new TypedParameter<double>(this, _value, _mutable, name);
owned.push_back(tmp);
return tmp;
}

std::list<std::string> ExprManager_Objects::expr_to_list(apival_t e, bool values)
{
NumericValue* tmp = static_cast<NumericValue*>(e);
return ::expr_to_list(tmp, values);
}

