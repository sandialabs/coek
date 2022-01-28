
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

//void zzz() {}

/*
TEST_CASE( "capi_misc", "[smoke]" ) {

  SECTION( "Misc API functions" ) {
    REQUIRE( misc_getnull() == 0 );
    REQUIRE( std::isnan(misc_getnan()) );
  }

  SECTION( "API printing" ) {
      std::stringstream os;
      std::streambuf* coutbuf = std::cout.rdbuf();
      std::cout.rdbuf(os.rdbuf());

      WHEN( "variable" ) {
        coek::Variable a(0.0, 1.0, 0.0, false, false, "a");
        std::cout << a << std::endl;

        std::string tmp = os.str();
        REQUIRE( tmp == "a{0}\n" );

        char ctmp[10];
        get_numval_str(a, ctmp, 10);
        tmp = ctmp;
        REQUIRE( tmp == "a{0.000}" );
      }

      WHEN( "parameter - int" ) {
        apival_t q = create_parameter_int(0, 2, true, "q");
        print_parameter(q);

        std::string tmp = os.str();
        REQUIRE( tmp == "q{2}\n" );

        char ctmp[10];
        get_numval_str(q, ctmp, 10);
        tmp = ctmp;
        REQUIRE( tmp == "q{2}" );
      }

      WHEN( "parameter - double" ) {
        apival_t q = create_parameter_double(0, 2.0, true, "q");
        print_parameter(q);

        std::string tmp = os.str();
        REQUIRE( tmp == "q{2}\n" );

        char ctmp[10];
        get_numval_str(q, ctmp, 10);
        tmp = ctmp;
        REQUIRE( tmp == "q{2.000}" );
      }

      WHEN( "expression" ) {
        apival_t a = create_variable(0, false, false, 0.0, 1.0, 0.0, "a");
        Variable* v = static_cast<Variable*>(a);
        v->index = 1;
        apival_t q = create_parameter_int(0, 0, true, "q");
        apival_t e = expr_plus_expression(a,q);
        print_expr(e);

        std::string tmp = os.str();
        REQUIRE( tmp == "a{0} + q{0}\n" );
      }

      std::cout.rdbuf(coutbuf);
  }

  SECTION( "API expressions" ) {
      WHEN( "size" ) {
        apival_t a = create_variable(0, false, false, 0.0, 1.0, 0.0, "a");
        apival_t q = create_parameter_int(0, 0, true, "q");
        apival_t e = expr_plus_expression(a,q);

        REQUIRE( expr_size(e) == 3 );
      }
  }
}
*/


TEST_CASE( "model_parameter", "[smoke]" ) {

  SECTION( "values" ) {
      WHEN( "parameter - 3" ) {
        coek::Parameter q(2, "q");
        REQUIRE( q.get_value() == 2 );
        q.set_value(3);
        REQUIRE( q.get_value() == 3 );
      }
  }

  SECTION( "constructors" ) {
      WHEN( "copy" ) {
        // Simple constructor
        coek::Parameter q(3);

        // Test copy constructor
        coek::Parameter Q(q);
        REQUIRE( Q.get_value() == 3 );
      }

      WHEN( "equal" ) {
        // Simple constructor
        coek::Parameter q(4);

        // Test copy constructor
        coek::Parameter Q(5);
        Q = q;
        REQUIRE( Q.get_value() == 4 );
      }
  }

  SECTION( "constructors" ) {
    coek::Parameter q(3, "q");
    REQUIRE( q.get_name() == "q" );
  }

  SECTION( "write" ) {
    std::stringstream sstr;
    coek::Parameter q(3, "q");
    sstr << q;
    REQUIRE( sstr.str() == "q" );
  }

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}

TEST_CASE( "model_IndexParameter", "[smoke]" ) {

  SECTION( "values" ) {
      WHEN( "IndexParameter - 3" ) {
        coek::IndexParameter q("q");
        q.set_value(1);
        int tmp = -1;
        auto success = q.get_value(tmp);
        REQUIRE( success == true );
        REQUIRE( tmp == 1 );
        q.set_value(3.5);
        success = q.get_value(tmp);
        REQUIRE( success == false );
      }
      WHEN( "IndexParameter - 3.5" ) {
        coek::IndexParameter q("");
        q.set_value(3.5);
        double tmp = -3.5;
        auto success = q.get_value(tmp);
        REQUIRE( success == true );
        REQUIRE( tmp == 3.5 );
        q.set_value(3);
        success = q.get_value(tmp);
        REQUIRE( success == false );
      }
      WHEN( "IndexParameter - 'here'" ) {
        coek::IndexParameter q("");
        q.set_value("here");
        std::string tmp = "there";
        auto success = q.get_value(tmp);
        REQUIRE( success == true );
        REQUIRE( tmp == "here" );
        q.set_value(3.5);
        success = q.get_value(tmp);
        REQUIRE( success == false );
      }
  }

  SECTION( "constructors" ) {
      WHEN( "copy" ) {
        // Simple constructor
        coek::IndexParameter q;
        q.set_value(3);

        // Test copy constructor
        coek::IndexParameter Q(q);
        int tmp=-1;
        auto success = Q.get_value(tmp);
        REQUIRE( success == true );
        REQUIRE( tmp == 3 );
      }

      WHEN( "equal" ) {
        // Simple constructor
        coek::IndexParameter q;
        q.set_value(4);

        // Test copy constructor
        coek::IndexParameter Q;
        Q.set_value(5);
        Q = q;
        int tmp=-1;
        auto success = Q.get_value(tmp);
        REQUIRE( success == true );
        REQUIRE( tmp == 4 );
      }
  }

  SECTION( "constructors" ) {
    coek::IndexParameter q("q");
    REQUIRE( q.get_name() == "q" );
  }

  SECTION( "write" ) {
    std::stringstream sstr;
    coek::IndexParameter q("q");
    sstr << q;
    REQUIRE( sstr.str() == "q" );
  }

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}

TEST_CASE( "model_variables", "[smoke]" ) {

  SECTION( "constructors" ) {
    WHEN( "simple" ) {
        coek::Variable a(0,1,2);
        REQUIRE( a.get_value() == 2 );
        REQUIRE( a.get_lb() == 0 );
        REQUIRE( a.get_ub() == 1 );
        // Keep this test?  The integer index depends on whether this test is run first.
        REQUIRE( a.get_name() == "x(0)" );
    }

    WHEN( "named" ) {
        coek::Variable a("test",0,1,2);
        REQUIRE( a.get_value() == 2 );
        REQUIRE( a.get_lb() == 0 );
        REQUIRE( a.get_ub() == 1 );
        REQUIRE( a.get_name() == "test" );
    }

    WHEN( "simple_with_expr_values" ) {
        coek::Parameter p(0);
        coek::Variable a(p, p+1, p+2);
        REQUIRE( a.get_value() == 2 );
        REQUIRE( a.get_lb() == 0 );
        REQUIRE( a.get_ub() == 1 );
        p.set_value(1);
        REQUIRE( a.get_value() == 3 );
        REQUIRE( a.get_lb() == 1 );
        REQUIRE( a.get_ub() == 2 );
    }

    WHEN( "named_with_expr_values" ) {
        coek::Parameter p(0);
        coek::Variable a("test", p, p+1, p+2);
        REQUIRE( a.get_value() == 2 );
        REQUIRE( a.get_lb() == 0 );
        REQUIRE( a.get_ub() == 1 );
        p.set_value(1);
        REQUIRE( a.get_value() == 3 );
        REQUIRE( a.get_lb() == 1 );
        REQUIRE( a.get_ub() == 2 );
    }

    WHEN( "initialize" ) {
        coek::Variable a("test");
        a.initialize(0,1,2, false, false, false);
        REQUIRE( a.get_value() == 2 );
        REQUIRE( a.get_lb() == 0 );
        REQUIRE( a.get_ub() == 1 );
        REQUIRE( a.get_name() == "test" );
    }

    WHEN( "initialize_with_parameters" ) {
        coek::Parameter p(0);
        coek::Variable a("test");
        a.initialize(p,p+1,p+2, false, false, false);
        REQUIRE( a.get_value() == 2 );
        REQUIRE( a.get_lb() == 0 );
        REQUIRE( a.get_ub() == 1 );
        REQUIRE( a.get_name() == "test" );
        p.set_value(1);
        REQUIRE( a.get_value() == 3 );
        REQUIRE( a.get_lb() == 1 );
        REQUIRE( a.get_ub() == 2 );
    }

    WHEN( "copy" ) {
        coek::Variable a("test",0,1,2);
        coek::Variable b(a);
        REQUIRE( b.get_value() == 2 );
        REQUIRE( b.get_lb() == 0 );
        REQUIRE( b.get_ub() == 1 );
        REQUIRE( b.get_name() == "test" );
        REQUIRE( a.id() == b.id() );
    }

    WHEN( "equal" ) {
        coek::Variable a("test",0,1,2);
        coek::Variable b;
        b = a;
        REQUIRE( b.get_value() == 2 );
        REQUIRE( b.get_lb() == 0 );
        REQUIRE( b.get_ub() == 1 );
        REQUIRE( b.get_name() == "test" );
        REQUIRE( a.id() == b.id() );
    }
  }

  SECTION( "index" ) {
    coek::Model model;
    coek::Variable a("a", 0.0, 1.0, 0.0, false, false);
    model.add_variable(a);
    coek::Variable b = model.add_variable("b",0,1,0,false,false); 
    REQUIRE( a.id() == (b.id() - 1) );
  }

  SECTION( "values" ) {
      WHEN( "variable - 3" ) {
        coek::Variable a("a", 0.0, 1.0, 0.0, false, false);
        REQUIRE( a.get_value() == 0 );
        a.set_value(3);
        REQUIRE( a.get_value() == 3 );
      }

/*
      WHEN( "variable array - 3" ) {
        void* a[2];
        create_variable_array(0, a, 2, false, false, 0.0, 1.0, 0.0, "a");
        variable_set_value(a[0], 3);

        REQUIRE( variable_get_value(a[0]) == 3 );
        REQUIRE( variable_get_value(a[1]) == 0 );
      }

      WHEN( "variable array (2) - 3" ) {
        apival_t model = create_model();

        void* a[2];
        create_variable_array(model, a, 2, false, false, 0.0, 1.0, 0.0, "a");
        variable_set_value(a[0], 3);

        REQUIRE( variable_get_value(a[0]) == 3 );
        REQUIRE( variable_get_value(a[1]) == 0 );
      }
*/
  }

  SECTION( "bounds" ) {
      WHEN( "lb" ) {
        coek::Variable a("a", 0.0, 1.0, 0.0, false, false);
        REQUIRE( a.get_lb() == 0.0 );
        a.set_lb(3.0);
        REQUIRE( a.get_lb() == 3.0 );
      }

      WHEN( "ub" ) {
        coek::Variable a("a", 0.0, 1.0, 0.0, false, false);
        REQUIRE( a.get_ub() == 1.0 );
        a.set_ub(3.0);
        REQUIRE( a.get_ub() == 3.0 );
      }
  }

  SECTION( "properties" ) {
    WHEN( "continuous" ) {
        coek::Variable a;
        REQUIRE( a.is_continuous() == true );
        REQUIRE( a.is_binary() == false );
        REQUIRE( a.is_integer() == false );
    }

    WHEN( "binary" ) {
        coek::Variable a("a", 0,1,0,true,false);
        REQUIRE( a.is_continuous() == false );
        REQUIRE( a.is_binary() == true );
        REQUIRE( a.is_integer() == false );
    }

    WHEN( "integer" ) {
        coek::Variable a("a", 0,10,5,false,true);
        REQUIRE( a.is_continuous() == false );
        REQUIRE( a.is_binary() == false );
        REQUIRE( a.is_integer() == true );
    }
  
    WHEN( "fixed" ) {
        coek::Variable a("a", 0,10,5,false,true);
        REQUIRE( a.get_fixed() == false );
        a.set_fixed(true);
        REQUIRE( a.get_fixed() == true );
        REQUIRE( a.get_value() == 5 );
        a.fix(3);
        REQUIRE( a.get_fixed() == true );
        REQUIRE( a.get_value() == 3 );
    }
  }

  SECTION( "write" ) {
    std::stringstream sstr;
    coek::Variable q("q", 0,1,0);
    sstr << q;
    REQUIRE( sstr.str() == "q" );
  }

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}


TEST_CASE( "model_monomial", "[smoke]" ) {

  SECTION( "trivial - int*variable" ) {
    coek::Variable v("v",0,1,0);
    coek::Expression e = 1*v;
    REQUIRE( e.repn == v.repn );
    static std::list<std::string> baseline = {"v"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "simple monomial - int*variable" ) {
    coek::Variable v("v",0,1,0);
    coek::Expression e = 2*v;
    static std::list<std::string> baseline = {"[", "*", "2", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "trivial - int*parameter" ) {
    coek::Parameter v(1,"v");
    coek::Expression e = 1*v;
    //REQUIRE( e.repn == v.repn );
    static std::list<std::string> baseline = {"v"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "simple monomial - int*parameter" ) {
    coek::Parameter v(1,"v");
    coek::Expression e = 2*v;
    static std::list<std::string> baseline = {"[", "*", "2.000", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "trivial - double*variable" ) {
    coek::Variable v("v",0,1,0);
    coek::Expression e = 1.0*v;
    REQUIRE( e.repn == v.repn );
    static std::list<std::string> baseline = {"v"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "simple monomial - double*variable" ) {
    coek::Variable v("v",0,1,0);
    coek::Expression e = 2.0*v;
    static std::list<std::string> baseline = {"[", "*", "2", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "trivial - double*parameter" ) {
    coek::Parameter v(1,"v");
    coek::Expression e = 1.0*v;
    //REQUIRE( e.repn == v.repn );
    static std::list<std::string> baseline = {"v"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "simple monomial - int*parameter" ) {
    coek::Parameter v(1,"v");
    coek::Expression e = 2.0*v;
    static std::list<std::string> baseline = {"[", "*", "2.000", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

/*
  SECTION( "unnamed monomial" ) {
    coek::Variable v(0,1,0);
    coek::Expression e = 2*v;
    static std::list<std::string> baseline = {"[", "*", "2", "x0", "]"};
    REQUIRE( e.to_list() == baseline );
  }
*/

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}


TEST_CASE( "model_expression", "[smoke]" ) {

  SECTION( "constructors" ) {
    WHEN( "simple" ) {
        coek::Expression a;
        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( a.to_list() == baseline );
    }

    WHEN( "double" ) {
        coek::Expression a(1.0);
        static std::list<std::string> baseline = {"1.000"};
        REQUIRE( a.to_list() == baseline );
    }

    WHEN( "equal" ) {
        coek::Expression a(1.0);
        coek::Expression b;
        b = a;
        static std::list<std::string> baseline = {"1.000"};
        REQUIRE( a.to_list() == baseline );
    }
  }

  SECTION( "is_constant" ) {
    WHEN( "constant" ) {
        coek::Expression a(1.0);
        REQUIRE( a.is_constant() == true );
    }

    WHEN( "parameter" ) {
        coek::Parameter p(1.0);
        coek::Expression a;
        a = p;
        REQUIRE( a.is_constant() == false );
    }

    WHEN( "index_parameter" ) {
        coek::IndexParameter p("p");
        coek::Expression a;
        a = p;
        REQUIRE( a.is_constant() == false );
    }

    WHEN( "variable" ) {
        coek::Variable p(0,1,0);
        coek::Expression a;
        a = p;
        REQUIRE( a.is_constant() == false );
    }
  }

  SECTION( "plus-equal" ) {
    WHEN("variable") {
        coek::Variable v("v");
        coek::Variable p("p",0,1,0);
        coek::Expression a = v;
        a += p;
        static std::list<std::string> baseline = {"[", "+", "v", "p", "]"};
        REQUIRE( a.to_list() == baseline );
    }
    WHEN("double") {
        coek::Variable v("v");
        coek::Expression a = v;
        double p=1;
        a += p;
        static std::list<std::string> baseline = {"[", "+", "v", "1.000", "]"};
        REQUIRE( a.to_list() == baseline );
    }
    WHEN("int") {
        coek::Variable v("v");
        coek::Expression a = v;
        int p=1;
        a += p;
        static std::list<std::string> baseline = {"[", "+", "v", "1.000", "]"};
        REQUIRE( a.to_list() == baseline );
    }
    WHEN("parameter") {
        coek::Variable v("v");
        coek::Expression a = v;
        coek::Parameter p(0,"p");
        a += p;
        static std::list<std::string> baseline = {"[", "+", "v", "p", "]"};
        REQUIRE( a.to_list() == baseline );
    }
    WHEN("index parameter") {
        coek::Variable v("v");
        coek::Expression a = v;
        coek::IndexParameter p("p");
        a += p;
        static std::list<std::string> baseline = {"[", "+", "v", "p", "]"};
        REQUIRE( a.to_list() == baseline );
    }
  }

  SECTION( "minus-equal" ) {
    WHEN("variable") {
        coek::Variable v("v");
        coek::Variable p("p",0,1,0);
        coek::Expression a = v;
        a -= p;
        static std::list<std::string> baseline = {"[", "+", "v", "[", "-", "p", "]", "]"};
        REQUIRE( a.to_list() == baseline );
    }
    WHEN("double") {
        coek::Variable v("v");
        coek::Expression a = v;
        double p=1;
        a -= p;
        static std::list<std::string> baseline = {"[", "+", "v", "[", "-", "1.000", "]", "]"};
        REQUIRE( a.to_list() == baseline );
    }
    WHEN("int") {
        coek::Variable v("v");
        coek::Expression a = v;
        int p=1;
        a -= p;
        static std::list<std::string> baseline = {"[", "+", "v", "[", "-", "1.000", "]", "]"};
        REQUIRE( a.to_list() == baseline );
    }
    WHEN("parameter") {
        coek::Variable v("v");
        coek::Expression a = v;
        coek::Parameter p(0,"p");
        a -= p;
        static std::list<std::string> baseline = {"[", "+", "v", "[", "-",  "p", "]", "]"};
        REQUIRE( a.to_list() == baseline );
    }
    WHEN("index parameter") {
        coek::Variable v("v");
        coek::Expression a = v;
        coek::IndexParameter p("p");
        a -= p;
        static std::list<std::string> baseline = {"[", "+", "v", "[", "-", "p", "]", "]"};
        REQUIRE( a.to_list() == baseline );
    }
  }

  SECTION( "times-equal" ) {
    WHEN("variable") {
        coek::Variable v("v");
        coek::Variable p("p",0,1,0);
        coek::Expression a = v;
        a *= p;
        static std::list<std::string> baseline = {"[", "*", "v", "p", "]"};
        REQUIRE( a.to_list() == baseline );
    }
    WHEN("double") {
        coek::Variable v("v");
        coek::Expression a = v;
        double p=1;
        a *= p;
        static std::list<std::string> baseline = {"[", "*", "v", "1.000", "]"};
        REQUIRE( a.to_list() == baseline );
    }
    WHEN("int") {
        coek::Variable v("v");
        coek::Expression a = v;
        int p=1;
        a *= p;
        static std::list<std::string> baseline = {"[", "*", "v", "1.000", "]"};
        REQUIRE( a.to_list() == baseline );
    }
    WHEN("parameter") {
        coek::Variable v("v");
        coek::Expression a = v;
        coek::Parameter p(0,"p");
        a *= p;
        static std::list<std::string> baseline = {"[", "*", "v", "p", "]"};
        REQUIRE( a.to_list() == baseline );
    }
    WHEN("index parameter") {
        coek::Variable v("v");
        coek::Expression a = v;
        coek::IndexParameter p("p");
        a *= p;
        static std::list<std::string> baseline = {"[", "*", "v", "p", "]"};
        REQUIRE( a.to_list() == baseline );
    }
  }

  SECTION( "divide-equal" ) {
    WHEN("variable") {
        coek::Variable v("v");
        coek::Variable p("p",0,1,0);
        coek::Expression a = v;
        a /= p;
        static std::list<std::string> baseline = {"[", "/", "v", "p", "]"};
        REQUIRE( a.to_list() == baseline );
    }
    WHEN("double") {
        coek::Variable v("v");
        coek::Expression a = v;
        double p=1;
        a /= p;
        static std::list<std::string> baseline = {"[", "/", "v", "1.000", "]"};
        REQUIRE( a.to_list() == baseline );
    }
    WHEN("int") {
        coek::Variable v("v");
        coek::Expression a = v;
        int p=1;
        a /= p;
        static std::list<std::string> baseline = {"[", "/", "v", "1.000", "]"};
        REQUIRE( a.to_list() == baseline );
    }
    WHEN("parameter") {
        coek::Variable v("v");
        coek::Expression a = v;
        coek::Parameter p(0,"p");
        a /= p;
        static std::list<std::string> baseline = {"[", "/", "v", "p", "]"};
        REQUIRE( a.to_list() == baseline );
    }
    WHEN("index parameter") {
        coek::Variable v("v");
        coek::Expression a = v;
        coek::IndexParameter p("p");
        a /= p;
        static std::list<std::string> baseline = {"[", "/", "v", "p", "]"};
        REQUIRE( a.to_list() == baseline );
    }
  }

  SECTION( "collect_terms" ) {
    coek::QuadraticExpr repn;
    coek::Model m;
    coek::Variable v = m.add_variable("v",0,0,0,false,false);
    coek::Expression a = 1 + v + v*v;

    repn.collect_terms(a);
    std::stringstream sstr;
    sstr << repn;
    REQUIRE( sstr.str() == "Constant: 1\nLinear: \n1 v\nQuadratic: \n1 v v\n");
  }

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}


/*
TEST_CASE( "capi_errors", "[smoke]" ) {
    SECTION( "parameter" ) {
        REQUIRE_THROWS_AS( create_parameter_int(0, 0, true, "q"), std::runtime_error );
        REQUIRE_THROWS_AS( create_parameter_double(0, 0, true, "Q"), std::runtime_error );
        REQUIRE_THROWS_AS( get_parameter_zero(0), std::runtime_error );
        REQUIRE_THROWS_AS( get_parameter_one(0), std::runtime_error );
        REQUIRE_THROWS_AS( get_parameter_negative_one(0), std::runtime_error );
    }

    SECTION( "variable" ) {
        REQUIRE_THROWS_AS( create_variable(0, false, false, 0.0, 1.0, 0.0, "a"), std::runtime_error );
        REQUIRE_THROWS_AS( create_variable_array(0, 0, 0, false, false, 0.0, 1.0, 0.0, "A"), std::runtime_error );
    }
}
*/


TEST_CASE( "model_unary_expression", "[smoke]" ) {

  SECTION( "positive" ) {
    WHEN( "param" ) {
        coek::Parameter q(1.0, "q");
        coek::Expression e;
        e = +q;
        static std::list<std::string> baseline = {"q"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "iparam" ) {
        coek::IndexParameter q("q");
        coek::Expression e;
        e = +q;
        static std::list<std::string> baseline = {"q"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "var" ) {
        coek::Variable v("v",0,1,0);
        coek::Expression e;
        e = +v;
        static std::list<std::string> baseline = {"v"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "expr" ) {
        coek::Expression E(-1);
        coek::Expression e;
        e = +E;
        static std::list<std::string> baseline = {"-1.000"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "negate" ) {
    WHEN( "param" ) {
        coek::Parameter q(1.0, "q");
        coek::Expression e;
        e = -q;
        static std::list<std::string> baseline = {"[", "-", "q", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.get_value() == -1 );
    }

    WHEN( "iparam" ) {
        coek::IndexParameter q("q");
        coek::Expression e;
        e = -q;
        static std::list<std::string> baseline = {"[", "-", "q", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "var" ) {
        coek::Variable v("v",0,1,2);
        coek::Expression e;
        e = -v;
        static std::list<std::string> baseline = {"[", "*", "-1", "v", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.get_value() == -2 );
    }

    WHEN( "expr" ) {
        coek::Expression E(-1);
        coek::Expression e;
        e = -E;
        static std::list<std::string> baseline = {"1.000"};
        REQUIRE( e.to_list() == baseline );
    }
  }

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}


TEST_CASE( "model_add_expression", "[smoke]" ) {

{
coek::Variable a("a",0.0, 1.0, 3.0, false, false);
coek::Variable b("b",0.0, 1.0, 5.0, false, false);
coek::Variable c("c", 0.0, 1.0, 0.0, false, false);
coek::Variable d("d",0.0, 1.0, 0.0, false, false);
coek::Parameter z(0.0, "z");
coek::IndexParameter Z("Z");
coek::Expression E = b;

  SECTION( "Test simple" ) {
    WHEN("e = a + b") {
        coek::Expression e = a + b;

        static std::list<std::string> baseline = {"[", "+", "a", "b", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.get_value() == 8 );
    }

    WHEN("e = a+b + 2*a") {
        coek::Expression e1 = a + b;
        coek::Expression e2 = 2 * a;
        coek::Expression e = e1 + e2;

        static std::list<std::string> baseline = { "[", "+", "a", "b", "[", "*", "2", "a", "]", "]" };
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test const + value" ) {
    WHEN( "e = 5 + a" ) {
        coek::Expression e = 5 + a;

        static std::list<std::string> baseline = {"[", "+", "5.000", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0 + a" ) {
        coek::Expression e = 5.0 + a;

        static std::list<std::string> baseline = {"[", "+", "5.000", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5 + z" ) {
        coek::Expression e = 5 + z;

        static std::list<std::string> baseline = {"[", "+", "5.000", "z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0 + z" ) {
        coek::Expression e = 5.0 + z;

        static std::list<std::string> baseline = {"[", "+", "5.000", "z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5 + Z" ) {
        coek::Expression e = 5 + Z;

        static std::list<std::string> baseline = {"[", "+", "5.000", "Z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0 + Z" ) {
        coek::Expression e = 5.0 + Z;

        static std::list<std::string> baseline = {"[", "+", "5.000", "Z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5 + E" ) {
        coek::Expression e = 5 + E;

        static std::list<std::string> baseline = {"[", "+", "5.000", "b", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0 + E" ) {
        coek::Expression e = 5.0 + E;

        static std::list<std::string> baseline = {"[", "+", "5.000", "b", "]"};
        REQUIRE( e.to_list() == baseline );
    }

  }

  SECTION( "Test var + value" ) {
    WHEN( "e = a + 5" ) {
        coek::Expression e = a + 5;

        static std::list<std::string> baseline = {"[", "+", "a", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a + 5.0" ) {
        coek::Expression e = a + 5.0;

        static std::list<std::string> baseline = {"[", "+", "a", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a + a" ) {
        coek::Expression e = a + a;

        static std::list<std::string> baseline = {"[", "+", "a", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a + z" ) {
        coek::Expression e = a + z;

        static std::list<std::string> baseline = {"[", "+", "a", "z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a + Z" ) {
        coek::Expression e = a + Z;

        static std::list<std::string> baseline = {"[", "+", "a", "Z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a + E" ) {
        coek::Expression e = a + E;

        static std::list<std::string> baseline = {"[", "+", "a", "b", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test param + value" ) {
    WHEN( "e = z + 5" ) {
        coek::Expression e = z + 5;

        static std::list<std::string> baseline = {"[", "+", "z", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z + 5.0" ) {
        coek::Expression e = z + 5.0;

        static std::list<std::string> baseline = {"[", "+", "z", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z + a" ) {
        coek::Expression e = z + a;

        static std::list<std::string> baseline = {"[", "+", "z", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z + z" ) {
        coek::Expression e = z + z;

        static std::list<std::string> baseline = {"[", "+", "z", "z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z + Z" ) {
        coek::Expression e = z + Z;

        static std::list<std::string> baseline = {"[", "+", "z", "Z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z + E" ) {
        coek::Expression e = z + E;

        static std::list<std::string> baseline = {"[", "+", "z", "b", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test indexparam + value" ) {
    WHEN( "e = Z + 5" ) {
        coek::Expression e = Z + 5;

        static std::list<std::string> baseline = {"[", "+", "Z", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z + 5.0" ) {
        coek::Expression e = Z + 5.0;

        static std::list<std::string> baseline = {"[", "+", "Z", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z + a" ) {
        coek::Expression e = Z + a;

        static std::list<std::string> baseline = {"[", "+", "Z", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z + z" ) {
        coek::Expression e = Z + z;

        static std::list<std::string> baseline = {"[", "+", "Z", "z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z + Z" ) {
        coek::Expression e = Z + Z;

        static std::list<std::string> baseline = {"[", "+", "Z", "Z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z + E" ) {
        coek::Expression e = Z + E;

        static std::list<std::string> baseline = {"[", "+", "Z", "b", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test expr + value" ) {
    WHEN( "e = E + 5" ) {
        coek::Expression e = E + 5;

        static std::list<std::string> baseline = {"[", "+", "b", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E + 5.0" ) {
        coek::Expression e = E + 5.0;

        static std::list<std::string> baseline = {"[", "+", "b", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E + a" ) {
        coek::Expression e = E + a;

        static std::list<std::string> baseline = {"[", "+", "b", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E + z" ) {
        coek::Expression e = E + z;

        static std::list<std::string> baseline = {"[", "+", "b", "z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E + Z" ) {
        coek::Expression e = E + Z;

        static std::list<std::string> baseline = {"[", "+", "b", "Z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E + E" ) {
        coek::Expression e = E + E;

        static std::list<std::string> baseline = {"[", "+", "b", "b", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test nestedSum" ) {

    WHEN( "e = (a+b)+5" ) {
        coek::Expression e = (a+b)+5;

        static std::list<std::string> baseline = {"[", "+", "a", "b", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5+(a+b)" ) {
        coek::Expression e = 5+(a+b);

        static std::list<std::string> baseline = {"[", "+", "5.000", "[", "+", "a", "b", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = (a+b)+5.0" ) {
        coek::Expression e = (a+b)+5.0;

        static std::list<std::string> baseline = {"[", "+", "a", "b", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0+(a+b)" ) {
        coek::Expression e = 5.0+(a+b);

        static std::list<std::string> baseline = {"[", "+", "5.000", "[", "+", "a", "b", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = (a+b)+c" ) {
        coek::Expression e = (a+b)+c;

        static std::list<std::string> baseline = {"[", "+", "a", "b", "c", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = c+(a+b)" ) {
        coek::Expression e = c+(a+b);

        static std::list<std::string> baseline = {"[", "+", "c", "[", "+", "a", "b", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = (a+b)+(c+d)" ) {
        coek::Expression e = (a+b)+(c+d);

        static std::list<std::string> baseline = {"[", "+", "a", "b", "[", "+", "c", "d", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test nestedSum" ) {

    WHEN( "e = 2*(a+b)+c" ) {
        coek::Expression e = 2*(a+b)+c;

        static std::list<std::string> baseline = {"[", "+", "[", "*", "2.000", "[", "+", "a", "b", "]", "]", "c", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 3*(2*(a+b)+c)" ) {
        coek::Expression e = 3*(2*(a+b)+c);

        static std::list<std::string> baseline = {"[", "*", "3.000", "[", "+", "[", "*", "2.000", "[", "+", "a", "b", "]", "]", "c", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test trivialSum" ) {
    coek::Parameter q(1, "q");
    coek::Parameter Q(0, "Q");
    coek::IndexParameter Z("Z");
    coek::Expression E = a;

    WHEN( "e = 0 + a" ) {
        coek::Expression e = 0 + a;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0.0 + a" ) {
        coek::Expression e = 0.0 + a;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0 + q" ) {
        coek::Expression e = 0 + q;

        static std::list<std::string> baseline = {"q"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0.0 + q" ) {
        coek::Expression e = 0.0 + q;

        static std::list<std::string> baseline = {"q"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0 + Z" ) {
        coek::Expression e = 0 + Z;

        static std::list<std::string> baseline = {"Z"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0.0 + Z" ) {
        coek::Expression e = 0.0 + Z;

        static std::list<std::string> baseline = {"Z"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0 + E" ) {
        coek::Expression e = 0 + E;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0.0 + E" ) {
        coek::Expression e = 0.0 + E;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a + 0" ) {
        coek::Expression e = a + 0;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a + 0.0" ) {
        coek::Expression e = a + 0.0;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = q + 0" ) {
        coek::Expression e = q + 0;

        static std::list<std::string> baseline = {"q"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = q + 0.0" ) {
        coek::Expression e = q + 0.0;

        static std::list<std::string> baseline = {"q"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z + 0" ) {
        coek::Expression e = Z + 0;

        static std::list<std::string> baseline = {"Z"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z + 0.0" ) {
        coek::Expression e = Z + 0.0;

        static std::list<std::string> baseline = {"Z"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E + 0" ) {
        coek::Expression e = E + 0;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E + 0.0" ) {
        coek::Expression e = E + 0.0;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test sum with zero-valued parameter" ) {
    coek::Parameter q(1, "q");
    coek::Parameter Q(0, "Q");

    WHEN( "e = a + q{0}" ) {
        coek::Expression e = a + q;

        static std::list<std::string> baseline = {"[","+","a","q","]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = q{0} + a" ) {
        coek::Expression e = q + a;

        static std::list<std::string> baseline = {"[","+","q","a","]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a + Q{0.0}" ) {
        coek::Expression e = a + Q;

        static std::list<std::string> baseline = {"[","+","a","Q","]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Q{0.0} + a" ) {
        coek::Expression e = Q + a;

        static std::list<std::string> baseline = {"[","+","Q","a","]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test nestedTrivialProduct" ) {

    WHEN( "e = a*5 + b" ) {
        coek::Expression e = a*5 + b;

        static std::list<std::string> baseline = {"[", "+", "[", "*", "5", "a", "]", "b", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = b + a*5" ) {
        coek::Expression e = b + a*5;

        static std::list<std::string> baseline = {"[", "+", "b", "[", "*", "5", "a", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a*5 + (b+c)" ) {
        coek::Expression e = a*5 + (b+c);

        static std::list<std::string> baseline = { "[", "+", "[", "*", "5", "a", "]", "[", "+", "b", "c", "]", "]" };
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = (b+c) + a*5" ) {
        coek::Expression e = (b+c) + a*5;

        static std::list<std::string> baseline = { "[", "+", "b", "c", "[", "*", "5", "a", "]", "]" };
        REQUIRE( e.to_list() == baseline );
    }
  }

}

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}


TEST_CASE( "model_minus_expression", "[smoke]" ) {

{
coek::Variable a("a",0.0, 1.0, 3.0, false, false);
coek::Variable b("b",0.0, 1.0, 5.0, false, false);
coek::Variable c("c",0.0, 1.0, 0.0, false, false);
coek::Variable d("d",0.0, 1.0, 0.0, false, false);

coek::Parameter z(1.0, "z");
coek::IndexParameter Z("Z");
coek::Expression f;
coek::Expression E = b;

  SECTION( "Test simpleDiff" ) {
    coek::Expression e = a - b;

    static std::list<std::string> baseline = { "[", "+", "a", "[", "*", "-1", "b", "]", "]" };
    REQUIRE( e.to_list() == baseline );
    REQUIRE( e.get_value() == -2 );
  }

  SECTION( "Test termDiff" ) {
    coek::Expression e = 5 - 2*a;

    static std::list<std::string> baseline = { "[", "+", "5.000", "[", "*", "-2", "a", "]", "]" };
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test const - value" ) {
    WHEN( "e = 5 - a" ) {
        coek::Expression e = 5 - a;

        static std::list<std::string> baseline = {"[", "+", "5.000", "[", "*", "-1", "a", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0 - a" ) {
        coek::Expression e = 5.0 - a;

        static std::list<std::string> baseline = {"[", "+", "5.000", "[", "*", "-1", "a", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5 - z" ) {
        coek::Expression e = 5 - z;

        static std::list<std::string> baseline = {"[", "+", "5.000", "[", "-", "z", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0 - z" ) {
        coek::Expression e = 5.0 - z;

        static std::list<std::string> baseline = {"[", "+", "5.000", "[", "-", "z", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5 - Z" ) {
        coek::Expression e = 5 - Z;

        static std::list<std::string> baseline = {"[", "+", "5.000", "[", "-", "Z", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0 - Z" ) {
        coek::Expression e = 5.0 - Z;

        static std::list<std::string> baseline = {"[", "+", "5.000", "[", "-", "Z", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5 - E" ) {
        coek::Expression e = 5 - E;

        static std::list<std::string> baseline = {"[", "+", "5.000", "[", "*", "-1", "b", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0 - E" ) {
        coek::Expression e = 5.0 - E;

        static std::list<std::string> baseline = {"[", "+", "5.000", "[", "*", "-1", "b", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

  }

  SECTION( "Test var - value" ) {
    WHEN( "e = a - 5" ) {
        coek::Expression e = a - 5;

        static std::list<std::string> baseline = {"[", "+", "a", "-5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a - 5.0" ) {
        coek::Expression e = a - 5.0;

        static std::list<std::string> baseline = {"[", "+", "a", "-5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a - a" ) {
        coek::Expression e = a - a;

        static std::list<std::string> baseline = {"[", "+", "a", "[", "*", "-1", "a", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a - z" ) {
        coek::Expression e = a - z;

        static std::list<std::string> baseline = {"[", "+", "a", "[", "-", "z", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a - Z" ) {
        coek::Expression e = a - Z;

        static std::list<std::string> baseline = {"[", "+", "a", "[", "-", "Z", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a - E" ) {
        coek::Expression e = a - E;

        static std::list<std::string> baseline = {"[", "+", "a", "[", "*", "-1", "b", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test param - value" ) {
    WHEN( "e = z - 5" ) {
        coek::Expression e = z - 5;

        static std::list<std::string> baseline = {"[", "+", "z", "-5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z - 5.0" ) {
        coek::Expression e = z - 5.0;

        static std::list<std::string> baseline = {"[", "+", "z", "-5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z - a" ) {
        coek::Expression e = z - a;

        static std::list<std::string> baseline = {"[", "+", "z", "[", "*", "-1", "a", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z - z" ) {
        coek::Expression e = z - z;

        static std::list<std::string> baseline = {"[", "+", "z", "[", "-", "z", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z - Z" ) {
        coek::Expression e = z - Z;

        static std::list<std::string> baseline = {"[", "+", "z", "[", "-", "Z", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z - E" ) {
        coek::Expression e = z - E;

        static std::list<std::string> baseline = {"[", "+", "z", "[", "*", "-1", "b", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test indexparam - value" ) {
    WHEN( "e = Z - 5" ) {
        coek::Expression e = Z - 5;

        static std::list<std::string> baseline = {"[", "+", "Z", "-5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z - 5.0" ) {
        coek::Expression e = Z - 5.0;

        static std::list<std::string> baseline = {"[", "+", "Z", "-5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z - a" ) {
        coek::Expression e = Z - a;

        static std::list<std::string> baseline = {"[", "+", "Z", "[", "*", "-1", "a", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z - z" ) {
        coek::Expression e = Z - z;

        static std::list<std::string> baseline = {"[", "+", "Z", "[", "-", "z", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z - Z" ) {
        coek::Expression e = Z - Z;

        static std::list<std::string> baseline = {"[", "+", "Z", "[", "-", "Z", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z - E" ) {
        coek::Expression e = Z - E;

        static std::list<std::string> baseline = {"[", "+", "Z", "[", "*", "-1", "b", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test expr - value" ) {
    WHEN( "e = E - 5" ) {
        coek::Expression e = E - 5;

        static std::list<std::string> baseline = {"[", "+", "b", "-5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E - 5.0" ) {
        coek::Expression e = E - 5.0;

        static std::list<std::string> baseline = {"[", "+", "b", "-5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E - a" ) {
        coek::Expression e = E - a;

        static std::list<std::string> baseline = {"[", "+", "b", "[", "*", "-1", "a", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E - z" ) {
        coek::Expression e = E - z;

        static std::list<std::string> baseline = {"[", "+", "b", "[", "-", "z", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E - Z" ) {
        coek::Expression e = E - Z;

        static std::list<std::string> baseline = {"[", "+", "b", "[", "-", "Z", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E - E" ) {
        coek::Expression e = E - E;

        static std::list<std::string> baseline = {"[", "+", "b", "[", "*", "-1", "b", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test nestedDiff" ) {

    WHEN( "e = (a-b)-5" ) {
      coek::Expression e = (a-b) - 5;

      static std::list<std::string> baseline = { "[", "+", "a", "[", "*", "-1", "b", "]", "-5.000", "]" };
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5-(a-b)" ) {
      coek::Expression e = 5 - (a-b);

      static std::list<std::string> baseline = { "[", "+", "5.000", "[", "-", "[", "+", "a", "[", "*", "-1", "b", "]", "]", "]", "]" };
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = (a-b)-c" ) {
      coek::Expression e = (a-b) - c;

      static std::list<std::string> baseline = { "[", "+", "a", "[", "*", "-1", "b", "]", "[", "*", "-1", "c", "]", "]" };
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = (a-b)-(c-d)" ) {
      coek::Expression e = (a-b) - (c - d);

      static std::list<std::string> baseline = { "[", "+", "a", "[", "*", "-1", "b", "]", "[", "-", "[", "+", "c", "[", "*", "-1", "d", "]", "]", "]", "]" };
      REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test sumOf_nestedTrivialProduct2" ) {

    WHEN( "a*5 - b" ) {
      coek::Expression e = a*5 - b;
      
      static std::list<std::string> baseline = { "[", "+", "[", "*", "5", "a", "]", "[", "*", "-1", "b", "]", "]" };
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "b - a*5" ) {
      coek::Expression e = b - a*5;
      
      static std::list<std::string> baseline = { "[", "+", "b", "[", "*", "-5", "a", "]", "]" };
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "a*5 - (b-c)" ) {
      coek::Expression e = a*5 - (b-c);
      
      static std::list<std::string> baseline = {"[", "+", "[", "*", "5", "a", "]", "[", "-", "[", "+", "b", "[", "*", "-1", "c", "]", "]", "]", "]" };
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "(b-c) - a*5" ) {
      coek::Expression e = (b-c) - a*5;
      
      static std::list<std::string> baseline = {"[", "+", "b", "[", "*", "-1", "c", "]", "[", "*", "-5", "a", "]", "]"};
      REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test trivialDiff" ) {
    coek::Parameter q(1, "q");
    coek::Parameter Q(0, "Q");
    coek::IndexParameter Z("Z");
    coek::Expression E = a;

    WHEN( "e = 0 - a" ) {
        coek::Expression e = 0 - a;

        static std::list<std::string> baseline = {"[", "*", "-1", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0.0 - a" ) {
        coek::Expression e = 0.0 - a;

        static std::list<std::string> baseline = {"[", "*", "-1", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0 - q" ) {
        coek::Expression e = 0 - q;

        static std::list<std::string> baseline = {"[", "-", "q", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0.0 - q" ) {
        coek::Expression e = 0.0 - q;

        static std::list<std::string> baseline = {"[", "-", "q", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0 - Z" ) {
        coek::Expression e = 0 - Z;

        static std::list<std::string> baseline = {"[", "-", "Z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0.0 - Z" ) {
        coek::Expression e = 0.0 - Z;

        static std::list<std::string> baseline = {"[", "-", "Z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0 - E" ) {
        coek::Expression e = 0 - E;

        static std::list<std::string> baseline = {"[", "*", "-1", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0.0 - E" ) {
        coek::Expression e = 0.0 - E;

        static std::list<std::string> baseline = {"[", "*", "-1", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a - 0" ) {
        coek::Expression e = a - 0;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a - 0.0" ) {
        coek::Expression e = a - 0.0;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = q - 0" ) {
        coek::Expression e = q - 0;

        static std::list<std::string> baseline = {"q"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = q - 0.0" ) {
        coek::Expression e = q - 0.0;

        static std::list<std::string> baseline = {"q"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z - 0" ) {
        coek::Expression e = Z - 0;

        static std::list<std::string> baseline = {"Z"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z - 0.0" ) {
        coek::Expression e = Z - 0.0;

        static std::list<std::string> baseline = {"Z"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E - 0" ) {
        coek::Expression e = E - 0;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E - 0.0" ) {
        coek::Expression e = E - 0.0;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test diff with zero-valued parameter" ) {
    coek::Parameter q(0, "q");
    coek::Parameter Q(0.0, "Q");

    WHEN( "e = a - q{0}" ) {
      coek::Expression e = a - q;

      static std::list<std::string> baseline = {"[","+","a","[","-","q","]","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = q{0} - a" ) {
      coek::Expression e = q - a;

      static std::list<std::string> baseline = {"[","+","q","[","*","-1","a","]","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = q_{0} - a" ) {
      coek::Expression e = 0*q - a;

      static std::list<std::string> baseline = {"[", "+", "0.000", "[", "*", "-1", "a", "]", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a - Q{0.0}" ) {
      coek::Expression e = a - Q;

      static std::list<std::string> baseline = {"[","+","a","[","-","Q","]","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Q{0.0} - a" ) {
      coek::Expression e = Q - a;

      static std::list<std::string> baseline = {"[","+","Q","[","*","-1","a","]","]"};
      REQUIRE( e.to_list() == baseline );
    }
  }


}
#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}


TEST_CASE( "model_neg_expression", "[smoke]" ) {

  SECTION( "Test negation_param" ) {
    coek::Parameter p(2, "p");

    WHEN( "e = -p" ) {
      coek::Expression e = -p;

      static std::list<std::string> baseline = {"[", "-", "p", "]"};
      REQUIRE( e.to_list() == baseline );
      REQUIRE( e.get_value() == -2 );
    }

    WHEN( "e = -(-p)" ) {
      coek::Expression e = -(-p);

      static std::list<std::string> baseline = {"[", "-", "[", "-", "p", "]", "]"};
      REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test negation_terms" ) {
    coek::Parameter p(2, "p");
    coek::Variable v("v",0.0, 1.0, 0.0, false, false);

    WHEN( "e = - p*v" ) {
      coek::Expression e = - p*v;

      static std::list<std::string> baseline = {"[", "*", "[", "-", "p", "]", "v", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = -(- p*v)" ) {
      coek::Expression e = -(- p*v);

      static std::list<std::string> baseline = {"[", "-", "[", "*", "[", "-", "p", "]", "v", "]", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = - 5*v" ) {
      coek::Expression e = - 5*v;

      static std::list<std::string> baseline = {"[", "*", "-5", "v", "]"};
      REQUIRE( e.to_list() == baseline );
    }
  }

#ifdef DEBUG
  REQUIRE( coek::env.check_memory() == true );
#endif
}


TEST_CASE( "model_mul_expression", "[smoke]" ) {

{
coek::Variable a("a", 0.0, 1.0, 3.0, false, false);
coek::Variable b("b", 0.0, 1.0, 5.0, false, false);
coek::Variable c("c", 0.0, 1.0, 0.0, false, false);
coek::Variable d("d", 0.0, 1.0, 0.0, false, false);

coek::Parameter z(0.0, "z");
coek::IndexParameter Z("Z");
coek::Expression f;
coek::Expression E = b;

  SECTION( "Test simpleProduct" ) {
    coek::Expression e = a*b;

    static std::list<std::string> baseline = {"[", "*", "a", "b", "]"};
    REQUIRE( e.to_list() == baseline );
    REQUIRE( e.get_value() == 15 );
  }

  SECTION( "Test const * value" ) {
    WHEN( "e = 5 * a" ) {
        coek::Expression e = 5 * a;

        static std::list<std::string> baseline = {"[", "*", "5", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0 * a" ) {
        coek::Expression e = 5.0 * a;

        static std::list<std::string> baseline = {"[", "*", "5", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5 * z" ) {
        coek::Expression e = 5 * z;

        static std::list<std::string> baseline = {"[", "*", "5.000", "z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0 * z" ) {
        coek::Expression e = 5.0 * z;

        static std::list<std::string> baseline = {"[", "*", "5.000", "z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5 * Z" ) {
        coek::Expression e = 5 * Z;

        static std::list<std::string> baseline = {"[", "*", "5.000", "Z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0 * Z" ) {
        coek::Expression e = 5.0 * Z;

        static std::list<std::string> baseline = {"[", "*", "5.000", "Z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5 * E" ) {
        coek::Expression e = 5 * E;

        static std::list<std::string> baseline = {"[", "*", "5.000", "b", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0 * E" ) {
        coek::Expression e = 5.0 * E;

        static std::list<std::string> baseline = {"[", "*", "5.000", "b", "]"};
        REQUIRE( e.to_list() == baseline );
    }

  }

  SECTION( "Test var * value" ) {
    WHEN( "e = a * 5" ) {
        coek::Expression e = a * 5;

        static std::list<std::string> baseline = {"[", "*", "5", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a * 5.0" ) {
        coek::Expression e = a * 5.0;

        static std::list<std::string> baseline = {"[", "*", "5", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a * a" ) {
        coek::Expression e = a * a;

        static std::list<std::string> baseline = {"[", "*", "a", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a * z" ) {
        coek::Expression e = a * z;

        static std::list<std::string> baseline = {"[", "*", "a", "z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a * Z" ) {
        coek::Expression e = a * Z;

        static std::list<std::string> baseline = {"[", "*", "a", "Z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a * E" ) {
        coek::Expression e = a * E;

        static std::list<std::string> baseline = {"[", "*", "a", "b", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test param * value" ) {
    WHEN( "e = z * 5" ) {
        coek::Expression e = z * 5;

        static std::list<std::string> baseline = {"[", "*", "z", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z * 5.0" ) {
        coek::Expression e = z * 5.0;

        static std::list<std::string> baseline = {"[", "*", "z", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z * a" ) {
        coek::Expression e = z * a;

        static std::list<std::string> baseline = {"[", "*", "z", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z * z" ) {
        coek::Expression e = z * z;

        static std::list<std::string> baseline = {"[", "*", "z", "z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z * Z" ) {
        coek::Expression e = z * Z;

        static std::list<std::string> baseline = {"[", "*", "z", "Z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z * E" ) {
        coek::Expression e = z * E;

        static std::list<std::string> baseline = {"[", "*", "z", "b", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test indexparam * value" ) {
    WHEN( "e = Z * 5" ) {
        coek::Expression e = Z * 5;

        static std::list<std::string> baseline = {"[", "*", "Z", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z * 5.0" ) {
        coek::Expression e = Z * 5.0;

        static std::list<std::string> baseline = {"[", "*", "Z", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z * a" ) {
        coek::Expression e = Z * a;

        static std::list<std::string> baseline = {"[", "*", "Z", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z * z" ) {
        coek::Expression e = Z * z;

        static std::list<std::string> baseline = {"[", "*", "Z", "z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z * Z" ) {
        coek::Expression e = Z * Z;

        static std::list<std::string> baseline = {"[", "*", "Z", "Z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z * E" ) {
        coek::Expression e = Z * E;

        static std::list<std::string> baseline = {"[", "*", "Z", "b", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test expr * value" ) {
    WHEN( "e = E * 5" ) {
        coek::Expression e = E * 5;

        static std::list<std::string> baseline = {"[", "*", "b", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E * 5.0" ) {
        coek::Expression e = E * 5.0;

        static std::list<std::string> baseline = {"[", "*", "b", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E * a" ) {
        coek::Expression e = E * a;

        static std::list<std::string> baseline = {"[", "*", "b", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E * z" ) {
        coek::Expression e = E * z;

        static std::list<std::string> baseline = {"[", "*", "b", "z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E * Z" ) {
        coek::Expression e = E * Z;

        static std::list<std::string> baseline = {"[", "*", "b", "Z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E * E" ) {
        coek::Expression e = E * E;

        static std::list<std::string> baseline = {"[", "*", "b", "b", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test nestedProduct" ) {
    WHEN( "e = (a*b)*5" ) {
      coek::Expression e = (a*b)*5;

      static std::list<std::string> baseline = {"[", "*", "[", "*", "a", "b", "]", "5.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5*(a*b)" ) {
      coek::Expression e = 5*(a*b);

      static std::list<std::string> baseline = {"[", "*", "5.000", "[", "*", "a", "b", "]", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = (a*b)*c" ) {
      coek::Expression e = (a*b)*c;

      static std::list<std::string> baseline = {"[", "*", "[", "*", "a", "b", "]", "c", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = c*(a*b)" ) {
      coek::Expression e = c*(a*b);

      static std::list<std::string> baseline = {"[", "*", "c", "[", "*", "a", "b", "]", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = (a*b)*(c*d)" ) {
      coek::Expression e = (a*b)*(c*d);

      static std::list<std::string> baseline = {"[", "*", "[", "*", "a", "b", "]", "[", "*", "c", "d", "]", "]"};
      REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test nestedProduct2" ) {

    WHEN( "e = (c+(a+b)) * ((a+b)+d)" ) {
      coek::Expression e = (c+(a+b))*((a+b)+d);

      static std::list<std::string> baseline = {"[", "*", "[", "+", "c", "[", "+", "a", "b", "]", "]", "[", "+", "a", "b", "d", "]", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = (c*(a+b)) * ((a+b)*d)" ) {
      coek::Expression e = (c*(a+b))*((a+b)*d);

      static std::list<std::string> baseline = {"[", "*", "[", "*", "c", "[", "+", "a", "b", "]", "]", "[", "*", "[", "+", "a", "b", "]", "d", "]", "]"};
      REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test nestedProduct3" ) {

    WHEN( "e = (3*b)*5" ) {
      coek::Expression e = (3*b)*5;

      static std::list<std::string> baseline = {"[", "*", "[", "*", "3", "b", "]", "5.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = (a*b)*5" ) {
      coek::Expression e = (a*b)*5;

      static std::list<std::string> baseline = {"[", "*", "[", "*", "a", "b", "]", "5.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5*(3*b)" ) {
      coek::Expression e = 5*(3*b);

      static std::list<std::string> baseline = {"[", "*", "5.000", "[", "*", "3", "b", "]", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5*(a*b)" ) {
      coek::Expression e = 5*(a*b);

      static std::list<std::string> baseline = {"[", "*", "5.000", "[", "*", "a", "b", "]", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = (3*b)*c" ) {
      coek::Expression e = (3*b)*c;

      static std::list<std::string> baseline = {"[", "*", "[", "*", "3", "b", "]", "c", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = c*(a*b)" ) {
      coek::Expression e = c*(a*b);

      static std::list<std::string> baseline = {"[", "*", "c", "[", "*", "a", "b", "]", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = (a*b)*(c*d)" ) {
      coek::Expression e = (a*b)*(c*d);

      static std::list<std::string> baseline = {"[", "*", "[", "*", "a", "b", "]", "[", "*", "c", "d", "]", "]"};
      REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test trivialMul - 0" ) {
    coek::Parameter q(1, "q");
    coek::Parameter Q(0, "Q");
    coek::IndexParameter Z("Z");
    coek::Expression E = a;

    WHEN( "e = 0 * a" ) {
        coek::Expression e = 0 * a;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0.0 * a" ) {
        coek::Expression e = 0.0 * a;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0 * q" ) {
        coek::Expression e = 0 * q;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0.0 * q" ) {
        coek::Expression e = 0.0 * q;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0 * Z" ) {
        coek::Expression e = 0 * Z;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0.0 * Z" ) {
        coek::Expression e = 0.0 * Z;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0 * E" ) {
        coek::Expression e = 0 * E;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0.0 * E" ) {
        coek::Expression e = 0.0 * E;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a * 0" ) {
        coek::Expression e = a * 0;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a * 0.0" ) {
        coek::Expression e = a * 0.0;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = q * 0" ) {
        coek::Expression e = q * 0;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = q * 0.0" ) {
        coek::Expression e = q * 0.0;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z * 0" ) {
        coek::Expression e = Z * 0;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z * 0.0" ) {
        coek::Expression e = Z * 0.0;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E * 0" ) {
        coek::Expression e = E * 0;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E * 0.0" ) {
        coek::Expression e = E * 0.0;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test trivialMul - 1" ) {
    coek::Parameter q(1, "q");
    coek::Parameter Q(0, "Q");
    coek::IndexParameter Z("Z");
    coek::Expression E = a;

    WHEN( "e = 1 * a" ) {
        coek::Expression e = 1 * a;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 1.0 * a" ) {
        coek::Expression e = 1.0 * a;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 1 * q" ) {
        coek::Expression e = 1 * q;

        static std::list<std::string> baseline = {"q"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 1.0 * q" ) {
        coek::Expression e = 1.0 * q;

        static std::list<std::string> baseline = {"q"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 1 * Z" ) {
        coek::Expression e = 1 * Z;

        static std::list<std::string> baseline = {"Z"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 1.0 * Z" ) {
        coek::Expression e = 1.0 * Z;

        static std::list<std::string> baseline = {"Z"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 1 * E" ) {
        coek::Expression e = 1 * E;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 1.0 * E" ) {
        coek::Expression e = 1.0 * E;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a * 1" ) {
        coek::Expression e = a * 1;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a * 1.0" ) {
        coek::Expression e = a * 1.0;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = q * 1" ) {
        coek::Expression e = q * 1;

        static std::list<std::string> baseline = {"q"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = q * 1.0" ) {
        coek::Expression e = q * 1.0;

        static std::list<std::string> baseline = {"q"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z * 1" ) {
        coek::Expression e = Z * 1;

        static std::list<std::string> baseline = {"Z"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z * 1.0" ) {
        coek::Expression e = Z * 1.0;

        static std::list<std::string> baseline = {"Z"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E * 1" ) {
        coek::Expression e = E * 1;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E * 1.0" ) {
        coek::Expression e = E * 1.0;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test mul with trivial parameter" ) {
    coek::Parameter q(0, "q");
    coek::Parameter Q(0.0, "Q");
    coek::Parameter r(1, "r");
    coek::Parameter R(1.0, "R");

    WHEN( "e = a * q{0}" ) {
      coek::Expression e = a * q;

      static std::list<std::string> baseline = {"[","*","a","q","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = q{0} * a" ) {
      coek::Expression e = q * a;

      static std::list<std::string> baseline = {"[","*","q","a","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a * Q{0.0}" ) {
      coek::Expression e = a * Q;

      static std::list<std::string> baseline = {"[","*","a","Q","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Q{0.0} * a" ) {
      coek::Expression e = Q * a;

      static std::list<std::string> baseline = {"[","*","Q","a","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a * r{1}" ) {
      coek::Expression e = a * r;

      static std::list<std::string> baseline = {"[","*","a","r","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = r{1} * a" ) {
      coek::Expression e = r * a;

      static std::list<std::string> baseline = {"[","*","r","a","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a * R{1.0}" ) {
      coek::Expression e = a * R;

      static std::list<std::string> baseline = {"[","*","a","R","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = R{1.0} * a" ) {
      coek::Expression e = R * a;

      static std::list<std::string> baseline = {"[","*","R","a","]"};
      REQUIRE( e.to_list() == baseline );
    }
  }

}
#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}


TEST_CASE( "model_div_expression", "[smoke]" ) {

{
coek::Variable a("a", 0.0, 1.0, 3.0, false, false);
coek::Variable b("b", 0.0, 1.0, 5.0, false, false);
coek::Variable c("c", 0.0, 1.0, 0.0, false, false);
coek::Variable d("d", 0.0, 1.0, 0.0, false, false);

coek::Parameter z(0.0, "z");
coek::IndexParameter Z("Z");
coek::Expression f;
coek::Expression E = b;

  SECTION( "Test simpleDivision" ) {
    coek::Expression e = a/b;

    static std::list<std::string> baseline = {"[", "/", "a", "b", "]"};
    REQUIRE( e.to_list() == baseline );
    REQUIRE( e.get_value() == 0.6 );
  }

  SECTION( "Test const / value" ) {
    WHEN( "e = 5 / a" ) {
        coek::Expression e = 5 / a;

        static std::list<std::string> baseline = {"[", "/", "5.000", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0 / a" ) {
        coek::Expression e = 5.0 / a;

        static std::list<std::string> baseline = {"[", "/", "5.000", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5 / z" ) {
        coek::Expression e = 5 / z;

        static std::list<std::string> baseline = {"[", "/", "5.000", "z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0 / z" ) {
        coek::Expression e = 5.0 / z;

        static std::list<std::string> baseline = {"[", "/", "5.000", "z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5 / Z" ) {
        coek::Expression e = 5 / Z;

        static std::list<std::string> baseline = {"[", "/", "5.000", "Z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0 / Z" ) {
        coek::Expression e = 5.0 / Z;

        static std::list<std::string> baseline = {"[", "/", "5.000", "Z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5 / E" ) {
        coek::Expression e = 5 / E;

        static std::list<std::string> baseline = {"[", "/", "5.000", "b", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0 / E" ) {
        coek::Expression e = 5.0 / E;

        static std::list<std::string> baseline = {"[", "/", "5.000", "b", "]"};
        REQUIRE( e.to_list() == baseline );
    }

  }

  SECTION( "Test var / value" ) {
    WHEN( "e = a / 5" ) {
        coek::Expression e = a / 5;

        static std::list<std::string> baseline = {"[", "*", "0.2", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a / 5.0" ) {
        coek::Expression e = a / 5.0;

        static std::list<std::string> baseline = {"[", "*", "0.2", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a / a" ) {
        coek::Expression e = a / a;

        static std::list<std::string> baseline = {"[", "/", "a", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a / z" ) {
        coek::Expression e = a / z;

        static std::list<std::string> baseline = {"[", "/", "a", "z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a / Z" ) {
        coek::Expression e = a / Z;

        static std::list<std::string> baseline = {"[", "/", "a", "Z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a / E" ) {
        coek::Expression e = a / E;

        static std::list<std::string> baseline = {"[", "/", "a", "b", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test param / value" ) {
    WHEN( "e = z / 5" ) {
        coek::Expression e = z / 5;

        static std::list<std::string> baseline = {"[", "/", "z", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z / 5.0" ) {
        coek::Expression e = z / 5.0;

        static std::list<std::string> baseline = {"[", "/", "z", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z / a" ) {
        coek::Expression e = z / a;

        static std::list<std::string> baseline = {"[", "/", "z", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z / z" ) {
        coek::Expression e = z / z;

        static std::list<std::string> baseline = {"[", "/", "z", "z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z / Z" ) {
        coek::Expression e = z / Z;

        static std::list<std::string> baseline = {"[", "/", "z", "Z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z / E" ) {
        coek::Expression e = z / E;

        static std::list<std::string> baseline = {"[", "/", "z", "b", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test indexparam / value" ) {
    WHEN( "e = Z / 5" ) {
        coek::Expression e = Z / 5;

        static std::list<std::string> baseline = {"[", "/", "Z", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z / 5.0" ) {
        coek::Expression e = Z / 5.0;

        static std::list<std::string> baseline = {"[", "/", "Z", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z / a" ) {
        coek::Expression e = Z / a;

        static std::list<std::string> baseline = {"[", "/", "Z", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z / z" ) {
        coek::Expression e = Z / z;

        static std::list<std::string> baseline = {"[", "/", "Z", "z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z / Z" ) {
        coek::Expression e = Z / Z;

        static std::list<std::string> baseline = {"[", "/", "Z", "Z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z / E" ) {
        coek::Expression e = Z / E;

        static std::list<std::string> baseline = {"[", "/", "Z", "b", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test expr / value" ) {
    WHEN( "e = E / 5" ) {
        coek::Expression e = E / 5;

        static std::list<std::string> baseline = {"[", "/", "b", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E / 5.0" ) {
        coek::Expression e = E / 5.0;

        static std::list<std::string> baseline = {"[", "/", "b", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E / a" ) {
        coek::Expression e = E / a;

        static std::list<std::string> baseline = {"[", "/", "b", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E / z" ) {
        coek::Expression e = E / z;

        static std::list<std::string> baseline = {"[", "/", "b", "z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E / Z" ) {
        coek::Expression e = E / Z;

        static std::list<std::string> baseline = {"[", "/", "b", "Z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E / E" ) {
        coek::Expression e = E / E;

        static std::list<std::string> baseline = {"[", "/", "b", "b", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test nested Division" ) {

    WHEN( "e = (3*b)/5" ) {
      coek::Expression e = (3*b)/5;

      static std::list<std::string> baseline = {"[", "/", "[", "*", "3", "b", "]", "5.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = (a/b)/5" ) {
      coek::Expression e = (a/b)/5;

      static std::list<std::string> baseline = {"[", "/", "[", "/", "a", "b", "]", "5.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5/(a/b)" ) {
      coek::Expression e = 5/(a/b);

      static std::list<std::string> baseline = {"[", "/", "5.000", "[", "/", "a", "b", "]", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = (a/b)/c" ) {
      coek::Expression e = (a/b)/c;

      static std::list<std::string> baseline = {"[", "/", "[", "/", "a", "b", "]", "c", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = c/(a/b)" ) {
      coek::Expression e = c/(a/b);

      static std::list<std::string> baseline = {"[", "/", "c", "[", "/", "a", "b", "]", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = (a/b)/(c/d)" ) {
      coek::Expression e = (a/b)/(c/d);

      static std::list<std::string> baseline = {"[", "/", "[", "/", "a", "b", "]", "[", "/", "c", "d", "]", "]"};
      REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test trivialDiv - 0" ) {
    coek::Parameter q(1, "q");
    coek::IndexParameter Z("Z");
    coek::Expression E = a;

    WHEN( "e = 0 / a" ) {
        coek::Expression e = 0 / a;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0.0 / a" ) {
        coek::Expression e = 0.0 / a;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0 / q" ) {
        coek::Expression e = 0 / q;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0.0 / q" ) {
        coek::Expression e = 0.0 / q;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0 / Z" ) {
        coek::Expression e = 0 / Z;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0.0 / Z" ) {
        coek::Expression e = 0.0 / Z;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0 / E" ) {
        coek::Expression e = 0 / E;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0.0 / E" ) {
        coek::Expression e = 0.0 / E;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a / 0" ) {
      REQUIRE_THROWS_WITH( a/0,
        "Division by zero.");
    }

    WHEN( "e = a / 0.0" ) {
      REQUIRE_THROWS_WITH( a/0.0,
        "Division by zero.");
    }

    WHEN( "e = q / 0" ) {
      REQUIRE_THROWS_WITH( q/0,
        "Division by zero.");
    }

    WHEN( "e = q / 0.0" ) {
      REQUIRE_THROWS_WITH( q/0.0,
        "Division by zero.");
    }

    WHEN( "e = Z / 0" ) {
      REQUIRE_THROWS_WITH( Z/0,
        "Division by zero.");
    }

    WHEN( "e = Z / 0.0" ) {
      REQUIRE_THROWS_WITH( Z/0.0,
        "Division by zero.");
    }

    WHEN( "e = E / 0" ) {
      REQUIRE_THROWS_WITH( E/0,
        "Division by zero.");
    }

    WHEN( "e = E / 0.0" ) {
      REQUIRE_THROWS_WITH( E/0.0,
        "Division by zero.");
    }
  }

  SECTION( "Test trivialDiv - 1" ) {
    coek::Parameter q(1, "q");
    coek::IndexParameter Z("Z");

    WHEN( "e = a/1" ) {
        coek::Expression e = a/1;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a/1.0" ) {
        coek::Expression e = a/1.0;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = q/1" ) {
        coek::Expression e = q/1;

        static std::list<std::string> baseline = {"q"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = q/1.0" ) {
        coek::Expression e = q/1.0;

        static std::list<std::string> baseline = {"q"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z/1" ) {
        coek::Expression e = Z/1;

        static std::list<std::string> baseline = {"Z"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z/1.0" ) {
        coek::Expression e = Z/1.0;

        static std::list<std::string> baseline = {"Z"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E/1" ) {
        coek::Expression e = E/1;

        static std::list<std::string> baseline = {"b"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E/1.0" ) {
        coek::Expression e = E/1.0;

        static std::list<std::string> baseline = {"b"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test div with trivial parameter" ) {

    coek::Parameter q(0, "q");
    coek::Parameter Q(0.0, "Q");
    coek::Parameter r(1, "r");
    coek::Parameter R(1.0, "R");

    WHEN( "e = q{0} / a" ) {
      coek::Expression e = q/a;

      static std::list<std::string> baseline = {"[","/","q","a","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Q{0.0} / a" ) {
      coek::Expression e = Q/a;

      static std::list<std::string> baseline = {"[","/","Q","a","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a/ r{1}" ) {
      coek::Expression e = a/r;

      static std::list<std::string> baseline = {"[","/","a","r","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a / R{1.0}" ) {
      coek::Expression e = a/R;

      static std::list<std::string> baseline = {"[","/","a","R","]"};
      REQUIRE( e.to_list() == baseline );
    }

  }

}
#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}


/*
TEST_CASE( "capi_pow_expression", "[smoke]" ) {

  apival_t a = create_variable(model, false, false, 0.0, 1.0, 0.0, "a");
  apival_t b = create_variable(model, false, false, 0.0, 1.0, 0.0, "b");

  SECTION( "Test simplePow" ) {
    apival_t e = expr_pow_expression(a,b);

    static std::list<std::string> baseline = {"[", "**", "a", "b", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test constPow" ) {

    WHEN( "e = a**5" ) {
      apival_t e = expr_pow_int(a, 5);

      static std::list<std::string> baseline = {"[", "**", "a", "5.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5**a" ) {
      apival_t e = expr_rpow_int(5, a);

      static std::list<std::string> baseline = {"[", "**", "5.000", "a", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a**5.0" ) {
      apival_t e = expr_pow_double(a, 5.0);

      static std::list<std::string> baseline = {"[", "**", "a", "5.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0**a" ) {
      apival_t e = expr_rpow_double(5.0, a);

      static std::list<std::string> baseline = {"[", "**", "5.000", "a", "]"};
      REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test trivialPow" ) {
    apival_t q = create_parameter_int(model, 0, true, "q");
    apival_t Q = create_parameter_double(model, 0.0, true, "Q");
    apival_t r = create_parameter_int(model, 1, true, "r");
    apival_t R = create_parameter_double(model, 1.0, true, "R");
    apival_t q_ = create_parameter_int(model, 0, false, "");
    apival_t Q_ = create_parameter_double(model, 0.0, false, "");
    apival_t r_ = create_parameter_int(model, 1, false, "");
    apival_t R_ = create_parameter_double(model, 1.0, false, "");

    WHEN( "e = q{0} ** a" ) {
      apival_t e = expr_pow_expression(q, a);

      static std::list<std::string> baseline = {"[","**","q","a","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = q_{0} ** a" ) {
      apival_t e = expr_pow_expression(q_, a);

      static std::list<std::string> baseline = {"0.000"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0 ** a" ) {
      apival_t e = expr_rpow_int(0, a);

      static std::list<std::string> baseline = {"0.000"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Q{0.0} ** a" ) {
      apival_t e = expr_pow_expression(Q, a);

      static std::list<std::string> baseline = {"[","**","Q","a","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Q_{0.0} ** a" ) {
      apival_t e = expr_pow_expression(Q_, a);

      static std::list<std::string> baseline = {"0.000"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0.0 ** a" ) {
      apival_t e = expr_rpow_double(0.0, a);

      static std::list<std::string> baseline = {"0.000"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = r{1} ** a" ) {
      apival_t e = expr_pow_expression(r, a);

      static std::list<std::string> baseline = {"[","**","r","a","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = r_{1} ** a" ) {
      apival_t e = expr_pow_expression(r_, a);

      static std::list<std::string> baseline = {"1"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 1 ** a" ) {
      apival_t e = expr_rpow_int(1, a);

      static std::list<std::string> baseline = {"1"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = R{1.0} ** a" ) {
      apival_t e = expr_pow_expression(R, a);

      static std::list<std::string> baseline = {"[","**","R","a","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = R_{1.0} ** a" ) {
      apival_t e = expr_pow_expression(R_, a);

      static std::list<std::string> baseline = {"1"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 1.0 ** a" ) {
      apival_t e = expr_rpow_double(1.0, a);

      static std::list<std::string> baseline = {"1"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a ** q{0}" ) {
      apival_t e = expr_pow_expression(a, q);

      static std::list<std::string> baseline = {"[","**","a","q","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a ** q_{0}" ) {
      apival_t e = expr_pow_expression(a, q_);

      static std::list<std::string> baseline = {"1"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a ** 0" ) {
      apival_t e = expr_pow_int(a, 0);

      static std::list<std::string> baseline = {"1"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a ** Q{0.0}" ) {
      apival_t e = expr_pow_expression(a, Q);

      static std::list<std::string> baseline = {"[","**","a","Q","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a ** Q_{0.0}" ) {
      apival_t e = expr_pow_expression(a, Q_);

      static std::list<std::string> baseline = {"1"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a ** 0.0" ) {
      apival_t e = expr_pow_double(a, 0.0);

      static std::list<std::string> baseline = {"1"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a ** r{1}" ) {
      apival_t e = expr_pow_expression(a, r);

      static std::list<std::string> baseline = {"[","**","a","r","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a ** r_{1}" ) {
      apival_t e = expr_pow_expression(a, r_);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a ** 1" ) {
      apival_t e = expr_pow_int(a, 1);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a ** R{1.0}" ) {
      apival_t e = expr_pow_expression(a, R);

      static std::list<std::string> baseline = {"[","**","a","R","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a ** R_{1.0}" ) {
      apival_t e = expr_pow_expression(a, R_);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a ** 1.0" ) {
      apival_t e = expr_pow_double(a, 1.0);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( e.to_list() == baseline );
    }
  }
}
*/


TEST_CASE( "intrinsics", "[smoke]" ) {

{
coek::Variable v("v", 0.0, 1.0, 0.0);
coek::Parameter p(0.0, "p");

  SECTION( "Test abs" ) {
    {
    coek::Expression constant(-1);
    REQUIRE( constant.is_constant() );
    coek::Expression e = abs(constant);
    REQUIRE( e.get_value() == 1.0 );
    }

    coek::Expression e = abs(v);
    v.set_value(1);
    REQUIRE( e.get_value() == 1.0 );
    v.set_value(-1);
    REQUIRE( e.get_value() == 1.0 );

    static std::list<std::string> baseline = {"[", "abs", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test abs - constant" ) {
    coek::Expression constant(0);
    coek::Expression e = abs(constant);
    REQUIRE( e.get_value() == 0.0 );

    static std::list<std::string> baseline = {"0.000"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test ceil" ) {
    coek::Expression e = ceil(v);
    v.set_value(1.5);
    REQUIRE( e.get_value() == 2.0 );
    v.set_value(-1.5);
    REQUIRE( e.get_value() == -1.0 );

    static std::list<std::string> baseline = {"[", "ceil", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test ceil - constant" ) {
    coek::Expression constant(1.5);
    coek::Expression e = ceil(constant);
    REQUIRE( e.get_value() == 2.0 );

    static std::list<std::string> baseline = {"2.000"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test floor" ) {
    coek::Expression e = floor(v);
    v.set_value(1.5);
    REQUIRE( e.get_value() == 1.0 );
    v.set_value(-1.5);
    REQUIRE( e.get_value() == -2.0 );

    static std::list<std::string> baseline = {"[", "floor", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test floor - constant" ) {
    coek::Expression constant(1.5);
    coek::Expression e = floor(constant);
    REQUIRE( e.get_value() == 1.0 );

    static std::list<std::string> baseline = {"1.000"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test exp" ) {
    coek::Expression e = exp(v);
    v.set_value(1);
    REQUIRE( e.get_value() == Approx(E) );
    v.set_value(0);
    REQUIRE( e.get_value() == 1.0 );

    static std::list<std::string> baseline = {"[", "exp", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test exp - constant" ) {
    coek::Expression constant(1);
    coek::Expression e = exp(constant);
    REQUIRE( e.get_value() == Approx(E) );

    static std::list<std::string> baseline = {"2.718"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test log" ) {
    coek::Expression e = log(v);
    v.set_value(1);
    REQUIRE( e.get_value() == Approx(0.0) );
    v.set_value( exp(1.0) );
    REQUIRE( e.get_value() == 1.0 );

    static std::list<std::string> baseline = {"[", "log", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test log - constant" ) {
    coek::Expression constant(1);
    coek::Expression e = log(constant);
    REQUIRE( e.get_value() == Approx(0.0) );

    static std::list<std::string> baseline = {"0.000"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test log10" ) {
    coek::Expression e = log10(v);
    v.set_value(1);
    REQUIRE( e.get_value() == Approx(0.0) );
    v.set_value(10);
    REQUIRE( e.get_value() == 1.0 );

    static std::list<std::string> baseline = {"[", "log10", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test log10 - constant" ) {
    coek::Expression constant(1);
    coek::Expression e = log(constant);
    REQUIRE( e.get_value() == Approx(0.0) );

    static std::list<std::string> baseline = {"0.000"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test pow" ) {
    {
    coek::Expression constant0(0);
    coek::Expression e = pow(constant0,p);
    p.set_value(1);
    REQUIRE( e.get_value() == Approx(0.0) );
    }
    {
    coek::Expression constant1(1);
    coek::Expression e = pow(constant1,p);
    p.set_value(1);
    REQUIRE( e.get_value() == Approx(1.0) );
    }
    {
    coek::Expression constant2(2);
    coek::Expression constant1(1);
    coek::Expression e = pow(constant2,constant1);
    REQUIRE( e.get_value() == Approx(2.0) );
    }
    {
    v.set_value(2);
    coek::Expression constant1(1);
    coek::Expression e = pow(v,constant1);
    REQUIRE( e.get_value() == Approx(2.0) );
    }

    coek::Expression e = pow(v,p);
    v.set_value(2);
    p.set_value(0);
    REQUIRE( e.get_value() == Approx(1.0) );
    p.set_value(1);
    REQUIRE( e.get_value() == Approx(2.0) );

    static std::list<std::string> baseline = {"[", "pow", "v", "p", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test pow - constant" ) {
    coek::Expression constant(0);
    coek::Expression e = pow(v,constant);
    REQUIRE( e.get_value() == Approx(1.0) );

    static std::list<std::string> baseline = {"1.000"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test sqrt" ) {
    coek::Expression e = sqrt(v);
    v.set_value(1);
    REQUIRE( e.get_value() == Approx(1.0) );
    v.set_value(4);
    REQUIRE( e.get_value() == 2.0 );

    static std::list<std::string> baseline = {"[", "sqrt", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test sqrt - constant" ) {
    coek::Expression constant(1);
    coek::Expression e = sqrt(constant);
    REQUIRE( e.get_value() == Approx(1.0) );

    static std::list<std::string> baseline = {"1.000"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test sin" ) {
    coek::Expression e = sin(v);
    v.set_value(0);
    REQUIRE( e.get_value() == Approx(0.0) );
    v.set_value(PI/2.0);
    REQUIRE( e.get_value() == Approx(1.0) );

    static std::list<std::string> baseline = {"[", "sin", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test sin - constant" ) {
    coek::Expression constant(0);
    coek::Expression e = sqrt(constant);
    REQUIRE( e.get_value() == Approx(0.0) );

    static std::list<std::string> baseline = {"0.000"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test cos" ) {
    coek::Expression e = cos(v);
    v.set_value(0);
    REQUIRE( e.get_value() == Approx(1.0) );
    v.set_value(PI/2.0);
    REQUIRE( e.get_value() == Approx(0.0).margin(1e-7) );

    static std::list<std::string> baseline = {"[", "cos", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test cos - constant" ) {
    coek::Expression constant(0);
    coek::Expression e = cos(constant);
    REQUIRE( e.get_value() == Approx(1.0) );

    static std::list<std::string> baseline = {"1.000"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test tan" ) {
    coek::Expression e = tan(v);
    v.set_value(0);
    REQUIRE( e.get_value() == Approx(0.0) );
    v.set_value(PI/4.0);
    REQUIRE( e.get_value() == Approx(1.0) );

    static std::list<std::string> baseline = {"[", "tan", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test tan - constant" ) {
    coek::Expression constant(0);
    coek::Expression e = tan(constant);
    REQUIRE( e.get_value() == Approx(0.0) );

    static std::list<std::string> baseline = {"0.000"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test asin" ) {
    coek::Expression e = asin(v);
    v.set_value(0);
    REQUIRE( e.get_value() == Approx(0.0) );
    v.set_value(1);
    REQUIRE( e.get_value() == Approx(PI/2.0) );

    static std::list<std::string> baseline = {"[", "asin", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test asin - constant" ) {
    coek::Expression constant(0);
    coek::Expression e = asin(constant);
    REQUIRE( e.get_value() == Approx(0.0) );

    static std::list<std::string> baseline = {"0.000"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test acos" ) {
    coek::Expression e = acos(v);
    v.set_value(1);
    REQUIRE( e.get_value() == Approx(0.0) );
    v.set_value(0);
    REQUIRE( e.get_value() == Approx(PI/2.0) );

    static std::list<std::string> baseline = {"[", "acos", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test acos - constant" ) {
    coek::Expression constant(1);
    coek::Expression e = acos(constant);
    REQUIRE( e.get_value() == Approx(0.0) );

    static std::list<std::string> baseline = {"0.000"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test atan" ) {
    coek::Expression e = atan(v);
    v.set_value(0);
    REQUIRE( e.get_value() == Approx(0.0) );
    v.set_value(1);
    REQUIRE( e.get_value() == Approx(PI/4.0) );

    static std::list<std::string> baseline = {"[", "atan", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test atan - constant" ) {
    coek::Expression constant(0);
    coek::Expression e = atan(constant);
    REQUIRE( e.get_value() == Approx(0.0) );

    static std::list<std::string> baseline = {"0.000"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test sinh" ) {
    coek::Expression e = sinh(v);
    v.set_value(0);
    REQUIRE( e.get_value() == Approx(0.0) );
    v.set_value(1);
    REQUIRE( e.get_value() == Approx((E-1/E)/2.0) );

    static std::list<std::string> baseline = {"[", "sinh", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test sinh - constant" ) {
    coek::Expression constant(0);
    coek::Expression e = sinh(constant);
    REQUIRE( e.get_value() == Approx(0.0) );

    static std::list<std::string> baseline = {"0.000"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test cosh" ) {
    coek::Expression e = cosh(v);
    v.set_value(0);
    REQUIRE( e.get_value() == Approx(1.0) );
    v.set_value(1);
    REQUIRE( e.get_value() == Approx((E+1/E)/2.0) );

    static std::list<std::string> baseline = {"[", "cosh", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test cosh - constant" ) {
    coek::Expression constant(0);
    coek::Expression e = cosh(constant);
    REQUIRE( e.get_value() == Approx(1.0) );

    static std::list<std::string> baseline = {"1.000"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test tanh" ) {
    coek::Expression e = tanh(v);
    v.set_value(0);
    REQUIRE( e.get_value() == Approx(0.0) );
    v.set_value(1);
    REQUIRE( e.get_value() == Approx((E-1/E)/(E+1/E)) );

    static std::list<std::string> baseline = {"[", "tanh", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test tanh - constant" ) {
    coek::Expression constant(0);
    coek::Expression e = tanh(constant);
    REQUIRE( e.get_value() == Approx(0.0) );

    static std::list<std::string> baseline = {"0.000"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test asinh" ) {
    coek::Expression e = asinh(v);
    v.set_value(0);
    REQUIRE( e.get_value() == Approx(0.0) );
    v.set_value( (E-1/E)/2.0 );
    REQUIRE( e.get_value() == Approx(1.0) );

    static std::list<std::string> baseline = {"[", "asinh", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test asinh - constant" ) {
    coek::Expression constant(0);
    coek::Expression e = asinh(constant);
    REQUIRE( e.get_value() == Approx(0.0) );

    static std::list<std::string> baseline = {"0.000"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test acosh" ) {
    coek::Expression e = acosh(v);
    v.set_value(1);
    REQUIRE( e.get_value() == Approx(0.0) );
    v.set_value( (E+1/E)/2.0 );
    REQUIRE( e.get_value() == Approx(1.0) );

    static std::list<std::string> baseline = {"[", "acosh", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test acosh - constant" ) {
    coek::Expression constant(1);
    coek::Expression e = acosh(constant);
    REQUIRE( e.get_value() == Approx(0.0) );

    static std::list<std::string> baseline = {"0.000"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test atanh" ) {
    coek::Expression e = atanh(v);
    v.set_value(0);
    REQUIRE( e.get_value() == Approx(0.0) );
    v.set_value( (E-1/E)/(E+1/E) );
    REQUIRE( e.get_value() == Approx(1.0) );

    static std::list<std::string> baseline = {"[", "atanh", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test atanh - constant" ) {
    coek::Expression constant(0);
    coek::Expression e = atanh(constant);
    REQUIRE( e.get_value() == Approx(0.0) );

    static std::list<std::string> baseline = {"0.000"};
    REQUIRE( e.to_list() == baseline );
  }
}

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}


TEST_CASE( "model_constraint", "[smoke]" ) {

{
coek::Variable v("v", 0.0, 1.0, 1.0, false, false);
coek::Parameter p(0.0, "p");
coek::IndexParameter P("P");
P.set_value(0);
coek::Expression f;

  SECTION( "lt-operator" ) {

    WHEN( "1 < p" ) {
        coek::Constraint e = 1 < p;
        static std::list<std::string> baseline = {"[", "<", "1.000", "p", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == false );
    }

    WHEN( "p < 1" ) {
        coek::Constraint e = p < 1;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "p", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == true );
    }

    WHEN( "1.0 < p" ) {
        coek::Constraint e = 1.0 < p;
        static std::list<std::string> baseline = {"[", "<", "1.000", "p", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == false );
    }

    WHEN( "p < 1.0" ) {
        coek::Constraint e = p < 1.0;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "p", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == true );
    }

    WHEN( "1 < P" ) {
        coek::Constraint e = 1 < P;
        static std::list<std::string> baseline = {"[", "<", "1.000", "P", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == false );
    }

    WHEN( "P < 1" ) {
        coek::Constraint e = P < 1;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "P", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == true );
    }

    WHEN( "1.0 < P" ) {
        coek::Constraint e = 1.0 < P;
        static std::list<std::string> baseline = {"[", "<", "1.000", "P", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == false );
    }

    WHEN( "P < 1.0" ) {
        coek::Constraint e = P < 1.0;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "P", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == true );
    }

    WHEN( "1 < v" ) {
        coek::Constraint e = 1 < v;
        static std::list<std::string> baseline = {"[", "<", "1.000", "v", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == false );
    }

    WHEN( "v < 1" ) {
        coek::Constraint e = v < 1;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "v", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == false );
    }

    WHEN( "1.0 < v" ) {
        coek::Constraint e = 1.0 < v;
        static std::list<std::string> baseline = {"[", "<", "1.000", "v", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == false );
    }

    WHEN( "v < 1.0" ) {
        coek::Constraint e = v < 1.0;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "v", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == false );
    }

    WHEN( "1 < f" ) {
        coek::Constraint e = 1 < f;
        static std::list<std::string> baseline = {"[", "<", "1.000", "0.000", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f < 1" ) {
        coek::Constraint e = f < 1;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "0.000", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1.0 < f" ) {
        coek::Constraint e = 1.0 < f;
        static std::list<std::string> baseline = {"[", "<", "1.000", "0.000", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f < 1.0" ) {
        coek::Constraint e = f < 1.0;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "0.000", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    // TODO - Error because no variables?
    WHEN( "p < p" ) {
        coek::Constraint e = p < p;
        static std::list<std::string> baseline = {"[", "<", "p", "p", "Inf","]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p < P" ) {
        coek::Constraint e = p < P;
        static std::list<std::string> baseline = {"[", "<", "p", "P", "Inf","]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P < p" ) {
        coek::Constraint e = P < p;
        static std::list<std::string> baseline = {"[", "<", "P", "p", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p < v" ) {
        coek::Constraint e = p < v;
        static std::list<std::string> baseline = {"[", "<", "p", "v", "Inf","]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v < p" ) {
        coek::Constraint e = v < p;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "v", "p", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p < f" ) {
        coek::Constraint e = p < f;
        static std::list<std::string> baseline = {"[", "<", "p", "0.000", "Inf","]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f < p" ) {
        coek::Constraint e = f < p;
        static std::list<std::string> baseline = {"[", "<", "0.000", "p", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    // TODO - Error because no variables?
    WHEN( "P < P" ) {
        coek::Constraint e = P < P;
        static std::list<std::string> baseline = {"[", "<", "P", "P", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P < v" ) {
        coek::Constraint e = P < v;
        static std::list<std::string> baseline = {"[", "<", "P", "v", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v < P" ) {
        coek::Constraint e = v < P;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "v", "P", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P < f" ) {
        coek::Constraint e = P < f;
        static std::list<std::string> baseline = {"[", "<", "P", "0.000", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f < P" ) {
        coek::Constraint e = f < P;
        static std::list<std::string> baseline = {"[", "<", "0.000", "P", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f < v" ) {
        coek::Constraint e = f < v;
        static std::list<std::string> baseline = {"[", "<", "0.000", "v", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v < f" ) {
        coek::Constraint e = v < f;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "v", "0.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v < v" ) {
        coek::Constraint e = v < v;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "[", "+", "v", "[", "*", "-1", "v", "]", "]", "0.000","]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f < f" ) {
        coek::Constraint e = f < f;
        static std::list<std::string> baseline = {"[", "<", "0.000", "0.000", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "le-operator" ) {

    WHEN( "0 <= p" ) {
        coek::Constraint e = 0 <= p;
        static std::list<std::string> baseline = {"[", "<=", "0.000", "p", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == true );
    }

    WHEN( "p <= 0" ) {
        coek::Constraint e = p <= 0;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "p", "0.000", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == true );
    }

    WHEN( "0.0 <= p" ) {
        coek::Constraint e = 0.0 <= p;
        static std::list<std::string> baseline = {"[", "<=", "0.000", "p", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == true );
    }

    WHEN( "p <= 0.0" ) {
        coek::Constraint e = p <= 0.0;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "p", "0.000", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == true );
    }

    WHEN( "1 <= p" ) {
        coek::Constraint e = 1 <= p;
        static std::list<std::string> baseline = {"[", "<=", "1.000", "p", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == false );
    }

    WHEN( "p <= 1" ) {
        coek::Constraint e = p <= 1;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "p", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == true );
    }

    WHEN( "1.0 <= p" ) {
        coek::Constraint e = 1.0 <= p;
        static std::list<std::string> baseline = {"[", "<=", "1.000", "p", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == false );
    }

    WHEN( "p <= 1.0" ) {
        coek::Constraint e = p <= 1.0;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "p", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == true );
    }

    WHEN( "1 <= P" ) {
        coek::Constraint e = 1 <= P;
        static std::list<std::string> baseline = {"[", "<=", "1.000", "P", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == false );
    }

    WHEN( "P <= 1" ) {
        coek::Constraint e = P <= 1;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "P", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == true );
    }

    WHEN( "1.0 <= P" ) {
        coek::Constraint e = 1.0 <= P;
        static std::list<std::string> baseline = {"[", "<=", "1.000", "P", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == false );
    }

    WHEN( "P <= 1.0" ) {
        coek::Constraint e = P <= 1.0;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "P", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == true );
    }

    WHEN( "1 <= v" ) {
        coek::Constraint e = 1 <= v;
        static std::list<std::string> baseline = {"[", "<=", "1.000", "v", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == true );
    }

    WHEN( "v <= 1" ) {
        coek::Constraint e = v <= 1;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "v", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == true );
    }

    WHEN( "1.0 <= v" ) {
        coek::Constraint e = 1.0 <= v;
        static std::list<std::string> baseline = {"[", "<=", "1.000", "v", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == true );
    }

    WHEN( "v <= 1.0" ) {
        coek::Constraint e = v <= 1.0;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "v", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == true );
    }

    WHEN( "1 <= f" ) {
        coek::Constraint e = 1 <= f;
        static std::list<std::string> baseline = {"[", "<=", "1.000", "0.000", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f <= 1" ) {
        coek::Constraint e = f <= 1;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "0.000", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1.0 <= f" ) {
        coek::Constraint e = 1.0 <= f;
        static std::list<std::string> baseline = {"[", "<=", "1.000", "0.000", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f <= 1.0" ) {
        coek::Constraint e = f <= 1.0;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "0.000", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p <= p" ) {
        coek::Constraint e = p <= p;
        static std::list<std::string> baseline = {"[", "<=", "p", "p", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p <= P" ) {
        coek::Constraint e = p <= P;
        static std::list<std::string> baseline = {"[", "<=", "p", "P", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P <= p" ) {
        coek::Constraint e = P <= p;
        static std::list<std::string> baseline = {"[", "<=", "P", "p", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p <= v" ) {
        coek::Constraint e = p <= v;
        static std::list<std::string> baseline = {"[", "<=", "p", "v", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v <= p" ) {
        coek::Constraint e = v <= p;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "v", "p", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p <= f" ) {
        coek::Constraint e = p <= f;
        static std::list<std::string> baseline = {"[", "<=", "p", "0.000", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f <= p" ) {
        coek::Constraint e = f <= p;
        static std::list<std::string> baseline = {"[", "<=", "0.000", "p", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P <= P" ) {
        coek::Constraint e = P <= P;
        static std::list<std::string> baseline = {"[", "<=", "P", "P", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P <= v" ) {
        coek::Constraint e = P <= v;
        static std::list<std::string> baseline = {"[", "<=", "P", "v", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v <= P" ) {
        coek::Constraint e = v <= P;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "v", "P", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P <= f" ) {
        coek::Constraint e = P <= f;
        static std::list<std::string> baseline = {"[", "<=", "P", "0.000", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f <= P" ) {
        coek::Constraint e = f <= P;
        static std::list<std::string> baseline = {"[", "<=", "0.000", "P", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f <= v" ) {
        coek::Constraint e = f <= v;
        static std::list<std::string> baseline = {"[", "<=", "0.000", "v", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v <= f" ) {
        coek::Constraint e = v <= f;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "v", "0.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v <= v" ) {
        coek::Constraint e = v <= v;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "[", "+", "v", "[", "*", "-1", "v", "]", "]", "0.000","]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f <= f" ) {
        coek::Constraint e = f <= f;
        static std::list<std::string> baseline = {"[", "<=", "0.000", "0.000", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "eq-operator" ) {

    WHEN( "0 == p" ) {
        coek::Constraint e = 0 == p;
        static std::list<std::string> baseline = {"[", "==", "p", "0.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p == 0" ) {
        coek::Constraint e = p == 0;
        static std::list<std::string> baseline = {"[", "==", "p", "0.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "0.0 == p" ) {
        coek::Constraint e = 0.0 == p;
        static std::list<std::string> baseline = {"[", "==", "p", "0.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p == 0.0" ) {
        coek::Constraint e = p == 0.0;
        static std::list<std::string> baseline = {"[", "==", "p", "0.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1 == p" ) {
        coek::Constraint e = 1 == p;
        static std::list<std::string> baseline = {"[", "==", "p", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p == 1" ) {
        coek::Constraint e = p == 1;
        static std::list<std::string> baseline = {"[", "==", "p", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1.0 == p" ) {
        coek::Constraint e = 1.0 == p;
        static std::list<std::string> baseline = {"[", "==", "p", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p == 1.0" ) {
        coek::Constraint e = p == 1.0;
        static std::list<std::string> baseline = {"[", "==", "p", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1 == P" ) {
        coek::Constraint e = 1 == P;
        static std::list<std::string> baseline = {"[", "==", "P", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P == 1" ) {
        coek::Constraint e = P == 1;
        static std::list<std::string> baseline = {"[", "==", "P", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1.0 == P" ) {
        coek::Constraint e = 1.0 == P;
        static std::list<std::string> baseline = {"[", "==", "P", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P == 1.0" ) {
        coek::Constraint e = P == 1.0;
        static std::list<std::string> baseline = {"[", "==", "P", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1 == v" ) {
        coek::Constraint e = 1 == v;
        static std::list<std::string> baseline = {"[", "==", "v", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v == 1" ) {
        coek::Constraint e = v == 1;
        static std::list<std::string> baseline = {"[", "==", "v", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1.0 == v" ) {
        coek::Constraint e = 1.0 == v;
        static std::list<std::string> baseline = {"[", "==", "v", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v == 1.0" ) {
        coek::Constraint e = v == 1.0;
        static std::list<std::string> baseline = {"[", "==", "v", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1 == f" ) {
        coek::Constraint e = 1 == f;
        static std::list<std::string> baseline = {"[", "==", "0.000", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f == 1" ) {
        coek::Constraint e = f == 1;
        static std::list<std::string> baseline = {"[", "==", "0.000", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1.0 == f" ) {
        coek::Constraint e = 1.0 == f;
        static std::list<std::string> baseline = {"[", "==", "0.000", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f == 1.0" ) {
        coek::Constraint e = f == 1.0;
        static std::list<std::string> baseline = {"[", "==", "0.000", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p == p" ) {
        coek::Constraint e = p == p;
        static std::list<std::string> baseline = {"[", "==", "p", "p", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p == P" ) {
        coek::Constraint e = p == P;
        static std::list<std::string> baseline = {"[", "==", "P", "p", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P == p" ) {
        coek::Constraint e = P == p;
        static std::list<std::string> baseline = {"[", "==", "p", "P", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p == v" ) {
        coek::Constraint e = p == v;
        static std::list<std::string> baseline = {"[", "==", "v", "p", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v == p" ) {
        coek::Constraint e = v == p;
        static std::list<std::string> baseline = {"[", "==", "v", "p", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p == f" ) {
        coek::Constraint e = p == f;
        static std::list<std::string> baseline = {"[", "==", "0.000", "p", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f == p" ) {
        coek::Constraint e = f == p;
        static std::list<std::string> baseline = {"[", "==", "p", "0.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P == P" ) {
        coek::Constraint e = P == P;
        static std::list<std::string> baseline = {"[", "==", "P", "P", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P == v" ) {
        coek::Constraint e = P == v;
        static std::list<std::string> baseline = {"[", "==", "v", "P", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v == P" ) {
        coek::Constraint e = v == P;
        static std::list<std::string> baseline = {"[", "==", "v", "P", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P == f" ) {
        coek::Constraint e = P == f;
        static std::list<std::string> baseline = {"[", "==", "0.000", "P", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f == P" ) {
        coek::Constraint e = f == P;
        static std::list<std::string> baseline = {"[", "==", "P", "0.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f == v" ) {
        coek::Constraint e = f == v;
        static std::list<std::string> baseline = {"[", "==", "v", "0.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v == f" ) {
        coek::Constraint e = v == f;
        static std::list<std::string> baseline = {"[", "==", "v", "0.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v == v" ) {
        coek::Constraint e = v == v;
        static std::list<std::string> baseline = {"[", "==", "[", "+", "v", "[", "*", "-1", "v", "]", "]", "0.000","]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f == f" ) {
        coek::Constraint e = f == f;
        static std::list<std::string> baseline = {"[", "==", "0.000", "0.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "gt-operator" ) {

    WHEN( "1 > p" ) {
        coek::Constraint e = 1 > p;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "p", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p > 1" ) {
        coek::Constraint e = p > 1;
        static std::list<std::string> baseline = {"[", "<", "1.000", "p", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1.0 > p" ) {
        coek::Constraint e = 1.0 > p;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "p", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p > 1.0" ) {
        coek::Constraint e = p > 1.0;
        static std::list<std::string> baseline = {"[", "<", "1.000", "p", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1 > P" ) {
        coek::Constraint e = 1 > P;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "P", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P > 1" ) {
        coek::Constraint e = P > 1;
        static std::list<std::string> baseline = {"[", "<", "1.000", "P", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1.0 > P" ) {
        coek::Constraint e = 1.0 > P;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "P", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P > 1.0" ) {
        coek::Constraint e = P > 1.0;
        static std::list<std::string> baseline = {"[", "<", "1.000", "P", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1 > v" ) {
        coek::Constraint e = 1 > v;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "v", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v > 1" ) {
        coek::Constraint e = v > 1;
        static std::list<std::string> baseline = {"[", "<", "1.000", "v", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1.0 > v" ) {
        coek::Constraint e = 1.0 > v;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "v", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v > 1.0" ) {
        coek::Constraint e = v > 1.0;
        static std::list<std::string> baseline = {"[", "<", "1.000", "v", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1 > f" ) {
        coek::Constraint e = 1 > f;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "0.000", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f > 1" ) {
        coek::Constraint e = f > 1;
        static std::list<std::string> baseline = {"[", "<", "1.000", "0.000", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1.0 > f" ) {
        coek::Constraint e = 1.0 > f;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "0.000", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f > 1.0" ) {
        coek::Constraint e = f > 1.0;
        static std::list<std::string> baseline = {"[", "<", "1.000", "0.000", "Inf","]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p > p" ) {
        coek::Constraint e = p > p;
        static std::list<std::string> baseline = { "[", "<", "p", "p", "Inf", "]" };
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p > P" ) {
        coek::Constraint e = p > P;
        static std::list<std::string> baseline = { "[", "<", "P", "p", "Inf", "]" };
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P > p" ) {
        coek::Constraint e = P > p;
        static std::list<std::string> baseline = { "[", "<", "p", "P", "Inf", "]" };
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p > v" ) {
        coek::Constraint e = p > v;
        static std::list<std::string> baseline = { "[", "<", "-Inf", "v", "p", "]" };
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v > p" ) {
        coek::Constraint e = v > p;
        static std::list<std::string> baseline = { "[", "<", "p", "v", "Inf", "]" };
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p > f" ) {
        coek::Constraint e = p > f;
        static std::list<std::string> baseline = {"[", "<", "0.000", "p", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f > p" ) {
        coek::Constraint e = f > p;
        static std::list<std::string> baseline = {"[", "<", "p", "0.000", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P > P" ) {
        coek::Constraint e = P > P;
        static std::list<std::string> baseline = { "[", "<", "P", "P", "Inf", "]" };
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P > v" ) {
        coek::Constraint e = P > v;
        static std::list<std::string> baseline = { "[", "<", "-Inf", "v", "P", "]" };
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v > P" ) {
        coek::Constraint e = v > P;
        static std::list<std::string> baseline = { "[", "<", "P", "v", "Inf", "]" };
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P > f" ) {
        coek::Constraint e = P > f;
        static std::list<std::string> baseline = {"[", "<", "0.000", "P", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f > P" ) {
        coek::Constraint e = f > P;
        static std::list<std::string> baseline = {"[", "<", "P", "0.000", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f > v" ) {
        coek::Constraint e = f > v;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "v", "0.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v > f" ) {
        coek::Constraint e = v > f;
        static std::list<std::string> baseline = {"[", "<", "0.000", "v", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v > v" ) {
        coek::Constraint e = v > v;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "[", "+", "v", "[", "*", "-1", "v", "]", "]", "0.000","]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f > f" ) {
        coek::Constraint e = f > f;
        static std::list<std::string> baseline = {"[", "<", "0.000", "0.000", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "ge-operator" ) {

    WHEN( "1 >= p" ) {
        coek::Constraint e = 1 >= p;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "p", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p >= 1" ) {
        coek::Constraint e = p >= 1;
        static std::list<std::string> baseline = {"[", "<=", "1.000", "p", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1.0 >= p" ) {
        coek::Constraint e = 1.0 >= p;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "p", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p >= 1.0" ) {
        coek::Constraint e = p >= 1.0;
        static std::list<std::string> baseline = {"[", "<=", "1.000", "p", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1 >= P" ) {
        coek::Constraint e = 1 >= P;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "P", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P >= 1" ) {
        coek::Constraint e = P >= 1;
        static std::list<std::string> baseline = {"[", "<=", "1.000", "P", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1.0 >= P" ) {
        coek::Constraint e = 1.0 >= P;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "P", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P >= 1.0" ) {
        coek::Constraint e = P >= 1.0;
        static std::list<std::string> baseline = {"[", "<=", "1.000", "P", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1 >= v" ) {
        coek::Constraint e = 1 >= v;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "v", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v >= 1" ) {
        coek::Constraint e = v >= 1;
        static std::list<std::string> baseline = {"[", "<=", "1.000", "v", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1.0 >= v" ) {
        coek::Constraint e = 1.0 >= v;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "v", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v >= 1.0" ) {
        coek::Constraint e = v >= 1.0;
        static std::list<std::string> baseline = {"[", "<=", "1.000", "v", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1 >= f" ) {
        coek::Constraint e = 1 >= f;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "0.000", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f >= 1" ) {
        coek::Constraint e = f >= 1;
        static std::list<std::string> baseline = {"[", "<=", "1.000", "0.000", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1.0 >= f" ) {
        coek::Constraint e = 1.0 >= f;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "0.000", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f >= 1.0" ) {
        coek::Constraint e = f >= 1.0;
        static std::list<std::string> baseline = {"[", "<=", "1.000", "0.000", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p >= p" ) {
        coek::Constraint e = p >= p;
        static std::list<std::string> baseline = { "[", "<=", "p", "p", "Inf", "]" };
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p >= P" ) {
        coek::Constraint e = p >= P;
        static std::list<std::string> baseline = {"[", "<=", "P", "p", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P >= p" ) {
        coek::Constraint e = P >= p;
        static std::list<std::string> baseline = {"[", "<=", "p", "P", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p >= v" ) {
        coek::Constraint e = p >= v;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "v", "p", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v >= p" ) {
        coek::Constraint e = v >= p;
        static std::list<std::string> baseline = {"[", "<=", "p", "v", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p >= f" ) {
        coek::Constraint e = p >= f;
        static std::list<std::string> baseline = {"[", "<=", "0.000", "p", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f >= p" ) {
        coek::Constraint e = f >= p;
        static std::list<std::string> baseline = {"[", "<=", "p", "0.000", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P >= P" ) {
        coek::Constraint e = P >= P;
        static std::list<std::string> baseline = { "[", "<=", "P", "P", "Inf", "]" };
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P >= v" ) {
        coek::Constraint e = P >= v;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "v", "P", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v >= P" ) {
        coek::Constraint e = v >= P;
        static std::list<std::string> baseline = {"[", "<=", "P", "v", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P >= f" ) {
        coek::Constraint e = P >= f;
        static std::list<std::string> baseline = {"[", "<=", "0.000", "P", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f >= P" ) {
        coek::Constraint e = f >= P;
        static std::list<std::string> baseline = {"[", "<=", "P", "0.000", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f >= v" ) {
        coek::Constraint e = f >= v;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "v", "0.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v >= f" ) {
        coek::Constraint e = v >= f;
        static std::list<std::string> baseline = {"[", "<=", "0.000", "v", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v >= v" ) {
        coek::Constraint e = v >= v;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "[", "+", "v", "[", "*", "-1", "v", "]", "]", "0.000","]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f >= f" ) {
        coek::Constraint e = f >= f;
        static std::list<std::string> baseline = {"[", "<=", "0.000", "0.000", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "constructor" ) {
    WHEN( "equal" ) {
        coek::Constraint e = v <= 0;
        coek::Constraint f = v >= 0;
        f = e;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "v", "0.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test inequality" ) {

    WHEN( "inline" ) {
        coek::Variable v(0,1,0);
        REQUIRE( (v - 1 == 0).is_inequality() == false );
        REQUIRE( (v - 1 == 0).is_equality() == true );
    }

    WHEN( "v <= 0" ) {
      coek::Constraint e = v <= 0;

      static std::list<std::string> baseline = {"[", "<=", "-Inf", "v", "0.000", "]"};
      REQUIRE( e.to_list() == baseline );
      REQUIRE( e.is_inequality() == true );
      REQUIRE( e.is_equality() == false );
      }

    WHEN( "v < 0" ) {
      coek::Constraint e = v < 0;

      static std::list<std::string> baseline = {"[", "<", "-Inf", "v", "0.000", "]"};
      REQUIRE( e.to_list() == baseline );
      REQUIRE( e.is_inequality() == true );
      REQUIRE( e.is_equality() == false );
      }

    WHEN( "v < 0.0" ) {
      coek::Constraint e = v < 0.0;

      static std::list<std::string> baseline = {"[", "<", "-Inf", "v", "0.000", "]"};
      REQUIRE( e.to_list() == baseline );
      REQUIRE( e.is_inequality() == true );
      REQUIRE( e.is_equality() == false );
      }

    WHEN( "-1 < v < 0" ) {
      coek::Constraint e = inequality(-1, v, 0, true);

      static std::list<std::string> baseline = {"[", "<", "-1.000", "v", "0.000", "]"};
      REQUIRE( e.to_list() == baseline );
      REQUIRE( e.is_inequality() == true );
      REQUIRE( e.is_equality() == false );
      }

    WHEN( "-1 <= v <= 0" ) {
      coek::Constraint e = inequality(-1, v, 0);

      static std::list<std::string> baseline = {"[", "<=", "-1.000", "v", "0.000", "]"};
      REQUIRE( e.to_list() == baseline );
      REQUIRE( e.is_inequality() == true );
      REQUIRE( e.is_equality() == false );
      }

    WHEN( "-1.0 < v < 0.0" ) {
      coek::Constraint e = inequality(-1.0, v, 0.0, true);

      static std::list<std::string> baseline = {"[", "<", "-1.000", "v", "0.000", "]"};
      REQUIRE( e.to_list() == baseline );
      REQUIRE( e.is_inequality() == true );
      REQUIRE( e.is_equality() == false );
      }

    WHEN( "-1.0 <= v <= 0.0" ) {
      coek::Constraint e = inequality(-1.0, v, 0.0);

      static std::list<std::string> baseline = {"[", "<=", "-1.000", "v", "0.000", "]"};
      REQUIRE( e.to_list() == baseline );
      REQUIRE( e.is_inequality() == true );
      REQUIRE( e.is_equality() == false );
      }

    WHEN( "v + p <= 0" ) {
      coek::Constraint e = v + p <= 0;

      static std::list<std::string> baseline = {"[", "<=", "-Inf", "[", "+", "v", "p", "]", "0.000", "]"};
      REQUIRE( e.to_list() == baseline );
      REQUIRE( e.is_inequality() == true );
      REQUIRE( e.is_equality() == false );
      }

    WHEN( "v + p < 0" ) {
      coek::Constraint e = v + p < 0;

      static std::list<std::string> baseline = {"[", "<", "-Inf", "[", "+", "v", "p", "]", "0.000", "]"};
      REQUIRE( e.to_list() == baseline );
      REQUIRE( e.is_inequality() == true );
      REQUIRE( e.is_equality() == false );
      }

    WHEN( "v + P <= 0" ) {
      coek::Constraint e = v + P <= 0;

      static std::list<std::string> baseline = {"[", "<=", "-Inf", "[", "+", "v", "P", "]", "0.000", "]"};
      REQUIRE( e.to_list() == baseline );
      REQUIRE( e.is_inequality() == true );
      REQUIRE( e.is_equality() == false );
      }

    WHEN( "v + P < 0" ) {
      coek::Constraint e = v + P < 0;

      static std::list<std::string> baseline = {"[", "<", "-Inf", "[", "+", "v", "P", "]", "0.000", "]"};
      REQUIRE( e.to_list() == baseline );
      REQUIRE( e.is_inequality() == true );
      REQUIRE( e.is_equality() == false );
      }

    }

  SECTION( "Test Equality" ) {

    WHEN( "v == 0" ) {
      coek::Constraint e = v == 0;

      static std::list<std::string> baseline = {"[", "==", "v", "0.000", "]"};
      REQUIRE( e.to_list() == baseline );
      REQUIRE( e.is_inequality() == false );
      REQUIRE( e.is_equality() == true );
      }

    WHEN( "v == 0.0" ) {
      coek::Constraint e = v == 0.0;

      static std::list<std::string> baseline = {"[", "==", "v", "0.000", "]"};
      REQUIRE( e.to_list() == baseline );
      REQUIRE( e.is_inequality() == false );
      REQUIRE( e.is_equality() == true );
      }

    WHEN( "v + p == 0" ) {
      coek::Constraint e = v + p == 0;

      static std::list<std::string> baseline = {"[", "==", "[", "+", "v", "p", "]", "0.000", "]"};
      REQUIRE( e.to_list() == baseline );
      REQUIRE( e.is_inequality() == false );
      REQUIRE( e.is_equality() == true );
      }

    WHEN( "v + P == 0" ) {
      coek::Constraint e = v + P == 0;

      static std::list<std::string> baseline = {"[", "==", "[", "+", "v", "P", "]", "0.000", "]"};
      REQUIRE( e.to_list() == baseline );
      REQUIRE( e.is_inequality() == false );
      REQUIRE( e.is_equality() == true );
      }

    }

  SECTION( "collect_terms" ) {
    coek::QuadraticExpr repn;
    coek::Model m;
    coek::Variable v = m.add_variable("v",0,1,0);
    coek::Constraint a = 0 >= 1 + v;

    repn.collect_terms(a);
    std::stringstream sstr;
    sstr << repn;
    REQUIRE( sstr.str() == "Constant: 1\nLinear: \n1 v\nQuadratic: \n");
  }

  SECTION( "Constraint Unit Tests" ) {

/*
    WHEN( "Empty" ) {
        coek::Constraint c;
        REQUIRE( c.id() == 0 );
    }
*/
  }

}
#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}


TEST_CASE( "model_compute", "[smoke]" ) {

{
coek::Variable a("a", 0.0, 1.0, 0.0, false, false);
coek::Variable b("b", 0.0, 1.0, 1.0, false, false);
coek::Parameter q(2, "q");

  SECTION( "expression" ) {

    WHEN( "e = q" ) {
        coek::Expression e = q;
        REQUIRE( e.get_value() == 2 );
    }

    WHEN( "e = a" ) {
        coek::Expression e = a;
        REQUIRE( e.get_value() == 0 );
    }

    WHEN( "e = 3*b + q" ) {
        coek::Expression e = 3*b + q;
        REQUIRE( e.get_value() == 5.0 );
    }

  }

  SECTION( "constraint" ) {

    WHEN( "e = 3*b + q == 0" ) {
        coek::Constraint e = 3*b + q == 0;
        REQUIRE( e.body().get_value() == 5.0 );
        REQUIRE( e.is_feasible() == false );
    }
  }

}
#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}


TEST_CASE( "model_setup", "[smoke]" ) {

{
coek::Model model;
coek::Variable a = model.add_variable("a",0.0, 1.0, 0.0, false, true);
coek::Variable b("b", 0.0, 1.0, 0.0, true, false);
model.add_variable(b);
coek::Parameter q(2, "q");

  SECTION( "add" ) {

    WHEN( "objective" ) {
        coek::Expression e =  3*b + q;
        REQUIRE( model.num_objectives() == 0 );
        model.add_objective( e );
        REQUIRE( model.num_objectives() == 1 );
    }

    WHEN( "inequality" ) {
        coek::Constraint c = 3*b + q <= 0;
        REQUIRE( model.num_constraints() == 0 );
        model.add_constraint(c);
        REQUIRE( model.num_constraints() == 1 );
    }

    WHEN( "equality" ) {
        coek::Constraint c = 3*b + q == 0;
        REQUIRE( model.num_constraints() == 0 );
        model.add_constraint(c);
        REQUIRE( model.num_constraints() == 1 );
    }
  }

  SECTION( "model setup" ) {
    coek::Expression e0 = 3*a + q;
    model.add_objective( e0 );
    coek::Constraint e2 = 3*b + q <= 0;
    model.add_constraint( e2 );
    coek::Constraint e3 = 3*b + q == 0;
    model.add_constraint( e3 );

    WHEN( "print (df == 0)" ) {
        std::stringstream os;
        os << model;
        std::string tmp = os.str();
        REQUIRE( tmp == "MODEL\n\
  Objectives\n\
    min( 3*a + q )\n\
  Constraints\n\
    3*b + q <= 0\n\
    3*b + q == 0\n\
");
    }
  }

/*
        REQUIRE( tmp == "MODEL\n\
  Objectives\n\
    3*b{1} + q{2}\n\
    3*a{0} + q{2}\n\
\n\
  Constraints\n\
    3*b{1} + q{2}  <=  0\n\
    3*b{1} + q{2}  ==  0\n\
" );
*/

#if 0
  SECTION( "model writing" ) {
    coek::Expression e0 = 3*a + q;
    model.add_objective( e0 );
    coek::Constraint e2 = 3*b + q - a <= 0;
    model.add_constraint( e2 );
    coek::Constraint e3 = 3*b + b == 0;
    model.add_constraint( e3 );
    coek::Constraint e4 = 3*b*a + q + b*b + b*b == 0;
    model.add_constraint( e4 );
    coek::Constraint e5 = 3*b*b + q - a*b - a*a <= 0;
    model.add_constraint( e5 );
    coek::Variable c = model.add_variable();
    c.set_lb(0.0);
    coek::Variable d = model.add_variable();
    d.set_ub(0.0);
    model.add (c + d == 0);

    model.write("testing.lp");
  }
#endif


}
#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}

#ifdef COEK_WITH_COMPACT_MODEL
TEST_CASE( "compact_model", "[smoke]" ) {

    SECTION("add_variable") {
        auto I = coek::RangeSet(0,3);
        coek::IndexParameter i("i");
        coek::CompactModel Model;
        //zzz();
        Model.add_variable(i+1, 2*i, 3*i+2, false, false, Forall(i).In(I));
        auto model = Model.expand();

        REQUIRE( model.num_variables() == 4 );
        {
        static std::list<std::string> baseline = {"[", "+", "0.000", "1.000", "]"};
        REQUIRE( model.get_variable(0).get_lb_expression().to_list() == baseline );
        }
        {
        static std::list<std::string> baseline = {"[", "+", "1.000", "1.000", "]"};
        REQUIRE( model.get_variable(1).get_lb_expression().to_list() == baseline );
        }
        {
        static std::list<std::string> baseline = {"[", "+", "2.000", "1.000", "]"};
        REQUIRE( model.get_variable(2).get_lb_expression().to_list() == baseline );
        }
        {
        static std::list<std::string> baseline = {"[", "+", "3.000", "1.000", "]"};
        REQUIRE( model.get_variable(3).get_lb_expression().to_list() == baseline );
        }
    }

    SECTION("add_objective") {
        auto I = coek::RangeSet(0,3);
        coek::IndexParameter i("i");
        coek::CompactModel Model;
        auto x = Model.add_variable("x");
        Model.add_objective(i*x, Forall(i).In(I), coek::Model::maximize);
        auto model = Model.expand();

        REQUIRE( model.num_objectives() == 4 );
        {
        static std::list<std::string> baseline = {"[", "*", "0.000", "x", "]"};
        REQUIRE( model.get_objective(0).body().to_list() == baseline );
        }
        {
        static std::list<std::string> baseline = {"[", "*", "1.000", "x", "]"};
        REQUIRE( model.get_objective(1).body().to_list() == baseline );
        }
        {
        static std::list<std::string> baseline = {"[", "*", "2.000", "x", "]"};
        REQUIRE( model.get_objective(2).body().to_list() == baseline );
        }
        {
        static std::list<std::string> baseline = {"[", "*", "3.000", "x", "]"};
        REQUIRE( model.get_objective(3).body().to_list() == baseline );
        }
    }

    SECTION("add_constraint") {
        auto I = coek::RangeSet(0,3);
        coek::IndexParameter i("i");
        coek::CompactModel Model;
        auto x = Model.add_variable("x");
        Model.add_constraint(i*x == 0, Forall(i).In(I));
        auto model = Model.expand();

        REQUIRE( model.num_constraints() == 4 );
        {
        static std::list<std::string> baseline = {"[", "==", "[", "*", "0.000", "x", "]", "0.000", "]"};
        REQUIRE( model.get_constraint(0).to_list() == baseline );
        }
        {
        static std::list<std::string> baseline = {"[", "==", "[", "*", "1.000", "x", "]", "0.000", "]"};
        REQUIRE( model.get_constraint(1).to_list() == baseline );
        }
        {
        static std::list<std::string> baseline = {"[", "==", "[", "*", "2.000", "x", "]", "0.000", "]"};
        REQUIRE( model.get_constraint(2).to_list() == baseline );
        }
        {
        static std::list<std::string> baseline = {"[", "==", "[", "*", "3.000", "x", "]", "0.000", "]"};
        REQUIRE( model.get_constraint(3).to_list() == baseline );
        }
    }

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}
#endif
