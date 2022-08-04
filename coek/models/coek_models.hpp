#include <iostream>
#include <string>
#include <vector>
#include <coek/coek.hpp>

//
// jump
//

// lqcp
void lqcp_scalar(coek::Model& model, const std::vector<int>& data);
void lqcp_map(coek::Model& model, const std::vector<int>& data);
void lqcp_array(coek::Model& model, const std::vector<int>& data);
#ifdef COEK_WITH_COMPACT_MODEL
void lqcp_compact(coek::CompactModel& model, const std::vector<int>& data);
#endif

//
// misc
//

void knapsack_scalar(coek::Model& model, const std::vector<int>& data);
void nqueens_scalar(coek::Model& model, const std::vector<int>& data);

// pmedian
void pmedian_scalar(coek::Model& model, const std::vector<int>& data);
//void pmedian_map(coek::Model& model, const std::vector<int>& data);
//void pmedian_array(coek::Model& model, const std::vector<int>& data);
//void pmedian_compact(coek::Model& model, const std::vector<int>& data);


inline void print_models(std::ostream& os)
{
os <<
"  knapsack_scalar N\n"
"  lqcp_array N\n"
#ifdef COEK_WITH_COMPACT_MODEL
"  lqcp_compact N\n"
#endif
"  lqcp_map N\n"
"  lqcp_scalar N\n"
"  nqueens_scalar N\n"
"  pmedian_scalar N P\n"
;
}

inline void create_instance(coek::Model& model, const std::string& name, const std::vector<int>& data)
{
if (false) {}

//
// jump
//
else if (name == "lqcp_scalar")
    lqcp_scalar(model, data);
else if (name == "lqcp_map")
    lqcp_map(model, data);
else if (name == "lqcp_array")
    lqcp_array(model, data);
#ifdef COEK_WITH_COMPACT_MODEL
else if (name == "lqcp_compact") {
    coek::CompactModel cmodel;
    lqcp_compact(cmodel, data);
    model = cmodel.expand();
    }
#endif

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

