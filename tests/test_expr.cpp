
#include "catch.hpp"
#include "expr_types.hpp"
#include "context_objects.hpp"


TEST_CASE( "expr_values", "[smoke]" ) {

  ExpressionContext_Objects context;
  
  SECTION( "Test Immutable Parameter" ) {

    // Create an immutable parameter
    TypedParameter<int> q( &context, 2, false, "q");
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
    TypedParameter<int> q( &context, 2, true, "q");
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
    Variable p( &context, false, false, 0.0, 1.0, 2.0, "p");
    REQUIRE( p._value == 2.0 );
    REQUIRE( p.value() == 2.0 );

    // Set the value to 3
    p._value = 3.0;
    REQUIRE( p._value == 3.0 );
    REQUIRE( p.value() == 3.0 );

    }

  SECTION( "Test Expression" ) {
    Variable p( &context, false, false, 0.0, 1.0, 2.0, "p");
    TypedParameter<int> q( &context, 3, true, "q");
    AddExpression<NumericValue*, NumericValue*> e(&context, &p, &q);

    REQUIRE( p.value() == 2.0 );
    REQUIRE( q.value() == 3.0 );
    REQUIRE( e.value() == 5.0 );
    }

  SECTION( "Test Constraint" ) {
    Variable p( &context, false, false, 0.0, 1.0, 2.0, "p");
    TypedParameter<int> q( &context, 3, true, "q");
    InequalityExpression e(&context, &p, false);

    REQUIRE( p.value() == 2.0 );
    REQUIRE( e.value() == 2.0 );
    }

}

