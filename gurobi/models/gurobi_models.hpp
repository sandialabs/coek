#include <iostream>
#include <string>
#include <vector>
#include "gurobi_c++.h"

//
// jump
//
void fac(GRBModel& model, const std::vector<int>& data);
void lqcp(GRBModel& model, const std::vector<int>& data);

//
// misc
//
void knapsack_scalar(GRBModel& model, const std::vector<int>& data);
void nqueens_scalar(GRBModel& model, const std::vector<int>& data);
void pmedian_scalar(GRBModel& model, const std::vector<int>& data);


inline void print_models(std::ostream& os)
{
os <<
"  knapsack_scalar N\n"
"  lqcp N\n"
"  fac F\n"
"  nqueens_scalar N\n"
"  pmedian_scalar N P\n"
;
}

inline void create_instance(GRBModel& model, const std::string& name, const std::vector<int>& data)
{
if (false) {}

//
// jump
//
else if (name == "fac")
    fac(model, data);
else if (name == "lqcp")
    lqcp(model, data);

//
// misc
//
else if (name == "knapsack_scalar")
    knapsack_scalar(model, data);

else if (name == "nqueens_scalar")
    nqueens_scalar(model, data);

else if (name == "pmedian_scalar")
    pmedian_scalar(model, data);

else
    throw std::runtime_error("Unknown test model: " + name);
}

