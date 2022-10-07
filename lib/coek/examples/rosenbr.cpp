#include <coek/coek.hpp>

coek::Model rosenbr()
{
    coek::Model m;

    auto x0 = m.add(coek::variable().value(1));
    auto x1 = m.add(coek::variable().value(2));

    m.add_objective(100 * pow(x1 - pow(x0, 2), 2) + pow(x0 - 1, 2));

    return m;
}

//   Source:  problem 1 in
//   J.J. More', B.S. Garbow and K.E. Hillstrom,
//   "Testing Unconstrained Optimization Software",
//   ACM Transactions on Mathematical Software, vol. 7(1), pp. 17-41, 1981.
