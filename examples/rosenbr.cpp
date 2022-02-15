#include <coek/coek.hpp>

//   Source:  problem 1 in
//   J.J. More', B.S. Garbow and K.E. Hillstrom,
//   "Testing Unconstrained Optimization Software",
//   ACM Transactions on Mathematical Software, vol. 7(1), pp. 17-41, 1981.


void rosenbr_example(coek::Model& m)
{
std::vector<coek::Variable> x(2);
x[0].value(1);
x[1].value(2);
m.add_variable(x[0]);
m.add_variable(x[1]);

m.add_objective( 100*pow(x[1] - pow(x[0],2), 2) + pow(x[0]-1, 2) );
}
