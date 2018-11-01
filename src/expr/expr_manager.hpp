#pragma once

#include <list>
#include <string>
extern "C" {
#include "coek_capi.h"
};

//
// Class used to own expression objects that are dynamically constructed
//
class ExprManager 
{
public:

    virtual ~ExprManager() {}

    //
    //----- CORE EXPRESSION METHODS -----
    //

    virtual apival_t negate(apival_t) = 0;

    virtual apival_t plus(apival_t, apival_t) = 0;

    virtual apival_t minus(apival_t, apival_t) = 0;

    virtual apival_t times(apival_t, apival_t) = 0;

    virtual apival_t divide(apival_t, apival_t) = 0;

    virtual apival_t abs(apival_t) = 0;

    virtual apival_t pow(apival_t, apival_t) = 0;

    virtual apival_t ceil(apival_t) = 0;

    virtual apival_t floor(apival_t) = 0;

    virtual apival_t exp(apival_t) = 0;

    virtual apival_t log(apival_t) = 0;

    virtual apival_t log10(apival_t) = 0;

    virtual apival_t sqrt(apival_t) = 0;

    virtual apival_t sin(apival_t) = 0;

    virtual apival_t cos(apival_t) = 0;

    virtual apival_t tan(apival_t) = 0;

    virtual apival_t sinh(apival_t) = 0;

    virtual apival_t cosh(apival_t) = 0;

    virtual apival_t tanh(apival_t) = 0;

    virtual apival_t asin(apival_t) = 0;

    virtual apival_t acos(apival_t) = 0;

    virtual apival_t atan(apival_t) = 0;

    virtual apival_t asinh(apival_t) = 0;

    virtual apival_t acosh(apival_t) = 0;

    virtual apival_t atanh(apival_t) = 0;

    //
    //----- CONSTRAINT METHODS -----
    //

    virtual apival_t inequality(apival_t, bool) = 0;

    virtual apival_t equality(apival_t) = 0;

    //
    //----- NUMERIC VALUES -----
    //

    virtual apival_t var(bool, bool, double, double, double, const char*) = 0;

    virtual apival_t param(int, bool, const char*) = 0;

    virtual apival_t param(double, bool, const char*) = 0;

    apival_t one;
    apival_t zero;
    apival_t negative_one;

    virtual std::list<std::string> expr_to_list(apival_t e, bool values) = 0;
};

