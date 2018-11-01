
#include <cmath>
#include <cassert>
#include <sstream>

//#include "expr_types.hpp"
#include "expr/expr_manager.hpp"
#include "admodel/simple.hpp"
#include "solver.hpp"


/*** GLOBAL DATA ***/

Simple_ExprModel* model = 0;
ExprManager* manager = 0;
std::list<Simple_ExprModel*> models;


extern "C" void* misc_getnull(void)
{ return 0; }

extern "C" double misc_getnan(void)
{ return NAN; }


/*** ADD ***/

extern "C" void* expr_plus_int(void* lhs, int rhs)
{
if (rhs == 0) {
    return lhs;
    }
return manager->plus( lhs, manager->param(rhs, false, "") );
}

extern "C" void* expr_plus_double(void* lhs, double rhs)
{
if (rhs == 0.0) {
    return lhs;
    }
return manager->plus( lhs, manager->param(rhs, false, "") );
}

extern "C" void* expr_plus_expression(void* lhs, void* rhs)
{
return manager->plus( lhs, rhs );
}

extern "C" void* expr_rplus_int(int lhs, void* rhs)
{
if (lhs == 0) {
    return rhs;
    }
return manager->plus( manager->param(lhs, false, ""), rhs );
}

extern "C" void* expr_rplus_double(double lhs, void* rhs)
{
if (lhs == 0.0) {
    return rhs;
    }
return manager->plus( manager->param(lhs, false, ""), rhs );
}


/*** SUB ***/

extern "C" void* expr_minus_int(void* lhs, int rhs)
{
if (rhs == 0) {
    return lhs;
    }
return manager->minus( lhs, manager->param(rhs, false, "") );
}

extern "C" void* expr_minus_double(void* lhs, double rhs)
{
if (rhs == 0.0) {
    return lhs;
    }
return manager->minus( lhs, manager->param(rhs, false, "") );
}

extern "C" void* expr_minus_expression(void* lhs, void* rhs)
{
return manager->minus( lhs, rhs );
}

extern "C" void* expr_rminus_int(int lhs, void* rhs)
{
if (lhs == 0) {
    return manager->negate( rhs );
    }
return manager->minus( manager->param(lhs, false, ""), rhs );
}

extern "C" void* expr_rminus_double(double lhs, void* rhs)
{
if (lhs == 0.0) {
    return manager->negate( rhs );
    }
return manager->minus( manager->param(lhs, false, ""), rhs );
}


/*** MUL ***/

extern "C" void* expr_times_int(void* lhs, int rhs)
{
if (rhs == 0) {
    return manager->zero;
    }
if (rhs == 1) {
    return lhs;
    }
if (rhs == -1) {
    return manager->negate( lhs );
    }
return manager->times( lhs, manager->param(rhs, false, "") );
}

extern "C" void* expr_times_double(void* lhs, double rhs)
{
if (rhs == 0.0) {
    return manager->zero;
    }
if (rhs == 1.0) {
    return lhs;
    }
if (rhs == -1.0) {
    return manager->negate( lhs );
    }
return manager->times( lhs, manager->param(rhs, false, "") );
}

extern "C" void* expr_times_expression(void* lhs, void* rhs)
{
return manager->times( lhs, rhs );
}

extern "C" void* expr_rtimes_int(int lhs, void* rhs)
{
if (lhs == 0) {
    return manager->zero;
    }
if (lhs == 1) {
    return rhs;
    }
if (lhs == -1) {
    return manager->negate( rhs );
    }
return manager->times( manager->param(lhs, false, ""), rhs );
}

extern "C" void* expr_rtimes_double(double lhs, void* rhs)
{
if (lhs == 0.0) {
    return manager->zero;
    }
if (lhs == 1.0) {
    return rhs;
    }
if (lhs == -1.0) {
    return manager->negate( rhs );
    }
return manager->times( manager->param(lhs, false, ""), rhs );
}


/*** DIV ***/

extern "C" void* expr_divide_int(void* lhs, int rhs)
{
if (rhs == 1) {
    return lhs;
    }
if (rhs == -1) {
    return manager->negate( lhs );
    }
return manager->divide( lhs, manager->param(rhs, false, "") );
}

extern "C" void* expr_divide_double(void* lhs, double rhs)
{
if (rhs == 1.0) {
    return lhs;
    }
if (rhs == -1.0) {
    return manager->negate( lhs );
    }
return manager->divide( lhs, manager->param(rhs, false, "") );
}

extern "C" void* expr_divide_expression(void* lhs, void* rhs)
{
return manager->divide( lhs, rhs );
}

extern "C" void* expr_rdivide_int(int lhs, void* rhs)
{
if (lhs == 0) {
    return manager->zero;
    }
return manager->divide( manager->param(lhs, false, ""), rhs );
}

extern "C" void* expr_rdivide_double(double lhs, void* rhs)
{
if (lhs == 0.0) {
    return manager->zero;
    }
return manager->divide( manager->param(lhs, false, ""), rhs );
}


/*** POW ***/

extern "C" void* expr_pow_int(void* lhs, int rhs)
{
return manager->pow( lhs, manager->param(rhs, false, "") );
}

extern "C" void* expr_pow_double(void* lhs, double rhs)
{
return manager->pow( lhs, manager->param(rhs, false, "") );
}

extern "C" void* expr_pow_expression(void* lhs, void* rhs)
{
return manager->pow( lhs, rhs );
}

extern "C" void* expr_rpow_int(int lhs, void* rhs)
{
return manager->pow( manager->param(lhs, false, ""), rhs );
}

extern "C" void* expr_rpow_double(double lhs, void* rhs)
{
return manager->pow( manager->param(lhs, false, ""), rhs );
}


/*** OTHER ***/
extern "C" void* expr_negate(void* _expr)
{
return manager->negate( _expr );
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

extern "C" void* create_parameter_int(int value, int mutable_flag, const char* name)
{
return manager->param(value, mutable_flag, name);
}

extern "C" void* create_parameter_double(double value, int mutable_flag, const char* name)
{
return manager->param(value, mutable_flag, name);
}

extern "C" void* create_variable(int binary, int integer, double lb, double ub, double init, const char* name)
{
return manager->var(binary, integer, lb, ub, init, name);
}

extern "C" void create_variable_array(void* vars[], int num, int binary, int integer, double lb, double ub, double init, const char* name)
{
for (int i=0; i<num; i++)
    vars[i] =  manager->var(binary, integer, lb, ub, init, name);
}

extern "C" int get_variable_index(void* ptr)
{
Variable* v = static_cast<Variable*>(ptr);
return v->index;
}

extern "C" void variable_set_value(void* ptr, double val)
{
Variable* v = static_cast<Variable*>(ptr);
v->_value = val;
}

extern "C" double variable_get_value(void* ptr)
{
Variable* v = static_cast<Variable*>(ptr);
return v->_value;
}

extern "C" void variable_set_lb(void* ptr, double val)
{
Variable* v = static_cast<Variable*>(ptr);
v->lb = val;
}

extern "C" double variable_get_lb(void* ptr)
{
Variable* v = static_cast<Variable*>(ptr);
return v->lb;
}

extern "C" void variable_set_ub(void* ptr, double val)
{
Variable* v = static_cast<Variable*>(ptr);
v->ub = val;
}

extern "C" double variable_get_ub(void* ptr)
{
Variable* v = static_cast<Variable*>(ptr);
return v->ub;
}

extern "C" void get_numval_str(void* ptr, char* buf, int max)
{
NumericValue* v = static_cast<NumericValue*>(ptr);
v->snprintf(buf, max);
}

extern "C" double expr_get_value(void* ptr)
{
NumericValue* v = static_cast<NumericValue*>(ptr);
return v->value();
}

extern "C" double compute_numval_value(void* ptr)
{
NumericValue* v = static_cast<NumericValue*>(ptr);
return v->value();
}

extern "C" double compute_constraint_value(void* ptr, int body)
{
Constraint* v = static_cast<Constraint*>(ptr);
if (body)
    return v->value();
return v->boolean_value();
}

extern "C" void* create_inequality(void* expr, int strict_flag)
{
return manager->inequality(expr, strict_flag);
}

extern "C" void* create_equality(void* expr)
{
return manager->equality(expr);
}

extern "C" void* create_model()
{
Simple_ExprModel* tmp = new Simple_ExprModel();
models.push_back(tmp);
model = tmp;
manager = &(model->manager);
return tmp;
}

extern "C" void add_objective(void* model, void* expr)
{
Simple_ExprModel* tmp = static_cast<Simple_ExprModel*>(model);
NumericValue* _expr = static_cast<NumericValue*>(expr);
tmp->objectives.push_back(_expr);
}

extern "C" void add_inequality(void* model, void* ineq)
{
Simple_ExprModel* tmp = static_cast<Simple_ExprModel*>(model);
NumericValue* _ineq = static_cast<NumericValue*>(ineq);
tmp->inequalities.push_back(_ineq);
}

extern "C" void add_equality(void* model, void* eq)
{
Simple_ExprModel* tmp = static_cast<Simple_ExprModel*>(model);
NumericValue* _eq = static_cast<NumericValue*>(eq);
tmp->equalities.push_back(_eq);
}

extern "C" void print_model(void* model, int /*df*/)
{
Simple_ExprModel* tmp = static_cast<Simple_ExprModel*>(model);
tmp->print(std::cout);  // df?
}

extern "C" void build_model(void* /*model*/)
{
//Simple_ExprModel* tmp = static_cast<Simple_ExprModel*>(model);
//tmp->build();
}

extern "C" int get_nvariables(void* model)
{
ADModel* tmp = static_cast<ADModel*>(model);
return tmp->num_variables();
}

extern "C" double compute_objective_f(void* model, int i)
{
ADModel* tmp = static_cast<ADModel*>(model);
return tmp->compute_f(i);
}

extern "C" void compute_objective_df(void* model, double* df, int n, int i)
{
ADModel* tmp = static_cast<ADModel*>(model);
double f;
std::vector<double> _df(n);
tmp->compute_df(f, _df, i);
for (int i=0; i<n; i++)
  df[i] = _df[i];
}

extern "C" void compute_constraint_f(void* model, double* c, int n)
{
ADModel* tmp = static_cast<ADModel*>(model);
std::vector<double> _c(n);
tmp->compute_c(_c);
for (int i=0; i<n; i++)
  c[i] = _c[i];
}

extern "C" void compute_constraint_df(void* model, double* dc, int n, int i)
{
ADModel* tmp = static_cast<ADModel*>(model);
std::vector<double> _dc(n);
tmp->compute_dc(_dc, i);
for (int i=0; i<n; i++)
  dc[i] = _dc[i];
}

extern "C" void compute_Hv(void* model, double* v, double* Hv, int n, int i)
{
ADModel* tmp = static_cast<ADModel*>(model);
std::vector<double> _v(n);
for (int j=0; j<n; j++)
  _v[j] = v[j];
std::vector<double> _Hv(n);
tmp->compute_Hv(_v, _Hv, i);
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


extern "C" void* get_solver(const char* name)
{
return create_solver(name);
}

extern "C" void set_solver_model(void* solver, void* model)
{
Solver* _solver = static_cast<Solver*>(solver);
ADModel* _model = static_cast<ADModel*>(model);
_solver->set_model(_model);
}

extern "C" int solver_solve(void* solver)
{
Solver* _solver = static_cast<Solver*>(solver);
return _solver->solve();
}



/*** INTRINSIC ***/




extern "C" void* intrinsic_abs(void* expr)
{
NumericValue* e = static_cast<NumericValue*>(expr);
return e->manager->abs(e);
}

extern "C" void* intrinsic_pow(void* base, void* exponent)
{
NumericValue* b = static_cast<NumericValue*>(base);
NumericValue* e = static_cast<NumericValue*>(exponent);
return e->manager->pow(b, e);
}

extern "C" void* intrinsic_ceil(void* expr)
{
NumericValue* e = static_cast<NumericValue*>(expr);
return e->manager->ceil(e);
}

extern "C" void* intrinsic_floor(void* expr)
{
NumericValue* e = static_cast<NumericValue*>(expr);
return e->manager->floor(e);
}

extern "C" void* intrinsic_exp(void* expr)
{
NumericValue* e = static_cast<NumericValue*>(expr);
return e->manager->exp(e);
}

extern "C" void* intrinsic_log(void* expr)
{
NumericValue* e = static_cast<NumericValue*>(expr);
return e->manager->log(e);
}

extern "C" void* intrinsic_log10(void* expr)
{
NumericValue* e = static_cast<NumericValue*>(expr);
return e->manager->log10(e);
}

extern "C" void* intrinsic_sqrt(void* expr)
{
NumericValue* e = static_cast<NumericValue*>(expr);
return e->manager->sqrt(e);
}

extern "C" void* intrinsic_sin(void* expr)
{
NumericValue* e = static_cast<NumericValue*>(expr);
return e->manager->sin(e);
}

extern "C" void* intrinsic_cos(void* expr)
{
NumericValue* e = static_cast<NumericValue*>(expr);
return e->manager->cos(e);
}

extern "C" void* intrinsic_tan(void* expr)
{
NumericValue* e = static_cast<NumericValue*>(expr);
return e->manager->tan(e);
}

extern "C" void* intrinsic_sinh(void* expr)
{
NumericValue* e = static_cast<NumericValue*>(expr);
return e->manager->sinh(e);
}

extern "C" void* intrinsic_cosh(void* expr)
{
NumericValue* e = static_cast<NumericValue*>(expr);
return e->manager->cosh(e);
}

extern "C" void* intrinsic_tanh(void* expr)
{
NumericValue* e = static_cast<NumericValue*>(expr);
return e->manager->tanh(e);
}

extern "C" void* intrinsic_asin(void* expr)
{
NumericValue* e = static_cast<NumericValue*>(expr);
return e->manager->asin(e);
}

extern "C" void* intrinsic_acos(void* expr)
{
NumericValue* e = static_cast<NumericValue*>(expr);
return e->manager->acos(e);
}

extern "C" void* intrinsic_atan(void* expr)
{
NumericValue* e = static_cast<NumericValue*>(expr);
return e->manager->atan(e);
}

extern "C" void* intrinsic_asinh(void* expr)
{
NumericValue* e = static_cast<NumericValue*>(expr);
return e->manager->asinh(e);
}

extern "C" void* intrinsic_acosh(void* expr)
{
NumericValue* e = static_cast<NumericValue*>(expr);
return e->manager->acosh(e);
}

extern "C" void* intrinsic_atanh(void* expr)
{
NumericValue* e = static_cast<NumericValue*>(expr);
return e->manager->atanh(e);
}

