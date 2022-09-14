#include <coek/coek.hpp>

coek::Model srosenbr_array()
{
coek::Model m;

size_t N = 10000;
auto x = m.add( coek::variable("x", N) );
for (size_t i : coek::range(N)) {
    if (i%2 == 0)
        x(i).value(-1.2);
    else
        x(i).value(1);
    }

auto obj = coek::expression();
for (size_t i : coek::range(N/2))
    obj += 100*pow(x(2*i) - pow(x(2*i-1),2), 2) + pow(x(2*i-1)-1, 2);
m.add_objective( obj );

return m;
}


// Source:  problem 21 in
// J.J. More', B.S. Garbow and K.E. Hillstrom,
// "Testing Unconstrained Optimization Software",
// ACM Transactions on Mathematical Software, vol. 7(1), pp. 17-41, 1981.
