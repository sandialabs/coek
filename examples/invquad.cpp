#include <vector>
#include <coek/coek.hpp>

void invquad_example(coek::Model& m, std::vector<coek::Parameter>& p)
{
std::vector<coek::Variable> x(p.size());
for (auto it=x.begin(); it != x.end(); ++it) {
    *it = coek::Variable(-10, 10, 0);
    m.addVariable(*it);
    }

coek::Expression e;
for (size_t i=0; i<x.size(); i++)
    e += (x[i]-p[i])*(x[i]-p[i]);

m.add( -e );
}
