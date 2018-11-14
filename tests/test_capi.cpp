
#include <cmath>
#include <memory>
#include <sstream>
#include <iostream>
extern "C" {
#include "coek_capi.h"
};
#include "admodel/simple.hpp"
#include "expr/expr_types.hpp"

#include "catch.hpp"

const double PI = 3.141592653589793238463;
const double E = exp(1.0);


TEST_CASE( "capi_misc", "[smoke]" ) {
  coek_initialize();

  SECTION( "Misc API functions" ) {
    REQUIRE( misc_getnull() == 0 );
    REQUIRE( std::isnan(misc_getnan()) );
  }

  SECTION( "API printing" ) {
      std::stringstream os;
      std::streambuf* coutbuf = std::cout.rdbuf();
      std::cout.rdbuf(os.rdbuf());

      WHEN( "variable" ) {
        apival_t a = create_variable(0, false, false, 0.0, 1.0, 0.0, "a");
        Variable* v = static_cast<Variable*>(a);
        v->index = 1;
        print_var(a);

        std::string tmp = os.str();
        REQUIRE( tmp == "a{0}\n" );
        REQUIRE( variable_get_index(v) == 1 );

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

  coek_finalize();
}


TEST_CASE( "capi_parameter", "[smoke]" ) {
  coek_initialize();

  SECTION( "constants" ) {
      apival_t model = create_model();

      WHEN( "parameter - 0" ) {
        apival_t q = get_parameter_zero(model);

        REQUIRE( expr_get_value(q) == 0 );
      }

      WHEN( "parameter - 1" ) {
        apival_t q = get_parameter_one(model);

        REQUIRE( expr_get_value(q) == 1 );
      }

      WHEN( "parameter - -1" ) {
        apival_t q = get_parameter_negative_one(model);

        REQUIRE( expr_get_value(q) == -1 );
      }
  }

  SECTION( "values" ) {
      WHEN( "parameter - 0" ) {
        apival_t q = get_parameter_zero(0);

        REQUIRE( expr_get_value(q) == 0 );
      }

      WHEN( "parameter - 1" ) {
        apival_t q = get_parameter_one(0);

        REQUIRE( expr_get_value(q) == 1 );
      }

      WHEN( "parameter - -1" ) {
        apival_t q = get_parameter_negative_one(0);

        REQUIRE( expr_get_value(q) == -1 );
      }

      WHEN( "parameter - 3" ) {
        apival_t q = create_parameter_int(0, 2, true, "q");
        parameter_set_value(q, 3);

        REQUIRE( expr_get_value(q) == 3 );
      }
  }

  coek_finalize();
}


TEST_CASE( "capi_variables", "[smoke]" ) {
  coek_initialize();

  SECTION( "values" ) {
      WHEN( "variable - 3" ) {
        apival_t a = create_variable(0, false, false, 0.0, 1.0, 0.0, "a");
        variable_set_value(a, 3);

        REQUIRE( variable_get_value(a) == 3 );
      }

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
  }

  SECTION( "bounds" ) {
      WHEN( "lb" ) {
        apival_t a = create_variable(0, false, false, 0.0, 1.0, 0.0, "a");
        REQUIRE( variable_get_lb(a) == 0.0 );
        variable_set_lb(a, 3.0);
        REQUIRE( variable_get_lb(a) == 3.0 );
      }

      WHEN( "ub" ) {
        apival_t a = create_variable(0, false, false, 0.0, 1.0, 0.0, "a");
        REQUIRE( variable_get_ub(a) == 1.0 );
        variable_set_ub(a, 3.0);
        REQUIRE( variable_get_ub(a) == 3.0 );
      }
  }

}


TEST_CASE( "capi_errors", "[smoke]" ) {
    coek_finalize();

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


TEST_CASE( "capi_add_expression", "[smoke]" ) {

  apival_t model = create_model();
  ExprManager* manager = &(static_cast<Simple_ADModel*>(model)->manager);

  apival_t a = create_variable(model, false, false, 0.0, 1.0, 0.0, "a");
  apival_t b = create_variable(model, false, false, 0.0, 1.0, 0.0, "b");
  apival_t c = create_variable(model, false, false, 0.0, 1.0, 0.0, "c");
  apival_t d = create_variable(model, false, false, 0.0, 1.0, 0.0, "d");

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
    apival_t q = create_parameter_int(model, 0, true, "q");
    apival_t Q = create_parameter_double(model, 0.0, true, "Q");
    apival_t q_ = create_parameter_int(model, 0, false, "");
    apival_t Q_ = create_parameter_double(model, 0.0, false, "");

    WHEN( "e = a + q{0}" ) {
      apival_t e = expr_plus_expression(a, q);

      static std::list<std::string> baseline = {"[","+","a","q","]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a + q_{0}" ) {
      apival_t e = expr_plus_expression(a, q_);

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

      static std::list<std::string> baseline = {"[","+","q","a","]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = q_{0} + a" ) {
      apival_t e = expr_plus_expression(q_, a);

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

      static std::list<std::string> baseline = {"[","+","a","Q","]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a + Q_{0.0}" ) {
      apival_t e = expr_plus_expression(a, Q_);

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

      static std::list<std::string> baseline = {"[","+","Q","a","]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = Q_{0.0} + a" ) {
      apival_t e = expr_plus_expression(Q_, a);

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


TEST_CASE( "capi_minus_expression", "[smoke]" ) {

  apival_t model = create_model();
  ExprManager* manager = &(static_cast<Simple_ADModel*>(model)->manager);

  apival_t a = create_variable(model, false, false, 0.0, 1.0, 0.0, "a");
  apival_t b = create_variable(model, false, false, 0.0, 1.0, 0.0, "b");
  apival_t c = create_variable(model, false, false, 0.0, 1.0, 0.0, "c");
  apival_t d = create_variable(model, false, false, 0.0, 1.0, 0.0, "d");

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
    apival_t p = create_parameter_int(model, 5, true, "p");

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
    apival_t q = create_parameter_int(model, 0, true, "q");
    apival_t Q = create_parameter_double(model, 0.0, true, "Q");
    apival_t q_ = create_parameter_int(model, 0, false, "");
    apival_t Q_ = create_parameter_double(model, 0.0, false, "");

    WHEN( "e = a - q{0}" ) {
      apival_t e = expr_minus_expression(a, q);

      static std::list<std::string> baseline = {"[","-","a","q","]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a - q_{0}" ) {
      apival_t e = expr_minus_expression(a, q_);

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

      static std::list<std::string> baseline = {"[","-","q","a","]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = q_{0} - a" ) {
      apival_t e = expr_minus_expression(q_, a);

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

      static std::list<std::string> baseline = {"[","-","a","Q","]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a - Q_{0.0}" ) {
      apival_t e = expr_minus_expression(a, Q_);

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

      static std::list<std::string> baseline = {"[","-","Q","a","]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = Q_{0.0} - a" ) {
      apival_t e = expr_minus_expression(Q_, a);

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

  apival_t model = create_model();
  ExprManager* manager = &(static_cast<Simple_ADModel*>(model)->manager);

  SECTION( "Test negation_param" ) {
    apival_t p = create_parameter_int(model, 2, false, "p");

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
    apival_t p = create_parameter_int(model, 2, false, "p");
    apival_t v = create_variable(model, false, false, 0.0, 1.0, 0.0, "v");

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

  apival_t model = create_model();
  ExprManager* manager = &(static_cast<Simple_ADModel*>(model)->manager);

  apival_t a = create_variable(model, false, false, 0.0, 1.0, 0.0, "a");
  apival_t b = create_variable(model, false, false, 0.0, 1.0, 0.0, "b");
  apival_t c = create_variable(model, false, false, 0.0, 1.0, 0.0, "c");
  apival_t d = create_variable(model, false, false, 0.0, 1.0, 0.0, "d");

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
    apival_t q = create_parameter_int(model, 0, true, "q");
    apival_t Q = create_parameter_double(model, 0.0, true, "Q");
    apival_t r = create_parameter_int(model, 1, true, "r");
    apival_t R = create_parameter_double(model, 1.0, true, "R");
    apival_t s = create_parameter_int(model, -1, true, "s");
    apival_t S = create_parameter_double(model, -1.0, true, "S");
    apival_t q_ = create_parameter_int(model, 0, false, "");
    apival_t Q_ = create_parameter_double(model, 0.0, false, "");
    apival_t r_ = create_parameter_int(model, 1, false, "");
    apival_t R_ = create_parameter_double(model, 1.0, false, "");
    apival_t s_ = create_parameter_int(model, -1, false, "");
    apival_t S_ = create_parameter_double(model, -1.0, false, "");

    WHEN( "e = a * q{0}" ) {
      apival_t e = expr_times_expression(a, q);

      static std::list<std::string> baseline = {"[","*","a","q","]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a * q_{0}" ) {
      apival_t e = expr_times_expression(a, q_);

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

      static std::list<std::string> baseline = {"[","*","q","a","]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = q_{0} * a" ) {
      apival_t e = expr_times_expression(q_, a);

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

      static std::list<std::string> baseline = {"[","*","a","Q","]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a * Q_{0.0}" ) {
      apival_t e = expr_times_expression(a, Q_);

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

      static std::list<std::string> baseline = {"[","*","Q","a","]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = Q_{0.0} * a" ) {
      apival_t e = expr_times_expression(Q_, a);

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

      static std::list<std::string> baseline = {"[","*","a","r","]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a * r_{1}" ) {
      apival_t e = expr_times_expression(a, r_);

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

      static std::list<std::string> baseline = {"[","*","r","a","]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = r_{1} * a" ) {
      apival_t e = expr_times_expression(r_, a);

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

      static std::list<std::string> baseline = {"[","*","a","R","]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a * R_{1.0}" ) {
      apival_t e = expr_times_expression(a, R_);

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

      static std::list<std::string> baseline = {"[","*","R","a","]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = R_{1.0} * a" ) {
      apival_t e = expr_times_expression(R_, a);

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

      static std::list<std::string> baseline = {"[","*","a","s","]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a * s_{-1}" ) {
      apival_t e = expr_times_expression(a, s_);

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

      static std::list<std::string> baseline = {"[","*","s","a","]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = s_{-1} * a" ) {
      apival_t e = expr_times_expression(s_, a);

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

      static std::list<std::string> baseline = {"[","*","a","S","]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a * S_{-1.0}" ) {
      apival_t e = expr_times_expression(a, S_);

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

      static std::list<std::string> baseline = {"[","*","S","a","]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = S_{-1.0} * a" ) {
      apival_t e = expr_times_expression(S_, a);

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

  apival_t model = create_model();
  ExprManager* manager = &(static_cast<Simple_ADModel*>(model)->manager);

  apival_t a = create_variable(model, false, false, 0.0, 1.0, 0.0, "a");
  apival_t b = create_variable(model, false, false, 0.0, 1.0, 0.0, "b");
  apival_t c = create_variable(model, false, false, 0.0, 1.0, 0.0, "c");
  apival_t d = create_variable(model, false, false, 0.0, 1.0, 0.0, "d");

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
    apival_t q = create_parameter_int(model, 0, true, "q");
    apival_t Q = create_parameter_double(model, 0.0, true, "Q");
    apival_t r = create_parameter_int(model, 1, true, "r");
    apival_t R = create_parameter_double(model, 1.0, true, "R");
    apival_t s = create_parameter_int(model, -1, true, "s");
    apival_t S = create_parameter_double(model, -1.0, true, "S");
    apival_t q_ = create_parameter_int(model, 0, false, "");
    apival_t Q_ = create_parameter_double(model, 0.0, false, "");
    apival_t r_ = create_parameter_int(model, 1, false, "");
    apival_t R_ = create_parameter_double(model, 1.0, false, "");
    apival_t s_ = create_parameter_int(model, -1, false, "");
    apival_t S_ = create_parameter_double(model, -1.0, false, "");

    WHEN( "e = q{0} / a" ) {
      apival_t e = expr_divide_expression(q, a);

      static std::list<std::string> baseline = {"[","/","q","a","]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = q_{0} / a" ) {
      apival_t e = expr_divide_expression(q_, a);

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

      static std::list<std::string> baseline = {"[","/","Q","a","]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = Q_{0.0} / a" ) {
      apival_t e = expr_divide_expression(Q_, a);

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

      static std::list<std::string> baseline = {"[","/","a","r","]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a/ r_{1}" ) {
      apival_t e = expr_divide_expression(a, r_);

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

      static std::list<std::string> baseline = {"[","/","a","R","]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a / R_{1.0}" ) {
      apival_t e = expr_divide_expression(a, R_);

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

      static std::list<std::string> baseline = {"[","/","a","s","]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a/ s_{-1}" ) {
      apival_t e = expr_divide_expression(a, s_);

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

      static std::list<std::string> baseline = {"[","/","a","S","]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a / S_{-1.0}" ) {
      apival_t e = expr_divide_expression(a, S_);

      static std::list<std::string> baseline = {"[", "-", "a", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a / -1.0" ) {
      apival_t e = expr_divide_double(a, -1.0);

      static std::list<std::string> baseline = {"[", "-", "a", "]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = 3 / q_" ) {
      apival_t e = expr_rdivide_int(3, q_);

      REQUIRE( e == 0 );
    }

    WHEN( "e = 3.0 / q_" ) {
      apival_t e = expr_rdivide_double(3.0, q_);

      REQUIRE( e == 0 );
    }

  }

}


TEST_CASE( "capi_pow_expression", "[smoke]" ) {

  apival_t model = create_model();
  ExprManager* manager = &(static_cast<Simple_ADModel*>(model)->manager);

  apival_t a = create_variable(model, false, false, 0.0, 1.0, 0.0, "a");
  apival_t b = create_variable(model, false, false, 0.0, 1.0, 0.0, "b");

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
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = q_{0} ** a" ) {
      apival_t e = expr_pow_expression(q_, a);

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

      static std::list<std::string> baseline = {"[","**","Q","a","]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = Q_{0.0} ** a" ) {
      apival_t e = expr_pow_expression(Q_, a);

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

      static std::list<std::string> baseline = {"[","**","r","a","]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = r_{1} ** a" ) {
      apival_t e = expr_pow_expression(r_, a);

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

      static std::list<std::string> baseline = {"[","**","R","a","]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = R_{1.0} ** a" ) {
      apival_t e = expr_pow_expression(R_, a);

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

      static std::list<std::string> baseline = {"[","**","a","q","]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a ** q_{0}" ) {
      apival_t e = expr_pow_expression(a, q_);

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

      static std::list<std::string> baseline = {"[","**","a","Q","]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a ** Q_{0.0}" ) {
      apival_t e = expr_pow_expression(a, Q_);

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

      static std::list<std::string> baseline = {"[","**","a","r","]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a ** r_{1}" ) {
      apival_t e = expr_pow_expression(a, r_);

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

      static std::list<std::string> baseline = {"[","**","a","R","]"};
      REQUIRE( manager->expr_to_list(e,false) == baseline );
    }

    WHEN( "e = a ** R_{1.0}" ) {
      apival_t e = expr_pow_expression(a, R_);

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

  apival_t model = create_model();
  ExprManager* manager = &(static_cast<Simple_ADModel*>(model)->manager);

  apival_t v = create_variable(model, false, false, 0.0, 1.0, 0.0, "v");
  apival_t p = create_variable(model, false, false, 0.0, 1.0, 0.0, "p");

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

  apival_t model = create_model();
  ExprManager* manager = &(static_cast<Simple_ADModel*>(model)->manager);

  apival_t v = create_variable(model, false, false, 0.0, 1.0, 0.0, "v");
  apival_t p = create_variable(model, false, false, 0.0, 1.0, 0.0, "p");

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


TEST_CASE( "capi_compute", "[smoke]" ) {

  apival_t model = create_model();

  apival_t a = create_variable(model, false, false, 0.0, 1.0, 0.0, "a");
  apival_t b = create_variable(model, false, false, 0.0, 1.0, 1.0, "b");
  apival_t q = create_parameter_int(0, 2, true, "q");

  SECTION( "expression" ) {

    WHEN( "e = q" ) {
        apival_t e = q;
        REQUIRE( compute_numval_value(e) == 2 );
    }

    WHEN( "e = a" ) {
        apival_t e = a;
        REQUIRE( compute_numval_value(e) == 0 );
    }

    WHEN( "e = 3*b + q" ) {
        apival_t e = expr_plus_expression( expr_rtimes_int(3, b), q);
        REQUIRE( compute_numval_value(e) == 5.0 );
    }

  }

  SECTION( "constraint" ) {

    WHEN( "e = 3*b + q == 0" ) {
        apival_t e = create_equality(expr_plus_expression( expr_rtimes_int(3, b), q));
        REQUIRE( compute_constraint_value(e, true) == 5.0 );
        REQUIRE( compute_constraint_value(e, false) == 0 );
    }

    WHEN( "e = 3*b + q <= 0" ) {
        apival_t e = create_inequality(expr_plus_expression( expr_rtimes_int(3, b), q), false);
        REQUIRE( compute_constraint_value(e, true) == 5.0 );
        REQUIRE( compute_constraint_value(e, false) == 0 );
    }

  }

  coek_finalize();
}


TEST_CASE( "capi_model", "[smoke]" ) {

  coek_finalize();
  apival_t model = create_model();
  Simple_ExprModel* _model = static_cast<Simple_ADModel*>(model);

  //apival_t a = create_variable(model, false, false, 0.0, 1.0, 0.0, "a");
  apival_t b = create_variable(model, false, false, 0.0, 1.0, 1.0, "b");
  apival_t q = create_parameter_int(0, 2, true, "q");

  SECTION( "add" ) {

    WHEN( "objective" ) {
        apival_t e = expr_plus_expression( expr_rtimes_int(3, b), q);
        REQUIRE( _model->objectives.size() == 0 );
        add_objective(model, e);
        REQUIRE( _model->objectives.size() == 1 );
    }

    WHEN( "inequality" ) {
        apival_t e = create_inequality(expr_plus_expression( expr_rtimes_int(3, b), q), false);
        REQUIRE( _model->inequalities.size() == 0 );
        add_inequality(model, e);
        REQUIRE( _model->inequalities.size() == 1 );
    }

    WHEN( "equality" ) {
        apival_t e = create_equality(expr_plus_expression( expr_rtimes_int(3, b), q));
        REQUIRE( _model->equalities.size() == 0 );
        add_equality(model, e);
        REQUIRE( _model->equalities.size() == 1 );
    }

  }

  SECTION( "print" ) {

    WHEN( "df == 0" ) {
        apival_t e = expr_plus_expression( expr_rtimes_int(3, b), q);
        add_objective(model, e);
        e = create_inequality(expr_plus_expression( expr_rtimes_int(3, b), q), false);
        add_inequality(model, e);
        e = create_equality(expr_plus_expression( expr_rtimes_int(3, b), q));
        add_equality(model, e);

        std::stringstream os;
        std::streambuf* coutbuf = std::cout.rdbuf();
        std::cout.rdbuf(os.rdbuf());

        print_model(model, 0);
        std::string tmp = os.str();
        REQUIRE( tmp == "MODEL\n\
  Objectives\n\
    x{3}*b{1} + q{2}\n\
\n\
  Inequality Constraints\n\
    x{3}*b{1} + q{2}  <=  0\n\
\n\
  Equality Constraints\n\
    x{3}*b{1} + q{2}  ==  0\n\
" );

        std::cout.rdbuf(coutbuf);
    }
  }

}
