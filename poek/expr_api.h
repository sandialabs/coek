
/* Declaration of C expression API functions */

/** EXPRESSIONS **/

/* __add__ */
void* add_expr_int(void* lhs, int rhs);
void* add_expr_double(void* lhs, double rhs);
void* add_expr_expression(void* lhs, void* rhs);
void* radd_expr_int(int lhs, void* rhs);
void* radd_expr_double(double lhs, void* rhs);

/* __mul__ */
void* mul_expr_int(void* lhs, int rhs);
void* mul_expr_double(void* lhs, double rhs);
void* mul_expr_expression(void* lhs, void* rhs);
void* rmul_expr_int(int lhs, void* rhs);
void* rmul_expr_double(double lhs, void* rhs);


void print_var(void* self);
void print_expr(void* self);
void print_model(void* self);

int expr_size(void* self);


void* create_parameter_int(int value, int mutable_flag);
void* create_parameter_double(double value, int mutable_flag);

void* create_variable(int binary, int integer);
int get_variable_index(void* var);

void* create_inequality(void* self);
void* create_equality(void* self);
void* create_model(void);

void add_objective(void* model, void* expr);
void add_inequality(void* model, void* ineq);
void add_equality(void* model, void* eq);

void build_model(void* model);

void set_variable_value(void* variable, double value);
double compute_objective(void* model, int i);
