#include <list> 
#include "expr_types.hpp"

extern std::list<Expression*> expressions;

extern "C" void* intrinsic_abs(void* expr)
{
Expression* tmp = new AbsExpression(static_cast<NumericValue*>(expr));
return tmp;
}

extern "C" void* intrinsic_pow(void* base, void* exponent)
{
Expression* tmp = new PowExpression(static_cast<NumericValue*>(base), static_cast<NumericValue*>(exponent));
return tmp;
}

extern "C" void* intrinsic_ceil(void* expr)
{
Expression* tmp = new CeilExpression(static_cast<NumericValue*>(expr));
return tmp;
}

extern "C" void* intrinsic_floor(void* expr)
{
Expression* tmp = new FloorExpression(static_cast<NumericValue*>(expr));
return tmp;
}

extern "C" void* intrinsic_exp(void* expr)
{
Expression* tmp = new ExpExpression(static_cast<NumericValue*>(expr));
return tmp;
}

extern "C" void* intrinsic_log(void* expr)
{
Expression* tmp = new LogExpression(static_cast<NumericValue*>(expr));
return tmp;
}

extern "C" void* intrinsic_log10(void* expr)
{
Expression* tmp = new Log10Expression(static_cast<NumericValue*>(expr));
return tmp;
}

extern "C" void* intrinsic_sqrt(void* expr)
{
Expression* tmp = new SqrtExpression(static_cast<NumericValue*>(expr));
return tmp;
}

extern "C" void* intrinsic_sin(void* expr)
{
Expression* tmp = new SinExpression(static_cast<NumericValue*>(expr));
return tmp;
}

extern "C" void* intrinsic_cos(void* expr)
{
Expression* tmp = new CosExpression(static_cast<NumericValue*>(expr));
return tmp;
}

extern "C" void* intrinsic_tan(void* expr)
{
Expression* tmp = new TanExpression(static_cast<NumericValue*>(expr));
return tmp;
}

extern "C" void* intrinsic_sinh(void* expr)
{
Expression* tmp = new SinhExpression(static_cast<NumericValue*>(expr));
return tmp;
}

extern "C" void* intrinsic_cosh(void* expr)
{
Expression* tmp = new CoshExpression(static_cast<NumericValue*>(expr));
return tmp;
}

extern "C" void* intrinsic_tanh(void* expr)
{
Expression* tmp = new TanhExpression(static_cast<NumericValue*>(expr));
return tmp;
}

extern "C" void* intrinsic_asin(void* expr)
{
Expression* tmp = new AsinExpression(static_cast<NumericValue*>(expr));
return tmp;
}

extern "C" void* intrinsic_acos(void* expr)
{
Expression* tmp = new AcosExpression(static_cast<NumericValue*>(expr));
return tmp;
}

extern "C" void* intrinsic_atan(void* expr)
{
Expression* tmp = new AtanExpression(static_cast<NumericValue*>(expr));
return tmp;
}

extern "C" void* intrinsic_asinh(void* expr)
{
Expression* tmp = new AsinhExpression(static_cast<NumericValue*>(expr));
return tmp;
}

extern "C" void* intrinsic_acosh(void* expr)
{
Expression* tmp = new AcoshExpression(static_cast<NumericValue*>(expr));
return tmp;
}

extern "C" void* intrinsic_atanh(void* expr)
{
Expression* tmp = new AtanhExpression(static_cast<NumericValue*>(expr));
return tmp;
}


