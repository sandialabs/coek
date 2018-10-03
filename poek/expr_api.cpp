
#include <cassert>
#include <sstream>

#include "model.h"

/*** GLOBAL DATA ***/

std::list<Model*> models;
std::list<Expression*> expressions;
std::list<Parameter*> parameters;
std::list<Variable*> variables;


extern "C" void* misc_getnull(void)
{
return 0;
}


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


/*** SUB ***/

extern "C" void* sub_expr_int(void* lhs, int rhs)
{
if (rhs == 0) {
    return lhs;
    }
NumericValue* _lhs = static_cast<NumericValue*>(lhs);
Parameter* _rhs = new TypedParameter<int>(rhs,false);
Expression* tmp = new SubExpression(_lhs, _rhs);

parameters.push_back(_rhs);
expressions.push_back(tmp);
return tmp;
}

extern "C" void* sub_expr_double(void* lhs, double rhs)
{
if (rhs == 0.0) {
    return lhs;
    }
NumericValue* _lhs = static_cast<NumericValue*>(lhs);
Parameter* _rhs = new TypedParameter<double>(rhs,false);
Expression* tmp = new SubExpression(_lhs, _rhs);

parameters.push_back(_rhs);
expressions.push_back(tmp);
return tmp;
}

extern "C" void* sub_expr_expression(void* lhs, void* rhs)
{
NumericValue* _lhs = static_cast<NumericValue*>(lhs);
NumericValue* _rhs = static_cast<NumericValue*>(rhs);
Expression* tmp = new SubExpression(_lhs, _rhs);

expressions.push_back(tmp);
return tmp;
}

extern "C" void* rsub_expr_int(int lhs, void* rhs)
{
NumericValue* _rhs = static_cast<NumericValue*>(rhs);
if (lhs == 0) {
    Expression* tmp = new NegExpression(_rhs);
    expressions.push_back(tmp);
    return tmp;
    }
Parameter* _lhs = new TypedParameter<int>(lhs,false);
Expression* tmp = new SubExpression(_lhs, _rhs);

parameters.push_back(_lhs);
expressions.push_back(tmp);
return tmp;
}

extern "C" void* rsub_expr_double(double lhs, void* rhs)
{
NumericValue* _rhs = static_cast<NumericValue*>(rhs);
if (lhs == 0.0) {
    Expression* tmp = new NegExpression(_rhs);
    expressions.push_back(tmp);
    return tmp;
    }
Parameter* _lhs = new TypedParameter<double>(lhs,false);
Expression* tmp = new SubExpression(_lhs, _rhs);

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
if (_rhs->is_parameter()) {
    Parameter* p = static_cast<Parameter*>(rhs);
    if (!(p->mutable_flag)) {
        if (p->_value==0)
            return 0;
        if (p->_value==1)
            return lhs;
        }
    }
if (_lhs->is_parameter()) {
    Parameter* p = static_cast<Parameter*>(lhs);
    if (!(p->mutable_flag)) {
        if (p->_value==0)
            return 0;
        if (p->_value==1)
            return rhs;
        }
    }
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


/*** DIV ***/

extern "C" void* div_expr_int(void* lhs, int rhs)
{
NumericValue* _lhs = static_cast<NumericValue*>(lhs);
Parameter* _rhs = new TypedParameter<int>(rhs,false);
Expression* tmp = new DivExpression(_lhs, _rhs);

parameters.push_back(_rhs);
expressions.push_back(tmp);
return tmp;
}

extern "C" void* div_expr_double(void* lhs, double rhs)
{
NumericValue* _lhs = static_cast<NumericValue*>(lhs);
Parameter* _rhs = new TypedParameter<double>(rhs,false);
Expression* tmp = new DivExpression(_lhs, _rhs);

parameters.push_back(_rhs);
expressions.push_back(tmp);
return tmp;
}

extern "C" void* div_expr_expression(void* lhs, void* rhs)
{
NumericValue* _lhs = static_cast<NumericValue*>(lhs);
NumericValue* _rhs = static_cast<NumericValue*>(rhs);
if (_lhs->is_parameter()) {
    Parameter* p = static_cast<Parameter*>(lhs);
    if (!(p->mutable_flag)) {
        if (p->_value==0)
            return 0;
        }
    }
if (_rhs->is_parameter()) {
    Parameter* p = static_cast<Parameter*>(rhs);
    if (!(p->mutable_flag)) {
        if (p->_value==0)
            return rhs;         // Indicate a divide by zero error
        if (p->_value==1)
            return lhs;
        }
    }

Expression* tmp = new DivExpression(_lhs, _rhs);

expressions.push_back(tmp);
return tmp;
}

extern "C" void* rdiv_expr_int(int lhs, void* rhs)
{
Parameter* _lhs = new TypedParameter<int>(lhs,false);
NumericValue* _rhs = static_cast<NumericValue*>(rhs);
if (_rhs->is_parameter()) {
    Parameter* p = static_cast<Parameter*>(_rhs);
    if (!(p->mutable_flag)) {
        if (p->_value==0)
            return rhs;         // Indicate a divide by zero error
        if (p->_value==1)
            return _lhs;
        }
    }
Expression* tmp = new DivExpression(_lhs, _rhs);

parameters.push_back(_lhs);
expressions.push_back(tmp);
return tmp;
}

extern "C" void* rdiv_expr_double(double lhs, void* rhs)
{
Parameter* _lhs = new TypedParameter<double>(lhs,false);
NumericValue* _rhs = static_cast<NumericValue*>(rhs);
if (_rhs->is_parameter()) {
    Parameter* p = static_cast<Parameter*>(_rhs);
    if (!(p->mutable_flag)) {
        if (p->_value==0)
            return rhs;         // Indicate a divide by zero error
        if (p->_value==1)
            return _lhs;
        }
    }
Expression* tmp = new DivExpression(_lhs, _rhs);

parameters.push_back(_lhs);
expressions.push_back(tmp);
return tmp;
}


/*** POW ***/

extern "C" void* pow_expr_int(void* lhs, int rhs)
{
NumericValue* _lhs = static_cast<NumericValue*>(lhs);
Parameter* _rhs = new TypedParameter<int>(rhs,false);
if (_rhs->is_parameter()) {
    Parameter* p = static_cast<Parameter*>(_rhs);
    if (!(p->mutable_flag)) {
        if (p->_value==0)
            return &OneParameter;
        if (p->_value==1)
            return lhs;
        }
    }
Expression* tmp = new PowExpression(_lhs, _rhs);

parameters.push_back(_rhs);
expressions.push_back(tmp);
return tmp;
}

extern "C" void* pow_expr_double(void* lhs, double rhs)
{
NumericValue* _lhs = static_cast<NumericValue*>(lhs);
Parameter* _rhs = new TypedParameter<double>(rhs,false);
if (_rhs->is_parameter()) {
    Parameter* p = static_cast<Parameter*>(_rhs);
    if (!(p->mutable_flag)) {
        if (p->_value==0)
            return &OneParameter;
        if (p->_value==1)
            return lhs;
        }
    }
Expression* tmp = new PowExpression(_lhs, _rhs);

parameters.push_back(_rhs);
expressions.push_back(tmp);
return tmp;
}

extern "C" void* pow_expr_expression(void* lhs, void* rhs)
{
NumericValue* _lhs = static_cast<NumericValue*>(lhs);
NumericValue* _rhs = static_cast<NumericValue*>(rhs);
if (_lhs->is_parameter()) {
    Parameter* p = static_cast<Parameter*>(_lhs);
    if (!(p->mutable_flag)) {
        if (p->_value==0)
            return 0;
        if (p->_value==1)
            return lhs;
        }
    }
if (_rhs->is_parameter()) {
    Parameter* p = static_cast<Parameter*>(_rhs);
    if (!(p->mutable_flag)) {
        if (p->_value==0)
            return &OneParameter;
        if (p->_value==1)
            return lhs;
        }
    }
Expression* tmp = new PowExpression(_lhs, _rhs);

expressions.push_back(tmp);
return tmp;
}

extern "C" void* rpow_expr_int(int lhs, void* rhs)
{
Parameter* _lhs = new TypedParameter<int>(lhs,false);
NumericValue* _rhs = static_cast<NumericValue*>(rhs);
Expression* tmp = new PowExpression(_lhs, _rhs);

parameters.push_back(_lhs);
expressions.push_back(tmp);
return tmp;
}

extern "C" void* rpow_expr_double(double lhs, void* rhs)
{
Parameter* _lhs = new TypedParameter<double>(lhs,false);
NumericValue* _rhs = static_cast<NumericValue*>(rhs);
Expression* tmp = new PowExpression(_lhs, _rhs);

parameters.push_back(_lhs);
expressions.push_back(tmp);
return tmp;
}


/*** OTHER ***/
extern "C" void* neg_expr(void* _expr)
{
NumericValue* expr = static_cast<NumericValue*>(_expr);
Expression* tmp = new NegExpression(expr);

expressions.push_back(tmp);
return tmp;
}


/*** MISC ***/


extern "C" void print_parameter(void* param)
{
Parameter* _param = static_cast<Parameter*>(param);
_param->print(std::cout);
std::cout << std::endl;
}

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

extern "C" void* expr_diff(void* expr, void* variable)
{
NumericValue* root = static_cast<NumericValue*>(expr);
Variable* var = static_cast<Variable*>(variable);

std::map<Variable*, NumericValue*> diff;
symbolic_diff_all(root, diff);

if (diff.find(var) == diff.end())
    return 0;
return diff[var];
}

extern "C" void* create_parameter_int(int value, int mutable_flag, char* name)
{
Parameter* tmp = new TypedParameter<int>(value, mutable_flag, name);
parameters.push_back(tmp);
return tmp;
}

extern "C" void* create_parameter_double(double value, int mutable_flag, char* name)
{
Parameter* tmp = new TypedParameter<double>(value, mutable_flag, name);
parameters.push_back(tmp);
return tmp;
}

extern "C" void* create_variable(int binary, int integer, char* name)
{
Variable* tmp = new Variable(binary, integer, name);
variables.push_back(tmp);
return tmp;
}

extern "C" void create_variable_array(void* vars[], int num, int binary, int integer, char* name)
{
for (int i=0; i<num; i++) {
    Variable* tmp = new Variable(binary, integer, name);
    vars[i] = tmp;
    variables.push_back(tmp);
    }
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

extern "C" void get_numval_str(void* ptr, char* buf, int max)
{
NumericValue* v = static_cast<NumericValue*>(ptr);
v->snprintf(buf, max);
}

extern "C" double get_numval_value(void* ptr)
{
NumericValue* v = static_cast<NumericValue*>(ptr);
return v->_value;
}

extern "C" double compute_numval_value(void* ptr)
{
NumericValue* v = static_cast<NumericValue*>(ptr);
return v->value();
}

extern "C" double compute_constraint_value(void* ptr)
{
Constraint* v = static_cast<Constraint*>(ptr);
return v->boolean_value();
}

extern "C" void* create_inequality(void* expr, int strict_flag)
{
NumericValue* _expr = static_cast<NumericValue*>(expr);
InequalityExpression* tmp = new InequalityExpression(_expr, strict_flag);
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
Model* tmp = static_cast<Model*>(model);
double f;
std::vector<double> _df(n);
tmp->_compute_df(f, _df, i);
for (int i=0; i<n; i++)
  df[i] = _df[i];
}

extern "C" void compute_constraint_f(void* model, double* c, int n)
{
Model* tmp = static_cast<Model*>(model);
std::vector<double> _c(n);
tmp->_compute_c(_c);
for (int i=0; i<n; i++)
  c[i] = _c[i];
}

extern "C" void compute_constraint_df(void* model, double* dc, int n, int i)
{
Model* tmp = static_cast<Model*>(model);
std::vector<double> _dc(n);
tmp->_compute_dc(_dc, i);
for (int i=0; i<n; i++)
  dc[i] = _dc[i];
}

extern "C" void compute_Hv(void* model, double* v, double* Hv, int n, int i)
{
Model* tmp = static_cast<Model*>(model);
std::vector<double> _v(n);
for (int j=0; j<n; j++)
  _v[j] = v[j];
std::vector<double> _Hv(n);
tmp->_compute_Hv(_v, _Hv, i);
for (int j=0; j<n; j++)
  Hv[j] = _Hv[j];
}

// Used to test callback logic
extern "C" void test_callback(int(*callback)(int,int))
{
int value=0;

value = (*callback)(2,3);

assert(value == 5);
std::cout << (*callback)(3,5) << std::endl;
}

// Run a DFS walker
extern "C" void visitor_walk(void* root, void(*enter_callback)(void*,void*,void*), void(*exit_callback)(void*,void*,void*), void* visitor)
{
NumericValue* _root = static_cast<NumericValue*>(root);
walk_expression_tree(_root, enter_callback, exit_callback, visitor);
}

