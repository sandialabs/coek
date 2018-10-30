
#include "context_objects.hpp"
#include "catch.hpp"

const double PI = 3.141592653589793238463;
const double E = exp(1.0);


TEST_CASE( "add_expression", "[smoke]" ) {

  ExpressionContext_Objects context;
  numval_t a = context._var(false, false, 0.0, 1.0, 0.0, "a");
  numval_t b = context._var(false, false, 0.0, 1.0, 0.0, "b");
  numval_t c = context._var(false, false, 0.0, 1.0, 0.0, "c");
  numval_t d = context._var(false, false, 0.0, 1.0, 0.0, "d");

  SECTION( "Test simpleSum" ) {
    numval_t e = context.plus(a, b);

    static std::list<std::string> baseline = {"[", "+", "a", "b", "]"};
    REQUIRE( expr_to_list(e) == baseline );
  }

  SECTION( "Test simpleSum_API" ) {
    numval_t q  = context._param(2, true);
    numval_t e1 = context.plus(a, b);
    numval_t e2 = context.times(q, a);
    numval_t e = context.plus(e1, e2);

    static std::list<std::string> baseline = {"[", "+", "[", "+", "a", "b", "]", "[", "*", "2", "a", "]", "]" };
    REQUIRE( expr_to_list(e) == baseline );
  }

  SECTION( "Test constSum" ) {
    numval_t q = context._param(5, true);
    numval_t Q = context._param(5.0, true);

    WHEN( "e = 5 + 5.0" ) {
      numval_t e = context.plus(q, Q);

      static std::list<std::string> baseline = {"10.000"};
      REQUIRE( expr_to_list(e) == baseline );
    }
    WHEN( "e = a + 5" ) {
      numval_t e = context.plus(a, q);

      static std::list<std::string> baseline = {"[", "+", "a", "5", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 5 + a" ) {
      numval_t e = context.plus(q, a);

      static std::list<std::string> baseline = {"[", "+", "5", "a", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = a + 5.0" ) {
      numval_t e = context.plus(a, Q);

      static std::list<std::string> baseline = {"[", "+", "a", "5.000", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 5.0 + a" ) {
      numval_t e = context.plus(Q, a);

      static std::list<std::string> baseline = {"[", "+", "5.000", "a", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }
  }

  SECTION( "Test nestedSum" ) {
    numval_t q = context._param(5, true);

    WHEN( "e = (a+b)+5" ) {
      numval_t e = context.plus( context.plus(a, b), q );

      static std::list<std::string> baseline = {"[", "+", "[", "+", "a", "b", "]", "5", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 5+(a+b)" ) {
      numval_t e = context.plus( q, context.plus(a, b) );

      static std::list<std::string> baseline = {"[", "+", "5", "[", "+", "a", "b", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = (a+b)+c" ) {
      numval_t e = context.plus( context.plus(a, b), c );

      static std::list<std::string> baseline = {"[", "+", "[", "+", "a", "b", "]", "c", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = c+(a+b)" ) {
      numval_t e = context.plus( c, context.plus(a, b) );

      static std::list<std::string> baseline = {"[", "+", "c", "[", "+", "a", "b", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = (a+b)+(c+d)" ) {
      numval_t e = context.plus( context.plus(a, b), context.plus(c, d) );

      static std::list<std::string> baseline = {"[", "+", "[", "+", "a", "b", "]", "[", "+", "c", "d", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

  }

  SECTION( "Test nestedSum" ) {
    numval_t q = context._param(2, true);

    WHEN( "e = 2*(a+b)+c" ) {
      numval_t e = context.plus(context.times( q, context.plus(a, b)), c );

      static std::list<std::string> baseline = {"[", "+", "[", "*", "2", "[", "+", "a", "b", "]", "]", "c", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 3*(2*(a+b)+c)" ) {
      numval_t Q = context._param(3, true);
      numval_t e = context.times(Q, context.plus(context.times(q, context.plus(a, b)), c));

      static std::list<std::string> baseline = {"[", "*", "3", "[", "+", "[", "*", "2", "[", "+", "a", "b", "]", "]", "c", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }
  }

  SECTION( "Test trivialSum" ) {

    numval_t q = context._param(0, true);
    numval_t Q = context._param(0.0, true);

    WHEN( "e = a + 0" ) {
      numval_t e = context.plus(a, q);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 0 + a" ) {
      numval_t e = context.plus(q, a);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = a + 0.0" ) {
      numval_t e = context.plus(a, Q);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 0.0 + a" ) {
      numval_t e = context.plus(Q, a);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( expr_to_list(e) == baseline );
    }
  }

  SECTION( "Test nestedTrivialProduct" ) {
    numval_t q = context._param(5, true);

    WHEN( "e = a*5 + b" ) {
      numval_t e = context.plus(context.times(a, q), b);

      static std::list<std::string> baseline = {"[", "+", "[", "*", "a", "5", "]", "b", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = b + a*5" ) {
      numval_t e = context.plus(b, context.times(a, q));

      static std::list<std::string> baseline = {"[", "+", "b", "[", "*", "a", "5", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = a*5 + (b+c)" ) {
      numval_t e = context.plus(context.times(a, q), context.plus(b,c));

      static std::list<std::string> baseline = {"[", "+", "[", "*", "a", "5", "]", "[", "+", "b", "c", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = (b+c) + a*5" ) {
      numval_t e = context.plus(context.plus(b,c), context.times(a, q));

      static std::list<std::string> baseline = {"[", "+", "[", "+", "b", "c", "]", "[", "*", "a", "5", "]",  "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }
  }

}


TEST_CASE( "diff_expression", "[smoke]" ) {

  ExpressionContext_Objects context;
  numval_t a = context._var(false, false, 0.0, 1.0, 0.0, "a");
  numval_t b = context._var(false, false, 0.0, 1.0, 0.0, "b");
  numval_t c = context._var(false, false, 0.0, 1.0, 0.0, "c");
  numval_t d = context._var(false, false, 0.0, 1.0, 0.0, "d");

  SECTION( "Test simpleDiff" ) {
    numval_t e = context.minus(a, b);

    static std::list<std::string> baseline = {"[", "-", "a", "b", "]"};
    REQUIRE( expr_to_list(e) == baseline );
  }

  SECTION( "Test constDiff" ) {
    numval_t q = context._param(5, true);
    numval_t Q = context._param(5.0, true);

    WHEN( "e = 5 - 1.0" ) {
      numval_t R = context._param(1.0, true);
      numval_t e = context.minus(q, R);

      static std::list<std::string> baseline = {"4.000"};
      REQUIRE( expr_to_list(e) == baseline );
      }

    WHEN( "e = a - 5" ) {
      numval_t e = context.minus(a, q);

      static std::list<std::string> baseline = {"[", "-", "a", "5", "]"};
      REQUIRE( expr_to_list(e) == baseline );
      }

    WHEN( "e = 5 - a" ) {
      numval_t e = context.minus(q, a);

      static std::list<std::string> baseline = {"[", "-", "5", "a", "]"};
      REQUIRE( expr_to_list(e) == baseline );
      }

    WHEN( "e = a - 5.0" ) {
      numval_t e = context.minus(a, Q);

      static std::list<std::string> baseline = {"[", "-", "a", "5.000", "]"};
      REQUIRE( expr_to_list(e) == baseline );
      }

    WHEN( "e = 5.0 - a" ) {
      numval_t e = context.minus(Q, a);

      static std::list<std::string> baseline = {"[", "-", "5.000", "a", "]"};
      REQUIRE( expr_to_list(e) == baseline );
      }
  }

  SECTION( "Test paramDiff" ) {
    numval_t p = context._param(5, true, "p");

    WHEN( "e = a - p" ) {
      numval_t e = context.minus(a, p);

      static std::list<std::string> baseline = {"[", "-", "a", "p", "]"};
      REQUIRE( expr_to_list(e) == baseline );
      }

    WHEN( "e = p - a" ) {
      numval_t e = context.minus(p, a);

      static std::list<std::string> baseline = {"[", "-", "p", "a", "]"};
      REQUIRE( expr_to_list(e) == baseline );
      }
  }

  SECTION( "Test termDiff" ) {
    numval_t q = context._param(2, true);
    numval_t Q = context._param(5, true);

    numval_t e = context.minus(Q, context.times(q, a));

    static std::list<std::string> baseline = {"[", "-", "5", "[", "*", "2", "a", "]", "]"};
    REQUIRE( expr_to_list(e) == baseline );
  }

  SECTION( "Test nestedDiff" ) {
    numval_t Q = context._param(5, true);

    WHEN( "e = (a-b)-5" ) {
      numval_t e = context.minus(context.minus(a,b), Q);

      static std::list<std::string> baseline = {"[", "-", "[", "-", "a", "b", "]", "5", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 5-(a-b)" ) {
      numval_t e = context.minus(Q, context.minus(a,b));

      static std::list<std::string> baseline = {"[", "-", "5", "[", "-", "a", "b", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = (a-b)-c" ) {
      numval_t e = context.minus(context.minus(a,b), c);

      static std::list<std::string> baseline = {"[", "-", "[", "-", "a", "b", "]", "c", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = (a-b)-(c-d)" ) {
      numval_t e = context.minus(context.minus(a,b), context.minus(c,d));

      static std::list<std::string> baseline = {"[", "-", "[", "-", "a", "b", "]", "[", "-", "c", "d", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }
  }

  SECTION( "Test sumOf_nestedTrivialProduct2" ) {
    numval_t Q = context._param(5, true);

    WHEN( "a*5 - b" ) {
      numval_t e = context.minus(context.times(a,Q), b);
      
      static std::list<std::string> baseline = {"[", "-", "[", "*", "a", "5", "]", "b", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "b - a*5" ) {
      numval_t e = context.minus(b, context.times(a,Q));
      
      static std::list<std::string> baseline = {"[", "-", "b", "[", "*", "a", "5", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "a*5 - (b-c)" ) {
      numval_t e = context.minus(context.times(a,Q), context.minus(b,c));
      
      static std::list<std::string> baseline = {"[", "-", "[", "*", "a", "5", "]", "[", "-", "b", "c", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "(b-c) - a*5" ) {
      numval_t e = context.minus(context.minus(b,c), context.times(a,Q));
      
      static std::list<std::string> baseline = {"[", "-", "[", "-", "b", "c", "]", "[", "*", "a", "5", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }
  }

  SECTION( "Test trivialDiff" ) {

    numval_t q = context._param(0, true);
    numval_t Q = context._param(0.0, true);

    WHEN( "e = a - 0" ) {
      numval_t e = context.minus(a, q);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 0 - a" ) {
      numval_t e = context.minus(q, a);

      static std::list<std::string> baseline = {"[", "-", "a", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = a - 0.0" ) {
      numval_t e = context.minus(a, Q);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 0.0 - a" ) {
      numval_t e = context.minus(Q, a);

      static std::list<std::string> baseline = {"[", "-", "a", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }
  }


}


TEST_CASE( "neg_expression", "[smoke]" ) {

  ExpressionContext_Objects context;

  SECTION( "Test negation_param" ) {
    numval_t p = context._param(2, false, "p");

    WHEN( "e = -p" ) {
      numval_t e = context.negate(p);

      static std::list<std::string> baseline = {"[", "-", "p", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = -(-p)" ) {
      numval_t e = context.negate(context.negate(p));

      static std::list<std::string> baseline = {"[", "-", "[", "-", "p", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }
  }

  SECTION( "Test negation_terms" ) {
    numval_t p = context._param(2, false, "p");
    numval_t q = context._param(5, false);
    numval_t v = context._var(false, false, 0.0, 1.0, 0.0, "v");

    WHEN( "e = - p*v" ) {
      numval_t e = context.negate( context.times(p,v) );

      static std::list<std::string> baseline = {"[", "-", "[", "*", "p", "v", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = -(- p*v)" ) {
      numval_t e = context.negate( context.negate( context.times(p,v) ) );

      static std::list<std::string> baseline = {"[", "-", "[", "-", "[", "*", "p", "v", "]", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = - 5*v" ) {
      numval_t e = context.negate( context.times(q,v) );

      static std::list<std::string> baseline = {"[", "-", "[", "*", "5", "v", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = -(- q*v)" ) {
      numval_t e = context.negate( context.negate( context.times(q,v) ) );

      static std::list<std::string> baseline = {"[", "-", "[", "-", "[", "*", "5", "v", "]", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }
  }

}


TEST_CASE( "mul_expression", "[smoke]" ) {

  ExpressionContext_Objects context;

  numval_t a = context._var(false, false, 0.0, 1.0, 0.0, "a");
  numval_t b = context._var(false, false, 0.0, 1.0, 0.0, "b");
  numval_t c = context._var(false, false, 0.0, 1.0, 0.0, "c");
  numval_t d = context._var(false, false, 0.0, 1.0, 0.0, "d");

  SECTION( "Test simpleProduct" ) {
    numval_t e = context.times(a,b);

    static std::list<std::string> baseline = {"[", "*", "a", "b", "]"};
    REQUIRE( expr_to_list(e) == baseline );
  }

  SECTION( "Test constProduct" ) {
    numval_t q = context._param(5, false);
    numval_t Q = context._param(5.0, false);

    WHEN( "e = 5*5.0" ) {
      numval_t e = context.times(q,Q);

      static std::list<std::string> baseline = {"25.000"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = a*5" ) {
      numval_t e = context.times(a,q);

      static std::list<std::string> baseline = {"[", "*", "a", "5", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 5*a" ) {
      numval_t e = context.times(q,a);

      static std::list<std::string> baseline = {"[", "*", "5", "a", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = a*5.0" ) {
      numval_t e = context.times(a,Q);

      static std::list<std::string> baseline = {"[", "*", "a", "5.000", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 5.0*a" ) {
      numval_t e = context.times(Q,a);

      static std::list<std::string> baseline = {"[", "*", "5.000", "a", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }
  }

  SECTION( "Test nestedProduct" ) {
    numval_t q = context._param(5, false);

    WHEN( "e = (a*b)*5" ) {
      numval_t e = context.times( context.times(a,b), q );

      static std::list<std::string> baseline = {"[", "*", "[", "*", "a", "b", "]", "5", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 5*(a*b)" ) {
      numval_t e = context.times( q, context.times(a,b) );

      static std::list<std::string> baseline = {"[", "*", "5", "[", "*", "a", "b", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = (a*b)*c" ) {
      numval_t e = context.times( context.times(a,b), c );

      static std::list<std::string> baseline = {"[", "*", "[", "*", "a", "b", "]", "c", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = c*(a*b)" ) {
      numval_t e = context.times( c, context.times(a,b) );

      static std::list<std::string> baseline = {"[", "*", "c", "[", "*", "a", "b", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = (a*b)*(c*d)" ) {
      numval_t e = context.times( context.times(a,b), context.times(c,d) );

      static std::list<std::string> baseline = {"[", "*", "[", "*", "a", "b", "]", "[", "*", "c", "d", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }
  }

  SECTION( "Test nestedProduct2" ) {

    WHEN( "e = (c+(a+b)) * ((a+b)+d)" ) {
      numval_t e1 = context.plus(a,b);
      numval_t e = context.times( context.plus(c,e1), context.plus(e1,d) );

      static std::list<std::string> baseline = {"[", "*", "[", "+", "c", "[", "+", "a", "b", "]", "]", "[", "+", "[", "+", "a", "b", "]", "d", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = (c*(a+b)) * ((a+b)*d)" ) {
      numval_t e1 = context.plus(a,b);
      numval_t e = context.times( context.times(c,e1), context.times(e1,d) );

      static std::list<std::string> baseline = {"[", "*", "[", "*", "c", "[", "+", "a", "b", "]", "]", "[", "*", "[", "+", "a", "b", "]", "d", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }
  }

  SECTION( "Test nestedProduct3" ) {
    numval_t q = context._param(3, false);
    numval_t Q = context._param(5, false);

    WHEN( "e = (3*b)*5" ) {
      numval_t e = context.times(context.times(q,b), Q);

      static std::list<std::string> baseline = {"[", "*", "[", "*", "3", "b", "]", "5", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = (a*b)*5" ) {
      numval_t e = context.times(context.times(a,b), Q);

      static std::list<std::string> baseline = {"[", "*", "[", "*", "a", "b", "]", "5", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 5*(3*b)" ) {
      numval_t e = context.times(Q, context.times(q,b));

      static std::list<std::string> baseline = {"[", "*", "5", "[", "*", "3", "b", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 5*(a*b)" ) {
      numval_t e = context.times(Q, context.times(a,b));

      static std::list<std::string> baseline = {"[", "*", "5", "[", "*", "a", "b", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = (3*b)*c" ) {
      numval_t e = context.times(context.times(q,b), c);

      static std::list<std::string> baseline = {"[", "*", "[", "*", "3", "b", "]", "c", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = c*(a*b)" ) {
      numval_t e = context.times(c, context.times(a,b));

      static std::list<std::string> baseline = {"[", "*", "c", "[", "*", "a", "b", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = (a*b)*(c*d)" ) {
      numval_t e = context.times(context.times(a,b), context.times(c,d));

      static std::list<std::string> baseline = {"[", "*", "[", "*", "a", "b", "]", "[", "*", "c", "d", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }
  }

  SECTION( "Test trivialProduct" ) {

    numval_t q = context._param(0, true);
    numval_t Q = context._param(0.0, true);
    numval_t r = context._param(1, true);
    numval_t R = context._param(1.0, true);

    WHEN( "e = a * 0" ) {
      numval_t e = context.times(a, q);

      static std::list<std::string> baseline = {"0"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 0 * a" ) {
      numval_t e = context.times(q, a);

      static std::list<std::string> baseline = {"0"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = a * 0.0" ) {
      numval_t e = context.times(a, Q);

      static std::list<std::string> baseline = {"0"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 0.0 * a" ) {
      numval_t e = context.times(Q, a);

      static std::list<std::string> baseline = {"0"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = a * 1" ) {
      numval_t e = context.times(a, r);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 1 * a" ) {
      numval_t e = context.times(r, a);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = a * 1.0" ) {
      numval_t e = context.times(a, R);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 1.0 * a" ) {
      numval_t e = context.times(R, a);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( expr_to_list(e) == baseline );
    }
  }

}


TEST_CASE( "div_expression", "[smoke]" ) {

  ExpressionContext_Objects context;

  numval_t a = context._var(false, false, 0.0, 1.0, 0.0, "a");
  numval_t b = context._var(false, false, 0.0, 1.0, 0.0, "b");
  numval_t c = context._var(false, false, 0.0, 1.0, 0.0, "c");
  numval_t d = context._var(false, false, 0.0, 1.0, 0.0, "d");

  SECTION( "Test simpleDivision" ) {
    numval_t e = context.divide(a,b);

    static std::list<std::string> baseline = {"[", "/", "a", "b", "]"};
    REQUIRE( expr_to_list(e) == baseline );
  }

  SECTION( "Test constDivision" ) {
    numval_t q = context._param(5, false);
    numval_t Q = context._param(5.0, false);

    WHEN( "e = 5/5.0" ) {
      numval_t e = context.divide(q,Q);

      static std::list<std::string> baseline = {"1.000"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = a/5" ) {
      numval_t e = context.divide(a,q);

      static std::list<std::string> baseline = {"[", "/", "a", "5", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 5/a" ) {
      numval_t e = context.divide(q,a);

      static std::list<std::string> baseline = {"[", "/", "5", "a", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = a/5.0" ) {
      numval_t e = context.divide(a,Q);

      static std::list<std::string> baseline = {"[", "/", "a", "5.000", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 5.0/a" ) {
      numval_t e = context.divide(Q,a);

      static std::list<std::string> baseline = {"[", "/", "5.000", "a", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }
  }

  SECTION( "Test nestedDivision" ) {
    numval_t p = context._param(3, false);
    numval_t q = context._param(5, false);

    WHEN( "e = (3*b)/5" ) {
      numval_t e = context.divide(context.times(p,b), q);

      static std::list<std::string> baseline = {"[", "/", "[", "*", "3", "b", "]", "5", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = (a/b)/5" ) {
      numval_t e = context.divide(context.divide(a,b), q);

      static std::list<std::string> baseline = {"[", "/", "[", "/", "a", "b", "]", "5", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 5/(a/b)" ) {
      numval_t e = context.divide(q, context.divide(a,b));

      static std::list<std::string> baseline = {"[", "/", "5", "[", "/", "a", "b", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = (a/b)/c" ) {
      numval_t e = context.divide(context.divide(a,b), c);

      static std::list<std::string> baseline = {"[", "/", "[", "/", "a", "b", "]", "c", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = c/(a/b)" ) {
      numval_t e = context.divide(c, context.divide(a,b));

      static std::list<std::string> baseline = {"[", "/", "c", "[", "/", "a", "b", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = (a/b)/(c/d)" ) {
      numval_t e = context.divide(context.divide(a,b), context.divide(c,d));

      static std::list<std::string> baseline = {"[", "/", "[", "/", "a", "b", "]", "[", "/", "c", "d", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }
  }

  SECTION( "Test trivialDivision" ) {
    numval_t q = context._param(0, true);
    numval_t Q = context._param(0.0, true);
    numval_t r = context._param(1, true);
    numval_t R = context._param(1.0, true);
    numval_t s = context._param(-1, true);
    numval_t S = context._param(-1.0, true);

    WHEN( "e = 0 / a" ) {
      numval_t e = context.divide(q, a);

      static std::list<std::string> baseline = {"0"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 0.0 / a" ) {
      numval_t e = context.divide(Q, a);

      static std::list<std::string> baseline = {"0"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = a / 1" ) {
      numval_t e = context.divide(a, r);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = a / 1.0" ) {
      numval_t e = context.divide(a, R);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = a / -1" ) {
      numval_t e = context.divide(a, s);

      static std::list<std::string> baseline = {"[", "-", "a", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = a / -1.0" ) {
      numval_t e = context.divide(a, S);

      static std::list<std::string> baseline = {"[", "-", "a", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }
  }

}


TEST_CASE( "pow_expression", "[smoke]" ) {

  ExpressionContext_Objects context;

  numval_t a = context._var(false, false, 0.0, 1.0, 0.0, "a");
  numval_t b = context._var(false, false, 0.0, 1.0, 0.0, "b");

  SECTION( "Test simplePow" ) {
    numval_t e = context.pow(a,b);

    static std::list<std::string> baseline = {"[", "**", "a", "b", "]"};
    REQUIRE( expr_to_list(e) == baseline );
  }

  SECTION( "Test constPow" ) {
    numval_t q = context._param(5, false);
    numval_t Q = context._param(5.0, false);

    WHEN( "e = a**5" ) {
      numval_t e = context.pow(a, q);

      static std::list<std::string> baseline = {"[", "**", "a", "5", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 5**a" ) {
      numval_t e = context.pow(q, a);

      static std::list<std::string> baseline = {"[", "**", "5", "a", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = a**5.0" ) {
      numval_t e = context.pow(a, Q);

      static std::list<std::string> baseline = {"[", "**", "a", "5.000", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 5.0**a" ) {
      numval_t e = context.pow(Q, a);

      static std::list<std::string> baseline = {"[", "**", "5.000", "a", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }
  }

  SECTION( "Test trivialPow" ) {

    numval_t q = context._param(0, true);
    numval_t Q = context._param(0.0, true);
    numval_t r = context._param(1, true);
    numval_t R = context._param(1.0, true);

    WHEN( "e = 0 ** a" ) {
      numval_t e = context.pow(q, a);

      static std::list<std::string> baseline = {"0"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 0.0 ** a" ) {
      numval_t e = context.pow(Q, a);

      static std::list<std::string> baseline = {"0"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 1 ** a" ) {
      numval_t e = context.pow(r, a);

      static std::list<std::string> baseline = {"1"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 1.0 ** a" ) {
      numval_t e = context.pow(R, a);

      static std::list<std::string> baseline = {"1"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = a ** 0" ) {
      numval_t e = context.pow(a, q);

      static std::list<std::string> baseline = {"1"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = a ** 0.0" ) {
      numval_t e = context.pow(a, Q);

      static std::list<std::string> baseline = {"1"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = a ** 1" ) {
      numval_t e = context.pow(a, r);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = a ** 1.0" ) {
      numval_t e = context.pow(a, R);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( expr_to_list(e) == baseline );
    }
  }
}


TEST_CASE( "intrinsics", "[smoke]" ) {

  ExpressionContext_Objects context;

  numval_t v = context._var(false, false, 0.0, 1.0, 0.0, "v");
  numval_t p = context._var(false, false, 0.0, 1.0, 0.0, "p");

  SECTION( "Test ceil" ) {
    numval_t e = context.ceil(v);
    v->_value = 1.5;
    REQUIRE( e->value() == 2.0 );
    v->_value = -1.5;
    REQUIRE( e->value() == -1.0 );
  }

  SECTION( "Test floor" ) {
    numval_t e = context.floor(v);
    v->_value = 1.5;
    REQUIRE( e->value() == 1.0 );
    v->_value = -1.5;
    REQUIRE( e->value() == -2.0 );
  }

  SECTION( "Test exp" ) {
    numval_t e = context.exp(v);
    v->_value = 1;
    REQUIRE( e->value() == Approx( E ) );
    v->_value = 0;
    REQUIRE( e->value() == 1 );
  }

  SECTION( "Test log" ) {
    numval_t e = context.log(v);
    v->_value = 1;
    REQUIRE( e->value() == Approx( 0.0 ) );
    v->_value = exp(1.0);
    REQUIRE( e->value() == Approx( 1.0 ) );
  }

  SECTION( "Test log10" ) {
    numval_t e = context.log10(v);
    v->_value = 1;
    REQUIRE( e->value() == Approx( 0.0 ) );
    v->_value = 10;
    REQUIRE( e->value() == Approx( 1.0 ) );
  }

  SECTION( "Test pow" ) {
    numval_t e = context.pow(v, p);
    v->_value = 2;
    p->_value = 0;
    REQUIRE( e->value() == Approx( 1.0 ) );
    p->_value = 1;
    REQUIRE( e->value() == Approx( 2.0 ) );
  }

  SECTION( "Test sqrt" ) {
    numval_t e = context.sqrt(v);
    v->_value = 1;
    REQUIRE( e->value() == Approx( 1.0 ) );
    v->_value = 4;
    REQUIRE( e->value() == Approx( 2.0 ) );
  }

  SECTION( "Test sin" ) {
    numval_t e = context.sin(v);
    v->_value = 0;
    REQUIRE( e->value() == Approx( 0.0 ) );
    v->_value = PI/2.0;
    REQUIRE( e->value() == Approx( 1.0 ) );
  }

  SECTION( "Test cos" ) {
    numval_t e = context.cos(v);
    v->_value = 0;
    REQUIRE( e->value() == Approx( 1.0 ) );
    v->_value = PI/2.0;
    REQUIRE( e->value() == Approx( 0.0 ).margin(1e-7) );
  }

  SECTION( "Test tan" ) {
    numval_t e = context.tan(v);
    v->_value = 0;
    REQUIRE( e->value() == Approx( 0.0 ) );
    v->_value = PI/4.0;
    REQUIRE( e->value() == Approx( 1.0 ) );
  }

  SECTION( "Test asin" ) {
    numval_t e = context.asin(v);
    v->_value = 0;
    REQUIRE( e->value() == Approx( 0.0 ) );
    v->_value = 1.0;
    REQUIRE( e->value() == Approx( PI/2.0 ) );
  }

  SECTION( "Test acos" ) {
    numval_t e = context.acos(v);
    v->_value = 1;
    REQUIRE( e->value() == Approx( 0.0 ) );
    v->_value = 0;
    REQUIRE( e->value() == Approx( PI/2.0 ) );
  }

  SECTION( "Test atan" ) {
    numval_t e = context.atan(v);
    v->_value = 0;
    REQUIRE( e->value() == Approx( 0.0 ) );
    v->_value = 1;
    REQUIRE( e->value() == Approx( PI/4.0 ) );
  }

  SECTION( "Test sinh" ) {
    numval_t e = context.sinh(v);
    v->_value = 0;
    REQUIRE( e->value() == Approx( 0.0 ) );
    v->_value = 1;
    REQUIRE( e->value() == Approx( (E-1/E)/2.0 ) );
  }

  SECTION( "Test cosh" ) {
    numval_t e = context.cosh(v);
    v->_value = 0;
    REQUIRE( e->value() == Approx( 1.0 ) );
    v->_value = 1;
    REQUIRE( e->value() == Approx( (E+1/E)/2.0 ) );
  }

  SECTION( "Test tanh" ) {
    numval_t e = context.tanh(v);
    v->_value = 0;
    REQUIRE( e->value() == Approx( 0.0 ) );
    v->_value = 1;
    REQUIRE( e->value() == Approx( (E-1/E)/(E+1/E) ) );
  }

  SECTION( "Test asinh" ) {
    numval_t e = context.asinh(v);
    v->_value = 0;
    REQUIRE( e->value() == Approx( 0.0 ) );
    v->_value = (E-1/E)/2.0;
    REQUIRE( e->value() == Approx( 1.0 ) );
  }

  SECTION( "Test acosh" ) {
    numval_t e = context.acosh(v);
    v->_value = 1;
    REQUIRE( e->value() == Approx( 0.0 ) );
    v->_value = (E+1/E)/2.0;
    REQUIRE( e->value() == Approx( 1.0 ) );
  }

  SECTION( "Test atanh" ) {
    numval_t e = context.atanh(v);
    v->_value = 0;
    REQUIRE( e->value() == Approx( 0.0 ) );
    v->_value = (E-1/E)/(E+1/E);
    REQUIRE( e->value() == Approx( 1.0 ) );
  }

}
