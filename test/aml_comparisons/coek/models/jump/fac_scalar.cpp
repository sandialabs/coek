#include <cmath>
#include <coek/coek.hpp>
#include <cstdlib>
#include <vector>

void fac_scalar(coek::Model& model, size_t F)
{
    size_t G = F;

    // Create variables
    auto d = model.add(coek::variable().bounds(0, COEK_INFINITY).value(1.0));

    std::vector<std::vector<coek::Variable>> y;
    for (size_t f = 0; f < F; f++) {
        y.push_back(std::vector<coek::Variable>(2));
        for (size_t k = 0; k < 2; k++) {
            model.add(y[f][k].bounds(0, 1).value(0));
        }
    }

    std::vector<std::vector<std::vector<coek::Variable>>> z;
    for (size_t i = 0; i <= G; i++) {
        z.push_back(std::vector<std::vector<coek::Variable>>());
        for (size_t j = 0; j <= G; j++) {
            z[i].push_back(std::vector<coek::Variable>(F));
            for (size_t f = 0; f < F; f++) {
                model.add(z[i][j][f].bounds(0, 1).value(0).within(coek::VariableTypes::Boolean));
            }
        }
    }

    std::vector<std::vector<std::vector<coek::Variable>>> s;
    for (size_t i = 0; i <= G; i++) {
        s.push_back(std::vector<std::vector<coek::Variable>>());
        for (size_t j = 0; j <= G; j++) {
            s[i].push_back(std::vector<coek::Variable>(F));
            for (size_t f = 0; f < F; f++) {
                model.add(s[i][j][f].bounds(0, COEK_INFINITY).value(0));
            }
        }
    }

    std::vector<std::vector<std::vector<std::vector<coek::Variable>>>> r;
    for (size_t i = 0; i <= G; i++) {
        r.push_back(std::vector<std::vector<std::vector<coek::Variable>>>());
        for (size_t j = 0; j <= G; j++) {
            r[i].push_back(std::vector<std::vector<coek::Variable>>());
            for (size_t f = 0; f < F; f++) {
                r[i][j].push_back(std::vector<coek::Variable>(2));
                for (size_t k = 0; k < 2; k++) {
                    model.add(r[i][j][f][k].bounds(-COEK_INFINITY, COEK_INFINITY).value(0));
                }
            }
        }
    }

    // Add objective

    model.add_objective(d);

    // Add constraints

    // Each customer is assigned to a facility
    for (size_t i = 0; i <= G; i++) {
        for (size_t j = 0; j <= G; j++) {
            coek::Expression lhs;
            for (size_t f = 0; f < F; f++) lhs += z[i][j][f];
            model.add(lhs == 1);
        }
    }

    const double M = 2 * sqrt(2.0);

    for (size_t i = 0; i <= G; i++) {
        for (size_t j = 0; j <= G; j++) {
            for (size_t f = 0; f < F; f++) {
                model.add(s[i][j][f] == d + M * (1 - z[i][j][f]));
                model.add(r[i][j][f][0] == (1.0 * i) / G - y[f][0]);
                model.add(r[i][j][f][1] == (1.0 * j) / G - y[f][1]);
                model.add(r[i][j][f][0] * r[i][j][f][0] + r[i][j][f][1] * r[i][j][f][1]
                          <= s[i][j][f] * s[i][j][f]);
            }
        }
    }
}
