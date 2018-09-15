
#include <sstream>

#include "model.h"

/*** GLOBAL DATA ***/

std::list<Model*> models;
std::list<Expression*> expressions;
std::list<Parameter*> parameters;
std::list<Variable*> variables;


/*** ADD ***/

template <typename LHS, typename RHS>
inline AddExpression<LHS, RHS>* AddExpression_create(LHS lhs, RHS rhs)
{ return new AddExpression<LHS, RHS>(lhs, rhs); }

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

extern "C" void* add_expr_double(void* lhs, double rhs)
{
if (rhs == 0.0) {
    return lhs;
    }
Expression* tmp;
NumericValue* e = static_cast<NumericValue*>(lhs);
double _rhs = static_cast<double>(rhs);
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

extern "C" void* add_expr_expression(void* lhs, void* rhs)
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

extern "C" void* radd_expr_double(double lhs, void* rhs)
{
if (lhs == 0.0) {
    return rhs;
    }
Expression* tmp;
NumericValue* e = static_cast<NumericValue*>(rhs);
if (e->is_variable()) {
    Variable* _rhs = static_cast<Variable*>(rhs);
    tmp = new AddExpression<double,Variable*>(lhs, _rhs);
    }
else {
    Expression* _rhs = static_cast<Expression*>(rhs);
    tmp = new AddExpression<double,Expression*>(lhs, _rhs);
    }
expressions.push_back(tmp);
return tmp;
}


/*** MUL ***/

template <typename LHS, typename RHS>
inline MulExpression<LHS, RHS>* MulExpression_create(LHS lhs, RHS rhs)
{ return new MulExpression<LHS, RHS>(lhs, rhs); }

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

extern "C" void* mul_expr_double(void* lhs, double rhs)
{
if (rhs == 1.0) {
    return lhs;
    }
Expression* tmp;
NumericValue* e = static_cast<NumericValue*>(lhs);
double _rhs = static_cast<double>(rhs);
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

extern "C" void* mul_expr_expression(void* lhs, void* rhs)
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

extern "C" void* rmul_expr_double(double lhs, void* rhs)
{
if (lhs == 1.0) {
    return rhs;
    }
Expression* tmp;
NumericValue* e = static_cast<NumericValue*>(rhs);
if (e->is_variable()) {
    Variable* _rhs = static_cast<Variable*>(rhs);
    tmp = new MulExpression<double,Variable*>(lhs, _rhs);
    }
else {
    Expression* _rhs = static_cast<Expression*>(rhs);
    tmp = new MulExpression<double,Expression*>(lhs, _rhs);
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

extern "C" void* create_parameter_int(int value, int mutable_flag)
{
Parameter* tmp = new TypedParameter<int>(value, mutable_flag);
parameters.push_back(tmp);
return tmp;
}

extern "C" void* create_parameter_double(double value, int mutable_flag)
{
Parameter* tmp = new TypedParameter<double>(value, mutable_flag);
parameters.push_back(tmp);
return tmp;
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

extern "C" void set_variable_value(void* ptr, double val)
{
Variable* v = static_cast<Variable*>(ptr);
v->_value = val;
}

extern "C" void* create_inequality(void* expr)
{
NumericValue* _expr = static_cast<NumericValue*>(expr);
InequalityExpression* tmp = new InequalityExpression(_expr);
expressions.push_back(tmp);
return tmp;
}

extern "C" void* create_equality(void* expr)
{
NumericValue* _expr = static_cast<NumericValue*>(expr);
EqualityExpression* tmp = new EqualityExpression(_expr);
expressions.push_back(tmp);
return tmp;
}

extern "C" void* create_model()
{
Model* tmp = new Model();
models.push_back(tmp);
return tmp;
}

extern "C" void add_objective(void* model, void* expr)
{
Model* tmp = static_cast<Model*>(model);
Expression* _expr = static_cast<Expression*>(expr);
tmp->objectives.push_back(_expr);
}

extern "C" void add_inequality(void* model, void* ineq)
{
Model* tmp = static_cast<Model*>(model);
Expression* _ineq = static_cast<Expression*>(ineq);
tmp->inequalities.push_back(_ineq);
}

extern "C" void add_equality(void* model, void* eq)
{
Model* tmp = static_cast<Model*>(model);
Expression* _eq = static_cast<Expression*>(eq);
tmp->equalities.push_back(_eq);
}

extern "C" void print_model(void* expr)
{
Model* tmp = static_cast<Model*>(expr);
tmp->print(std::cout);
}

extern "C" void build_model(void* expr)
{
Model* tmp = static_cast<Model*>(expr);
tmp->build();
}

extern "C" double compute_objective(void* model, int i)
{
Model* tmp = static_cast<Model*>(model);
return tmp->_compute_f(i);
}


