
#include <typeinfo>
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


TEST_CASE( "model_variable", "[smoke]" ) {

  SECTION( "constructors" ) {
    WHEN( "simple" ) {
        auto a = coek::variable().lower(0).upper(1).value(2);
        REQUIRE( a.value() == 2 );
        REQUIRE( a.lower() == 0 );
        REQUIRE( a.upper() == 1 );
        // Keep this test?  The integer index depends on whether this test is run first.
        REQUIRE( a.name() == "x(0)" );
    }

    WHEN( "named" ) {
        auto a = coek::variable("test").lower(0).upper(1).value(2);
        REQUIRE( a.value() == 2 );
        REQUIRE( a.lower() == 0 );
        REQUIRE( a.upper() == 1 );
        REQUIRE( a.name() == "test" );
    }

    WHEN( "simple_with_expr_values" ) {
        auto p = coek::parameter();
        auto a = coek::variable().lower(p).upper(p+1).value(p+2);
        REQUIRE( a.value() == 2 );
        REQUIRE( a.lower() == 0 );
        REQUIRE( a.upper() == 1 );
        p.value(1);
        REQUIRE( a.value() == 3 );
        REQUIRE( a.lower() == 1 );
        REQUIRE( a.upper() == 2 );
    }

    WHEN( "named_with_expr_values" ) {
        auto p = coek::parameter();
        auto a = coek::variable("test").lower(p).upper(p+1).value(p+2);
        REQUIRE( a.value() == 2 );
        REQUIRE( a.lower() == 0 );
        REQUIRE( a.upper() == 1 );
        p.value(1);
        REQUIRE( a.value() == 3 );
        REQUIRE( a.lower() == 1 );
        REQUIRE( a.upper() == 2 );
    }

    WHEN( "copy" ) {
        auto a = coek::variable("test").lower(0).upper(1).value(2);
        coek::Variable b(a);
        REQUIRE( b.value() == 2 );
        REQUIRE( b.lower() == 0 );
        REQUIRE( b.upper() == 1 );
        REQUIRE( b.name() == "test" );
        REQUIRE( a.id() == b.id() );
    }

    WHEN( "equal" ) {
        auto a = coek::variable("test").lower(0).upper(1).value(2);
        auto b = coek::variable();
        b = a;
        REQUIRE( b.value() == 2 );
        REQUIRE( b.lower() == 0 );
        REQUIRE( b.upper() == 1 );
        REQUIRE( b.name() == "test" );
        REQUIRE( a.id() == b.id() );
    }
  }

  SECTION( "index" ) {
    coek::Model model;
    auto a = coek::variable("a").lower(0).upper(1).value(0);
    model.add_variable(a);
    auto b = coek::variable("b").lower(0).upper(1).value(0);
    REQUIRE( a.id() == (b.id() - 1) );
  }

  SECTION( "values" ) {
      WHEN( "variable - 3" ) {
        auto a = coek::variable("a").lower(0).upper(1).value(0);
        REQUIRE( a.value() == 0 );
        a.value(3);
        REQUIRE( a.value() == 3 );
      }

/*
      WHEN( "variable array - 3" ) {
        void* a[2];
        create_variable_array(0, a, 2, false, false, 0.0, 1.0, 0.0, "a");
        variable_value(a[0], 3);

        REQUIRE( variable_value(a[0]) == 3 );
        REQUIRE( variable_value(a[1]) == 0 );
      }

      WHEN( "variable array (2) - 3" ) {
        apival_t model = create_model();

        void* a[2];
        create_variable_array(model, a, 2, false, false, 0.0, 1.0, 0.0, "a");
        variable_value(a[0], 3);

        REQUIRE( variable_value(a[0]) == 3 );
        REQUIRE( variable_value(a[1]) == 0 );
      }
*/
  }

  SECTION( "bounds" ) {
      WHEN( "lb" ) {
        auto a = coek::variable("a").lower(0).upper(1).value(0);
        REQUIRE( a.lower() == 0.0 );
        a.lower(3.0);
        REQUIRE( a.lower() == 3.0 );
      }

      WHEN( "ub" ) {
        auto a = coek::variable("a").lower(0).upper(1).value(0);
        REQUIRE( a.upper() == 1.0 );
        a.upper(3.0);
        REQUIRE( a.upper() == 3.0 );
      }
  }

  SECTION( "properties" ) {
    WHEN( "continuous" ) {
        auto a = coek::variable();
        REQUIRE( a.is_continuous() == true );
        REQUIRE( a.is_binary() == false );
        REQUIRE( a.is_integer() == false );
    }

    WHEN( "binary" ) {
        auto a = coek::variable("a").lower(0).upper(1).value(0).within(coek::Binary);;
        REQUIRE( a.is_continuous() == false );
        REQUIRE( a.is_binary() == true );
        REQUIRE( a.is_integer() == false );
    }

    WHEN( "integer" ) {
        auto a = coek::variable("a").lower(0).upper(10).value(5).within(coek::Integers);;
        REQUIRE( a.is_continuous() == false );
        REQUIRE( a.is_binary() == false );
        REQUIRE( a.is_integer() == true );
    }
  
    WHEN( "fixed" ) {
        auto a = coek::variable("a").lower(0).upper(10).value(5).within(coek::Integers);;
        REQUIRE( a.fixed() == false );
        a.fixed(true);
        REQUIRE( a.fixed() == true );
        REQUIRE( a.value() == 5 );
        a.fix(3);
        REQUIRE( a.fixed() == true );
        REQUIRE( a.value() == 3 );
    }
  }

  SECTION( "write" ) {
    std::stringstream sstr;
    auto q = coek::variable("q");
    sstr << q;
    REQUIRE( sstr.str() == "q" );
  }

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}


#ifdef DEBUG
#define ENV_MEMCHECK REQUIRE( coek::env.check_memory() == true )
#else
#define ENV_MEMCHECK 
#endif


#ifdef COEK_WITH_COMPACT_MODEL
TEST_CASE( "1D_var_map", "[smoke]" ) {

  SECTION( "int" ) {
      std::vector<int> v = {1,5,3,7};

      WHEN( "constructors" ) {
        auto s = coek::SetOf( v );
        auto v1 = coek::variable( s );
        REQUIRE( v1.size() == 4 );
        auto v2 = coek::variable("v2", s );
        REQUIRE( v2.size() == 4 );
        auto v3 = coek::variable_map( s );
        REQUIRE( v3.size() == 4 );
        auto v4 = coek::variable_map("v4", s );
        REQUIRE( v4.size() == 4 );
      }

      WHEN( "size" ) {
        auto s = coek::SetOf( v );
        auto vars = coek::variable( s );
        REQUIRE( vars.size() == 4 );
      }

      WHEN( "typeof" ) {
        auto s = coek::SetOf( v );
        auto vars = coek::variable( s );
        REQUIRE( typeid(vars(1)).name() == typeid(coek::Variable).name() );
      }

      WHEN( "index" ) {
        auto s = coek::SetOf( v );
        auto i = coek::set_index("i");
        auto j = coek::set_index("j");

        std::vector<int> vals(4);
        size_t ii=0;
        for (auto it=s.begin({i}); it != s.end(); ++it) {
            i.get_value(vals[ii++]);
            }
        REQUIRE( v == vals );
        }

      WHEN( "index_error" ) {
        auto s = coek::SetOf( v );
        auto i = coek::set_index("i");
        auto j = coek::set_index("j");
        CHECK_THROWS( s.begin({i,j}) );
        }
  }

  SECTION( "int_ranged" ) {
      std::vector<int> v = {1,3,5,7};
      auto s = coek::RangeSet(1, 7, 2);

      WHEN( "size" ) {
        auto vars = coek::variable( s );
        REQUIRE( vars.size() == 4 );
      }

      WHEN( "typeof" ) {
        auto vars = coek::variable( s );
        REQUIRE( typeid(vars(1)).name() == typeid(coek::Variable).name() );
      }

      WHEN( "index" ) {
        auto vars = coek::variable( s ).value(1);
        std::vector<int> vals(4);

        auto i = coek::set_index("i");
        size_t ii=0;
        for (auto it=s.begin({i}); it != s.end(); ++it)
            i.get_value(vals[ii++]);
        REQUIRE( v == vals );
        }

      WHEN( "index_error" ) {
        auto i = coek::set_index("i");
        auto j = coek::set_index("j");
        CHECK_THROWS( s.begin({i,j}) );
        }
  }

  SECTION( "abstract" ) {
      std::vector<int> v = {1,5,3,7};
      auto s = coek::SetOf( v );
      auto r = coek::SetOf( {0} );
      auto S = s - r;
      auto vars = coek::variable("vars",S).lower(0).upper(1).value(0);

      WHEN( "typeof" ) {
        REQUIRE( typeid(vars(1)).name() == typeid(coek::Variable).name() );
      }

      WHEN( "index" ) {
        auto s = coek::SetOf( v );
        auto i = coek::set_index("i");
        auto j = coek::set_index("j");
        CHECK_THROWS( s.begin({i,j}) );
        }

      WHEN( "index1" ) {
        auto i = coek::set_index("i");
        REQUIRE( typeid(vars(1)).name() == typeid(coek::Variable).name() );
        REQUIRE( typeid(vars(i)).name() == typeid(coek::Expression).name() );
      }

      WHEN( "index2" ) {
        auto v = coek::variable();
        coek::Expression f = v;
        auto e = vars(1);
        //static std::list<std::string> baseline = {"vars(1)"};
        REQUIRE( e.name() == "vars(1)");
        //auto it = e.to_list().begin();
        //REQUIRE_THAT( *it, Catch::Matchers::StartsWith("vars") );
      }

      WHEN( "index3" ) {
        auto i = coek::set_index("i");
        auto e = vars(i);
        static std::list<std::string> baseline = {"vars(i)"};
        REQUIRE( e.to_list() == baseline);
      }

      WHEN( "index4" ) {
        auto i = coek::set_index("i");
        auto e = vars(i+1);
        static std::list<std::string> baseline = {"vars(i + 1)"};
        REQUIRE( e.to_list() == baseline);
      }
  }

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}


TEST_CASE( "1D_var_array", "[smoke]" ) {

  SECTION( "int_vector" ) {

      WHEN( "constructors" ) {
        auto v1 = coek::variable( 4 );
        REQUIRE( v1.size() == 4 );
        auto v2 = coek::variable("v2", 4 );
        REQUIRE( v2.size() == 4 );
        auto v3 = coek::variable_array( 4 );
        REQUIRE( v3.size() == 4 );
        auto v4 = coek::variable_array("v4", 4 );
        REQUIRE( v4.size() == 4 );
      }

      WHEN( "size" ) {
        auto vars = coek::variable( 4 );
        REQUIRE( vars.size() == 4 );
      }

      WHEN( "typeof" ) {
        auto vars = coek::variable( 4 );
        REQUIRE( typeid(vars(1)).name() == typeid(coek::Variable).name() );
      }

      WHEN( "index" ) {
        auto vars = coek::variable( 4 ).value(1);
        for (size_t i=0; i<4; i++)
            REQUIRE( vars(i).value() == 1 );
        for (int i=0; i<4; i++)
            REQUIRE( vars(i).value() == 1 );
        }
  }

  SECTION( "int_vector_dim" ) {
      std::vector<int> v = {0,1,2,3};

      WHEN( "size" ) {
        std::vector<size_t> dim {4};
        auto vars = coek::variable( "v" , dim );
        REQUIRE( vars.size() == 4 );
      }

      WHEN( "typeof" ) {
        std::vector<size_t> dim {4};
        auto vars = coek::variable( dim );
        REQUIRE( typeid(vars(1)).name() == typeid(coek::Variable).name() );
      }

      WHEN( "index" ) {
        std::vector<size_t> dim {4};
        auto vars = coek::variable(dim).value(1);
        for (size_t i=0; i<4; i++)
            REQUIRE( vars(i).value() == 1 );
        }
  }

  SECTION( "int_initializer_list" ) {
      WHEN( "size" ) {
        std::vector<size_t> dim {4};
        auto vars = coek::variable( "v" , {4} );
        REQUIRE( vars.size() == 4 );
      }

      WHEN( "typeof" ) {
        auto vars = coek::variable( {4} );
        REQUIRE( typeid(vars(1)).name() == typeid(coek::Variable).name() );
      }

      WHEN( "index" ) {
        auto vars = coek::variable({4}).value(1);
        for (size_t i=0; i<4; i++)
            REQUIRE( vars(i).value() == 1 );
        }
  }

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}


TEST_CASE( "2D_var_map", "[smoke]" ) {

  SECTION( "int" ) {
      std::vector<int> v = {1,5,3,7};
      std::vector<int> w = {2,6,4,8};

      WHEN( "constructors" ) {
        auto V = coek::SetOf( v );
        auto W = coek::SetOf( w );
        auto S = V*W;
        auto v1 = coek::variable( S );
        REQUIRE( v1.size() == 16 );
        auto v2 = coek::variable("v2", S );
        REQUIRE( v2.size() == 16 );
        auto v3 = coek::variable_map( S );
        REQUIRE( v3.size() == 16 );
        auto v4 = coek::variable_map("v4", S );
        REQUIRE( v4.size() == 16 );
      }

      WHEN( "size" ) {
        auto V = coek::SetOf( v );
        auto W = coek::SetOf( w );
        auto S = V*W;
        auto vars = coek::variable( S );
        REQUIRE( vars.size() == 16 );
      }

      WHEN( "typeof" ) {
        auto V = coek::SetOf( v );
        auto W = coek::SetOf( w );
        auto S = V*W;
        auto vars = coek::variable( S );
        REQUIRE( typeid(vars(1,2)).name() == typeid(coek::Variable).name() );
      }

      WHEN( "index" ) {
        auto V = coek::SetOf( v );
        auto W = coek::SetOf( w );
        auto S = V*W;
        auto i = coek::set_index("i");
        auto j = coek::set_index("j");

        std::set<int> ivals;
        std::set<int> jvals;
        for (auto it=S.begin({i,j}); it != S.end(); ++it) {
            int tmp;
            i.get_value(tmp);
            ivals.insert(tmp);
            j.get_value(tmp);
            jvals.insert(tmp);
            }

        std::set<int> vset(v.begin(), v.end());
        std::set<int> wset(w.begin(), w.end());
        REQUIRE( vset == ivals );
        REQUIRE( wset == jvals );
        }

      WHEN( "set_index_error" ) {
        auto V = coek::SetOf( v );
        auto W = coek::SetOf( w );
        auto S = V*W;
        auto i = coek::set_index("i");
        auto j = coek::set_index("j");
        CHECK_THROWS( S.begin({i}) );
        }

      WHEN( "var_index_error" ) {
        auto V = coek::SetOf( v );
        auto W = coek::SetOf( w );
        auto vars = coek::variable( V*W ).name("x");
        REQUIRE( vars.dim() == 2 );
        REQUIRE_THROWS_WITH( vars(0), 
            "Unexpected index value: x is an 2-D variable map but is being indexed with 1 indices.");
        } 
  }

  SECTION( "abstract" ) {
      std::vector<int> v = {1,5,3,7};
      std::vector<int> w = {2,6,4,8};
      auto V = coek::SetOf( v );
      auto W = coek::SetOf( w );
      auto r = coek::SetOf( {0} );
      auto S = V*W;
      auto vars = coek::variable("vars",S).lower(0).upper(1).value(0);

      WHEN( "typeof" ) {
        REQUIRE( typeid(vars(1,2)).name() == typeid(coek::Variable).name() );
      }

      WHEN( "index" ) {
        auto V = coek::SetOf( v );
        auto W = coek::SetOf( w );
        auto S = V*W;
        auto i = coek::set_index("i");
        auto j = coek::set_index("j");
        CHECK_THROWS( S.begin({i}) );
        S.begin({i,j});
        }

      WHEN( "index1" ) {
        auto i = coek::set_index("i");
        auto j = coek::set_index("j");
        REQUIRE( typeid(vars(1,2)).name() == typeid(coek::Variable).name() );
        REQUIRE( typeid(vars(10,11)).name() == typeid(coek::Variable).name() );
        REQUIRE( typeid(vars(i,j)).name() == typeid(coek::Expression).name() );
      }

      WHEN( "index2" ) {
        auto e = vars(1,2);
        REQUIRE( e.name() == "vars(1,2)");
        //auto it = e.to_list().begin();
        //REQUIRE_THAT( *it, Catch::Matchers::StartsWith("vars") );
      }

      WHEN( "index3" ) {
        auto i = coek::set_index("i");
        auto j = coek::set_index("j");
        auto e = vars(i,j);
        static std::list<std::string> baseline = {"vars(i,j)"};
        REQUIRE( e.to_list() == baseline);
      }

      WHEN( "index4" ) {
        auto i = coek::set_index("i");
        auto j = coek::set_index("j");
        auto e = vars(i+1,j-1);
        static std::list<std::string> baseline = {"vars(i + 1,j + -1)"};
        REQUIRE( e.to_list() == baseline);
      }
  }

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}


TEST_CASE( "2D_var_array", "[smoke]" ) {

  SECTION( "int_vector_dim" ) {

      WHEN( "constructors" ) {
        auto v1 = coek::variable( {4,4} );
        REQUIRE( v1.size() == 16 );
        auto v2 = coek::variable("v2", {4,4} );
        REQUIRE( v2.size() == 16 );
        auto v3 = coek::variable_array( {4,4} );
        REQUIRE( v3.size() == 16 );
        auto v4 = coek::variable_array("v4", {4,4} );
        REQUIRE( v4.size() == 16 );
      }

      WHEN( "size" ) {
        std::vector<size_t> dim {4,3};
        auto vars = coek::variable( "v" , dim );
        REQUIRE( vars.size() == 12 );
      }

      WHEN( "typeof" ) {
        std::vector<size_t> dim {4,3};
        auto vars = coek::variable(dim);
        REQUIRE( typeid(vars(1,1)).name() == typeid(coek::Variable).name() );
      }

      WHEN( "index" ) {
        std::vector<size_t> dim {4,5};
        auto vars = coek::variable(dim).value(1);
        for (size_t i=0; i<4; i++)
            REQUIRE( vars(i,i).value() == 1 );
        }

      WHEN( "index error" ) {
        std::vector<size_t> dim {4,3};
        auto vars = coek::variable(dim).name("x");
        REQUIRE( vars.dim() == 2 );
        REQUIRE_THROWS_WITH( vars(0), 
            "Unexpected index value: x is an 2-D variable array but is being indexed with 1 indices.");
        }
  }

  SECTION( "int_initializer_list" ) {
      WHEN( "size" ) {
        auto vars = coek::variable( "v" , {4,3} );
        REQUIRE( vars.size() == 12 );
      }

      WHEN( "typeof" ) {
        auto vars = coek::variable({4,3});
        REQUIRE( typeid(vars(1,1)).name() == typeid(coek::Variable).name() );
      }

      WHEN( "index" ) {
        auto vars = coek::variable({4,4}).value(1);
        for (size_t i=0; i<4; i++)
            REQUIRE( vars(i,i).value() == 1 );
        }

      WHEN( "index error" ) {
        auto vars = coek::variable({4,3}).name("x");
        REQUIRE( vars.dim() == 2 );
        REQUIRE_THROWS_WITH( vars(0), 
            "Unexpected index value: x is an 2-D variable array but is being indexed with 1 indices.");
        }
  }

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}


TEST_CASE( "3D_var_api", "[smoke]" ) {

  SECTION( "map" ) {
      std::vector<int> vec = {1,2,3,4};
      auto p = coek::parameter("p").value(1);

      WHEN( "add model" ) {
        auto V = coek::SetOf( vec );
        auto v = coek::variable_map( V*V*V ).name("x").value(1);
        coek::Model model;
        auto vv = model.add( v );
        REQUIRE( vv.size() == 64 );
        }

      WHEN( "index api" ) {
        auto V = coek::SetOf( vec );
        auto v = coek::variable_map( V*V*V ).name("x").value(1);
        REQUIRE( v.size() == 64 );

        int ival = 1;
        size_t uval = 1;
        auto e = p+1;
        // Misc tests to improve code coverage for the collect_args() methods
        REQUIRE( v(e,uval,ival).expand().value() == 1 );

        REQUIRE( v(uval,uval,e).expand().value() == 1 );

        REQUIRE( v(ival,ival,e).expand().value() == 1 );

        REQUIRE( v(uval,uval,uval).value() == 1 );

        REQUIRE( v(ival,ival,ival).value() == 1 );

        REQUIRE_THROWS_WITH( v(uval,uval),
            "Unexpected index value: x is an 3-D variable map but is being indexed with 2 indices.");

        REQUIRE_THROWS_WITH( v(ival,ival),
            "Unexpected index value: x is an 3-D variable map but is being indexed with 2 indices.");

        REQUIRE_THROWS_WITH( v(e,ival),
            "Unexpected index value: x is an 3-D variable map but is being indexed with 2 indices.");

        REQUIRE_THROWS_WITH( v(ival,ival,-1),
            "Unknown index value: x(1,1,-1)");
      }

    WHEN( "var value" ) {
        auto q = coek::parameter("q").value(1);
        auto V = coek::SetOf( vec );
        auto v = coek::variable_map( V ).name("x").value(1);

        REQUIRE( v(1).value() == 1 );
        v.value(q+1);
        REQUIRE( v(1).value() == 2 );
        q.value(2);
        REQUIRE( v(1).value() == 3 );
        }

    WHEN( "var lower" ) {
        auto q = coek::parameter("q").value(1);
        auto V = coek::SetOf( vec );
        auto v = coek::variable_map( V ).name("x").lower(1);

        REQUIRE( v(1).lower() == 1 );
        v.lower(q+1);
        REQUIRE( v(1).lower() == 2 );
        q.value(2);
        REQUIRE( v(1).lower() == 3 );
        }

    WHEN( "var upper" ) {
        auto q = coek::parameter("q").value(1);
        auto V = coek::SetOf( vec );
        auto v = coek::variable_map( V ).name("x").upper(1);

        REQUIRE( v(1).upper() == 1 );
        v.upper(q+1);
        REQUIRE( v(1).upper() == 2 );
        q.value(2);
        REQUIRE( v(1).upper() == 3 );
        }

    WHEN( "var bounds" ) {
        auto q = coek::parameter("q").value(1);
        auto V = coek::SetOf( vec );
        auto v = coek::variable_map( V ).name("x").bounds(1,2);

        REQUIRE( v(1).lower() == 1 );
        REQUIRE( v(1).upper() == 2 );
        v.bounds(q+1,q+2);
        REQUIRE( v(1).lower() == 2 );
        REQUIRE( v(1).upper() == 3 );
        q.value(2);
        REQUIRE( v(1).lower() == 3 );
        REQUIRE( v(1).upper() == 4 );
        v.bounds(q+1,10);
        REQUIRE( v(1).lower() == 3 );
        REQUIRE( v(1).upper() == 10 );
        v.bounds(10,10*q);
        REQUIRE( v(1).lower() == 10 );
        REQUIRE( v(1).upper() == 20 );
        }

    WHEN( "var within" ) {
        auto q = coek::parameter("q").value(1);
        auto V = coek::SetOf( vec );
        auto v = coek::variable_map( V ).name("x").within(coek::Reals);

        REQUIRE( v(1).within() == coek::Reals );
        v.within(coek::Integers);
        REQUIRE( v(1).within() == coek::Integers );
        }
  }

  SECTION( "array" ) {
      auto p = coek::parameter("p").value(1);

      WHEN( "add model" ) {
        auto v = coek::variable_array( {4,4,4} ).name("x").value(1);
        coek::Model model;
        auto vv = model.add( v );
        REQUIRE( vv.size() == 64 );
        }

      WHEN( "index api" ) {
        auto v = coek::variable_array({4,4,4}).name("x").value(1);
        REQUIRE( v.size() == 64 );

        int ival = 1;
        size_t uval = 1;
        auto e = p+1;
        // Misc tests to improve code coverage for the collect_args() methods
        REQUIRE( v(e,uval,ival).expand().value() == 1 );

        REQUIRE( v(uval,uval,e).expand().value() == 1 );

        REQUIRE( v(ival,ival,e).expand().value() == 1 );

        REQUIRE( v(uval,uval,uval).value() == 1 );

        REQUIRE( v(ival,ival,ival).value() == 1 );

        REQUIRE_THROWS_WITH( v(uval),
            "Unexpected index value: x is an 3-D variable array but is being indexed with 1 indices.");

        REQUIRE_THROWS_WITH( v(uval,uval),
            "Unexpected index value: x is an 3-D variable array but is being indexed with 2 indices.");

        REQUIRE_THROWS_WITH( v(e,ival),
            "Unexpected index value: x is an 3-D variable array but is being indexed with 2 indices.");

        REQUIRE_THROWS_WITH( v(ival,ival),
            "Unexpected index value: x is an 3-D variable array but is being indexed with 2 indices.");

        REQUIRE_THROWS_WITH( v(100,100,100),
            "Unknown index value: x(100,100,100)");
      }

    WHEN( "var value" ) {
        auto q = coek::parameter("q").value(1);
        auto v = coek::variable_array( 4 ).name("x").value(1);

        REQUIRE( v(1).value() == 1 );
        v.value(q+1);
        REQUIRE( v(1).value() == 2 );
        q.value(2);
        REQUIRE( v(1).value() == 3 );
        }

    WHEN( "var lower" ) {
        auto q = coek::parameter("q").value(1);
        auto v = coek::variable_array( 4 ).name("x").lower(1);

        REQUIRE( v(1).lower() == 1 );
        v.lower(q+1);
        REQUIRE( v(1).lower() == 2 );
        q.value(2);
        REQUIRE( v(1).lower() == 3 );
        }

    WHEN( "var upper" ) {
        auto q = coek::parameter("q").value(1);
        auto v = coek::variable_array( 4 ).name("x").upper(1);

        REQUIRE( v(1).upper() == 1 );
        v.upper(q+1);
        REQUIRE( v(1).upper() == 2 );
        q.value(2);
        REQUIRE( v(1).upper() == 3 );
        }

    WHEN( "var bounds" ) {
        auto q = coek::parameter("q").value(1);
        auto v = coek::variable_array( 4 ).name("x").bounds(1,2);

        REQUIRE( v(1).lower() == 1 );
        REQUIRE( v(1).upper() == 2 );
        v.bounds(q+1,q+2);
        REQUIRE( v(1).lower() == 2 );
        REQUIRE( v(1).upper() == 3 );
        q.value(2);
        REQUIRE( v(1).lower() == 3 );
        REQUIRE( v(1).upper() == 4 );
        v.bounds(q+1,10);
        REQUIRE( v(1).lower() == 3 );
        REQUIRE( v(1).upper() == 10 );
        v.bounds(10,10*q);
        REQUIRE( v(1).lower() == 10 );
        REQUIRE( v(1).upper() == 20 );
        }

    WHEN( "var within" ) {
        auto q = coek::parameter("q").value(1);
        auto v = coek::variable_array( 4 ).name("x");

        REQUIRE( v(1).within() == coek::Reals );
        v.within(coek::Integers);
        REQUIRE( v(1).within() == coek::Integers );
        }
  }

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}

#endif