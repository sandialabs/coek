#include <iostream>
#include <string>
#include <vector>
#include <coek/coek.hpp>

//
// jump
//
void fac_array(coek::Model& model, size_t F);
void fac_scalar(coek::Model& model, size_t F);
void lqcp_array(coek::Model& model, size_t N);
#ifdef COEK_WITH_COMPACT_MODEL
void lqcp_compact(coek::CompactModel& model, size_t N);
void lqcp_map(coek::Model& model, size_t N);
#endif
void lqcp_scalar(coek::Model& model, size_t N);

//
// misc
//
void knapsack_array(coek::Model& model, size_t N);
void knapsack_scalar(coek::Model& model, size_t N);
void nqueens_array(coek::Model& model, size_t N);
void nqueens_scalar(coek::Model& model, size_t N);
void pmedian_array(coek::Model& model, size_t N, size_t P);
void pmedian_scalar(coek::Model& model, size_t N, size_t P);


inline void print_models(std::ostream& os)
{
os <<
"  fac_array N\n"
"  fac_scalar N\n"
"  knapsack_array N\n"
"  knapsack_scalar N\n"
"  lqcp_array N\n"
#ifdef COEK_WITH_COMPACT_MODEL
"  lqcp_compact N\n"
"  lqcp_map N\n"
#endif
"  lqcp_scalar N\n"
"  nqueens_array N\n"
"  nqueens_scalar N\n"
"  pmedian_array N P\n"
"  pmedian_scalar N P\n"
;
}


inline void check_data(const std::string& name, const std::vector<size_t>& data, size_t num)
{
if (data.size() != num)
    throw std::runtime_error("Expected " + std::to_string(num) + " model parameters but only have " + std::to_string(data.size()));
}


inline void create_instance(coek::Model& model, const std::string& name, const std::vector<size_t>& data)
{
if (false) {}

//
// jump
//
else if (name == "fac_array") {
    check_data(name, data, 1);
    fac_array(model, data[0]);
    }
else if (name == "fac_scalar") {
    check_data(name, data, 1);
    fac_scalar(model, data[0]);
    }
else if (name == "lqcp_array") {
    check_data(name, data, 1);
    lqcp_array(model, data[0]);
    }
#ifdef COEK_WITH_COMPACT_MODEL
else if (name == "lqcp_compact") {
    check_data(name, data, 1);
    coek::CompactModel cmodel;
    lqcp_compact(cmodel, data[0]);
    model = cmodel.expand();
    }
else if (name == "lqcp_map") {
    check_data(name, data, 1);
    lqcp_map(model, data[0]);
    }
#endif
else if (name == "lqcp_scalar") {
    check_data(name, data, 1);
    lqcp_scalar(model, data[0]);
    }
//
// misc
//
else if (name == "knapsack_array") {
    check_data(name, data, 1);
    knapsack_array(model, data[0]);
    }
else if (name == "knapsack_scalar") {
    check_data(name, data, 1);
    knapsack_scalar(model, data[0]);
    }
else if (name == "nqueens_array") {
    check_data(name, data, 1);
    nqueens_array(model, data[0]);
    }
else if (name == "nqueens_scalar") {
    check_data(name, data, 1);
    nqueens_scalar(model, data[0]);
    }
else if (name == "pmedian_array") {
    check_data(name, data, 2);
    pmedian_array(model, data[0], data[1]);
    }
else if (name == "pmedian_scalar") {
    check_data(name, data, 2);
    pmedian_scalar(model, data[0], data[1]);
    }
else
    throw std::runtime_error("Unknown test model: " + name);
}

