
#include "catch.hpp"
#include "admodel/cppad.hpp"
#include "model/simple.hpp"


TEST_CASE( "cppad_admodel", "[smoke]" ) {

  CppAD_ADModel model;
  
  SECTION( "Test Objective" ) {

    auto x = model.var(0,0,0.0,0.0,1.0,"x");
    auto y = model.var(0,0,0.0,0.0,1.0,"y");

    model.add_objective( model.manager.plus(x,y) );

    REQUIRE( model.num_variables() == 0 );
    }

}
