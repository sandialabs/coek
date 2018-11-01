
#include <memory>
extern "C" {
#include "coek_capi.h"
};
#include "admodel/simple.hpp"

#include "catch.hpp"

const double PI = 3.141592653589793238463;
const double E = exp(1.0);


TEST_CASE( "capi_add_expression", "[smoke]" ) {

  apival_t _model = create_model();
  std::unique_ptr<Simple_ExprModel> model( static_cast<Simple_ExprModel*>(_model) );
  ExprManager* manager = &(model.get()->manager);

  apival_t a = create_variable(false, false, 0.0, 1.0, 0.0, "a");
  apival_t b = create_variable(false, false, 0.0, 1.0, 0.0, "b");
  apival_t c = create_variable(false, false, 0.0, 1.0, 0.0, "c");
  apival_t d = create_variable(false, false, 0.0, 1.0, 0.0, "d");

  SECTION( "Test simpleSum" ) {
    apival_t e = expr_plus_expression(a, b);

    static std::list<std::string> baseline = {"[", "+", "a", "b", "]"};
    REQUIRE( manager->expr_to_list(e,false) == baseline );
  }

  SECTION( "Test simpleSum_API" ) {
    apival_t e1 = expr_plus_expression(a, b);
    apival_t e2 = expr_rtimes_int(2, a);
    apival_t e = expr_plus_expression(e1, e2);

    static std::list<std::string> baseline = {"[", "+", "[", "+", "a", "b", "]", "[", "*", "2", "a", "]", "]" };
    REQUIRE( manager->expr_to_list(e,false) == baseline );
  }

  SECTION( "Test constSum" ) {
    WHEN( "e = a + 5" ) {
      apival_t e = expr_plus_int(a, 5);

      static std::list<std::string> baseline = {"[", "+", "a", "5", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = 5 + a" ) {
      apival_t e = expr_rplus_int(5, a);

      static std::list<std::string> baseline = {"[", "+", "5", "a", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a + 5.0" ) {
      apival_t e = expr_plus_double(a, 5.0);

      static std::list<std::string> baseline = {"[", "+", "a", "5.000", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = 5.0 + a" ) {
      apival_t e = expr_rplus_double(5.0, a);

      static std::list<std::string> baseline = {"[", "+", "5.000", "a", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }
  }

  SECTION( "Test nestedSum" ) {

    WHEN( "e = (a+b)+5" ) {
      apival_t e = expr_plus_int(expr_plus_expression(a, b), 5);

      static std::list<std::string> baseline = {"[", "+", "[", "+", "a", "b", "]", "5", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = 5+(a+b)" ) {
      apival_t e = expr_rplus_int(5, expr_plus_expression(a, b));

      static std::list<std::string> baseline = {"[", "+", "5", "[", "+", "a", "b", "]", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = (a+b)+c" ) {
      apival_t e = expr_plus_expression(expr_plus_expression(a, b), c);

      static std::list<std::string> baseline = {"[", "+", "[", "+", "a", "b", "]", "c", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = c+(a+b)" ) {
      apival_t e = expr_plus_expression(c, expr_plus_expression(a, b));

      static std::list<std::string> baseline = {"[", "+", "c", "[", "+", "a", "b", "]", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = (a+b)+(c+d)" ) {
      apival_t e = expr_plus_expression(expr_plus_expression(a, b), expr_plus_expression(c, d));

      static std::list<std::string> baseline = {"[", "+", "[", "+", "a", "b", "]", "[", "+", "c", "d", "]", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

  }

  SECTION( "Test nestedSum" ) {

    WHEN( "e = 2*(a+b)+c" ) {
      apival_t e = expr_plus_expression(expr_rtimes_int(2, expr_plus_expression(a, b)), c);

      static std::list<std::string> baseline = {"[", "+", "[", "*", "2", "[", "+", "a", "b", "]", "]", "c", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = 3*(2*(a+b)+c)" ) {
      apival_t e = expr_rtimes_int(3, expr_plus_expression(expr_rtimes_int(2, expr_plus_expression(a, b)), c));

      static std::list<std::string> baseline = {"[", "*", "3", "[", "+", "[", "*", "2", "[", "+", "a", "b", "]", "]", "c", "]", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }
  }

  SECTION( "Test trivialSum" ) {
    apival_t q = create_parameter_int(0, true, "");
    apival_t Q = create_parameter_double(0.0, true, "");

    WHEN( "e = a + q{0}" ) {
      apival_t e = expr_plus_expression(a, q);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a + 0" ) {
      apival_t e = expr_plus_int(a, 0);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = q{0} + a" ) {
      apival_t e = expr_plus_expression(q, a);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = 0 + a" ) {
      apival_t e = expr_rplus_int(0, a);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a + Q{0.0}" ) {
      apival_t e = expr_plus_expression(a, Q);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a + 0.0" ) {
      apival_t e = expr_plus_double(a, 0.0);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = Q{0.0} + a" ) {
      apival_t e = expr_plus_expression(Q, a);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = 0.0 + a" ) {
      apival_t e = expr_rplus_double(0.0, a);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }
  }

  SECTION( "Test nestedTrivialProduct" ) {

    WHEN( "e = a*5 + b" ) {
      apival_t e = expr_plus_expression(expr_times_int(a,5), b);

      static std::list<std::string> baseline = {"[", "+", "[", "*", "a", "5", "]", "b", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = b + a*5" ) {
      apival_t e = expr_plus_expression(b, expr_times_int(a,5));

      static std::list<std::string> baseline = {"[", "+", "b", "[", "*", "a", "5", "]", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a*5 + (b+c)" ) {
      apival_t e = expr_plus_expression(expr_times_int(a,5), expr_plus_expression(b,c));

      static std::list<std::string> baseline = {"[", "+", "[", "*", "a", "5", "]", "[", "+", "b", "c", "]", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = (b+c) + a*5" ) {
      apival_t e = expr_plus_expression(expr_plus_expression(b,c), expr_times_int(a,5));

      static std::list<std::string> baseline = {"[", "+", "[", "+", "b", "c", "]", "[", "*", "a", "5", "]",  "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }
  }

}


TEST_CASE( "capi_diff_expression", "[smoke]" ) {

  apival_t _model = create_model();
  std::unique_ptr<Simple_ExprModel> model( static_cast<Simple_ExprModel*>(_model) );
  ExprManager* manager = &(model.get()->manager);

  apival_t a = create_variable(false, false, 0.0, 1.0, 0.0, "a");
  apival_t b = create_variable(false, false, 0.0, 1.0, 0.0, "b");
  apival_t c = create_variable(false, false, 0.0, 1.0, 0.0, "c");
  apival_t d = create_variable(false, false, 0.0, 1.0, 0.0, "d");

  SECTION( "Test simpleDiff" ) {
    apival_t e = expr_minus_expression(a, b);

    static std::list<std::string> baseline = {"[", "-", "a", "b", "]"};
    REQUIRE( manager->expr_to_list(e,false) == baseline );
  }

  SECTION( "Test constDiff" ) {

    WHEN( "e = a - 5" ) {
      apival_t e = expr_minus_int(a, 5);

      static std::list<std::string> baseline = {"[", "-", "a", "5", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
      }

    WHEN( "e = 5 - a" ) {
      apival_t e = expr_rminus_int(5, a);

      static std::list<std::string> baseline = {"[", "-", "5", "a", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
      }

    WHEN( "e = a - 5.0" ) {
      apival_t e = expr_minus_double(a, 5.0);

      static std::list<std::string> baseline = {"[", "-", "a", "5.000", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
      }

    WHEN( "e = 5.0 - a" ) {
      apival_t e = expr_rminus_double(5.0, a);


      static std::list<std::string> baseline = {"[", "-", "5.000", "a", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
      }
  }

  SECTION( "Test paramDiff" ) {
    apival_t p = create_parameter_int(5, true, "p");

    WHEN( "e = a - p" ) {
      apival_t e = expr_minus_expression(a, p);

      static std::list<std::string> baseline = {"[", "-", "a", "p", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
      }

    WHEN( "e = p - a" ) {
      apival_t e = expr_minus_expression(p, a);

      static std::list<std::string> baseline = {"[", "-", "p", "a", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
      }
  }

  SECTION( "Test termDiff" ) {
    apival_t e = expr_rminus_int(5, expr_rtimes_int(2, a) );

    static std::list<std::string> baseline = {"[", "-", "5", "[", "*", "2", "a", "]", "]"};
    REQUIRE( manager->expr_to_list(e,false) == baseline );
  }

  SECTION( "Test nestedDiff" ) {

    WHEN( "e = (a-b)-5" ) {
      apival_t e = expr_minus_int(expr_minus_expression(a, b), 5);

      static std::list<std::string> baseline = {"[", "-", "[", "-", "a", "b", "]", "5", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = 5-(a-b)" ) {
      apival_t e = expr_rminus_int(5, expr_minus_expression(a, b));

      static std::list<std::string> baseline = {"[", "-", "5", "[", "-", "a", "b", "]", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = (a-b)-c" ) {
      apival_t e = expr_minus_expression(expr_minus_expression(a, b), c);

      static std::list<std::string> baseline = {"[", "-", "[", "-", "a", "b", "]", "c", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = (a-b)-(c-d)" ) {
      apival_t e = expr_minus_expression(expr_minus_expression(a, b), expr_minus_expression(c,d));

      static std::list<std::string> baseline = {"[", "-", "[", "-", "a", "b", "]", "[", "-", "c", "d", "]", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }
  }

  SECTION( "Test sumOf_nestedTrivialProduct2" ) {

    WHEN( "a*5 - b" ) {
      apival_t e = expr_minus_expression(expr_times_int(a, 5), b);
      
      static std::list<std::string> baseline = {"[", "-", "[", "*", "a", "5", "]", "b", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "b - a*5" ) {
      apival_t e = expr_minus_expression(b, expr_times_int(a, 5));
      
      static std::list<std::string> baseline = {"[", "-", "b", "[", "*", "a", "5", "]", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "a*5 - (b-c)" ) {
      apival_t e = expr_minus_expression(expr_times_int(a, 5), expr_minus_expression(b, c));
      
      static std::list<std::string> baseline = {"[", "-", "[", "*", "a", "5", "]", "[", "-", "b", "c", "]", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "(b-c) - a*5" ) {
      apival_t e = expr_minus_expression(expr_minus_expression(b, c), expr_times_int(a, 5));
      
      static std::list<std::string> baseline = {"[", "-", "[", "-", "b", "c", "]", "[", "*", "a", "5", "]", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }
  }

  SECTION( "Test trivialDiff" ) {
    apival_t q = create_parameter_int(0, true, "");
    apival_t Q = create_parameter_double(0.0, true, "");

    WHEN( "e = a - q{0}" ) {
      apival_t e = expr_minus_expression(a, q);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a - 0" ) {
      apival_t e = expr_minus_int(a, 0);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = q{0} - a" ) {
      apival_t e = expr_minus_expression(q, a);

      static std::list<std::string> baseline = {"[", "-", "a", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = 0 - a" ) {
      apival_t e = expr_rminus_int(0, a);

      static std::list<std::string> baseline = {"[", "-", "a", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a - Q{0.0}" ) {
      apival_t e = expr_minus_expression(a, Q);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a - 0.0" ) {
      apival_t e = expr_minus_double(a, 0.0);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = Q{0.0} - a" ) {
      apival_t e = expr_minus_expression(Q, a);

      static std::list<std::string> baseline = {"[", "-", "a", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = 0.0 - a" ) {
      apival_t e = expr_rminus_double(0.0, a);

      static std::list<std::string> baseline = {"[", "-", "a", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }
  }

}


TEST_CASE( "capi_neg_expression", "[smoke]" ) {

  apival_t _model = create_model();
  std::unique_ptr<Simple_ExprModel> model( static_cast<Simple_ExprModel*>(_model) );
  ExprManager* manager = &(model.get()->manager);

  SECTION( "Test negation_param" ) {
    apival_t p = create_parameter_int(2, false, "p");

    WHEN( "e = -p" ) {
      apival_t e = expr_negate(p);

      static std::list<std::string> baseline = {"[", "-", "p", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = -(-p)" ) {
      apival_t e = expr_negate(expr_negate(p));

      static std::list<std::string> baseline = {"[", "-", "[", "-", "p", "]", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }
  }

  SECTION( "Test negation_terms" ) {
    apival_t p = create_parameter_int(2, false, "p");
    apival_t v = create_variable(false, false, 0.0, 1.0, 0.0, "v");

    WHEN( "e = - p*v" ) {
      apival_t e = expr_negate( expr_times_expression(p,v) );

      static std::list<std::string> baseline = {"[", "-", "[", "*", "p", "v", "]", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = -(- p*v)" ) {
      apival_t e = expr_negate( expr_negate( expr_times_expression(p,v) ) );

      static std::list<std::string> baseline = {"[", "-", "[", "-", "[", "*", "p", "v", "]", "]", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = - 5*v" ) {
      apival_t e = expr_negate( expr_rtimes_int(5,v) );

      static std::list<std::string> baseline = {"[", "-", "[", "*", "5", "v", "]", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = -(- q*v)" ) {
      apival_t e = expr_negate( expr_negate( expr_rtimes_int(5,v) ) );

      static std::list<std::string> baseline = {"[", "-", "[", "-", "[", "*", "5", "v", "]", "]", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }
  }

}


TEST_CASE( "capi_mul_expression", "[smoke]" ) {

  apival_t _model = create_model();
  std::unique_ptr<Simple_ExprModel> model( static_cast<Simple_ExprModel*>(_model) );
  ExprManager* manager = &(model.get()->manager);

  apival_t a = create_variable(false, false, 0.0, 1.0, 0.0, "a");
  apival_t b = create_variable(false, false, 0.0, 1.0, 0.0, "b");
  apival_t c = create_variable(false, false, 0.0, 1.0, 0.0, "c");
  apival_t d = create_variable(false, false, 0.0, 1.0, 0.0, "d");

  SECTION( "Test simpleProduct" ) {
    apival_t e = expr_times_expression(a,b);

    static std::list<std::string> baseline = {"[", "*", "a", "b", "]"};
    REQUIRE( manager->expr_to_list(e,false) == baseline );
  }

  SECTION( "Test constProduct" ) {

    WHEN( "e = a*5" ) {
      apival_t e = expr_times_int(a,5);

      static std::list<std::string> baseline = {"[", "*", "a", "5", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = 5*a" ) {
      apival_t e = expr_rtimes_int(5,a);

      static std::list<std::string> baseline = {"[", "*", "5", "a", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a*5.0" ) {
      apival_t e = expr_times_double(a,5.0);

      static std::list<std::string> baseline = {"[", "*", "a", "5.000", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = 5.0*a" ) {
      apival_t e = expr_rtimes_double(5.0,a);

      static std::list<std::string> baseline = {"[", "*", "5.000", "a", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }
  }

  SECTION( "Test nestedProduct" ) {
    WHEN( "e = (a*b)*5" ) {
      apival_t e = expr_times_int( expr_times_expression(a,b), 5 );

      static std::list<std::string> baseline = {"[", "*", "[", "*", "a", "b", "]", "5", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = 5*(a*b)" ) {
      apival_t e = expr_rtimes_int( 5, expr_times_expression(a,b) );

      static std::list<std::string> baseline = {"[", "*", "5", "[", "*", "a", "b", "]", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = (a*b)*c" ) {
      apival_t e = expr_times_expression( expr_times_expression(a,b), c );

      static std::list<std::string> baseline = {"[", "*", "[", "*", "a", "b", "]", "c", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = c*(a*b)" ) {
      apival_t e = expr_times_expression( c, expr_times_expression(a,b) );

      static std::list<std::string> baseline = {"[", "*", "c", "[", "*", "a", "b", "]", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = (a*b)*(c*d)" ) {
      apival_t e = expr_times_expression( expr_times_expression(a,b), expr_times_expression(c,d) );

      static std::list<std::string> baseline = {"[", "*", "[", "*", "a", "b", "]", "[", "*", "c", "d", "]", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }
  }

  SECTION( "Test nestedProduct2" ) {

    WHEN( "e = (c+(a+b)) * ((a+b)+d)" ) {
      apival_t e1 = expr_plus_expression(a,b);
      apival_t e = expr_times_expression( expr_plus_expression(c,e1), expr_plus_expression(e1,d) );

      static std::list<std::string> baseline = {"[", "*", "[", "+", "c", "[", "+", "a", "b", "]", "]", "[", "+", "[", "+", "a", "b", "]", "d", "]", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = (c*(a+b)) * ((a+b)*d)" ) {
      apival_t e1 = expr_plus_expression(a,b);
      apival_t e = expr_times_expression( expr_times_expression(c,e1), expr_times_expression(e1,d) );

      static std::list<std::string> baseline = {"[", "*", "[", "*", "c", "[", "+", "a", "b", "]", "]", "[", "*", "[", "+", "a", "b", "]", "d", "]", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }
  }

  SECTION( "Test nestedProduct3" ) {

    WHEN( "e = (3*b)*5" ) {
      apival_t e = expr_times_int(expr_rtimes_int(3,b), 5);

      static std::list<std::string> baseline = {"[", "*", "[", "*", "3", "b", "]", "5", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = (a*b)*5" ) {
      apival_t e = expr_times_int(expr_times_expression(a,b), 5);

      static std::list<std::string> baseline = {"[", "*", "[", "*", "a", "b", "]", "5", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = 5*(3*b)" ) {
      apival_t e = expr_rtimes_int(5, expr_rtimes_int(3,b));

      static std::list<std::string> baseline = {"[", "*", "5", "[", "*", "3", "b", "]", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = 5*(a*b)" ) {
      apival_t e = expr_rtimes_int(5, expr_times_expression(a,b));

      static std::list<std::string> baseline = {"[", "*", "5", "[", "*", "a", "b", "]", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = (3*b)*c" ) {
      apival_t e = expr_times_expression(expr_rtimes_int(3,b), c);

      static std::list<std::string> baseline = {"[", "*", "[", "*", "3", "b", "]", "c", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = c*(a*b)" ) {
      apival_t e = expr_times_expression(c, expr_times_expression(a,b));

      static std::list<std::string> baseline = {"[", "*", "c", "[", "*", "a", "b", "]", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = (a*b)*(c*d)" ) {
      apival_t e = expr_times_expression(expr_times_expression(a,b), expr_times_expression(c,d));

      static std::list<std::string> baseline = {"[", "*", "[", "*", "a", "b", "]", "[", "*", "c", "d", "]", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }
  }

  SECTION( "Test trivialProduct" ) {
    apival_t q = create_parameter_int(0, true, "");
    apival_t Q = create_parameter_double(0.0, true, "");
    apival_t r = create_parameter_int(1, true, "");
    apival_t R = create_parameter_double(1.0, true, "");
    apival_t s = create_parameter_int(-1, true, "");
    apival_t S = create_parameter_double(-1.0, true, "");

    WHEN( "e = a * q{0}" ) {
      apival_t e = expr_times_expression(a, q);

      static std::list<std::string> baseline = {"0"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a * 0" ) {
      apival_t e = expr_times_int(a, 0);

      static std::list<std::string> baseline = {"0"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = q{0} * a" ) {
      apival_t e = expr_times_expression(q, a);

      static std::list<std::string> baseline = {"0"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = 0 * a" ) {
      apival_t e = expr_rtimes_int(0, a);

      static std::list<std::string> baseline = {"0"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a * Q{0.0}" ) {
      apival_t e = expr_times_expression(a, Q);

      static std::list<std::string> baseline = {"0"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a * 0.0" ) {
      apival_t e = expr_times_double(a, 0.0);

      static std::list<std::string> baseline = {"0"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = Q{0.0} * a" ) {
      apival_t e = expr_times_expression(Q, a);

      static std::list<std::string> baseline = {"0"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = 0.0 * a" ) {
      apival_t e = expr_rtimes_double(0.0, a);

      static std::list<std::string> baseline = {"0"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a * r{1}" ) {
      apival_t e = expr_times_expression(a, r);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a * 1" ) {
      apival_t e = expr_times_int(a, 1);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = r{1} * a" ) {
      apival_t e = expr_times_expression(r, a);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = 1 * a" ) {
      apival_t e = expr_rtimes_int(1, a);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a * R{1.0}" ) {
      apival_t e = expr_times_expression(a, R);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a * 1.0" ) {
      apival_t e = expr_times_double(a, 1.0);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = R{1.0} * a" ) {
      apival_t e = expr_times_expression(R, a);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = 1.0 * a" ) {
      apival_t e = expr_rtimes_double(1.0, a);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a * s{-1}" ) {
      apival_t e = expr_times_expression(a, s);

      static std::list<std::string> baseline = {"[", "-", "a", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a * -1" ) {
      apival_t e = expr_times_int(a, -1);

      static std::list<std::string> baseline = {"[", "-", "a", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = s{-1} * a" ) {
      apival_t e = expr_times_expression(s, a);

      static std::list<std::string> baseline = {"[", "-", "a", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = -1 * a" ) {
      apival_t e = expr_rtimes_int(-1, a);

      static std::list<std::string> baseline = {"[", "-", "a", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a * S{-1.0}" ) {
      apival_t e = expr_times_expression(a, S);

      static std::list<std::string> baseline = {"[", "-", "a", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a * -1.0" ) {
      apival_t e = expr_times_double(a, -1.0);

      static std::list<std::string> baseline = {"[", "-", "a", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = S{-1.0} * a" ) {
      apival_t e = expr_times_expression(S, a);

      static std::list<std::string> baseline = {"[", "-", "a", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = -1.0 * a" ) {
      apival_t e = expr_rtimes_double(-1.0, a);

      static std::list<std::string> baseline = {"[", "-", "a", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }
  }

}


TEST_CASE( "capi_div_expression", "[smoke]" ) {

  apival_t _model = create_model();
  std::unique_ptr<Simple_ExprModel> model( static_cast<Simple_ExprModel*>(_model) );
  ExprManager* manager = &(model.get()->manager);

  apival_t a = create_variable(false, false, 0.0, 1.0, 0.0, "a");
  apival_t b = create_variable(false, false, 0.0, 1.0, 0.0, "b");
  apival_t c = create_variable(false, false, 0.0, 1.0, 0.0, "c");
  apival_t d = create_variable(false, false, 0.0, 1.0, 0.0, "d");

  SECTION( "Test simpleDivision" ) {
    apival_t e = expr_divide_expression(a,b);

    static std::list<std::string> baseline = {"[", "/", "a", "b", "]"};
    REQUIRE( manager->expr_to_list(e,false) == baseline );
  }

  SECTION( "Test constDivision" ) {

    WHEN( "e = a/5" ) {
      apival_t e = expr_divide_int(a,5);

      static std::list<std::string> baseline = {"[", "/", "a", "5", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = 5/a" ) {
      apival_t e = expr_rdivide_int(5,a);

      static std::list<std::string> baseline = {"[", "/", "5", "a", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a/5.0" ) {
      apival_t e = expr_divide_double(a,5.0);

      static std::list<std::string> baseline = {"[", "/", "a", "5.000", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = 5.0/a" ) {
      apival_t e = expr_rdivide_double(5.0,a);

      static std::list<std::string> baseline = {"[", "/", "5.000", "a", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }
  }

  SECTION( "Test constDivision" ) {

    WHEN( "e = (3*b)/5" ) {
      apival_t e = expr_divide_int(expr_rtimes_int(3,b), 5);

      static std::list<std::string> baseline = {"[", "/", "[", "*", "3", "b", "]", "5", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = (a/b)/5" ) {
      apival_t e = expr_divide_int(expr_divide_expression(a,b), 5);

      static std::list<std::string> baseline = {"[", "/", "[", "/", "a", "b", "]", "5", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = 5/(a/b)" ) {
      apival_t e = expr_rdivide_int(5, expr_divide_expression(a,b));

      static std::list<std::string> baseline = {"[", "/", "5", "[", "/", "a", "b", "]", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = (a/b)/c" ) {
      apival_t e = expr_divide_expression(expr_divide_expression(a,b), c);

      static std::list<std::string> baseline = {"[", "/", "[", "/", "a", "b", "]", "c", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = c/(a/b)" ) {
      apival_t e = expr_divide_expression(c, expr_divide_expression(a,b));

      static std::list<std::string> baseline = {"[", "/", "c", "[", "/", "a", "b", "]", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = (a/b)/(c/d)" ) {
      apival_t e = expr_divide_expression(expr_divide_expression(a,b), expr_divide_expression(c,d));

      static std::list<std::string> baseline = {"[", "/", "[", "/", "a", "b", "]", "[", "/", "c", "d", "]", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }
  }

  SECTION( "Test trivialDivision" ) {
    apival_t q = create_parameter_int(0, true, "");
    apival_t Q = create_parameter_double(0.0, true, "");
    apival_t r = create_parameter_int(1, true, "");
    apival_t R = create_parameter_double(1.0, true, "");
    apival_t s = create_parameter_int(-1, true, "");
    apival_t S = create_parameter_double(-1.0, true, "");

    WHEN( "e = q{0} / a" ) {
      apival_t e = expr_divide_expression(q, a);

      static std::list<std::string> baseline = {"0"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = 0 / a" ) {
      apival_t e = expr_rdivide_int(0, a);

      static std::list<std::string> baseline = {"0"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = Q{0.0} / a" ) {
      apival_t e = expr_divide_expression(Q, a);

      static std::list<std::string> baseline = {"0"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = 0.0 / a" ) {
      apival_t e = expr_rdivide_double(0.0, a);

      static std::list<std::string> baseline = {"0"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a/ r{1}" ) {
      apival_t e = expr_divide_expression(a, r);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a / 1" ) {
      apival_t e = expr_divide_int(a, 1);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a / R{1.0}" ) {
      apival_t e = expr_divide_expression(a, R);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a / 1.0" ) {
      apival_t e = expr_divide_double(a, 1.0);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a/ s{-1}" ) {
      apival_t e = expr_divide_expression(a, s);

      static std::list<std::string> baseline = {"[", "-", "a", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a / -1" ) {
      apival_t e = expr_divide_int(a, -1);

      static std::list<std::string> baseline = {"[", "-", "a", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a / S{-1.0}" ) {
      apival_t e = expr_divide_expression(a, S);

      static std::list<std::string> baseline = {"[", "-", "a", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a / -1.0" ) {
      apival_t e = expr_divide_double(a, -1.0);

      static std::list<std::string> baseline = {"[", "-", "a", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }
  }

}


TEST_CASE( "capi_pow_expression", "[smoke]" ) {

  apival_t _model = create_model();
  std::unique_ptr<Simple_ExprModel> model( static_cast<Simple_ExprModel*>(_model) );
  ExprManager* manager = &(model.get()->manager);

  apival_t a = create_variable(false, false, 0.0, 1.0, 0.0, "a");
  apival_t b = create_variable(false, false, 0.0, 1.0, 0.0, "b");

  SECTION( "Test simplePow" ) {
    apival_t e = expr_pow_expression(a,b);

    static std::list<std::string> baseline = {"[", "**", "a", "b", "]"};
    REQUIRE( manager->expr_to_list(e,false) == baseline );
  }

  SECTION( "Test constPow" ) {

    WHEN( "e = a**5" ) {
      apival_t e = expr_pow_int(a, 5);

      static std::list<std::string> baseline = {"[", "**", "a", "5", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = 5**a" ) {
      apival_t e = expr_rpow_int(5, a);

      static std::list<std::string> baseline = {"[", "**", "5", "a", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a**5.0" ) {
      apival_t e = expr_pow_double(a, 5.0);

      static std::list<std::string> baseline = {"[", "**", "a", "5.000", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = 5.0**a" ) {
      apival_t e = expr_rpow_double(5.0, a);

      static std::list<std::string> baseline = {"[", "**", "5.000", "a", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }
  }

  SECTION( "Test trivialPow" ) {
    apival_t q = create_parameter_int(0, true, "");
    apival_t Q = create_parameter_double(0.0, true, "");
    apival_t r = create_parameter_int(1, true, "");
    apival_t R = create_parameter_double(1.0, true, "");

    WHEN( "e = q{0} ** a" ) {
      apival_t e = expr_pow_expression(q, a);

      static std::list<std::string> baseline = {"0"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = 0 ** a" ) {
      apival_t e = expr_rpow_int(0, a);

      static std::list<std::string> baseline = {"0"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = Q{0.0} ** a" ) {
      apival_t e = expr_pow_expression(Q, a);

      static std::list<std::string> baseline = {"0"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = 0.0 ** a" ) {
      apival_t e = expr_rpow_double(0.0, a);

      static std::list<std::string> baseline = {"0"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = r{1} ** a" ) {
      apival_t e = expr_pow_expression(r, a);

      static std::list<std::string> baseline = {"1"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = 1 ** a" ) {
      apival_t e = expr_rpow_int(1, a);

      static std::list<std::string> baseline = {"1"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = R{1.0} ** a" ) {
      apival_t e = expr_pow_expression(R, a);

      static std::list<std::string> baseline = {"1"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = 1.0 ** a" ) {
      apival_t e = expr_rpow_double(1.0, a);

      static std::list<std::string> baseline = {"1"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a ** q{0}" ) {
      apival_t e = expr_pow_expression(a, q);

      static std::list<std::string> baseline = {"1"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a ** 0" ) {
      apival_t e = expr_pow_int(a, 0);

      static std::list<std::string> baseline = {"1"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a ** Q{0.0}" ) {
      apival_t e = expr_pow_expression(a, Q);

      static std::list<std::string> baseline = {"1"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a ** 0.0" ) {
      apival_t e = expr_pow_double(a, 0.0);

      static std::list<std::string> baseline = {"1"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a ** r{1}" ) {
      apival_t e = expr_pow_expression(a, r);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a ** 1" ) {
      apival_t e = expr_pow_int(a, 1);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a ** R{1.0}" ) {
      apival_t e = expr_pow_expression(a, R);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a ** 1.0" ) {
      apival_t e = expr_pow_double(a, 1.0);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }
  }

}


TEST_CASE( "capi_intrinsics", "[smoke]" ) {

  apival_t _model = create_model();
  std::unique_ptr<Simple_ExprModel> model( static_cast<Simple_ExprModel*>(_model) );
  ExprManager* manager = &(model.get()->manager);

  apival_t v = create_variable(false, false, 0.0, 1.0, 0.0, "v");
  apival_t p = create_variable(false, false, 0.0, 1.0, 0.0, "p");

  SECTION( "Test abs" ) {
    apival_t e = intrinsic_abs(v);
    variable_set_value(v, 1);
    REQUIRE( expr_get_value(e) == 1.0 );
    variable_set_value(v, -1);
    REQUIRE( expr_get_value(e) == 1.0 );

    static std::list<std::string> baseline = {"[", "abs", "v", "]"};
    REQUIRE( manager->expr_to_list(e,false) == baseline );
  }

  SECTION( "Test ceil" ) {
    apival_t e = intrinsic_ceil(v);
    variable_set_value(v, 1.5);
    REQUIRE( expr_get_value(e) == 2.0 );
    variable_set_value(v, -1.5);
    REQUIRE( expr_get_value(e) == -1.0 );

    static std::list<std::string> baseline = {"[", "ceil", "v", "]"};
    REQUIRE( manager->expr_to_list(e,false) == baseline );
  }

  SECTION( "Test floor" ) {
    apival_t e = intrinsic_floor(v);
    variable_set_value(v, 1.5);
    REQUIRE( expr_get_value(e) == 1.0 );
    variable_set_value(v, -1.5);
    REQUIRE( expr_get_value(e) == -2.0 );

    static std::list<std::string> baseline = {"[", "floor", "v", "]"};
    REQUIRE( manager->expr_to_list(e,false) == baseline );
  }

  SECTION( "Test exp" ) {
    apival_t e = intrinsic_exp(v);
    variable_set_value(v, 1);
    REQUIRE( expr_get_value(e) == Approx( E ) );
    variable_set_value(v, 0);
    REQUIRE( expr_get_value(e) == 1 );

    static std::list<std::string> baseline = {"[", "exp", "v", "]"};
    REQUIRE( manager->expr_to_list(e,false) == baseline );
  }

  SECTION( "Test log" ) {
    apival_t e = intrinsic_log(v);
    variable_set_value(v, 1);
    REQUIRE( expr_get_value(e) == Approx( 0.0 ) );
    variable_set_value(v, exp(1.0));
    REQUIRE( expr_get_value(e) == Approx( 1.0 ) );

    static std::list<std::string> baseline = {"[", "log", "v", "]"};
    REQUIRE( manager->expr_to_list(e,false) == baseline );
  }

  SECTION( "Test log10" ) {
    apival_t e = intrinsic_log10(v);
    variable_set_value(v, 1);
    REQUIRE( expr_get_value(e) == Approx( 0.0 ) );
    variable_set_value(v, 10);
    REQUIRE( expr_get_value(e) == Approx( 1.0 ) );

    static std::list<std::string> baseline = {"[", "log10", "v", "]"};
    REQUIRE( manager->expr_to_list(e,false) == baseline );
  }

  SECTION( "Test pow" ) {
    apival_t e = intrinsic_pow(v, p);
    variable_set_value(v, 2);
    variable_set_value(p, 0);
    REQUIRE( expr_get_value(e) == Approx( 1.0 ) );
    variable_set_value(p, 1);
    REQUIRE( expr_get_value(e) == Approx( 2.0 ) );

    static std::list<std::string> baseline = {"[", "**", "v", "p", "]"};
    REQUIRE( manager->expr_to_list(e,false) == baseline );
  }

  SECTION( "Test sqrt" ) {
    apival_t e = intrinsic_sqrt(v);
    variable_set_value(v, 1);
    REQUIRE( expr_get_value(e) == Approx( 1.0 ) );
    variable_set_value(v, 4);
    REQUIRE( expr_get_value(e) == Approx( 2.0 ) );

    static std::list<std::string> baseline = {"[", "sqrt", "v", "]"};
    REQUIRE( manager->expr_to_list(e,false) == baseline );
  }

  SECTION( "Test sin" ) {
    apival_t e = intrinsic_sin(v);
    variable_set_value(v, 0);
    REQUIRE( expr_get_value(e) == Approx( 0.0 ) );
    variable_set_value(v, PI/2.0);
    REQUIRE( expr_get_value(e) == Approx( 1.0 ) );

    static std::list<std::string> baseline = {"[", "sin", "v", "]"};
    REQUIRE( manager->expr_to_list(e,false) == baseline );
  }

  SECTION( "Test cos" ) {
    apival_t e = intrinsic_cos(v);
    variable_set_value(v, 0);
    REQUIRE( expr_get_value(e) == Approx( 1.0 ) );
    variable_set_value(v, PI/2.0);
    REQUIRE( expr_get_value(e) == Approx( 0.0 ).margin(1e-7) );

    static std::list<std::string> baseline = {"[", "cos", "v", "]"};
    REQUIRE( manager->expr_to_list(e,false) == baseline );
  }

  SECTION( "Test tan" ) {
    apival_t e = intrinsic_tan(v);
    variable_set_value(v, 0);
    REQUIRE( expr_get_value(e) == Approx( 0.0 ) );
    variable_set_value(v, PI/4.0);
    REQUIRE( expr_get_value(e) == Approx( 1.0 ) );

    static std::list<std::string> baseline = {"[", "tan", "v", "]"};
    REQUIRE( manager->expr_to_list(e,false) == baseline );
  }

  SECTION( "Test asin" ) {
    apival_t e = intrinsic_asin(v);
    variable_set_value(v, 0);
    REQUIRE( expr_get_value(e) == Approx( 0.0 ) );
    variable_set_value(v, 1.0);
    REQUIRE( expr_get_value(e) == Approx( PI/2.0 ) );

    static std::list<std::string> baseline = {"[", "asin", "v", "]"};
    REQUIRE( manager->expr_to_list(e,false) == baseline );
  }

  SECTION( "Test acos" ) {
    apival_t e = intrinsic_acos(v);
    variable_set_value(v, 1);
    REQUIRE( expr_get_value(e) == Approx( 0.0 ) );
    variable_set_value(v, 0);
    REQUIRE( expr_get_value(e) == Approx( PI/2.0 ) );

    static std::list<std::string> baseline = {"[", "acos", "v", "]"};
    REQUIRE( manager->expr_to_list(e,false) == baseline );
  }

  SECTION( "Test atan" ) {
    apival_t e = intrinsic_atan(v);
    variable_set_value(v, 0);
    REQUIRE( expr_get_value(e) == Approx( 0.0 ) );
    variable_set_value(v, 1);
    REQUIRE( expr_get_value(e) == Approx( PI/4.0 ) );

    static std::list<std::string> baseline = {"[", "atan", "v", "]"};
    REQUIRE( manager->expr_to_list(e,false) == baseline );
  }

  SECTION( "Test sinh" ) {
    apival_t e = intrinsic_sinh(v);
    variable_set_value(v, 0);
    REQUIRE( expr_get_value(e) == Approx( 0.0 ) );
    variable_set_value(v, 1);
    REQUIRE( expr_get_value(e) == Approx( (E-1/E)/2.0 ) );

    static std::list<std::string> baseline = {"[", "sinh", "v", "]"};
    REQUIRE( manager->expr_to_list(e,false) == baseline );
  }

  SECTION( "Test cosh" ) {
    apival_t e = intrinsic_cosh(v);
    variable_set_value(v, 0);
    REQUIRE( expr_get_value(e) == Approx( 1.0 ) );
    variable_set_value(v, 1);
    REQUIRE( expr_get_value(e) == Approx( (E+1/E)/2.0 ) );

    static std::list<std::string> baseline = {"[", "cosh", "v", "]"};
    REQUIRE( manager->expr_to_list(e,false) == baseline );
  }

  SECTION( "Test tanh" ) {
    apival_t e = intrinsic_tanh(v);
    variable_set_value(v, 0);
    REQUIRE( expr_get_value(e) == Approx( 0.0 ) );
    variable_set_value(v, 1);
    REQUIRE( expr_get_value(e) == Approx( (E-1/E)/(E+1/E) ) );

    static std::list<std::string> baseline = {"[", "tanh", "v", "]"};
    REQUIRE( manager->expr_to_list(e,false) == baseline );
  }

  SECTION( "Test asinh" ) {
    apival_t e = intrinsic_asinh(v);
    variable_set_value(v, 0);
    REQUIRE( expr_get_value(e) == Approx( 0.0 ) );
    variable_set_value(v, (E-1/E)/2.0);
    REQUIRE( expr_get_value(e) == Approx( 1.0 ) );

    static std::list<std::string> baseline = {"[", "asinh", "v", "]"};
    REQUIRE( manager->expr_to_list(e,false) == baseline );
  }

  SECTION( "Test acosh" ) {
    apival_t e = intrinsic_acosh(v);
    variable_set_value(v, 1);
    REQUIRE( expr_get_value(e) == Approx( 0.0 ) );
    variable_set_value(v, (E+1/E)/2.0);
    REQUIRE( expr_get_value(e) == Approx( 1.0 ) );

    static std::list<std::string> baseline = {"[", "acosh", "v", "]"};
    REQUIRE( manager->expr_to_list(e,false) == baseline );
  }

  SECTION( "Test atanh" ) {
    apival_t e = intrinsic_atanh(v);
    variable_set_value(v, 0);
    REQUIRE( expr_get_value(e) == Approx( 0.0 ) );
    variable_set_value(v, (E-1/E)/(E+1/E));
    REQUIRE( expr_get_value(e) == Approx( 1.0 ) );

    static std::list<std::string> baseline = {"[", "atanh", "v", "]"};
    REQUIRE( manager->expr_to_list(e,false) == baseline );
  }

}


TEST_CASE( "capi_constraint", "[smoke]" ) {

  apival_t _model = create_model();
  std::unique_ptr<Simple_ExprModel> model( static_cast<Simple_ExprModel*>(_model) );
  ExprManager* manager = &(model.get()->manager);

  apival_t v = create_variable(false, false, 0.0, 1.0, 0.0, "v");
  apival_t p = create_variable(false, false, 0.0, 1.0, 0.0, "p");

  SECTION( "Test inequality" ) {

    WHEN( "v <= 0" ) {
      apival_t e = create_inequality(v, false);

      static std::list<std::string> baseline = {"[", "<=", "v", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
      }

    WHEN( "v < 0" ) {
      apival_t e = create_inequality(v, true);

      static std::list<std::string> baseline = {"[", "<", "v", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
      }

    WHEN( "v + p <= 0" ) {
      apival_t e = create_inequality(expr_plus_expression(v,p), false);

      static std::list<std::string> baseline = {"[", "<=", "[", "+", "v", "p", "]", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
      }

    WHEN( "v + p < 0" ) {
      apival_t e = create_inequality(expr_plus_expression(v,p), true);

      static std::list<std::string> baseline = {"[", "<", "[", "+", "v", "p", "]", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
      }

    }

  SECTION( "Test Equality" ) {

    WHEN( "v == 0" ) {
      apival_t e = create_equality(v);

      static std::list<std::string> baseline = {"[", "==", "v", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
      }

    WHEN( "v + p == 0" ) {
      apival_t e = create_equality(expr_plus_expression(v,p));

      static std::list<std::string> baseline = {"[", "==", "[", "+", "v", "p", "]", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
      }

    }
}
