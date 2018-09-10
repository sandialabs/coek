
/* Declaration of C expression API functions */

/** EXPRESSIONS **/

/* __add__ */
void* add_expr_int(void* lhs, int rhs);
void* add_expr_float(void* lhs, float rhs);
void* add_expr_expression(void* lhs, void* rhs);
void* radd_expr_int(int lhs, void* rhs);
void* radd_expr_float(float lhs, void* rhs);

/* __mul__ */
void* mul_expr_int(void* lhs, int rhs);
void* mul_expr_float(void* lhs, float rhs);
void* mul_expr_expression(void* lhs, void* rhs);
void* rmul_expr_int(int lhs, void* rhs);
void* rmul_expr_float(float lhs, void* rhs);


void print_var(void* self);
void print_expr(void* self);

int expr_size(void* self);


void* create_variable(int binary, int integer);
int get_variable_index(void* var);

