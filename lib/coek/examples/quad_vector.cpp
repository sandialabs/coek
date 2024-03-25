#include <coek/coek.hpp>
#include <vector>

coek::Model quad_vector(std::vector<coek::Parameter>& p)
{
    // Create model
    coek::Model m;

    // Initialize variables and add them to the model
    std::vector<coek::Variable> x(p.size());
    for (auto& var : x)
        m.add( var.bounds(-10, 10).value(0.0) );

    // Create objective and add it to the model
    auto e = coek::expression();
    for (auto i : indices(x)) e += (x[i] - p[i]) * (x[i] - p[i]);
    m.add_objective(e);

    return m;
}
