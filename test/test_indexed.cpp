#include <typeinfo>
#include "catch2/catch.hpp"
#include "coek/ast/base_terms.hpp"
#include "coek/ast/value_terms.hpp"
#include "coek/coek.hpp"

void zzz() {}

#define NLP_INTRINSIC_TEST1(FN, MEMCHECK)\
    WHEN( #FN " 1" ) {\
          {\
          coek::Model m;\
          auto v = m.add_variable("v").lower(0).upper(1).value(0);\
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
          auto v = m.add_variable("v").lower(0).upper(1).value(0);\
          v.fixed(true);\
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
          auto v = m.add_variable("v").lower(0).upper(1).value(0);\
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
          auto v = m.add_variable("v").lower(0).upper(1).value(0);\
          v.fixed(true);\
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
          auto v = m.add_variable("v").lower(0).upper(1).value(0);\
          v.fixed(true);\
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
          auto v = m.add_variable("v").lower(0).upper(1).value(0);\
          v.fixed(true);\
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


#ifdef COEK_WITH_COMPACT_MODEL
TEST_CASE( "expr_sequence", "[smoke]" ) {

    SECTION( "concrete_expr" ) {
        std::vector<int> v = {1,5,3,7};
        auto s = coek::SetOf( v );
        auto y = coek::variable("y",s);
        auto x = coek::variable("x",s*s);
        auto i = coek::set_index("i");
        auto j = coek::set_index("j");

        WHEN( "y(i)" ) {
            auto tmp = coek::ExpressionSequence(coek::Forall(i).In(s), y(i));
            size_t ii=0;
            for (auto it=tmp.begin(); it != tmp.end(); ++it) {
                std::list<std::string> baseline = {"y(" + std::to_string(v[ii]) + ")"};
                REQUIRE( it->to_list() == baseline);
                ii++;
                }
        }

        WHEN( "1 + y(i)" ) {
            auto tmp = coek::ExpressionSequence(coek::Forall(i).In(s), 1+y(i));
            size_t ii=0;
            for (auto it=tmp.begin(); it != tmp.end(); ++it) {
                std::list<std::string> baseline = {"[", "+", "1.000", "y(" + std::to_string(v[ii]) + ")", "]"};
                REQUIRE( it->to_list() == baseline);
                ii++;
                }
        }

        WHEN( "i + y(i)" ) {
            auto tmp = coek::ExpressionSequence(coek::Forall(i).In(s), i+y(i));
            size_t ii=0;
            for (auto it=tmp.begin(); it != tmp.end(); ++it) {
                std::list<std::string> baseline = {"[", "+", std::to_string(v[ii])+".000", "y(" + std::to_string(v[ii]) + ")", "]"};
                REQUIRE( it->to_list() == baseline);
                ii++;
                }
        }

        WHEN( "y(i+2)" ) {
            std::vector<int> w = {1,5,3};
            auto S = coek::SetOf( w );
            auto tmp = coek::ExpressionSequence(coek::Forall(i).In(S), y(i+2));
            size_t ii=0;
            for (auto it=tmp.begin(); it != tmp.end(); ++it) {
                std::list<std::string> baseline = {"y(" + std::to_string(v[ii]+2) + ")"};
                REQUIRE( it->to_list() == baseline);
                ii++;
                }
        }

        WHEN( "y(i)+i*y(i+2)" ) {
            std::vector<int> w = {1,5,3};
            auto S = coek::SetOf( w );
            auto tmp = coek::ExpressionSequence(coek::Forall(i).In(S), y(i)+i*y(i+2));
            size_t ii=0;
            for (auto it=tmp.begin(); it != tmp.end(); ++it) {
                std::list<std::string> baseline = {"[", "+", "y(" + std::to_string(v[ii]) + ")", "[", "*", std::to_string(v[ii])+".000", "y(" + std::to_string(v[ii]+2) + ")", "]", "]"};
                REQUIRE( it->to_list() == baseline);
                ii++;
                }
        }

        WHEN( "Sum( y(i) )" ) {
            auto tmp = coek::Sum( coek::Forall(i).In(s), y(i) );
            auto e = tmp.expand();
            std::list<std::string> baseline = { "[", "+", "y(1)", "y(5)", "y(3)", "y(7)", "]" };
            REQUIRE( e.to_list() == baseline);
        }

        WHEN( "Sum( x(i,i) )" ) {
            auto tmp = Sum( coek::Forall(i).In(s), x(i,i) );
            auto e = tmp.expand();
            std::list<std::string> baseline = { "[", "+", "x(1,1)", "x(5,5)", "x(3,3)", "x(7,7)", "]" };
            REQUIRE( e.to_list() == baseline);
        }

        WHEN( "Sum( x(i,j) ) [1]" ) {
            auto tmp = Sum( coek::Forall(i,j).In(s*s), x(i,j) );
            auto e = tmp.expand();
            std::list<std::string> baseline =  { "[", "+", "x(1,1)", "x(1,5)", "x(1,3)", "x(1,7)", "x(5,1)", "x(5,5)", "x(5,3)", "x(5,7)", "x(3,1)", "x(3,5)", "x(3,3)", "x(3,7)", "x(7,1)", "x(7,5)", "x(7,3)", "x(7,7)", "]" };
            REQUIRE( e.to_list() == baseline);
        }

        WHEN( "Sum( x(i,j) ) [2]" ) {
            auto tmp = Sum( coek::Forall(i).In(s) .Forall(j).In(s), x(i,j) );
            auto e = tmp.expand();
            std::list<std::string> baseline = { "[", "+", "x(1,1)", "x(1,5)", "x(1,3)", "x(1,7)", "x(5,1)", "x(5,5)", "x(5,3)", "x(5,7)", "x(3,1)", "x(3,5)", "x(3,3)", "x(3,7)", "x(7,1)", "x(7,5)", "x(7,3)", "x(7,7)", "]" };
            REQUIRE( e.to_list() == baseline);
        }

        WHEN( "Sum( y(i)*x(i,i) )" ) {
            auto tmp = Sum( coek::Forall(i).In(s), y(i)*x(i,i) );
            auto e = tmp.expand();
            std::list<std::string> baseline = { "[", "+", "[", "*", "y(1)", "x(1,1)", "]", "[", "*", "y(5)", "x(5,5)", "]", "[", "*", "y(3)", "x(3,3)", "]", "[", "*", "y(7)", "x(7,7)", "]", "]" };
            REQUIRE( e.to_list() == baseline);
        }

        WHEN( "Sum_i( y(i)*Sum_j(x(i,j)) )" ) {
            auto tmp = Sum( coek::Forall(i).In(s), y(i)*Sum(coek::Forall(j).In(s), x(i,j)) );
            auto e = tmp.expand();
            std::list<std::string> baseline =  { "[", "+", "[", "*", "y(1)", "[", "+", "x(1,1)", "x(1,5)", "x(1,3)", "x(1,7)", "]", "]", "[", "*", "y(5)", "[", "+", "x(5,1)", "x(5,5)", "x(5,3)", "x(5,7)", "]", "]", "[", "*", "y(3)", "[", "+", "x(3,1)", "x(3,5)", "x(3,3)", "x(3,7)", "]", "]", "[", "*", "y(7)", "[", "+", "x(7,1)", "x(7,5)", "x(7,3)", "x(7,7)", "]", "]", "]" };
            REQUIRE( e.to_list() == baseline);
        }
    }

    SECTION( "concrete_con" ) {
        std::vector<int> v = {1,5,3,7};
        auto s = coek::SetOf( v );
        auto y = coek::variable("y",s);
        auto x = coek::variable("x",s*s);
        auto i = coek::set_index("i");

        WHEN( "y(i) == 0" ) {
            auto tmp = coek::ConstraintSequence(coek::Forall(i).In(s), y(i) == 0);
            size_t ii=0;
            for (auto it=tmp.begin(); it != tmp.end(); ++it) {
                std::list<std::string> baseline = {"[", "==", "y(" + std::to_string(v[ii]) + ")", "0.000", "]"};
                REQUIRE( it->to_list() == baseline);
                ii++;
                }
        }

        WHEN( "1 + y(i) == 0" ) {
            auto tmp = coek::ConstraintSequence(coek::Forall(i).In(s), 1+y(i) == 0);
            size_t ii=0;
            for (auto it=tmp.begin(); it != tmp.end(); ++it) {
                std::list<std::string> baseline = {"[", "==", "[", "+", "1.000", "y(" + std::to_string(v[ii]) + ")", "]", "0.000", "]"};
                REQUIRE( it->to_list() == baseline);
                ii++;
                }
        }

        WHEN( "i + y(i) == 0" ) {
            auto tmp = coek::ConstraintSequence(coek::Forall(i).In(s), i+y(i) == 0);
            size_t ii=0;
            for (auto it=tmp.begin(); it != tmp.end(); ++it) {
                std::list<std::string> baseline = {"[", "==", "[", "+", std::to_string(v[ii])+".000", "y(" + std::to_string(v[ii]) + ")", "]", "0.000", "]"};
                REQUIRE( it->to_list() == baseline);
                ii++;
                }
        }

        WHEN( "y(i+2) == 0" ) {
            std::vector<int> w = {1,5,3};
            auto S = coek::SetOf( w );
            auto tmp = coek::ConstraintSequence(coek::Forall(i).In(S), y(i+2) == 0);
            size_t ii=0;
            for (auto it=tmp.begin(); it != tmp.end(); ++it) {
                std::list<std::string> baseline = {"[", "==", "y(" + std::to_string(v[ii]+2) + ")", "0.000", "]"};
                REQUIRE( it->to_list() == baseline);
                ii++;
                }
        }

        WHEN( "y(i)+i*y(i+2) == 0" ) {
            std::vector<int> w = {1,5,3};
            auto S = coek::SetOf( w );
            auto tmp = coek::ConstraintSequence(Forall(i).In(S), y(i)+i*y(i+2)==0);
            size_t ii=0;
            for (auto it=tmp.begin(); it != tmp.end(); ++it) {
                std::list<std::string> baseline = {"[", "==", "[", "+", "y(" + std::to_string(v[ii]) + ")", "[", "*", std::to_string(v[ii])+".000", "y(" + std::to_string(v[ii]+2) + ")", "]", "]", "0.000", "]"};
                REQUIRE( it->to_list() == baseline);
                ii++;
                }
        }

        WHEN( "Sum( y(i) ) == 0" ) {
            auto tmp = Sum( coek::Forall(i).In(s), y(i) ) == 0;
            auto e = tmp.expand();
            std::list<std::string> baseline = { "[", "==", "[", "+", "y(1)", "y(5)", "y(3)", "y(7)", "]", "0.000", "]" };
            REQUIRE( e.to_list() == baseline);
        }

        WHEN( "Sum( y(i)*x(i,i) ) == 0" ) {
            auto tmp = Sum( coek::Forall(i).In(s), y(i)*x(i,i) ) == 0;
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
        auto p = coek::parameter("p").value(3);
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
        auto p = coek::parameter("p").value(3);
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
        auto v = m.add_variable("v").lower(0).upper(1).value(3);
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
        auto v = m.add_variable("v").lower(0).upper(1).value(3);
        v.fixed(true);
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
        auto v = m.add_variable("v").lower(0).upper(1).value(3);
        v.fixed(true);
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
        auto v = m.add_variable("v").lower(0).upper(1).value(0);
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
        auto v = m.add_variable("v").lower(0).upper(1).value(3);
        v.fixed(true);
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
        auto v = m.add_variable("v").lower(0).upper(1).value(3);
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
        auto v = m.add_variable("v").lower(0).upper(1).value(3);
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
        auto v = m.add_variable("v").lower(0).upper(1).value(3);
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
        auto p = coek::parameter("p");
        auto w = m.add_variable("w").lower(0).upper(1).value(3);
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
        auto p = coek::parameter("p");
        auto w = m.add_variable("w").lower(0).upper(1).value(3);
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
        auto w = m.add_variable("w").lower(0).upper(1).value(0);
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
        auto v = m.add_variable("v").lower(0).upper(1).value(0);
        auto w = m.add_variable("w").lower(0).upper(1).value(0);
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
        auto v = m.add_variable("v").lower(0).upper(1).value(0);
        auto w = m.add_variable("w").lower(0).upper(1).value(0);
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
        auto w = m.add_variable("w").lower(0).upper(1).value(0);
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
        auto w = m.add_variable("w").lower(0).upper(1).value(0);
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
        auto p = coek::parameter("p");
        auto w = m.add_variable("w").lower(0).upper(1).value(0);
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
        auto p = coek::parameter("p");
        auto w = m.add_variable("w").lower(0).upper(1).value(0);
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
        auto w = m.add_variable("w").lower(0).upper(1).value(0);
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
        auto w = coek::parameter("w").value(1);
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
        auto w = m.add_variable("w").lower(0).upper(1).value(0);
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
        auto w = m.add_variable("w").lower(0).upper(1).value(0);
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
        auto p = coek::parameter("p").value(2);
        auto w = m.add_variable("w").lower(0).upper(1).value(0);
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
        auto p = coek::parameter("p");
        auto w = m.add_variable("w").lower(0).upper(1).value(3);
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
        auto p = coek::parameter("p");
        auto w = m.add_variable("w").lower(0).upper(1).value(3);
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
#endif
