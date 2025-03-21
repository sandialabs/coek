#include <cmath>
#include <coek/coek.hpp>
#include <cstdlib>
#include <vector>

void fac_compact(coek::CompactModel& model, size_t F)
{
    size_t G = F;

    auto i = coek::set_element("i");
    auto j = coek::set_element("j");
    auto f = coek::set_element("f");

    auto Fset = coek::RangeSet(0, F - 1);
    auto ZeroOne = coek::RangeSet(0, 1);
    auto Y = Fset * ZeroOne;
    auto Gp = coek::RangeSet(0, G);
    auto GGF = Gp * Gp * Fset;

    // Create variables
    auto d = model.add(coek::variable("d").bounds(0, COEK_INFINITY).value(1.0));

    auto y = model.add(coek::variable("y", Y).bounds(0, 1).value(1.0));

    auto z = model.add(
        coek::variable("z", GGF).bounds(0, 1).value(1.0).within(coek::VariableTypes::Boolean));

    auto s = model.add(coek::variable("s", GGF).bounds(0, COEK_INFINITY).value(0));

    auto r = model.add(
        coek::variable("r", GGF * ZeroOne).bounds(-COEK_INFINITY, COEK_INFINITY).value(0));

    // Add objective

    model.add(coek::objective(d));

    // Add constraints

    // Each customer is assigned to a facility
    model.add(coek::constraint("assmt", coek::Sum(s(i, j, f), coek::Forall(f).In(Fset)) == 1,
                               coek::Forall(i).In(Gp).Forall(j).In(Gp)));

    const double M = 2 * sqrt(2.0);

    model.add(coek::constraint("quadrhs", s(i, j, f) == d + M * (1 - z(i, j, f)),
                               coek::Forall(i).In(Gp).Forall(j).In(Gp).Forall(f).In(Fset)));

    model.add(coek::constraint("quaddistk1", r(i, j, f, 0) == i / G - y(f, 0),
                               coek::Forall(i).In(Gp).Forall(j).In(Gp).Forall(f).In(Fset)));

    model.add(coek::constraint("quaddistk2", r(i, j, f, 1) == j / G - y(f, 1),
                               coek::Forall(i).In(Gp).Forall(j).In(Gp).Forall(f).In(Fset)));

    model.add(coek::constraint(
        "quaddist",
        r(i, j, f, 0) * r(i, j, f, 0) + r(i, j, f, 1) * r(i, j, f, 1) <= s(i, j, f) * s(i, j, f),
        coek::Forall(i).In(Gp).Forall(j).In(Gp).Forall(f).In(Fset)));
}
