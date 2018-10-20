#include <stdlib.h>
#include <stdio.h>
#include "gurobi_c++.h"
#include "expr_types.h"

GRBEnv* env = 0;
GRBModel* model = 0;




extern "C" void* solver_gurobi_env(void)
{
env = new GRBEnv();
return env;
}


extern "C" void* solver_gurobi_model(void* _env)
{
GRBEnv* __env = static_cast<GRBEnv*>(_env);
model = new GRBModel(__env);
return model;
}


void collect_terms(NumericValue* root, std::map<int, double>& linear, std::map<std::pair<int,int>, double>& quadratic)
{
}


extern "C" void gurobi_solve(void* model, void* x, int n)
{
/*
GRBModel* _model = static_cast<GRBModel*>(model);

// TODO

double* _x = static_cast<double*>(x);
*/
}
