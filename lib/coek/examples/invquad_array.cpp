#include <coek/coek.hpp>
#include <vector>

coek::Model invquad_array(std::vector<coek::Parameter>& p)
{
    // Create model
    coek::Model m;

    // Initialize variables and add them to the model
    auto x = coek::variable("x", p.size()).bounds(-10, 10).value(0.0);
    m.add(x);

    // Create objective and add it to the model
    auto e = coek::expression();
    for (size_t i : coek::indices(p))
        e -= (x(i) - p[i]) * (x(i) - p[i]);
    m.add_objective(e);

    return m;
}
