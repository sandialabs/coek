
#include <sstream>

#include "model.h"

/*** GLOBAL DATA ***/

std::list<Model*> models;
std::list<Expression*> expressions;
std::list<Parameter*> parameters;
std::list<Variable*> variables;


/*** ADD ***/

extern "C" void* add_expr_int(void* lhs, int rhs)
{
if (rhs == 0) {
    return lhs;
    }
NumericValue* _lhs = static_cast<NumericValue*>(lhs);
Parameter* _rhs = new TypedParameter<int>(rhs,false);
Expression* tmp = new AddExpression<NumericValue*,NumericValue*>(_lhs, _rhs);

parameters.push_back(_rhs);
expressions.push_back(tmp);
return tmp;
}

extern "C" void* add_expr_double(void* lhs, double rhs)
{
if (rhs == 0.0) {
    return lhs;
    }
NumericValue* _lhs = static_cast<NumericValue*>(lhs);
Parameter* _rhs = new TypedParameter<double>(rhs,false);
Expression* tmp = new AddExpression<NumericValue*,NumericValue*>(_lhs, _rhs);

parameters.push_back(_rhs);
expressions.push_back(tmp);
return tmp;
}

extern "C" void* add_expr_expression(void* lhs, void* rhs)
{
NumericValue* _lhs = static_cast<NumericValue*>(lhs);
NumericValue* _rhs = static_cast<NumericValue*>(rhs);
Expression* tmp = new AddExpression<NumericValue*,NumericValue*>(_lhs, _rhs);

expressions.push_back(tmp);
return tmp;
}

extern "C" void* radd_expr_int(int lhs, void* rhs)
{
if (lhs == 0) {
    return rhs;
    }
Parameter* _lhs = new TypedParameter<int>(lhs,false);
NumericValue* _rhs = static_cast<NumericValue*>(rhs);
Expression* tmp = new AddExpression<NumericValue*,NumericValue*>(_lhs, _rhs);

parameters.push_back(_lhs);
expressions.push_back(tmp);
return tmp;
}

extern "C" void* radd_expr_double(double lhs, void* rhs)
{
if (lhs == 0.0) {
    return rhs;
    }
Parameter* _lhs = new TypedParameter<double>(lhs,false);
NumericValue* _rhs = static_cast<NumericValue*>(rhs);
Expression* tmp = new AddExpression<NumericValue*,NumericValue*>(_lhs, _rhs);

parameters.push_back(_lhs);
expressions.push_back(tmp);
return tmp;
}


/*** MUL ***/

extern "C" void* mul_expr_int(void* lhs, int rhs)
{
if (rhs == 0) {
    return lhs;
    }
NumericValue* _lhs = static_cast<NumericValue*>(lhs);
Parameter* _rhs = new TypedParameter<int>(rhs,false);
Expression* tmp = new MulExpression<NumericValue*,NumericValue*>(_lhs, _rhs);

parameters.push_back(_rhs);
expressions.push_back(tmp);
return tmp;
}

extern "C" void* mul_expr_double(void* lhs, double rhs)
{
if (rhs == 0.0) {
    return lhs;
    }
NumericValue* _lhs = static_cast<NumericValue*>(lhs);
Parameter* _rhs = new TypedParameter<double>(rhs,false);
Expression* tmp = new MulExpression<NumericValue*,NumericValue*>(_lhs, _rhs);

parameters.push_back(_rhs);
expressions.push_back(tmp);
return tmp;
}

extern "C" void* mul_expr_expression(void* lhs, void* rhs)
{
NumericValue* _lhs = static_cast<NumericValue*>(lhs);
NumericValue* _rhs = static_cast<NumericValue*>(rhs);
Expression* tmp = new MulExpression<NumericValue*,NumericValue*>(_lhs, _rhs);

expressions.push_back(tmp);
return tmp;
}

extern "C" void* rmul_expr_int(int lhs, void* rhs)
{
if (lhs == 0) {
    return rhs;
    }
Parameter* _lhs = new TypedParameter<int>(lhs,false);
NumericValue* _rhs = static_cast<NumericValue*>(rhs);
Expression* tmp = new MulExpression<NumericValue*,NumericValue*>(_lhs, _rhs);

parameters.push_back(_lhs);
expressions.push_back(tmp);
return tmp;
}

extern "C" void* rmul_expr_double(double lhs, void* rhs)
{
if (lhs == 0.0) {
    return rhs;
    }
Parameter* _lhs = new TypedParameter<double>(lhs,false);
NumericValue* _rhs = static_cast<NumericValue*>(rhs);
Expression* tmp = new MulExpression<NumericValue*,NumericValue*>(_lhs, _rhs);

parameters.push_back(_lhs);
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

extern "C" double get_variable_value(void* ptr)
{
Variable* v = static_cast<Variable*>(ptr);
return v->_value;
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
Model* tmp = new Model1();
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

extern "C" void print_model(void* model, int df)
{
Model* tmp = static_cast<Model*>(model);
tmp->print(std::cout, df);
}

extern "C" void build_model(void* model)
{
Model* tmp = static_cast<Model*>(model);
tmp->build();
}

extern "C" int get_nvariables(void* model)
{
Model* tmp = static_cast<Model*>(model);
return tmp->num_variables();
}

extern "C" double compute_objective_f(void* model, int i)
{
Model* tmp = static_cast<Model*>(model);
return tmp->_compute_f(i);
}

extern "C" void compute_objective_df(void* model, double* df, int n, int i)
{
std::vector<double> _df(n);
Model* tmp = static_cast<Model*>(model);
tmp->_compute_df(_df, i);
for (int i=0; i<n; i++)
  df[i] = _df[i];
}


