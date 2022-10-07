//#define _GLIBCXX_USE_CXX11_ABI 0
#include <map>
#include <vector>

#include "gurobi_c++.h"

void pmedian_scalar(GRBModel& model, size_t N, size_t P)
{
    size_t M = N;

    std::vector<std::vector<double>> d(N, std::vector<double>(M));
    for (size_t n = 0; n < N; n++)
        for (size_t m = 0; m < M; m++) d[n][m] = 1.0 + 1.0 / (n + m + 1);

    std::vector<std::vector<GRBVar>> x(N, std::vector<GRBVar>(M));
    for (size_t n = 0; n < N; n++)
        for (size_t m = 0; m < M; m++) x[n][m] = model.addVar(0, 1, 0, GRB_CONTINUOUS);

    std::vector<GRBVar> y(N);
    for (size_t n = 0; n < N; n++) y[n] = model.addVar(0, 1, 0, GRB_CONTINUOUS);

    // obj
    GRBLinExpr obj;
    for (size_t n = 0; n < N; n++)
        for (size_t m = 0; m < M; m++) obj += d[n][m] * x[n][m];
    model.setObjective(obj);

    // single_x
    for (size_t m = 0; m < M; m++) {
        GRBLinExpr c;
        for (size_t n = 0; n < N; n++) c += x[n][m];
        model.addConstr(c == 1);
    }

    // bound_y
    for (size_t n = 0; n < N; n++)
        for (size_t m = 0; m < M; m++) model.addConstr(x[n][m] - y[n] <= 0);

    // num_facilities
    GRBLinExpr num_facilities;
    for (size_t n = 0; n < N; n++) num_facilities += y[n];
    model.addConstr(num_facilities == (int)P);
}
