// #define _GLIBCXX_USE_CXX11_ABI 0
#include <functional>
#include <map>
#include <random>
#include <string>
#include <vector>

#include "gurobi_c++.h"

void knapsack_scalar(GRBModel& model, size_t N)
{
    N *= 1000;
    double W = N / 10.0;

    std::mt19937 rng(10000);
    std::uniform_real_distribution<double> distribution(0, 1);
    auto uniform = std::bind(distribution, rng);

    std::vector<double> v(N);
    std::vector<double> w(N);
    for (size_t n = 0; n < N; n++) {
        v[n] = uniform();
        w[n] = uniform();
    }

    std::vector<GRBVar> x(N);
    for (size_t n = 0; n < N; n++) x[n] = model.addVar(0, 1, 0, GRB_CONTINUOUS);

    // obj
    GRBLinExpr obj;
    for (size_t n = 0; n < N; n++) obj += v[n] * x[n];
    model.setObjective(obj);

    // con
    GRBLinExpr con;
    for (size_t n = 0; n < N; n++) con += w[n] * x[n];
    model.addConstr(con <= W);
}
