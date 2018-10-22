
#include "expr_types.hpp"
#include "catch.hpp"


TEST_CASE( "Test Values", "[smoke]" ) {

  SECTION( "Test Immutable Parameter" ) {

    // Create an immutable parameter
    TypedParameter<int> q( 2, false, "q");
    REQUIRE( q._value == 2 );
    REQUIRE( q._tvalue == 2 );
    REQUIRE( q.value() == 2 );

    // The immutable flag only is used in Python
    q._tvalue = 3;
    REQUIRE( q._tvalue == 3 );
    REQUIRE( q.value() == 3 );

    }

  SECTION( "Test Mutable Parameter" ) {

    // Create an mutable parameter
    TypedParameter<int> q( 2, true, "q");
    REQUIRE( q._value == 2 );
    REQUIRE( q._tvalue == 2 );
    REQUIRE( q.value() == 2 );

    // The mutable flag only is used in Python
    q._tvalue = 3;
    REQUIRE( q._tvalue == 3 );
    REQUIRE( q.value() == 3 );

    }

  SECTION( "Test Variable" ) {

    // Create a variable parameter
    Variable p( false, false, 0.0, 1.0, 2.0, "p");
    REQUIRE( p._value == 2.0 );
    REQUIRE( p.value() == 2.0 );

    // Set the value to 3
    p._value = 3.0;
    REQUIRE( p._value == 3.0 );
    REQUIRE( p.value() == 3.0 );

    }

  SECTION( "Test Expression" ) {
    Variable p( false, false, 0.0, 1.0, 2.0, "p");
    TypedParameter<int> q( 3, true, "q");
    AddExpression<NumericValue*, NumericValue*> e(&p, &q);

    REQUIRE( p.value() == 2.0 );
    REQUIRE( q.value() == 3.0 );
    REQUIRE( e.value() == 5.0 );
    }

  SECTION( "Test Constraint" ) {
    Variable p( false, false, 0.0, 1.0, 2.0, "p");
    TypedParameter<int> q( 3, true, "q");
    InequalityExpression e(&p, false);

    REQUIRE( p.value() == 2.0 );
    REQUIRE( e.value() == 2.0 );
    }

}


TEST_CASE( "Test AddExpression", "[smoke]" ) {

  Variable a( false, false, 0.0, 1.0, 0.0, "a");
  Variable b( false, false, 0.0, 1.0, 0.0, "b");
  Variable c( false, false, 0.0, 1.0, 0.0, "c");
  Variable d( false, false, 0.0, 1.0, 0.0, "d");

  SECTION( "Test simpleSum" ) {
    AddExpression<NumericValue*, NumericValue*> e(&a, &b);

    static std::list<std::string> baseline = {"[", "+", "a", "b", "]"};
    REQUIRE( expr_to_list(&e) == baseline );
  }

  SECTION( "Test simpleSum_API" ) {
    AddExpression<NumericValue*, NumericValue*> e1(&a, &b);
    TypedParameter<int> q( 2, true);
    MulExpression<NumericValue*, NumericValue*> e2(&q, &a);
    AddExpression<NumericValue*, NumericValue*> e(&e1, &e2);

    static std::list<std::string> baseline = {"[", "+", "[", "+", "a", "b", "]", "[", "*", "2", "a", "]", "]" };
    REQUIRE( expr_to_list(&e) == baseline );
  }

  SECTION( "Test constSum" ) {
    TypedParameter<int> q( 5, true);
    TypedParameter<double> Q( 5.0, true);

    WHEN( "e = a + 5" ) {
      AddExpression<NumericValue*, NumericValue*> e(&a, &q);

      static std::list<std::string> baseline = {"[", "+", "a", "5", "]"};
      REQUIRE( expr_to_list(&e) == baseline );
    }

    WHEN( "e = 5 + a" ) {
      AddExpression<NumericValue*, NumericValue*> e(&q, &a);

      static std::list<std::string> baseline = {"[", "+", "5", "a", "]"};
      REQUIRE( expr_to_list(&e) == baseline );
    }

    WHEN( "e = a + 5.0" ) {
      AddExpression<NumericValue*, NumericValue*> e(&a, &Q);

      static std::list<std::string> baseline = {"[", "+", "a", "5.000", "]"};
      REQUIRE( expr_to_list(&e) == baseline );
    }

    WHEN( "e = 5.0 + a" ) {
      AddExpression<NumericValue*, NumericValue*> e(&Q, &a);

      static std::list<std::string> baseline = {"[", "+", "5.000", "a", "]"};
      REQUIRE( expr_to_list(&e) == baseline );
    }
  }

  SECTION( "Test nestedSum" ) {
    TypedParameter<int> q( 5, true);

    WHEN( "e = (a+b)+5" ) {
      AddExpression<NumericValue*, NumericValue*> e1(&a, &b);
      AddExpression<NumericValue*, NumericValue*> e(&e1, &q);

      static std::list<std::string> baseline = {"[", "+", "[", "+", "a", "b", "]", "5", "]"};
      REQUIRE( expr_to_list(&e) == baseline );
    }

    WHEN( "e = 5+(a+b)" ) {
      AddExpression<NumericValue*, NumericValue*> e1(&a, &b);
      AddExpression<NumericValue*, NumericValue*> e(&q, &e1);

      static std::list<std::string> baseline = {"[", "+", "5", "[", "+", "a", "b", "]", "]"};
      REQUIRE( expr_to_list(&e) == baseline );
    }

    WHEN( "e = (a+b)+c" ) {
      AddExpression<NumericValue*, NumericValue*> e1(&a, &b);
      AddExpression<NumericValue*, NumericValue*> e(&e1, &c);

      static std::list<std::string> baseline = {"[", "+", "[", "+", "a", "b", "]", "c", "]"};
      REQUIRE( expr_to_list(&e) == baseline );
    }

    WHEN( "e = c+(a+b)" ) {
      AddExpression<NumericValue*, NumericValue*> e1(&a, &b);
      AddExpression<NumericValue*, NumericValue*> e(&c, &e1);

      static std::list<std::string> baseline = {"[", "+", "c", "[", "+", "a", "b", "]", "]"};
      REQUIRE( expr_to_list(&e) == baseline );
    }

    WHEN( "e = (a+b)+(c+d)" ) {
      AddExpression<NumericValue*, NumericValue*> e1(&a, &b);
      AddExpression<NumericValue*, NumericValue*> e2(&c, &d);
      AddExpression<NumericValue*, NumericValue*> e(&e1, &e2);

      static std::list<std::string> baseline = {"[", "+", "[", "+", "a", "b", "]", "[", "+", "c", "d", "]", "]"};
      REQUIRE( expr_to_list(&e) == baseline );
    }

  }

  SECTION( "Test nestedSum" ) {
    TypedParameter<int> q( 2, true);

    WHEN( "e = 2*(a+b)+c" ) {
      AddExpression<NumericValue*, NumericValue*> e1(&a, &b);
      MulExpression<NumericValue*, NumericValue*> e2(&q, &e1);
      AddExpression<NumericValue*, NumericValue*> e(&e2, &c);

      static std::list<std::string> baseline = {"[", "+", "[", "*", "2", "[", "+", "a", "b", "]", "]", "c", "]"};
      REQUIRE( expr_to_list(&e) == baseline );
    }

    WHEN( "e = 3*(2*(a+b)+c)" ) {
      TypedParameter<int> Q( 3, true);
      AddExpression<NumericValue*, NumericValue*> e1(&a, &b);
      MulExpression<NumericValue*, NumericValue*> e2(&q, &e1);
      AddExpression<NumericValue*, NumericValue*> e3(&e2, &c);
      MulExpression<NumericValue*, NumericValue*> e(&Q, &e3);

      static std::list<std::string> baseline = {"[", "*", "3", "[", "+", "[", "*", "2", "[", "+", "a", "b", "]", "]", "c", "]", "]"};
      REQUIRE( expr_to_list(&e) == baseline );
    }
  }

  //SECTION( "Test trivialSum" ) {
  // TODO: test the plus() method?
  //}

  SECTION( "Test nestedTrivialProduct" ) {
    TypedParameter<int> q( 5, true);

    WHEN( "e = a*5 + b" ) {
      MulExpression<NumericValue*, NumericValue*> e1(&a, &q);
      AddExpression<NumericValue*, NumericValue*> e(&e1, &b);

      static std::list<std::string> baseline = {"[", "+", "[", "*", "a", "5", "]", "b", "]"};
      REQUIRE( expr_to_list(&e) == baseline );
    }

    WHEN( "e = b + a*5" ) {
      MulExpression<NumericValue*, NumericValue*> e1(&a, &q);
      AddExpression<NumericValue*, NumericValue*> e(&b, &e1);

      static std::list<std::string> baseline = {"[", "+", "b", "[", "*", "a", "5", "]", "]"};
      REQUIRE( expr_to_list(&e) == baseline );
    }

    WHEN( "e = a*5 + (b+c)" ) {
      MulExpression<NumericValue*, NumericValue*> e1(&a, &q);
      AddExpression<NumericValue*, NumericValue*> e2(&b, &c);
      AddExpression<NumericValue*, NumericValue*> e(&e1, &e2);

      static std::list<std::string> baseline = {"[", "+", "[", "*", "a", "5", "]", "[", "+", "b", "c", "]", "]"};
      REQUIRE( expr_to_list(&e) == baseline );
    }

    WHEN( "e = (b+c) + a*5" ) {
      MulExpression<NumericValue*, NumericValue*> e1(&a, &q);
      AddExpression<NumericValue*, NumericValue*> e2(&b, &c);
      AddExpression<NumericValue*, NumericValue*> e(&e2, &e1);

      static std::list<std::string> baseline = {"[", "+", "[", "+", "b", "c", "]", "[", "*", "a", "5", "]",  "]"};
      REQUIRE( expr_to_list(&e) == baseline );
    }
  }

}


TEST_CASE( "Test DiffExpression", "[smoke]" ) {

  Variable a( false, false, 0.0, 1.0, 0.0, "a");
  Variable b( false, false, 0.0, 1.0, 0.0, "b");
  Variable c( false, false, 0.0, 1.0, 0.0, "c");
  Variable d( false, false, 0.0, 1.0, 0.0, "d");

  SECTION( "Test simpleDiff" ) {
    SubExpression e(&a, &b);

    static std::list<std::string> baseline = {"[", "-", "a", "b", "]"};
    REQUIRE( expr_to_list(&e) == baseline );
  }

  SECTION( "Test constDiff" ) {
    TypedParameter<int> q( 5, true);
    TypedParameter<double> Q( 5.0, true);

    WHEN( "e = a - 5" ) {
      SubExpression e(&a, &q);

      static std::list<std::string> baseline = {"[", "-", "a", "5", "]"};
      REQUIRE( expr_to_list(&e) == baseline );
      }

    WHEN( "e = 5 - a" ) {
      SubExpression e(&q, &a);

      static std::list<std::string> baseline = {"[", "-", "5", "a", "]"};
      REQUIRE( expr_to_list(&e) == baseline );
      }

    WHEN( "e = a - 5.0" ) {
      SubExpression e(&a, &Q);

      static std::list<std::string> baseline = {"[", "-", "a", "5.000", "]"};
      REQUIRE( expr_to_list(&e) == baseline );
      }

    WHEN( "e = 5.0 - a" ) {
      SubExpression e(&Q, &a);

      static std::list<std::string> baseline = {"[", "-", "5.000", "a", "]"};
      REQUIRE( expr_to_list(&e) == baseline );
      }
  }

  SECTION( "Test paramDiff" ) {
    TypedParameter<int> p( 5, true, "p");

    WHEN( "e = a - p" ) {
      SubExpression e(&a, &p);

      static std::list<std::string> baseline = {"[", "-", "a", "p", "]"};
      REQUIRE( expr_to_list(&e) == baseline );
      }

    WHEN( "e = p - a" ) {
      SubExpression e(&p, &a);

      static std::list<std::string> baseline = {"[", "-", "p", "a", "]"};
      REQUIRE( expr_to_list(&e) == baseline );
      }
  }

  SECTION( "Test termDiff" ) {
    TypedParameter<int> q( 2, true);
    TypedParameter<int> Q( 5, true);

    MulExpression<NumericValue*, NumericValue*> e1(&q, &a);
    SubExpression e(&Q, &e1);

    static std::list<std::string> baseline = {"[", "-", "5", "[", "*", "2", "a", "]", "]"};
    REQUIRE( expr_to_list(&e) == baseline );
  }

  SECTION( "Test nestedDiff" ) {
    TypedParameter<int> Q( 5, true);

    WHEN( "e = (a-b)-5" ) {
      SubExpression e1(&a, &b);
      SubExpression e(&e1, &Q);

      static std::list<std::string> baseline = {"[", "-", "[", "-", "a", "b", "]", "5", "]"};
      REQUIRE( expr_to_list(&e) == baseline );
    }

    WHEN( "e = 5-(a-b)" ) {
      SubExpression e1(&a, &b);
      SubExpression e(&Q, &e1);

      static std::list<std::string> baseline = {"[", "-", "5", "[", "-", "a", "b", "]", "]"};
      REQUIRE( expr_to_list(&e) == baseline );
    }

    WHEN( "e = (a-b)-c" ) {
      SubExpression e1(&a, &b);
      SubExpression e(&e1, &c);

      static std::list<std::string> baseline = {"[", "-", "[", "-", "a", "b", "]", "c", "]"};
      REQUIRE( expr_to_list(&e) == baseline );
    }

    WHEN( "e = (a-b)-(c-d)" ) {
      SubExpression e1(&a, &b);
      SubExpression e2(&c, &d);
      SubExpression e(&e1, &e2);

      static std::list<std::string> baseline = {"[", "-", "[", "-", "a", "b", "]", "[", "-", "c", "d", "]", "]"};
      REQUIRE( expr_to_list(&e) == baseline );
    }
  }

}


TEST_CASE( "Test NegExpression", "[smoke]" ) {

  SECTION( "Test negation_param" ) {
    TypedParameter<int> p( 2, false, "p");

    WHEN( "e = -p" ) {
      NegExpression e(&p);

      static std::list<std::string> baseline = {"[", "-", "p", "]"};
      REQUIRE( expr_to_list(&e) == baseline );
    }

    WHEN( "e = -(-p)" ) {
      NegExpression e1(&p);
      NegExpression e(&e1);

      static std::list<std::string> baseline = {"[", "-", "[", "-", "p", "]", "]"};
      REQUIRE( expr_to_list(&e) == baseline );
    }
  }

  SECTION( "Test negation_terms" ) {
    TypedParameter<int> p( 2, false, "p");
    TypedParameter<int> q( 5, false);
    Variable v( false, false, 0.0, 1.0, 0.0, "v");

    WHEN( "e = - p*v" ) {
      MulExpression<NumericValue*,NumericValue*> e1(&p,&v);
      NegExpression e(&e1);

      static std::list<std::string> baseline = {"[", "-", "[", "*", "p", "v", "]", "]"};
      REQUIRE( expr_to_list(&e) == baseline );
    }

    WHEN( "e = -(- p*v)" ) {
      MulExpression<NumericValue*,NumericValue*> e1(&p,&v);
      NegExpression e2(&e1);
      NegExpression e(&e2);

      static std::list<std::string> baseline = {"[", "-", "[", "-", "[", "*", "p", "v", "]", "]", "]"};
      REQUIRE( expr_to_list(&e) == baseline );
    }

    WHEN( "e = - 5*v" ) {
      MulExpression<NumericValue*,NumericValue*> e1(&q,&v);
      NegExpression e(&e1);

      static std::list<std::string> baseline = {"[", "-", "[", "*", "5", "v", "]", "]"};
      REQUIRE( expr_to_list(&e) == baseline );
    }

    WHEN( "e = -(- q*v)" ) {
      MulExpression<NumericValue*,NumericValue*> e1(&q,&v);
      NegExpression e2(&e1);
      NegExpression e(&e2);

      static std::list<std::string> baseline = {"[", "-", "[", "-", "[", "*", "5", "v", "]", "]", "]"};
      REQUIRE( expr_to_list(&e) == baseline );
    }
  }

  //SECTION( "Test trivialDiff" ) {
  // TODO: test the minus() method?
  //}

}
