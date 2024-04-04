#include <cmath>
#include <coek/coek.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

namespace {

inline double yt(size_t j, double dx) { return 0.5 * (1 - (j * dx) * (j * dx)); }

}  // namespace

#ifdef COEK_WITH_COMPACT_MODEL
void lqcp_map(coek::Model& model, size_t n)
{
    size_t m = n;
    size_t n1 = n - 1;
    double dx = 1.0 / n;
    double T = 1.58;
    double dt = T / m;
    double h2 = dx * dx;
    double a = 0.001;

    auto M = coek::RangeSet(0, m);
    auto N = coek::RangeSet(0, n);
    auto y = model.add(coek::variable("y", M * N)).bounds(0, 1).value(0);
    auto u = model.add(coek::variable("u", coek::RangeSet(1, m))).bounds(-1, 1).value(0);

    // OBJECTIVE
    // First term
    auto term1 = coek::expression();
    term1 += (y(m, 0) - yt(0, dx)) * (y(m, 0) - yt(0, dx));
    for (size_t j = 1; j <= n - 1; j++)
        term1 += 2 * (y(m, j) - yt(j, dx)) * (y(m, j) - yt(j, dx));
    term1 += (y(m, n) - yt(n, dx)) * (y(m, n) - yt(n, dx));

    // Second term
    auto term2 = coek::expression();
    for (size_t i = 1; i <= m - 1; i++)
        term2 += 2 * u(i) * u(i);
    term2 += u(m) * u(m);

    model.add(coek::objective(0.25 * dx * term1 + 0.25 * a * dt * term2));

    // PDE
    const auto pde_coef = T * 0.5 * n;  // == dt*0.5/h2
    for (size_t i = 0; i < m; i++)
        for (size_t j = 1; j < n; j++)
            model.add(y(i + 1, j) - y(i, j)
                      == pde_coef
                             * (y(i, j - 1) - 2 * y(i, j) + y(i, j + 1) + y(i + 1, j - 1)
                                - 2 * y(i + 1, j) + y(i + 1, j + 1)));

    // IC
    for (size_t j = 0; j <= n; j++)
        model.add(y(0, j) == 0);

    // BC
    for (size_t i = 1; i <= m; i++)
        model.add(y(i, 2) - 4 * y(i, 1) + 3 * y(i, 0) == 0);
    for (size_t i = 1; i <= m; i++)
        model.add((y(i, n - 2) - 4 * y(i, n1) + 3 * y(i, n)) / (2 * dx) == u(i) - y(i, n));
}
#endif
