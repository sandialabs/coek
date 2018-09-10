
#include <string>
#include <list>
#include <sstream>
#include <iostream>

extern "C" {

#include "expr_api.h"

}


/*
class Parameter { };

template <typename TYPE>
class DerivedParameter : public Parameter
{
public:

    TYPE value;

    DerivedParameter(TYPE _value)
        { value = _value; }

};
*/


int nvariables = 0;


class NumericValue
{
public:

    virtual void print(std::ostream& ostr) = 0;

    virtual bool is_variable() { return false; }
    virtual bool is_expression() { return false; }

    virtual int size() {return 1;}
};


class Variable : public NumericValue
{
public:

    bool binary;
    bool integer;
    int index;

    Variable(bool _binary, bool _integer) : NumericValue()
        {
        binary = _binary; 
        integer = _integer; 
        index = nvariables++;
        }

    void print(std::ostream& ostr)
        { ostr << 'x' << index; }

    bool is_variable() { return true; }

};


class Expression: public NumericValue
{
public:

    bool is_expression() { return true; }

    void print(std::ostream& ostr) {ostr << "ERROR: Undefined print() method.";}
};


std::list<Expression*> expressions;
/*std::list<Parameter*> parameters;*/
std::list<Variable*> variables;


/*** ADD ***/

template <typename LHS, typename RHS>
class AddExpression : public Expression
{
public:

    LHS lhs;
    RHS rhs;

    AddExpression(LHS _lhs, RHS _rhs) : Expression()
        {lhs = _lhs; rhs = _rhs;}

    void print(std::ostream& ostr) {AddExpression_print(ostr, this);}

    int size() {return AddExpression_size(this);}
};

template <typename LHS, typename RHS>
void AddExpression_print(std::ostream& ostr, AddExpression<LHS, RHS>* expr)
{ expr->lhs->print(ostr); ostr << " + "; expr->rhs->print(ostr); }

template <typename LHS>
void AddExpression_print(std::ostream& ostr, AddExpression<LHS, int>* expr)
{ expr->lhs->print(ostr); ostr << " + " << expr->rhs; }

template <typename LHS>
void AddExpression_print(std::ostream& ostr, AddExpression<LHS, float>* expr)
{ expr->lhs->print(ostr); ostr << " + " << expr->rhs; }

template <typename RHS>
void AddExpression_print(std::ostream& ostr, AddExpression<int, RHS>* expr)
{ ostr << expr->lhs << " + "; expr->rhs->print(ostr); }

template <typename RHS>
void AddExpression_print(std::ostream& ostr, AddExpression<float, RHS>* expr)
{ ostr << expr->lhs << " + "; expr->rhs->print(ostr); }


template <typename LHS, typename RHS>
int AddExpression_size(AddExpression<LHS, RHS>* expr)
{ return expr->lhs->size() + expr->rhs->size() + 1; }

template <typename LHS>
int AddExpression_size(AddExpression<LHS, int>* expr)
{ return expr->lhs->size() + 2; }

template <typename LHS>
int AddExpression_size(AddExpression<LHS, float>* expr)
{ return expr->lhs->size() + 2; }

template <typename RHS>
int AddExpression_size(AddExpression<int, RHS>* expr)
{ return expr->rhs->size() + 2; }

template <typename RHS>
int AddExpression_size(AddExpression<float, RHS>* expr)
{ return expr->rhs->size() + 2; }



template <typename LHS, typename RHS>
inline AddExpression<LHS, RHS>* AddExpression_create(LHS lhs, RHS rhs)
{ return new AddExpression<LHS, RHS>(lhs, rhs); }

/* __add__ */
extern "C" void* add_expr_int(void* lhs, int rhs)
{
if (rhs == 0) {
    return lhs;
    }
Expression* tmp;
NumericValue* e = static_cast<NumericValue*>(lhs);
int _rhs = static_cast<int>(rhs);
if (e->is_variable()) {
    Variable* _lhs = static_cast<Variable*>(lhs);
    tmp = AddExpression_create(_lhs, _rhs);
    }
else {
    Expression* _lhs = static_cast<Expression*>(lhs);
    tmp = AddExpression_create(_lhs, _rhs);
    }
expressions.push_back(tmp);
return tmp;
}

extern "C" void* add_expr_float(void* lhs, float rhs)
{
if (rhs == 0.0) {
    return lhs;
    }
Expression* tmp;
NumericValue* e = static_cast<NumericValue*>(lhs);
float _rhs = static_cast<float>(rhs);
if (e->is_variable()) {
    Variable* _lhs = static_cast<Variable*>(lhs);
    tmp = AddExpression_create(_lhs, _rhs);
    }
else {
    Expression* _lhs = static_cast<Expression*>(lhs);
    tmp = AddExpression_create(_lhs, _rhs);
    }
expressions.push_back(tmp);
return tmp;
}

void* add_expr_expression(void* lhs, void* rhs)
{
Expression* tmp;
NumericValue* _lhs = static_cast<NumericValue*>(lhs);
NumericValue* _rhs = static_cast<NumericValue*>(rhs);
if (_lhs->is_variable()) {
    Variable* __lhs = static_cast<Variable*>(lhs);
    if (_rhs->is_variable()) {
        Variable* __rhs = static_cast<Variable*>(rhs);
        tmp = AddExpression_create(__lhs, __rhs);
        }
    else {
        Expression* __rhs = static_cast<Expression*>(rhs);
        tmp = AddExpression_create(__lhs, __rhs);
        }
    }
else {
    Expression* __lhs = static_cast<Expression*>(lhs);
    if (_rhs->is_variable()) {
        Variable* __rhs = static_cast<Variable*>(rhs);
        tmp = AddExpression_create(__lhs, __rhs);
        }
    else {
        Expression* __rhs = static_cast<Expression*>(rhs);
        tmp = AddExpression_create(__lhs, __rhs);
        }
    }
expressions.push_back(tmp);
return tmp;
}


/* __radd__ */
extern "C" void* radd_expr_int(int lhs, void* rhs)
{
if (lhs == 0) {
    return rhs;
    }
Expression* tmp;
NumericValue* e = static_cast<NumericValue*>(rhs);
if (e->is_variable()) {
    Variable* _rhs = static_cast<Variable*>(rhs);
    tmp = new AddExpression<int,Variable*>(lhs, _rhs);
    }
else {
    Expression* _rhs = static_cast<Expression*>(rhs);
    tmp = new AddExpression<int,Expression*>(lhs, _rhs);
    }
expressions.push_back(tmp);
return tmp;
}

extern "C" void* radd_expr_float(float lhs, void* rhs)
{
if (lhs == 0.0) {
    return rhs;
    }
Expression* tmp;
NumericValue* e = static_cast<NumericValue*>(rhs);
if (e->is_variable()) {
    Variable* _rhs = static_cast<Variable*>(rhs);
    tmp = new AddExpression<float,Variable*>(lhs, _rhs);
    }
else {
    Expression* _rhs = static_cast<Expression*>(rhs);
    tmp = new AddExpression<float,Expression*>(lhs, _rhs);
    }
expressions.push_back(tmp);
return tmp;
}


/*** MUL ***/

template <typename LHS, typename RHS>
class MulExpression : public Expression
{
public:

    LHS lhs;
    RHS rhs;

    MulExpression(LHS _lhs, RHS _rhs) : Expression()
        {lhs = _lhs; rhs = _rhs;}

    void print(std::ostream& ostr) {MulExpression_print(ostr, this);}

    int size() {return MulExpression_size(this);}
};

template <typename LHS, typename RHS>
void MulExpression_print(std::ostream& ostr, MulExpression<LHS, RHS>* expr)
{
if (expr->lhs->is_variable())
    expr->lhs->print(ostr);
else {
    ostr << "("; expr->lhs->print(ostr); ostr << ")";
    }
ostr << "*";
if (expr->rhs->is_variable())
    expr->rhs->print(ostr);
else {
    ostr << "("; expr->rhs->print(ostr); ostr << ")";
    }
}

template <typename LHS>
void MulExpression_print(std::ostream& ostr, MulExpression<LHS, int>* expr)
{ 
if (expr->lhs->is_variable())
    expr->lhs->print(ostr);
else {
    ostr << "("; expr->lhs->print(ostr); ostr << ")";
    }
ostr << "*";
ostr << expr->rhs;
}

template <typename LHS>
void MulExpression_print(std::ostream& ostr, MulExpression<LHS, float>* expr)
{
if (expr->lhs->is_variable())
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
if (expr->rhs->is_variable())
    expr->rhs->print(ostr);
else {
    ostr << "("; expr->rhs->print(ostr); ostr << ")";
    }
}

template <typename RHS>
void MulExpression_print(std::ostream& ostr, MulExpression<float, RHS>* expr)
{
ostr << expr->lhs;
ostr << "*";
if (expr->rhs->is_variable())
    expr->rhs->print(ostr);
else {
    ostr << "("; expr->rhs->print(ostr); ostr << ")";
    }
}

template <typename LHS, typename RHS>
int MulExpression_size(MulExpression<LHS, RHS>* expr)
{ return expr->lhs->size() + expr->rhs->size() + 1; }

template <typename LHS>
int MulExpression_size(MulExpression<LHS, int>* expr)
{ return expr->lhs->size() + 2; }

template <typename LHS>
int MulExpression_size(MulExpression<LHS, float>* expr)
{ return expr->lhs->size() + 2; }

template <typename RHS>
int MulExpression_size(MulExpression<int, RHS>* expr)
{ return expr->rhs->size() + 2; }

template <typename RHS>
int MulExpression_size(MulExpression<float, RHS>* expr)
{ return expr->rhs->size() + 2; }



template <typename LHS, typename RHS>
inline MulExpression<LHS, RHS>* MulExpression_create(LHS lhs, RHS rhs)
{ return new MulExpression<LHS, RHS>(lhs, rhs); }


/* __mul__ */
extern "C" void* mul_expr_int(void* lhs, int rhs)
{
if (rhs == 1) {
    return lhs;
    }
Expression* tmp;
NumericValue* e = static_cast<NumericValue*>(lhs);
int _rhs = static_cast<int>(rhs);
if (e->is_variable()) {
    Variable* _lhs = static_cast<Variable*>(lhs);
    tmp = MulExpression_create(_lhs, _rhs);
    }
else {
    Expression* _lhs = static_cast<Expression*>(lhs);
    tmp = MulExpression_create(_lhs, _rhs);
    }
expressions.push_back(tmp);
return tmp;
}

extern "C" void* mul_expr_float(void* lhs, float rhs)
{
if (rhs == 1.0) {
    return lhs;
    }
Expression* tmp;
NumericValue* e = static_cast<NumericValue*>(lhs);
float _rhs = static_cast<float>(rhs);
if (e->is_variable()) {
    Variable* _lhs = static_cast<Variable*>(lhs);
    tmp = MulExpression_create(_lhs, _rhs);
    }
else {
    Expression* _lhs = static_cast<Expression*>(lhs);
    tmp = MulExpression_create(_lhs, _rhs);
    }
expressions.push_back(tmp);
return tmp;
}

void* mul_expr_expression(void* lhs, void* rhs)
{
Expression* tmp;
NumericValue* _lhs = static_cast<NumericValue*>(lhs);
NumericValue* _rhs = static_cast<NumericValue*>(rhs);
if (_lhs->is_variable()) {
    Variable* __lhs = static_cast<Variable*>(lhs);
    if (_rhs->is_variable()) {
        Variable* __rhs = static_cast<Variable*>(rhs);
        tmp = MulExpression_create(__lhs, __rhs);
        }
    else {
        Expression* __rhs = static_cast<Expression*>(rhs);
        tmp = MulExpression_create(__lhs, __rhs);
        }
    }
else {
    Expression* __lhs = static_cast<Expression*>(lhs);
    if (_rhs->is_variable()) {
        Variable* __rhs = static_cast<Variable*>(rhs);
        tmp = MulExpression_create(__lhs, __rhs);
        }
    else {
        Expression* __rhs = static_cast<Expression*>(rhs);
        tmp = MulExpression_create(__lhs, __rhs);
        }
    }
expressions.push_back(tmp);
return tmp;
}


/* __rmul__ */
extern "C" void* rmul_expr_int(int lhs, void* rhs)
{
if (lhs == 1) {
    return rhs;
    }
Expression* tmp;
NumericValue* e = static_cast<NumericValue*>(rhs);
if (e->is_variable()) {
    Variable* _rhs = static_cast<Variable*>(rhs);
    tmp = new MulExpression<int,Variable*>(lhs, _rhs);
    }
else {
    Expression* _rhs = static_cast<Expression*>(rhs);
    tmp = new MulExpression<int,Expression*>(lhs, _rhs);
    }
expressions.push_back(tmp);
return tmp;
}

extern "C" void* rmul_expr_float(float lhs, void* rhs)
{
if (lhs == 1.0) {
    return rhs;
    }
Expression* tmp;
NumericValue* e = static_cast<NumericValue*>(rhs);
if (e->is_variable()) {
    Variable* _rhs = static_cast<Variable*>(rhs);
    tmp = new MulExpression<float,Variable*>(lhs, _rhs);
    }
else {
    Expression* _rhs = static_cast<Expression*>(rhs);
    tmp = new MulExpression<float,Expression*>(lhs, _rhs);
    }
expressions.push_back(tmp);
return tmp;
}


/*** MISC ***/


extern "C" void print_var(void* var)
{
Variable* _var = static_cast<Variable*>(var);
_var->print(std::cout);
std::cout << std::endl;
}

extern "C" void print_expr(void* expr)
{
Expression* _expr = static_cast<Expression*>(expr);
_expr->print(std::cout);
std::cout << std::endl;
}

extern "C" int expr_size(void* expr)
{
NumericValue* _expr = static_cast<NumericValue*>(expr);
return _expr->size();
}

extern "C" void* create_variable(int binary, int integer)
{
Variable* tmp = new Variable(binary, integer);
variables.push_back(tmp);
return tmp;
}

extern "C" int get_variable_index(void* ptr)
{
Variable* v = static_cast<Variable*>(ptr);
return v->index;
}

