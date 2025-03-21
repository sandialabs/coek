#include <coek/coek.hpp>
#include <functional>
#include <map>
#include <random>
// #include <vector>
#include <iostream>
#include <coek/util/io_utils.hpp>

void knapsack_compact(coek::CompactModel& model, size_t N)
{
    N *= 1000;
    double W = N / 10.0;

    std::mt19937 rng(10000);
    std::uniform_real_distribution<double> distribution(0, 1);
    auto uniform = std::bind(distribution, rng);

    std::map<int, double> rawv;
    std::map<int, double> raww;
    for (size_t i : coek::range(N)) {
        rawv[i] = uniform();
        raww[i] = uniform();
    }

    auto I = coek::RangeSet(0, N - 1);
    auto i = coek::set_element("i");

    auto v = model.add(coek::data(I)).value(rawv);
    auto w = model.add(coek::data(I)).value(raww);
    auto x = model.add(coek::variable(I)).bounds(0, 1).value(0);

    // obj
    model.add(coek::objective("obj", coek::Sum(v(i) * x(i), coek::Forall(i).In(I))));

    // con
    model.add(coek::constraint("con", coek::Sum(w(i) * x(i), coek::Forall(i).In(I)) <= W));
}
