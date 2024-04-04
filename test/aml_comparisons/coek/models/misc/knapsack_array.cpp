#include <coek/coek.hpp>
#include <functional>
#include <map>
#include <random>
#include <vector>

void knapsack_array(coek::Model& model, size_t N)
{
    N *= 1000;
    double W = N / 10.0;

    std::mt19937 rng(10000);
    std::uniform_real_distribution<double> distribution(0, 1);
    auto uniform = std::bind(distribution, rng);

    std::vector<double> v(N);
    std::vector<double> w(N);
    for (size_t n : coek::range(N)) {
        v[n] = uniform();
        w[n] = uniform();
    }

    auto x = coek::variable(N).bounds(0, 1).value(0);
    model.add(x);

    // obj
    auto obj = coek::expression();
    for (size_t n : coek::range(N))
        obj += v[n] * x(n);
    model.add_objective(obj);

    // con
    auto con = coek::expression();
    for (size_t n : coek::range(N))
        con += w[n] * x(n);
    model.add(con <= W);
}
