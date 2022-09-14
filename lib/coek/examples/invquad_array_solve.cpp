#include <vector>
#include <coek/coek.hpp>

void invquad_array_solve()
{
// Create model
coek::Model m;

size_t N=10;
std::vector<coek::Parameter> p(10);
for (auto& param : p)
    param.value(0.5);

// Initialize variables and add them to the model
auto x = coek::variable("x", N)
                    .bounds(-10,10).value(0.0);
m.add(x);

// Create objective and add it to the model
auto e = coek::expression();
for (size_t i : coek::range(p.size()))
    e -= (x(i)-p[i])*(x(i)-p[i]);
m.add_objective( e );

// Optimize the model
coek::NLPModel nlp(m, "cppad");
coek::NLPSolver solver("ipopt");
solver.set_option("print_level", 0);
solver.solve(nlp);

// x^*_i = -10
for (size_t i=0; i<N; i++)
    std::cout << "Value of " << x(i).name() << ": " << x(i).value() << std::endl;
}
