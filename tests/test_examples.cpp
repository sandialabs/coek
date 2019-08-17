
#include <iostream>
#include "catch.hpp"
#include "expr/ast_term.hpp"
#include "coek_model.hpp"

//
// EXAMPLES
//
void rosenbr_example(coek::Model& model);
std::vector<double> rosenbr_soln {1,1};

void simplelp1_example(coek::Model& model);
std::vector<double> simplelp1_soln {375,250};


void check(std::vector<coek::Variable>& variables, std::vector<double>& soln)
{
for (size_t i=0; i<variables.size(); i++)
    REQUIRE(variables[i].get_value() == Approx(soln[i]));
}


#ifdef WITH_IPOPT
TEST_CASE( "ipopt", "[smoke]" ) {

  SECTION( "rosenbr" ) {
    coek::Model m;
    rosenbr_example(m);

    coek::NLPModel nlp(m, "cppad");
    coek::NLPSolver solver("ipopt");

    solver.set_option("print_level", 0);
    solver.solve(nlp);

    check(m.variables, rosenbr_soln);
  }

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}
#endif


#ifdef WITH_GUROBI
TEST_CASE( "gurobi", "[smoke]" ) {

  SECTION( "simplelp1" ) {
    coek::Model m;
    simplelp1_example(m);

    coek::Solver solver("gurobi");

    solver.set_option("OutputFlag", 0);
    solver.solve(m);

    check(m.variables, simplelp1_soln);
  }

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}
#endif
