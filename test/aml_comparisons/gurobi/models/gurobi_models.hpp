#include <iostream>
#include <string>
#include <vector>

#include "gurobi_c++.h"

//
// jump
//
void fac(GRBModel& model, size_t F);
void lqcp(GRBModel& model, size_t N);

//
// misc
//
void knapsack_scalar(GRBModel& model, size_t N);
void nqueens_scalar(GRBModel& model, size_t N);
void pmedian_scalar(GRBModel& model, size_t N, size_t P);

inline void print_models(std::ostream& os)
{
    os << "  knapsack-scalar N\n"
          "  lqcp N\n"
          "  fac F\n"
          "  nqueens-scalar N\n"
          "  pmedian-scalar N P\n";
}

inline void check_data(const std::string& name, const std::vector<size_t>& data, size_t num)
{
    if (data.size() != num)
        throw std::runtime_error("Expected " + std::to_string(num)
                                 + " model parameters but only have "
                                 + std::to_string(data.size()));
}

inline void create_instance(GRBModel& model, const std::string& name,
                            const std::vector<size_t>& data)
{
    if (false) {
    }

    //
    // jump
    //
    else if (name == "fac") {
        check_data(name, data, 1);
        fac(model, data[0]);
    }
    else if (name == "lqcp") {
        check_data(name, data, 1);
        lqcp(model, data[0]);
    }
    //
    // misc
    //
    else if (name == "knapsack-scalar") {
        check_data(name, data, 1);
        knapsack_scalar(model, data[0]);
    }
    else if (name == "nqueens-scalar") {
        check_data(name, data, 1);
        nqueens_scalar(model, data[0]);
    }
    else if (name == "pmedian-scalar") {
        check_data(name, data, 2);
        pmedian_scalar(model, data[0], data[1]);
    }
    else
        throw std::runtime_error("Unknown test model: " + name);
}
