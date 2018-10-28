#include <list> 
#include "expr_types.hpp"

extern std::list<Expression*> expressions;


extern "C" void* intrinsic_abs(void* expr)
{
NumericValue* e = static_cast<NumericValue*>(expr);
return e->context->abs(e);
}

extern "C" void* intrinsic_pow(void* base, void* exponent)
{
NumericValue* b = static_cast<NumericValue*>(base);
NumericValue* e = static_cast<NumericValue*>(exponent);
return e->context->pow(b, e);
}

extern "C" void* intrinsic_ceil(void* expr)
{
NumericValue* e = static_cast<NumericValue*>(expr);
return e->context->ceil(e);
}

extern "C" void* intrinsic_floor(void* expr)
{
NumericValue* e = static_cast<NumericValue*>(expr);
return e->context->floor(e);
}

extern "C" void* intrinsic_exp(void* expr)
{
NumericValue* e = static_cast<NumericValue*>(expr);
return e->context->exp(e);
}

extern "C" void* intrinsic_log(void* expr)
{
NumericValue* e = static_cast<NumericValue*>(expr);
return e->context->log(e);
}

extern "C" void* intrinsic_log10(void* expr)
{
NumericValue* e = static_cast<NumericValue*>(expr);
return e->context->log10(e);
}

extern "C" void* intrinsic_sqrt(void* expr)
{
NumericValue* e = static_cast<NumericValue*>(expr);
return e->context->sqrt(e);
}

extern "C" void* intrinsic_sin(void* expr)
{
NumericValue* e = static_cast<NumericValue*>(expr);
return e->context->sin(e);
}

extern "C" void* intrinsic_cos(void* expr)
{
NumericValue* e = static_cast<NumericValue*>(expr);
return e->context->cos(e);
}

extern "C" void* intrinsic_tan(void* expr)
{
NumericValue* e = static_cast<NumericValue*>(expr);
return e->context->tan(e);
}

extern "C" void* intrinsic_sinh(void* expr)
{
NumericValue* e = static_cast<NumericValue*>(expr);
return e->context->sinh(e);
}

extern "C" void* intrinsic_cosh(void* expr)
{
NumericValue* e = static_cast<NumericValue*>(expr);
return e->context->cosh(e);
}

extern "C" void* intrinsic_tanh(void* expr)
{
NumericValue* e = static_cast<NumericValue*>(expr);
return e->context->tanh(e);
}

extern "C" void* intrinsic_asin(void* expr)
{
NumericValue* e = static_cast<NumericValue*>(expr);
return e->context->asin(e);
}

extern "C" void* intrinsic_acos(void* expr)
{
NumericValue* e = static_cast<NumericValue*>(expr);
return e->context->acos(e);
}

extern "C" void* intrinsic_atan(void* expr)
{
NumericValue* e = static_cast<NumericValue*>(expr);
return e->context->atan(e);
}

extern "C" void* intrinsic_asinh(void* expr)
{
NumericValue* e = static_cast<NumericValue*>(expr);
return e->context->asinh(e);
}

extern "C" void* intrinsic_acosh(void* expr)
{
NumericValue* e = static_cast<NumericValue*>(expr);
return e->context->acosh(e);
}

extern "C" void* intrinsic_atanh(void* expr)
{
NumericValue* e = static_cast<NumericValue*>(expr);
return e->context->atanh(e);
}


