
#include <cmath>
#include <memory>
#include <sstream>
#include <iostream>
#include "catch2/catch.hpp"

#include "coek/ast/base_terms.hpp"
#include "coek/coek.hpp"
#include "coek/solvers/solver_repn.hpp"


TEST_CASE( "solver_test", "[smoke]" ) {

  SECTION( "params" ) {
    coek::Model model;
    auto v = model.add_variable("v").lower(0).upper(1).within(coek::Integers);
    auto w = model.add_variable("w").lower(0).upper(1).within(coek::Integers);
    auto p = coek::parameter("p").value(2);
    auto q = coek::parameter("q").value(2);

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

    p.value(3);
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

    p.value(4);
    q.value(4);
    solver.resolve();

    REQUIRE( solver.repn->vcache.size() == 0);
    REQUIRE( solver.repn->pcache.size() == 2);
    REQUIRE( solver.repn->vupdates.size() == 0);
    REQUIRE( solver.repn->pupdates.size() == 2);
  }

  SECTION( "vars" ) {
    coek::Model model;
    auto v = model.add_variable("v").lower(0).upper(1).value(0).within(coek::Integers);
    auto w = model.add_variable("w").lower(0).upper(1).value(0).within(coek::Integers);
    w.fixed(true);
    auto p = coek::parameter("p").value(2);
    auto q = coek::parameter("q").value(2);

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

    p.value(3);
    w.value(3);
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

    p.value(4);
    w.value(5);
    solver.resolve();

    REQUIRE( solver.repn->vcache.size() == 1);
    REQUIRE( solver.repn->pcache.size() == 2);
    REQUIRE( solver.repn->vupdates.size() == 1);
    REQUIRE( solver.repn->pupdates.size() == 1);
  }

  SECTION( "solve" ) {
    coek::Model model;
    auto v = model.add_variable("v").lower(0).upper(1).within(coek::Integers);
    auto w = model.add_variable("w").lower(0).upper(1).within(coek::Integers);
    auto p = coek::parameter("p").value(2);
    auto q = coek::parameter("q").value(2);

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
