#include <coek/coek.hpp>

// Source:  problem 21 in
// J.J. More', B.S. Garbow and K.E. Hillstrom,
// "Testing Unconstrained Optimization Software",
// ACM Transactions on Mathematical Software, vol. 7(1), pp. 17-41, 1981.


void srosenbr_example(coek::Model& m)
{
size_t N = 10000;
std::vector<coek::Variable> x(N);
for (size_t i=0; i<N; i++) {
    if (i%2 == 0)
        x[i].set_value(-1.2);
    else
        x[i].set_value(1);
    m.add_variable(x[i]);
    }

coek::Expression obj;
for (size_t i=0; i<N/2; i++)
    obj += 100*pow(x[2*i] - pow(x[2*i-1],2), 2) + pow(x[2*i-1]-1, 2);
m.add_objective( obj );
}

