#include <coek/coek.hpp>
#include <map>
#include <vector>

void pmedian_scalar(coek::Model& model, size_t N, size_t P)
{
    size_t M = N;  // Customers

    std::vector<std::vector<double>> d;
    for (size_t n = 0; n < N; n++) {
        d.push_back(std::vector<double>(M));
        for (size_t m = 0; m < M; m++)
            d[n][m] = 1.0 + 1.0 / (n + m + 1);
    }

    std::vector<std::vector<coek::Variable>> x;
    for (size_t n = 0; n < N; n++) {
        x.push_back(std::vector<coek::Variable>(M));
        for (size_t m = 0; m < M; m++)
            model.add(x[n][m].bounds(0, 1).value(0));
    }

    std::vector<coek::Variable> y(N);
    for (size_t n = 0; n < N; n++)
        model.add(y[n].bounds(0, 1).value(0));

    // obj
    coek::Expression obj;
    for (size_t n = 0; n < N; n++)
        for (size_t m = 0; m < M; m++)
            obj += d[n][m] * x[n][m];
    model.add_objective(obj);

    // single_x
    for (size_t m = 0; m < M; m++) {
        coek::Expression c;
        for (size_t n = 0; n < N; n++)
            c += x[n][m];
        model.add(c == 1);
    }

    // bound_y
    for (size_t n = 0; n < N; n++)
        for (size_t m = 0; m < M; m++)
            model.add(x[n][m] - y[n] <= 0);

    // num_facilities
    coek::Expression num_facilities;
    for (size_t n = 0; n < N; n++)
        num_facilities += y[n];
    model.add(num_facilities == (int)P);
}
