#include <coek/coek.hpp>
#include <vector>

void invquad_array_resolve()
{
    // Create model
    coek::Model m;

    size_t N = 5;
    auto p = coek::parameter("p", N).value(0.5);

    // Initialize variables and add them to the model
    auto x = coek::variable("x", N).bounds(-10, 10).value(0.0);
    m.add(x);

    // Create objective and add it to the model
    auto e = coek::expression();
    for (size_t i : coek::range(N)) {
        e -= (x(i) - p(i)) * (x(i) - p(i));
    }
    m.add_objective(e);

    // Optimize the model
    coek::NLPModel nlp(m, "cppad");
    coek::NLPSolver solver("ipopt");
    solver.set_option("print_level", 0);
    solver.load(nlp);

    solver.resolve();
    // x^*_i = -10
    for (size_t i : coek::range(N)) {
        std::cout << "Value of " << x(i).name() << ": " << x(i).value() << std::endl;
    }

    p.value(-0.5);
    solver.resolve();
    // x^*_i = -10
    for (size_t i : coek::range(N)) {
        std::cout << "Value of " << x(i).name() << ": " << x(i).value() << std::endl;
    }
}
