#include <coek/coek.hpp>
#include <iostream>
#include <string>
#include <vector>

//
// jump
//
void fac_scalar(coek::Model& model, size_t F);
void lqcp_scalar(coek::Model& model, size_t N);
#if __cpp_lib_variant
void fac_array(coek::Model& model, size_t F);
void lqcp_array(coek::Model& model, size_t N);
#ifdef COEK_WITH_COMPACT_MODEL
void lqcp_compact(coek::CompactModel& model, size_t N);
void lqcp_map(coek::Model& model, size_t N);
#endif
#endif

//
// misc
//
void knapsack_scalar(coek::Model& model, size_t N);
void nqueens_scalar(coek::Model& model, size_t N);
void pmedian_scalar(coek::Model& model, size_t N, size_t P);
#if __cpp_lib_variant
void knapsack_array(coek::Model& model, size_t N);
void nqueens_array(coek::Model& model, size_t N);
void pmedian_array(coek::Model& model, size_t N, size_t P);
#endif

inline void print_models(std::ostream& os)
{
    os << 
#if __cpp_lib_variant
          "  fac-array N\n"
#endif
          "  fac-scalar N\n"
#if __cpp_lib_variant
          "  knapsack-array N\n"
#endif
          "  knapsack-scalar N\n"
          "  lqcp-array N\n"
#ifdef COEK_WITH_COMPACT_MODEL
          "  lqcp-compact N\n"
          "  lqcp-map N\n"
#endif
          "  lqcp-scalar N\n"
#if __cpp_lib_variant
          "  nqueens-array N\n"
#endif
          "  nqueens-scalar N\n"
#if __cpp_lib_variant
          "  pmedian-array N P\n"
#endif
          "  pmedian-scalar N P\n";
}

inline void check_data(const std::string& name, const std::vector<size_t>& data, size_t num)
{
    if (data.size() != num)
        throw std::runtime_error("Expected " + std::to_string(num) + " parameters in model " + name
                                 + " but only have " + std::to_string(data.size()));
}

inline void create_instance(coek::Model& model, const std::string& name,
                            const std::vector<size_t>& data)
{
    if (false) {
    }

    //
    // jump
    //
#if __cpp_lib_variant
    else if (name == "fac-array") {
        check_data(name, data, 1);
        fac_array(model, data[0]);
    }
#endif
    else if (name == "fac-scalar") {
        check_data(name, data, 1);
        fac_scalar(model, data[0]);
    }
#if __cpp_lib_variant
    else if (name == "lqcp-array") {
        check_data(name, data, 1);
        lqcp_array(model, data[0]);
    }
#ifdef COEK_WITH_COMPACT_MODEL
    else if (name == "lqcp-compact") {
        check_data(name, data, 1);
        coek::CompactModel cmodel;
        lqcp_compact(cmodel, data[0]);
        model = cmodel.expand();
    }
    else if (name == "lqcp-map") {
        check_data(name, data, 1);
        lqcp_map(model, data[0]);
    }
#endif
#endif
    else if (name == "lqcp-scalar") {
        check_data(name, data, 1);
        lqcp_scalar(model, data[0]);
    }
    //
    // misc
    //
#if __cpp_lib_variant
    else if (name == "knapsack-array") {
        check_data(name, data, 1);
        knapsack_array(model, data[0]);
    }
#endif
    else if (name == "knapsack-scalar") {
        check_data(name, data, 1);
        knapsack_scalar(model, data[0]);
    }
#if __cpp_lib_variant
    else if (name == "nqueens-array") {
        check_data(name, data, 1);
        nqueens_array(model, data[0]);
    }
#endif
    else if (name == "nqueens-scalar") {
        check_data(name, data, 1);
        nqueens_scalar(model, data[0]);
    }
#if __cpp_lib_variant
    else if (name == "pmedian-array") {
        check_data(name, data, 2);
        pmedian_array(model, data[0], data[1]);
    }
#endif
    else if (name == "pmedian-scalar") {
        check_data(name, data, 2);
        pmedian_scalar(model, data[0], data[1]);
    }
    else
        throw std::runtime_error("Unknown test model: " + name);
}
