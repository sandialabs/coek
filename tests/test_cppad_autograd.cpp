#include <iostream>
#include "catch.hpp"
#include "coek_model.hpp"
#include "expr/ast_term.hpp"

const double PI = 3.141592653589793238463;
const double E = exp(1.0);


TEST_CASE( "cppad_add", "[smoke]" ) {

  SECTION( "error1" ) {
    coek::Variable v;
    coek::Model model;
    model.add(v);

    coek::NLPModel nlp;
    REQUIRE_THROWS_WITH(nlp.initialize(model, "cppad"),
            "Unexpected variable not owned by a model.");
    }

  SECTION( "error2" ) {
    coek::Variable v;
    coek::Model model;
    model.add(2*v);

    coek::NLPModel nlp;
    REQUIRE_THROWS_WITH(nlp.initialize(model, "cppad"),
            "Unexpected variable not owned by a model.");
    }

  SECTION( "error3" ) {
    coek::Model model;
    coek::NLPModel nlp;
    REQUIRE_THROWS_WITH(nlp.initialize(model, "bad"),
            "Unknown autograd type.");
    }

  SECTION( "Add Objective" ) {
    coek::Model model;
    auto x = model.getVariable(0,1,0,"x");
    auto y = model.getVariable(0,1,0,"y");

    model.add( x + y );

    REQUIRE( model.repn->variables.size() == 2 );

    coek::NLPModel m;

    REQUIRE( m.num_variables() == 0 );
    REQUIRE( m.num_objectives() == 0 );
    REQUIRE( m.num_constraints() == 0 );
    m.initialize(model, "cppad");
    REQUIRE( m.num_variables() == 2 );
    REQUIRE( m.num_objectives() == 1 );
    REQUIRE( m.num_constraints() == 0 );
    }

  SECTION( "Add Inequality" ) {
    coek::Model model;
    auto x = model.getVariable(0,1,0,"x");
    auto y = model.getVariable(0,1,0,"y");

    auto e = x + y <= 0;
    model.add(e);

    coek::NLPModel m;
    REQUIRE( m.num_variables() == 0 );
    REQUIRE( m.num_objectives() == 0 );
    REQUIRE( m.num_constraints() == 0 );
    m.initialize(model, "cppad");
    REQUIRE( m.num_variables() == 2 );
    REQUIRE( m.num_objectives() == 0 );
    REQUIRE( m.num_constraints() == 1 );
    }

  SECTION( "Add Equality" ) {
    coek::Model model;
    auto x = model.getVariable(0,1,0,"x");
    auto y = model.getVariable(0,1,0,"y");

    auto e = x + y == 0;
    model.add(e);

    coek::NLPModel m;
    REQUIRE( m.num_variables() == 0 );
    REQUIRE( m.num_objectives() == 0 );
    REQUIRE( m.num_constraints() == 0 );
    m.initialize(model, "cppad");
    REQUIRE( m.num_variables() == 2 );
    REQUIRE( m.num_objectives() == 0 );
    REQUIRE( m.num_constraints() == 1 );
    }
}


TEST_CASE( "cppad_ad", "[smoke]" ) {
  
  SECTION( "f" ) {
    coek::Model model;
    auto a = model.getVariable(0,1,0.0,"a");
    auto b = model.getVariable(0,1,0.0,"b");

    model.add(a + b);
    model.add(a * b);

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
    auto a = model.getVariable(0,1,0.0,"a");
    auto b = model.getVariable(0,1,0.0,"b");

    model.add( a + b );
    model.add( a * b );

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
    auto a = model.getVariable(0,1,0,"a");
    auto b = model.getVariable(0,1,0,"b");

    model.add( a + b <= 0 );
    model.add( a * b == 0 );

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
    auto a = model.getVariable(0,1,0,"a");
    auto b = model.getVariable(0,1,0,"b");

    model.add( a + b <= 0 );
    model.add( a * b == 0 );

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
        auto a = model.getVariable(0,1,0,"a");
        auto b = model.getVariable(0,1,0,"b");

        model.add( a );
        model.add( a <= 0 );
        model.add( a * b <= 0 );
        model.add( b <= 0 );

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
        auto a = model.getVariable(0,1,0,"a");
        auto b = model.getVariable(0,1,0,"b");
        auto c = model.getVariable(0,1,0,"c");
        auto d = model.getVariable(0,1,0,"d");

        model.add( d );
        model.add( a + a*b + b <= 0 );
        model.add( b + b*c + c <= 0 );

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
        auto a = model.getVariable(0,1,0,"a");
        auto b = model.getVariable(0,1,0,"b");

        model.add( a );
        model.add( a <= 0 );
        model.add( a * b <= 0 );
        model.add( b <= 0 );

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
        auto a = model.getVariable(0,1,0,"a");
        auto b = model.getVariable(0,1,0,"b");
        auto c = model.getVariable(0,1,0,"c");

        model.add( a );
        model.add( a + a*b + b <= 0 );
        model.add( b + b*c + c <= 0 );

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
        auto a = model.getVariable(0,1,0,"a");
        auto b = model.getVariable(0,1,0,"b");

        model.add( a*a + b );
        model.add( a <= 0 );
        model.add( a * b <= 0 );
        model.add( b <= 0 );

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
        auto a = model.getVariable(0,1,0,"a");
        auto b = model.getVariable(0,1,0,"b");
        auto c = model.getVariable(0,1,0,"c");
        auto d = model.getVariable(0,1,0,"d");

        model.add( d*d*c );
        model.add( a + a*b + b <= 0 );
        model.add( b + b*c + c <= 0 );

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
        auto a = model.getVariable(0,1,0,"a");
        auto b = model.getVariable(0,1,0,"b");
        auto c = model.getVariable(0,1,0,"c");
        auto d = model.getVariable(0,1,0,"d");

        model.add( d*d*c );
        model.add( a + a*b + b <= 0 );
        model.add( b + b*c + c <= 0 );

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
        auto a = model.getVariable(0.1,100,1,"a");
        auto b = model.getVariable(0.1,100,2,"b");
        model.add( pow(b - pow(a,2),2) + pow(a-1,2) );

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
        auto a = model.getVariable(0,1,0,"a");
        auto b = model.getVariable(0,1,0,"b");

        model.add( a*a + b );
        model.add( a <= 0 );
        model.add( a * b <= 0 );
        model.add( b <= 0 );

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
        auto a = model.getVariable(0,1,0,"a");
        auto b = model.getVariable(0,1,0,"b");
        auto c = model.getVariable(0,1,0,"c");
        auto d = model.getVariable(0,1,0,"d");

        model.add( d*d*c );
        model.add( a + a*b + b <= 0 );
        model.add( b + b*c + c <= 0 );

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
    coek::Expression f = 3;
    auto v = model.getVariable(0,1,0,"v");
    model.add(f*v);
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
        coek::Parameter p(3);
        coek::Expression f = p;
        auto v = model.getVariable(0,1,0,"v");
        model.add(f*v);
        coek::NLPModel nlp(model, "cppad");

        std::vector<double> x{0};
        std::vector<double> baseline{3};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans == baseline );

        p.set_value(4);
        nlp.reset();
        std::vector<double> baseline2{4};
        nlp.compute_df(x, ans);
        REQUIRE( ans == baseline2 );
    }
    WHEN( "many" ) {
        coek::Model model;
        coek::Parameter p1(1);
        coek::Parameter p2(2);
        coek::Parameter p3(3);
        coek::Parameter p4(4);
        coek::Parameter p5(5);
        coek::Expression f = p1+2*p2+3*p3+4*p4+5*p5;
        auto v = model.getVariable(0,1,0,"v");
        model.add(f*v);
        coek::NLPModel nlp(model, "cppad");

        std::vector<double> x{0};
        std::vector<double> baseline{1+4+9+16+25};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans == baseline );

        p5.set_value(10);
        nlp.reset();
        std::vector<double> baseline2{1+4+9+16+50};
        nlp.compute_df(x, ans);
        REQUIRE( ans == baseline2 );
    }
  }

  SECTION( "var" ) {
    WHEN( "fixed" ) {
        coek::Model model;
        auto v = model.getVariable(0,1,0,"v");
        auto w = model.getVariable(0,1,0,"w");
        v.set_fixed(true);
        coek::Expression f = v + 2*w;
        model.add( f );
        coek::NLPModel nlp(model, "cppad");

        std::vector<double> x{0};
        std::vector<double> baseline{2};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans == baseline );
    }

    WHEN( "unfixed" ) {
        coek::Model model;
        auto v = model.getVariable(0,1,0,"v");
        auto w = model.getVariable(0,1,0,"w");
        coek::Expression f = v;
        model.add( f );
        model.add( w );
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
        coek::Variable v = m.getVariable(0,1,0,"v");
        coek::Variable w = m.getVariable(0,1,0,"w");
        coek::Expression f = 2*v;
        m.add( f );
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{0};
        std::vector<double> baseline{2};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans == baseline );
    }

    WHEN( "fixed" ) {
        coek::Model m;
        coek::Variable v = m.getVariable(0,1,0,"v");
        coek::Variable w = m.getVariable(0,1,0,"w");
        v.set_fixed(true);
        coek::Expression f = 2*v;
        m.add(f + 3*w);
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
        coek::Parameter p(0);
        coek::Variable v = m.getVariable(0,1,0,"v");
        coek::Expression f = 2*(v+v)+v;
        m.add(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{0};
        std::vector<double> baseline{5};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans == baseline );
    }
    WHEN( "simple" ) {
        coek::Model m;
        coek::Parameter p(0);
        coek::Variable v = m.getVariable(0,1,0,"v");
        coek::Expression f = 3*p+2*v;
        m.add(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{0};
        std::vector<double> baseline{2};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans == baseline );
    }
    WHEN( "multiple" ) {
        coek::Model m;
        coek::Variable v = m.getVariable(0,1,0,"v");
        coek::Expression f = 7*v+v;
        m.add(f);
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
        coek::Variable v = m.getVariable(0,1,0,"v");
        coek::Expression f = -(v+1);
        m.add(f);
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
        coek::Variable v = m.getVariable(0,1,0,"v");
        coek::Expression f = p*v;
        m.add(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{0};
        std::vector<double> baseline{0};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans == baseline );
    }
    WHEN( "lhs constant" ) {
        coek::Model m;
        coek::Parameter p(2,"p");
        coek::Variable v = m.getVariable(0, 1, 0, "v");
        coek::Expression f = p*v;
        m.add(f);
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
        coek::Variable v = m.getVariable(0, 1, 0, "v");
        coek::Expression f = v*p;
        m.add(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{0};
        std::vector<double> baseline{0};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans == baseline );
    }
    WHEN( "rhs constant" ) {
        coek::Model m;
        coek::Parameter p(2, "p");
        coek::Variable v = m.getVariable(0, 1, 0, "v");
        coek::Expression f = v*p;
        m.add(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{0};
        std::vector<double> baseline{2};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans == baseline );
    }
    WHEN( "simple quadratic" ) {
        coek::Model m;
        coek::Variable v = m.getVariable(0, 1, 0, "v");
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = v*w;
        m.add(f);
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
        coek::Parameter p(0,"p");
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = p/w;
        m.add(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{1};
        std::vector<double> baseline{0};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans == baseline );
    }
    WHEN( "rhs nonzero" ) {
        coek::Model m;
        coek::Parameter p(2.0,"p");
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = w/p;
        m.add(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{1};
        std::vector<double> baseline{0.5};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans == baseline );
    }
    WHEN( "rhs polynomial" ) {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = w/(1+w);
        m.add(f);
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
        coek::Variable v = m.getVariable(0, 1, 0, "v");
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = w*v + v*(2*w+1);
        m.add(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{2,3};
        std::vector<double> baseline{10,6};
        std::vector<double> ans(2);
        nlp.compute_df(x, ans);
        REQUIRE( ans == baseline );
    }
    WHEN( "variable partial plus monomial - 2" ) {
        coek::Model m;
        coek::Variable v = m.getVariable(0, 1, 0, "v");
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = v*(2*w+1);
        m.add(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{2,3};
        std::vector<double> baseline{7,4};
        std::vector<double> ans(2);
        nlp.compute_df(x, ans);
        REQUIRE( ans == baseline );
    }
    WHEN( "constant partial plus monomial" ) {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = 3*w + 2*w;
        m.add(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{2};
        std::vector<double> baseline{5};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans == baseline );
    }
    WHEN( "negative monomial" ) {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = -(-w) + (-(-w));
        m.add(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{2};
        std::vector<double> baseline{2};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans == baseline );
    }
    WHEN( "shared subexpr" ) {
        coek::Model m;
        coek::Variable v = m.getVariable(0, 1, 0, "v");
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = v + 2*w;
        m.add(2*f+3*f);
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
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = abs(2*w);
        m.add(f);
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
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = exp(2*w);
        m.add(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{1};
        std::vector<double> baseline{ 2*pow(E,2.0) };
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans[0] == Approx(baseline[0]) );
    }
    WHEN( "log" ) {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = log(2*w);
        m.add(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{2};
        std::vector<double> baseline{0.5};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans[0] == Approx(baseline[0]) );
    }
    WHEN( "log10" ) {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = log10(2*w);
        m.add(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{2};
        std::vector<double> baseline{0.5/log(10.0)};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans[0] == Approx(baseline[0]) );
    }
    WHEN( "sqrt" ) {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = sqrt(2*w);
        m.add(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{2};
        std::vector<double> baseline{0.5};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans[0] == Approx(baseline[0]) );
    }
    WHEN( "sin" ) {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = sin(2*w);
        m.add(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{2};
        std::vector<double> baseline{2*cos(4)};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans[0] == Approx(baseline[0]) );
    }
    WHEN( "cos" ) {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = cos(2*w);
        m.add(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{2};
        std::vector<double> baseline{-2*sin(4)};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans[0] == Approx(baseline[0]) );
    }
    WHEN( "tan" ) {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = tan(2*w);
        m.add(f);
        coek::NLPModel nlp(m, "cppad");

        std::vector<double> x{2};
        std::vector<double> baseline{2/pow(cos(4),2)};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE( ans[0] == Approx(baseline[0]) );
    }
    WHEN( "sinh" ) {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = sinh(2*w);
        m.add(f);
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
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = cosh(2*w);
        m.add(f);
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
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = tanh(2*w);
        m.add(f);
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
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = asin(2*w);
        m.add(f);
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
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = acos(2*w);
        m.add(f);
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
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = atan(2*w);
        m.add(f);
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
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = asinh(2*w);
        m.add(f);
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
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = acosh(2*w);
        m.add(f);
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
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = atanh(2*w);
        m.add(f);
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
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = pow(w, 3);
        m.add(f);
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
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = pow(3, 2*w);
        m.add(f);
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

