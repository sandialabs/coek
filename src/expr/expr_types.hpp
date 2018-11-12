#pragma once

#include <cassert>
#include <cstdio>
#include <cmath>
#include <iostream>
#include <map>
#include <list>
#include <set>
#include <string>

#include "expr/expr_manager.hpp"


#define POSITIVE_INFINITY 1.0e19
#define NEGATIVE_INFINITY -1.0e19

inline bool isnan(double x) {return 1;}


class NumericValue
{
public:

    ExprManager* manager;

    NumericValue(ExprManager* _manager) : manager(_manager) {}

    virtual ~NumericValue() {}

    double _value;

    double adjoint; /* aO */

    double dO;  /* deriv of op w.r.t. t in x + t*p */
    double adO; /* adjoint (in Hv computation) of dO */

    virtual void print(std::ostream& ostr) = 0;

    virtual bool is_variable() { return false; }
    virtual bool is_expression() { return false; }
    virtual bool is_parameter() { return false; }
    virtual bool is_mutable_parameter() { return false; }

    virtual int size() {return 1;}

    virtual double value() = 0;

    virtual double compute_value() = 0;

    // Used for symbolic differentiation
    virtual NumericValue* partial(unsigned int i) = 0;

    // Used for reverse_ad
    virtual void compute_adjoint() = 0;

    // Used for Hv - forward step
    virtual void compute_hv_fwd() = 0;

    // Used for Hv - backward step
    virtual void compute_hv_back() = 0;

    virtual void snprintf(char* buf, int max)
        {std::snprintf(buf, max, "%.3f", _value);}
};


class Parameter : public NumericValue
{
public:

    bool mutable_flag;

    Parameter(ExprManager* _manager, bool _mutable_flag)
        : NumericValue(_manager), mutable_flag(_mutable_flag) {}

    virtual bool is_parameter() { return true; }

    virtual bool is_mutable_parameter() { return mutable_flag; }

    NumericValue* partial(unsigned int i);

    void compute_adjoint() {}

    void compute_hv_fwd() {}

    void compute_hv_back() {}

    virtual void set_value(double val) = 0;
};


template <typename TYPE>
inline void TypedParameter_snprintf(char* buf, int max, TYPE _value, std::string& name)
{
if (name.size() > 0)
    std::snprintf(buf, max, "%s{%.3f}", name.c_str(), _value);
else
    std::snprintf(buf, max, "%.3f", _value);
}

template <>
inline void TypedParameter_snprintf(char* buf, int max, int _value, std::string& name)
{
if (name.size() > 0)
    std::snprintf(buf, max, "%s{%d}", name.c_str(), _value);
else
    std::snprintf(buf, max, "%d", _value);
}

template <typename TYPE>
class TypedParameter : public Parameter
{
public:

    TYPE _tvalue;
    std::string name;

    TypedParameter(ExprManager* _manager, TYPE __value, bool _mutable_flag, const char* _name) 
        : Parameter(_manager, _mutable_flag)
        {_value = __value; _tvalue = __value; name = _name;}

    TypedParameter(ExprManager* _manager, TYPE __value, bool _mutable_flag) 
        : Parameter(_manager, _mutable_flag)
        {_value = __value; _tvalue = __value;}

    double value() {return _tvalue;}

    double compute_value() {return _tvalue;}

    void print(std::ostream& ostr)
        {
        if (name.size() == 0)
            ostr << 'x' << "{" << _tvalue << '}';
        else
            ostr << name << "{" << _tvalue << '}';
        }

    void snprintf(char* buf, int max)
        {TypedParameter_snprintf(buf, max, this->_tvalue, name);}

    void set_value(double val) 
        { TypedParameter_set_value(this, val); }

};


template <typename TYPE>
inline void TypedParameter_set_value(TypedParameter<TYPE>* param, double val)
{
param->_tvalue = static_cast<TYPE>(val);
param->_value = val;
}

inline NumericValue* Parameter::partial(unsigned int i)
{return static_cast<NumericValue*>(manager->zero);}



class Variable : public NumericValue
{
public:

    static double _nan;
    static int nvariables;
    bool binary;
    bool integer;
    int index;
    std::string name;

    double lb;
    double ub;

    Variable(ExprManager* _manager, bool _binary, bool _integer, double _lb, double _ub, double init, const char* _name) 
        : NumericValue(_manager)
        {
        _value = init;
        binary = _binary; 
        integer = _integer; 
        index = nvariables++;
        if (_name)
            name = _name;
        if (binary) {
            lb = 0;
            ub = 1;
        } else {
            if (isnan(_lb))
                lb = NEGATIVE_INFINITY;
            else
                lb = _lb;
            if (isnan(_ub))
                ub = POSITIVE_INFINITY;
            else
                ub = _ub;
        }
        }

    virtual double value() {return _value;}

    double compute_value() {return _value;}

    void compute_adjoint() {}

    void compute_hv_fwd() {}

    void compute_hv_back() {}

    void print(std::ostream& ostr)
        {
        if (name.size() == 0)
            ostr << 'x' << index << "{" << _value << '}';
        else
            ostr << name << index << "{" << _value << '}';
        }

    bool is_variable() { return true; }

    NumericValue* partial(unsigned int i)
        {return static_cast<NumericValue*>(manager->one);}

    void snprintf(char* buf, int max)
        {
        if (name.size() == 0)
            std::snprintf(buf, max, "x%d{%.3f}", index, this->_value);
        else
            std::snprintf(buf, max, "%s{%.3f}", name.c_str(), this->_value);
        }
};



class Expression: public NumericValue
{
public:

    Expression(ExprManager* _manager) : NumericValue(_manager) {}

    bool is_expression() { return true; }

    void print(std::ostream& ostr) {ostr << "ERROR: Undefined print() method.";}

    virtual unsigned int num_sub_expressions() = 0;

    virtual NumericValue* expression(unsigned int i) = 0;

};


class Constraint : public Expression
{
public:

    Constraint(ExprManager* _manager) : Expression(_manager) {}

    virtual bool boolean_value() = 0;
};


class InequalityExpression : public Constraint
{
public:

    NumericValue* body;
    bool strict;

    InequalityExpression(ExprManager* _manager, NumericValue* _body, bool _strict)
        : Constraint(_manager)
        {body = _body; strict=_strict;}

    void print(std::ostream& ostr) 
        {
        body->print(ostr);
        if (strict)
            ostr << "  <  0";
        else
            ostr << "  <=  0";
        }

    int size() {return body->size();}

    unsigned int num_sub_expressions() {return 1;}

    NumericValue* expression(unsigned int i) { if (i == 0) return body; else return 0; }

    double value() {return body->value();}

    bool boolean_value() {return strict ? value() < 0 : value() <= 0;}

    double compute_value() {_value = body->_value; return _value;}

    virtual NumericValue* partial(unsigned int i) {return 0;}

    void compute_adjoint() {body->adjoint += adjoint;}

    void compute_hv_fwd() {adjoint = body->adjoint;}

    void compute_hv_back() {body->adjoint = adjoint;}

    void snprintf(char* buf, int max)
        {
        if (strict)
            std::snprintf(buf, max, "<");
        else
            std::snprintf(buf, max, "<=");
        }
};


class EqualityExpression : public Constraint
{
public:

    NumericValue* body;

    EqualityExpression(ExprManager* _manager, NumericValue* _body)
        : Constraint(_manager)
        {body = _body;}

    void print(std::ostream& ostr) {body->print(ostr); ostr << "  ==  0";}

    int size() {return body->size();}

    unsigned int num_sub_expressions() {return 1;}

    NumericValue* expression(unsigned int i) { if (i == 0) return body; else return 0; }

    double value() {return body->value();}

    bool boolean_value() {return value() == 0;}

    double compute_value() {_value = body->_value; return _value;}

    virtual NumericValue* partial(unsigned int i) {return 0;}

    void compute_adjoint() {body->adjoint += adjoint;}

    void compute_hv_fwd() {adjoint = body->adjoint;}

    void compute_hv_back() {body->adjoint = adjoint;}

    void snprintf(char* buf, int max)
        {std::snprintf(buf, max, "==");}

};


bool variable_comparator(const Variable* lhs, const Variable* rhs);


/*** UNARY ***/

class UnaryExpression : public Expression
{
public:

    /// Data used for Hessian calculations
    //double O;   /* op value */
    //double aO;  /* adjoint (in Hv computation) of O */
    //double adO; /* adjoint (in Hv computation) of dO */
    double dL;  /* deriv of op w.r.t. left operand L */
    double dL2; /* second partial w.r.t. L,L */
    double dR;  /* deriv of op w.r.t. right operand R */
    double dLR; /* second partial w.r.t. L,R */
    double dR2; /* second partial w.r.t. R,R */

    NumericValue* body;

    std::string name;

    UnaryExpression(ExprManager* _manager, NumericValue* _body) 
        : Expression(_manager)
        {body = _body;}

    unsigned int num_sub_expressions() 
        {
        return 1;
        }

    NumericValue* expression(unsigned int i) 
        {
	  //assert(i == 0);
        return body;
        }

    int size() 
        {return 1 + body->size();}

    void compute_hv_back()
        {
        // TODO
        }

    void print(std::ostream& ostr) 
        {ostr << name << "("; body->print(ostr); ostr << ")";}

    NumericValue* partial(unsigned int i)
	{ assert(0); }

    void compute_adjoint()
	{ assert(0); }

    void compute_hv_fwd()
	{ assert(0); }

    void snprintf(char* buf, int max)
        {std::snprintf(buf, max, "%s", this->name.c_str());}

};


class AbsExpression : public UnaryExpression
{
public:

    AbsExpression(ExprManager* _manager, NumericValue* _body) 
        : UnaryExpression(_manager, _body)
        {this->name = "abs";}

    double value() {return fabs(body->value()); }

    double compute_value()
        { return this->_value = fabs(body->_value); }

    NumericValue* partial(unsigned int i)
        {
        if (body->_value < 0) {
            return static_cast<NumericValue*>(manager->negative_one);
            }
        else if (body->_value > 0) {
            return static_cast<NumericValue*>(manager->one);
            }
        throw std::logic_error("Argument is zero");       // std::invalid_argument
        }

    void compute_adjoint()
        {
        if (body->_value < 0)
            this->body->adjoint -= this->adjoint;
        else if (body->_value > 0)
            this->body->adjoint += this->adjoint;
        //std::cout << "AbsExpression::compute_adjoint " << this->adjoint << " " << this->body->adjoint << std::endl;
        throw std::logic_error("Argument is zero");       // std::invalid_argument
        }

    void compute_hv_fwd()
        {
        // TODO
        }

};


class NegExpression : public UnaryExpression
{
public:

    NegExpression(ExprManager* _manager, NumericValue* _body) 
        : UnaryExpression(_manager, _body)
        {this->name = "neg";}

    double value() { return - body->value(); }

    double compute_value()
        { return this->_value = - body->_value; }

    NumericValue* partial(unsigned int i)
        {
        return static_cast<NumericValue*>(manager->negative_one);
        }

    void compute_adjoint()
        {
        this->body->adjoint -= this->adjoint;
        //std::cout << "NegExpression::compute_adjoint " << this->adjoint << " " << this->body->adjoint << std::endl;
        }

    void compute_hv_fwd()
        {
        // TODO
        }

    void snprintf(char* buf, int max)
        {std::snprintf(buf, max, "-");}

};

class CeilExpression : public UnaryExpression
{
public:

    CeilExpression(ExprManager* _manager, NumericValue* _body) 
        : UnaryExpression(_manager, _body)
        {this->name = "ceil";}

    double value() { return ceil(body->value()); }

    double compute_value()
        { return this->_value = ceil(body->_value); }
};

class FloorExpression : public UnaryExpression
{
public:

    FloorExpression(ExprManager* _manager, NumericValue* _body) 
        : UnaryExpression(_manager, _body)
        {this->name = "floor";}

    double value() {return floor(body->value()); }

    double compute_value()
        { return this->_value = floor(body->_value); }
};

class ExpExpression : public UnaryExpression
{
public:

    ExpExpression(ExprManager* _manager, NumericValue* _body) 
        : UnaryExpression(_manager, _body)
        {this->name = "exp";}

    double value() {return exp(body->value()); }

    double compute_value()
        { return this->_value = exp(body->_value); }
};

class LogExpression : public UnaryExpression
{
public:

    LogExpression(ExprManager* _manager, NumericValue* _body) 
        : UnaryExpression(_manager, _body)
        {this->name = "log";}

    double value() {return log(body->value()); }

    double compute_value()
        { return this->_value = log(body->_value); }
};

class Log10Expression : public UnaryExpression
{
public:

    Log10Expression(ExprManager* _manager, NumericValue* _body) 
        : UnaryExpression(_manager, _body)
        {this->name = "log10";}

    double value() {return log10(body->value()); }

    double compute_value()
        { return this->_value = log10(body->_value); }
};

class SqrtExpression : public UnaryExpression
{
public:

    SqrtExpression(ExprManager* _manager, NumericValue* _body) 
        : UnaryExpression(_manager, _body)
        {this->name = "sqrt";}

    double value() {return sqrt(body->value()); }

    double compute_value()
        { return this->_value = sqrt(body->_value); }
};

class SinExpression : public UnaryExpression
{
public:

    SinExpression(ExprManager* _manager, NumericValue* _body) 
        : UnaryExpression(_manager, _body)
        {this->name = "sin";}

    double value() {return sin(body->value()); }

    double compute_value()
        { return this->_value = sin(body->_value); }
};

class CosExpression : public UnaryExpression
{
public:

    CosExpression(ExprManager* _manager, NumericValue* _body) 
        : UnaryExpression(_manager, _body)
        {this->name = "cos";}

    double value() {return cos(body->value()); }

    double compute_value()
        { return this->_value = cos(body->_value); }
};

class TanExpression : public UnaryExpression
{
public:

    TanExpression(ExprManager* _manager, NumericValue* _body) 
        : UnaryExpression(_manager, _body)
        {this->name = "tan";}

    double value() {return tan(body->value()); }

    double compute_value()
        { return this->_value = tan(body->_value); }
};

class AsinExpression : public UnaryExpression
{
public:

    AsinExpression(ExprManager* _manager, NumericValue* _body) 
        : UnaryExpression(_manager, _body)
        {this->name = "asin";}

    double value() {return asin(body->value()); }

    double compute_value()
        { return this->_value = asin(body->_value); }
};

class AcosExpression : public UnaryExpression
{
public:

    AcosExpression(ExprManager* _manager, NumericValue* _body) 
        : UnaryExpression(_manager, _body)
        {this->name = "acos";}

    double value() {return acos(body->value()); }

    double compute_value()
        { return this->_value = acos(body->_value); }
};

class AtanExpression : public UnaryExpression
{
public:

    AtanExpression(ExprManager* _manager, NumericValue* _body) 
        : UnaryExpression(_manager, _body)
        {this->name = "atan";}

    double value() {return atan(body->value()); }

    double compute_value()
        { return this->_value = atan(body->_value); }
};

class SinhExpression : public UnaryExpression
{
public:

    SinhExpression(ExprManager* _manager, NumericValue* _body) 
        : UnaryExpression(_manager, _body)
        {this->name = "sinh";}

    double value() {return sinh(body->value()); }

    double compute_value()
        { return this->_value = sinh(body->_value); }
};

class CoshExpression : public UnaryExpression
{
public:

    CoshExpression(ExprManager* _manager, NumericValue* _body) 
        : UnaryExpression(_manager, _body)
        {this->name = "cosh";}

    double value() {return cosh(body->value()); }

    double compute_value()
        { return this->_value = cosh(body->_value); }
};

class TanhExpression : public UnaryExpression
{
public:

    TanhExpression(ExprManager* _manager, NumericValue* _body) 
        : UnaryExpression(_manager, _body)
        {this->name = "tanh";}

    double value() {return tanh(body->value()); }

    double compute_value()
        { return this->_value = tanh(body->_value); }
};

class AsinhExpression : public UnaryExpression
{
public:

    AsinhExpression(ExprManager* _manager, NumericValue* _body) 
        : UnaryExpression(_manager, _body)
        {this->name = "asinh";}

    double value() {return asinh(body->value()); }

    double compute_value()
        { return this->_value = asinh(body->_value); }
};

class AcoshExpression : public UnaryExpression
{
public:

    AcoshExpression(ExprManager* _manager, NumericValue* _body) 
        : UnaryExpression(_manager, _body)
        {this->name = "acosh";}

    double value() {return acosh(body->value()); }

    double compute_value()
        { return this->_value = acosh(body->_value); }
};

class AtanhExpression : public UnaryExpression
{
public:

    AtanhExpression(ExprManager* _manager, NumericValue* _body) 
        : UnaryExpression(_manager, _body)
        {this->name = "atanh";}

    double value() {return atanh(body->value()); }

    double compute_value()
        { return this->_value = atanh(body->_value); }
};


/*** BINARY ***/

template <typename LHS, typename RHS>
class BinaryExpression : public Expression
{
public:

    /// Data used for Hessian calculations
    //double O;   /* op value */
    //double aO;  /* adjoint (in Hv computation) of O */
    //double adO; /* adjoint (in Hv computation) of dO */
    double dL;  /* deriv of op w.r.t. left operand L */
    double dL2; /* second partial w.r.t. L,L */
    double dR;  /* deriv of op w.r.t. right operand R */
    double dLR; /* second partial w.r.t. L,R */
    double dR2; /* second partial w.r.t. R,R */

    LHS lhs;
    RHS rhs;

    BinaryExpression(ExprManager* _manager, LHS _lhs, RHS _rhs)
        : Expression(_manager)
        {lhs = _lhs; rhs = _rhs;}

    unsigned int num_sub_expressions() {return BinaryExpression_num_sub_expressions(this);}

    NumericValue* expression(unsigned int i) { return BinaryExpression_expression(i, this); }

    int size() {return BinaryExpression_size(this);}

    void compute_hv_back()
        {
        //e1 = e->L;
        //e2 = e->R;
        //adO = e->adO;
        //std::cout << "HVBACK adjoint=" << this->adjoint << " adO=" << this->adO << std::endl;
        //std::cout << "HVBACK dL=" << this->dL << " dR=" << this->dR << " dL2=" << this->dL2 << " dLR=" << this->dLR << " dR2=" << this->dR2 << std::endl;
        double t1 = this->adO * this->lhs->dO;
        double t2 = this->adO * this->rhs->dO;
        this->lhs->adjoint += this->adjoint*this->dL + t1*this->dL2 + t2*this->dLR;
        this->rhs->adjoint += this->adjoint*this->dR + t1*this->dLR + t2*this->dR2;
        this->lhs->adO += this->adO * this->dL;
        this->rhs->adO += this->adO * this->dR;
        }

};

template <typename LHS, typename RHS>
int BinaryExpression_size(BinaryExpression<LHS, RHS>* expr)
{ return expr->lhs->size() + expr->rhs->size() + 1; }

template <typename LHS>
int BinaryExpression_size(BinaryExpression<LHS, int>* expr)
{ return expr->lhs->size() + 2; }

template <typename LHS>
int BinaryExpression_size(BinaryExpression<LHS, double>* expr)
{ return expr->lhs->size() + 2; }

template <typename RHS>
int BinaryExpression_size(BinaryExpression<int, RHS>* expr)
{ return expr->rhs->size() + 2; }

template <typename RHS>
int BinaryExpression_size(BinaryExpression<double, RHS>* expr)
{ return expr->rhs->size() + 2; }


template <typename LHS, typename RHS>
unsigned int BinaryExpression_num_sub_expressions(BinaryExpression<LHS, RHS>* expr)
{ return 2; }

template <typename LHS>
unsigned int BinaryExpression_num_sub_expressions(BinaryExpression<LHS, int>* expr)
{ return 1; }

template <typename LHS>
unsigned int BinaryExpression_num_sub_expressions(BinaryExpression<LHS, double>* expr)
{ return 1; }

template <typename RHS>
unsigned int BinaryExpression_num_sub_expressions(BinaryExpression<int, RHS>* expr)
{ return 1; }

template <typename RHS>
unsigned int BinaryExpression_num_sub_expressions(BinaryExpression<double, RHS>* expr)
{ return 1; }


template <typename LHS, typename RHS>
NumericValue* BinaryExpression_expression(unsigned int i, BinaryExpression<LHS, RHS>* expr)
{
if (i == 0)
    return expr->lhs;
if (i == 1)
    return expr->rhs;
return 0;
}

template <typename LHS>
NumericValue* BinaryExpression_expression(unsigned int i, BinaryExpression<LHS, int>* expr)
{
if (i == 0)
    return expr->lhs;
return 0;
}

template <typename LHS>
NumericValue* BinaryExpression_expression(unsigned int i, BinaryExpression<LHS, double>* expr)
{
if (i == 0)
    return expr->lhs;
return 0;
}

template <typename RHS>
NumericValue* BinaryExpression_expression(unsigned int i, BinaryExpression<int, RHS>* expr)
{
if (i == 0)
    return expr->rhs;
return 0;
}

template <typename RHS>
NumericValue* BinaryExpression_expression(unsigned int i, BinaryExpression<double, RHS>* expr)
{
if (i == 0)
    return expr->rhs;
return 0;
}


/*** POW ***/

class PowExpression : public BinaryExpression<NumericValue*,NumericValue*>
{
public:

    PowExpression(ExprManager* _manager, NumericValue* _lhs, NumericValue* _rhs) 
        : BinaryExpression<NumericValue*,NumericValue*>(_manager,_lhs,_rhs)
        {}

    void print(std::ostream& ostr)
        {ostr << "("; this->lhs->print(ostr); ostr << ")**("; this->rhs->print(ostr); ostr << ")"; }

    double value()
        {return pow(lhs->value(), rhs->value());}

    double compute_value()
        {
        this->_value = pow(lhs->_value, rhs->_value);
        return this->_value;
        }

    NumericValue* partial(unsigned int i);

    void compute_adjoint();

    // TODO
    void compute_hv_fwd()
        {
        compute_value();
        this->adjoint = 0;
        this->adO = 0;
        this->dL = this->dR = 1;
        this->dL2 = this->dLR = this->dR2 = 0;
        this->dO = this->lhs->dO*this->dL + this->rhs->dO*this->dR;
        }

    void snprintf(char* buf, int max)
        {std::snprintf(buf, max, "**");}

};

/*** ADD ***/

template <typename LHS, typename RHS>
class AddExpression : public BinaryExpression<LHS,RHS>
{
public:

    AddExpression(ExprManager* _manager, LHS _lhs, RHS _rhs) 
        : BinaryExpression<LHS,RHS>(_manager,_lhs,_rhs)
        {}

    void print(std::ostream& ostr) {AddExpression_print(ostr, this);}

    double value() {return AddExpression_value(this);}

    double compute_value()
        {
        this->_value = this->lhs->_value + this->rhs->_value;
        return this->_value;
        }

    NumericValue* partial(unsigned int i)
        {return static_cast<NumericValue*>(this->manager->one);}

    void compute_adjoint()
        {
        this->lhs->adjoint += this->adjoint;
        this->rhs->adjoint += this->adjoint;
        //std::cout << "AddExpression::compute_adjoint " << this->adjoint << " " << this->lhs->adjoint << " " << this->rhs->adjoint << std::endl;
        }

    void compute_hv_fwd()
        {
        compute_value();
        this->adjoint = 0;
        this->adO = 0;
        this->dL = this->dR = 1;
        this->dL2 = this->dLR = this->dR2 = 0;
        this->dO = this->lhs->dO*this->dL + this->rhs->dO*this->dR;
        }

    void snprintf(char* buf, int max)
        {std::snprintf(buf, max, "+");}

};

template <typename LHS, typename RHS>
void AddExpression_print(std::ostream& ostr, AddExpression<LHS, RHS>* expr)
{expr->lhs->print(ostr); ostr << " + "; expr->rhs->print(ostr); }

template <typename LHS>
void AddExpression_print(std::ostream& ostr, AddExpression<LHS, int>* expr)
{ expr->lhs->print(ostr); ostr << " + " << expr->rhs; }

template <typename LHS>
void AddExpression_print(std::ostream& ostr, AddExpression<LHS, double>* expr)
{ expr->lhs->print(ostr); ostr << " + " << expr->rhs; }

template <typename RHS>
void AddExpression_print(std::ostream& ostr, AddExpression<int, RHS>* expr)
{ ostr << expr->lhs << " + "; expr->rhs->print(ostr); }

template <typename RHS>
void AddExpression_print(std::ostream& ostr, AddExpression<double, RHS>* expr)
{ ostr << expr->lhs << " + "; expr->rhs->print(ostr); }


template <typename LHS, typename RHS>
double AddExpression_value(AddExpression<LHS, RHS>* expr)
{ return expr->lhs->value() + expr->rhs->value(); }

template <typename LHS>
double AddExpression_value(AddExpression<LHS, int>* expr)
{ return expr->lhs->value() + expr->rhs; }

template <typename LHS>
double AddExpression_value(AddExpression<LHS, double>* expr)
{ return expr->lhs->value() + expr->rhs; }

template <typename RHS>
double AddExpression_value(AddExpression<int, RHS>* expr)
{ return expr->lhs + expr->rhs->value(); }

template <typename RHS>
double AddExpression_value(AddExpression<double, RHS>* expr)
{ return expr->lhs + expr->rhs->value(); }


/*** SUB ***/

class SubExpression : public BinaryExpression<NumericValue*,NumericValue*>
{
public:

    SubExpression(ExprManager* _manager, NumericValue* _lhs, NumericValue* _rhs) 
        : BinaryExpression<NumericValue*,NumericValue*>(_manager,_lhs,_rhs)
        {}

    void print(std::ostream& ostr)
        {this->lhs->print(ostr); ostr << " - "; this->rhs->print(ostr); }

    double value() {return this->lhs->value() - this->rhs->value();}

    double compute_value()
        { return this->_value = this->lhs->_value - this->rhs->_value; }

    NumericValue* partial(unsigned int i)
        {
        if (i == 0)
            return static_cast<NumericValue*>(manager->one);
        else
            return static_cast<NumericValue*>(manager->negative_one);
        }

    void compute_adjoint()
        {
        this->lhs->adjoint += this->adjoint;
        this->rhs->adjoint -= this->adjoint;
        //std::cout << "SubExpression::compute_adjoint " << this->adjoint << " " << this->lhs->adjoint << " " << this->rhs->adjoint << std::endl;
        }

    void compute_hv_fwd()
        {
        compute_value();
        this->adjoint = 0;
        this->adO = 0;
        this->dL = this->dR = 1;
        this->dL2 = this->dLR = this->dR2 = 0;
        this->dO = this->lhs->dO*this->dL + this->rhs->dO*this->dR;
        }

    void snprintf(char* buf, int max)
        {std::snprintf(buf, max, "-");}

};


/*** MUL ***/

template <typename LHS, typename RHS>
class MulExpression : public BinaryExpression<LHS,RHS>
{
public:

    MulExpression(ExprManager* _manager, LHS _lhs, RHS _rhs) 
        : BinaryExpression<LHS,RHS>(_manager,_lhs,_rhs)
        {}

    void print(std::ostream& ostr) {MulExpression_print(ostr, this);}

    double value() {return MulExpression_value(this); }

    double compute_value()
        {
        this->_value = this->lhs->_value * this->rhs->_value;
        return this->_value;
        }

    NumericValue* partial(unsigned int i)
        {
        if (i == 0)
            return this->rhs;
        else if (i == 1)
            return this->lhs;
        return 0;
        }

    void compute_adjoint()
        {
        this->lhs->adjoint += this->adjoint * this->rhs->_value;
        this->rhs->adjoint += this->adjoint * this->lhs->_value;
        //std::cout << "MulExpression::compute_adjoint " << this->adjoint << " " << this->lhs->adjoint << " " << this->rhs->adjoint << std::endl;
        }

    void compute_hv_fwd()
        {
        compute_value();
        this->adjoint = 0;
        this->adO = 0;
        this->dL = this->rhs->_value;
        this->dR = this->lhs->_value;
        this->dL2 = this->dR2 = 0;
        this->dLR = 1;
        this->dO = this->lhs->dO*this->dL + this->rhs->dO*this->dR;
        }

    void snprintf(char* buf, int max)
        {std::snprintf(buf, max, "*");}
};


template <typename LHS, typename RHS>
void MulExpression_print(std::ostream& ostr, MulExpression<LHS, RHS>* expr)
{
if (expr->lhs->is_variable() || expr->lhs->is_parameter())
    expr->lhs->print(ostr);
else {
    ostr << "("; expr->lhs->print(ostr); ostr << ")";
    }
ostr << "*";
if (expr->rhs->is_variable() || expr->rhs->is_parameter())
    expr->rhs->print(ostr);
else {
    ostr << "("; expr->rhs->print(ostr); ostr << ")";
    }
}

template <typename LHS>
void MulExpression_print(std::ostream& ostr, MulExpression<LHS, int>* expr)
{ 
if (expr->lhs->is_variable() || expr->lhs->is_parameter())
    expr->lhs->print(ostr);
else {
    ostr << "("; expr->lhs->print(ostr); ostr << ")";
    }
ostr << "*";
ostr << expr->rhs;
}

template <typename LHS>
void MulExpression_print(std::ostream& ostr, MulExpression<LHS, double>* expr)
{
if (expr->lhs->is_variable() || expr->lhs->is_parameter())
    expr->lhs->print(ostr);
else {
    ostr << "("; expr->lhs->print(ostr); ostr << ")";
    }
ostr << "*";
ostr << expr->rhs;
}

template <typename RHS>
void MulExpression_print(std::ostream& ostr, MulExpression<int, RHS>* expr)
{
ostr << expr->lhs;
ostr << "*";
if (expr->rhs->is_variable() || expr->rhs->is_parameter())
    expr->rhs->print(ostr);
else {
    ostr << "("; expr->rhs->print(ostr); ostr << ")";
    }
}

template <typename RHS>
void MulExpression_print(std::ostream& ostr, MulExpression<double, RHS>* expr)
{
ostr << expr->lhs;
ostr << "*";
if (expr->rhs->is_variable() || expr->rhs->is_parameter())
    expr->rhs->print(ostr);
else {
    ostr << "("; expr->rhs->print(ostr); ostr << ")";
    }
}


template <typename LHS, typename RHS>
double MulExpression_value(MulExpression<LHS, RHS>* expr)
{ return expr->lhs->value() * expr->rhs->value(); }

template <typename LHS>
double MulExpression_value(MulExpression<LHS, int>* expr)
{ return expr->lhs->value() * expr->rhs; }

template <typename LHS>
double MulExpression_value(MulExpression<LHS, double>* expr)
{ return expr->lhs->value() * expr->rhs; }

template <typename RHS>
double MulExpression_value(MulExpression<int, RHS>* expr)
{ return expr->lhs * expr->rhs->value(); }

template <typename RHS>
double MulExpression_value(MulExpression<double, RHS>* expr)
{ return expr->lhs * expr->rhs->value(); }


/*** DIV ***/

class DivExpression : public BinaryExpression<NumericValue*,NumericValue*>
{
public:

    DivExpression(ExprManager* _manager, NumericValue* _lhs, NumericValue* _rhs) 
        : BinaryExpression<NumericValue*,NumericValue*>(_manager,_lhs,_rhs)
        {}

    void print(std::ostream& ostr)
        {
        if (this->lhs->is_variable() || this->lhs->is_parameter())
            this->lhs->print(ostr);
        else {
            ostr << "("; this->lhs->print(ostr); ostr << ")";
            }
        ostr << "/";
        if (this->rhs->is_variable() || this->rhs->is_parameter())
            this->rhs->print(ostr);
        else {
            ostr << "("; this->rhs->print(ostr); ostr << ")";
            }
        }

    double value() 
        {return this->lhs->value() / this->rhs->value(); }

    double compute_value()
        {return this->_value = this->lhs->_value / this->rhs->_value;}

    // TODO
    NumericValue* partial(unsigned int i)
        {
        if (i == 0)
            return this->rhs;
        else if (i == 1)
            return this->lhs;
        return 0;
        }

    // TODO
    void compute_adjoint()
        {
        this->lhs->adjoint += this->adjoint * this->rhs->_value;
        this->rhs->adjoint += this->adjoint * this->lhs->_value;
        //std::cout << "DivExpression::compute_adjoint " << this->adjoint << " " << this->lhs->adjoint << " " << this->rhs->adjoint << std::endl;
        }

    // TODO
    void compute_hv_fwd()
        {
        compute_value();
        this->adjoint = 0;
        this->adO = 0;
        this->dL = this->rhs->_value;
        this->dR = this->lhs->_value;
        this->dL2 = this->dR2 = 0;
        this->dLR = 1;
        this->dO = this->lhs->dO*this->dL + this->rhs->dO*this->dR;
        }

    void snprintf(char* buf, int max)
        {std::snprintf(buf, max, "/");}
};


void symbolic_diff_all(NumericValue* root, std::map<Variable*, NumericValue*>& diff);

void walk_expression_tree(NumericValue* root, void(*enter_callback)(void*,void*,void*), void(*exit_callback)(void*,void*,void*),void* visitor);

double compute_expression_value(NumericValue* root);

typedef std::set<Variable*, bool(*)(const Variable*, const Variable*)> ordered_variable_t;
typedef ordered_variable_t::iterator ordered_variable_iterator_t;
void build_expression(NumericValue* root, std::list<NumericValue*>& build, ordered_variable_t& variables);
std::list<std::string> expr_to_list(NumericValue* e, bool values);
std::list<std::string> expr_to_list(NumericValue* e);

