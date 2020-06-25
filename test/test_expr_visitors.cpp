
#include <sstream>
#include <iostream>
#include "catch.hpp"
#include "coek/expr/ast_term.hpp"
#include "coek/expr/ast_visitors.hpp"
#include "coek/coek.hpp"

#define WRITER_INTRINSIC_TEST1(FN)\
    WHEN( #FN ) {\
          coek::Variable v(0, 1, 0, "v");\
          coek::Expression e = FN(v+1);\
          std::stringstream sstr;\
          sstr << e;\
          REQUIRE( sstr.str() == #FN "(v + 1)" );\
      }

#define WRITER_INTRINSIC_TEST2(FN)\
    WHEN( #FN ) {\
          coek::Variable v(0, 1, 0, "v");\
          coek::Expression e = FN(v+1, 2*v);\
          std::stringstream sstr;\
          sstr << e;\
          REQUIRE( sstr.str() == #FN "(v + 1, 2*v)" );\
      }


#define REPN_INTRINSIC_TEST1(FN)\
    WHEN( #FN " 1" ) {\
          coek::Model m;\
          coek::Variable v = m.getVariable(0, 1, 0, "v");\
          coek::Expression e = FN(v+1);\
          coek::QuadraticExpr repn;\
          REQUIRE_THROWS_WITH(repn.collect_terms(e),\
                Catch::Matchers::StartsWith("Nonlinear expressions are not supported for QuadraticExpr"));\
      }\
    WHEN( #FN " 2" ) {\
          coek::Model m;\
          coek::Variable v = m.getVariable(0, 1, 0, "v");\
          v.set_fixed(true);\
          coek::Expression e = FN(v+1);\
          coek::QuadraticExpr repn;\
          repn.collect_terms(e);\
          REQUIRE( repn.linear_coefs.size() + repn.quadratic_coefs.size() == 0 );\
      }

#define REPN_INTRINSIC_TEST2(FN)\
    WHEN( #FN " 1" ) {\
          coek::Model m;\
          coek::Variable v = m.getVariable(0, 1, 0, "v");\
          coek::Expression e = FN(v+1, v-1);\
          coek::QuadraticExpr repn;\
          REQUIRE_THROWS_WITH(repn.collect_terms(e),\
                Catch::Matchers::StartsWith("Nonlinear expressions are not supported for QuadraticExpr"));\
      }\
    WHEN( #FN " 2" ) {\
          coek::Model m;\
          coek::Variable v = m.getVariable(0, 1, 0, "v");\
          v.set_fixed(true);\
          coek::Expression e = FN(v+1, v-1);\
          coek::QuadraticExpr repn;\
          repn.collect_terms(e);\
          REQUIRE( repn.linear_coefs.size() + repn.quadratic_coefs.size() == 0 );\
      }

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
          coek::MutableNLPExpr repn;\
          repn.collect_terms(e);\
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
          coek::MutableNLPExpr repn;\
          repn.collect_terms(e);\
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


TEST_CASE( "expr_writer", "[smoke]" ) {

  SECTION( "constant" ) {
    coek::Expression e(3);
    std::stringstream sstr;
    sstr << e;
    REQUIRE( sstr.str() == "3" );
  }

  SECTION( "param" ) {
    WHEN( "unnamed ") {
        coek::Parameter q(3);
        std::stringstream sstr;
        sstr << q;
        REQUIRE( sstr.str() == "3.000" );
    }
    WHEN( "named ") {
        coek::Parameter q(3, "q");
        std::stringstream sstr;
        sstr << q;
        REQUIRE( sstr.str() == "q" );
    }
  }

  SECTION( "var" ) {
    WHEN( "unnamed ") {
        coek::Variable v(0, 1, 0, "");
        std::stringstream sstr;
        sstr << v;
        REQUIRE( sstr.str()[0] == 'x' );
    }
    WHEN( "named ") {
        coek::Variable v(0, 1, 0, "v");
        std::stringstream sstr;
        sstr << v;
        REQUIRE( sstr.str() == "v" );
    }
  }

  SECTION( "monomial" ) {
    WHEN( "unweighted ") {
        coek::Variable v(0, 1, 0, "v");
        coek::Expression e = 1*v;
        std::stringstream sstr;
        sstr << e;
        REQUIRE( sstr.str() == "v" );
    }
    WHEN( "weighted ") {
        coek::Variable v(0, 1, 0, "v");
        coek::Expression e = 2*v;
        std::stringstream sstr;
        sstr << e;
        REQUIRE( sstr.str() == "2*v" );
    }
  }

  SECTION( "constraint" ) {
    WHEN( "lt ") {
        coek::Variable v(0, 1, 0, "v");
        coek::Constraint e = v < 1;
        std::stringstream sstr;
        sstr << e;
        REQUIRE( sstr.str() == "v < 1" );
    }
    WHEN( "leq ") {
        coek::Variable v(0, 1, 0, "v");
        coek::Constraint e = v <= 1;
        std::stringstream sstr;
        sstr << e;
        REQUIRE( sstr.str() == "v <= 1" );
    }
    WHEN( "eq ") {
        coek::Variable v(0, 1, 0, "v");
        coek::Constraint e = v == 1;
        std::stringstream sstr;
        sstr << e;
        REQUIRE( sstr.str() == "v == 1" );
    }
  }

  SECTION( "negate" ) {
        coek::Variable v(0, 1, 0, "v");
        coek::Expression e = -(v+1);
        std::stringstream sstr;
        sstr << e;
        REQUIRE( sstr.str() == "- (v + 1)" );
  }

  SECTION( "plus" ) {
    WHEN("2 terms") {
        coek::Variable v(0, 1, 0, "v");
        coek::Expression e = v+1;
        std::stringstream sstr;
        sstr << e;
        REQUIRE( sstr.str() == "v + 1" );
    }
    WHEN("combine sums") {
        coek::Variable v(0, 1, 0, "v");
        coek::Expression e = v+1+(-1-v);
        std::stringstream sstr;
        sstr << e;
        REQUIRE( sstr.str() == "v + 1 + -1 + -1*v" );
    }
    WHEN("3 terms sums") {
        coek::Variable v(0, 1, 0, "v");
        coek::Expression e = v+1+3*(-1-v);
        std::stringstream sstr;
        sstr << e;
        REQUIRE( sstr.str() == "v + 1 + (3)*(-1 + -1*v)" );
    }
  }

  SECTION( "times" ) {
        coek::Variable v(0, 1, 0, "v");
        coek::Variable w(0, 1, 0, "w");
        coek::Expression e = v*(w+1);
        std::stringstream sstr;
        sstr << e;
        REQUIRE( sstr.str() == "(v)*(w + 1)" );
  }

  SECTION( "divide" ) {
        coek::Variable v(0, 1, 0, "v");
        coek::Variable w(0, 1, 0, "w");
        coek::Expression e = v/(w+1);
        std::stringstream sstr;
        sstr << e;
        REQUIRE( sstr.str() == "(v)/(w + 1)" );
  }

  SECTION( "intrinsic funcs" ) {
    WRITER_INTRINSIC_TEST1(abs)
    WRITER_INTRINSIC_TEST1(ceil)
    WRITER_INTRINSIC_TEST1(floor)
    WRITER_INTRINSIC_TEST1(exp)
    WRITER_INTRINSIC_TEST1(log)
    WRITER_INTRINSIC_TEST1(log10)
    WRITER_INTRINSIC_TEST1(sqrt)
    WRITER_INTRINSIC_TEST1(sin)
    WRITER_INTRINSIC_TEST1(cos)
    WRITER_INTRINSIC_TEST1(tan)
    WRITER_INTRINSIC_TEST1(sinh)
    WRITER_INTRINSIC_TEST1(cosh)
    WRITER_INTRINSIC_TEST1(tanh)
    WRITER_INTRINSIC_TEST1(asin)
    WRITER_INTRINSIC_TEST1(acos)
    WRITER_INTRINSIC_TEST1(atan)
    WRITER_INTRINSIC_TEST1(asinh)
    WRITER_INTRINSIC_TEST1(acosh)
    WRITER_INTRINSIC_TEST1(atanh)
    WRITER_INTRINSIC_TEST2(pow)
  }

  SECTION( "affine_expression1" ) {
        std::vector<coek::Variable> v(4);
        std::vector<double> w(4);
        for (int i=0; i<4; i++) {
            v[i] = coek::Variable(0, 1, 0, "v[" + std::to_string(i) + "]");
            w[i] = i+1;
            }
        coek::Expression e = affine_expression(w, v, 5.0);
        std::stringstream sstr;
        sstr << e;
        REQUIRE( sstr.str() == "5 + v[0] + 2*v[1] + 3*v[2] + 4*v[3]" );
  }

  SECTION( "affine_expression" ) {
        std::vector<coek::Variable> v(4);
        for (int i=0; i<4; i++) {
            v[i] = coek::Variable(0, 1, 0, "v[" + std::to_string(i) + "]");
            }
        coek::Expression e = affine_expression(v, 5.0);
        std::stringstream sstr;
        sstr << e;
        REQUIRE( sstr.str() == "5 + v[0] + v[1] + v[2] + v[3]" );
  }

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}


TEST_CASE( "expr_to_QuadraticExpr", "[smoke]" ) {

  SECTION( "constant" ) {
    coek::Expression e(3);
    coek::QuadraticExpr repn;
    repn.collect_terms(e);

    REQUIRE( repn.constval == 3 );
    REQUIRE( repn.linear_coefs.size() == 0 );
    REQUIRE( repn.quadratic_coefs.size() == 0 );
  }

  SECTION( "param" ) {
    coek::Parameter p(3);
    coek::Expression e = p;
    coek::QuadraticExpr repn;
    repn.collect_terms(e);

    REQUIRE( repn.constval == 3 );
    REQUIRE( repn.linear_coefs.size() == 0 );
    REQUIRE( repn.quadratic_coefs.size() == 0 );
  }

  SECTION( "var" ) {
    WHEN( "unfixed" ) {
        coek::Model m;
        coek::Variable v = m.getVariable(0,1,0,"v");
        coek::Expression e = v;
        coek::QuadraticExpr repn;
        repn.collect_terms(e);

        REQUIRE( repn.constval == 0 );
        REQUIRE( repn.linear_coefs.size() == 1 );
        REQUIRE( repn.linear_coefs[0] == 1 );
        REQUIRE( repn.linear_vars[0] == e.repn );
        REQUIRE( repn.quadratic_coefs.size() == 0 );
    }
    WHEN( "fixed" ) {
        coek::Model m;
        coek::Variable v = m.getVariable(0,1,3,"v");
        v.set_fixed(true);
        coek::Expression e = v;
        coek::QuadraticExpr repn;
        repn.collect_terms(e);

        REQUIRE( repn.constval == 3 );
        REQUIRE( repn.linear_coefs.size() == 0 );
        REQUIRE( repn.quadratic_coefs.size() == 0 );
    }
  }

  SECTION( "monomial" ) {
    WHEN( "unfixed" ) {
        coek::Model m;
        coek::Variable v = m.getVariable(0,1,0,"v");
        coek::Expression e = 2*v;
        coek::QuadraticExpr repn;
        repn.collect_terms(e);

        REQUIRE( repn.constval == 0 );
        REQUIRE( repn.linear_coefs.size() == 1 );
        REQUIRE( repn.linear_coefs[0] == 2 );
        coek::MonomialTerm* tmp = dynamic_cast<coek::MonomialTerm*>(e.repn);
        REQUIRE( repn.linear_vars[0]->index == tmp->var->index );
        REQUIRE( repn.quadratic_coefs.size() == 0 );
    }
    WHEN( "fixed" ) {
        coek::Model m;
        coek::Variable v = m.getVariable(0,1,3,"v");
        v.set_fixed(true);
        coek::Expression e = 2*v;
        coek::QuadraticExpr repn;
        repn.collect_terms(e);

        REQUIRE( repn.constval == 6 );
        REQUIRE( repn.linear_coefs.size() == 0 );
        REQUIRE( repn.quadratic_coefs.size() == 0 );
    }
  }

  SECTION( "times" ) {
    WHEN( "lhs zero" ) {
        coek::Parameter p(0);
        coek::Variable w(0, 1, 0, "w");
        coek::Expression e = p*w;
        coek::QuadraticExpr repn;
        repn.collect_terms(e);

        REQUIRE( repn.constval == 0 );
        REQUIRE( repn.linear_coefs.size() == 0 );
        REQUIRE( repn.quadratic_coefs.size() == 0 );
    }
    WHEN( "lhs constant" ) {
        coek::Model m;
        coek::Parameter p(2);
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression e = p*w;
        coek::QuadraticExpr repn;
        repn.collect_terms(e);

        REQUIRE( repn.constval == 0 );
        REQUIRE( repn.linear_coefs.size() == 1 );
        REQUIRE( repn.linear_coefs[0] == 2 );
        REQUIRE( repn.linear_vars[0] == w.repn );
        REQUIRE( repn.quadratic_coefs.size() == 0 );
    }
    WHEN( "rhs zero" ) {
        coek::Model m;
        coek::Parameter p(0);
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression e = w*p;
        coek::QuadraticExpr repn;
        repn.collect_terms(e);

        REQUIRE( repn.constval == 0 );
        REQUIRE( repn.linear_coefs.size() == 0 );
        REQUIRE( repn.quadratic_coefs.size() == 0 );
    }
    WHEN( "rhs constant" ) {
        coek::Model m;
        coek::Parameter p(2);
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression e = (w + w*w)*p;
        coek::QuadraticExpr repn;
        repn.collect_terms(e);

        REQUIRE( repn.constval == 0 );
        REQUIRE( repn.linear_coefs.size() == 1 );
        REQUIRE( repn.linear_coefs[0] == 2 );
        REQUIRE( repn.linear_vars[0] == w.repn );
        REQUIRE( repn.quadratic_coefs.size() == 1 );
        REQUIRE( repn.quadratic_coefs[0] == 2 );
        REQUIRE( repn.quadratic_lvars[0] == w.repn );
        REQUIRE( repn.quadratic_rvars[0] == w.repn );
    }
    WHEN( "simple quadratic" ) {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression e = w*w;
        coek::QuadraticExpr repn;
        repn.collect_terms(e);

        REQUIRE( repn.constval == 0 );
        REQUIRE( repn.linear_coefs.size() == 0 );
        REQUIRE( repn.quadratic_coefs.size() == 1 );
        REQUIRE( repn.quadratic_coefs[0] == 1 );
        REQUIRE( repn.quadratic_lvars[0] == w.repn );
        REQUIRE( repn.quadratic_rvars[0] == w.repn );
    }
    WHEN( "complex quadratic 1" ) {
        coek::Model m;
        coek::Variable v = m.getVariable(0, 1, 0, "v");
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression e = (2 + 3*w + v)*(4 + 5*v + w);
        coek::QuadraticExpr repn;
        repn.collect_terms(e);

        REQUIRE( repn.constval == 8 );
        REQUIRE( repn.linear_coefs.size() == 4 );
        REQUIRE( repn.linear_coefs[0] == 10 );
        REQUIRE( repn.linear_coefs[1] == 2 );
        REQUIRE( repn.linear_coefs[2] == 12 );
        REQUIRE( repn.linear_coefs[3] == 4 );
        REQUIRE( repn.quadratic_coefs.size() == 4 );
        REQUIRE( repn.quadratic_coefs[0] == 15 );
        REQUIRE( repn.quadratic_coefs[1] == 3 );
        REQUIRE( repn.quadratic_coefs[2] == 5 );
        REQUIRE( repn.quadratic_coefs[3] == 1 );
    }
    WHEN( "error 1" ) {
        coek::Model m;
        coek::Variable v = m.getVariable(0, 1, 0, "v");
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression e = (w + v*v)*v;
        coek::QuadraticExpr repn;
        REQUIRE_THROWS_WITH(repn.collect_terms(e),
            "Non-quadratic expressions cannot be expressed in a QuadraticExpr object.");
    }
    WHEN( "error 2" ) {
        coek::Model m;
        coek::Variable v = m.getVariable(0, 1, 0, "v");
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression e = v*(w + v*v);
        coek::QuadraticExpr repn;
        REQUIRE_THROWS_WITH(repn.collect_terms(e),
            "Non-quadratic expressions cannot be expressed in a QuadraticExpr object.");
    }
  }

  SECTION( "divide" ) {
    WHEN( "lhs zero" ) {
        coek::Model m;
        coek::Parameter p(0);
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression e = p/w;
        coek::QuadraticExpr repn;
        repn.collect_terms(e);

        REQUIRE( repn.constval == 0 );
        REQUIRE( repn.linear_coefs.size() == 0 );
        REQUIRE( repn.quadratic_coefs.size() == 0 );
    }
    WHEN( "rhs zero" ) {
        coek::Model m;
        coek::Parameter p(0);
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression e = w/p;
        coek::QuadraticExpr repn;
        REQUIRE_THROWS_WITH(repn.collect_terms(e),
            "Division by zero error.");
    }
    WHEN( "rhs polynomial" ) {
        coek::Model m;
        coek::Parameter p(0);
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression e = w/(1+w);
        coek::QuadraticExpr repn;
        REQUIRE_THROWS_WITH(repn.collect_terms(e),
            "Non-constant expressions cannot appear in the denominator of quadratic expressions.");
    }
    WHEN( "rhs nonzero" ) {
        coek::Model m;
        coek::Parameter p(2.0);
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression e = (w*w+w+1)/p;
        coek::QuadraticExpr repn;
        repn.collect_terms(e);

        REQUIRE( repn.constval == 0.5 );
        REQUIRE( repn.linear_coefs.size() == 1 );
        REQUIRE( repn.linear_coefs[0] == 0.5 );
        REQUIRE( repn.quadratic_coefs.size() == 1 );
        REQUIRE( repn.quadratic_coefs[0] == 0.5 );
    }
  }

  SECTION( "intrinsic funcs" ) {
    REPN_INTRINSIC_TEST1(abs)
    REPN_INTRINSIC_TEST1(ceil)
    REPN_INTRINSIC_TEST1(floor)
    REPN_INTRINSIC_TEST1(exp)
    REPN_INTRINSIC_TEST1(log)
    REPN_INTRINSIC_TEST1(log10)
    REPN_INTRINSIC_TEST1(sqrt)
    REPN_INTRINSIC_TEST1(sin)
    REPN_INTRINSIC_TEST1(cos)
    REPN_INTRINSIC_TEST1(tan)
    REPN_INTRINSIC_TEST1(sinh)
    REPN_INTRINSIC_TEST1(cosh)
    REPN_INTRINSIC_TEST1(tanh)
    REPN_INTRINSIC_TEST1(asin)
    REPN_INTRINSIC_TEST1(acos)
    REPN_INTRINSIC_TEST1(atan)
    REPN_INTRINSIC_TEST1(asinh)
    REPN_INTRINSIC_TEST1(acosh)
    REPN_INTRINSIC_TEST1(atanh)
    REPN_INTRINSIC_TEST2(pow)
  }

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}


TEST_CASE( "symbolic_diff", "[smoke]" ) {

  SECTION( "constant" ) {
    coek::Expression f(3);
    coek::Variable v;
    auto e = f.diff(v);
    static std::list<std::string> baseline = {"0.000"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "param" ) {
    coek::Parameter p(3);
    coek::Expression f = p;
    coek::Variable v;
    auto e = f.diff(v);
    static std::list<std::string> baseline = {"0.000"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "var" ) {
    WHEN( "fixed" ) {
        coek::Variable v;
        v.set_fixed(true);
        coek::Expression f = v;
        auto e = f.diff(v);
        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "same" ) {
        coek::Variable v;
        coek::Expression f = v;
        auto e = f.diff(v);
        static std::list<std::string> baseline = {"1.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "different" ) {
        coek::Variable v;
        coek::Variable w;
        coek::Expression f = w;
        auto e = f.diff(v);
        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "monomial" ) {
    WHEN( "other" ) {
        coek::Model m;
        coek::Variable v = m.getVariable(0,1,0,"v");
        coek::Variable w = m.getVariable(0,1,0,"w");
        coek::Expression f = 2*v;
        auto e = f.diff(w);
        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "same" ) {
        coek::Model m;
        coek::Variable v = m.getVariable(0,1,0,"v");
        coek::Expression f = 2*v;
        auto e = f.diff(v);
        static std::list<std::string> baseline = {"2.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "fixed" ) {
        coek::Model m;
        coek::Variable v = m.getVariable(0,1,0,"v");
        v.set_fixed(true);
        coek::Expression f = 2*v;
        auto e = f.diff(v);
        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "plus" ) {
    WHEN( "linear" ) {
        coek::Parameter p(0);
        coek::Variable v(0, 1, 0, "v");
        coek::Expression f = 2*(v+v)+v;
        auto e = f.diff(v);
        static std::list<std::string> baseline = {"5.000"};
        REQUIRE( e.to_list() == baseline );
    }
    WHEN( "simple" ) {
        coek::Parameter p(0);
        coek::Variable v(0, 1, 0, "v");
        coek::Expression f = 3*p+2*v;
        auto e = f.diff(v);
        static std::list<std::string> baseline = {"2.000"};
        REQUIRE( e.to_list() == baseline );
    }
    WHEN( "multiple" ) {
        coek::Parameter p(0);
        coek::Variable v(0, 1, 0, "v");
        coek::Expression f = 7*v+v;
        auto e = f.diff(v);
        static std::list<std::string> baseline = {"8.000"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "negate" ) {
    WHEN( "linear" ) {
        coek::Parameter p(0);
        coek::Variable v(0, 1, 0, "v");
        coek::Expression f = -(v+1);
        auto e = f.diff(v);
        static std::list<std::string> baseline = {"-1.000"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "times" ) {
    WHEN( "lhs zero" ) {
        coek::Expression p;
        coek::Variable v(0, 1, 0, "v");
        coek::Expression f = p*v;
        auto e = f.diff(v);
        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }
    WHEN( "lhs constant" ) {
        coek::Model m;
        coek::Parameter p(2,"p");
        coek::Variable v = m.getVariable(0, 1, 0, "v");
        coek::Expression f = p*v;
        auto e = f.diff(v);
        static std::list<std::string> baseline = {"p"};
        REQUIRE( e.to_list() == baseline );
    }
    WHEN( "rhs zero" ) {
        coek::Model m;
        coek::Expression p;
        coek::Variable v = m.getVariable(0, 1, 0, "v");
        coek::Expression f = v*p;
        auto e = f.diff(v);
        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }
    WHEN( "rhs constant" ) {
        coek::Model m;
        coek::Parameter p(2, "p");
        coek::Variable v = m.getVariable(0, 1, 0, "v");
        coek::Expression f = v*p;
        auto e = f.diff(v);
        static std::list<std::string> baseline = {"p"};
        REQUIRE( e.to_list() == baseline );
    }
    WHEN( "simple quadratic LHS" ) {
        coek::Model m;
        coek::Variable v = m.getVariable(0, 1, 0, "v");
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = v*w;
        auto e = f.diff(v);
        static std::list<std::string> baseline = {"w"};
        REQUIRE( e.to_list() == baseline );
    }
    WHEN( "simple quadratic RHS" ) {
        coek::Model m;
        coek::Variable v = m.getVariable(0, 1, 0, "v");
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = v*w;
        auto e = f.diff(w);
        static std::list<std::string> baseline = {"v"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "divide" ) {
    WHEN( "lhs zero" ) {
        coek::Model m;
        coek::Parameter p(0,"p");
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = p/w;
        auto e = f.diff(w);
        static std::list<std::string> baseline = {"[", "/", "[", "*", "-1.000", "p", "]", "[", "*", "w", "w", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }
    WHEN( "rhs nonzero" ) {
        coek::Model m;
        coek::Parameter p(2.0,"p");
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = w/p;
        auto e = f.diff(w);
        static std::list<std::string> baseline = {"[", "/", "1.000", "p", "]"};
        REQUIRE( e.to_list() == baseline );
    }
    WHEN( "rhs polynomial" ) {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = w/(1+w);
        auto e = f.diff(w);
        static std::list<std::string> baseline = { "[", "+", "[", "/", "1.000", "[", "+", "1.000", "w", "]", "]", "[", "/", "[", "*", "-1.000", "w", "]", "[", "*", "[", "+", "1.000", "w", "]", "[", "+", "1.000", "w", "]", "]", "]", "]" };
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "coverage" ) {
    WHEN( "variable partial plus monomial - 1" ) {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Variable v = m.getVariable(0, 1, 0, "v");
        coek::Expression f = w*v + v*(2*w+1);
        auto e = f.diff(w);
        static std::list<std::string> baseline = { "[", "+", "v", "[", "*", "2.000", "v", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }
    WHEN( "variable partial plus monomial - 2" ) {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Variable v = m.getVariable(0, 1, 0, "v");
        coek::Expression f = v*(2*w+1);
        auto e = f.diff(w);
        static std::list<std::string> baseline = { "[", "*", "2.000", "v", "]"};
        REQUIRE( e.to_list() == baseline );
    }
    WHEN( "constant partial plus monomial" ) {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = 3*w + 2*w;
        auto e = f.diff(w);
        static std::list<std::string> baseline = { "5.000"};
        REQUIRE( e.to_list() == baseline );
    }
    WHEN( "negative monomial" ) {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = -(-w) + (-(-w));
        auto e = f.diff(w);
        static std::list<std::string> baseline = { "2.000"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "intrinsic funcs" ) {
    REPN_INTRINSIC_TEST1(abs)
    REPN_INTRINSIC_TEST1(ceil)
    REPN_INTRINSIC_TEST1(floor)
    WHEN( "exp" ) {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = exp(2*w);
        auto e = f.diff(w);
        static std::list<std::string> baseline =  { "[", "*", "2.000", "[", "exp", "[", "*", "2", "w", "]", "]", "]" };
        REQUIRE( e.to_list() == baseline );
    }
    WHEN( "log" ) {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = log(2*w);
        auto e = f.diff(w);
        static std::list<std::string> baseline = { "[", "*", "2.000", "[", "/", "1.000", "[", "*", "2", "w", "]", "]", "]" };
        REQUIRE( e.to_list() == baseline );
    }
    WHEN( "log10" ) {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = log10(2*w);
        auto e = f.diff(w);
        static std::list<std::string> baseline = { "[", "*", "2.000", "[", "/", "1.000", "[", "*", "2.303", "[", "*", "2", "w", "]", "]", "]", "]" };
        REQUIRE( e.to_list() == baseline );
    }
    WHEN( "sqrt" ) {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = sqrt(2*w);
        auto e = f.diff(w);
        static std::list<std::string> baseline = { "[", "*", "2.000", "[", "pow", "[", "*", "2", "w", "]", "-0.500", "]", "]" };
        REQUIRE( e.to_list() == baseline );
    }
    WHEN( "sin" ) {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = sin(2*w);
        auto e = f.diff(w);
        static std::list<std::string> baseline = { "[", "*", "2.000", "[", "cos", "[", "*", "2", "w", "]", "]", "]" };
        REQUIRE( e.to_list() == baseline );
    }
    WHEN( "cos" ) {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = cos(2*w);
        auto e = f.diff(w);
        static std::list<std::string> baseline = { "[", "*", "2.000", "[", "-", "[", "sin", "[", "*", "2", "w", "]", "]", "]", "]" };
        REQUIRE( e.to_list() == baseline );
    }
    WHEN( "tan" ) {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = tan(2*w);
        auto e = f.diff(w);
        static std::list<std::string> baseline = { "[", "*", "2.000", "[", "/", "1.000", "[", "pow", "[", "cos", "[", "*", "2", "w", "]", "]", "2.000", "]", "]", "]" };
        REQUIRE( e.to_list() == baseline );
    }
    WHEN( "sinh" ) {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = sinh(2*w);
        auto e = f.diff(w);
        static std::list<std::string> baseline = { "[", "*", "2.000", "[", "cosh", "[", "*", "2", "w", "]", "]", "]" };
        REQUIRE( e.to_list() == baseline );
    }
    WHEN( "cosh" ) {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = cosh(2*w);
        auto e = f.diff(w);
        static std::list<std::string> baseline = { "[", "*", "2.000", "[", "sinh", "[", "*", "2", "w", "]", "]", "]" };
        REQUIRE( e.to_list() == baseline );
    }
    WHEN( "tanh" ) {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = tanh(2*w);
        auto e = f.diff(w);
        static std::list<std::string> baseline = { "[", "*", "2.000", "[", "+", "1.000", "[", "*", "-1.000", "[", "pow", "[", "tan", "[", "*", "2", "w", "]", "]", "2.000", "]", "]", "]", "]" };
        REQUIRE( e.to_list() == baseline );
    }
    WHEN( "asin" ) {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = asin(2*w);
        auto e = f.diff(w);
        static std::list<std::string> baseline = { "[", "*", "2.000", "[", "/", "1.000", "[", "sqrt", "[", "+", "1.000", "[", "-", "[", "*", "[", "*", "2", "w", "]", "[", "*", "2", "w", "]", "]", "]", "]", "]", "]", "]" };
        REQUIRE( e.to_list() == baseline );
    }
    WHEN( "acos" ) {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = acos(2*w);
        auto e = f.diff(w);
        static std::list<std::string> baseline = { "[", "*", "2.000", "[", "-", "[", "/", "1.000", "[", "sqrt", "[", "+", "1.000", "[", "-", "[", "*", "[", "*", "2", "w", "]", "[", "*", "2", "w", "]", "]", "]", "]", "]", "]", "]", "]" };
        REQUIRE( e.to_list() == baseline );
    }
    WHEN( "atan" ) {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = atan(2*w);
        auto e = f.diff(w);
        static std::list<std::string> baseline = { "[", "*", "2.000", "[", "/", "1.000", "[", "+", "1.000", "[", "*", "[", "*", "2", "w", "]", "[", "*", "2", "w", "]", "]", "]", "]", "]" };
        REQUIRE( e.to_list() == baseline );
    }
    WHEN( "asinh" ) {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = asinh(2*w);
        auto e = f.diff(w);
        static std::list<std::string> baseline = { "[", "*", "2.000", "[", "/", "1.000", "[", "sqrt", "[", "+", "1.000", "[", "*", "[", "*", "2", "w", "]", "[", "*", "2", "w", "]", "]", "]", "]", "]", "]" };
        REQUIRE( e.to_list() == baseline );
    }
    WHEN( "acosh" ) {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = acosh(2*w);
        auto e = f.diff(w);
        static std::list<std::string> baseline = { "[", "*", "2.000", "[", "/", "1.000", "[", "sqrt", "[", "+", "[", "*", "[", "*", "2", "w", "]", "[", "*", "2", "w", "]", "]", "-1.000", "]", "]", "]", "]" };
        REQUIRE( e.to_list() == baseline );
    }
    WHEN( "atanh" ) {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = atanh(2*w);
        auto e = f.diff(w);
        static std::list<std::string> baseline = { "[", "*", "2.000", "[", "/", "1.000", "[", "+", "[", "*", "[", "*", "2", "w", "]", "[", "*", "2", "w", "]", "]", "-1.000", "]", "]", "]" };
        REQUIRE( e.to_list() == baseline );
    }
    WHEN( "pow - 1" ) {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = pow(2*w, 3);
        auto e = f.diff(w);
        static std::list<std::string> baseline = { "[", "*", "2.000", "[", "*", "3.000", "[", "pow", "[", "*", "2", "w", "]", "[", "+", "3.000", "-1.000", "]", "]", "]", "]" };
        REQUIRE( e.to_list() == baseline );
    }
    WHEN( "pow - 2" ) {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = pow(3, 2*w);
        auto e = f.diff(w);
        static std::list<std::string> baseline = { "[", "*", "2.000", "[", "*", "1.099", "[", "pow", "3.000", "[", "*", "2", "w", "]", "]", "]", "]" };
        REQUIRE( e.to_list() == baseline );
    }
  }

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}


TEST_CASE( "expr_to_MutableNLPExpr", "[smoke]" ) {

  SECTION( "constant" ) {
    {
    coek::Expression e(3);
    coek::MutableNLPExpr repn;
    repn.collect_terms(e);

    static std::list<std::string> constval = { "3.000" };
    REQUIRE( repn.mutable_values == false );
    REQUIRE( repn.constval.to_list() == constval );
    REQUIRE( repn.linear_coefs.size() == 0 );
    REQUIRE( repn.quadratic_coefs.size() == 0 );
    }
    #ifdef DEBUG
    REQUIRE( coek::env.check_memory() == true );
    #endif
  }

  SECTION( "param" ) {
    WHEN( "simple" ) {
        {
        coek::Parameter p(3, "p");
        coek::Expression e = p;
        coek::MutableNLPExpr repn;
        repn.collect_terms(e);

        static std::list<std::string> constval = { "p" };
        REQUIRE( repn.mutable_values == true );
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 0 );
        REQUIRE( repn.quadratic_coefs.size() == 0 );
        }
        #ifdef DEBUG
        REQUIRE( coek::env.check_memory() == true );
        #endif
    }
    WHEN( "nontrivial multiplier" ) {
        {
        coek::Parameter p(3, "p");
        coek::Expression e = p/2;
        coek::MutableNLPExpr repn;
        repn.collect_terms(e);

        static std::list<std::string> constval = { "[", "*", "0.500", "p", "]" };
        REQUIRE( repn.mutable_values == true );
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 0 );
        REQUIRE( repn.quadratic_coefs.size() == 0 );
        }
        #ifdef DEBUG
        REQUIRE( coek::env.check_memory() == true );
        #endif
    }
  }

  SECTION( "var" ) {
    WHEN( "unfixed" ) {
        {
        coek::Model m;
        coek::Variable v = m.getVariable(0,1,3,"v");
        coek::Expression e = v;
        coek::MutableNLPExpr repn;
        repn.collect_terms(e);

        static std::list<std::string> constval = { "0.000" };
        static std::list<std::string> coefval = { "1.000" };
        REQUIRE( repn.mutable_values == false );
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 1 );
        REQUIRE( repn.linear_coefs[0].to_list() == coefval );
        REQUIRE( repn.linear_vars[0] == e.repn );
        REQUIRE( repn.quadratic_coefs.size() == 0 );
        }
        #ifdef DEBUG
        REQUIRE( coek::env.check_memory() == true );
        #endif
    }
    WHEN( "fixed" ) {
        {
        coek::Model m;
        coek::Variable v = m.getVariable(0,1,3,"v");
        v.set_fixed(true);
        coek::Expression e = v;
        coek::MutableNLPExpr repn;
        repn.collect_terms(e);

        static std::list<std::string> constval = { "v" };
        REQUIRE( repn.mutable_values == true );
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 0 );
        REQUIRE( repn.quadratic_coefs.size() == 0 );
        }
        #ifdef DEBUG
        REQUIRE( coek::env.check_memory() == true );
        #endif
    }
    WHEN( "fixed - nontrivial multiplier" ) {
        {
        coek::Model m;
        coek::Variable v = m.getVariable(0,1,3,"v");
        v.set_fixed(true);
        coek::Expression f(2);
        coek::Expression e = v/f;
        coek::MutableNLPExpr repn;
        repn.collect_terms(e);

        static std::list<std::string> constval = { "[", "*", "0.500", "v", "]" };
        REQUIRE( repn.mutable_values == true );
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 0 );
        REQUIRE( repn.quadratic_coefs.size() == 0 );
        }
        #ifdef DEBUG
        REQUIRE( coek::env.check_memory() == true );
        #endif
    }
  }

  SECTION( "monomial" ) {
    WHEN( "unfixed" ) {
        {
        coek::Model m;
        coek::Variable v = m.getVariable(0,1,0,"v");
        coek::Expression e = 2*v;
        coek::MutableNLPExpr repn;
        repn.collect_terms(e);

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
        #ifdef DEBUG
        REQUIRE( coek::env.check_memory() == true );
        #endif
    }
    WHEN( "fixed" ) {
        {
        coek::Model m;
        coek::Variable v = m.getVariable(0,1,3,"v");
        v.set_fixed(true);
        coek::Expression e = 2*v;
        coek::MutableNLPExpr repn;
        repn.collect_terms(e);

        static std::list<std::string> constval = { "[", "*", "2.000", "v", "]" };
        REQUIRE( repn.mutable_values == true );
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 0 );
        REQUIRE( repn.quadratic_coefs.size() == 0 );
        }
        #ifdef DEBUG
        REQUIRE( coek::env.check_memory() == true );
        #endif
    }
  }

  SECTION( "negate" ) {
        {
        coek::Model m;
        coek::Variable v = m.getVariable(0,1,3,"v");
        coek::Expression e = -(v+1);
        coek::MutableNLPExpr repn;
        repn.collect_terms(e);

        static std::list<std::string> constval = { "-1.000" };
        static std::list<std::string> coefval = { "-1.000" };
        REQUIRE( repn.mutable_values == false );
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 1 );
        REQUIRE( repn.linear_coefs[0].to_list() == coefval );
        REQUIRE( repn.linear_vars[0] == v.repn );
        REQUIRE( repn.quadratic_coefs.size() == 0 );
        }
        #ifdef DEBUG
        REQUIRE( coek::env.check_memory() == true );
        #endif
  }

  SECTION( "plus" ) {
    WHEN("2 terms") {
        {
        coek::Model m;
        coek::Variable v = m.getVariable(0,1,3,"v");
        coek::Expression e = v+1;
        coek::MutableNLPExpr repn;
        repn.collect_terms(e);

        static std::list<std::string> constval = { "1.000" };
        static std::list<std::string> coefval = { "1.000" };
        REQUIRE( repn.mutable_values == false );
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 1 );
        REQUIRE( repn.linear_coefs[0].to_list() == coefval );
        REQUIRE( repn.linear_vars[0] == v.repn );
        REQUIRE( repn.quadratic_coefs.size() == 0 );
        }
        #ifdef DEBUG
        REQUIRE( coek::env.check_memory() == true );
        #endif
    }
    WHEN("combine sums") {
        {
        coek::Model m;
        coek::Variable v = m.getVariable(0,1,3,"v");
        coek::Expression e = v+1+(-1-v);
        coek::MutableNLPExpr repn;
        repn.collect_terms(e);

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
        #ifdef DEBUG
        REQUIRE( coek::env.check_memory() == true );
        #endif
    }
  }

  SECTION( "times" ) {
    WHEN( "lhs constant" ) {
        {
        coek::Model m;
        coek::Parameter p(0, "p");
        coek::Variable w = m.getVariable(0,1,3,"w");
        coek::Expression e = p*w;
        coek::MutableNLPExpr repn;
        repn.collect_terms(e);

        static std::list<std::string> constval = { "0.000" };
        static std::list<std::string> coefval = { "p" };
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 1 );
        REQUIRE( repn.linear_coefs[0].to_list() == coefval );
        REQUIRE( repn.linear_vars[0] == w.repn );
        REQUIRE( repn.quadratic_coefs.size() == 0 );
        }
        #ifdef DEBUG
        REQUIRE( coek::env.check_memory() == true );
        #endif
    }
    WHEN( "rhs constant" ) {
        {
        coek::Model m;
        coek::Parameter p(0, "p");
        coek::Variable w = m.getVariable(0,1,3,"w");
        coek::Expression e = w*p;
        coek::MutableNLPExpr repn;
        repn.collect_terms(e);

        static std::list<std::string> constval = { "0.000" };
        static std::list<std::string> coefval = { "p" };
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 1 );
        REQUIRE( repn.linear_coefs[0].to_list() == coefval );
        REQUIRE( repn.linear_vars[0] == w.repn );
        REQUIRE( repn.quadratic_coefs.size() == 0 );
        }
        #ifdef DEBUG
        REQUIRE( coek::env.check_memory() == true );
        #endif
    }
    WHEN( "simple quadratic" ) {
        {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression e = w*w;
        coek::MutableNLPExpr repn;
        repn.collect_terms(e);

        static std::list<std::string> constval = { "0.000" };
        static std::list<std::string> qcoefval = { "1.000" };
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 0 );
        REQUIRE( repn.quadratic_coefs.size() == 1 );
        REQUIRE( repn.quadratic_coefs[0].to_list() == qcoefval );
        REQUIRE( repn.quadratic_lvars[0] == w.repn );
        REQUIRE( repn.quadratic_rvars[0] == w.repn );
        }
        #ifdef DEBUG
        REQUIRE( coek::env.check_memory() == true );
        #endif
    }
    WHEN( "complex quadratic 1a" ) {
        {
        // Products of linear expressions are not expanded
        coek::Model m;
        coek::Variable v = m.getVariable(0, 1, 0, "v");
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression e = (2 + 3*w + v)*(4 + 5*v + w);
        coek::MutableNLPExpr repn;
        repn.collect_terms(e);

        static std::list<std::string> constval = { "0.000" };
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 0 );
        REQUIRE( repn.quadratic_coefs.size() == 0 );
        }
        #ifdef DEBUG
        REQUIRE( coek::env.check_memory() == true );
        #endif
    }
    WHEN( "complex quadratic 1b" ) {
        {
        coek::Model m;
        coek::Variable v = m.getVariable(0, 1, 0, "v");
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression e = 3*w*(4 + 5*v + w);
        coek::MutableNLPExpr repn;
        repn.collect_terms(e);

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
        #ifdef DEBUG
        REQUIRE( coek::env.check_memory() == true );
        #endif
    }
    WHEN( "complex quadratic 2" ) {
        {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression e = 3*(w*w + 2);
        coek::MutableNLPExpr repn;
        repn.collect_terms(e);

        static std::list<std::string> constval = { "6.000" };
        static std::list<std::string> qcoefval = { "3.000" };
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 0 );
        REQUIRE( repn.quadratic_coefs.size() == 1 );
        REQUIRE( repn.quadratic_coefs[0].to_list() == qcoefval );
        REQUIRE( repn.quadratic_lvars[0] == w.repn );
        REQUIRE( repn.quadratic_rvars[0] == w.repn );
        }
        #ifdef DEBUG
        REQUIRE( coek::env.check_memory() == true );
        #endif
    }
    WHEN( "complex quadratic 3" ) {
        {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression e = (w*w + 2)*3;
        coek::MutableNLPExpr repn;
        repn.collect_terms(e);

        static std::list<std::string> constval = { "6.000" };
        static std::list<std::string> qcoefval = { "3.000" };
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 0 );
        REQUIRE( repn.quadratic_coefs.size() == 1 );
        REQUIRE( repn.quadratic_coefs[0].to_list() == qcoefval );
        REQUIRE( repn.quadratic_lvars[0] == w.repn );
        REQUIRE( repn.quadratic_rvars[0] == w.repn );
        }
        #ifdef DEBUG
        REQUIRE( coek::env.check_memory() == true );
        #endif
    }
  }

  SECTION( "divide" ) {
    WHEN( "lhs parameter - zero" ) {
        {
        coek::Model m;
        coek::Parameter p(0, "p");
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression e = p/w;
        coek::MutableNLPExpr repn;
        repn.collect_terms(e);

        static std::list<std::string> constval = { "0.000" };
        static std::list<std::string> nonlinear = { "[", "/", "p", "w", "]" };
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 0 );
        REQUIRE( repn.quadratic_coefs.size() == 0 );
        REQUIRE( repn.nonlinear.to_list() == nonlinear );
        }
        #ifdef DEBUG
        REQUIRE( coek::env.check_memory() == true );
        #endif
    }
    WHEN( "rhs parameter - zero" ) {
        {
        coek::Model m;
        coek::Parameter p(0, "p");
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression e = w/p;
        coek::MutableNLPExpr repn;
        repn.collect_terms(e);

        static std::list<std::string> constval = { "0.000" };
        static std::list<std::string> lcoef0 = { "[", "/", "1.000", "p", "]" };
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 1 );
        REQUIRE( repn.linear_coefs[0].to_list() == lcoef0 );
        REQUIRE( repn.quadratic_coefs.size() == 0 );
        REQUIRE( repn.nonlinear.to_list() == constval );
        }
        #ifdef DEBUG
        REQUIRE( coek::env.check_memory() == true );
        #endif
    }
    WHEN( "lhs constant - zero" ) {
        {
        coek::Model m;
        coek::Expression p;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression e = p/w;
        coek::MutableNLPExpr repn;
        repn.collect_terms(e);

        static std::list<std::string> constval = { "0.000" };
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 0 );
        REQUIRE( repn.quadratic_coefs.size() == 0 );
        REQUIRE( repn.nonlinear.to_list() == constval );
        }
        #ifdef DEBUG
        REQUIRE( coek::env.check_memory() == true );
        #endif
    }
    WHEN( "lhs constant - zero AND rhs constant" ) {
        {
        coek::Model m;
        coek::Expression p;
        coek::Parameter w(1,"w");
        coek::Expression e = p/(w+1);
        coek::MutableNLPExpr repn;
        repn.collect_terms(e);

        static std::list<std::string> constval = { "0.000" };
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 0 );
        REQUIRE( repn.quadratic_coefs.size() == 0 );
        REQUIRE( repn.nonlinear.to_list() == constval );
        }
        #ifdef DEBUG
        REQUIRE( coek::env.check_memory() == true );
        #endif
    }
    WHEN( "rhs constant - zero" ) {
        {
        coek::Model m;
        coek::Expression p;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression e = w/p;
        coek::MutableNLPExpr repn;
        REQUIRE_THROWS_WITH(repn.collect_terms(e),
            "Division by zero error.");
        }
        #ifdef DEBUG
        REQUIRE( coek::env.check_memory() == true );
        #endif
    }
    WHEN( "rhs polynomial" ) {
        {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression e = w/(1+w);
        coek::MutableNLPExpr repn;
        repn.collect_terms(e);

        static std::list<std::string> constval = { "0.000" };
        static std::list<std::string> nonlinear = { "[", "/", "w", "[", "+", "1.000", "w", "]", "]" };
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 0 );
        REQUIRE( repn.quadratic_coefs.size() == 0 );
        REQUIRE( repn.nonlinear.to_list() == nonlinear );
        }
        #ifdef DEBUG
        REQUIRE( coek::env.check_memory() == true );
        #endif
    }
    WHEN( "rhs nonzero" ) {
        {
        coek::Model m;
        coek::Parameter p(2.0, "p");
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression e = (w*w+w+1)/p;
        coek::MutableNLPExpr repn;
        repn.collect_terms(e);

        static std::list<std::string> constval = { "[", "/", "1.000", "p", "]" };
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 1 );
        REQUIRE( repn.linear_coefs[0].to_list() == constval );
        REQUIRE( repn.quadratic_coefs.size() == 1 );
        REQUIRE( repn.quadratic_coefs[0].to_list() == constval );
        }
        #ifdef DEBUG
        REQUIRE( coek::env.check_memory() == true );
        #endif
    }
  }

  SECTION( "constriaints" ) {
    WHEN( "inequality" ) {
        {
        coek::Model m;
        coek::Parameter p(0, "p");
        coek::Variable w = m.getVariable(0,1,3,"w");
        coek::Constraint e = p*w + 1 <= 2;
        coek::MutableNLPExpr repn;
        repn.collect_terms(e);

        static std::list<std::string> constval = { "1.000" };
        static std::list<std::string> coefval = { "p" };
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 1 );
        REQUIRE( repn.linear_coefs[0].to_list() == coefval );
        REQUIRE( repn.linear_vars[0] == w.repn );
        REQUIRE( repn.quadratic_coefs.size() == 0 );
        }
        #ifdef DEBUG
        REQUIRE( coek::env.check_memory() == true );
        #endif
    }
    WHEN( "equality" ) {
        {
        coek::Model m;
        coek::Parameter p(0, "p");
        coek::Variable w = m.getVariable(0,1,3,"w");
        coek::Constraint e = p*w -1 == 2;
        coek::MutableNLPExpr repn;
        repn.collect_terms(e);

        static std::list<std::string> constval = { "-1.000" };
        static std::list<std::string> coefval = { "p" };
        REQUIRE( repn.constval.to_list() == constval );
        REQUIRE( repn.linear_coefs.size() == 1 );
        REQUIRE( repn.linear_coefs[0].to_list() == coefval );
        REQUIRE( repn.linear_vars[0] == w.repn );
        REQUIRE( repn.quadratic_coefs.size() == 0 );
        }
        #ifdef DEBUG
        REQUIRE( coek::env.check_memory() == true );
        #endif
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

}


TEST_CASE( "mutable_values", "[smoke]" ) {

std::unordered_set<coek::VariableTerm*> fixed_vars;
std::unordered_set<coek::ParameterTerm*> params;

  SECTION( "constant" ) {
    coek::Expression e(3);
    mutable_values(e.repn, fixed_vars, params);

    static std::unordered_set<coek::VariableTerm*> vbaseline {};
    static std::unordered_set<coek::ParameterTerm*> pbaseline {};
    REQUIRE( fixed_vars == vbaseline );
    REQUIRE( params == pbaseline );
  }

  SECTION( "param" ) {
    coek::Parameter p(3);
    coek::Expression e = p;
    mutable_values(e.repn, fixed_vars, params);

    static std::unordered_set<coek::VariableTerm*> vbaseline {};
    static std::unordered_set<coek::ParameterTerm*> pbaseline { p.repn };
    REQUIRE( fixed_vars == vbaseline );
    REQUIRE( params == pbaseline );
  }

  SECTION( "var" ) {
    WHEN( "fixed" ) {
        coek::Variable v;
        v.set_fixed(true);
        coek::Expression e = v;
        mutable_values(e.repn, fixed_vars, params);

        static std::unordered_set<coek::VariableTerm*> vbaseline { v.repn };
        static std::unordered_set<coek::ParameterTerm*> pbaseline {};
        REQUIRE( fixed_vars == vbaseline );
        REQUIRE( params == pbaseline );
    }

    WHEN( "same" ) {
        coek::Variable v;
        coek::Expression e = v;
        mutable_values(e.repn, fixed_vars, params);

        static std::unordered_set<coek::VariableTerm*> vbaseline {};
        static std::unordered_set<coek::ParameterTerm*> pbaseline {};
        REQUIRE( fixed_vars == vbaseline );
        REQUIRE( params == pbaseline );
    }
  }

  SECTION( "monomial" ) {
    WHEN( "other" ) {
        coek::Model m;
        coek::Variable w = m.getVariable(0,1,0,"w");
        coek::Expression e = 2*w;
        mutable_values(e.repn, fixed_vars, params);

        static std::unordered_set<coek::VariableTerm*> vbaseline {};
        static std::unordered_set<coek::ParameterTerm*> pbaseline {};
        REQUIRE( fixed_vars == vbaseline );
        REQUIRE( params == pbaseline );
    }

    WHEN( "fixed" ) {
        coek::Model m;
        coek::Variable v = m.getVariable(0,1,0,"v");
        v.set_fixed(true);
        coek::Expression e = 2*v;
        mutable_values(e.repn, fixed_vars, params);

        static std::unordered_set<coek::VariableTerm*> vbaseline { v.repn };
        static std::unordered_set<coek::ParameterTerm*> pbaseline {};
        REQUIRE( fixed_vars == vbaseline );
        REQUIRE( params == pbaseline );
    }
  }

  SECTION( "plus" ) {
    WHEN( "linear" ) {
        coek::Variable v(0, 1, 0, "v");
        v.set_fixed(true);
        coek::Expression e = 2*(v+v)+v;
        mutable_values(e.repn, fixed_vars, params);

        static std::unordered_set<coek::VariableTerm*> vbaseline { v.repn };
        static std::unordered_set<coek::ParameterTerm*> pbaseline {};
        REQUIRE( fixed_vars == vbaseline );
        REQUIRE( params == pbaseline );
    }
    WHEN( "simple" ) {
        coek::Parameter p(0);
        coek::Variable v(0, 1, 0, "v");
        v.set_fixed(true);
        coek::Expression e = 3*p+2*v;
        mutable_values(e.repn, fixed_vars, params);

        static std::unordered_set<coek::VariableTerm*> vbaseline { v.repn };
        static std::unordered_set<coek::ParameterTerm*> pbaseline { p.repn };
        REQUIRE( fixed_vars == vbaseline );
        REQUIRE( params == pbaseline );
    }
  }

  SECTION( "negate" ) {
    WHEN( "linear" ) {
        coek::Parameter p(0);
        coek::Variable v(0, 1, 0, "v");
        v.set_fixed(true);
        coek::Expression e = -(v+1);
        mutable_values(e.repn, fixed_vars, params);

        static std::unordered_set<coek::VariableTerm*> vbaseline { v.repn };
        static std::unordered_set<coek::ParameterTerm*> pbaseline { };
        REQUIRE( fixed_vars == vbaseline );
        REQUIRE( params == pbaseline );
    }
  }

  SECTION( "times" ) {
    WHEN( "lhs constant" ) {
        coek::Model m;
        coek::Parameter p(2,"p");
        coek::Variable v = m.getVariable(0, 1, 0, "v");
        v.set_fixed(true);
        coek::Expression e = p*v;
        mutable_values(e.repn, fixed_vars, params);

        static std::unordered_set<coek::VariableTerm*> vbaseline { v.repn };
        static std::unordered_set<coek::ParameterTerm*> pbaseline { p.repn };
        REQUIRE( fixed_vars == vbaseline );
        REQUIRE( params == pbaseline );
    }
    WHEN( "simple quadratic LHS" ) {
        coek::Model m;
        coek::Variable v = m.getVariable(0, 1, 0, "v");
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        v.set_fixed(true);
        coek::Expression e = v*w;
        mutable_values(e.repn, fixed_vars, params);

        static std::unordered_set<coek::VariableTerm*> vbaseline { v.repn };
        static std::unordered_set<coek::ParameterTerm*> pbaseline { };
        REQUIRE( fixed_vars == vbaseline );
        REQUIRE( params == pbaseline );
    }
  }

  SECTION( "divide" ) {
    WHEN( "lhs zero" ) {
        coek::Model m;
        coek::Parameter p(0,"p");
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        w.set_fixed(true);
        coek::Expression e = p/w;
        mutable_values(e.repn, fixed_vars, params);

        static std::unordered_set<coek::VariableTerm*> vbaseline { w.repn };
        static std::unordered_set<coek::ParameterTerm*> pbaseline { p.repn };
        REQUIRE( fixed_vars == vbaseline );
        REQUIRE( params == pbaseline );
    }
    WHEN( "rhs nonzero" ) {
        coek::Model m;
        coek::Parameter p(2.0,"p");
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        w.set_fixed(true);
        coek::Expression e = w/p;
        mutable_values(e.repn, fixed_vars, params);

        static std::unordered_set<coek::VariableTerm*> vbaseline { w.repn };
        static std::unordered_set<coek::ParameterTerm*> pbaseline { p.repn };
        REQUIRE( fixed_vars == vbaseline );
        REQUIRE( params == pbaseline );
    }
    WHEN( "rhs polynomial" ) {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        w.set_fixed(true);
        coek::Expression e = w/(1+w);
        mutable_values(e.repn, fixed_vars, params);

        static std::unordered_set<coek::VariableTerm*> vbaseline { w.repn };
        static std::unordered_set<coek::ParameterTerm*> pbaseline { };
        REQUIRE( fixed_vars == vbaseline );
        REQUIRE( params == pbaseline );
    }
  }

  SECTION( "coverage" ) {
    WHEN( "variable partial plus monomial - 1" ) {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Variable v = m.getVariable(0, 1, 0, "v");
        coek::Expression e = w*v + v*(2*w+1);
        v.set_fixed(true);
        w.set_fixed(true);
        mutable_values(e.repn, fixed_vars, params);

        static std::unordered_set<coek::VariableTerm*> vbaseline { v.repn, w.repn };
        static std::unordered_set<coek::ParameterTerm*> pbaseline { };
        REQUIRE( fixed_vars == vbaseline );
        REQUIRE( params == pbaseline );
    }
    WHEN( "variable partial plus monomial - 2" ) {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Variable v = m.getVariable(0, 1, 0, "v");
        v.set_fixed(true);
        w.set_fixed(true);
        coek::Expression e = v*(2*w+1);
        mutable_values(e.repn, fixed_vars, params);

        static std::unordered_set<coek::VariableTerm*> vbaseline { v.repn, w.repn };
        static std::unordered_set<coek::ParameterTerm*> pbaseline { };
        REQUIRE( fixed_vars == vbaseline );
        REQUIRE( params == pbaseline );
    }
    WHEN( "constant partial plus monomial" ) {
        coek::Model m;
        coek::Variable v = m.getVariable(0, 1, 0, "v");
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Expression f = 3*w + 2*w;
        w.set_fixed(true);
        coek::Expression e = v*(2*w+1);
        mutable_values(e.repn, fixed_vars, params);

        static std::unordered_set<coek::VariableTerm*> vbaseline { w.repn };
        static std::unordered_set<coek::ParameterTerm*> pbaseline { };
        REQUIRE( fixed_vars == vbaseline );
        REQUIRE( params == pbaseline );
    }
    WHEN( "negative monomial" ) {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        w.set_fixed(true);
        coek::Expression e = -(-w) + (-(-w));
        mutable_values(e.repn, fixed_vars, params);

        static std::unordered_set<coek::VariableTerm*> vbaseline { w.repn };
        static std::unordered_set<coek::ParameterTerm*> pbaseline { };
        REQUIRE( fixed_vars == vbaseline );
        REQUIRE( params == pbaseline );
    }
  }

  SECTION( "intrinsic funcs" ) {
    MV_INTRINSIC_TEST1(abs)
    MV_INTRINSIC_TEST1(ceil)
    MV_INTRINSIC_TEST1(floor)
    MV_INTRINSIC_TEST1(exp)
    MV_INTRINSIC_TEST1(log)
    MV_INTRINSIC_TEST1(log10)
    MV_INTRINSIC_TEST1(sqrt)
    MV_INTRINSIC_TEST1(sin)
    MV_INTRINSIC_TEST1(cos)
    MV_INTRINSIC_TEST1(tan)
    MV_INTRINSIC_TEST1(sinh)
    MV_INTRINSIC_TEST1(cosh)
    MV_INTRINSIC_TEST1(tanh)
    MV_INTRINSIC_TEST1(asin)
    MV_INTRINSIC_TEST1(acos)
    MV_INTRINSIC_TEST1(atan)
    MV_INTRINSIC_TEST1(asinh)
    MV_INTRINSIC_TEST1(acosh)
    MV_INTRINSIC_TEST1(atanh)
    MV_INTRINSIC_TEST2(pow)
  }

  SECTION( "constraint" ) {
    WHEN( "inequality" ) {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Variable v = m.getVariable(0, 1, 0, "v");
        coek::Constraint e = w*v + v*(2*w+1) <= 0;
        v.set_fixed(true);
        w.set_fixed(true);
        mutable_values(e.repn, fixed_vars, params);

        static std::unordered_set<coek::VariableTerm*> vbaseline { v.repn, w.repn };
        static std::unordered_set<coek::ParameterTerm*> pbaseline { };
        REQUIRE( fixed_vars == vbaseline );
        REQUIRE( params == pbaseline );
    }
    WHEN( "equality" ) {
        coek::Model m;
        coek::Variable w = m.getVariable(0, 1, 0, "w");
        coek::Variable v = m.getVariable(0, 1, 0, "v");
        coek::Constraint e = w*v + v*(2*w+1) == 0;
        v.set_fixed(true);
        w.set_fixed(true);
        mutable_values(e.repn, fixed_vars, params);

        static std::unordered_set<coek::VariableTerm*> vbaseline { v.repn, w.repn };
        static std::unordered_set<coek::ParameterTerm*> pbaseline { };
        REQUIRE( fixed_vars == vbaseline );
        REQUIRE( params == pbaseline );
    }
  }  

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}


