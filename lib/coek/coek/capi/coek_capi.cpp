
#include <cassert>
#include <cmath>
#include <sstream>

#include "coek_expr.hpp"
#include "coek_model.hpp"

extern "C" void* misc_getnull(void) { return 0; }

extern "C" double misc_getnan(void) { return NAN; }

/********************************************************************/
/********************************************************************/
/* PARAMETER                                                        */
/********************************************************************/
/********************************************************************/

extern "C" void* parameter_create(double value, const char* name)
{
    return new coek::Parameter(value, name);
}

extern "C" void parameter_delete(void* arg)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    delete arg_;
}

extern "C" const char* parameter_get_name(void* arg)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    return arg_->get_name().c_str();
}

extern "C" void parameter_set_value(void* arg, double value)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    return arg_->set_value(value);
}

extern "C" double parameter_get_value(void* arg)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    return arg_->get_value();
}

/* __abs__ */

extern "C" void* parameter__abs__(void* arg)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    return new coek::Expression(coek::abs(*arg_));
}

/* __neg__ */

extern "C" void* parameter__neg__(void* arg)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    return new coek::Expression(-*arg_);
}

/* __add__ */

extern "C" void* parameter__add__expression(void* arg, void* rhs)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    coek::Expression* rhs_ = static_cast<coek::Expression*>(rhs);
    return new coek::Expression(*arg_ + *rhs_);
}

extern "C" void* parameter__add__variable(void* arg, void* rhs)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    coek::Variable* rhs_ = static_cast<coek::Variable*>(rhs);
    return new coek::Expression(*arg_ + *rhs_);
}

extern "C" void* parameter__add__parameter(void* arg, void* rhs)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    coek::Parameter* rhs_ = static_cast<coek::Parameter*>(rhs);
    return new coek::Expression(*arg_ + *rhs_);
}

extern "C" void* parameter__add__int(void* arg, int rhs)
{
    if (rhs == 0)
        return arg;

    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    return new coek::Expression(*arg_ + rhs);
}

extern "C" void* parameter__add__double(void* arg, double rhs)
{
    if (rhs == 0)
        return arg;

    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    return new coek::Expression(*arg_ + rhs);
}

extern "C" void* parameter__radd__int(void* arg, int lhs)
{
    if (lhs == 0)
        return arg;

    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    return new coek::Expression(lhs + *arg_);
}

extern "C" void* parameter__radd__double(void* arg, double lhs)
{
    if (lhs == 0)
        return arg;

    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    return new coek::Expression(lhs + *arg_);
}

/* __sub__ */

extern "C" void* parameter__sub__expression(void* arg, void* rhs)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    coek::Expression* rhs_ = static_cast<coek::Expression*>(rhs);
    return new coek::Expression(*arg_ - *rhs_);
}

extern "C" void* parameter__sub__variable(void* arg, void* rhs)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    coek::Variable* rhs_ = static_cast<coek::Variable*>(rhs);
    return new coek::Expression(*arg_ - *rhs_);
}

extern "C" void* parameter__sub__parameter(void* arg, void* rhs)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    coek::Parameter* rhs_ = static_cast<coek::Parameter*>(rhs);
    return new coek::Expression(*arg_ - *rhs_);
}

extern "C" void* parameter__sub__int(void* arg, int rhs)
{
    if (rhs == 0)
        return arg;

    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    return new coek::Expression(*arg_ - rhs);
}

extern "C" void* parameter__sub__double(void* arg, double rhs)
{
    if (rhs == 0)
        return arg;

    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    return new coek::Expression(*arg_ - rhs);
}

extern "C" void* parameter__rsub__int(void* arg, int lhs)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    return new coek::Expression(lhs + *arg_);
}

extern "C" void* parameter__rsub__double(void* arg, double lhs)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    return new coek::Expression(lhs - *arg_);
}

/* __mul__ */

extern "C" void* parameter__mul__expression(void* arg, void* rhs)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    coek::Expression* rhs_ = static_cast<coek::Expression*>(rhs);
    return new coek::Expression(*arg_ * *rhs_);
}

extern "C" void* parameter__mul__variable(void* arg, void* rhs)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    coek::Variable* rhs_ = static_cast<coek::Variable*>(rhs);
    return new coek::Expression(*arg_ * *rhs_);
}

extern "C" void* parameter__mul__parameter(void* arg, void* rhs)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    coek::Parameter* rhs_ = static_cast<coek::Parameter*>(rhs);
    return new coek::Expression(*arg_ * *rhs_);
}

extern "C" void* parameter__mul__int(void* arg, int rhs)
{
    if (rhs == 1)
        return arg;

    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    return new coek::Expression(*arg_ * rhs);
}

extern "C" void* parameter__mul__double(void* arg, double rhs)
{
    if (rhs == 1)
        return arg;

    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    return new coek::Expression(*arg_ * rhs);
}

extern "C" void* parameter__rmul__int(void* arg, int lhs)
{
    if (lhs == 1)
        return arg;

    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    return new coek::Expression(lhs * *arg_);
}

extern "C" void* parameter__rmul__double(void* arg, double lhs)
{
    if (lhs == 1)
        return arg;

    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    return new coek::Expression(lhs * *arg_);
}

/* __div__ */

extern "C" void* parameter__div__expression(void* arg, void* rhs)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    coek::Expression* rhs_ = static_cast<coek::Expression*>(rhs);
    return new coek::Expression(*arg_ / *rhs_);
}

extern "C" void* parameter__div__variable(void* arg, void* rhs)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    coek::Variable* rhs_ = static_cast<coek::Variable*>(rhs);
    return new coek::Expression(*arg_ / *rhs_);
}

extern "C" void* parameter__div__parameter(void* arg, void* rhs)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    coek::Parameter* rhs_ = static_cast<coek::Parameter*>(rhs);
    return new coek::Expression(*arg_ / *rhs_);
}

extern "C" void* parameter__div__int(void* arg, int rhs)
{
    if (rhs == 1)
        return arg;

    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    return new coek::Expression(*arg_ / rhs);
}

extern "C" void* parameter__div__double(void* arg, double rhs)
{
    if (rhs == 1)
        return arg;

    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    return new coek::Expression(*arg_ / rhs);
}

extern "C" void* parameter__rdiv__int(void* arg, int lhs)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    return new coek::Expression(lhs / *arg_);
}

extern "C" void* parameter__rdiv__double(void* arg, double lhs)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    return new coek::Expression(lhs / *arg_);
}

/* __pow__ */

extern "C" void* parameter__pow__expression(void* arg, void* rhs)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    coek::Expression* rhs_ = static_cast<coek::Expression*>(rhs);
    return new coek::Expression(coek::pow(*arg_, *rhs_));
}

extern "C" void* parameter__pow__variable(void* arg, void* rhs)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    coek::Variable* rhs_ = static_cast<coek::Variable*>(rhs);
    return new coek::Expression(coek::pow(*arg_, *rhs_));
}

extern "C" void* parameter__pow__parameter(void* arg, void* rhs)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    coek::Parameter* rhs_ = static_cast<coek::Parameter*>(rhs);
    return new coek::Expression(coek::pow(*arg_, *rhs_));
}

extern "C" void* parameter__pow__int(void* arg, int rhs)
{
    if (rhs == 1)
        return arg;

    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    return new coek::Expression(coek::pow(*arg_, rhs));
}

extern "C" void* parameter__pow__double(void* arg, double rhs)
{
    if (rhs == 1)
        return arg;

    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    return new coek::Expression(coek::pow(*arg_, rhs));
}

extern "C" void* parameter__rpow__int(void* arg, int lhs)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    return new coek::Expression(coek::pow(lhs, *arg_));
}

extern "C" void* parameter__rpow__double(void* arg, double lhs)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    return new coek::Expression(coek::pow(lhs, *arg_));
}

/* __lt__ */

extern "C" void* parameter__lt__expression(void* arg, void* rhs)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    coek::Expression* rhs_ = static_cast<coek::Expression*>(rhs);
    return new coek::Constraint(*arg_ < *rhs_);
}

extern "C" void* parameter__lt__variable(void* arg, void* rhs)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    coek::Variable* rhs_ = static_cast<coek::Variable*>(rhs);
    return new coek::Constraint(*arg_ < *rhs_);
}

extern "C" void* parameter__lt__parameter(void* arg, void* rhs)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    coek::Parameter* rhs_ = static_cast<coek::Parameter*>(rhs);
    return new coek::Constraint(*arg_ < *rhs_);
}

extern "C" void* parameter__lt__int(void* arg, int rhs)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    return new coek::Constraint(*arg_ < rhs);
}

extern "C" void* parameter__lt__double(void* arg, double rhs)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    return new coek::Constraint(*arg_ < rhs);
}

/* __le__ */

extern "C" void* parameter__le__expression(void* arg, void* rhs)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    coek::Expression* rhs_ = static_cast<coek::Expression*>(rhs);
    return new coek::Constraint(*arg_ <= *rhs_);
}

extern "C" void* parameter__le__variable(void* arg, void* rhs)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    coek::Variable* rhs_ = static_cast<coek::Variable*>(rhs);
    return new coek::Constraint(*arg_ <= *rhs_);
}

extern "C" void* parameter__le__parameter(void* arg, void* rhs)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    coek::Parameter* rhs_ = static_cast<coek::Parameter*>(rhs);
    return new coek::Constraint(*arg_ <= *rhs_);
}

extern "C" void* parameter__le__int(void* arg, int rhs)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    return new coek::Constraint(*arg_ <= rhs);
}

extern "C" void* parameter__le__double(void* arg, double rhs)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    return new coek::Constraint(*arg_ <= rhs);
}

/* __gt__ */

extern "C" void* parameter__gt__expression(void* arg, void* rhs)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    coek::Expression* rhs_ = static_cast<coek::Expression*>(rhs);
    return new coek::Constraint(*arg_ > *rhs_);
}

extern "C" void* parameter__gt__variable(void* arg, void* rhs)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    coek::Variable* rhs_ = static_cast<coek::Variable*>(rhs);
    return new coek::Constraint(*arg_ > *rhs_);
}

extern "C" void* parameter__gt__parameter(void* arg, void* rhs)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    coek::Parameter* rhs_ = static_cast<coek::Parameter*>(rhs);
    return new coek::Constraint(*arg_ > *rhs_);
}

extern "C" void* parameter__gt__int(void* arg, int rhs)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    return new coek::Constraint(*arg_ > rhs);
}

extern "C" void* parameter__gt__double(void* arg, double rhs)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    return new coek::Constraint(*arg_ > rhs);
}

/* __ge__ */

extern "C" void* parameter__ge__expression(void* arg, void* rhs)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    coek::Expression* rhs_ = static_cast<coek::Expression*>(rhs);
    return new coek::Constraint(*arg_ >= *rhs_);
}

extern "C" void* parameter__ge__variable(void* arg, void* rhs)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    coek::Variable* rhs_ = static_cast<coek::Variable*>(rhs);
    return new coek::Constraint(*arg_ >= *rhs_);
}

extern "C" void* parameter__ge__parameter(void* arg, void* rhs)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    coek::Parameter* rhs_ = static_cast<coek::Parameter*>(rhs);
    return new coek::Constraint(*arg_ >= *rhs_);
}

extern "C" void* parameter__ge__int(void* arg, int rhs)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    return new coek::Constraint(*arg_ >= rhs);
}

extern "C" void* parameter__ge__double(void* arg, double rhs)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    return new coek::Constraint(*arg_ >= rhs);
}

/* __eq__ */

extern "C" void* parameter__eq__expression(void* arg, void* rhs)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    coek::Expression* rhs_ = static_cast<coek::Expression*>(rhs);
    return new coek::Constraint(*arg_ == *rhs_);
}

extern "C" void* parameter__eq__variable(void* arg, void* rhs)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    coek::Variable* rhs_ = static_cast<coek::Variable*>(rhs);
    return new coek::Constraint(*arg_ == *rhs_);
}

extern "C" void* parameter__eq__parameter(void* arg, void* rhs)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    coek::Parameter* rhs_ = static_cast<coek::Parameter*>(rhs);
    return new coek::Constraint(*arg_ == *rhs_);
}

extern "C" void* parameter__eq__int(void* arg, int rhs)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    return new coek::Constraint(*arg_ == rhs);
}

extern "C" void* parameter__eq__double(void* arg, double rhs)
{
    coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);
    return new coek::Constraint(*arg_ == rhs);
}

/********************************************************************/
/********************************************************************/
/* VARIABLE                                                         */
/********************************************************************/
/********************************************************************/

extern "C" void* variable_create(double lb, double ub, double init, bool binary, bool integer,
                                 const char* name)
{
    return new coek::Variable(lb, ub, init, binary, integer, name);
}

extern "C" void variable_delete(void* arg)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    delete arg_;
}

extern "C" const char* variable_get_name(void* arg)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    return arg_->get_name().c_str();
}

extern "C" void variable_set_value(void* arg, double value)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    return arg_->set_value(value);
}

extern "C" double variable_get_value(void* arg)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    return arg_->get_value();
}

extern "C" void variable_set_initial(void* arg, double value)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    return arg_->set_initial(value);
}

extern "C" double variable_get_initial(void* arg)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    return arg_->get_initial();
}

extern "C" void variable_set_lb(void* arg, double value)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    return arg_->set_lb(value);
}

extern "C" double variable_get_lb(void* arg)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    return arg_->get_lb();
}

extern "C" void variable_set_ub(void* arg, double value)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    return arg_->set_ub(value);
}

extern "C" double variable_get_ub(void* arg)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    return arg_->get_ub();
}

extern "C" void variable_set_fixed(void* arg, bool value)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    return arg_->set_fixed(value);
}

extern "C" bool variable_get_fixed(void* arg)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    return arg_->get_fixed();
}

/* __abs__ */

extern "C" void* variable__abs__(void* arg)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    return new coek::Expression(coek::abs(*arg_));
}

/* __neg__ */

extern "C" void* variable__neg__(void* arg)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    return new coek::Expression(-*arg_);
}

/* __add__ */

extern "C" void* variable__add__expression(void* arg, void* rhs)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    coek::Expression* rhs_ = static_cast<coek::Expression*>(rhs);
    return new coek::Expression(*arg_ + *rhs_);
}

extern "C" void* variable__add__variable(void* arg, void* rhs)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    coek::Variable* rhs_ = static_cast<coek::Variable*>(rhs);
    return new coek::Expression(*arg_ + *rhs_);
}

extern "C" void* variable__add__parameter(void* arg, void* rhs)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    coek::Parameter* rhs_ = static_cast<coek::Parameter*>(rhs);
    return new coek::Expression(*arg_ + *rhs_);
}

extern "C" void* variable__add__int(void* arg, int rhs)
{
    if (rhs == 0)
        return arg;

    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    return new coek::Expression(*arg_ + rhs);
}

extern "C" void* variable__add__double(void* arg, double rhs)
{
    if (rhs == 0)
        return arg;

    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    return new coek::Expression(*arg_ + rhs);
}

extern "C" void* variable__radd__int(void* arg, int lhs)
{
    if (lhs == 0)
        return arg;

    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    return new coek::Expression(lhs + *arg_);
}

extern "C" void* variable__radd__double(void* arg, double lhs)
{
    if (lhs == 0)
        return arg;

    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    return new coek::Expression(lhs + *arg_);
}

/* __sub__ */

extern "C" void* variable__sub__expression(void* arg, void* rhs)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    coek::Expression* rhs_ = static_cast<coek::Expression*>(rhs);
    return new coek::Expression(*arg_ - *rhs_);
}

extern "C" void* variable__sub__variable(void* arg, void* rhs)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    coek::Variable* rhs_ = static_cast<coek::Variable*>(rhs);
    return new coek::Expression(*arg_ - *rhs_);
}

extern "C" void* variable__sub__parameter(void* arg, void* rhs)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    coek::Parameter* rhs_ = static_cast<coek::Parameter*>(rhs);
    return new coek::Expression(*arg_ - *rhs_);
}

extern "C" void* variable__sub__int(void* arg, int rhs)
{
    if (rhs == 0)
        return arg;

    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    return new coek::Expression(*arg_ - rhs);
}

extern "C" void* variable__sub__double(void* arg, double rhs)
{
    if (rhs == 0)
        return arg;

    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    return new coek::Expression(*arg_ - rhs);
}

extern "C" void* variable__rsub__int(void* arg, int lhs)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    return new coek::Expression(lhs + *arg_);
}

extern "C" void* variable__rsub__double(void* arg, double lhs)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    return new coek::Expression(lhs - *arg_);
}

/* __mul__ */

extern "C" void* variable__mul__expression(void* arg, void* rhs)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    coek::Expression* rhs_ = static_cast<coek::Expression*>(rhs);
    return new coek::Expression(*arg_ * *rhs_);
}

extern "C" void* variable__mul__variable(void* arg, void* rhs)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    coek::Variable* rhs_ = static_cast<coek::Variable*>(rhs);
    return new coek::Expression(*arg_ * *rhs_);
}

extern "C" void* variable__mul__parameter(void* arg, void* rhs)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    coek::Parameter* rhs_ = static_cast<coek::Parameter*>(rhs);
    return new coek::Expression(*arg_ * *rhs_);
}

extern "C" void* variable__mul__int(void* arg, int rhs)
{
    if (rhs == 1)
        return arg;

    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    return new coek::Expression(*arg_ * rhs);
}

extern "C" void* variable__mul__double(void* arg, double rhs)
{
    if (rhs == 1)
        return arg;

    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    return new coek::Expression(*arg_ * rhs);
}

extern "C" void* variable__rmul__int(void* arg, int lhs)
{
    if (lhs == 1)
        return arg;

    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    return new coek::Expression(lhs * *arg_);
}

extern "C" void* variable__rmul__double(void* arg, double lhs)
{
    if (lhs == 1)
        return arg;

    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    return new coek::Expression(lhs * *arg_);
}

/* __div__ */

extern "C" void* variable__div__expression(void* arg, void* rhs)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    coek::Expression* rhs_ = static_cast<coek::Expression*>(rhs);
    return new coek::Expression(*arg_ / *rhs_);
}

extern "C" void* variable__div__variable(void* arg, void* rhs)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    coek::Variable* rhs_ = static_cast<coek::Variable*>(rhs);
    return new coek::Expression(*arg_ / *rhs_);
}

extern "C" void* variable__div__parameter(void* arg, void* rhs)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    coek::Parameter* rhs_ = static_cast<coek::Parameter*>(rhs);
    return new coek::Expression(*arg_ / *rhs_);
}

extern "C" void* variable__div__int(void* arg, int rhs)
{
    if (rhs == 1)
        return arg;

    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    return new coek::Expression(*arg_ / rhs);
}

extern "C" void* variable__div__double(void* arg, double rhs)
{
    if (rhs == 1)
        return arg;

    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    return new coek::Expression(*arg_ / rhs);
}

extern "C" void* variable__rdiv__int(void* arg, int lhs)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    return new coek::Expression(lhs / *arg_);
}

extern "C" void* variable__rdiv__double(void* arg, double lhs)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    return new coek::Expression(lhs / *arg_);
}

/* __pow__ */

extern "C" void* variable__pow__expression(void* arg, void* rhs)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    coek::Expression* rhs_ = static_cast<coek::Expression*>(rhs);
    return new coek::Expression(coek::pow(*arg_, *rhs_));
}

extern "C" void* variable__pow__variable(void* arg, void* rhs)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    coek::Variable* rhs_ = static_cast<coek::Variable*>(rhs);
    return new coek::Expression(coek::pow(*arg_, *rhs_));
}

extern "C" void* variable__pow__parameter(void* arg, void* rhs)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    coek::Parameter* rhs_ = static_cast<coek::Parameter*>(rhs);
    return new coek::Expression(coek::pow(*arg_, *rhs_));
}

extern "C" void* variable__pow__int(void* arg, int rhs)
{
    if (rhs == 1)
        return arg;

    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    return new coek::Expression(coek::pow(*arg_, rhs));
}

extern "C" void* variable__pow__double(void* arg, double rhs)
{
    if (rhs == 1)
        return arg;

    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    return new coek::Expression(coek::pow(*arg_, rhs));
}

extern "C" void* variable__rpow__int(void* arg, int lhs)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    return new coek::Expression(coek::pow(lhs, *arg_));
}

extern "C" void* variable__rpow__double(void* arg, double lhs)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    return new coek::Expression(coek::pow(lhs, *arg_));
}

/* __lt__ */

extern "C" void* variable__lt__expression(void* arg, void* rhs)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    coek::Expression* rhs_ = static_cast<coek::Expression*>(rhs);
    return new coek::Constraint(*arg_ < *rhs_);
}

extern "C" void* variable__lt__variable(void* arg, void* rhs)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    coek::Variable* rhs_ = static_cast<coek::Variable*>(rhs);
    return new coek::Constraint(*arg_ < *rhs_);
}

extern "C" void* variable__lt__parameter(void* arg, void* rhs)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    coek::Parameter* rhs_ = static_cast<coek::Parameter*>(rhs);
    return new coek::Constraint(*arg_ < *rhs_);
}

extern "C" void* variable__lt__int(void* arg, int rhs)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    return new coek::Constraint(*arg_ < rhs);
}

extern "C" void* variable__lt__double(void* arg, double rhs)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    return new coek::Constraint(*arg_ < rhs);
}

/* __le__ */

extern "C" void* variable__le__expression(void* arg, void* rhs)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    coek::Expression* rhs_ = static_cast<coek::Expression*>(rhs);
    return new coek::Constraint(*arg_ <= *rhs_);
}

extern "C" void* variable__le__variable(void* arg, void* rhs)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    coek::Variable* rhs_ = static_cast<coek::Variable*>(rhs);
    return new coek::Constraint(*arg_ <= *rhs_);
}

extern "C" void* variable__le__parameter(void* arg, void* rhs)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    coek::Parameter* rhs_ = static_cast<coek::Parameter*>(rhs);
    return new coek::Constraint(*arg_ <= *rhs_);
}

extern "C" void* variable__le__int(void* arg, int rhs)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    return new coek::Constraint(*arg_ <= rhs);
}

extern "C" void* variable__le__double(void* arg, double rhs)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    return new coek::Constraint(*arg_ <= rhs);
}

/* __gt__ */

extern "C" void* variable__gt__expression(void* arg, void* rhs)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    coek::Expression* rhs_ = static_cast<coek::Expression*>(rhs);
    return new coek::Constraint(*arg_ > *rhs_);
}

extern "C" void* variable__gt__variable(void* arg, void* rhs)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    coek::Variable* rhs_ = static_cast<coek::Variable*>(rhs);
    return new coek::Constraint(*arg_ > *rhs_);
}

extern "C" void* variable__gt__parameter(void* arg, void* rhs)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    coek::Parameter* rhs_ = static_cast<coek::Parameter*>(rhs);
    return new coek::Constraint(*arg_ > *rhs_);
}

extern "C" void* variable__gt__int(void* arg, int rhs)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    return new coek::Constraint(*arg_ > rhs);
}

extern "C" void* variable__gt__double(void* arg, double rhs)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    return new coek::Constraint(*arg_ > rhs);
}

/* __ge__ */

extern "C" void* variable__ge__expression(void* arg, void* rhs)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    coek::Expression* rhs_ = static_cast<coek::Expression*>(rhs);
    return new coek::Constraint(*arg_ >= *rhs_);
}

extern "C" void* variable__ge__variable(void* arg, void* rhs)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    coek::Variable* rhs_ = static_cast<coek::Variable*>(rhs);
    return new coek::Constraint(*arg_ >= *rhs_);
}

extern "C" void* variable__ge__parameter(void* arg, void* rhs)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    coek::Parameter* rhs_ = static_cast<coek::Parameter*>(rhs);
    return new coek::Constraint(*arg_ >= *rhs_);
}

extern "C" void* variable__ge__int(void* arg, int rhs)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    return new coek::Constraint(*arg_ >= rhs);
}

extern "C" void* variable__ge__double(void* arg, double rhs)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    return new coek::Constraint(*arg_ >= rhs);
}

/* __eq__ */

extern "C" void* variable__eq__expression(void* arg, void* rhs)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    coek::Expression* rhs_ = static_cast<coek::Expression*>(rhs);
    return new coek::Constraint(*arg_ == *rhs_);
}

extern "C" void* variable__eq__variable(void* arg, void* rhs)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    coek::Variable* rhs_ = static_cast<coek::Variable*>(rhs);
    return new coek::Constraint(*arg_ == *rhs_);
}

extern "C" void* variable__eq__parameter(void* arg, void* rhs)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    coek::Parameter* rhs_ = static_cast<coek::Parameter*>(rhs);
    return new coek::Constraint(*arg_ == *rhs_);
}

extern "C" void* variable__eq__int(void* arg, int rhs)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    return new coek::Constraint(*arg_ == rhs);
}

extern "C" void* variable__eq__double(void* arg, double rhs)
{
    coek::Variable* arg_ = static_cast<coek::Variable*>(arg);
    return new coek::Constraint(*arg_ == rhs);
}

/********************************************************************/
/********************************************************************/
/* EXPRESSION                                                       */
/********************************************************************/
/********************************************************************/

extern "C" void* expression_create() { return new coek::Expression(); }

extern "C" void expression_delete(void* arg)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    delete arg_;
}

extern "C" double expression_get_value(void* arg)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    return arg_->get_value();
}

/* __abs__ */

extern "C" void* expression__abs__(void* arg)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    return new coek::Expression(coek::abs(*arg_));
}

/* __neg__ */

extern "C" void* expression__neg__(void* arg)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    return new coek::Expression(-*arg_);
}

/* __add__ */

extern "C" void* expression__add__expression(void* arg, void* rhs)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    coek::Expression* rhs_ = static_cast<coek::Expression*>(rhs);
    return new coek::Expression(*arg_ + *rhs_);
}

extern "C" void* expression__add__variable(void* arg, void* rhs)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    coek::Variable* rhs_ = static_cast<coek::Variable*>(rhs);
    return new coek::Expression(*arg_ + *rhs_);
}

extern "C" void* expression__add__parameter(void* arg, void* rhs)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    coek::Parameter* rhs_ = static_cast<coek::Parameter*>(rhs);
    return new coek::Expression(*arg_ + *rhs_);
}

extern "C" void* expression__add__int(void* arg, int rhs)
{
    if (rhs == 0)
        return arg;

    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    return new coek::Expression(*arg_ + rhs);
}

extern "C" void* expression__add__double(void* arg, double rhs)
{
    if (rhs == 0)
        return arg;

    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    return new coek::Expression(*arg_ + rhs);
}

extern "C" void* expression__radd__int(void* arg, int lhs)
{
    if (lhs == 0)
        return arg;

    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    return new coek::Expression(lhs + *arg_);
}

extern "C" void* expression__radd__double(void* arg, double lhs)
{
    if (lhs == 0)
        return arg;

    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    return new coek::Expression(lhs + *arg_);
}

/* __sub__ */

extern "C" void* expression__sub__expression(void* arg, void* rhs)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    coek::Expression* rhs_ = static_cast<coek::Expression*>(rhs);
    return new coek::Expression(*arg_ - *rhs_);
}

extern "C" void* expression__sub__variable(void* arg, void* rhs)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    coek::Variable* rhs_ = static_cast<coek::Variable*>(rhs);
    return new coek::Expression(*arg_ - *rhs_);
}

extern "C" void* expression__sub__parameter(void* arg, void* rhs)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    coek::Parameter* rhs_ = static_cast<coek::Parameter*>(rhs);
    return new coek::Expression(*arg_ - *rhs_);
}

extern "C" void* expression__sub__int(void* arg, int rhs)
{
    if (rhs == 0)
        return arg;

    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    return new coek::Expression(*arg_ - rhs);
}

extern "C" void* expression__sub__double(void* arg, double rhs)
{
    if (rhs == 0)
        return arg;

    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    return new coek::Expression(*arg_ - rhs);
}

extern "C" void* expression__rsub__int(void* arg, int lhs)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    return new coek::Expression(lhs + *arg_);
}

extern "C" void* expression__rsub__double(void* arg, double lhs)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    return new coek::Expression(lhs - *arg_);
}

/* __mul__ */

extern "C" void* expression__mul__expression(void* arg, void* rhs)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    coek::Expression* rhs_ = static_cast<coek::Expression*>(rhs);
    return new coek::Expression(*arg_ * *rhs_);
}

extern "C" void* expression__mul__variable(void* arg, void* rhs)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    coek::Variable* rhs_ = static_cast<coek::Variable*>(rhs);
    return new coek::Expression(*arg_ * *rhs_);
}

extern "C" void* expression__mul__parameter(void* arg, void* rhs)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    coek::Parameter* rhs_ = static_cast<coek::Parameter*>(rhs);
    return new coek::Expression(*arg_ * *rhs_);
}

extern "C" void* expression__mul__int(void* arg, int rhs)
{
    if (rhs == 1)
        return arg;

    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    return new coek::Expression(*arg_ * rhs);
}

extern "C" void* expression__mul__double(void* arg, double rhs)
{
    if (rhs == 1)
        return arg;

    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    return new coek::Expression(*arg_ * rhs);
}

extern "C" void* expression__rmul__int(void* arg, int lhs)
{
    if (lhs == 1)
        return arg;

    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    return new coek::Expression(lhs * *arg_);
}

extern "C" void* expression__rmul__double(void* arg, double lhs)
{
    if (lhs == 1)
        return arg;

    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    return new coek::Expression(lhs * *arg_);
}

/* __div__ */

extern "C" void* expression__div__expression(void* arg, void* rhs)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    coek::Expression* rhs_ = static_cast<coek::Expression*>(rhs);
    return new coek::Expression(*arg_ / *rhs_);
}

extern "C" void* expression__div__variable(void* arg, void* rhs)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    coek::Variable* rhs_ = static_cast<coek::Variable*>(rhs);
    return new coek::Expression(*arg_ / *rhs_);
}

extern "C" void* expression__div__parameter(void* arg, void* rhs)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    coek::Parameter* rhs_ = static_cast<coek::Parameter*>(rhs);
    return new coek::Expression(*arg_ / *rhs_);
}

extern "C" void* expression__div__int(void* arg, int rhs)
{
    if (rhs == 1)
        return arg;

    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    return new coek::Expression(*arg_ / rhs);
}

extern "C" void* expression__div__double(void* arg, double rhs)
{
    if (rhs == 1)
        return arg;

    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    return new coek::Expression(*arg_ / rhs);
}

extern "C" void* expression__rdiv__int(void* arg, int lhs)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    return new coek::Expression(lhs / *arg_);
}

extern "C" void* expression__rdiv__double(void* arg, double lhs)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    return new coek::Expression(lhs / *arg_);
}

/* __pow__ */

extern "C" void* expression__pow__expression(void* arg, void* rhs)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    coek::Expression* rhs_ = static_cast<coek::Expression*>(rhs);
    return new coek::Expression(coek::pow(*arg_, *rhs_));
}

extern "C" void* expression__pow__variable(void* arg, void* rhs)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    coek::Variable* rhs_ = static_cast<coek::Variable*>(rhs);
    return new coek::Expression(coek::pow(*arg_, *rhs_));
}

extern "C" void* expression__pow__parameter(void* arg, void* rhs)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    coek::Parameter* rhs_ = static_cast<coek::Parameter*>(rhs);
    return new coek::Expression(coek::pow(*arg_, *rhs_));
}

extern "C" void* expression__pow__int(void* arg, int rhs)
{
    if (rhs == 1)
        return arg;

    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    return new coek::Expression(coek::pow(*arg_, rhs));
}

extern "C" void* expression__pow__double(void* arg, double rhs)
{
    if (rhs == 1)
        return arg;

    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    return new coek::Expression(coek::pow(*arg_, rhs));
}

extern "C" void* expression__rpow__int(void* arg, int lhs)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    return new coek::Expression(coek::pow(lhs, *arg_));
}

extern "C" void* expression__rpow__double(void* arg, double lhs)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    return new coek::Expression(coek::pow(lhs, *arg_));
}

/* __lt__ */

extern "C" void* expression__lt__expression(void* arg, void* rhs)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    coek::Expression* rhs_ = static_cast<coek::Expression*>(rhs);
    return new coek::Constraint(*arg_ < *rhs_);
}

extern "C" void* expression__lt__variable(void* arg, void* rhs)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    coek::Variable* rhs_ = static_cast<coek::Variable*>(rhs);
    return new coek::Constraint(*arg_ < *rhs_);
}

extern "C" void* expression__lt__parameter(void* arg, void* rhs)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    coek::Parameter* rhs_ = static_cast<coek::Parameter*>(rhs);
    return new coek::Constraint(*arg_ < *rhs_);
}

extern "C" void* expression__lt__int(void* arg, int rhs)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    return new coek::Constraint(*arg_ < rhs);
}

extern "C" void* expression__lt__double(void* arg, double rhs)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    return new coek::Constraint(*arg_ < rhs);
}

/* __le__ */

extern "C" void* expression__le__expression(void* arg, void* rhs)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    coek::Expression* rhs_ = static_cast<coek::Expression*>(rhs);
    return new coek::Constraint(*arg_ <= *rhs_);
}

extern "C" void* expression__le__variable(void* arg, void* rhs)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    coek::Variable* rhs_ = static_cast<coek::Variable*>(rhs);
    return new coek::Constraint(*arg_ <= *rhs_);
}

extern "C" void* expression__le__parameter(void* arg, void* rhs)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    coek::Parameter* rhs_ = static_cast<coek::Parameter*>(rhs);
    return new coek::Constraint(*arg_ <= *rhs_);
}

extern "C" void* expression__le__int(void* arg, int rhs)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    return new coek::Constraint(*arg_ <= rhs);
}

extern "C" void* expression__le__double(void* arg, double rhs)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    return new coek::Constraint(*arg_ <= rhs);
}

/* __gt__ */

extern "C" void* expression__gt__expression(void* arg, void* rhs)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    coek::Expression* rhs_ = static_cast<coek::Expression*>(rhs);
    return new coek::Constraint(*arg_ > *rhs_);
}

extern "C" void* expression__gt__variable(void* arg, void* rhs)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    coek::Variable* rhs_ = static_cast<coek::Variable*>(rhs);
    return new coek::Constraint(*arg_ > *rhs_);
}

extern "C" void* expression__gt__parameter(void* arg, void* rhs)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    coek::Parameter* rhs_ = static_cast<coek::Parameter*>(rhs);
    return new coek::Constraint(*arg_ > *rhs_);
}

extern "C" void* expression__gt__int(void* arg, int rhs)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    return new coek::Constraint(*arg_ > rhs);
}

extern "C" void* expression__gt__double(void* arg, double rhs)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    return new coek::Constraint(*arg_ > rhs);
}

/* __ge__ */

extern "C" void* expression__ge__expression(void* arg, void* rhs)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    coek::Expression* rhs_ = static_cast<coek::Expression*>(rhs);
    return new coek::Constraint(*arg_ >= *rhs_);
}

extern "C" void* expression__ge__variable(void* arg, void* rhs)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    coek::Variable* rhs_ = static_cast<coek::Variable*>(rhs);
    return new coek::Constraint(*arg_ >= *rhs_);
}

extern "C" void* expression__ge__parameter(void* arg, void* rhs)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    coek::Parameter* rhs_ = static_cast<coek::Parameter*>(rhs);
    return new coek::Constraint(*arg_ >= *rhs_);
}

extern "C" void* expression__ge__int(void* arg, int rhs)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    return new coek::Constraint(*arg_ >= rhs);
}

extern "C" void* expression__ge__double(void* arg, double rhs)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    return new coek::Constraint(*arg_ >= rhs);
}

/* __eq__ */

extern "C" void* expression__eq__expression(void* arg, void* rhs)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    coek::Expression* rhs_ = static_cast<coek::Expression*>(rhs);
    return new coek::Constraint(*arg_ == *rhs_);
}

extern "C" void* expression__eq__variable(void* arg, void* rhs)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    coek::Variable* rhs_ = static_cast<coek::Variable*>(rhs);
    return new coek::Constraint(*arg_ == *rhs_);
}

extern "C" void* expression__eq__parameter(void* arg, void* rhs)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    coek::Parameter* rhs_ = static_cast<coek::Parameter*>(rhs);
    return new coek::Constraint(*arg_ == *rhs_);
}

extern "C" void* expression__eq__int(void* arg, int rhs)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    return new coek::Constraint(*arg_ == rhs);
}

extern "C" void* expression__eq__double(void* arg, double rhs)
{
    coek::Expression* arg_ = static_cast<coek::Expression*>(arg);
    return new coek::Constraint(*arg_ == rhs);
}

/********************************************************************/
/********************************************************************/
/* CONSTRAINT                                                       */
/********************************************************************/
/********************************************************************/

extern "C" void constraint_delete(void* arg)
{
    coek::Constraint* arg_ = static_cast<coek::Constraint*>(arg);
    delete arg_;
}

extern "C" double constraint_get_value(void* arg)
{
    coek::Constraint* arg_ = static_cast<coek::Constraint*>(arg);
    return arg_->get_value();
}

extern "C" double constraint_is_feasible(void* arg)
{
    coek::Constraint* arg_ = static_cast<coek::Constraint*>(arg);
    return arg_->is_feasible();
}

/********************************************************************/
/********************************************************************/
/* INTRINSICS                                                       */
/********************************************************************/
/********************************************************************/

#define INTRINSIC1(FN)                                                \
    extern "C" void* FN##_Parameter(void* arg)                        \
    {                                                                 \
        coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);   \
        return new coek::Expression(coek::FN(*arg_));                 \
    }                                                                 \
                                                                      \
    extern "C" void* FN##_Variable(void* arg)                         \
    {                                                                 \
        coek::Variable* arg_ = static_cast<coek::Variable*>(arg);     \
        return new coek::Expression(coek::FN(*arg_));                 \
    }                                                                 \
                                                                      \
    extern "C" void* FN##_Expression(void* arg)                       \
    {                                                                 \
        coek::Expression* arg_ = static_cast<coek::Expression*>(arg); \
        return new coek::Expression(coek::FN(*arg_));                 \
    }

INTRINSIC1(ceil)
INTRINSIC1(floor)
INTRINSIC1(exp)
INTRINSIC1(log)
INTRINSIC1(log10)
INTRINSIC1(sqrt)
INTRINSIC1(sin)
INTRINSIC1(cos)
INTRINSIC1(tan)
INTRINSIC1(asin)
INTRINSIC1(acos)
INTRINSIC1(atan)
INTRINSIC1(sinh)
INTRINSIC1(cosh)
INTRINSIC1(tanh)
INTRINSIC1(asinh)
INTRINSIC1(acosh)
INTRINSIC1(atanh)

#define INTRINSIC2(FN, TYPE)                                          \
    extern "C" void* FN##_parameter_##TYPE(void* arg, void* arg2)     \
    {                                                                 \
        coek::Parameter* arg_ = static_cast<coek::Parameter*>(arg);   \
        coek::TYPE* arg2_ = static_cast<coek::TYPE*>(arg);            \
        return new coek::Expression(coek::FN(*arg_, *arg2_));         \
    }                                                                 \
                                                                      \
    extern "C" void* FN##_variable_##TYPE(void* arg, void* arg2)      \
    {                                                                 \
        coek::Variable* arg_ = static_cast<coek::Variable*>(arg);     \
        coek::TYPE* arg2_ = static_cast<coek::TYPE*>(arg);            \
        return new coek::Expression(coek::FN(*arg_, *arg2_));         \
    }                                                                 \
                                                                      \
    extern "C" void* FN##_expression_##TYPE(void* arg, void* arg2)    \
    {                                                                 \
        coek::Expression* arg_ = static_cast<coek::Expression*>(arg); \
        coek::TYPE* arg2_ = static_cast<coek::TYPE*>(arg);            \
        return new coek::Expression(coek::FN(*arg_, *arg2_));         \
    }

INTRINSIC2(pow, Parameter)
INTRINSIC2(pow, Variable)
INTRINSIC2(pow, Expression)
