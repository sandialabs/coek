
#include "catch.hpp"
#include "admodel/cppad.hpp"
#include "model/simple.hpp"


TEST_CASE( "cppad_admodel", "[smoke]" ) {

  Simple_ExprModel model;
  
  SECTION( "Test Objective" ) {

    auto x = model.var(0,0,0.0,0.0,1.0,"x");
    auto y = model.var(0,0,0.0,0.0,1.0,"y");

    model.add_objective( model.manager.plus(x,y) );

    CppAD_ADModel admodel;

    REQUIRE( admodel.num_variables() == 0 );

    initialize_admodel(admodel, model);

    // NOTE: This is an expected failure
    REQUIRE( admodel.num_variables() == 0 );
    }

}
