#pragma once

#include <list>
#include "context.hpp"
#include "expr_types.hpp"


typedef NumericValue* numval_t;

//
// Class that manages expressions with dynamically created objects
//
class ExpressionContext_Objects : public ExpressionContext 
{
public:

    std::list<numval_t> owned;

    ExpressionContext_Objects() 
       : OneParameter(this,1,false),
         ZeroParameter(this,0,false),
         NegativeOneParameter(this,-1,false)
       {
       one = &OneParameter;
       zero = &ZeroParameter;
       negative_one = &NegativeOneParameter;
       }

    ~ExpressionContext_Objects();

    //
    //----- CORE EXPRESSION METHODS -----
    //

    apival_t negate(apival_t expr)
        {return negate(static_cast<numval_t>(expr));}
    numval_t negate(numval_t expr);

    apival_t plus(apival_t lhs, apival_t rhs)
        {return plus(static_cast<numval_t>(lhs), static_cast<numval_t>(rhs));}
    numval_t plus(numval_t, numval_t);

    apival_t minus(apival_t lhs, apival_t rhs)
        {return minus(static_cast<numval_t>(lhs), static_cast<numval_t>(rhs));}
    numval_t minus(numval_t, numval_t);

    apival_t times(apival_t lhs, apival_t rhs)
        {return times(static_cast<numval_t>(lhs), static_cast<numval_t>(rhs));}
    numval_t times(numval_t lhs, numval_t rhs);

    apival_t divide(apival_t lhs, apival_t rhs)
        {return divide(static_cast<numval_t>(lhs), static_cast<numval_t>(rhs));}
    numval_t divide(numval_t lhs, numval_t rhs);

    apival_t abs(apival_t expr)
        {return abs(static_cast<numval_t>(expr));}
    numval_t abs(numval_t expr);

    apival_t pow(apival_t lhs, apival_t rhs)
        {return pow(static_cast<numval_t>(lhs), static_cast<numval_t>(rhs));}
    numval_t pow(numval_t lhs, numval_t rhs);

    apival_t ceil(apival_t expr)
        {return ceil(static_cast<numval_t>(expr));}
    numval_t ceil(numval_t expr);

    apival_t floor(apival_t expr)
        {return floor(static_cast<numval_t>(expr));}
    numval_t floor(numval_t expr);

    apival_t exp(apival_t expr)
        {return exp(static_cast<numval_t>(expr));}
    numval_t exp(numval_t expr);

    apival_t log(apival_t expr)
        {return log(static_cast<numval_t>(expr));}
    numval_t log(numval_t expr);

    apival_t log10(apival_t expr)
        {return log10(static_cast<numval_t>(expr));}
    numval_t log10(numval_t expr);

    apival_t sqrt(apival_t expr)
        {return sqrt(static_cast<numval_t>(expr));}
    numval_t sqrt(numval_t expr);

    apival_t sin(apival_t expr)
        {return sin(static_cast<numval_t>(expr));}
    numval_t sin(numval_t expr);

    apival_t cos(apival_t expr)
        {return cos(static_cast<numval_t>(expr));}
    numval_t cos(numval_t expr);

    apival_t tan(apival_t expr)
        {return tan(static_cast<numval_t>(expr));}
    numval_t tan(numval_t expr);

    apival_t sinh(apival_t expr)
        {return sinh(static_cast<numval_t>(expr));}
    numval_t sinh(numval_t expr);

    apival_t cosh(apival_t expr)
        {return cosh(static_cast<numval_t>(expr));}
    numval_t cosh(numval_t expr);

    apival_t tanh(apival_t expr)
        {return tanh(static_cast<numval_t>(expr));}
    numval_t tanh(numval_t expr);

    apival_t asin(apival_t expr)
        {return asin(static_cast<numval_t>(expr));}
    numval_t asin(numval_t expr);

    apival_t acos(apival_t expr)
        {return acos(static_cast<numval_t>(expr));}
    numval_t acos(numval_t expr);

    apival_t atan(apival_t expr)
        {return atan(static_cast<numval_t>(expr));}
    numval_t atan(numval_t expr);

    apival_t asinh(apival_t expr)
        {return asinh(static_cast<numval_t>(expr));}
    numval_t asinh(numval_t expr);

    apival_t acosh(apival_t expr)
        {return acosh(static_cast<numval_t>(expr));}
    numval_t acosh(numval_t expr);

    apival_t atanh(apival_t expr)
        {return atanh(static_cast<numval_t>(expr));}
    numval_t atanh(numval_t expr);

    //
    //----- CONSTRAINT METHODS -----
    //

    apival_t inequality(apival_t expr, bool strict)
        {return inequality(static_cast<numval_t>(expr), strict);}
    numval_t inequality(numval_t expr, bool strict);

    apival_t equality(apival_t expr)
        {return equality(static_cast<numval_t>(expr));}
    numval_t equality(numval_t expr);

    //
    //----- NUMERIC VALUES -----
    //

    apival_t var(bool _binary, bool _integer, double lb, double ub, double init, const char* name)
      { return _var(_binary, _integer, lb, ub, init, name); }
    numval_t _var(bool _binary, bool _integer, double lb, double ub, double init)
      { return _var(_binary, _integer, lb, ub, init, ""); }
    numval_t _var(bool _binary, bool _integer, double lb, double ub, double init, const char* name);

    apival_t param(int _value, bool _mutable, const char* name)
      { return _param(_value, _mutable, name); }
    numval_t _param(int _value, bool _mutable)
      { return _param(_value, _mutable, ""); }
    numval_t _param(int _value, bool _mutable, const char* name);

    apival_t param(double _value, bool _mutable, const char* name)
      { return _param(_value, _mutable, name); }
    numval_t _param(double _value, bool _mutable)
      { return _param(_value, _mutable, ""); }
    numval_t _param(double _value, bool _mutable, const char* name);

    TypedParameter<int> OneParameter;
    TypedParameter<int> ZeroParameter;
    TypedParameter<int> NegativeOneParameter;

    std::list<std::string> expr_to_list(apival_t e, bool values);

};

