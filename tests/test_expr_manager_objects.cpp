
#include "expr/expr_manager_objects.hpp"
#include "catch.hpp"

const double PI = 3.141592653589793238463;
const double E = exp(1.0);

typedef typename ExprManager_Objects::numval_t numval_t;


TEST_CASE( "add_expression", "[smoke]" ) {

  ExprManager_Objects manager;
  numval_t a = manager._var(false, false, 0.0, 1.0, 0.0, "a");
  numval_t b = manager._var(false, false, 0.0, 1.0, 0.0, "b");
  numval_t c = manager._var(false, false, 0.0, 1.0, 0.0, "c");
  numval_t d = manager._var(false, false, 0.0, 1.0, 0.0, "d");

  SECTION( "Test simpleSum" ) {
    numval_t e = manager.plus(a, b);

    static std::list<std::string> baseline = {"[", "+", "a", "b", "]"};
    REQUIRE( expr_to_list(e) == baseline );
  }

  SECTION( "Test simpleSum_API" ) {
    numval_t q  = manager._param(2, true);
    numval_t e1 = manager.plus(a, b);
    numval_t e2 = manager.times(q, a);
    numval_t e = manager.plus(e1, e2);

    static std::list<std::string> baseline = {"[", "+", "[", "+", "a", "b", "]", "[", "*", "2", "a", "]", "]" };
    REQUIRE( expr_to_list(e) == baseline );
  }

  SECTION( "Test constSum" ) {
    numval_t q = manager._param(5, true);
    numval_t Q = manager._param(5.0, true);

    WHEN( "e = 5 + 5.0" ) {
      numval_t e = manager.plus(q, Q);

      static std::list<std::string> baseline = {"10.000"};
      REQUIRE( expr_to_list(e) == baseline );
    }
    WHEN( "e = a + 5" ) {
      numval_t e = manager.plus(a, q);

      static std::list<std::string> baseline = {"[", "+", "a", "5", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 5 + a" ) {
      numval_t e = manager.plus(q, a);

      static std::list<std::string> baseline = {"[", "+", "5", "a", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = a + 5.0" ) {
      numval_t e = manager.plus(a, Q);

      static std::list<std::string> baseline = {"[", "+", "a", "5.000", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 5.0 + a" ) {
      numval_t e = manager.plus(Q, a);

      static std::list<std::string> baseline = {"[", "+", "5.000", "a", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }
  }

  SECTION( "Test nestedSum" ) {
    numval_t q = manager._param(5, true);

    WHEN( "e = (a+b)+5" ) {
      numval_t e = manager.plus( manager.plus(a, b), q );

      static std::list<std::string> baseline = {"[", "+", "[", "+", "a", "b", "]", "5", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 5+(a+b)" ) {
      numval_t e = manager.plus( q, manager.plus(a, b) );

      static std::list<std::string> baseline = {"[", "+", "5", "[", "+", "a", "b", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = (a+b)+c" ) {
      numval_t e = manager.plus( manager.plus(a, b), c );

      static std::list<std::string> baseline = {"[", "+", "[", "+", "a", "b", "]", "c", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = c+(a+b)" ) {
      numval_t e = manager.plus( c, manager.plus(a, b) );

      static std::list<std::string> baseline = {"[", "+", "c", "[", "+", "a", "b", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = (a+b)+(c+d)" ) {
      numval_t e = manager.plus( manager.plus(a, b), manager.plus(c, d) );

      static std::list<std::string> baseline = {"[", "+", "[", "+", "a", "b", "]", "[", "+", "c", "d", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

  }

  SECTION( "Test nestedSum" ) {
    numval_t q = manager._param(2, true);

    WHEN( "e = 2*(a+b)+c" ) {
      numval_t e = manager.plus(manager.times( q, manager.plus(a, b)), c );

      static std::list<std::string> baseline = {"[", "+", "[", "*", "2", "[", "+", "a", "b", "]", "]", "c", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 3*(2*(a+b)+c)" ) {
      numval_t Q = manager._param(3, true);
      numval_t e = manager.times(Q, manager.plus(manager.times(q, manager.plus(a, b)), c));

      static std::list<std::string> baseline = {"[", "*", "3", "[", "+", "[", "*", "2", "[", "+", "a", "b", "]", "]", "c", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }
  }

  SECTION( "Test trivialSum" ) {

    numval_t q = manager._param(0, true);
    numval_t Q = manager._param(0.0, true);

    WHEN( "e = a + 0" ) {
      numval_t e = manager.plus(a, q);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 0 + a" ) {
      numval_t e = manager.plus(q, a);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = a + 0.0" ) {
      numval_t e = manager.plus(a, Q);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 0.0 + a" ) {
      numval_t e = manager.plus(Q, a);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( expr_to_list(e) == baseline );
    }
  }

  SECTION( "Test nestedTrivialProduct" ) {
    numval_t q = manager._param(5, true);

    WHEN( "e = a*5 + b" ) {
      numval_t e = manager.plus(manager.times(a, q), b);

      static std::list<std::string> baseline = {"[", "+", "[", "*", "a", "5", "]", "b", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = b + a*5" ) {
      numval_t e = manager.plus(b, manager.times(a, q));

      static std::list<std::string> baseline = {"[", "+", "b", "[", "*", "a", "5", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = a*5 + (b+c)" ) {
      numval_t e = manager.plus(manager.times(a, q), manager.plus(b,c));

      static std::list<std::string> baseline = {"[", "+", "[", "*", "a", "5", "]", "[", "+", "b", "c", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = (b+c) + a*5" ) {
      numval_t e = manager.plus(manager.plus(b,c), manager.times(a, q));

      static std::list<std::string> baseline = {"[", "+", "[", "+", "b", "c", "]", "[", "*", "a", "5", "]",  "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }
  }

}


TEST_CASE( "diff_expression", "[smoke]" ) {

  ExprManager_Objects manager;
  numval_t a = manager._var(false, false, 0.0, 1.0, 0.0, "a");
  numval_t b = manager._var(false, false, 0.0, 1.0, 0.0, "b");
  numval_t c = manager._var(false, false, 0.0, 1.0, 0.0, "c");
  numval_t d = manager._var(false, false, 0.0, 1.0, 0.0, "d");

  SECTION( "Test simpleDiff" ) {
    numval_t e = manager.minus(a, b);

    static std::list<std::string> baseline = {"[", "-", "a", "b", "]"};
    REQUIRE( expr_to_list(e) == baseline );
  }

  SECTION( "Test constDiff" ) {
    numval_t q = manager._param(5, true);
    numval_t Q = manager._param(5.0, true);

    WHEN( "e = 5 - 1.0" ) {
      numval_t R = manager._param(1.0, true);
      numval_t e = manager.minus(q, R);

      static std::list<std::string> baseline = {"4.000"};
      REQUIRE( expr_to_list(e) == baseline );
      }

    WHEN( "e = a - 5" ) {
      numval_t e = manager.minus(a, q);

      static std::list<std::string> baseline = {"[", "-", "a", "5", "]"};
      REQUIRE( expr_to_list(e) == baseline );
      }

    WHEN( "e = 5 - a" ) {
      numval_t e = manager.minus(q, a);

      static std::list<std::string> baseline = {"[", "-", "5", "a", "]"};
      REQUIRE( expr_to_list(e) == baseline );
      }

    WHEN( "e = a - 5.0" ) {
      numval_t e = manager.minus(a, Q);

      static std::list<std::string> baseline = {"[", "-", "a", "5.000", "]"};
      REQUIRE( expr_to_list(e) == baseline );
      }

    WHEN( "e = 5.0 - a" ) {
      numval_t e = manager.minus(Q, a);

      static std::list<std::string> baseline = {"[", "-", "5.000", "a", "]"};
      REQUIRE( expr_to_list(e) == baseline );
      }
  }

  SECTION( "Test paramDiff" ) {
    numval_t p = manager._param(5, true, "p");

    WHEN( "e = a - p" ) {
      numval_t e = manager.minus(a, p);

      static std::list<std::string> baseline = {"[", "-", "a", "p", "]"};
      REQUIRE( expr_to_list(e) == baseline );
      }

    WHEN( "e = p - a" ) {
      numval_t e = manager.minus(p, a);

      static std::list<std::string> baseline = {"[", "-", "p", "a", "]"};
      REQUIRE( expr_to_list(e) == baseline );
      }
  }

  SECTION( "Test termDiff" ) {
    numval_t q = manager._param(2, true);
    numval_t Q = manager._param(5, true);

    numval_t e = manager.minus(Q, manager.times(q, a));

    static std::list<std::string> baseline = {"[", "-", "5", "[", "*", "2", "a", "]", "]"};
    REQUIRE( expr_to_list(e) == baseline );
  }

  SECTION( "Test nestedDiff" ) {
    numval_t Q = manager._param(5, true);

    WHEN( "e = (a-b)-5" ) {
      numval_t e = manager.minus(manager.minus(a,b), Q);

      static std::list<std::string> baseline = {"[", "-", "[", "-", "a", "b", "]", "5", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 5-(a-b)" ) {
      numval_t e = manager.minus(Q, manager.minus(a,b));

      static std::list<std::string> baseline = {"[", "-", "5", "[", "-", "a", "b", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = (a-b)-c" ) {
      numval_t e = manager.minus(manager.minus(a,b), c);

      static std::list<std::string> baseline = {"[", "-", "[", "-", "a", "b", "]", "c", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = (a-b)-(c-d)" ) {
      numval_t e = manager.minus(manager.minus(a,b), manager.minus(c,d));

      static std::list<std::string> baseline = {"[", "-", "[", "-", "a", "b", "]", "[", "-", "c", "d", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }
  }

  SECTION( "Test sumOf_nestedTrivialProduct2" ) {
    numval_t Q = manager._param(5, true);

    WHEN( "a*5 - b" ) {
      numval_t e = manager.minus(manager.times(a,Q), b);
      
      static std::list<std::string> baseline = {"[", "-", "[", "*", "a", "5", "]", "b", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "b - a*5" ) {
      numval_t e = manager.minus(b, manager.times(a,Q));
      
      static std::list<std::string> baseline = {"[", "-", "b", "[", "*", "a", "5", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "a*5 - (b-c)" ) {
      numval_t e = manager.minus(manager.times(a,Q), manager.minus(b,c));
      
      static std::list<std::string> baseline = {"[", "-", "[", "*", "a", "5", "]", "[", "-", "b", "c", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "(b-c) - a*5" ) {
      numval_t e = manager.minus(manager.minus(b,c), manager.times(a,Q));
      
      static std::list<std::string> baseline = {"[", "-", "[", "-", "b", "c", "]", "[", "*", "a", "5", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }
  }

  SECTION( "Test trivialDiff" ) {

    numval_t q = manager._param(0, true);
    numval_t Q = manager._param(0.0, true);

    WHEN( "e = a - 0" ) {
      numval_t e = manager.minus(a, q);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 0 - a" ) {
      numval_t e = manager.minus(q, a);

      static std::list<std::string> baseline = {"[", "-", "a", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = a - 0.0" ) {
      numval_t e = manager.minus(a, Q);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 0.0 - a" ) {
      numval_t e = manager.minus(Q, a);

      static std::list<std::string> baseline = {"[", "-", "a", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }
  }


}


TEST_CASE( "neg_expression", "[smoke]" ) {

  ExprManager_Objects manager;

  SECTION( "Test negation_param" ) {
    numval_t p = manager._param(2, false, "p");

    WHEN( "e = -p" ) {
      numval_t e = manager.negate(p);

      static std::list<std::string> baseline = {"[", "-", "p", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = -(-p)" ) {
      numval_t e = manager.negate(manager.negate(p));

      static std::list<std::string> baseline = {"[", "-", "[", "-", "p", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }
  }

  SECTION( "Test negation_terms" ) {
    numval_t p = manager._param(2, false, "p");
    numval_t q = manager._param(5, false);
    numval_t v = manager._var(false, false, 0.0, 1.0, 0.0, "v");

    WHEN( "e = - p*v" ) {
      numval_t e = manager.negate( manager.times(p,v) );

      static std::list<std::string> baseline = {"[", "-", "[", "*", "p", "v", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = -(- p*v)" ) {
      numval_t e = manager.negate( manager.negate( manager.times(p,v) ) );

      static std::list<std::string> baseline = {"[", "-", "[", "-", "[", "*", "p", "v", "]", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = - 5*v" ) {
      numval_t e = manager.negate( manager.times(q,v) );

      static std::list<std::string> baseline = {"[", "-", "[", "*", "5", "v", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = -(- q*v)" ) {
      numval_t e = manager.negate( manager.negate( manager.times(q,v) ) );

      static std::list<std::string> baseline = {"[", "-", "[", "-", "[", "*", "5", "v", "]", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }
  }

}


TEST_CASE( "mul_expression", "[smoke]" ) {

  ExprManager_Objects manager;

  numval_t a = manager._var(false, false, 0.0, 1.0, 0.0, "a");
  numval_t b = manager._var(false, false, 0.0, 1.0, 0.0, "b");
  numval_t c = manager._var(false, false, 0.0, 1.0, 0.0, "c");
  numval_t d = manager._var(false, false, 0.0, 1.0, 0.0, "d");

  SECTION( "Test simpleProduct" ) {
    numval_t e = manager.times(a,b);

    static std::list<std::string> baseline = {"[", "*", "a", "b", "]"};
    REQUIRE( expr_to_list(e) == baseline );
  }

  SECTION( "Test constProduct" ) {
    numval_t q = manager._param(5, false);
    numval_t Q = manager._param(5.0, false);

    WHEN( "e = 5*5.0" ) {
      numval_t e = manager.times(q,Q);

      static std::list<std::string> baseline = {"25.000"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = a*5" ) {
      numval_t e = manager.times(a,q);

      static std::list<std::string> baseline = {"[", "*", "a", "5", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 5*a" ) {
      numval_t e = manager.times(q,a);

      static std::list<std::string> baseline = {"[", "*", "5", "a", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = a*5.0" ) {
      numval_t e = manager.times(a,Q);

      static std::list<std::string> baseline = {"[", "*", "a", "5.000", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 5.0*a" ) {
      numval_t e = manager.times(Q,a);

      static std::list<std::string> baseline = {"[", "*", "5.000", "a", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }
  }

  SECTION( "Test nestedProduct" ) {
    numval_t q = manager._param(5, false);

    WHEN( "e = (a*b)*5" ) {
      numval_t e = manager.times( manager.times(a,b), q );

      static std::list<std::string> baseline = {"[", "*", "[", "*", "a", "b", "]", "5", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 5*(a*b)" ) {
      numval_t e = manager.times( q, manager.times(a,b) );

      static std::list<std::string> baseline = {"[", "*", "5", "[", "*", "a", "b", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = (a*b)*c" ) {
      numval_t e = manager.times( manager.times(a,b), c );

      static std::list<std::string> baseline = {"[", "*", "[", "*", "a", "b", "]", "c", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = c*(a*b)" ) {
      numval_t e = manager.times( c, manager.times(a,b) );

      static std::list<std::string> baseline = {"[", "*", "c", "[", "*", "a", "b", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = (a*b)*(c*d)" ) {
      numval_t e = manager.times( manager.times(a,b), manager.times(c,d) );

      static std::list<std::string> baseline = {"[", "*", "[", "*", "a", "b", "]", "[", "*", "c", "d", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }
  }

  SECTION( "Test nestedProduct2" ) {

    WHEN( "e = (c+(a+b)) * ((a+b)+d)" ) {
      numval_t e1 = manager.plus(a,b);
      numval_t e = manager.times( manager.plus(c,e1), manager.plus(e1,d) );

      static std::list<std::string> baseline = {"[", "*", "[", "+", "c", "[", "+", "a", "b", "]", "]", "[", "+", "[", "+", "a", "b", "]", "d", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = (c*(a+b)) * ((a+b)*d)" ) {
      numval_t e1 = manager.plus(a,b);
      numval_t e = manager.times( manager.times(c,e1), manager.times(e1,d) );

      static std::list<std::string> baseline = {"[", "*", "[", "*", "c", "[", "+", "a", "b", "]", "]", "[", "*", "[", "+", "a", "b", "]", "d", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }
  }

  SECTION( "Test nestedProduct3" ) {
    numval_t q = manager._param(3, false);
    numval_t Q = manager._param(5, false);

    WHEN( "e = (3*b)*5" ) {
      numval_t e = manager.times(manager.times(q,b), Q);

      static std::list<std::string> baseline = {"[", "*", "[", "*", "3", "b", "]", "5", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = (a*b)*5" ) {
      numval_t e = manager.times(manager.times(a,b), Q);

      static std::list<std::string> baseline = {"[", "*", "[", "*", "a", "b", "]", "5", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 5*(3*b)" ) {
      numval_t e = manager.times(Q, manager.times(q,b));

      static std::list<std::string> baseline = {"[", "*", "5", "[", "*", "3", "b", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 5*(a*b)" ) {
      numval_t e = manager.times(Q, manager.times(a,b));

      static std::list<std::string> baseline = {"[", "*", "5", "[", "*", "a", "b", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = (3*b)*c" ) {
      numval_t e = manager.times(manager.times(q,b), c);

      static std::list<std::string> baseline = {"[", "*", "[", "*", "3", "b", "]", "c", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = c*(a*b)" ) {
      numval_t e = manager.times(c, manager.times(a,b));

      static std::list<std::string> baseline = {"[", "*", "c", "[", "*", "a", "b", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = (a*b)*(c*d)" ) {
      numval_t e = manager.times(manager.times(a,b), manager.times(c,d));

      static std::list<std::string> baseline = {"[", "*", "[", "*", "a", "b", "]", "[", "*", "c", "d", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }
  }

  SECTION( "Test trivialProduct" ) {

    numval_t q = manager._param(0, true);
    numval_t Q = manager._param(0.0, true);
    numval_t r = manager._param(1, true);
    numval_t R = manager._param(1.0, true);

    WHEN( "e = a * 0" ) {
      numval_t e = manager.times(a, q);

      static std::list<std::string> baseline = {"0"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 0 * a" ) {
      numval_t e = manager.times(q, a);

      static std::list<std::string> baseline = {"0"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = a * 0.0" ) {
      numval_t e = manager.times(a, Q);

      static std::list<std::string> baseline = {"0"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 0.0 * a" ) {
      numval_t e = manager.times(Q, a);

      static std::list<std::string> baseline = {"0"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = a * 1" ) {
      numval_t e = manager.times(a, r);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 1 * a" ) {
      numval_t e = manager.times(r, a);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = a * 1.0" ) {
      numval_t e = manager.times(a, R);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 1.0 * a" ) {
      numval_t e = manager.times(R, a);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( expr_to_list(e) == baseline );
    }
  }

}


TEST_CASE( "div_expression", "[smoke]" ) {

  ExprManager_Objects manager;

  numval_t a = manager._var(false, false, 0.0, 1.0, 0.0, "a");
  numval_t b = manager._var(false, false, 0.0, 1.0, 0.0, "b");
  numval_t c = manager._var(false, false, 0.0, 1.0, 0.0, "c");
  numval_t d = manager._var(false, false, 0.0, 1.0, 0.0, "d");

  SECTION( "Test simpleDivision" ) {
    numval_t e = manager.divide(a,b);

    static std::list<std::string> baseline = {"[", "/", "a", "b", "]"};
    REQUIRE( expr_to_list(e) == baseline );
  }

  SECTION( "Test constDivision" ) {
    numval_t q = manager._param(5, false);
    numval_t Q = manager._param(5.0, false);

    WHEN( "e = 5/5.0" ) {
      numval_t e = manager.divide(q,Q);

      static std::list<std::string> baseline = {"1.000"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = a/5" ) {
      numval_t e = manager.divide(a,q);

      static std::list<std::string> baseline = {"[", "/", "a", "5", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 5/a" ) {
      numval_t e = manager.divide(q,a);

      static std::list<std::string> baseline = {"[", "/", "5", "a", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = a/5.0" ) {
      numval_t e = manager.divide(a,Q);

      static std::list<std::string> baseline = {"[", "/", "a", "5.000", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 5.0/a" ) {
      numval_t e = manager.divide(Q,a);

      static std::list<std::string> baseline = {"[", "/", "5.000", "a", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }
  }

  SECTION( "Test nestedDivision" ) {
    numval_t p = manager._param(3, false);
    numval_t q = manager._param(5, false);

    WHEN( "e = (3*b)/5" ) {
      numval_t e = manager.divide(manager.times(p,b), q);

      static std::list<std::string> baseline = {"[", "/", "[", "*", "3", "b", "]", "5", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = (a/b)/5" ) {
      numval_t e = manager.divide(manager.divide(a,b), q);

      static std::list<std::string> baseline = {"[", "/", "[", "/", "a", "b", "]", "5", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 5/(a/b)" ) {
      numval_t e = manager.divide(q, manager.divide(a,b));

      static std::list<std::string> baseline = {"[", "/", "5", "[", "/", "a", "b", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = (a/b)/c" ) {
      numval_t e = manager.divide(manager.divide(a,b), c);

      static std::list<std::string> baseline = {"[", "/", "[", "/", "a", "b", "]", "c", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = c/(a/b)" ) {
      numval_t e = manager.divide(c, manager.divide(a,b));

      static std::list<std::string> baseline = {"[", "/", "c", "[", "/", "a", "b", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = (a/b)/(c/d)" ) {
      numval_t e = manager.divide(manager.divide(a,b), manager.divide(c,d));

      static std::list<std::string> baseline = {"[", "/", "[", "/", "a", "b", "]", "[", "/", "c", "d", "]", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }
  }

  SECTION( "Test trivialDivision" ) {
    numval_t q = manager._param(0, true);
    numval_t Q = manager._param(0.0, true);
    numval_t r = manager._param(1, true);
    numval_t R = manager._param(1.0, true);
    numval_t s = manager._param(-1, true);
    numval_t S = manager._param(-1.0, true);

    WHEN( "e = 0 / a" ) {
      numval_t e = manager.divide(q, a);

      static std::list<std::string> baseline = {"0"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 0.0 / a" ) {
      numval_t e = manager.divide(Q, a);

      static std::list<std::string> baseline = {"0"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = a / 1" ) {
      numval_t e = manager.divide(a, r);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = a / 1.0" ) {
      numval_t e = manager.divide(a, R);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = a / -1" ) {
      numval_t e = manager.divide(a, s);

      static std::list<std::string> baseline = {"[", "-", "a", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = a / -1.0" ) {
      numval_t e = manager.divide(a, S);

      static std::list<std::string> baseline = {"[", "-", "a", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }
  }

}


TEST_CASE( "pow_expression", "[smoke]" ) {

  ExprManager_Objects manager;

  numval_t a = manager._var(false, false, 0.0, 1.0, 0.0, "a");
  numval_t b = manager._var(false, false, 0.0, 1.0, 0.0, "b");

  SECTION( "Test simplePow" ) {
    numval_t e = manager.pow(a,b);

    static std::list<std::string> baseline = {"[", "**", "a", "b", "]"};
    REQUIRE( expr_to_list(e) == baseline );
  }

  SECTION( "Test constPow" ) {
    numval_t q = manager._param(5, false);
    numval_t Q = manager._param(5.0, false);

    WHEN( "e = a**5" ) {
      numval_t e = manager.pow(a, q);

      static std::list<std::string> baseline = {"[", "**", "a", "5", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 5**a" ) {
      numval_t e = manager.pow(q, a);

      static std::list<std::string> baseline = {"[", "**", "5", "a", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = a**5.0" ) {
      numval_t e = manager.pow(a, Q);

      static std::list<std::string> baseline = {"[", "**", "a", "5.000", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 5.0**a" ) {
      numval_t e = manager.pow(Q, a);

      static std::list<std::string> baseline = {"[", "**", "5.000", "a", "]"};
      REQUIRE( expr_to_list(e) == baseline );
    }
  }

  SECTION( "Test trivialPow" ) {

    numval_t q = manager._param(0, true);
    numval_t Q = manager._param(0.0, true);
    numval_t r = manager._param(1, true);
    numval_t R = manager._param(1.0, true);

    WHEN( "e = 0 ** a" ) {
      numval_t e = manager.pow(q, a);

      static std::list<std::string> baseline = {"0"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 0.0 ** a" ) {
      numval_t e = manager.pow(Q, a);

      static std::list<std::string> baseline = {"0"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 1 ** a" ) {
      numval_t e = manager.pow(r, a);

      static std::list<std::string> baseline = {"1"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = 1.0 ** a" ) {
      numval_t e = manager.pow(R, a);

      static std::list<std::string> baseline = {"1"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = a ** 0" ) {
      numval_t e = manager.pow(a, q);

      static std::list<std::string> baseline = {"1"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = a ** 0.0" ) {
      numval_t e = manager.pow(a, Q);

      static std::list<std::string> baseline = {"1"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = a ** 1" ) {
      numval_t e = manager.pow(a, r);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( expr_to_list(e) == baseline );
    }

    WHEN( "e = a ** 1.0" ) {
      numval_t e = manager.pow(a, R);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( expr_to_list(e) == baseline );
    }
  }
}


TEST_CASE( "intrinsics", "[smoke]" ) {

  ExprManager_Objects manager;

  numval_t v = manager._var(false, false, 0.0, 1.0, 0.0, "v");
  numval_t p = manager._var(false, false, 0.0, 1.0, 0.0, "p");

  SECTION( "Test ceil" ) {
    numval_t e = manager.ceil(v);
    v->_value = 1.5;
    REQUIRE( e->value() == 2.0 );
    v->_value = -1.5;
    REQUIRE( e->value() == -1.0 );
  }

  SECTION( "Test floor" ) {
    numval_t e = manager.floor(v);
    v->_value = 1.5;
    REQUIRE( e->value() == 1.0 );
    v->_value = -1.5;
    REQUIRE( e->value() == -2.0 );
  }

  SECTION( "Test exp" ) {
    numval_t e = manager.exp(v);
    v->_value = 1;
    REQUIRE( e->value() == Approx( E ) );
    v->_value = 0;
    REQUIRE( e->value() == 1 );
  }

  SECTION( "Test log" ) {
    numval_t e = manager.log(v);
    v->_value = 1;
    REQUIRE( e->value() == Approx( 0.0 ) );
    v->_value = exp(1.0);
    REQUIRE( e->value() == Approx( 1.0 ) );
  }

  SECTION( "Test log10" ) {
    numval_t e = manager.log10(v);
    v->_value = 1;
    REQUIRE( e->value() == Approx( 0.0 ) );
    v->_value = 10;
    REQUIRE( e->value() == Approx( 1.0 ) );
  }

  SECTION( "Test pow" ) {
    numval_t e = manager.pow(v, p);
    v->_value = 2;
    p->_value = 0;
    REQUIRE( e->value() == Approx( 1.0 ) );
    p->_value = 1;
    REQUIRE( e->value() == Approx( 2.0 ) );
  }

  SECTION( "Test sqrt" ) {
    numval_t e = manager.sqrt(v);
    v->_value = 1;
    REQUIRE( e->value() == Approx( 1.0 ) );
    v->_value = 4;
    REQUIRE( e->value() == Approx( 2.0 ) );
  }

  SECTION( "Test sin" ) {
    numval_t e = manager.sin(v);
    v->_value = 0;
    REQUIRE( e->value() == Approx( 0.0 ) );
    v->_value = PI/2.0;
    REQUIRE( e->value() == Approx( 1.0 ) );
  }

  SECTION( "Test cos" ) {
    numval_t e = manager.cos(v);
    v->_value = 0;
    REQUIRE( e->value() == Approx( 1.0 ) );
    v->_value = PI/2.0;
    REQUIRE( e->value() == Approx( 0.0 ).margin(1e-7) );
  }

  SECTION( "Test tan" ) {
    numval_t e = manager.tan(v);
    v->_value = 0;
    REQUIRE( e->value() == Approx( 0.0 ) );
    v->_value = PI/4.0;
    REQUIRE( e->value() == Approx( 1.0 ) );
  }

  SECTION( "Test asin" ) {
    numval_t e = manager.asin(v);
    v->_value = 0;
    REQUIRE( e->value() == Approx( 0.0 ) );
    v->_value = 1.0;
    REQUIRE( e->value() == Approx( PI/2.0 ) );
  }

  SECTION( "Test acos" ) {
    numval_t e = manager.acos(v);
    v->_value = 1;
    REQUIRE( e->value() == Approx( 0.0 ) );
    v->_value = 0;
    REQUIRE( e->value() == Approx( PI/2.0 ) );
  }

  SECTION( "Test atan" ) {
    numval_t e = manager.atan(v);
    v->_value = 0;
    REQUIRE( e->value() == Approx( 0.0 ) );
    v->_value = 1;
    REQUIRE( e->value() == Approx( PI/4.0 ) );
  }

  SECTION( "Test sinh" ) {
    numval_t e = manager.sinh(v);
    v->_value = 0;
    REQUIRE( e->value() == Approx( 0.0 ) );
    v->_value = 1;
    REQUIRE( e->value() == Approx( (E-1/E)/2.0 ) );
  }

  SECTION( "Test cosh" ) {
    numval_t e = manager.cosh(v);
    v->_value = 0;
    REQUIRE( e->value() == Approx( 1.0 ) );
    v->_value = 1;
    REQUIRE( e->value() == Approx( (E+1/E)/2.0 ) );
  }

  SECTION( "Test tanh" ) {
    numval_t e = manager.tanh(v);
    v->_value = 0;
    REQUIRE( e->value() == Approx( 0.0 ) );
    v->_value = 1;
    REQUIRE( e->value() == Approx( (E-1/E)/(E+1/E) ) );
  }

  SECTION( "Test asinh" ) {
    numval_t e = manager.asinh(v);
    v->_value = 0;
    REQUIRE( e->value() == Approx( 0.0 ) );
    v->_value = (E-1/E)/2.0;
    REQUIRE( e->value() == Approx( 1.0 ) );
  }

  SECTION( "Test acosh" ) {
    numval_t e = manager.acosh(v);
    v->_value = 1;
    REQUIRE( e->value() == Approx( 0.0 ) );
    v->_value = (E+1/E)/2.0;
    REQUIRE( e->value() == Approx( 1.0 ) );
  }

  SECTION( "Test atanh" ) {
    numval_t e = manager.atanh(v);
    v->_value = 0;
    REQUIRE( e->value() == Approx( 0.0 ) );
    v->_value = (E-1/E)/(E+1/E);
    REQUIRE( e->value() == Approx( 1.0 ) );
  }

}
