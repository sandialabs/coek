
#include "catch.hpp"
#include "admodel/cppad.hpp"


TEST_CASE( "cppad_admodel add", "[smoke]" ) {

  coek_finalize();
  CppAD_ADModel model;
  
  SECTION( "Add Objective" ) {
    auto x = model.var(0,0,0.0,0.0,1.0,"x");
    auto y = model.var(0,0,0.0,0.0,1.0,"y");

    model.add_objective( model.manager.plus(x,y) );

    REQUIRE( model.num_variables() == 0 );

    model.build();

    REQUIRE( model.num_variables() == 2 );
    }

  SECTION( "Add Inequality" ) {
    auto x = model.var(0,0,0.0,0.0,1.0,"x");
    auto y = model.var(0,0,0.0,0.0,1.0,"y");

    auto e = model.manager.inequality(model.manager.plus(x,y));
    model.add_inequality( e );

    REQUIRE( model.num_variables() == 0 );

    model.build();

    REQUIRE( model.num_variables() == 2 );
    }

  SECTION( "Add Equality" ) {
    auto x = model.var(0,0,0.0,0.0,1.0,"x");
    auto y = model.var(0,0,0.0,0.0,1.0,"y");

    model.add_equality( model.manager.equality(model.manager.plus(x,y)) );

    REQUIRE( model.num_variables() == 0 );

    model.build();

    REQUIRE( model.num_variables() == 2 );
    }
}


TEST_CASE( "cppad_admodel ad", "[smoke]" ) {

  coek_finalize();
  CppAD_ADModel model;
  
  SECTION( "f" ) {
    auto a = model.var(0,0,0.0,0.0,1.0,"a");
    auto b = model.var(0,0,0.0,0.0,1.0,"b");

    model.add_objective( model.manager.plus(a,b) );
    model.add_objective( model.manager.times(a,b) );

    model.build();

    std::vector<double> x{3,5};
    REQUIRE( model.compute_f(x) == 8.0 );
    REQUIRE( model.compute_f(1) == 15.0 );
    REQUIRE( model.compute_f(x,1) == 15.0 );

    std::vector<double> y{3,6};
    REQUIRE( model.compute_f(y) == 9.0 );
    REQUIRE( model.compute_f(1) == 18.0 );
    REQUIRE( model.compute_f(y,1) == 18.0 );
    }

  SECTION( "df" ) {
    auto a = model.var(0,0,0.0,0.0,1.0,"a");
    auto b = model.var(0,0,0.0,0.0,1.0,"b");

    model.add_objective( model.manager.plus(a,b) );
    model.add_objective( model.manager.times(a,b) );

    model.build();

    std::vector<double> x{3,5};
    std::vector<double> df(2);
    double f;
    REQUIRE( model.compute_f(x) == 8.0 );

    model.compute_df(x, df);
    REQUIRE( df[0] == 1.0 );
    REQUIRE( df[1] == 1.0 );
    model.compute_df(df, 1);
    REQUIRE( df[0] == 5.0 );
    REQUIRE( df[1] == 3.0 );
    model.compute_df(f, df, 1);
    REQUIRE( f == 15.0 );
     
    std::vector<double> y{3,6};
    REQUIRE( model.compute_f(y) == 9.0 );

    model.compute_df(y, df);
    REQUIRE( df[0] == 1.0 );
    REQUIRE( df[1] == 1.0 );
    model.compute_df(df, 1);
    REQUIRE( df[0] == 6.0 );
    REQUIRE( df[1] == 3.0 );
    model.compute_df(f, df, 1);
    REQUIRE( f == 18.0 );
    }

  SECTION( "c" ) {
    auto a = model.var(0,0,0.0,0.0,1.0,"a");
    auto b = model.var(0,0,0.0,0.0,1.0,"b");

    model.add_inequality( model.manager.inequality(model.manager.plus(a,b)) );

    model.add_equality( model.manager.equality(model.manager.times(a,b)) );

    model.build();

    std::vector<double> x{3,5};
    std::vector<double> c(2);
    model.compute_c(x, c);
    REQUIRE( c[0] == 8.0 );
    REQUIRE( c[1] == 15.0 );
    model.compute_c(c);
    REQUIRE( c[0] == 8.0 );
    REQUIRE( c[1] == 15.0 );

    std::vector<double> y{3,6};
    model.compute_c(y, c);
    REQUIRE( c[0] == 9.0 );
    REQUIRE( c[1] == 18.0 );
    model.compute_c(c);
    REQUIRE( c[0] == 9.0 );
    REQUIRE( c[1] == 18.0 );
    }

  SECTION( "dc" ) {
    auto a = model.var(0,0,0.0,0.0,1.0,"a");
    auto b = model.var(0,0,0.0,0.0,1.0,"b");

    model.add_inequality( model.manager.inequality(model.manager.plus(a,b)) );

    model.add_equality( model.manager.equality(model.manager.times(a,b)) );

    model.build();

    std::vector<double> x{3,5};
    std::vector<double> dc(2);
    model.compute_dc(x, dc, 0);
    REQUIRE( dc[0] == 1.0 );
    REQUIRE( dc[1] == 1.0 );
    model.compute_dc(dc, 1);
    REQUIRE( dc[0] == 5.0 );
    REQUIRE( dc[1] == 3.0 );
    model.compute_dc(x, dc, 1);
    REQUIRE( dc[0] == 5.0 );
    REQUIRE( dc[1] == 3.0 );

    std::vector<double> y{3,6};
    model.compute_dc(y, dc, 0);
    REQUIRE( dc[0] == 1.0 );
    REQUIRE( dc[1] == 1.0 );
    model.compute_dc(dc, 1);
    REQUIRE( dc[0] == 6.0 );
    REQUIRE( dc[1] == 3.0 );
    model.compute_dc(y, dc, 1);
    REQUIRE( dc[0] == 6.0 );
    REQUIRE( dc[1] == 3.0 );
    }

  coek_finalize();
}

