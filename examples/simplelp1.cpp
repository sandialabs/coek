#include <coek/coek.hpp>

void simplelp1_example(coek::Model& m)
{
auto x = m.add_variable("x").bounds(0,m.inf);
auto y = m.add_variable("y").bounds(0,m.inf);

m.add_objective( 50*x + 40*y).sense( coek::Model::maximize );
m.add_constraint( 2*x + 3*y <= 1500 );
m.add_constraint( 2*x + y <= 1000 );
}
