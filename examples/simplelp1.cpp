#include "coek_model.hpp"

void simplelp1_example(coek::Model& m)
{
auto x = m.getVariable(0, m.inf, "x");
auto y = m.getVariable(0, m.inf, "y");

m.add( 50*x + 40*y, coek::Model::maximize );
m.add( 2*x + 3*y <= 1500 );
m.add( 2*x + y <= 1000 );
}
