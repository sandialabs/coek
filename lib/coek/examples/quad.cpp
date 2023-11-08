#include <coek/coek.hpp>
#include <vector>

void quad_example(coek::Model& m, std::vector<coek::Parameter>& p)
{
    std::vector<coek::Variable> x(p.size());
    for (auto it = x.begin(); it != x.end(); ++it) {
        *it = coek::variable().lower(-10).upper(10).value(0.0);
        m.add(*it);
    }

    auto e = coek::expression();
    for (size_t i = 0; i < x.size(); i++) e += (x[i] - p[i]) * (x[i] - p[i]);

    m.add_objective(e);
}
