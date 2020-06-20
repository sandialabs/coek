#include "catch.hpp"

#include <typeinfo>
#include "expr/ast_term.hpp"
#include "coek_indexed.hpp"
#include "coek_model.hpp"

#define NLP_INTRINSIC_TEST1(FN, MEMCHECK)\
    WHEN( #FN " 1" ) {\
          {\
          coek::Model m;\
          coek::Variable v = m.getVariable(0, 1, 0, "v");\
          coek::Expression e = FN(v+1);\
          coek::MutableNLPExpr repn;\
          static std::list<std::string> constval = {"0.000"};\
          static std::list<std::string> nonlinear = {"[", #FN, "[", "+", "v", "1.000", "]", "]" };\
          REQUIRE( repn.constval.to_list() == constval );\
          REQUIRE( repn.linear_coefs.size() + repn.quadratic_coefs.size() == 0 );\
          }\
          MEMCHECK;\
      }\
    WHEN( #FN " 2" ) {\
          {\
          coek::Model m;\
          coek::Variable v = m.getVariable(0, 1, 0, "v");\
          v.set_fixed(true);\
          coek::Expression e = FN(v+1);\
          auto E = e.expand();\
          coek::MutableNLPExpr repn;\
          repn.collect_terms(E);\
          static std::list<std::string> nonlinear = {"0.000"};\
          REQUIRE( repn.linear_coefs.size() + repn.quadratic_coefs.size() == 0 );\
          REQUIRE( repn.nonlinear.to_list() == nonlinear );\
          }\
          MEMCHECK;\
      }

#define NLP_INTRINSIC_TEST2(FN, MEMCHECK)\
    WHEN( #FN " 1" ) {\
          {\
          coek::Model m;\
          coek::Variable v = m.getVariable(0, 1, 0, "v");\
          coek::Expression e = FN(v+1, v);\
          coek::MutableNLPExpr repn;\
          static std::list<std::string> constval = {"0.000"};\
          static std::list<std::string> nonlinear = {"[", #FN, "[", "+", "v", "1.000", "]", "v", "]" };\
          REQUIRE( repn.constval.to_list() == constval );\
          REQUIRE( repn.linear_coefs.size() + repn.quadratic_coefs.size() == 0 );\
          }\
          MEMCHECK;\
      }\
    WHEN( #FN " 2" ) {\
          {\
          coek::Model m;\
          coek::Variable v = m.getVariable(0, 1, 0, "v");\
          v.set_fixed(true);\
          coek::Expression e = FN(v+1, v);\
          auto E = e.expand();\
          coek::MutableNLPExpr repn;\
          repn.collect_terms(E);\
          static std::list<std::string> nonlinear = {"0.000"};\
          REQUIRE( repn.linear_coefs.size() + repn.quadratic_coefs.size() == 0 );\
          REQUIRE( repn.nonlinear.to_list() == nonlinear );\
          }\
          MEMCHECK;\
      }

#define MV_INTRINSIC_TEST1(FN)\
    WHEN( #FN ) {\
          coek::Model m;\
          coek::Variable v = m.getVariable(0, 1, 0, "v");\
          v.set_fixed(true);\
          coek::Expression e = FN(v+1);\
          mutable_values(e.repn, fixed_vars, params);\
          static std::unordered_set<coek::VariableTerm*> vbaseline { v.repn };\
          static std::unordered_set<coek::ParameterTerm*> pbaseline { };\
          REQUIRE( fixed_vars == vbaseline );\
          REQUIRE( params == pbaseline );\
      }

#define MV_INTRINSIC_TEST2(FN)\
    WHEN( #FN ) {\
          coek::Model m;\
          coek::Variable v = m.getVariable(0, 1, 0, "v");\
          v.set_fixed(true);\
          coek::Expression e = FN(v+1, v);\
          mutable_values(e.repn, fixed_vars, params);\
          static std::unordered_set<coek::VariableTerm*> vbaseline { v.repn };\
          static std::unordered_set<coek::ParameterTerm*> pbaseline { };\
          REQUIRE( fixed_vars == vbaseline );\
          REQUIRE( params == pbaseline );\
      }

#ifdef DEBUG
#define ENV_MEMCHECK REQUIRE( coek::env.check_memory() == true )
#else
#define ENV_MEMCHECK 
#endif


void xxx() {}

TEST_CASE( "1D_indexed_var", "[smoke]" ) {

  SECTION( "int" ) {
      std::vector<int> v = {1,5,3,7};

      WHEN( "size" ) {
        auto s = coek::SetOf( v );
        auto vars = IndexedVariable( s );
        REQUIRE( vars.size() == 4 );
      }

      WHEN( "typeof" ) {
        auto s = coek::SetOf( v );
        auto vars = IndexedVariable( s );
        REQUIRE( typeid(vars(1)).name() == typeid(coek::Expression).name() );
      }

      WHEN( "index" ) {
        auto s = coek::SetOf( v );
        coek::IndexParameter i("i");
        coek::IndexParameter j("j");

        std::vector<int> vals(4);
        int ii=0;
        for (auto it=s.begin({i}); it != s.end(); ++it) {
            i.get_value(vals[ii++]);
            }
        REQUIRE( v == vals );
        }

      WHEN( "index_error" ) {
        auto s = coek::SetOf( v );
        coek::IndexParameter i("i");
        coek::IndexParameter j("j");
        CHECK_THROWS( s.begin({i,j}) );
        }
  }

  SECTION( "int_ranged" ) {
      std::vector<int> v = {1,3,5,7};
      auto s = coek::RangeSet(1, 7, 2);

      WHEN( "size" ) {
        auto vars = IndexedVariable( s );
        REQUIRE( vars.size() == 4 );
      }

      WHEN( "typeof" ) {
        auto vars = IndexedVariable( s );
        REQUIRE( typeid(vars(1)).name() == typeid(coek::Expression).name() );
      }

      WHEN( "index" ) {
        coek::IndexParameter i("i");
        coek::IndexParameter j("j");

        std::vector<int> vals(4);
        int ii=0;
        for (auto it=s.begin({i}); it != s.end(); ++it) {
            i.get_value(vals[ii++]);
            }
        REQUIRE( v == vals );
        }

      WHEN( "index_error" ) {
        coek::IndexParameter i("i");
        coek::IndexParameter j("j");
        CHECK_THROWS( s.begin({i,j}) );
        }
  }

#if 0
  SECTION( "string" ) {
      std::vector<std::string> v = {"a","c","b","d"};

      WHEN( "size" ) {
        auto s = coek::SetOf( v );
        auto vars = IndexedVariable( s );
        REQUIRE( vars.size() == 4 );
      }

      WHEN( "typeof" ) {
        auto s = coek::SetOf( v );
        auto vars = IndexedVariable( s );
        REQUIRE( typeid(vars("a")).name() == typeid(coek::Expression).name() );
      }

      WHEN( "index" ) {
        auto s = coek::SetOf( v );
        coek::IndexParameter i("i");
        coek::IndexParameter j("j");
        CHECK_THROWS( s.begin({i,j}) );
        }

      WHEN( "index" ) {
        auto s = coek::SetOf( v );
        coek::IndexParameter i("i");
        coek::IndexParameter j("j");

        std::vector<std::string> vals(4);
        int ii=0;
        for (auto it=s.begin({i}); it != s.end(); ++it) {
            i.get_value(vals[ii++]);
            }
        REQUIRE( v == vals );
        }

      WHEN( "index_error" ) {
        auto s = coek::SetOf( v );
        coek::IndexParameter i("i");
        coek::IndexParameter j("j");
        CHECK_THROWS( s.begin({i,j}) );
        }
  }
#endif

  SECTION( "abstract" ) {
      std::vector<int> v = {1,5,3,7};
      auto s = coek::SetOf( v );
      auto r = coek::SetOf( {0} );
      auto S = s - r;
      auto vars = IndexedVariable( S, 0, 1, 0, "vars" );

      WHEN( "typeof" ) {
        REQUIRE( typeid(vars(1)).name() == typeid(coek::Expression).name() );
      }

      WHEN( "index" ) {
        auto s = coek::SetOf( v );
        coek::IndexParameter i("i");
        coek::IndexParameter j("j");
        CHECK_THROWS( s.begin({i,j}) );
        }

      WHEN( "index1" ) {
        coek::IndexParameter i("i");
        REQUIRE( typeid(vars(1)).name() == typeid(coek::Expression).name() );
        REQUIRE( typeid(vars(i)).name() == typeid(coek::Expression).name() );
      }

      WHEN( "index2" ) {
        coek::Variable v;
        coek::Expression f = v;
        auto e = vars(1);
        static std::list<std::string> baseline = {"vars(1)"};
        REQUIRE( e.to_list() == baseline);
        //auto it = e.to_list().begin();
        //REQUIRE_THAT( *it, Catch::Matchers::StartsWith("vars") );
      }

      WHEN( "index3" ) {
        coek::IndexParameter i("i");
        auto e = vars(i);
        static std::list<std::string> baseline = {"vars(i)"};
        REQUIRE( e.to_list() == baseline);
      }

      WHEN( "index4" ) {
        coek::IndexParameter i("i");
        auto e = vars(i+1);
        static std::list<std::string> baseline = {"vars(i + 1)"};
        REQUIRE( e.to_list() == baseline);
      }
  }

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}


TEST_CASE( "2D_indexed_var", "[smoke]" ) {

  SECTION( "int" ) {
      std::vector<int> v = {1,5,3,7};
      std::vector<int> w = {2,6,4,8};

      WHEN( "size" ) {
        auto V = coek::SetOf( v );
        auto W = coek::SetOf( w );
        auto S = V*W;
        auto vars = IndexedVariable( S );
        REQUIRE( vars.size() == 16 );
      }

      WHEN( "typeof" ) {
        auto V = coek::SetOf( v );
        auto W = coek::SetOf( w );
        auto S = V*W;
        auto vars = IndexedVariable( S );
        REQUIRE( typeid(vars(1,2)).name() == typeid(coek::Expression).name() );
      }

      WHEN( "index" ) {
        auto V = coek::SetOf( v );
        auto W = coek::SetOf( w );
        auto S = V*W;
        coek::IndexParameter i("i");
        coek::IndexParameter j("j");

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

      WHEN( "index_error" ) {
        auto V = coek::SetOf( v );
        auto W = coek::SetOf( w );
        auto S = V*W;
        coek::IndexParameter i("i");
        coek::IndexParameter j("j");
        CHECK_THROWS( S.begin({i}) );
        }
  }

#if 0
  SECTION( "string" ) {
      std::vector<std::string> v = {"a","c","b","d"};
      std::vector<std::string> w = {"A","C","B","D"};

      WHEN( "size" ) {
        auto V = coek::SetOf( v );
        auto W = coek::SetOf( w );
        auto S = V*W;
        auto vars = IndexedVariable( S );
        REQUIRE( vars.size() == 16 );
      }

      WHEN( "typeof" ) {
        auto V = coek::SetOf( v );
        auto W = coek::SetOf( w );
        auto S = V*W;
        auto vars = IndexedVariable( S );
        REQUIRE( typeid(vars("a","A")).name() == typeid(coek::Expression).name() );
      }

      WHEN( "index" ) {
        auto V = coek::SetOf( v );
        auto W = coek::SetOf( w );
        auto S = V*W;
        coek::IndexParameter i("i");
        coek::IndexParameter j("j");

        std::set<std::string> ivals;
        std::set<std::string> jvals;
        for (auto it=S.begin({i,j}); it != S.end(); ++it) {
            std::string tmp;
            i.get_value(tmp);
            ivals.insert(tmp);
            j.get_value(tmp);
            jvals.insert(tmp);
            }

        std::set<std::string> vset(v.begin(), v.end());
        std::set<std::string> wset(w.begin(), w.end());
        REQUIRE( vset == ivals );
        REQUIRE( wset == jvals );
        }

      WHEN( "index_error" ) {
        auto V = coek::SetOf( v );
        auto W = coek::SetOf( w );
        auto S = V*W;
        coek::IndexParameter i("i");
        coek::IndexParameter j("j");
        CHECK_THROWS( S.begin({i}) );
        }
  }
#endif

  SECTION( "abstract" ) {
      std::vector<int> v = {1,5,3,7};
      std::vector<int> w = {2,6,4,8};
      auto V = coek::SetOf( v );
      auto W = coek::SetOf( w );
      auto r = coek::SetOf( {0} );
      auto S = V*W;
      auto vars = IndexedVariable( S, 0, 1, 0, "vars" );

      WHEN( "typeof" ) {
        REQUIRE( typeid(vars(1,2)).name() == typeid(coek::Expression).name() );
      }

      WHEN( "index" ) {
        auto V = coek::SetOf( v );
        auto W = coek::SetOf( w );
        auto S = V*W;
        coek::IndexParameter i("i");
        coek::IndexParameter j("j");
        CHECK_THROWS( S.begin({i}) );
        S.begin({i,j});
        }

      WHEN( "index1" ) {
        coek::IndexParameter i("i");
        coek::IndexParameter j("j");
        REQUIRE( typeid(vars(1,2)).name() == typeid(coek::Expression).name() );
        REQUIRE( typeid(vars(10,11)).name() == typeid(coek::Expression).name() );
        REQUIRE( typeid(vars(i,j)).name() == typeid(coek::Expression).name() );
      }

      WHEN( "index2" ) {
        auto e = vars(1,2);
        static std::list<std::string> baseline = {"vars(1,2)"};
        REQUIRE( e.to_list() == baseline);
        //auto it = e.to_list().begin();
        //REQUIRE_THAT( *it, Catch::Matchers::StartsWith("vars") );
      }

      WHEN( "index3" ) {
        coek::IndexParameter i("i");
        coek::IndexParameter j("j");
        auto e = vars(i,j);
        static std::list<std::string> baseline = {"vars(i,j)"};
        REQUIRE( e.to_list() == baseline);
      }

      WHEN( "index4" ) {
        coek::IndexParameter i("i");
        coek::IndexParameter j("j");
        auto e = vars(i+1,j-1);
        static std::list<std::string> baseline = {"vars(i + 1,j + -1)"};
        REQUIRE( e.to_list() == baseline);
      }
  }

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}


TEST_CASE( "expr_sequence", "[smoke]" ) {

    SECTION( "concrete_expr" ) {
        std::vector<int> v = {1,5,3,7};
        auto s = coek::SetOf( v );
        auto y = IndexedVariable( s, "y" );
        auto x = IndexedVariable( s*s, "x" );
        coek::IndexParameter i("i");
        coek::IndexParameter j("j");

        WHEN( "y(i)" ) {
            auto tmp = (y(i)). forall(i).in( s );
            int ii=0;
            for (auto it=tmp.begin(); it != tmp.end(); ++it) {
                std::list<std::string> baseline = {"y(" + std::to_string(v[ii]) + ")"};
                REQUIRE( it->to_list() == baseline);
                ii++;
                }
        }

        WHEN( "1 + y(i)" ) {
            auto tmp = (1+y(i)). forall(i).in( s );
            int ii=0;
            for (auto it=tmp.begin(); it != tmp.end(); ++it) {
                std::list<std::string> baseline = {"[", "+", "1.000", "y(" + std::to_string(v[ii]) + ")", "]"};
                REQUIRE( it->to_list() == baseline);
                ii++;
                }
        }

        WHEN( "i + y(i)" ) {
            auto tmp = (i+y(i)). forall(i).in( s );
            int ii=0;
            for (auto it=tmp.begin(); it != tmp.end(); ++it) {
                std::list<std::string> baseline = {"[", "+", std::to_string(v[ii])+".000", "y(" + std::to_string(v[ii]) + ")", "]"};
                REQUIRE( it->to_list() == baseline);
                ii++;
                }
        }

        WHEN( "y(i+2)" ) {
            std::vector<int> w = {1,5,3};
            auto S = coek::SetOf( w );
            auto tmp = (y(i+2)). forall(i).in( S );
            int ii=0;
            for (auto it=tmp.begin(); it != tmp.end(); ++it) {
                std::list<std::string> baseline = {"y(" + std::to_string(v[ii]+2) + ")"};
                REQUIRE( it->to_list() == baseline);
                ii++;
                }
        }

        WHEN( "y(i)+i*y(i+2)" ) {
            std::vector<int> w = {1,5,3};
            auto S = coek::SetOf( w );
            auto tmp = (y(i)+i*y(i+2)). forall(i).in( S );
            int ii=0;
            for (auto it=tmp.begin(); it != tmp.end(); ++it) {
                std::list<std::string> baseline = {"[", "+", "y(" + std::to_string(v[ii]) + ")", "[", "*", std::to_string(v[ii])+".000", "y(" + std::to_string(v[ii]+2) + ")", "]", "]"};
                REQUIRE( it->to_list() == baseline);
                ii++;
                }
        }

        WHEN( "Sum( y(i) )" ) {
            auto tmp = Sum( (y(i)). forall(i).in( s ) );
            auto e = tmp.expand();
            std::list<std::string> baseline = { "[", "+", "y(1)", "y(5)", "y(3)", "y(7)", "]" };
            REQUIRE( e.to_list() == baseline);
        }

        WHEN( "Sum( x(i,i) )" ) {
            auto tmp = Sum( (x(i,i)). forall(i).in( s ) );
            auto e = tmp.expand();
            std::list<std::string> baseline = { "[", "+", "x(1,1)", "x(5,5)", "x(3,3)", "x(7,7)", "]" };
            REQUIRE( e.to_list() == baseline);
        }

        WHEN( "Sum( x(i,j) ) [1]" ) {
            auto tmp = Sum( (x(i,j)). forall(i,j).in( s*s ) );
            auto e = tmp.expand();
            std::list<std::string> baseline =  { "[", "+", "x(1,1)", "x(1,5)", "x(1,3)", "x(1,7)", "x(5,1)", "x(5,5)", "x(5,3)", "x(5,7)", "x(3,1)", "x(3,5)", "x(3,3)", "x(3,7)", "x(7,1)", "x(7,5)", "x(7,3)", "x(7,7)", "]" };
            REQUIRE( e.to_list() == baseline);
        }

        WHEN( "Sum( x(i,j) ) [2]" ) {
            auto tmp = Sum( (x(i,j)) .forall(i).in(s) .forall(j).in(s) );
            auto e = tmp.expand();
            std::list<std::string> baseline = { "[", "+", "x(1,1)", "x(1,5)", "x(1,3)", "x(1,7)", "x(5,1)", "x(5,5)", "x(5,3)", "x(5,7)", "x(3,1)", "x(3,5)", "x(3,3)", "x(3,7)", "x(7,1)", "x(7,5)", "x(7,3)", "x(7,7)", "]" };
            REQUIRE( e.to_list() == baseline);
        }

        WHEN( "Sum( y(i)*x(i,i) )" ) {
            auto tmp = Sum( (y(i)*x(i,i)). forall(i).in(s) );
            auto e = tmp.expand();
            std::list<std::string> baseline = { "[", "+", "[", "*", "y(1)", "x(1,1)", "]", "[", "*", "y(5)", "x(5,5)", "]", "[", "*", "y(3)", "x(3,3)", "]", "[", "*", "y(7)", "x(7,7)", "]", "]" };
            REQUIRE( e.to_list() == baseline);
        }

        WHEN( "Sum_i( y(i)*Sum_j(x(i,j)) )" ) {
            auto tmp = Sum( ( y(i)*Sum(x(i,j) .forall(j).in(s)) ). forall(i).in(s) );
            auto e = tmp.expand();
            std::list<std::string> baseline =  { "[", "+", "[", "*", "y(1)", "[", "+", "x(1,1)", "x(1,5)", "x(1,3)", "x(1,7)", "]", "]", "[", "*", "y(5)", "[", "+", "x(5,1)", "x(5,5)", "x(5,3)", "x(5,7)", "]", "]", "[", "*", "y(3)", "[", "+", "x(3,1)", "x(3,5)", "x(3,3)", "x(3,7)", "]", "]", "[", "*", "y(7)", "[", "+", "x(7,1)", "x(7,5)", "x(7,3)", "x(7,7)", "]", "]", "]" };
            REQUIRE( e.to_list() == baseline);
        }
    }

    SECTION( "concrete_con" ) {
        std::vector<int> v = {1,5,3,7};
        auto s = coek::SetOf( v );
        auto y = IndexedVariable( s, "y" );
        auto x = IndexedVariable( s*s, "x" );
        coek::IndexParameter i("i");

        WHEN( "y(i) == 0" ) {
            auto tmp = (y(i) == 0). forall(i).in( s );
            int ii=0;
            for (auto it=tmp.begin(); it != tmp.end(); ++it) {
                std::list<std::string> baseline = {"[", "==", "y(" + std::to_string(v[ii]) + ")", "0.000", "]"};
                REQUIRE( it->to_list() == baseline);
                ii++;
                }
        }

        WHEN( "1 + y(i) == 0" ) {
            auto tmp = (1+y(i) == 0). forall(i).in( s );
            int ii=0;
            for (auto it=tmp.begin(); it != tmp.end(); ++it) {
                std::list<std::string> baseline = {"[", "==", "[", "+", "1.000", "y(" + std::to_string(v[ii]) + ")", "]", "0.000", "]"};
                REQUIRE( it->to_list() == baseline);
                ii++;
                }
        }

        WHEN( "i + y(i) == 0" ) {
            auto tmp = (i+y(i) == 0). forall(i).in( s );
            int ii=0;
            for (auto it=tmp.begin(); it != tmp.end(); ++it) {
                std::list<std::string> baseline = {"[", "==", "[", "+", std::to_string(v[ii])+".000", "y(" + std::to_string(v[ii]) + ")", "]", "0.000", "]"};
                REQUIRE( it->to_list() == baseline);
                ii++;
                }
        }

        WHEN( "y(i+2) == 0" ) {
            std::vector<int> w = {1,5,3};
            auto S = coek::SetOf( w );
            auto tmp = (y(i+2) == 0). forall(i).in( S );
            int ii=0;
            for (auto it=tmp.begin(); it != tmp.end(); ++it) {
                std::list<std::string> baseline = {"[", "==", "y(" + std::to_string(v[ii]+2) + ")", "0.000", "]"};
                REQUIRE( it->to_list() == baseline);
                ii++;
                }
        }

        WHEN( "y(i)+i*y(i+2) == 0" ) {
            std::vector<int> w = {1,5,3};
            auto S = coek::SetOf( w );
            auto tmp = (y(i)+i*y(i+2) == 0). forall(i).in( S );
            int ii=0;
            for (auto it=tmp.begin(); it != tmp.end(); ++it) {
                std::list<std::string> baseline = {"[", "==", "[", "+", "y(" + std::to_string(v[ii]) + ")", "[", "*", std::to_string(v[ii])+".000", "y(" + std::to_string(v[ii]+2) + ")", "]", "]", "0.000", "]"};
                REQUIRE( it->to_list() == baseline);
                ii++;
                }
        }

        WHEN( "Sum( y(i) ) == 0" ) {
            auto tmp = Sum( (y(i)). forall(i).in( s ) ) == 0;
            auto e = tmp.expand();
            std::list<std::string> baseline = { "[", "==", "[", "+", "y(1)", "y(5)", "y(3)", "y(7)", "]", "0.000", "]" };
            REQUIRE( e.to_list() == baseline);
        }

        WHEN( "Sum( y(i)*x(i,i) ) == 0" ) {
            auto tmp = Sum( (y(i)*x(i,i)). forall(i).in(s) ) == 0;
            auto e = tmp.expand();
            std::list<std::string> baseline = { "[", "==", "[", "+", "[", "*", "y(1)", "x(1,1)", "]", "[", "*", "y(5)", "x(5,5)", "]", "[", "*", "y(3)", "x(3,3)", "]", "[", "*", "y(7)", "x(7,7)", "]", "]", "0.000", "]" };
            REQUIRE( e.to_list() == baseline);
        }
    }


#ifdef DEBUG
//coek::env.debug = false;
REQUIRE( coek::env.check_memory() == true );
#endif
}



TEST_CASE( "expr_expand", "[smoke]" ) {

  SECTION( "constant" ) {
    {
    coek::Expression e(3);
    auto E = e.expand();
    coek::MutableNLPExpr repn;
    repn.collect_terms(E);

    static std::list<std::string> constval = { "3.000" };
    REQUIRE( repn.mutable_values == false );
    REQUIRE( repn.constval.to_list() == constval );
    REQUIRE( repn.linear_coefs.size() == 0 );
    REQUIRE( repn.quadratic_coefs.size() == 0 );
    }
  }

  SECTION( "param" ) {
    WHEN( "simple" ) {
        {
        coek::Parameter p(3, "p");
        coek::Expression e = p;
        auto E = e.expand();
        coek::MutableNLPExpr repn;
        repn.collect_terms(E);

        static std::list<std::string> constval = { "p" };
        REQUIRE( repn.mutable_values == true );
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 0 );
        REQUIRE( repn.quadratic_coefs.size() == 0 );
        }
    }
    WHEN( "nontrivial multiplier" ) {
        {
        coek::Parameter p(3, "p");
        coek::Expression e = p/2;
        auto E = e.expand();
        coek::MutableNLPExpr repn;
        repn.collect_terms(E);

        static std::list<std::string> constval = { "[", "*", "0.500", "p", "]" };
        REQUIRE( repn.mutable_values == true );
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 0 );
        REQUIRE( repn.quadratic_coefs.size() == 0 );
        }
    }
  }

  SECTION( "var" ) {
    WHEN( "unfixed" ) {
        {
        coek::Model m;
        coek::Variable v = m.getVariable(0,1,3,"v");
        coek::Expression e = v;
        auto E = e.expand();
        coek::MutableNLPExpr repn;
        repn.collect_terms(E);

        static std::list<std::string> constval = { "0.000" };
        static std::list<std::string> coefval = { "1.000" };
        REQUIRE( repn.mutable_values == false );
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 1 );
        REQUIRE( repn.linear_coefs[0].to_list() == coefval );
        REQUIRE( repn.linear_vars[0] == e.repn );
        REQUIRE( repn.quadratic_coefs.size() == 0 );
        }
    }
    WHEN( "fixed" ) {
        {
        coek::Model m;
        coek::Variable v = m.getVariable(0,1,3,"v");
        v.set_fixed(true);
        coek::Expression e = v;
        auto E = e.expand();
        coek::MutableNLPExpr repn;
        repn.collect_terms(E);

        static std::list<std::string> constval = { "v" };
        REQUIRE( repn.mutable_values == true );
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 0 );
        REQUIRE( repn.quadratic_coefs.size() == 0 );
        }
    }
    WHEN( "fixed - nontrivial multiplier" ) {
        {
        coek::Model m;
        coek::Variable v = m.getVariable(0,1,3,"v");
        v.set_fixed(true);
        coek::Expression f(2);
        coek::Expression e = v/f;
        auto E = e.expand();
        coek::MutableNLPExpr repn;
        repn.collect_terms(E);

        static std::list<std::string> constval = { "[", "*", "0.500", "v", "]" };
        REQUIRE( repn.mutable_values == true );
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 0 );
        REQUIRE( repn.quadratic_coefs.size() == 0 );
        }
    }
  }

  SECTION( "monomial" ) {
    WHEN( "unfixed" ) {
        {
        coek::Model m;
        coek::Variable v = m.getVariable(0,1,0,"v");
        coek::Expression e = 2*v;
        auto E = e.expand();
        coek::MutableNLPExpr repn;
        repn.collect_terms(E);

        static std::list<std::string> constval = { "0.000" };
        static std::list<std::string> coefval = { "2.000" };
        REQUIRE( repn.mutable_values == false );
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 1 );
        REQUIRE( repn.linear_coefs[0].to_list() == coefval );
        coek::MonomialTerm* tmp = dynamic_cast<coek::MonomialTerm*>(e.repn);
        REQUIRE( repn.linear_vars[0]->index == tmp->var->index );
        REQUIRE( repn.quadratic_coefs.size() == 0 );
        }
    }
    WHEN( "fixed" ) {
        {
        coek::Model m;
        coek::Variable v = m.getVariable(0,1,3,"v");
        v.set_fixed(true);
        coek::Expression e = 2*v;
        auto E = e.expand();
        coek::MutableNLPExpr repn;
        repn.collect_terms(E);

        static std::list<std::string> constval = { "[", "*", "2.000", "v", "]" };
        REQUIRE( repn.mutable_values == true );
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 0 );
        REQUIRE( repn.quadratic_coefs.size() == 0 );
        }
    }
  }

  SECTION( "negate" ) {
        {
        coek::Model m;
        coek::Variable v = m.getVariable(0,1,3,"v");
        coek::Expression e = -(v+1);
        auto E = e.expand();
        coek::MutableNLPExpr repn;
        repn.collect_terms(E);

        static std::list<std::string> constval = { "-1.000" };
        static std::list<std::string> coefval = { "-1.000" };
        REQUIRE( repn.mutable_values == false );
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 1 );
        REQUIRE( repn.linear_coefs[0].to_list() == coefval );
        REQUIRE( repn.linear_vars[0] == v.repn );
        REQUIRE( repn.quadratic_coefs.size() == 0 );
        }
  }

  SECTION( "plus" ) {
    WHEN("2 terms") {
        {
        coek::Model m;
        coek::Variable v = m.getVariable(0,1,3,"v");
        coek::Expression e = v+1;
        coek::Expression E = e.expand();
        //coek::env.write(std::cout);
        coek::MutableNLPExpr repn;
        repn.collect_terms(E);

        static std::list<std::string> constval = { "1.000" };
        static std::list<std::string> coefval = { "1.000" };
        REQUIRE( repn.mutable_values == false );
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 1 );
        REQUIRE( repn.linear_coefs[0].to_list() == coefval );
        REQUIRE( repn.linear_vars[0] == v.repn );
        REQUIRE( repn.quadratic_coefs.size() == 0 );
        }
    }
    WHEN("combine sums") {
        {
        coek::Model m;
        coek::Variable v = m.getVariable(0,1,3,"v");
        coek::Expression e = v+1+(-1-v);
        //std::cout << e << std::endl;
        //coek::env.write(std::cout);
        //std::cout << "---" << std::endl;
        coek::Expression E = e.expand();
        //std::cout << E << std::endl;
        //coek::env.write(std::cout);
        coek::MutableNLPExpr repn;
        repn.collect_terms(E);

        static std::list<std::string> constval = { "0.000" };
        static std::list<std::string> coefval0 = { "1.000" };
        static std::list<std::string> coefval1 = { "-1.000" };
        REQUIRE( repn.mutable_values == false );
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 2 );
        REQUIRE( repn.linear_coefs[0].to_list() == coefval0 );
        REQUIRE( repn.linear_coefs[1].to_list() == coefval1 );
        REQUIRE( repn.linear_vars[0] == v.repn );
        REQUIRE( repn.linear_vars[1] == v.repn );
        REQUIRE( repn.quadratic_coefs.size() == 0 );
        }
    }
  }

  SECTION( "times" ) {
    WHEN( "lhs constant" ) {
        {
        coek::Model m;
        coek::Parameter p(0, "p");
        coek::Variable w = m.getVariable(0,1,3,"w");
        coek::Expression e = p*w;
        auto E = e.expand();
        coek::MutableNLPExpr repn;
        repn.collect_terms(E);

        static std::list<std::string> constval = { "0.000" };
        static std::list<std::string> coefval = { "p" };
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 1 );
        REQUIRE( repn.linear_coefs[0].to_list() == coefval );
        REQUIRE( repn.linear_vars[0] == w.repn );
        REQUIRE( repn.quadratic_coefs.size() == 0 );
        }
    }
    WHEN( "rhs constant" ) {
        {
        coek::Model m;
        coek::Parameter p(0, "p");
        coek::Variable w = m.getVariable(0,1,3,"w");
        coek::Expression e = w*p;
        auto E = e.expand();
        coek::MutableNLPExpr repn;
        repn.collect_terms(E);

        static std::list<std::string> constval = { "0.000" };
        static std::list<std::string> coefval = { "p" };
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 1 );
        REQUIRE( repn.linear_coefs[0].to_list() == coefval );
        REQUIRE( repn.linear_vars[0] == w.repn );
        REQUIRE( repn.quadratic_coefs.size() == 0 );
        }
    }
    WHEN( "simple quadratic" ) {
        {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression e = w*w;
        auto E = e.expand();
        coek::MutableNLPExpr repn;
        repn.collect_terms(E);

        static std::list<std::string> constval = { "0.000" };
        static std::list<std::string> qcoefval = { "1.000" };
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 0 );
        REQUIRE( repn.quadratic_coefs.size() == 1 );
        REQUIRE( repn.quadratic_coefs[0].to_list() == qcoefval );
        REQUIRE( repn.quadratic_lvars[0] == w.repn );
        REQUIRE( repn.quadratic_rvars[0] == w.repn );
        }
    }
    WHEN( "complex quadratic 1a" ) {
        {
        // By default, we do not expand products of linear expressions
        coek::Model m;
        coek::Variable v = m.getVariable(0, 1, 0, "v");
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression e = (2 + 3*w + v)*(4 + 5*v + w);
        auto E = e.expand();
        coek::MutableNLPExpr repn;
        repn.collect_terms(E);

        static std::list<std::string> constval = { "0.000" };
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 0 );
        REQUIRE( repn.quadratic_coefs.size() == 0 );
        }
    }
    WHEN( "complex quadratic 1b" ) {
        {
        coek::Model m;
        coek::Variable v = m.getVariable(0, 1, 0, "v");
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression e = 3*w*(4 + 5*v + w);
        auto E = e.expand();
        coek::MutableNLPExpr repn;
        repn.collect_terms(E);

        static std::list<std::string> constval = { "0.000" };
        static std::list<std::string> lcoef0 = { "12.000" };
        static std::list<std::string> qcoef0 = { "15.000" };
        static std::list<std::string> qcoef1 = { "3.000" };
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 1 );
        REQUIRE( repn.linear_coefs[0].to_list() == lcoef0 );
        REQUIRE( repn.quadratic_coefs.size() == 2 );
        REQUIRE( repn.quadratic_coefs[0].to_list() == qcoef0 );
        REQUIRE( repn.quadratic_coefs[1].to_list() == qcoef1 );
        }
    }
    WHEN( "complex quadratic 2" ) {
        {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression e = 3*(w*w + 2);
        auto E = e.expand();
        coek::MutableNLPExpr repn;
        repn.collect_terms(E);

        static std::list<std::string> constval = { "6.000" };
        static std::list<std::string> qcoefval = { "3.000" };
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 0 );
        REQUIRE( repn.quadratic_coefs.size() == 1 );
        REQUIRE( repn.quadratic_coefs[0].to_list() == qcoefval );
        REQUIRE( repn.quadratic_lvars[0] == w.repn );
        REQUIRE( repn.quadratic_rvars[0] == w.repn );
        }
    }
    WHEN( "complex quadratic 3" ) {
        {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression e = (w*w + 2)*3;
        auto E = e.expand();
        coek::MutableNLPExpr repn;
        repn.collect_terms(E);

        static std::list<std::string> constval = { "6.000" };
        static std::list<std::string> qcoefval = { "3.000" };
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 0 );
        REQUIRE( repn.quadratic_coefs.size() == 1 );
        REQUIRE( repn.quadratic_coefs[0].to_list() == qcoefval );
        REQUIRE( repn.quadratic_lvars[0] == w.repn );
        REQUIRE( repn.quadratic_rvars[0] == w.repn );
        }
    }
  }

  SECTION( "divide" ) {
    WHEN( "lhs parameter - zero" ) {
        {
        coek::Model m;
        coek::Parameter p(0, "p");
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression e = p/w;
        auto E = e.expand();
        coek::MutableNLPExpr repn;
        repn.collect_terms(E);

        static std::list<std::string> constval = { "0.000" };
        static std::list<std::string> nonlinear = { "[", "/", "p", "w", "]" };
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 0 );
        REQUIRE( repn.quadratic_coefs.size() == 0 );
        REQUIRE( repn.nonlinear.to_list() == nonlinear );
        }
    }
    WHEN( "rhs parameter - zero" ) {
        {
        coek::Model m;
        coek::Parameter p(0, "p");
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression e = w/p;
        auto E = e.expand();
        coek::MutableNLPExpr repn;
        repn.collect_terms(E);

        static std::list<std::string> constval = { "0.000" };
        static std::list<std::string> lcoef0 = { "[", "/", "1.000", "p", "]" };
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 1 );
        REQUIRE( repn.linear_coefs[0].to_list() == lcoef0 );
        REQUIRE( repn.quadratic_coefs.size() == 0 );
        REQUIRE( repn.nonlinear.to_list() == constval );
        }
    }
    WHEN( "lhs constant - zero" ) {
        {
        coek::Model m;
        coek::Expression p;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression e = p/w;
        auto E = e.expand();
        coek::MutableNLPExpr repn;
        repn.collect_terms(E);

        static std::list<std::string> constval = { "0.000" };
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 0 );
        REQUIRE( repn.quadratic_coefs.size() == 0 );
        REQUIRE( repn.nonlinear.to_list() == constval );
        }
    }
    WHEN( "lhs constant - zero AND rhs constant" ) {
        {
        coek::Model m;
        coek::Expression p;
        coek::Parameter w(1,"w");
        coek::Expression e = p/(w+1);
        auto E = e.expand();
        coek::MutableNLPExpr repn;
        repn.collect_terms(E);

        static std::list<std::string> constval = { "0.000" };
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 0 );
        REQUIRE( repn.quadratic_coefs.size() == 0 );
        REQUIRE( repn.nonlinear.to_list() == constval );
        }
    }
    WHEN( "rhs constant - zero" ) {
        {
        coek::Model m;
        coek::Expression p;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression e = w/p;
        auto E = e.expand();
        coek::MutableNLPExpr repn;
        REQUIRE_THROWS_WITH(repn.collect_terms(E),
            "Division by zero error.");
        }
    }
    WHEN( "rhs polynomial" ) {
        {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression e = w/(1+w);
        auto E = e.expand();
        coek::MutableNLPExpr repn;
        repn.collect_terms(E);

        static std::list<std::string> constval = { "0.000" };
        static std::list<std::string> nonlinear = { "[", "/", "w", "[", "+", "1.000", "w", "]", "]" };
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 0 );
        REQUIRE( repn.quadratic_coefs.size() == 0 );
        REQUIRE( repn.nonlinear.to_list() == nonlinear );
        }
    }
    WHEN( "rhs nonzero" ) {
        {
        coek::Model m;
        coek::Parameter p(2.0, "p");
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression e = (w*w+w+1)/p;
        auto E = e.expand();
        coek::MutableNLPExpr repn;
        repn.collect_terms(E);

        static std::list<std::string> constval = { "[", "/", "1.000", "p", "]" };
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 1 );
        REQUIRE( repn.linear_coefs[0].to_list() == constval );
        REQUIRE( repn.quadratic_coefs.size() == 1 );
        REQUIRE( repn.quadratic_coefs[0].to_list() == constval );
        }
    }
  }

  SECTION( "constriaints" ) {
    WHEN( "inequality" ) {
        {
        coek::Model m;
        coek::Parameter p(0, "p");
        coek::Variable w = m.getVariable(0,1,3,"w");
        coek::Constraint e = p*w -3 <= 2;
        auto E = e.expand();
        coek::MutableNLPExpr repn;
        repn.collect_terms(E);

        static std::list<std::string> constval = { "-3.000" };
        static std::list<std::string> coefval = { "p" };
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 1 );
        REQUIRE( repn.linear_coefs[0].to_list() == coefval );
        REQUIRE( repn.linear_vars[0] == w.repn );
        REQUIRE( repn.quadratic_coefs.size() == 0 );
        }
    }
    WHEN( "equality" ) {
        {
        coek::Model m;
        coek::Parameter p(0, "p");
        coek::Variable w = m.getVariable(0,1,3,"w");
        coek::Constraint e = p*w +1 == 2;
        auto E = e.expand();
        coek::MutableNLPExpr repn;
        repn.collect_terms(E);

        static std::list<std::string> constval = { "1.000" };
        static std::list<std::string> coefval = { "p" };
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 1 );
        REQUIRE( repn.linear_coefs[0].to_list() == coefval );
        REQUIRE( repn.linear_vars[0] == w.repn );
        REQUIRE( repn.quadratic_coefs.size() == 0 );
        }
    }
  }

  SECTION( "intrinsic funcs" ) {
    NLP_INTRINSIC_TEST1(abs, ENV_MEMCHECK)
    NLP_INTRINSIC_TEST1(ceil, ENV_MEMCHECK)
    NLP_INTRINSIC_TEST1(floor, ENV_MEMCHECK)
    NLP_INTRINSIC_TEST1(exp, ENV_MEMCHECK)
    NLP_INTRINSIC_TEST1(log, ENV_MEMCHECK)
    NLP_INTRINSIC_TEST1(log10, ENV_MEMCHECK)
    NLP_INTRINSIC_TEST1(sqrt, ENV_MEMCHECK)
    NLP_INTRINSIC_TEST1(sin, ENV_MEMCHECK)
    NLP_INTRINSIC_TEST1(cos, ENV_MEMCHECK)
    NLP_INTRINSIC_TEST1(tan, ENV_MEMCHECK)
    NLP_INTRINSIC_TEST1(sinh, ENV_MEMCHECK)
    NLP_INTRINSIC_TEST1(cosh, ENV_MEMCHECK)
    NLP_INTRINSIC_TEST1(tanh, ENV_MEMCHECK)
    NLP_INTRINSIC_TEST1(asin, ENV_MEMCHECK)
    NLP_INTRINSIC_TEST1(acos, ENV_MEMCHECK)
    NLP_INTRINSIC_TEST1(atan, ENV_MEMCHECK)
    NLP_INTRINSIC_TEST1(asinh, ENV_MEMCHECK)
    NLP_INTRINSIC_TEST1(acosh, ENV_MEMCHECK)
    NLP_INTRINSIC_TEST1(atanh, ENV_MEMCHECK)
    NLP_INTRINSIC_TEST2(pow, ENV_MEMCHECK)
  }

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}

