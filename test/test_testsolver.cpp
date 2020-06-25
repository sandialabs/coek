
#include <cmath>
#include <memory>
#include <sstream>
#include <iostream>
#include "catch.hpp"

#include "coek/expr/ast_term.hpp"
#include "coek/coek.hpp"
#include "coek/solvers/solver.hpp"


TEST_CASE( "solver_test", "[smoke]" ) {

  SECTION( "params" ) {
    coek::Model model;
    coek::Variable v = model.add_variable(0.0, 1.0, 0.0, false, true, "v");
    coek::Variable w = model.add_variable(0.0, 1.0, 0.0, false, true, "w");
    coek::Parameter p(2, "p");
    coek::Parameter q(2, "q");

    model.add_objective( 2*v + 3*w );
    model.add_constraint( 4*v + 5*w <= q );
    model.add_constraint( p*v + 5*w <= 1 );
    model.add_constraint( p*v + q*w <= 0 );

    coek::Solver solver("test");
    solver.load(model);

    REQUIRE( solver.repn->vcache.size() == 0);
    REQUIRE( solver.repn->pcache.size() == 0);
    REQUIRE( solver.repn->vupdates.size() == 0);
    REQUIRE( solver.repn->pupdates.size() == 0);

    solver.resolve();

    REQUIRE( solver.repn->vcache.size() == 0);
    REQUIRE( solver.repn->pcache.size() == 2);
    REQUIRE( solver.repn->vupdates.size() == 0);
    REQUIRE( solver.repn->pupdates.size() == 0);

    p.set_value(3);
    solver.resolve();

    REQUIRE( solver.repn->vcache.size() == 0);
    REQUIRE( solver.repn->pcache.size() == 2);
    REQUIRE( solver.repn->vupdates.size() == 0);
    REQUIRE( solver.repn->pupdates.size() == 1);

    solver.resolve();

    REQUIRE( solver.repn->vcache.size() == 0);
    REQUIRE( solver.repn->pcache.size() == 2);
    REQUIRE( solver.repn->vupdates.size() == 0);
    REQUIRE( solver.repn->pupdates.size() == 0);

    p.set_value(4);
    q.set_value(4);
    solver.resolve();

    REQUIRE( solver.repn->vcache.size() == 0);
    REQUIRE( solver.repn->pcache.size() == 2);
    REQUIRE( solver.repn->vupdates.size() == 0);
    REQUIRE( solver.repn->pupdates.size() == 2);
  }

  SECTION( "vars" ) {
    coek::Model model;
    coek::Variable v = model.add_variable(0.0, 1.0, 0.0, false, true, "v");
    coek::Variable w = model.add_variable(0.0, 1.0, 0.0, false, true, "w");
    w.set_fixed(true);
    coek::Parameter p(2, "p");
    coek::Parameter q(2, "q");

    model.add_objective( 2*v + 3*w );
    model.add_constraint( 4*v + 5*w <= q );
    model.add_constraint( p*v + 5*w <= 1 );
    model.add_constraint( p*v + q*w <= 0 );

    coek::Solver solver;
    solver.initialize("test");
    solver.load(model);

    REQUIRE( solver.repn->vcache.size() == 0);
    REQUIRE( solver.repn->pcache.size() == 0);
    REQUIRE( solver.repn->vupdates.size() == 0);
    REQUIRE( solver.repn->pupdates.size() == 0);

    solver.resolve();

    REQUIRE( solver.repn->vcache.size() == 1);
    REQUIRE( solver.repn->pcache.size() == 2);
    REQUIRE( solver.repn->vupdates.size() == 0);
    REQUIRE( solver.repn->pupdates.size() == 0);

    p.set_value(3);
    w.set_value(3);
    solver.resolve();

    REQUIRE( solver.repn->vcache.size() == 1);
    REQUIRE( solver.repn->pcache.size() == 2);
    REQUIRE( solver.repn->vupdates.size() == 1);
    REQUIRE( solver.repn->pupdates.size() == 1);

    solver.resolve();

    REQUIRE( solver.repn->vcache.size() == 1);
    REQUIRE( solver.repn->pcache.size() == 2);
    REQUIRE( solver.repn->vupdates.size() == 0);
    REQUIRE( solver.repn->pupdates.size() == 0);

    p.set_value(4);
    w.set_value(5);
    solver.resolve();

    REQUIRE( solver.repn->vcache.size() == 1);
    REQUIRE( solver.repn->pcache.size() == 2);
    REQUIRE( solver.repn->vupdates.size() == 1);
    REQUIRE( solver.repn->pupdates.size() == 1);
  }

  SECTION( "solve" ) {
    coek::Model model;
    coek::Variable v = model.add_variable(0.0, 1.0, 0.0, false, true, "v");
    coek::Variable w = model.add_variable(0.0, 1.0, 0.0, false, true, "w");
    coek::Parameter p(2, "p");
    coek::Parameter q(2, "q");

    model.add_objective( 2*v + 3*w );
    model.add_constraint( 4*v + 5*w <= q );
    model.add_constraint( p*v + 5*w <= 1 );
    model.add_constraint( p*v + q*w <= 0 );

    coek::Solver solver("test");
    solver.solve(model);
  }

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}
