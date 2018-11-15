
#include "catch.hpp"
#include "admodel/simple.hpp"


TEST_CASE( "simple_admodel", "[smoke]" ) {

  Simple_ADModel model;
  
  SECTION( "Test Objective" ) {

    auto x = model.var(0,0,0.0,0.0,1.0,"x");
    auto y = model.var(0,0,0.0,0.0,1.0,"y");

    model.add_objective( model.manager.plus(x,y) );

    REQUIRE( model.num_variables() == 0 );

    model.build();

    REQUIRE( model.num_variables() == 2 );
    }

}
