
#ifndef __expr_api_h
#define __expr_api_h


typedef void* apival_t;

/* Declaration of C expression API functions */

void* misc_getnull(void);
double misc_getnan(void);

/** EXPRESSIONS **/

/* __add__ */
void* expr_plus_int(void* lhs, int rhs);
void* expr_plus_double(void* lhs, double rhs);
void* expr_plus_expression(void* lhs, void* rhs);
void* expr_rplus_int(int lhs, void* rhs);
void* expr_rplus_double(double lhs, void* rhs);

/* __sub__ */
void* expr_minus_int(void* lhs, int rhs);
void* expr_minus_double(void* lhs, double rhs);
void* expr_minus_expression(void* lhs, void* rhs);
void* expr_rminus_int(int lhs, void* rhs);
void* expr_rminus_double(double lhs, void* rhs);

/* __mul__ */
void* expr_times_int(void* lhs, int rhs);
void* expr_times_double(void* lhs, double rhs);
void* expr_times_expression(void* lhs, void* rhs);
void* expr_rtimes_int(int lhs, void* rhs);
void* expr_rtimes_double(double lhs, void* rhs);

/* __div__ */
void* expr_divide_int(void* lhs, int rhs);
void* expr_divide_double(void* lhs, double rhs);
void* expr_divide_expression(void* lhs, void* rhs);
void* expr_rdivide_int(int lhs, void* rhs);
void* expr_rdivide_double(double lhs, void* rhs);

/* __pow__ */
void* expr_pow_int(void* lhs, int rhs);
void* expr_pow_double(void* lhs, double rhs);
void* expr_pow_expression(void* lhs, void* rhs);
void* expr_rpow_int(int lhs, void* rhs);
void* expr_rpow_double(double lhs, void* rhs);

/* OTHER */
void* expr_negate(void* expr);

void print_parameter(void* self);
void print_var(void* self);
void print_expr(void* self);
void print_model(void* self, int df);

int expr_size(void* self);
void* expr_diff(void* expr, void* variable);


void* create_parameter_int(int value, int mutable_flag, const char* name);
void* create_parameter_double(double value, int mutable_flag, const char* name);
void* get_parameter_zero();
void* get_parameter_one();
void* get_parameter_negative_one();

void*  create_variable(int binary, int integer, double lb, double ub, double init, const char* name);
void   create_variable_array(void* array[], int num, int binary, int integer, double lb, double ub, double init, const char* name);
int    variable_get_index(void* variable);
void   variable_set_value(void* variable, double value);
double variable_get_value(void* variable);
void   variable_set_lb(void* variable, double value);
double variable_get_lb(void* variable);
void   variable_set_ub(void* variable, double value);
double variable_get_ub(void* variable);

void get_numval_str(void* numval, char* buf, int n);
double expr_get_value(void* numval);
double compute_numval_value(void* numval);
double compute_constraint_value(void* numval, int body_flag);


void* create_inequality(void* self, int strict_flag);
void* create_equality(void* self);
void* create_model(void);

void add_objective(void* model, void* expr);
void add_inequality(void* model, void* ineq);
void add_equality(void* model, void* eq);

void build_model(void* model);
int get_nvariables(void* model);

double compute_objective_f(void* model, int i);
void compute_objective_df(void* model, double*, int n, int i);
void compute_constraint_f(void* model, double*, int n);
void compute_constraint_df(void* model, double*, int n, int i);

void compute_Hv(void* model, double* v, double* Hv, int n, int i);

void test_callback(int(*callback)(int, int));
void visitor_walk(void* expr, void(*enter_callback)(void*, void*, void*), void(*exit_callback)(void*, void*, void*), void* visitor);

void* get_solver(const char* name);
int solver_solve(void* solver);
void set_solver_model(void* solver, void* model);




/* Intrinsic Expression Functions */

void* intrinsic_abs(void* expr);
void* intrinsic_pow(void* lhs, void* rhs);
void* intrinsic_ceil(void* expr);
void* intrinsic_floor(void* expr);
void* intrinsic_exp(void* expr);
void* intrinsic_log(void* expr);
void* intrinsic_log10(void* expr);
void* intrinsic_sqrt(void* expr);
void* intrinsic_sin(void* expr);
void* intrinsic_cos(void* expr);
void* intrinsic_tan(void* expr);
void* intrinsic_sinh(void* expr);
void* intrinsic_cosh(void* expr);
void* intrinsic_tanh(void* expr);
void* intrinsic_asin(void* expr);
void* intrinsic_acos(void* expr);
void* intrinsic_atan(void* expr);
void* intrinsic_asinh(void* expr);
void* intrinsic_acosh(void* expr);
void* intrinsic_atanh(void* expr);


#endif
