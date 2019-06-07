
#include <cmath>
#include <cassert>
#include <sstream>

#include "expr/expr_manager.hpp"
#include "model.hpp"
#include "coek_util.hpp"
#include "solver.hpp"

using namespace coek;


/*** GLOBAL DATA ***/

ADModel* model = 0;
ExprManager* manager = 0;
bool default_model = true;
std::list<ADModel*> models;


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
NumericValue* e = static_cast<NumericValue*>(lhs);
return e->manager->plus( lhs, e->manager->param(rhs, false, "") );
}

extern "C" void* expr_plus_double(void* lhs, double rhs)
{
if (rhs == 0.0) {
    return lhs;
    }
NumericValue* e = static_cast<NumericValue*>(lhs);
return e->manager->plus( lhs, e->manager->param(rhs, false, "") );
}

extern "C" void* expr_plus_expression(void* lhs, void* rhs)
{
NumericValue* lhs_ = static_cast<NumericValue*>(lhs);
NumericValue* rhs_ = static_cast<NumericValue*>(rhs);
return lhs_->manager->plus( lhs_, rhs_ );
}

extern "C" void* expr_rplus_int(int lhs, void* rhs)
{
if (lhs == 0) {
    return rhs;
    }
NumericValue* e = static_cast<NumericValue*>(rhs);
return e->manager->plus( e->manager->param(lhs, false, ""), rhs );
}

extern "C" void* expr_rplus_double(double lhs, void* rhs)
{
if (lhs == 0.0) {
    return rhs;
    }
NumericValue* e = static_cast<NumericValue*>(rhs);
return e->manager->plus( e->manager->param(lhs, false, ""), rhs );
}


/*** SUB ***/

extern "C" void* expr_minus_int(void* lhs, int rhs)
{
if (rhs == 0) {
    return lhs;
    }
NumericValue* e = static_cast<NumericValue*>(lhs);
return e->manager->minus( lhs, e->manager->param(rhs, false, "") );
}

extern "C" void* expr_minus_double(void* lhs, double rhs)
{
if (rhs == 0.0) {
    return lhs;
    }
NumericValue* e = static_cast<NumericValue*>(lhs);
return e->manager->minus( lhs, e->manager->param(rhs, false, "") );
}

extern "C" void* expr_minus_expression(void* lhs, void* rhs)
{
NumericValue* lhs_ = static_cast<NumericValue*>(lhs);
NumericValue* rhs_ = static_cast<NumericValue*>(rhs);
return lhs_->manager->minus( lhs_, rhs_ );
}

extern "C" void* expr_rminus_int(int lhs, void* rhs)
{
NumericValue* e = static_cast<NumericValue*>(rhs);
if (lhs == 0) {
    return e->manager->negate( rhs );
    }
return e->manager->minus( e->manager->param(lhs, false, ""), rhs );
}

extern "C" void* expr_rminus_double(double lhs, void* rhs)
{
NumericValue* e = static_cast<NumericValue*>(rhs);
if (lhs == 0.0) {
    return e->manager->negate( rhs );
    }
return e->manager->minus( e->manager->param(lhs, false, ""), rhs );
}


/*** MUL ***/

extern "C" void* expr_times_int(void* lhs, int rhs)
{
if (rhs == 1) {
    return lhs;
    }

NumericValue* e = static_cast<NumericValue*>(lhs);
if (rhs == 0) {
    return e->manager->zero;
    }
if (rhs == -1) {
    return e->manager->negate( lhs );
    }
return e->manager->times( lhs, e->manager->param(rhs, false, "") );
}

extern "C" void* expr_times_double(void* lhs, double rhs)
{
if (rhs == 1.0) {
    return lhs;
    }

NumericValue* e = static_cast<NumericValue*>(lhs);
if (rhs == 0.0) {
    return e->manager->zero;
    }
if (rhs == -1.0) {
    return e->manager->negate( lhs );
    }
return e->manager->times( lhs, e->manager->param(rhs, false, "") );
}

extern "C" void* expr_times_expression(void* lhs, void* rhs)
{
NumericValue* lhs_ = static_cast<NumericValue*>(lhs);
NumericValue* rhs_ = static_cast<NumericValue*>(rhs);
return lhs_->manager->times( lhs_, rhs_ );
}

extern "C" void* expr_rtimes_int(int lhs, void* rhs)
{
if (lhs == 1) {
    return rhs;
    }

NumericValue* e = static_cast<NumericValue*>(rhs);
if (lhs == 0) {
    return e->manager->zero;
    }
if (lhs == -1) {
    return e->manager->negate( rhs );
    }
return e->manager->times( e->manager->param(lhs, false, ""), rhs );
}

extern "C" void* expr_rtimes_double(double lhs, void* rhs)
{
if (lhs == 1.0) {
    return rhs;
    }

NumericValue* e = static_cast<NumericValue*>(rhs);
if (lhs == 0.0) {
    return e->manager->zero;
    }
if (lhs == -1.0) {
    return e->manager->negate( rhs );
    }
return e->manager->times( e->manager->param(lhs, false, ""), rhs );
}


/*** DIV ***/

extern "C" void* expr_divide_int(void* lhs, int rhs)
{
if (rhs == 1) {
    return lhs;
    }

NumericValue* e = static_cast<NumericValue*>(lhs);
if (rhs == -1) {
    return e->manager->negate( lhs );
    }
return e->manager->divide( lhs, e->manager->param(rhs, false, "") );
}

extern "C" void* expr_divide_double(void* lhs, double rhs)
{
if (rhs == 1.0) {
    return lhs;
    }

NumericValue* e = static_cast<NumericValue*>(lhs);
if (rhs == -1.0) {
    return e->manager->negate( lhs );
    }
return e->manager->divide( lhs, e->manager->param(rhs, false, "") );
}

extern "C" void* expr_divide_expression(void* lhs, void* rhs)
{
NumericValue* lhs_ = static_cast<NumericValue*>(lhs);
NumericValue* rhs_ = static_cast<NumericValue*>(rhs);
return lhs_->manager->divide( lhs_, rhs_ );
}

extern "C" void* expr_rdivide_int(int lhs, void* rhs)
{
NumericValue* e = static_cast<NumericValue*>(rhs);
if (lhs == 0) {
    return e->manager->zero;
    }
if (e->is_parameter() and !e->is_mutable_parameter() and (e->_value==0.0))
   return 0;
return e->manager->divide( e->manager->param(lhs, false, ""), rhs );
}

extern "C" void* expr_rdivide_double(double lhs, void* rhs)
{
NumericValue* e = static_cast<NumericValue*>(rhs);
if (lhs == 0.0) {
    return e->manager->zero;
    }
if (e->is_parameter() and !e->is_mutable_parameter() and (e->_value==0.0))
   return 0;
return e->manager->divide( e->manager->param(lhs, false, ""), rhs );
}


/*** POW ***/

extern "C" void* expr_pow_int(void* lhs, int rhs)
{
NumericValue* e = static_cast<NumericValue*>(lhs);
return e->manager->pow( lhs, e->manager->param(rhs, false, "") );
}

extern "C" void* expr_pow_double(void* lhs, double rhs)
{
NumericValue* e = static_cast<NumericValue*>(lhs);
return e->manager->pow( lhs, e->manager->param(rhs, false, "") );
}

extern "C" void* expr_pow_expression(void* lhs, void* rhs)
{
NumericValue* lhs_ = static_cast<NumericValue*>(lhs);
NumericValue* rhs_ = static_cast<NumericValue*>(rhs);
return lhs_->manager->pow( lhs_, rhs_ );
}

extern "C" void* expr_rpow_int(int lhs, void* rhs)
{
NumericValue* e = static_cast<NumericValue*>(rhs);
return e->manager->pow( e->manager->param(lhs, false, ""), rhs );
}

extern "C" void* expr_rpow_double(double lhs, void* rhs)
{
NumericValue* e = static_cast<NumericValue*>(rhs);
return e->manager->pow( e->manager->param(lhs, false, ""), rhs );
}


/*** OTHER ***/
extern "C" void* expr_negate(void* _expr)
{
NumericValue* e = static_cast<NumericValue*>(_expr);
return e->manager->negate( e );
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
    return root->manager->zero;
return diff[var];
}

extern "C" void* create_parameter_int(void* _model, int value, int mutable_flag, const char* name)
{
if (_model == 0) {
    runtime_assert(manager != 0, "Error calling create_parameter_int().  An optimization model must be created before a parameter can be created!");
    return manager->param(value, mutable_flag, name);
    }
ADModel* tmp = static_cast<ADModel*>(_model);
return tmp->manager.param(value, mutable_flag, name);
}

extern "C" void* create_parameter_double(void* _model, double value, int mutable_flag, const char* name)
{
if (_model == 0) {
    runtime_assert(manager != 0, "Error calling create_parameter_double().  An optimization model must be created before a parameter can be created!");
    return manager->param(value, mutable_flag, name);
    }
ADModel* tmp = static_cast<ADModel*>(_model);
return tmp->manager.param(value, mutable_flag, name);
}

extern "C" void parameter_set_value(void* ptr, double val)
{
Parameter* v = static_cast<Parameter*>(ptr);
v->set_value(val);
}

extern "C" void* get_parameter_zero(void* _model)
{
if (_model == 0) {
    runtime_assert(manager != 0, "Error calling get_parameter_zero().  An optimization model must be created before a parameter can be created!");
    return manager->zero;
    }
ADModel* tmp = static_cast<ADModel*>(_model);
return tmp->manager.zero;
}

extern "C" void* get_parameter_one(void* _model)
{
if (_model == 0) {
    runtime_assert(manager != 0, "Error calling get_parameter_one().  An optimization model must be created before a parameter can be created!");
    return manager->one;
    }
ADModel* tmp = static_cast<ADModel*>(_model);
return tmp->manager.one;
}

extern "C" void* get_parameter_negative_one(void* _model)
{
if (_model == 0) {
    runtime_assert(manager != 0, "Error calling get_parameter_negative_one().  An optimization model must be created before a parameter can be created!");
    return manager->negative_one;
    }
ADModel* tmp = static_cast<ADModel*>(_model);
return tmp->manager.negative_one;
}

extern "C" void* create_variable(void* _model, int binary, int integer, double lb, double ub, double init, const char* name)
{
if (_model == 0) {
    runtime_assert(manager != 0, "Error calling create_variable().  An optimization model must be created before a variable can be created!");
    return manager->var(binary, integer, lb, ub, init, name);
    }
ADModel* tmp = static_cast<ADModel*>(_model);
return tmp->manager.var(binary, integer, lb, ub, init, name);
}

extern "C" void create_variable_array(void* _model, void* vars[], int num, int binary, int integer, double lb, double ub, double init, const char* name)
{
ExprManager* _manager;
if (_model == 0) {
    runtime_assert(manager != 0, "Error calling create_variable_array().  An optimization model must be created before a variable can be created!");
    _manager = manager;
    }
else {
    ADModel* tmp = static_cast<ADModel*>(_model);
    _manager = &(tmp->manager);
    }
for (int i=0; i<num; i++)
    vars[i] =  _manager->var(binary, integer, lb, ub, init, name);
}

extern "C" int variable_get_index(void* ptr)
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

extern "C" int variable_is_binary(void* ptr)
{
Variable* v = static_cast<Variable*>(ptr);
return v->binary;
}

extern "C" int variable_is_integer(void* ptr)
{
Variable* v = static_cast<Variable*>(ptr);
return v->integer;
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

extern "C" double expr_repn_constant(void* expr)
{
NumericValue* e = static_cast<NumericValue*>(expr);
return e->manager->constant(e);
}

extern "C" int expr_repn_nlinear_vars(void* expr)
{
NumericValue* e = static_cast<NumericValue*>(expr);
return e->manager->nlinear_vars(e);
}

extern "C" void* expr_repn_linear_var(void* expr, int i)
{
NumericValue* e = static_cast<NumericValue*>(expr);
return e->manager->linear_var(e, i);
}

extern "C" double expr_repn_linear_coef(void* expr, int i)
{
NumericValue* e = static_cast<NumericValue*>(expr);
return e->manager->linear_coef(e, i);
}

extern "C" void* create_inequality(void* expr, int strict_flag)
{
NumericValue* e = static_cast<NumericValue*>(expr);
return e->manager->inequality(e, strict_flag);
}

extern "C" void* create_equality(void* expr)
{
NumericValue* e = static_cast<NumericValue*>(expr);
return e->manager->equality(e);
}

extern "C" void* create_model()
{
// We always create a default model, so after creating it,
// the first call to this function simply returns the 
// default model.
if (default_model and (models.size() == 1)) {
   ADModel* tmp = models.front();
   default_model = false;
   return tmp;
   }

ADModel* tmp = new ADModel();
models.push_back(tmp);
model = tmp;
manager = &(model->manager);
return tmp;
}

extern "C" void write_model(void* _model, const char* filename)
{
ADModel* tmp = static_cast<ADModel*>(_model);
std::string fname = filename;
tmp->write(fname);
}

extern "C" void add_objective(void* _model, void* expr)
{
ADModel* tmp = static_cast<ADModel*>(_model);
NumericValue* _expr = static_cast<NumericValue*>(expr);
tmp->add_objective(_expr);
}

extern "C" void add_inequality(void* _model, void* ineq)
{
ADModel* tmp = static_cast<ADModel*>(_model);
NumericValue* _ineq = static_cast<NumericValue*>(ineq);
tmp->inequalities.push_back(_ineq);
}

extern "C" void add_equality(void* _model, void* eq)
{
ADModel* tmp = static_cast<ADModel*>(_model);
NumericValue* _eq = static_cast<NumericValue*>(eq);
tmp->equalities.push_back(_eq);
}

extern "C" void* get_objective(void* _model, int n)
{
ADModel* tmp = static_cast<ADModel*>(_model);
return tmp->get_objective(n);
}

extern "C" void* get_constraint(void* _model, int n)
{
ADModel* tmp = static_cast<ADModel*>(_model);
return tmp->get_constraint(n);
}

extern "C" void print_model(void* _model, int /*df*/)
{
ADModel* tmp = static_cast<ADModel*>(_model);
tmp->print(std::cout);  // df?
}

extern "C" void build_model(void* _model)
{
ADModel* tmp = static_cast<ADModel*>(_model);
tmp->build();
}

extern "C" int get_nvariables(void* _model)
{
ADModel* tmp = static_cast<ADModel*>(_model);
return tmp->num_variables();
}

extern "C" int get_nobjectives(void* _model)
{
ADModel* tmp = static_cast<ADModel*>(_model);
return tmp->num_objectives();
}

extern "C" int get_nconstraints(void* _model)
{
ADModel* tmp = static_cast<ADModel*>(_model);
return tmp->num_constraints();
}

extern "C" int get_niconstraints(void* _model)
{
ADModel* tmp = static_cast<ADModel*>(_model);
return tmp->num_ineqconstraints();
}

extern "C" double compute_objective_f(void* _model, int i)
{
ADModel* tmp = static_cast<ADModel*>(_model);
return tmp->compute_f(i);
}

extern "C" void compute_objective_df(void* _model, double* df, int n, int i)
{
ADModel* tmp = static_cast<ADModel*>(_model);
double f;
std::vector<double> _df(n);
tmp->compute_df(f, _df, i);
for (int i=0; i<n; i++)
  df[i] = _df[i];
}

extern "C" void compute_constraint_f(void* _model, double* c, int n)
{
ADModel* tmp = static_cast<ADModel*>(_model);
std::vector<double> _c(n);
tmp->compute_c(_c);
for (int i=0; i<n; i++)
  c[i] = _c[i];
}

extern "C" void compute_constraint_df(void* _model, double* dc, int n, int i)
{
ADModel* tmp = static_cast<ADModel*>(_model);
std::vector<double> _dc(n);
tmp->compute_dc(_dc, i);
for (int i=0; i<n; i++)
  dc[i] = _dc[i];
}

extern "C" void compute_Hv(void* _model, double* v, double* Hv, int n, int i)
{
ADModel* tmp = static_cast<ADModel*>(_model);
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

extern "C" void set_solver_model(void* solver, void* _model)
{
Solver* _solver = static_cast<Solver*>(solver);
ADModel* admodel = static_cast<ADModel*>(_model);
_solver->set_model(admodel);
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

extern "C" void coek_initialize(void)
{
create_model();
}

extern "C" void coek_finalize(void)
{
for (std::list<ADModel*>::iterator it = models.begin(); it != models.end(); it++)
  delete *it;
models.resize(0);
model = 0;
manager = 0;
}


class COEK_CAPI_INITIALIZE
{
  COEK_CAPI_INITIALIZE(void) {
    // Create the default model
    coek_initialize();
    }

  ~COEK_CAPI_INITIALIZE(void) {
    coek_finalize();
    }
};

COEK_CAPI_INITIALIZE _init();
