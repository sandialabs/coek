#include <coek/coek.hpp>
#include <map>

#ifdef COEK_WITH_COMPACT_MODEL
void pmedian_compact(coek::CompactModel& model, size_t N, size_t P)
{
    size_t M = N;  // Customers

    std::map<std::tuple<size_t, size_t>, double> d_raw;
    for (size_t n : coek::sequence<size_t>(1, N))
        for (size_t m : coek::sequence<size_t>(1, M))
            d_raw[{n, m}] = 1.0 + 1.0 / (n + m + 1);

    auto Locations = coek::RangeSet(1, N);
    auto Customers = coek::RangeSet(1, M);
    auto m = coek::set_element("m");
    auto n = coek::set_element("n");

    auto d = model.add(coek::data("d", Locations * Customers)).value(d_raw);

    auto x = model.add(coek::variable("x", Locations * Customers)).bounds(0, 1).value(0);
    auto y = model.add(coek::variable("y", Locations)).bounds(0, 1).value(0);

    // obj
    model.add(coek::objective(
        "obj",
        coek::Sum(d(n, m) * x(n, m), coek::Forall(n).In(Locations).Forall(m).In(Customers))));

    model.add(coek::constraint("single_x", coek::Sum(x(n, m), coek::Forall(n).In(Locations)) == 1,
                               coek::Forall(m).In(Customers)));

    model.add(coek::constraint("bound_y", x(n, m) <= y(n),
                               coek::Forall(n).In(Locations).Forall(m).In(Customers)));

    model.add(
        coek::constraint("num_facilities", coek::Sum(y(n), coek::Forall(n).In(Locations)) == P));
}
#endif
