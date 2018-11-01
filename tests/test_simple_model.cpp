
#include "catch.hpp"
#include "admodel/simple.hpp"


TEST_CASE( "simple_admodel", "[smoke]" ) {

  Simple_ExprModel model;
  
  SECTION( "Test Objective" ) {

    auto x = model.var(0,0,0.0,0.0,1.0,"x");
    auto y = model.var(0,0,0.0,0.0,1.0,"y");

    model.add_objective( model.manager.plus(x,y) );

    Simple_ADModel admodel;

    REQUIRE( admodel.num_variables() == 0 );

    initialize_admodel(admodel, model);

    REQUIRE( admodel.num_variables() == 2 );
    }

}
