#include <cmath>
#include <coek/coek.hpp>
#include <cstdlib>
#include <vector>

void fac_array(coek::Model& model, size_t F)
{
    size_t G = F;

    // Create variables
    auto d = model.add(coek::variable("d")).bounds(0, COEK_INFINITY).value(1.0);

    auto y = model.add(coek::variable("y", {F, 2})).bounds(0, 1).value(1.0);

    auto z = model.add(coek::variable("z", {G + 1, G + 1, F}))
                 .bounds(0, 1)
                 .value(1.0)
                 .within(coek::VariableTypes::Boolean);

    auto s = model.add(coek::variable("s", {G + 1, G + 1, F})).bounds(0, COEK_INFINITY).value(0);

    auto r = model.add(coek::variable("r", {G + 1, G + 1, F, 2}))
                 .bounds(-COEK_INFINITY, COEK_INFINITY)
                 .value(0);

    // Add objective

    model.add_objective(d);

    // Add constraints

    // Each customer is assigned to a facility
    // for (size_t i = 0; i <= G; i++) {
    //    for (size_t j = 0; j <= G; j++) {
    for (size_t i : coek::range<size_t>(G + 1)) {
        for (size_t j : coek::range<size_t>(G + 1)) {
            auto lhs = coek::expression();
            // for (size_t f = 0; f < F; f++)
            for (size_t f : coek::range<size_t>(F)) lhs += z(i, j, f);
            model.add(lhs == 1);
        }
    }

    const double M = 2 * sqrt(2.0);

    // for (size_t i = 0; i <= G; i++) {
    //     for (size_t j = 0; j <= G; j++) {
    //         for (size_t f = 0; f < F; f++) {
    for (size_t i : coek::range<size_t>(G + 1)) {
        for (size_t j : coek::range<size_t>(G + 1)) {
            for (size_t f : coek::range<size_t>(F)) {
                model.add(s(i, j, f) == d + M * (1 - z(i, j, f)));
                model.add(r(i, j, f, 0) == (1.0 * i) / G - y(f, 0));
                model.add(r(i, j, f, 1) == (1.0 * j) / G - y(f, 1));
                model.add(r(i, j, f, 0) * r(i, j, f, 0) + r(i, j, f, 1) * r(i, j, f, 1)
                          <= s(i, j, f) * s(i, j, f));
            }
        }
    }
}
