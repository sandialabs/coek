#pragma once

typedef void* expr_t;

//
// Class used to own expression objects that are dynamically constructed
//
class ExpressionContext 
{
public:

    //
    //----- CORE EXPRESSION METHODS -----
    //

    virtual expr_t negate(expr_t) = 0;

    virtual expr_t plus(expr_t, expr_t) = 0;

    virtual expr_t minus(expr_t, expr_t) = 0;

    virtual expr_t times(expr_t, expr_t) = 0;

    virtual expr_t divide(expr_t, expr_t) = 0;

    virtual expr_t abs(expr_t) = 0;

    virtual expr_t pow(expr_t, expr_t) = 0;

    virtual expr_t ceil(expr_t) = 0;

    virtual expr_t floor(expr_t) = 0;

    virtual expr_t exp(expr_t) = 0;

    virtual expr_t log(expr_t) = 0;

    virtual expr_t log10(expr_t) = 0;

    virtual expr_t sqrt(expr_t) = 0;

    virtual expr_t sin(expr_t) = 0;

    virtual expr_t cos(expr_t) = 0;

    virtual expr_t tan(expr_t) = 0;

    virtual expr_t sinh(expr_t) = 0;

    virtual expr_t cosh(expr_t) = 0;

    virtual expr_t tanh(expr_t) = 0;

    virtual expr_t asin(expr_t) = 0;

    virtual expr_t acos(expr_t) = 0;

    virtual expr_t atan(expr_t) = 0;

    virtual expr_t asinh(expr_t) = 0;

    virtual expr_t acosh(expr_t) = 0;

    virtual expr_t atanh(expr_t) = 0;

    //
    //----- CONSTRAINT METHODS -----
    //

    virtual expr_t inequality(expr_t, bool) = 0;

    virtual expr_t equality(expr_t) = 0;

    //
    //----- NUMERIC VALUES -----
    //

    virtual expr_t var(bool, bool, double, double, double, const char*) = 0;

    virtual expr_t param(int, bool, const char*) = 0;

    virtual expr_t param(double, bool, const char*) = 0;

    expr_t one;
    expr_t zero;
    expr_t negative_one;

};

