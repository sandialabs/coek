#include <iostream>
#include "catch2/catch.hpp"
#include "coek/coek.hpp"
#include "coek/ast/base_terms.hpp"

const double PI = 3.141592653589793238463;
const double E = exp(1.0);


TEST_CASE( "cppad_add", "[smoke]" ) {

  SECTION( "error1" ) {
    auto v = coek::variable("v");
    coek::Model model;
    model.add_objective(v);

    coek::NLPModel nlp;
    REQUIRE_THROWS_WITH(nlp.initialize(model, "cppad"),
            "Model expressions contain variable 'v' that is not declared in the model.");
    }

  SECTION( "error2" ) {
    auto v = coek::variable("v");
    coek::Model model;
    model.add_objective(2*v);

    coek::NLPModel nlp;
    REQUIRE_THROWS_WITH(nlp.initialize(model, "cppad"),
            "Model expressions contain variable 'v' that is not declared in the model.");
    }

  SECTION( "error3" ) {
    coek::Model model;
    coek::NLPModel nlp;
    REQUIRE_THROWS_WITH(nlp.initialize(model, "bad"),
            "Unexpected NLP model type: bad");
    }

  SECTION( "Variables" ) {
    coek::Model model;
    auto x = model.add_variable("x").lower(0).upper(1).value(0);
    auto y = model.add_variable("y").lower(0).upper(1).value(0);
    model.add_objective("o", x + y );
    coek::NLPModel m;
    m.initialize(model, "cppad");

    WHEN ( "Set vector of values" ) {
        REQUIRE( m.compute_f() == 0.0 );

        std::vector<double> tmp = {1.0, 2.0};
        m.set_variable_view(tmp);

        REQUIRE( m.compute_f() == 3.0 );
        }
    }

  SECTION( "Add Objective" ) {
    coek::Model model;
    auto x = model.add_variable("x").lower(0).upper(1).value(0);
    auto y = model.add_variable("y").lower(0).upper(1).value(0);

    model.add_objective("o", x + y );

    REQUIRE( model.num_variables() == 2 );

    coek::NLPModel m;

    REQUIRE_THROWS_WITH(m.num_variables(), "Error accessing uninitialized NLPModel");
    REQUIRE_THROWS_WITH(m.num_objectives(), "Error accessing uninitialized NLPModel");
    REQUIRE_THROWS_WITH(m.num_constraints(), "Error accessing uninitialized NLPModel");

    m.initialize(model, "cppad");
    REQUIRE( m.num_variables() == 2 );
    REQUIRE( m.num_objectives() == 1 );
    REQUIRE( m.num_constraints() == 0 );

    auto o = m.get_objective(0);
    REQUIRE( o.get_name() == "o" );

    REQUIRE_THROWS(m.get_objective(1), "");
    }

  SECTION( "Add Inequality" ) {
    coek::Model model;
    auto x = model.add_variable("x").lower(0).upper(1).value(0);
    auto y = model.add_variable("y").lower(0).upper(1).value(0);

    auto e = x + y <= 0;
    model.add_constraint("c", e);

    coek::NLPModel m;
    m.initialize(model, "cppad");
    REQUIRE( m.num_variables() == 2 );
    REQUIRE( m.num_objectives() == 0 );
    REQUIRE( m.num_constraints() == 1 );

    auto c = m.get_constraint(0);
    REQUIRE( c.get_name() == "c" );

    REQUIRE_THROWS(m.get_constraint(1), "");
    }

  SECTION( "Add Equality" ) {
    coek::Model model;
    auto x = model.add_variable("x").lower(0).upper(1).value(0);
    auto y = model.add_variable("y").lower(0).upper(1).value(0);

    auto e = x + y == 0;
    model.add_constraint(e);

    coek::NLPModel m;
    m.initialize(model, "cppad");
    REQUIRE( m.num_variables() == 2 );
    REQUIRE( m.num_objectives() == 0 );
    REQUIRE( m.num_constraints() == 1 );
    }
}


TEST_CASE( "cppad_ad", "[smoke]" ) {
  
  SECTION( "f" ) {
    coek::Model model;
    auto a = model.add_variable("a").lower(0).upper(1).value(0);
    auto b = model.add_variable("b").lower(0).upper(1).value(0);

    model.add_objective(a + b);
    model.add_objective(a * b);

    coek::NLPModel nlp(model, "cppad");

    std::vector<double> x{3,5};
    REQUIRE( nlp.compute_f(x) == 8.0 );
    REQUIRE( nlp.compute_f(1) == 15.0 );
    REQUIRE( nlp.compute_f(x,1) == 15.0 );

    std::vector<double> y{3,6};
    REQUIRE( nlp.compute_f(y) == 9.0 );
    REQUIRE( nlp.compute_f(1) == 18.0 );
    REQUIRE( nlp.compute_f(y,1) == 18.0 );
    }

  SECTION( "df" ) {
    coek::Model model;
    auto a = model.add_variable("a").lower(0).upper(1).value(0);
    auto b = model.add_variable("b").lower(0).upper(1).value(0);

    model.add_objective( a + b );
    model.add_objective( a * b );

    coek::NLPModel nlp(model, "cppad");

    std::vector<double> x{3,5};
    std::vector<double> df(2);
    double f;
    REQUIRE( nlp.compute_f(x) == 8.0 );

    nlp.compute_df(x, df);
    REQUIRE( df[0] == 1.0 );
    REQUIRE( df[1] == 1.0 );
    nlp.compute_df(df, 1);
    REQUIRE( df[0] == 5.0 );
    REQUIRE( df[1] == 3.0 );
    nlp.compute_df(f, df, 1);
    REQUIRE( f == 15.0 );
     
    std::vector<double> y{3,6};
    REQUIRE( nlp.compute_f(y) == 9.0 );

    nlp.compute_df(y, df);
    REQUIRE( df[0] == 1.0 );
    REQUIRE( df[1] == 1.0 );
    nlp.compute_df(df, 1);
    REQUIRE( df[0] == 6.0 );
    REQUIRE( df[1] == 3.0 );
    nlp.compute_df(f, df, 1);
    REQUIRE( f == 18.0 );
    }

  SECTION( "c" ) {
    coek::Model model;
    auto a = model.add_variable("a");
    auto b = model.add_variable("b");

    model.add_constraint( a + b <= 0 );
    model.add_constraint( a * b == 0 );

    coek::NLPModel nlp(model, "cppad");

    std::vector<double> x{3,5};
    std::vector<double> c(2);
    nlp.compute_c(x, c);
    REQUIRE( c[0] == 8.0 );
    REQUIRE( c[1] == 15.0 );
    nlp.compute_c(c);
    REQUIRE( c[0] == 8.0 );
    REQUIRE( c[1] == 15.0 );

    std::vector<double> y{3,6};
    nlp.compute_c(y, c);
    REQUIRE( c[0] == 9.0 );
    REQUIRE( c[1] == 18.0 );
    nlp.compute_c(c);
    REQUIRE( c[0] == 9.0 );
    REQUIRE( c[1] == 18.0 );
    }

  SECTION( "dc" ) {
    coek::Model model;
    auto a = model.add_variable("a");
    auto b = model.add_variable("b");

    model.add_constraint( a + b <= 0 );
    model.add_constraint( a * b == 0 );

    coek::NLPModel nlp(model, "cppad");

    std::vector<double> x{3,5};
    std::vector<double> dc(2);
    nlp.compute_dc(x, dc, 0);
    REQUIRE( dc[0] == 1.0 );
    REQUIRE( dc[1] == 1.0 );
    nlp.compute_dc(dc, 1);
    REQUIRE( dc[0] == 5.0 );
    REQUIRE( dc[1] == 3.0 );
    nlp.compute_dc(x, dc, 1);
    REQUIRE( dc[0] == 5.0 );
    REQUIRE( dc[1] == 3.0 );

    std::vector<double> y{3,6};
    nlp.compute_dc(y, dc, 0);
    REQUIRE( dc[0] == 1.0 );
    REQUIRE( dc[1] == 1.0 );
    nlp.compute_dc(dc, 1);
    REQUIRE( dc[0] == 6.0 );
    REQUIRE( dc[1] == 3.0 );
    nlp.compute_dc(y, dc, 1);
    REQUIRE( dc[0] == 6.0 );
    REQUIRE( dc[1] == 3.0 );
    }


  SECTION( "sparse_j" ) {

    WHEN ( "nx < nc" ) {
        coek::Model model;
        auto a = model.add_variable("a");
        auto b = model.add_variable("b");

        model.add_objective( a );
        model.add_constraint( a <= 0 );
        model.add_constraint( a * b <= 0 );
        model.add_constraint( b <= 0 );

        coek::NLPModel nlp(model, "cppad");
        REQUIRE( nlp.num_nonzeros_Jacobian() == 4 );

        std::vector<double> x{0,1};
        std::vector<double> j(nlp.num_nonzeros_Jacobian());
        nlp.compute_J(x, j);
        REQUIRE( j[0] == 1 );
        REQUIRE( j[1] == 1 );
        REQUIRE( j[2] == 0 );
        REQUIRE( j[3] == 1 );
        }

    WHEN ( "nx > nc" ) {
        coek::Model model;
        auto a = model.add_variable("a");
        auto b = model.add_variable("b");
        auto c = model.add_variable("c");
        auto d = model.add_variable("d");

        model.add_objective( d );
        model.add_constraint( a + a*b + b <= 0 );
        model.add_constraint( b + b*c + c <= 0 );

        coek::NLPModel nlp(model, "cppad");
        REQUIRE( nlp.num_nonzeros_Jacobian() == 4 );

        std::vector<double> x{0,1,2,3};
        std::vector<double> j(nlp.num_nonzeros_Jacobian());
        nlp.compute_J(x, j);
        REQUIRE( j[0] == 2 );
        REQUIRE( j[1] == 1 );
        REQUIRE( j[2] == 3 );
        REQUIRE( j[3] == 2 );
        }
    }

  SECTION( "dense_j" ) {

    WHEN ( "nx < nc" ) {
        coek::Model model;
        auto a = model.add_variable("a");
        auto b = model.add_variable("b");

        model.add_objective( a );
        model.add_constraint( a <= 0 );
        model.add_constraint( a * b <= 0 );
        model.add_constraint( b <= 0 );

        coek::NLPModel nlp(model, "cppad", false);
        REQUIRE( nlp.num_nonzeros_Jacobian() == 6 );

        std::vector<double> x{0,1};
        std::vector<double> j(nlp.num_nonzeros_Jacobian());
        nlp.compute_J(x, j);
        REQUIRE( j[0] == 1 );
        REQUIRE( j[1] == 0 );
        REQUIRE( j[2] == 1 );
        REQUIRE( j[3] == 0 );
        REQUIRE( j[4] == 0 );
        REQUIRE( j[5] == 1 );
        }

    WHEN ( "nx > nc" ) {
        coek::Model model;
        auto a = model.add_variable("a");
        auto b = model.add_variable("b");
        auto c = model.add_variable("c");

        model.add_objective( a );
        model.add_constraint( a + a*b + b <= 0 );
        model.add_constraint( b + b*c + c <= 0 );

        coek::NLPModel nlp(model, "cppad", false);
        REQUIRE( nlp.num_nonzeros_Jacobian() == 6 );

        std::vector<double> x{0,1,2};
        std::vector<double> j(nlp.num_nonzeros_Jacobian());
        nlp.compute_J(x, j);
        REQUIRE( j[0] == 2 );
        REQUIRE( j[1] == 1 );
        REQUIRE( j[2] == 0 );
        REQUIRE( j[3] == 0 );
        REQUIRE( j[4] == 3 );
        REQUIRE( j[5] == 2 );
        }
    }

  SECTION( "sparse_h" ) {

    WHEN ( "nx < nc" ) {
        coek::Model model;
        auto a = model.add_variable("a");
        auto b = model.add_variable("b");

        model.add_objective( a*a + b );
        model.add_constraint( a <= 0 );
        model.add_constraint( a * b <= 0 );
        model.add_constraint( b <= 0 );

        coek::NLPModel nlp(model, "cppad");
        REQUIRE( nlp.num_nonzeros_Hessian_Lagrangian() == 2 );

        // H = [ [ 2, 1 ]
        //       [ 1, 0 ] ]
        std::vector<double> w{1,1,1,1};
        std::vector<double> x{0,1};
        std::vector<double> h(nlp.num_nonzeros_Hessian_Lagrangian());
        nlp.compute_H(x, w, h);
        REQUIRE( h[0] == 2 );
        REQUIRE( h[1] == 1 );
        }

    WHEN ( "nx > nc" ) {
        coek::Model model;
        auto a = model.add_variable("a");
        auto b = model.add_variable("b");
        auto c = model.add_variable("c");
        auto d = model.add_variable("d");

        model.add_objective( d*d*c );
        model.add_constraint( a + a*b + b <= 0 );
        model.add_constraint( b + b*c + c <= 0 );

        coek::NLPModel nlp(model, "cppad");
        REQUIRE( nlp.num_nonzeros_Jacobian() == 4 );

        // H = [ [ 0, 1, 0, 0 ]
        //       [ 1, 0, 1, 0 ]
        //       [ 0, 1, 0, 2d ]
        //       [ 0, 0, 2d, 2c ] ]
        std::vector<double> w{1,1,1};
        std::vector<double> x{0,1,2,3};
        std::vector<double> h(nlp.num_nonzeros_Hessian_Lagrangian());
        nlp.compute_H(x, w, h);
        REQUIRE( h[0] == 1 );
        REQUIRE( h[1] == 1 );
        REQUIRE( h[2] == 6 );
        REQUIRE( h[3] == 4 );
        }

    WHEN ( "nx > nc weighted" ) {
        coek::Model model;
        auto a = model.add_variable("a");
        auto b = model.add_variable("b");
        auto c = model.add_variable("c");
        auto d = model.add_variable("d");

        model.add_objective( d*d*c );
        model.add_constraint( a + a*b + b <= 0 );
        model.add_constraint( b + b*c + c <= 0 );

        coek::NLPModel nlp(model, "cppad");
        REQUIRE( nlp.num_nonzeros_Jacobian() == 4 );

        // H = [ [ 0, 2, 0, 0 ]
        //       [ 2, 0, 3, 0 ]
        //       [ 0, 3, 0, 2d ]
        //       [ 0, 0, 2d, 2c ] ]
        std::vector<double> w{1,2,3};
        std::vector<double> x{0,1,2,3};
        std::vector<double> h(nlp.num_nonzeros_Hessian_Lagrangian());
        nlp.compute_H(x, w, h);
        REQUIRE( h[0] == 2 );
        REQUIRE( h[1] == 3 );
        REQUIRE( h[2] == 6 );
        REQUIRE( h[3] == 4 );
        }

    WHEN ( "other 1" ) {
        coek::Model model;
        auto a = model.add_variable("a").lower(0.1).upper(100).value(1);
        auto b = model.add_variable("b").lower(0.1).upper(100).value(2);
        model.add_objective( pow(b - pow(a,2),2) + pow(a-1,2) );

        coek::NLPModel nlp(model, "cppad");

        // H = [ [ -4b+12a^2+2, -4a]
        //       [ -4a, 2 ] ]
        std::vector<double> h(nlp.num_nonzeros_Hessian_Lagrangian());
        std::vector<double> w{1};
        std::vector<double> x{1,2};
        nlp.compute_H(x, w, h);
        REQUIRE( h[0] == 6 );
        REQUIRE( h[1] == -4 );
        REQUIRE( h[2] == 2 );
        }
    }

  SECTION( "dense_h" ) {

    WHEN ( "nx < nc" ) {
        coek::Model model;
        auto a = model.add_variable("a");
        auto b = model.add_variable("b");

        model.add_objective( a*a + b );
        model.add_constraint( a <= 0 );
        model.add_constraint( a * b <= 0 );
        model.add_constraint( b <= 0 );

        coek::NLPModel nlp(model, "cppad", false);
        REQUIRE( nlp.num_nonzeros_Hessian_Lagrangian() == 3 );

        // H = [ [ 2, 1 ]
        //       [ 1, 0 ] ]
        std::vector<double> w{1,1,1,1};
        std::vector<double> x{0,1};
        std::vector<double> h(nlp.num_nonzeros_Hessian_Lagrangian());
        nlp.compute_H(x, w, h);
        REQUIRE( h[0] == 2 );
        REQUIRE( h[1] == 1 );
        REQUIRE( h[2] == 0 );
        }

    WHEN ( "nx > nc" ) {
        coek::Model model;
        auto a = model.add_variable("a");
        auto b = model.add_variable("b");
        auto c = model.add_variable("c");
        auto d = model.add_variable("d");

        model.add_objective( d*d*c );
        model.add_constraint( a + a*b + b <= 0 );
        model.add_constraint( b + b*c + c <= 0 );

        coek::NLPModel nlp(model, "cppad", false);
        REQUIRE( nlp.num_nonzeros_Hessian_Lagrangian() == 10 );

        // H = [ [ 0, 1, 0, 0 ]
        //       [ 1, 0, 1, 0 ]
        //       [ 0, 1, 0, 2d ]
        //       [ 0, 0, 2d, 2c ] ]
        std::vector<double> w{1,1,1};
        std::vector<double> x{0,1,2,3};
        std::vector<double> h(nlp.num_nonzeros_Hessian_Lagrangian());
        nlp.compute_H(x, w, h);
        REQUIRE( h[0] == 0 );
        REQUIRE( h[1] == 1 );
        REQUIRE( h[2] == 0 );
        REQUIRE( h[3] == 0 );
        REQUIRE( h[4] == 1 );
        REQUIRE( h[5] == 0 );
        REQUIRE( h[6] == 0 );
        REQUIRE( h[7] == 0 );
        REQUIRE( h[8] == 6 );
        REQUIRE( h[9] == 4 );
        }
    }

}


TEST_CASE( "diff_tests", "[smoke]" ) {

  // TODO - test constant expression

  SECTION( "constant" ) {
    coek::Model model;
    coek::Expression f(3);
    auto v = model.add_variable("v");
    model.add_objective(f*v);
    coek::NLPModel nlp(model, "cppad");
    
    std::vector<double> x{0};
    std::vector<double> baseline{3};
    std::vector<double> ans(1);
    nlp.compute_df(x, ans);
    REQUIRE( ans == baseline );
  }

  SECTION( "param" ) {
    WHEN( "simple" ) {
        coek::Model model;
        auto p = coek::parameter().value(3);
        coek::Expression f = p;
        auto v = model.add_variable("v");
        model.add_objective(f*v);
        coek::NLPModel nlp(model, "cppad");

        std::vector<double> x{0};
        std::vector<double> baseline{3};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans == baseline );

        p.value(4);
        nlp.reset();
        std::vector<double> baseline2{4};
        nlp.compute_df(x, ans);
        REQUIRE( ans == baseline2 );
    }
    WHEN( "many" ) {
        coek::Model model;
        auto p1 = coek::parameter().value(1);
        auto p2 = coek::parameter().value(2);
        auto p3 = coek::parameter().value(3);
        auto p4 = coek::parameter().value(4);
        auto p5 = coek::parameter().value(5);
        coek::Expression f = p1+2*p2+3*p3+4*p4+5*p5;
        auto v = model.add_variable("v");
        model.add_objective(f*v);
        coek::NLPModel nlp(model, "cppad");

        std::vector<double> x{0};
        std::vector<double> baseline{1+4+9+16+25};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans == baseline );

        p5.value(10);
        nlp.reset();
        std::vector<double> baseline2{1+4+9+16+50};
        nlp.compute_df(x, ans);
        REQUIRE( ans == baseline2 );
    }
  }

  SECTION( "var" ) {
    WHEN( "fixed" ) {
        coek::Model model;
        auto v = model.add_variable("v").value(0);
        auto w = model.add_variable("w").value(0);
        v.fixed(true);
        coek::Expression f = v + 2*w;
        model.add_objective( f );
        coek::NLPModel nlp(model, "cppad");

        std::vector<double> x{0};
        std::vector<double> baseline{2};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans == baseline );
    }

    WHEN( "unfixed" ) {
        coek::Model model;
        auto v = model.add_variable("v");
        auto w = model.add_variable("w");
        coek::Expression f = v;
        model.add_objective( f );
        model.add_objective( w );
        coek::NLPModel nlp(model, "cppad");

        std::vector<double> x{0,0};
        std::vector<double> baseline{1,0};
        std::vector<double> ans(2);
        nlp.compute_df(x, ans, 0);
        REQUIRE( ans == baseline );
        nlp.compute_df(x, ans, 1);
        std::vector<double> baseline2{0,1};
        REQUIRE( ans == baseline2 );
    }
  }

  SECTION( "monomial" ) {
    WHEN( "other" ) {
        coek::Model m;
        auto v = m.add_variable("v");
        auto w = m.add_variable("w");
        coek::Expression f = 2*v;
        m.add_objective( f );
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{0};
        std::vector<double> baseline{2};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans == baseline );
    }

    WHEN( "fixed" ) {
        coek::Model m;
        auto v = m.add_variable("v").value(0);
        auto w = m.add_variable("w").value(0);
        v.fixed(true);
        coek::Expression f = 2*v;
        m.add_objective(f + 3*w);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{0};
        std::vector<double> baseline{3};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans == baseline );
    }
  }

  SECTION( "plus" ) {
    WHEN( "linear" ) {
        coek::Model m;
        auto p = coek::parameter();
        auto v = m.add_variable("v");
        coek::Expression f = 2*(v+v)+v;
        m.add_objective(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{0};
        std::vector<double> baseline{5};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans == baseline );
    }
    WHEN( "simple" ) {
        coek::Model m;
        auto p = coek::parameter();
        auto v = m.add_variable("v");
        coek::Expression f = 3*p+2*v;
        m.add_objective(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{0};
        std::vector<double> baseline{2};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans == baseline );
    }
    WHEN( "multiple" ) {
        coek::Model m;
        auto v = m.add_variable("v");
        coek::Expression f = 7*v+v;
        m.add_objective(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{0};
        std::vector<double> baseline{8};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans == baseline );
    }
  }

  SECTION( "negate" ) {
    WHEN( "linear" ) {
        coek::Model m;
        auto v = m.add_variable("v");
        coek::Expression f = -(v+1);
        m.add_objective(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{0};
        std::vector<double> baseline{-1};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans == baseline );
    }
  }

  SECTION( "times" ) {
    WHEN( "lhs zero" ) {
        coek::Model m;
        coek::Expression p;
        auto v = m.add_variable("v");
        coek::Expression f = p*v;
        m.add_objective(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{0};
        std::vector<double> baseline{0};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans == baseline );
    }
    WHEN( "lhs constant" ) {
        coek::Model m;
        auto p = coek::parameter("p").value(2);
        auto v = m.add_variable("v");
        coek::Expression f = p*v;
        m.add_objective(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{0};
        std::vector<double> baseline{2};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans == baseline );
    }
    WHEN( "rhs zero" ) {
        coek::Model m;
        coek::Expression p;
        auto v = m.add_variable("v");
        coek::Expression f = v*p;
        m.add_objective(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{0};
        std::vector<double> baseline{0};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans == baseline );
    }
    WHEN( "rhs constant" ) {
        coek::Model m;
        auto p = coek::parameter("p").value(2);
        auto v = m.add_variable("v");
        coek::Expression f = v*p;
        m.add_objective(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{0};
        std::vector<double> baseline{2};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans == baseline );
    }
    WHEN( "simple quadratic" ) {
        coek::Model m;
        auto v = m.add_variable("v");
        auto w = m.add_variable("w").lower(0).upper(1).value(0);
        coek::Expression f = v*w;
        m.add_objective(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{2,3};
        std::vector<double> baseline{3,2};
        std::vector<double> ans(2);
        nlp.compute_df(x, ans);
        REQUIRE( ans == baseline );
    }
  }

  SECTION( "divide" ) {
    WHEN( "lhs zero" ) {
        coek::Model m;
        auto p = coek::parameter("p");
        auto w = m.add_variable("w").lower(0).upper(1).value(0);
        coek::Expression f = p/w;
        m.add_objective(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{1};
        std::vector<double> baseline{0};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans == baseline );
    }
    WHEN( "rhs nonzero" ) {
        coek::Model m;
        auto p = coek::parameter("p").value(2);
        auto w = m.add_variable("w").lower(0).upper(1).value(0);
        coek::Expression f = w/p;
        m.add_objective(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{1};
        std::vector<double> baseline{0.5};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans == baseline );
    }
    WHEN( "rhs polynomial" ) {
        coek::Model m;
        auto w = m.add_variable("w").lower(0).upper(1).value(0);
        coek::Expression f = w/(1+w);
        m.add_objective(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{1};
        std::vector<double> baseline{0.25};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans == baseline );
    }
  }

  SECTION( "coverage" ) {
    WHEN( "variable partial plus monomial - 1" ) {
        coek::Model m;
        auto v = m.add_variable("v");
        auto w = m.add_variable("w").lower(0).upper(1).value(0);
        coek::Expression f = w*v + v*(2*w+1);
        m.add_objective(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{2,3};
        std::vector<double> baseline{10,6};
        std::vector<double> ans(2);
        nlp.compute_df(x, ans);
        REQUIRE( ans == baseline );
    }
    WHEN( "variable partial plus monomial - 2" ) {
        coek::Model m;
        auto v = m.add_variable("v");
        auto w = m.add_variable("w").lower(0).upper(1).value(0);
        coek::Expression f = v*(2*w+1);
        m.add_objective(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{2,3};
        std::vector<double> baseline{7,4};
        std::vector<double> ans(2);
        nlp.compute_df(x, ans);
        REQUIRE( ans == baseline );
    }
    WHEN( "constant partial plus monomial" ) {
        coek::Model m;
        auto w = m.add_variable("w").lower(0).upper(1).value(0);
        coek::Expression f = 3*w + 2*w;
        m.add_objective(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{2};
        std::vector<double> baseline{5};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans == baseline );
    }
    WHEN( "negative monomial" ) {
        coek::Model m;
        auto w = m.add_variable("w").lower(0).upper(1).value(0);
        coek::Expression f = -(-w) + (-(-w));
        m.add_objective(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{2};
        std::vector<double> baseline{2};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans == baseline );
    }
    WHEN( "shared subexpr" ) {
        coek::Model m;
        auto v = m.add_variable("v");
        auto w = m.add_variable("w").lower(0).upper(1).value(0);
        coek::Expression f = v + 2*w;
        m.add_objective(2*f+3*f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{10,11};
        std::vector<double> baseline{5,10};
        std::vector<double> ans(2);
        nlp.compute_df(x, ans);
        REQUIRE( ans == baseline );
    }
  }

  SECTION( "intrinsic funcs" ) {
    WHEN( "exp" ) {
        coek::Model m;
        auto w = m.add_variable("w").lower(0).upper(1).value(0);
        coek::Expression f = abs(2*w);
        m.add_objective(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{-1};
        std::vector<double> baseline{-2};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans[0] == Approx(baseline[0]) );
    }
    //REPN_INTRINSIC_TEST1(ceil)
    //REPN_INTRINSIC_TEST1(floor)
    WHEN( "exp" ) {
        coek::Model m;
        auto w = m.add_variable("w").lower(0).upper(1).value(0);
        coek::Expression f = exp(2*w);
        m.add_objective(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{1};
        std::vector<double> baseline{ 2*pow(E,2.0) };
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans[0] == Approx(baseline[0]) );
    }
    WHEN( "log" ) {
        coek::Model m;
        auto w = m.add_variable("w").lower(0).upper(1).value(0);
        coek::Expression f = log(2*w);
        m.add_objective(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{2};
        std::vector<double> baseline{0.5};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans[0] == Approx(baseline[0]) );
    }
    WHEN( "log10" ) {
        coek::Model m;
        auto w = m.add_variable("w").lower(0).upper(1).value(0);
        coek::Expression f = log10(2*w);
        m.add_objective(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{2};
        std::vector<double> baseline{0.5/log(10.0)};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans[0] == Approx(baseline[0]) );
    }
    WHEN( "sqrt" ) {
        coek::Model m;
        auto w = m.add_variable("w").lower(0).upper(1).value(0);
        coek::Expression f = sqrt(2*w);
        m.add_objective(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{2};
        std::vector<double> baseline{0.5};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans[0] == Approx(baseline[0]) );
    }
    WHEN( "sin" ) {
        coek::Model m;
        auto w = m.add_variable("w").lower(0).upper(1).value(0);
        coek::Expression f = sin(2*w);
        m.add_objective(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{2};
        std::vector<double> baseline{2*cos(4)};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans[0] == Approx(baseline[0]) );
    }
    WHEN( "cos" ) {
        coek::Model m;
        auto w = m.add_variable("w").lower(0).upper(1).value(0);
        coek::Expression f = cos(2*w);
        m.add_objective(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{2};
        std::vector<double> baseline{-2*sin(4)};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans[0] == Approx(baseline[0]) );
    }
    WHEN( "tan" ) {
        coek::Model m;
        auto w = m.add_variable("w").lower(0).upper(1).value(0);
        coek::Expression f = tan(2*w);
        m.add_objective(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{2};
        std::vector<double> baseline{2/pow(cos(4),2)};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans[0] == Approx(baseline[0]) );
    }
    WHEN( "sinh" ) {
        coek::Model m;
        auto w = m.add_variable("w").lower(0).upper(1).value(0);
        coek::Expression f = sinh(2*w);
        m.add_objective(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{2};
        std::vector<double> baseline{2*cosh(4)};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans[0] == Approx(baseline[0]) );
        //static std::list<std::string> baseline = { "[", "*", "2.000", "[", "cosh", "[", "*", "2", "w", "]", "]", "]" };
    }
    WHEN( "cosh" ) {
        coek::Model m;
        auto w = m.add_variable("w").lower(0).upper(1).value(0);
        coek::Expression f = cosh(2*w);
        m.add_objective(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{2};
        std::vector<double> baseline{2*sinh(4)};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans[0] == Approx(baseline[0]) );
        //static std::list<std::string> baseline = { "[", "*", "2.000", "[", "sinh", "[", "*", "2", "w", "]", "]", "]" };
    }
    WHEN( "tanh" ) {
        coek::Model m;
        auto w = m.add_variable("w").lower(0).upper(1).value(0);
        coek::Expression f = tanh(2*w);
        m.add_objective(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{2};
        std::vector<double> baseline{2*(1-pow(tanh(4),2))};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans[0] == Approx(baseline[0]) );
        //static std::list<std::string> baseline = { "[", "*", "2.000", "[", "+", "1.000", "[", "*", "-1.000", "[", "pow", "[", "tan", "[", "*", "2", "w", "]", "]", "2.000", "]", "]", "]", "]" };
    }
    WHEN( "asin" ) {
        coek::Model m;
        auto w = m.add_variable("w").lower(0).upper(1).value(0);
        coek::Expression f = asin(2*w);
        m.add_objective(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{0.25};
        std::vector<double> baseline{2/sqrt(3.0/4.0)};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans[0] == Approx(baseline[0]) );
        //static std::list<std::string> baseline = { "[", "*", "2.000", "[", "/", "1.000", "[", "sqrt", "[", "+", "1.000", "[", "-", "[", "*", "[", "*", "2", "w", "]", "[", "*", "2", "w", "]", "]", "]", "]", "]", "]", "]" };
    }
    WHEN( "acos" ) {
        coek::Model m;
        auto w = m.add_variable("w").lower(0).upper(1).value(0);
        coek::Expression f = acos(2*w);
        m.add_objective(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{0.25};
        std::vector<double> baseline{-2/sqrt(3.0/4.0)};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans[0] == Approx(baseline[0]) );
        
        //static std::list<std::string> baseline = { "[", "*", "2.000", "[", "-", "[", "/", "1.000", "[", "sqrt", "[", "+", "1.000", "[", "-", "[", "*", "[", "*", "2", "w", "]", "[", "*", "2", "w", "]", "]", "]", "]", "]", "]", "]", "]" };
    }
    WHEN( "atan" ) {
        coek::Model m;
        auto w = m.add_variable("w").lower(0).upper(1).value(0);
        coek::Expression f = atan(2*w);
        m.add_objective(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{0.25};
        std::vector<double> baseline{2/(5.0/4.0)};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans[0] == Approx(baseline[0]) );
        //static std::list<std::string> baseline = { "[", "*", "2.000", "[", "/", "1.000", "[", "+", "1.000", "[", "*", "[", "*", "2", "w", "]", "[", "*", "2", "w", "]", "]", "]", "]", "]" };
    }
    WHEN( "asinh" ) {
        coek::Model m;
        auto w = m.add_variable("w").lower(0).upper(1).value(0);
        coek::Expression f = asinh(2*w);
        m.add_objective(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{0.25};
        std::vector<double> baseline{2/sqrt(5.0/4.0)};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans[0] == Approx(baseline[0]) );
        
        //static std::list<std::string> baseline = { "[", "*", "2.000", "[", "/", "1.000", "[", "sqrt", "[", "+", "1.000", "[", "*", "[", "*", "2", "w", "]", "[", "*", "2", "w", "]", "]", "]", "]", "]", "]" };
    }
    WHEN( "acosh" ) {
        coek::Model m;
        auto w = m.add_variable("w").lower(0).upper(1).value(0);
        coek::Expression f = acosh(2*w);
        m.add_objective(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{1};
        std::vector<double> baseline{2/sqrt(3.0)};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans[0] == Approx(baseline[0]) );
        
        //static std::list<std::string> baseline = { "[", "*", "2.000", "[", "/", "1.000", "[", "sqrt", "[", "+", "[", "*", "[", "*", "2", "w", "]", "[", "*", "2", "w", "]", "]", "-1.000", "]", "]", "]", "]" };
    }
    WHEN( "atanh" ) {
        coek::Model m;
        auto w = m.add_variable("w").lower(0).upper(1).value(0);
        coek::Expression f = atanh(2*w);
        m.add_objective(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{0.25};
        std::vector<double> baseline{2/(3.0/4.0)};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans[0] == Approx(baseline[0]) );
        
        //static std::list<std::string> baseline = { "[", "*", "2.000", "[", "/", "1.000", "[", "+", "[", "*", "[", "*", "2", "w", "]", "[", "*", "2", "w", "]", "]", "-1.000", "]", "]", "]" };
    }
    WHEN( "pow - 1" ) {
        coek::Model m;
        auto w = m.add_variable("w").lower(0).upper(1).value(0);
        coek::Expression f = pow(w, 3);
        m.add_objective(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{2};
        std::vector<double> baseline{3*4};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans[0] == Approx(baseline[0]) );
        
        //static std::list<std::string> baseline = { "[", "*", "2.000", "[", "*", "3.000", "[", "pow", "[", "*", "2", "w", "]", "[", "+", "3.000", "-1.000", "]", "]", "]", "]" };
    }
    WHEN( "pow - 2" ) {
        coek::Model m;
        auto w = m.add_variable("w").lower(0).upper(1).value(0);
        coek::Expression f = pow(3, 2*w);
        m.add_objective(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{2};
        std::vector<double> baseline{2*log(3)*pow(3,4)};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans[0] == Approx(baseline[0]) );
        
        //static std::list<std::string> baseline = { "[", "*", "2.000", "[", "*", "1.099", "[", "pow", "3.000", "[", "*", "2", "w", "]", "]", "]", "]" };
    }
  }

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}

