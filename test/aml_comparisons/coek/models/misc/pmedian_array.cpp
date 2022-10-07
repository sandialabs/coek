#include <coek/coek.hpp>
#include <map>
#include <vector>

void pmedian_array(coek::Model& model, size_t N, size_t P)
{
    size_t M = N;  // Customers

    std::vector<std::vector<double>> d;
    for (size_t n : coek::range(N)) {
        d.push_back(std::vector<double>(M));
        for (size_t m : coek::range(M)) d[n][m] = 1.0 + 1.0 / (n + m + 1);
    }

    auto x = model.add(coek::variable("x", {N, M})).bounds(0, 1).value(0);

    auto y = model.add(coek::variable("y", N)).bounds(0, 1).value(0);

    // obj
    auto obj = coek::expression();
    for (size_t n : coek::range(N))
        for (size_t m : coek::range(M)) obj += d[n][m] * x(n, m);
    model.add_objective(obj);

    // single_x
    for (size_t m : coek::range(M)) {
        auto c = coek::expression();
        for (size_t n : coek::range(N)) c += x(n, m);
        model.add(c == 1);
    }

    // bound_y
    for (size_t n : coek::range(N))
        for (size_t m : coek::range(M)) model.add(x(n, m) - y(n) <= 0);

    // num_facilities
    auto num_facilities = coek::expression();
    for (size_t n : coek::range(N)) num_facilities += y(n);
    model.add(num_facilities == (int)P);
}
