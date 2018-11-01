
#include "catch.hpp"
#include "expr/expr_types.hpp"
#include "expr/expr_manager_objects.hpp"


TEST_CASE( "expr_values", "[smoke]" ) {

  ExprManager_Objects manager;
  
  SECTION( "Test Immutable Parameter" ) {

    // Create an immutable parameter
    TypedParameter<int> q( &manager, 2, false, "q");
    REQUIRE( q._value == 2 );
    REQUIRE( q._tvalue == 2 );
    REQUIRE( q.value() == 2 );

    // The immutable flag only is used in Python
    q._tvalue = 3;
    REQUIRE( q._tvalue == 3 );
    REQUIRE( q.value() == 3 );

    }

  SECTION( "Test Mutable Parameter" ) {

    // Create an mutable parameter
    TypedParameter<int> q( &manager, 2, true, "q");
    REQUIRE( q._value == 2 );
    REQUIRE( q._tvalue == 2 );
    REQUIRE( q.value() == 2 );

    // The mutable flag only is used in Python
    q._tvalue = 3;
    REQUIRE( q._tvalue == 3 );
    REQUIRE( q.value() == 3 );

    }

  SECTION( "Test Variable" ) {

    // Create a variable parameter
    Variable p( &manager, false, false, 0.0, 1.0, 2.0, "p");
    REQUIRE( p._value == 2.0 );
    REQUIRE( p.value() == 2.0 );

    // Set the value to 3
    p._value = 3.0;
    REQUIRE( p._value == 3.0 );
    REQUIRE( p.value() == 3.0 );

    }

  SECTION( "Test Expression" ) {
    Variable p( &manager, false, false, 0.0, 1.0, 2.0, "p");
    TypedParameter<int> q( &manager, 3, true, "q");
    AddExpression<NumericValue*, NumericValue*> e(&manager, &p, &q);

    REQUIRE( p.value() == 2.0 );
    REQUIRE( q.value() == 3.0 );
    REQUIRE( e.value() == 5.0 );
    }

  SECTION( "Test Constraint" ) {
    Variable p( &manager, false, false, 0.0, 1.0, 2.0, "p");
    TypedParameter<int> q( &manager, 3, true, "q");
    InequalityExpression e(&manager, &p, false);

    REQUIRE( p.value() == 2.0 );
    REQUIRE( e.value() == 2.0 );
    }

}

