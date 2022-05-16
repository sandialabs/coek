
#include <cmath>
#include <memory>
#include <sstream>
#include <iostream>
#include "catch2/catch.hpp"

#include "coek/ast/base_terms.hpp"
#include "coek/ast/value_terms.hpp"
#include "coek/coek.hpp"

const double PI = 3.141592653589793238463;
const double E = exp(1.0);


TEST_CASE( "elementary_param", "[smoke]" ) {

  SECTION( "values" ) {
      WHEN( "parameter - 3" ) {
        auto q = coek::parameter("q").value(2);
        REQUIRE( q.value() == 2 );
        q.value(3);
        REQUIRE( q.value() == 3 );
      }
  }

  SECTION( "constructors" ) {
      WHEN( "copy" ) {
        // Simple constructor
        auto q = coek::parameter().value(3);

        // Test copy constructor
        coek::Parameter Q(q);
        REQUIRE( Q.value() == 3 );
      }

      WHEN( "equal" ) {
        // Simple constructor
        auto q = coek::parameter().value(4);

        // Test copy constructor
        auto Q = coek::parameter().value(5);
        Q = q;
        REQUIRE( Q.value() == 4 );
      }
  }

  SECTION( "constructors" ) {
    auto q = coek::parameter("q").value(3);
    REQUIRE( q.name() == "q" );
  }

  SECTION( "write" ) {
    std::stringstream sstr;
    auto q = coek::parameter("q").value(3);
    sstr << q;
    REQUIRE( sstr.str() == "q" );
  }

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}


TEST_CASE( "model_indexparam", "[smoke]" ) {

  SECTION( "values" ) {
      WHEN( "IndexParameter - 3" ) {
        auto q = coek::set_element("q");
        q.value(1);
        int tmp = -1;
        auto success = q.get_value(tmp);
        REQUIRE( success == true );
        REQUIRE( tmp == 1 );
        q.value(3.5);
        success = q.get_value(tmp);
        REQUIRE( success == false );
      }
      WHEN( "IndexParameter - 3.5" ) {
        auto q = coek::set_element();
        q.value(3.5);
        double tmp = -3.5;
        auto success = q.get_value(tmp);
        REQUIRE( success == true );
        REQUIRE( tmp == 3.5 );
        q.value(3);
        success = q.get_value(tmp);
        REQUIRE( success == false );
      }
      WHEN( "IndexParameter - 'here'" ) {
        auto q = coek::set_element();
        q.value("here");
        std::string tmp = "there";
        auto success = q.get_value(tmp);
        REQUIRE( success == true );
        REQUIRE( tmp == "here" );
        q.value(3.5);
        success = q.get_value(tmp);
        REQUIRE( success == false );
      }
  }

  SECTION( "constructors" ) {
      WHEN( "copy" ) {
        // Simple constructor
        auto q = coek::set_element();
        q.value(3);

        // Test copy constructor
        coek::IndexParameter Q(q);
        int tmp=-1;
        auto success = Q.get_value(tmp);
        REQUIRE( success == true );
        REQUIRE( tmp == 3 );
      }

      WHEN( "equal" ) {
        // Simple constructor
        auto q = coek::set_element();
        q.value(4);

        // Test copy constructor
        auto Q = coek::set_element();
        Q.value(5);
        Q = q;
        int tmp=-1;
        auto success = Q.get_value(tmp);
        REQUIRE( success == true );
        REQUIRE( tmp == 4 );
      }
  }

  SECTION( "constructors" ) {
    auto q = coek::set_element("q");
    REQUIRE( q.name() == "q" );
  }

  SECTION( "write" ) {
    std::stringstream sstr;
    auto q = coek::set_element("q");
    sstr << q;
    REQUIRE( sstr.str() == "q" );
  }

  SECTION( "name" ) {
    auto p = coek::set_element();
    auto q = coek::set_element("q");
    REQUIRE( p.name() == "unknown" );
    p.name("P");
    REQUIRE( p.name() == "P" );
    REQUIRE( q.name() == "q" );
  }

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}

