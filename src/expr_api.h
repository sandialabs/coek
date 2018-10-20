
/* Declaration of C expression API functions */

void* misc_getnull(void);
double misc_getnan(void);

/** EXPRESSIONS **/

/* __add__ */
void* add_expr_int(void* lhs, int rhs);
void* add_expr_double(void* lhs, double rhs);
void* add_expr_expression(void* lhs, void* rhs);
void* radd_expr_int(int lhs, void* rhs);
void* radd_expr_double(double lhs, void* rhs);

/* __sub__ */
void* sub_expr_int(void* lhs, int rhs);
void* sub_expr_double(void* lhs, double rhs);
void* sub_expr_expression(void* lhs, void* rhs);
void* rsub_expr_int(int lhs, void* rhs);
void* rsub_expr_double(double lhs, void* rhs);

/* __mul__ */
void* mul_expr_int(void* lhs, int rhs);
void* mul_expr_double(void* lhs, double rhs);
void* mul_expr_expression(void* lhs, void* rhs);
void* rmul_expr_int(int lhs, void* rhs);
void* rmul_expr_double(double lhs, void* rhs);

/* __div__ */
void* div_expr_int(void* lhs, int rhs);
void* div_expr_double(void* lhs, double rhs);
void* div_expr_expression(void* lhs, void* rhs);
void* rdiv_expr_int(int lhs, void* rhs);
void* rdiv_expr_double(double lhs, void* rhs);

/* __pow__ */
void* pow_expr_int(void* lhs, int rhs);
void* pow_expr_double(void* lhs, double rhs);
void* pow_expr_expression(void* lhs, void* rhs);
void* rpow_expr_int(int lhs, void* rhs);
void* rpow_expr_double(double lhs, void* rhs);

/* OTHER */
void* neg_expr(void* expr);

void print_parameter(void* self);
void print_var(void* self);
void print_expr(void* self);
void print_model(void* self, int df);

int expr_size(void* self);
void* expr_diff(void* expr, void* variable);


void* create_parameter_int(int value, int mutable_flag, const char* name);
void* create_parameter_double(double value, int mutable_flag, const char* name);

void* create_variable(int binary, int integer, double lb, double ub, double init, const char* name);
void create_variable_array(void* array[], int num, int binary, int integer, double lb, double ub, double init, const char* name);
int get_variable_index(void* variable);
void set_variable_value(void* variable, double value);
double get_variable_value(void* variable);
void set_variable_lb(void* variable, double value);
double get_variable_lb(void* variable);
void set_variable_ub(void* variable, double value);
double get_variable_ub(void* variable);

void get_numval_str(void* numval, char* buf, int n);
double get_numval_value(void* numval);
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
void* intrinsic_pow(void* expr);
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

