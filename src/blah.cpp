#include <vector>
#include <iostream>
extern "C" {
#include "expr_api.h"
}

#define N 1000000

class Variable;
class NumericValue;


int main(int, char**)
{
std::vector<Variable*> v(N);
for (int i=0; i<N; i++)
    v[i] = static_cast<Variable*>(create_variable(0,0,0.0,1.0,1.0,""));

void* e = create_parameter_int(0,0,"");
for (int i=0; i<N; i++) {
    void* _tmp = mul_expr_int(v[i], i);
    e = add_expr_expression(e, _tmp);
    }

//NumericValue* expr = static_cast<NumericValue*>(e);

//#std::cout << expr->size() std::endl;


return 0;
}
