
#include <iostream>
#include <sstream>

#include "catch2/catch.hpp"
#include "coek/ast/base_terms.hpp"
#include "coek/ast/constraint_terms.hpp"
#include "coek/ast/value_terms.hpp"
#include "coek/ast/visitor_fns.hpp"
#include "coek/coek.hpp"

#define WRITER_INTRINSIC_TEST1(FN)                               \
    WHEN(#FN)                                                    \
    {                                                            \
        auto v = coek::variable("v").lower(0).upper(1).value(0); \
        coek::Expression e = FN(v + 1);                          \
        std::stringstream sstr;                                  \
        sstr << e;                                               \
        REQUIRE(sstr.str() == #FN "(v + 1)");                    \
    }

#define WRITER_INTRINSIC_TEST2(FN)                               \
    WHEN(#FN)                                                    \
    {                                                            \
        auto v = coek::variable("v").lower(0).upper(1).value(0); \
        coek::Expression e = FN(v + 1, 2 * v);                   \
        std::stringstream sstr;                                  \
        sstr << e;                                               \
        REQUIRE(sstr.str() == #FN "(v + 1, 2*v)");               \
    }

#define REPN_INTRINSIC_TEST1(FN)                                                               \
    WHEN(#FN " 1")                                                                             \
    {                                                                                          \
        coek::Model m;                                                                         \
        auto v = coek::variable("v").lower(0).upper(1).value(0);                               \
        coek::Expression e = FN(v + 1);                                                        \
        coek::QuadraticExpr repn;                                                              \
        REQUIRE_THROWS_WITH(repn.collect_terms(e),                                             \
                            Catch::Matchers::StartsWith(                                       \
                                "Nonlinear expressions are not supported for QuadraticExpr")); \
    }                                                                                          \
    WHEN(#FN " 2")                                                                             \
    {                                                                                          \
        coek::Model m;                                                                         \
        auto v = coek::variable("v").lower(0).upper(1).value(0);                               \
        v.fixed(true);                                                                         \
        coek::Expression e = FN(v + 1);                                                        \
        coek::QuadraticExpr repn;                                                              \
        repn.collect_terms(e);                                                                 \
        REQUIRE(repn.linear_coefs.size() + repn.quadratic_coefs.size() == 0);                  \
    }

#define REPN_INTRINSIC_TEST2(FN)                                                               \
    WHEN(#FN " 1")                                                                             \
    {                                                                                          \
        coek::Model m;                                                                         \
        auto v = coek::variable("v").lower(0).upper(1).value(0);                               \
        coek::Expression e = FN(v + 1, v - 1);                                                 \
        coek::QuadraticExpr repn;                                                              \
        REQUIRE_THROWS_WITH(repn.collect_terms(e),                                             \
                            Catch::Matchers::StartsWith(                                       \
                                "Nonlinear expressions are not supported for QuadraticExpr")); \
    }                                                                                          \
    WHEN(#FN " 2")                                                                             \
    {                                                                                          \
        coek::Model m;                                                                         \
        auto v = coek::variable("v").lower(0).upper(1).value(0).fixed(true);                   \
        coek::Expression e = FN(v + 1, v - 1);                                                 \
        coek::QuadraticExpr repn;                                                              \
        repn.collect_terms(e);                                                                 \
        REQUIRE(repn.linear_coefs.size() + repn.quadratic_coefs.size() == 0);                  \
    }

#define NLP_INTRINSIC_TEST1(FN, MEMCHECK)                                         \
    WHEN(#FN " 1")                                                                \
    {                                                                             \
        {                                                                         \
            coek::Model m;                                                        \
            auto v = m.add_variable("v").lower(0).upper(1).value(0);              \
            coek::Expression e = FN(v + 1);                                       \
            coek::MutableNLPExpr repn;                                            \
            static std::list<std::string> constval = {std::to_string(0.0)};       \
            static std::list<std::string> nonlinear                               \
                = {"[", #FN, "[", "+", "v", std::to_string(1.0), "]", "]"};       \
            REQUIRE(repn.constval->to_list() == constval);                        \
            REQUIRE(repn.linear_coefs.size() + repn.quadratic_coefs.size() == 0); \
        }                                                                         \
        MEMCHECK;                                                                 \
    }                                                                             \
    WHEN(#FN " 2")                                                                \
    {                                                                             \
        {                                                                         \
            coek::Model m;                                                        \
            auto v = m.add_variable("v").lower(0).upper(1).value(0).fixed(true);  \
            coek::Expression e = FN(v + 1);                                       \
            coek::MutableNLPExpr repn;                                            \
            repn.collect_terms(e);                                                \
            static std::list<std::string> nonlinear = {std::to_string(0.0)};      \
            REQUIRE(repn.linear_coefs.size() + repn.quadratic_coefs.size() == 0); \
            REQUIRE(repn.nonlinear->to_list() == nonlinear);                      \
        }                                                                         \
        MEMCHECK;                                                                 \
    }

#define NLP_INTRINSIC_TEST2(FN, MEMCHECK)                                         \
    WHEN(#FN " 1")                                                                \
    {                                                                             \
        {                                                                         \
            coek::Model m;                                                        \
            auto v = m.add_variable("v").lower(0).upper(1).value(0);              \
            coek::Expression e = FN(v + 1, v);                                    \
            coek::MutableNLPExpr repn;                                            \
            static std::list<std::string> constval = {std::to_string(0.0)};       \
            static std::list<std::string> nonlinear                               \
                = {"[", #FN, "[", "+", "v", std::to_string(1.0), "]", "v", "]"};  \
            REQUIRE(repn.constval->to_list() == constval);                        \
            REQUIRE(repn.linear_coefs.size() + repn.quadratic_coefs.size() == 0); \
        }                                                                         \
        MEMCHECK;                                                                 \
    }                                                                             \
    WHEN(#FN " 2")                                                                \
    {                                                                             \
        {                                                                         \
            coek::Model m;                                                        \
            auto v = m.add_variable("v").lower(0).upper(1).value(0).fixed(true);  \
            coek::Expression e = FN(v + 1, v);                                    \
            coek::MutableNLPExpr repn;                                            \
            repn.collect_terms(e);                                                \
            static std::list<std::string> nonlinear = {std::to_string(0.0)};      \
            REQUIRE(repn.linear_coefs.size() + repn.quadratic_coefs.size() == 0); \
            REQUIRE(repn.nonlinear->to_list() == nonlinear);                      \
        }                                                                         \
        MEMCHECK;                                                                 \
    }

#define MV_INTRINSIC_TEST1(FN)                                                            \
    WHEN(#FN)                                                                             \
    {                                                                                     \
        coek::Model m;                                                                    \
        auto v = m.add_variable("v").lower(0).upper(1).value(0).fixed(true);              \
        coek::Expression e = FN(v + 1);                                                   \
        mutable_values(e.repn, fixed_vars, params);                                       \
        static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{v.repn}; \
        static std::unordered_set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};      \
        REQUIRE(fixed_vars == vbaseline);                                                 \
        REQUIRE(params == pbaseline);                                                     \
    }

#define MV_INTRINSIC_TEST2(FN)                                                            \
    WHEN(#FN)                                                                             \
    {                                                                                     \
        coek::Model m;                                                                    \
        auto v = m.add_variable("v").lower(0).upper(1).value(0).fixed(true);              \
        coek::Expression e = FN(v + 1, v);                                                \
        mutable_values(e.repn, fixed_vars, params);                                       \
        static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{v.repn}; \
        static std::unordered_set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};      \
        REQUIRE(fixed_vars == vbaseline);                                                 \
        REQUIRE(params == pbaseline);                                                     \
    }

#ifdef WITH_AST_ENV
#    define ENV_MEMCHECK REQUIRE(coek::env.check_memory() == true)
#else
#    define ENV_MEMCHECK
#endif

TEST_CASE("expr_writer", "[smoke]")
{
    SECTION("constant")
    {
        coek::Expression e(3);
        std::stringstream sstr;
        sstr << e;
        REQUIRE(sstr.str() == "3");
    }

    SECTION("param")
    {
        WHEN("unnamed ")
        {
            auto q = coek::parameter().value(3);
            std::stringstream sstr;
            sstr << q;
            REQUIRE(sstr.str() == std::to_string(3.0));
        }
        WHEN("named ")
        {
            auto q = coek::parameter("q").value(3);
            std::stringstream sstr;
            sstr << q;
            REQUIRE(sstr.str() == "q");
        }
    }

    SECTION("var")
    {
        WHEN("unnamed ")
        {
            auto v = coek::variable("");
            std::stringstream sstr;
            sstr << v;
            REQUIRE(sstr.str()[0] == 'X');
        }
        WHEN("named ")
        {
            auto v = coek::variable("v").lower(0).upper(1).value(0);
            std::stringstream sstr;
            sstr << v;
            REQUIRE(sstr.str() == "v");
        }
    }

    SECTION("monomial")
    {
        WHEN("unweighted ")
        {
            auto v = coek::variable("v");
            coek::Expression e = 1 * v;
            std::stringstream sstr;
            sstr << e;
            REQUIRE(sstr.str() == "v");
        }
        WHEN("weighted ")
        {
            auto v = coek::variable("v");
            coek::Expression e = 2 * v;
            std::stringstream sstr;
            sstr << e;
            REQUIRE(sstr.str() == "2*v");
        }
    }

    SECTION("constraint")
    {
        WHEN("lt ")
        {
            auto v = coek::variable("v");
            auto e = v < 1;
            std::stringstream sstr;
            sstr << e;
            REQUIRE(sstr.str() == "v < 1");
        }
        WHEN("leq ")
        {
            auto v = coek::variable("v");
            auto e = v <= 1;
            std::stringstream sstr;
            sstr << e;
            REQUIRE(sstr.str() == "v <= 1");
        }
        WHEN("eq ")
        {
            auto v = coek::variable("v");
            auto e = v == 1;
            std::stringstream sstr;
            sstr << e;
            REQUIRE(sstr.str() == "v == 1");
        }
    }

    SECTION("negate")
    {
        auto v = coek::variable("v");
        coek::Expression e = -(v + 1);
        std::stringstream sstr;
        sstr << e;
        REQUIRE(sstr.str() == "- (v + 1)");
    }

    SECTION("plus")
    {
        WHEN("2 terms")
        {
            auto v = coek::variable("v").lower(0).upper(1).value(0);
            coek::Expression e = v + 1;
            std::stringstream sstr;
            sstr << e;
            REQUIRE(sstr.str() == "v + 1");
        }
        WHEN("combine sums")
        {
            auto v = coek::variable("v").lower(0).upper(1).value(0);
            coek::Expression e = v + 1 + (-1 - v);
            std::stringstream sstr;
            sstr << e;
            REQUIRE(sstr.str() == "v + 1 + -1 + -1*v");
        }
        WHEN("3 terms sums")
        {
            auto v = coek::variable("v").lower(0).upper(1).value(0);
            coek::Expression e = v + 1 + 3 * (-1 - v);
            std::stringstream sstr;
            sstr << e;
            REQUIRE(sstr.str() == "v + 1 + (3)*(-1 + -1*v)");
        }
    }

    SECTION("times")
    {
        auto v = coek::variable("v").lower(0).upper(1).value(0);
        auto w = coek::variable("w").lower(0).upper(1).value(0);
        coek::Expression e = v * (w + 1);
        std::stringstream sstr;
        sstr << e;
        REQUIRE(sstr.str() == "(v)*(w + 1)");
    }

    SECTION("divide")
    {
        auto v = coek::variable("v").lower(0).upper(1).value(0);
        auto w = coek::variable("w").lower(0).upper(1).value(0);
        coek::Expression e = v / (w + 1);
        std::stringstream sstr;
        sstr << e;
        REQUIRE(sstr.str() == "(v)/(w + 1)");
    }

    SECTION("intrinsic funcs"){

        // clang-format off

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
        WRITER_INTRINSIC_TEST2(pow)}

    // clang-format on

    SECTION("affine_expression1")
    {
        std::vector<coek::Variable> v(4);
        std::vector<double> w(4);
        for (unsigned int i = 0; i < 4; i++) {
            v[i] = coek::variable("v[" + std::to_string(i) + "]").lower(0).upper(1).value(0);
            w[i] = i + 1;
        }
        coek::Expression e = affine_expression(w, v, 5.0);
        std::stringstream sstr;
        sstr << e;
        REQUIRE(sstr.str() == "5 + v[0] + 2*v[1] + 3*v[2] + 4*v[3]");
    }

    SECTION("affine_expression")
    {
        std::vector<coek::Variable> v(4);
        for (unsigned int i = 0; i < 4; i++) {
            v[i] = coek::variable("v[" + std::to_string(i) + "]").lower(0).upper(1).value(0);
        }
        coek::Expression e = affine_expression(v, 5.0);
        std::stringstream sstr;
        sstr << e;
        REQUIRE(sstr.str() == "5 + v[0] + v[1] + v[2] + v[3]");
    }
}

TEST_CASE("expr_to_QuadraticExpr", "[smoke]")
{
    SECTION("constant")
    {
        coek::Expression e(3);
        coek::QuadraticExpr repn;
        repn.collect_terms(e);

        REQUIRE(repn.constval == 3);
        REQUIRE(repn.linear_coefs.size() == 0);
        REQUIRE(repn.quadratic_coefs.size() == 0);
    }

    SECTION("param")
    {
        auto p = coek::parameter().value(3);
        coek::Expression e = p;
        coek::QuadraticExpr repn;
        repn.collect_terms(e);

        REQUIRE(repn.constval == 3);
        REQUIRE(repn.linear_coefs.size() == 0);
        REQUIRE(repn.quadratic_coefs.size() == 0);
    }

    SECTION("var")
    {
        WHEN("unfixed")
        {
            coek::Model m;
            auto v = m.add_variable("v").lower(0).upper(1).value(0);
            coek::Expression e = v;
            coek::QuadraticExpr repn;
            repn.collect_terms(e);

            REQUIRE(repn.constval == 0);
            REQUIRE(repn.linear_coefs.size() == 1);
            REQUIRE(repn.linear_coefs[0] == 1);
            REQUIRE(repn.linear_vars[0] == e.repn);
            REQUIRE(repn.quadratic_coefs.size() == 0);
        }
        WHEN("fixed")
        {
            coek::Model m;
            auto v = m.add_variable("v").lower(0).upper(1).value(3);
            v.fixed(true);
            coek::Expression e = v;
            coek::QuadraticExpr repn;
            repn.collect_terms(e);

            REQUIRE(repn.constval == 3);
            REQUIRE(repn.linear_coefs.size() == 0);
            REQUIRE(repn.quadratic_coefs.size() == 0);
        }
    }

    SECTION("monomial")
    {
        WHEN("unfixed")
        {
            coek::Model m;
            auto v = m.add_variable("v").lower(0).upper(1).value(0);
            coek::Expression e = 2 * v;
            coek::QuadraticExpr repn;
            repn.collect_terms(e);

            REQUIRE(repn.constval == 0);
            REQUIRE(repn.linear_coefs.size() == 1);
            REQUIRE(repn.linear_coefs[0] == 2);
            auto tmp = std::dynamic_pointer_cast<coek::MonomialTerm>(e.repn);
            REQUIRE(repn.linear_vars[0]->index == tmp->var->index);
            REQUIRE(repn.quadratic_coefs.size() == 0);
        }
        WHEN("fixed")
        {
            coek::Model m;
            auto v = m.add_variable("v").lower(0).upper(1).value(3);
            v.fixed(true);
            coek::Expression e = 2 * v;
            coek::QuadraticExpr repn;
            repn.collect_terms(e);

            REQUIRE(repn.constval == 6);
            REQUIRE(repn.linear_coefs.size() == 0);
            REQUIRE(repn.quadratic_coefs.size() == 0);
        }
    }

    SECTION("times")
    {
        WHEN("lhs zero")
        {
            auto p = coek::parameter();
            auto w = coek::variable("w").lower(0).upper(1).value(0);
            coek::Expression e = p * w;
            coek::QuadraticExpr repn;
            repn.collect_terms(e);

            REQUIRE(repn.constval == 0);
            REQUIRE(repn.linear_coefs.size() == 0);
            REQUIRE(repn.quadratic_coefs.size() == 0);
        }
        WHEN("lhs constant")
        {
            coek::Model m;
            auto p = coek::parameter().value(2);
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression e = p * w;
            coek::QuadraticExpr repn;
            repn.collect_terms(e);

            REQUIRE(repn.constval == 0);
            REQUIRE(repn.linear_coefs.size() == 1);
            REQUIRE(repn.linear_coefs[0] == 2);
            REQUIRE(repn.linear_vars[0] == w.repn);
            REQUIRE(repn.quadratic_coefs.size() == 0);
        }
        WHEN("rhs zero")
        {
            coek::Model m;
            auto p = coek::parameter();
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression e = w * p;
            coek::QuadraticExpr repn;
            repn.collect_terms(e);

            REQUIRE(repn.constval == 0);
            REQUIRE(repn.linear_coefs.size() == 0);
            REQUIRE(repn.quadratic_coefs.size() == 0);
        }
        WHEN("rhs constant")
        {
            coek::Model m;
            auto p = coek::parameter().value(2);
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression e = (w + w * w) * p;
            coek::QuadraticExpr repn;
            repn.collect_terms(e);

            REQUIRE(repn.constval == 0);
            REQUIRE(repn.linear_coefs.size() == 1);
            REQUIRE(repn.linear_coefs[0] == 2);
            REQUIRE(repn.linear_vars[0] == w.repn);
            REQUIRE(repn.quadratic_coefs.size() == 1);
            REQUIRE(repn.quadratic_coefs[0] == 2);
            REQUIRE(repn.quadratic_lvars[0] == w.repn);
            REQUIRE(repn.quadratic_rvars[0] == w.repn);
        }
        WHEN("simple quadratic")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression e = w * w;
            coek::QuadraticExpr repn;
            repn.collect_terms(e);

            REQUIRE(repn.constval == 0);
            REQUIRE(repn.linear_coefs.size() == 0);
            REQUIRE(repn.quadratic_coefs.size() == 1);
            REQUIRE(repn.quadratic_coefs[0] == 1);
            REQUIRE(repn.quadratic_lvars[0] == w.repn);
            REQUIRE(repn.quadratic_rvars[0] == w.repn);
        }
        WHEN("complex quadratic 1")
        {
            coek::Model m;
            auto v = m.add_variable("v").lower(0).upper(1).value(0);
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression e = (2 + 3 * w + v) * (4 + 5 * v + w);
            coek::QuadraticExpr repn;
            repn.collect_terms(e);

            REQUIRE(repn.constval == 8);
            REQUIRE(repn.linear_coefs.size() == 4);
            REQUIRE(repn.linear_coefs[0] == 10);
            REQUIRE(repn.linear_coefs[1] == 2);
            REQUIRE(repn.linear_coefs[2] == 12);
            REQUIRE(repn.linear_coefs[3] == 4);
            REQUIRE(repn.quadratic_coefs.size() == 4);
            REQUIRE(repn.quadratic_coefs[0] == 15);
            REQUIRE(repn.quadratic_coefs[1] == 3);
            REQUIRE(repn.quadratic_coefs[2] == 5);
            REQUIRE(repn.quadratic_coefs[3] == 1);
        }
        WHEN("error 1")
        {
            coek::Model m;
            auto v = m.add_variable("v").lower(0).upper(1).value(0);
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression e = (w + v * v) * v;
            coek::QuadraticExpr repn;
            REQUIRE_THROWS_WITH(
                repn.collect_terms(e),
                "Non-quadratic expressions cannot be expressed in a QuadraticExpr object.");
        }
        WHEN("error 2")
        {
            coek::Model m;
            auto v = m.add_variable("v").lower(0).upper(1).value(0);
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression e = v * (w + v * v);
            coek::QuadraticExpr repn;
            REQUIRE_THROWS_WITH(
                repn.collect_terms(e),
                "Non-quadratic expressions cannot be expressed in a QuadraticExpr object.");
        }
    }

    SECTION("divide")
    {
        WHEN("lhs zero")
        {
            coek::Model m;
            auto p = coek::parameter();
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression e = p / w;
            coek::QuadraticExpr repn;
            repn.collect_terms(e);

            REQUIRE(repn.constval == 0);
            REQUIRE(repn.linear_coefs.size() == 0);
            REQUIRE(repn.quadratic_coefs.size() == 0);
        }
        WHEN("rhs zero")
        {
            coek::Model m;
            auto p = coek::parameter();
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression e = w / p;
            coek::QuadraticExpr repn;
            REQUIRE_THROWS_WITH(repn.collect_terms(e), "Division by zero error.");
        }
        WHEN("rhs polynomial")
        {
            coek::Model m;
            auto p = coek::parameter();
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression e = w / (1 + w);
            coek::QuadraticExpr repn;
            REQUIRE_THROWS_WITH(repn.collect_terms(e),
                                "Non-constant expressions cannot appear in the denominator of "
                                "quadratic expressions.");
        }
        WHEN("rhs nonzero")
        {
            coek::Model m;
            auto p = coek::parameter().value(2);
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression e = (w * w + w + 1) / p;
            coek::QuadraticExpr repn;
            repn.collect_terms(e);

            REQUIRE(repn.constval == 0.5);
            REQUIRE(repn.linear_coefs.size() == 1);
            REQUIRE(repn.linear_coefs[0] == 0.5);
            REQUIRE(repn.quadratic_coefs.size() == 1);
            REQUIRE(repn.quadratic_coefs[0] == 0.5);
        }
    }

    SECTION("intrinsic funcs")
    {
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

        WHEN("pow(x,0)")
        {
            coek::Model m;
            auto x = m.add_variable("x");
            auto p = coek::parameter();
            coek::Expression e = pow(x, p);
            coek::QuadraticExpr repn;
            repn.collect_terms(e);

            REQUIRE(repn.constval == 1);
            REQUIRE(repn.linear_coefs.size() == 0);
            REQUIRE(repn.quadratic_coefs.size() == 0);
        }

        WHEN("pow(x,1)")
        {
            coek::Model m;
            auto x = m.add_variable("x");
            auto p = coek::parameter().value(1);
            coek::Expression e = pow(x, p);
            coek::QuadraticExpr repn;
            repn.collect_terms(e);

            REQUIRE(repn.constval == 0);
            REQUIRE(repn.linear_coefs.size() == 1);
            REQUIRE(repn.linear_coefs[0] == 1.0);
            REQUIRE(repn.quadratic_coefs.size() == 0);
        }

        WHEN("pow(x,1)")
        {
            coek::Model m;
            auto x = m.add_variable("x");
            auto p = coek::parameter().value(2);
            coek::Expression e = pow(x * x, p);
            coek::QuadraticExpr repn;
            REQUIRE_THROWS(repn.collect_terms(e));
        }
    }

#ifdef WITH_AST_ENV
    REQUIRE(coek::env.check_memory() == true);
#endif
}

TEST_CASE("symbolic_diff", "[smoke]")
{
    SECTION("constant")
    {
        coek::Expression f(3);
        auto v = coek::variable();
        auto e = f.diff(v);
        static std::list<std::string> baseline = {std::to_string(0.0)};
        REQUIRE(e.to_list() == baseline);
    }

    SECTION("param")
    {
        auto p = coek::parameter().value(3);
        coek::Expression f = p;
        auto v = coek::variable();
        auto e = f.diff(v);
        static std::list<std::string> baseline = {std::to_string(0.0)};
        REQUIRE(e.to_list() == baseline);
    }

    SECTION("var")
    {
        WHEN("fixed")
        {
            auto v = coek::variable();
            v.fixed(true);
            coek::Expression f = v;
            auto e = f.diff(v);
            static std::list<std::string> baseline = {std::to_string(0.0)};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("same")
        {
            auto v = coek::variable();
            coek::Expression f = v;
            auto e = f.diff(v);
            static std::list<std::string> baseline = {std::to_string(1.0)};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("different")
        {
            auto v = coek::variable();
            auto w = coek::variable();
            coek::Expression f = w;
            auto e = f.diff(v);
            static std::list<std::string> baseline = {std::to_string(0.0)};
            REQUIRE(e.to_list() == baseline);
        }
    }

    SECTION("monomial")
    {
        WHEN("other")
        {
            coek::Model m;
            auto v = m.add_variable("v").lower(0).upper(1).value(0);
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = 2 * v;
            auto e = f.diff(w);
            static std::list<std::string> baseline = {std::to_string(0.0)};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("same")
        {
            coek::Model m;
            auto v = m.add_variable("v").lower(0).upper(1).value(0);
            coek::Expression f = 2 * v;
            auto e = f.diff(v);
            static std::list<std::string> baseline = {std::to_string(2.0)};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("fixed")
        {
            coek::Model m;
            auto v = m.add_variable("v").lower(0).upper(1).value(0);
            v.fixed(true);
            coek::Expression f = 2 * v;
            auto e = f.diff(v);
            static std::list<std::string> baseline = {std::to_string(0.0)};
            REQUIRE(e.to_list() == baseline);
        }
    }

    SECTION("plus")
    {
        WHEN("linear")
        {
            auto p = coek::parameter();
            auto v = coek::variable("v").lower(0).upper(1).value(0);
            coek::Expression f = 2 * (v + v) + v;
            auto e = f.diff(v);
            static std::list<std::string> baseline = {std::to_string(5.0)};
            REQUIRE(e.to_list() == baseline);
        }
        WHEN("simple")
        {
            auto p = coek::parameter();
            auto v = coek::variable("v").lower(0).upper(1).value(0);
            coek::Expression f = 3 * p + 2 * v;
            auto e = f.diff(v);
            static std::list<std::string> baseline = {std::to_string(2.0)};
            REQUIRE(e.to_list() == baseline);
        }
        WHEN("multiple")
        {
            auto p = coek::parameter();
            auto v = coek::variable("v").lower(0).upper(1).value(0);
            coek::Expression f = 7 * v + v;
            auto e = f.diff(v);
            static std::list<std::string> baseline = {std::to_string(8.000)};
            REQUIRE(e.to_list() == baseline);
        }
    }

    SECTION("negate")
    {
        WHEN("linear")
        {
            auto p = coek::parameter();
            auto v = coek::variable("v").lower(0).upper(1).value(0);
            coek::Expression f = -(v + 1);
            auto e = f.diff(v);
            static std::list<std::string> baseline = {std::to_string(-1.0)};
            REQUIRE(e.to_list() == baseline);
        }
    }

    SECTION("times")
    {
        WHEN("lhs zero")
        {
            coek::Expression p;
            auto v = coek::variable("v").lower(0).upper(1).value(0);
            coek::Expression f = p * v;
            auto e = f.diff(v);
            static std::list<std::string> baseline = {std::to_string(0.0)};
            REQUIRE(e.to_list() == baseline);
        }
        WHEN("lhs constant")
        {
            coek::Model m;
            auto p = coek::parameter("p").value(2);
            auto v = m.add_variable("v").lower(0).upper(1).value(0);
            coek::Expression f = p * v;
            auto e = f.diff(v);
            static std::list<std::string> baseline = {"p"};
            REQUIRE(e.to_list() == baseline);
        }
        WHEN("rhs zero")
        {
            coek::Model m;
            coek::Expression p;
            auto v = m.add_variable("v").lower(0).upper(1).value(0);
            coek::Expression f = v * p;
            auto e = f.diff(v);
            static std::list<std::string> baseline = {std::to_string(0.0)};
            REQUIRE(e.to_list() == baseline);
        }
        WHEN("rhs constant")
        {
            coek::Model m;
            auto p = coek::parameter("p").value(2);
            auto v = m.add_variable("v").lower(0).upper(1).value(0);
            coek::Expression f = v * p;
            auto e = f.diff(v);
            static std::list<std::string> baseline = {"p"};
            REQUIRE(e.to_list() == baseline);
        }
        WHEN("simple quadratic LHS")
        {
            coek::Model m;
            auto v = m.add_variable("v").lower(0).upper(1).value(0);
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = v * w;
            auto e = f.diff(v);
            static std::list<std::string> baseline = {"w"};
            REQUIRE(e.to_list() == baseline);
        }
        WHEN("simple quadratic RHS")
        {
            coek::Model m;
            auto v = m.add_variable("v").lower(0).upper(1).value(0);
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = v * w;
            auto e = f.diff(w);
            static std::list<std::string> baseline = {"v"};
            REQUIRE(e.to_list() == baseline);
        }
    }

    SECTION("divide")
    {
        WHEN("lhs zero")
        {
            coek::Model m;
            auto p = coek::parameter("p");
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = p / w;
            auto e = f.diff(w);
            static std::list<std::string> baseline = {
                "[", "/", "[", "*", std::to_string(-1.0), "p", "]", "[", "*", "w", "w", "]", "]"};
            REQUIRE(e.to_list() == baseline);
        }
        WHEN("rhs nonzero")
        {
            coek::Model m;
            auto p = coek::parameter("p").value(2);
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = w / p;
            auto e = f.diff(w);
            static std::list<std::string> baseline = {"[", "/", std::to_string(1.0), "p", "]"};
            REQUIRE(e.to_list() == baseline);
        }
        WHEN("rhs polynomial")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = w / (1 + w);
            auto e = f.diff(w);
            static std::list<std::string> baseline = {"[",
                                                      "+",
                                                      "[",
                                                      "/",
                                                      std::to_string(1.0),
                                                      "[",
                                                      "+",
                                                      std::to_string(1.0),
                                                      "w",
                                                      "]",
                                                      "]",
                                                      "[",
                                                      "/",
                                                      "[",
                                                      "*",
                                                      std::to_string(-1.0),
                                                      "w",
                                                      "]",
                                                      "[",
                                                      "*",
                                                      "[",
                                                      "+",
                                                      std::to_string(1.0),
                                                      "w",
                                                      "]",
                                                      "[",
                                                      "+",
                                                      std::to_string(1.0),
                                                      "w",
                                                      "]",
                                                      "]",
                                                      "]",
                                                      "]"};
            REQUIRE(e.to_list() == baseline);
        }
    }

    SECTION("coverage")
    {
        WHEN("variable partial plus monomial - 1")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            auto v = m.add_variable("v").lower(0).upper(1).value(0);
            coek::Expression f = w * v + v * (2 * w + 1);
            auto e = f.diff(w);
            static std::list<std::string> baseline
                = {"[", "+", "v", "[", "*", std::to_string(2.0), "v", "]", "]"};
            REQUIRE(e.to_list() == baseline);
        }
        WHEN("variable partial plus monomial - 2")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            auto v = m.add_variable("v").lower(0).upper(1).value(0);
            coek::Expression f = v * (2 * w + 1);
            auto e = f.diff(w);
            static std::list<std::string> baseline = {"[", "*", std::to_string(2.0), "v", "]"};
            REQUIRE(e.to_list() == baseline);
        }
        WHEN("constant partial plus monomial")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = 3 * w + 2 * w;
            auto e = f.diff(w);
            static std::list<std::string> baseline = {std::to_string(5.0)};
            REQUIRE(e.to_list() == baseline);
        }
        WHEN("negative monomial")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = -(-w) + (-(-w));
            auto e = f.diff(w);
            static std::list<std::string> baseline = {std::to_string(2.0)};
            REQUIRE(e.to_list() == baseline);
        }
    }

    SECTION("intrinsic funcs")
    {
        REPN_INTRINSIC_TEST1(abs)
        REPN_INTRINSIC_TEST1(ceil)
        REPN_INTRINSIC_TEST1(floor)
        WHEN("exp")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = exp(2 * w);
            auto e = f.diff(w);
            static std::list<std::string> baseline
                = {"[", "*", std::to_string(2.0), "[", "exp", "[", "*", "2", "w", "]", "]", "]"};
            REQUIRE(e.to_list() == baseline);
        }
        WHEN("log")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = log(2 * w);
            auto e = f.diff(w);
            static std::list<std::string> baseline = {"[", "*", std::to_string(2.0),
                                                      "[", "/", std::to_string(1.0),
                                                      "[", "*", "2",
                                                      "w", "]", "]",
                                                      "]"};
            REQUIRE(e.to_list() == baseline);
        }
        WHEN("log10")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = log10(2 * w);
            auto e = f.diff(w);
            static std::list<std::string> baseline = {"[", "*", std::to_string(2.0),
                                                      "[", "/", std::to_string(1.0),
                                                      "[", "*", std::to_string(2.302585),
                                                      "[", "*", "2",
                                                      "w", "]", "]",
                                                      "]", "]"};
            REQUIRE(e.to_list() == baseline);
        }
        WHEN("sqrt")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = sqrt(2 * w);
            auto e = f.diff(w);
            static std::list<std::string> baseline
                = {"[", "*", std::to_string(2.0),    "[", "pow", "[", "*", "2",
                   "w", "]", std::to_string(-0.500), "]", "]"};
            REQUIRE(e.to_list() == baseline);
        }
        WHEN("sin")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = sin(2 * w);
            auto e = f.diff(w);
            static std::list<std::string> baseline
                = {"[", "*", std::to_string(2.0), "[", "cos", "[", "*", "2", "w", "]", "]", "]"};
            REQUIRE(e.to_list() == baseline);
        }
        WHEN("cos")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = cos(2 * w);
            auto e = f.diff(w);
            static std::list<std::string> baseline = {
                "[", "*", std::to_string(2.0), "[", "-", "[", "sin", "[", "*", "2", "w", "]", "]",
                "]", "]"};
            REQUIRE(e.to_list() == baseline);
        }
        WHEN("tan")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = tan(2 * w);
            auto e = f.diff(w);
            static std::list<std::string> baseline = {"[",
                                                      "*",
                                                      std::to_string(2.0),
                                                      "[",
                                                      "/",
                                                      std::to_string(1.0),
                                                      "[",
                                                      "pow",
                                                      "[",
                                                      "cos",
                                                      "[",
                                                      "*",
                                                      "2",
                                                      "w",
                                                      "]",
                                                      "]",
                                                      std::to_string(2.0),
                                                      "]",
                                                      "]",
                                                      "]"};
            REQUIRE(e.to_list() == baseline);
        }
        WHEN("sinh")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = sinh(2 * w);
            auto e = f.diff(w);
            static std::list<std::string> baseline
                = {"[", "*", std::to_string(2.0), "[", "cosh", "[", "*", "2", "w", "]", "]", "]"};
            REQUIRE(e.to_list() == baseline);
        }
        WHEN("cosh")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = cosh(2 * w);
            auto e = f.diff(w);
            static std::list<std::string> baseline
                = {"[", "*", std::to_string(2.0), "[", "sinh", "[", "*", "2", "w", "]", "]", "]"};
            REQUIRE(e.to_list() == baseline);
        }
        WHEN("tanh")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = tanh(2 * w);
            auto e = f.diff(w);
            static std::list<std::string> baseline = {"[",
                                                      "*",
                                                      std::to_string(2.0),
                                                      "[",
                                                      "+",
                                                      std::to_string(1.0),
                                                      "[",
                                                      "*",
                                                      std::to_string(-1.0),
                                                      "[",
                                                      "pow",
                                                      "[",
                                                      "tan",
                                                      "[",
                                                      "*",
                                                      "2",
                                                      "w",
                                                      "]",
                                                      "]",
                                                      std::to_string(2.0),
                                                      "]",
                                                      "]",
                                                      "]",
                                                      "]"};
            REQUIRE(e.to_list() == baseline);
        }
        WHEN("asin")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = asin(2 * w);
            auto e = f.diff(w);
            static std::list<std::string> baseline = {"[",
                                                      "*",
                                                      std::to_string(2.0),
                                                      "[",
                                                      "/",
                                                      std::to_string(1.0),
                                                      "[",
                                                      "sqrt",
                                                      "[",
                                                      "+",
                                                      std::to_string(1.0),
                                                      "[",
                                                      "-",
                                                      "[",
                                                      "*",
                                                      "[",
                                                      "*",
                                                      "2",
                                                      "w",
                                                      "]",
                                                      "[",
                                                      "*",
                                                      "2",
                                                      "w",
                                                      "]",
                                                      "]",
                                                      "]",
                                                      "]",
                                                      "]",
                                                      "]",
                                                      "]"};
            REQUIRE(e.to_list() == baseline);
        }
        WHEN("acos")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = acos(2 * w);
            auto e = f.diff(w);
            static std::list<std::string> baseline = {"[",
                                                      "*",
                                                      std::to_string(2.0),
                                                      "[",
                                                      "-",
                                                      "[",
                                                      "/",
                                                      std::to_string(1.0),
                                                      "[",
                                                      "sqrt",
                                                      "[",
                                                      "+",
                                                      std::to_string(1.0),
                                                      "[",
                                                      "-",
                                                      "[",
                                                      "*",
                                                      "[",
                                                      "*",
                                                      "2",
                                                      "w",
                                                      "]",
                                                      "[",
                                                      "*",
                                                      "2",
                                                      "w",
                                                      "]",
                                                      "]",
                                                      "]",
                                                      "]",
                                                      "]",
                                                      "]",
                                                      "]",
                                                      "]"};
            REQUIRE(e.to_list() == baseline);
        }
        WHEN("atan")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = atan(2 * w);
            auto e = f.diff(w);
            static std::list<std::string> baseline = {"[", "*", std::to_string(2.0),
                                                      "[", "/", std::to_string(1.0),
                                                      "[", "+", std::to_string(1.0),
                                                      "[", "*", "[",
                                                      "*", "2", "w",
                                                      "]", "[", "*",
                                                      "2", "w", "]",
                                                      "]", "]", "]",
                                                      "]"};
            REQUIRE(e.to_list() == baseline);
        }
        WHEN("asinh")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = asinh(2 * w);
            auto e = f.diff(w);
            static std::list<std::string> baseline = {"[",
                                                      "*",
                                                      std::to_string(2.0),
                                                      "[",
                                                      "/",
                                                      std::to_string(1.0),
                                                      "[",
                                                      "sqrt",
                                                      "[",
                                                      "+",
                                                      std::to_string(1.0),
                                                      "[",
                                                      "*",
                                                      "[",
                                                      "*",
                                                      "2",
                                                      "w",
                                                      "]",
                                                      "[",
                                                      "*",
                                                      "2",
                                                      "w",
                                                      "]",
                                                      "]",
                                                      "]",
                                                      "]",
                                                      "]",
                                                      "]"};
            REQUIRE(e.to_list() == baseline);
        }
        WHEN("acosh")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = acosh(2 * w);
            auto e = f.diff(w);
            static std::list<std::string> baseline = {"[", "*",    std::to_string(2.0),
                                                      "[", "/",    std::to_string(1.0),
                                                      "[", "sqrt", "[",
                                                      "+", "[",    "*",
                                                      "[", "*",    "2",
                                                      "w", "]",    "[",
                                                      "*", "2",    "w",
                                                      "]", "]",    std::to_string(-1.0),
                                                      "]", "]",    "]",
                                                      "]"};
            REQUIRE(e.to_list() == baseline);
        }
        WHEN("atanh")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = atanh(2 * w);
            auto e = f.diff(w);
            static std::list<std::string> baseline = {"[",
                                                      "*",
                                                      std::to_string(2.0),
                                                      "[",
                                                      "/",
                                                      std::to_string(1.0),
                                                      "[",
                                                      "+",
                                                      "[",
                                                      "*",
                                                      "[",
                                                      "*",
                                                      "2",
                                                      "w",
                                                      "]",
                                                      "[",
                                                      "*",
                                                      "2",
                                                      "w",
                                                      "]",
                                                      "]",
                                                      std::to_string(-1.0),
                                                      "]",
                                                      "]",
                                                      "]"};
            REQUIRE(e.to_list() == baseline);
        }
        WHEN("pow - 1")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = pow(2 * w, 3);
            auto e = f.diff(w);
            static std::list<std::string> baseline = {"[",
                                                      "*",
                                                      std::to_string(2.0),
                                                      "[",
                                                      "*",
                                                      std::to_string(3.0),
                                                      "[",
                                                      "pow",
                                                      "[",
                                                      "*",
                                                      "2",
                                                      "w",
                                                      "]",
                                                      "[",
                                                      "+",
                                                      std::to_string(3.0),
                                                      std::to_string(-1.0),
                                                      "]",
                                                      "]",
                                                      "]",
                                                      "]"};
            REQUIRE(e.to_list() == baseline);
        }
        WHEN("pow - 2")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = pow(3, 2 * w);
            auto e = f.diff(w);
            static std::list<std::string> baseline = {"[", "*",   std::to_string(2.0),
                                                      "[", "*",   std::to_string(1.098612),
                                                      "[", "pow", std::to_string(3.0),
                                                      "[", "*",   "2",
                                                      "w", "]",   "]",
                                                      "]", "]"};
            REQUIRE(e.to_list() == baseline);
        }
    }
}

TEST_CASE("expr_to_MutableNLPExpr", "[smoke]")
{
    SECTION("constant")
    {
        {
            coek::Expression e(3);
            coek::MutableNLPExpr repn;
            repn.collect_terms(e);

            static std::list<std::string> constval = {std::to_string(3.0)};
            REQUIRE(repn.mutable_values == false);
            REQUIRE(repn.constval->to_list() == constval);
            REQUIRE(repn.linear_coefs.size() == 0);
            REQUIRE(repn.quadratic_coefs.size() == 0);
        }
    }

    SECTION("param")
    {
        WHEN("simple")
        {
            {
                auto p = coek::parameter("p").value(3);
                coek::Expression e = p;
                coek::MutableNLPExpr repn;
                repn.collect_terms(e);

                static std::list<std::string> constval = {"p"};
                REQUIRE(repn.mutable_values == true);
                REQUIRE(repn.constval->to_list() == constval);
                REQUIRE(repn.linear_coefs.size() == 0);
                REQUIRE(repn.quadratic_coefs.size() == 0);
            }
        }
        WHEN("nontrivial multiplier")
        {
            {
                auto p = coek::parameter("p").value(3);
                coek::Expression e = p / 2;
                coek::MutableNLPExpr repn;
                repn.collect_terms(e);

                static std::list<std::string> constval
                    = {"[", "*", std::to_string(0.500), "p", "]"};
                REQUIRE(repn.mutable_values == true);
                REQUIRE(repn.constval->to_list() == constval);
                REQUIRE(repn.linear_coefs.size() == 0);
                REQUIRE(repn.quadratic_coefs.size() == 0);
            }
        }
    }

    SECTION("var")
    {
        WHEN("unfixed")
        {
            {
                coek::Model m;
                auto v = m.add_variable("v").lower(0).upper(1).value(3);
                coek::Expression e = v;
                coek::MutableNLPExpr repn;
                repn.collect_terms(e);

                static std::list<std::string> constval = {std::to_string(0.0)};
                static std::list<std::string> coefval = {std::to_string(1.0)};
                REQUIRE(repn.mutable_values == false);
                REQUIRE(repn.constval->to_list() == constval);
                REQUIRE(repn.linear_coefs.size() == 1);
                REQUIRE(repn.linear_coefs[0]->to_list() == coefval);
                REQUIRE(repn.linear_vars[0] == e.repn);
                REQUIRE(repn.quadratic_coefs.size() == 0);
            }
        }
        WHEN("fixed")
        {
            {
                coek::Model m;
                auto v = m.add_variable("v").lower(0).upper(1).value(3);
                v.fixed(true);
                coek::Expression e = v;
                coek::MutableNLPExpr repn;
                repn.collect_terms(e);

                static std::list<std::string> constval = {"v"};
                REQUIRE(repn.mutable_values == true);
                REQUIRE(repn.constval->to_list() == constval);
                REQUIRE(repn.linear_coefs.size() == 0);
                REQUIRE(repn.quadratic_coefs.size() == 0);
            }
        }
        WHEN("fixed - nontrivial multiplier")
        {
            {
                coek::Model m;
                auto v = m.add_variable("v").lower(0).upper(1).value(3);
                v.fixed(true);
                coek::Expression f(2);
                coek::Expression e = v / f;
                coek::MutableNLPExpr repn;
                repn.collect_terms(e);

                static std::list<std::string> constval
                    = {"[", "*", std::to_string(0.500), "v", "]"};
                REQUIRE(repn.mutable_values == true);
                REQUIRE(repn.constval->to_list() == constval);
                REQUIRE(repn.linear_coefs.size() == 0);
                REQUIRE(repn.quadratic_coefs.size() == 0);
            }
        }
    }

    SECTION("monomial")
    {
        WHEN("unfixed")
        {
            {
                coek::Model m;
                auto v = m.add_variable("v").lower(0).upper(1).value(0);
                coek::Expression e = 2 * v;
                coek::MutableNLPExpr repn;
                repn.collect_terms(e);

                static std::list<std::string> constval = {std::to_string(0.0)};
                static std::list<std::string> coefval = {std::to_string(2.0)};
                REQUIRE(repn.mutable_values == false);
                REQUIRE(repn.constval->to_list() == constval);
                REQUIRE(repn.linear_coefs.size() == 1);
                REQUIRE(repn.linear_coefs[0]->to_list() == coefval);
                auto tmp = std::dynamic_pointer_cast<coek::MonomialTerm>(e.repn);
                REQUIRE(repn.linear_vars[0]->index == tmp->var->index);
                REQUIRE(repn.quadratic_coefs.size() == 0);
            }
        }
        WHEN("fixed")
        {
            {
                coek::Model m;
                auto v = m.add_variable("v").lower(0).upper(1).value(3);
                v.fixed(true);
                coek::Expression e = 2 * v;
                coek::MutableNLPExpr repn;
                repn.collect_terms(e);

                static std::list<std::string> constval = {"[", "*", std::to_string(2.0), "v", "]"};
                REQUIRE(repn.mutable_values == true);
                REQUIRE(repn.constval->to_list() == constval);
                REQUIRE(repn.linear_coefs.size() == 0);
                REQUIRE(repn.quadratic_coefs.size() == 0);
            }
        }
    }

    SECTION("negate")
    {
        {
            coek::Model m;
            auto v = m.add_variable("v").lower(0).upper(1).value(3);
            coek::Expression e = -(v + 1);
            coek::MutableNLPExpr repn;
            repn.collect_terms(e);

            static std::list<std::string> constval = {std::to_string(-1.0)};
            static std::list<std::string> coefval = {std::to_string(-1.0)};
            REQUIRE(repn.mutable_values == false);
            REQUIRE(repn.constval->to_list() == constval);
            REQUIRE(repn.linear_coefs.size() == 1);
            REQUIRE(repn.linear_coefs[0]->to_list() == coefval);
            REQUIRE(repn.linear_vars[0] == v.repn);
            REQUIRE(repn.quadratic_coefs.size() == 0);
        }
    }

    SECTION("plus")
    {
        WHEN("2 terms")
        {
            {
                coek::Model m;
                auto v = m.add_variable("v").lower(0).upper(1).value(3);
                coek::Expression e = v + 1;
                coek::MutableNLPExpr repn;
                repn.collect_terms(e);

                static std::list<std::string> constval = {std::to_string(1.0)};
                static std::list<std::string> coefval = {std::to_string(1.0)};
                REQUIRE(repn.mutable_values == false);
                REQUIRE(repn.constval->to_list() == constval);
                REQUIRE(repn.linear_coefs.size() == 1);
                REQUIRE(repn.linear_coefs[0]->to_list() == coefval);
                REQUIRE(repn.linear_vars[0] == v.repn);
                REQUIRE(repn.quadratic_coefs.size() == 0);
            }
        }
        WHEN("combine sums")
        {
            {
                coek::Model m;
                auto v = m.add_variable("v").lower(0).upper(1).value(3);
                coek::Expression e = v + 1 + (-1 - v);
                coek::MutableNLPExpr repn;
                repn.collect_terms(e);

                static std::list<std::string> constval = {std::to_string(0.0)};
                static std::list<std::string> coefval0 = {std::to_string(1.0)};
                static std::list<std::string> coefval1 = {std::to_string(-1.0)};
                REQUIRE(repn.mutable_values == false);
                REQUIRE(repn.constval->to_list() == constval);
                REQUIRE(repn.linear_coefs.size() == 2);
                REQUIRE(repn.linear_coefs[0]->to_list() == coefval0);
                REQUIRE(repn.linear_coefs[1]->to_list() == coefval1);
                REQUIRE(repn.linear_vars[0] == v.repn);
                REQUIRE(repn.linear_vars[1] == v.repn);
                REQUIRE(repn.quadratic_coefs.size() == 0);
            }
        }
    }

    SECTION("times")
    {
        WHEN("lhs constant")
        {
            {
                coek::Model m;
                auto p = coek::parameter("p");
                auto w = m.add_variable("w").lower(0).upper(1).value(3);
                coek::Expression e = p * w;
                coek::MutableNLPExpr repn;
                repn.collect_terms(e);

                static std::list<std::string> constval = {std::to_string(0.0)};
                static std::list<std::string> coefval = {"p"};
                REQUIRE(repn.constval->to_list() == constval);
                REQUIRE(repn.linear_coefs.size() == 1);
                REQUIRE(repn.linear_coefs[0]->to_list() == coefval);
                REQUIRE(repn.linear_vars[0] == w.repn);
                REQUIRE(repn.quadratic_coefs.size() == 0);
            }
        }
        WHEN("rhs constant")
        {
            {
                coek::Model m;
                auto p = coek::parameter("p");
                auto w = m.add_variable("w").lower(0).upper(1).value(3);
                coek::Expression e = w * p;
                coek::MutableNLPExpr repn;
                repn.collect_terms(e);

                static std::list<std::string> constval = {std::to_string(0.0)};
                static std::list<std::string> coefval = {"p"};
                REQUIRE(repn.constval->to_list() == constval);
                REQUIRE(repn.linear_coefs.size() == 1);
                REQUIRE(repn.linear_coefs[0]->to_list() == coefval);
                REQUIRE(repn.linear_vars[0] == w.repn);
                REQUIRE(repn.quadratic_coefs.size() == 0);
            }
        }
        WHEN("simple quadratic")
        {
            {
                coek::Model m;
                auto w = m.add_variable("w").lower(0).upper(1).value(0);
                coek::Expression e = w * w;
                coek::MutableNLPExpr repn;
                repn.collect_terms(e);

                static std::list<std::string> constval = {std::to_string(0.0)};
                static std::list<std::string> qcoefval = {std::to_string(1.0)};
                REQUIRE(repn.constval->to_list() == constval);
                REQUIRE(repn.linear_coefs.size() == 0);
                REQUIRE(repn.quadratic_coefs.size() == 1);
                REQUIRE(repn.quadratic_coefs[0]->to_list() == qcoefval);
                REQUIRE(repn.quadratic_lvars[0] == w.repn);
                REQUIRE(repn.quadratic_rvars[0] == w.repn);
            }
        }
        WHEN("complex quadratic 1a")
        {
            {
                // Products of linear expressions are not expanded
                coek::Model m;
                auto v = m.add_variable("v").lower(0).upper(1).value(0);
                auto w = m.add_variable("w").lower(0).upper(1).value(0);
                coek::Expression e = (2 + 3 * w + v) * (4 + 5 * v + w);
                coek::MutableNLPExpr repn;
                repn.collect_terms(e);

                static std::list<std::string> constval = {std::to_string(0.0)};
                REQUIRE(repn.constval->to_list() == constval);
                REQUIRE(repn.linear_coefs.size() == 0);
                REQUIRE(repn.quadratic_coefs.size() == 0);
            }
        }
        WHEN("complex quadratic 1b")
        {
            {
                coek::Model m;
                auto v = m.add_variable("v").lower(0).upper(1).value(0);
                auto w = m.add_variable("w").lower(0).upper(1).value(0);
                coek::Expression e = 3 * w * (4 + 5 * v + w);
                coek::MutableNLPExpr repn;
                repn.collect_terms(e);

                static std::list<std::string> constval = {std::to_string(0.0)};
                static std::list<std::string> lcoef0 = {std::to_string(12.000)};
                static std::list<std::string> qcoef0 = {std::to_string(15.000)};
                static std::list<std::string> qcoef1 = {std::to_string(3.000)};
                REQUIRE(repn.constval->to_list() == constval);
                REQUIRE(repn.linear_coefs.size() == 1);
                REQUIRE(repn.linear_coefs[0]->to_list() == lcoef0);
                REQUIRE(repn.quadratic_coefs.size() == 2);
                REQUIRE(repn.quadratic_coefs[0]->to_list() == qcoef0);
                REQUIRE(repn.quadratic_coefs[1]->to_list() == qcoef1);
            }
        }
        WHEN("complex quadratic 2")
        {
            {
                coek::Model m;
                auto w = m.add_variable("w").lower(0).upper(1).value(0);
                coek::Expression e = 3 * (w * w + 2);
                coek::MutableNLPExpr repn;
                repn.collect_terms(e);

                static std::list<std::string> constval = {std::to_string(6.000)};
                static std::list<std::string> qcoefval = {std::to_string(3.000)};
                REQUIRE(repn.constval->to_list() == constval);
                REQUIRE(repn.linear_coefs.size() == 0);
                REQUIRE(repn.quadratic_coefs.size() == 1);
                REQUIRE(repn.quadratic_coefs[0]->to_list() == qcoefval);
                REQUIRE(repn.quadratic_lvars[0] == w.repn);
                REQUIRE(repn.quadratic_rvars[0] == w.repn);
            }
        }
        WHEN("complex quadratic 3")
        {
            {
                coek::Model m;
                auto w = m.add_variable("w").lower(0).upper(1).value(0);
                coek::Expression e = (w * w + 2) * 3;
                coek::MutableNLPExpr repn;
                repn.collect_terms(e);

                static std::list<std::string> constval = {std::to_string(6.0)};
                static std::list<std::string> qcoefval = {std::to_string(3.0)};
                REQUIRE(repn.constval->to_list() == constval);
                REQUIRE(repn.linear_coefs.size() == 0);
                REQUIRE(repn.quadratic_coefs.size() == 1);
                REQUIRE(repn.quadratic_coefs[0]->to_list() == qcoefval);
                REQUIRE(repn.quadratic_lvars[0] == w.repn);
                REQUIRE(repn.quadratic_rvars[0] == w.repn);
            }
        }
        WHEN("complex nonlinear")
        {
            {
                // Force use of ceil and floor functions within a nested product.
                // Force expression of multiplication between constant parameter and quadratic term
                coek::Model m;
                auto w = m.add_variable("w").lower(0).upper(1).value(0);
                auto p = coek::parameter("p");
                coek::Expression e = ceil(w) * floor(w) + p * (w * w);
                coek::MutableNLPExpr repn;
                repn.collect_terms(e);

                REQUIRE(repn.linear_coefs.size() == 0);
                REQUIRE(repn.quadratic_coefs.size() == 1);
                static std::list<std::string> baseline
                    = {"[", "*", "[", "ceil", "w", "]", "[", "floor", "w", "]", "]"};
                REQUIRE(repn.nonlinear->to_list() == baseline);
            }
        }
    }

    SECTION("divide")
    {
        WHEN("lhs parameter - zero")
        {
            {
                coek::Model m;
                auto p = coek::parameter("p");
                auto w = m.add_variable("w").lower(0).upper(1).value(0);
                coek::Expression e = p / w;
                coek::MutableNLPExpr repn;
                repn.collect_terms(e);

                static std::list<std::string> constval = {std::to_string(0.0)};
                static std::list<std::string> nonlinear = {"[", "/", "p", "w", "]"};
                REQUIRE(repn.constval->to_list() == constval);
                REQUIRE(repn.linear_coefs.size() == 0);
                REQUIRE(repn.quadratic_coefs.size() == 0);
                REQUIRE(repn.nonlinear->to_list() == nonlinear);
            }
        }
        WHEN("rhs parameter - zero")
        {
            {
                coek::Model m;
                auto p = coek::parameter("p");
                auto w = m.add_variable("w").lower(0).upper(1).value(0);
                coek::Expression e = w / p;
                coek::MutableNLPExpr repn;
                repn.collect_terms(e);

                static std::list<std::string> constval = {std::to_string(0.0)};
                static std::list<std::string> lcoef0 = {"[", "/", std::to_string(1.0), "p", "]"};
                REQUIRE(repn.constval->to_list() == constval);
                REQUIRE(repn.linear_coefs.size() == 1);
                REQUIRE(repn.linear_coefs[0]->to_list() == lcoef0);
                REQUIRE(repn.quadratic_coefs.size() == 0);
                REQUIRE(repn.nonlinear->to_list() == constval);
            }
        }
        WHEN("lhs constant - zero")
        {
            {
                coek::Model m;
                coek::Expression p;
                auto w = m.add_variable("w").lower(0).upper(1).value(0);
                coek::Expression e = p / w;
                coek::MutableNLPExpr repn;
                repn.collect_terms(e);

                static std::list<std::string> constval = {std::to_string(0.0)};
                REQUIRE(repn.constval->to_list() == constval);
                REQUIRE(repn.linear_coefs.size() == 0);
                REQUIRE(repn.quadratic_coefs.size() == 0);
                REQUIRE(repn.nonlinear->to_list() == constval);
            }
        }
        WHEN("lhs constant - zero AND rhs constant")
        {
            {
                coek::Model m;
                coek::Expression p;
                auto w = coek::parameter("w").value(1);
                coek::Expression e = p / (w + 1);
                coek::MutableNLPExpr repn;
                repn.collect_terms(e);

                static std::list<std::string> constval = {std::to_string(0.0)};
                REQUIRE(repn.constval->to_list() == constval);
                REQUIRE(repn.linear_coefs.size() == 0);
                REQUIRE(repn.quadratic_coefs.size() == 0);
                REQUIRE(repn.nonlinear->to_list() == constval);
            }
        }
        WHEN("rhs constant - zero")
        {
            {
                coek::Model m;
                coek::Expression p;
                auto w = m.add_variable("w").lower(0).upper(1).value(0);
                coek::Expression e = w / p;
                coek::MutableNLPExpr repn;
                REQUIRE_THROWS_WITH(repn.collect_terms(e), "Division by zero error.");
            }
        }
        WHEN("rhs polynomial")
        {
            {
                coek::Model m;
                auto w = m.add_variable("w").lower(0).upper(1).value(0);
                coek::Expression e = w / (1 + w);
                coek::MutableNLPExpr repn;
                repn.collect_terms(e);

                static std::list<std::string> constval = {std::to_string(0.0)};
                static std::list<std::string> nonlinear
                    = {"[", "/", "w", "[", "+", std::to_string(1.0), "w", "]", "]"};
                REQUIRE(repn.constval->to_list() == constval);
                REQUIRE(repn.linear_coefs.size() == 0);
                REQUIRE(repn.quadratic_coefs.size() == 0);
                REQUIRE(repn.nonlinear->to_list() == nonlinear);
            }
        }
        WHEN("rhs nonzero")
        {
            {
                coek::Model m;
                auto p = coek::parameter("p").value(2);
                auto w = m.add_variable("w").lower(0).upper(1).value(0);
                coek::Expression e = (w * w + w + 1) / p;
                coek::MutableNLPExpr repn;
                repn.collect_terms(e);

                static std::list<std::string> constval = {"[", "/", std::to_string(1.0), "p", "]"};
                REQUIRE(repn.constval->to_list() == constval);
                REQUIRE(repn.linear_coefs.size() == 1);
                REQUIRE(repn.linear_coefs[0]->to_list() == constval);
                REQUIRE(repn.quadratic_coefs.size() == 1);
                REQUIRE(repn.quadratic_coefs[0]->to_list() == constval);
            }
        }
    }

    SECTION("constriaints")
    {
        WHEN("inequality")
        {
            {
                coek::Model m;
                auto p = coek::parameter("p");
                auto w = m.add_variable("w").lower(0).upper(1).value(3);
                auto e = p * w + 1 <= 2;
                coek::MutableNLPExpr repn;
                repn.collect_terms(e);

                static std::list<std::string> constval = {std::to_string(1.0)};
                static std::list<std::string> coefval = {"p"};
                REQUIRE(repn.constval->to_list() == constval);
                REQUIRE(repn.linear_coefs.size() == 1);
                REQUIRE(repn.linear_coefs[0]->to_list() == coefval);
                REQUIRE(repn.linear_vars[0] == w.repn);
                REQUIRE(repn.quadratic_coefs.size() == 0);
            }
        }
        WHEN("equality")
        {
            {
                coek::Model m;
                auto p = coek::parameter("p");
                auto w = m.add_variable("w").lower(0).upper(1).value(3);
                auto e = p * w - 1 == 2;
                coek::MutableNLPExpr repn;
                repn.collect_terms(e);

                static std::list<std::string> constval = {std::to_string(-1.0)};
                static std::list<std::string> coefval = {"p"};
                REQUIRE(repn.constval->to_list() == constval);
                REQUIRE(repn.linear_coefs.size() == 1);
                REQUIRE(repn.linear_coefs[0]->to_list() == coefval);
                REQUIRE(repn.linear_vars[0] == w.repn);
                REQUIRE(repn.quadratic_coefs.size() == 0);
            }
        }
    }

    SECTION("intrinsic funcs")
    {
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

TEST_CASE("mutable_values", "[smoke]")
{
    std::unordered_set<std::shared_ptr<coek::VariableTerm>> fixed_vars;
    std::unordered_set<std::shared_ptr<coek::ParameterTerm>> params;

    SECTION("constant")
    {
        coek::Expression e(3);
        mutable_values(e.repn, fixed_vars, params);

        static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{};
        static std::unordered_set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};
        REQUIRE(fixed_vars == vbaseline);
        REQUIRE(params == pbaseline);
    }

    SECTION("param")
    {
        auto p = coek::parameter().value(3);
        coek::Expression e = p;
        mutable_values(e.repn, fixed_vars, params);

        static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{};
        static std::unordered_set<std::shared_ptr<coek::ParameterTerm>> pbaseline{p.repn};
        REQUIRE(fixed_vars == vbaseline);
        REQUIRE(params == pbaseline);
    }

    SECTION("indexparam")
    {
        auto p = coek::set_element("p");
        coek::Expression e = p;
        mutable_values(e.repn, fixed_vars, params);

        static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{};
        static std::unordered_set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};
        REQUIRE(fixed_vars == vbaseline);
        REQUIRE(params == pbaseline);
    }

    SECTION("var")
    {
        WHEN("fixed")
        {
            auto v = coek::variable();
            v.fixed(true);
            coek::Expression e = v;
            mutable_values(e.repn, fixed_vars, params);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{v.repn};
            static std::unordered_set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};
            REQUIRE(fixed_vars == vbaseline);
            REQUIRE(params == pbaseline);
        }

        WHEN("same")
        {
            auto v = coek::variable();
            coek::Expression e = v;
            mutable_values(e.repn, fixed_vars, params);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{};
            static std::unordered_set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};
            REQUIRE(fixed_vars == vbaseline);
            REQUIRE(params == pbaseline);
        }
    }

    SECTION("monomial")
    {
        WHEN("other")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression e = 2 * w;
            mutable_values(e.repn, fixed_vars, params);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{};
            static std::unordered_set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};
            REQUIRE(fixed_vars == vbaseline);
            REQUIRE(params == pbaseline);
        }

        WHEN("fixed")
        {
            coek::Model m;
            auto v = m.add_variable("v").lower(0).upper(1).value(0);
            v.fixed(true);
            coek::Expression e = 2 * v;
            mutable_values(e.repn, fixed_vars, params);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{v.repn};
            static std::unordered_set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};
            REQUIRE(fixed_vars == vbaseline);
            REQUIRE(params == pbaseline);
        }
    }

    SECTION("plus")
    {
        WHEN("linear")
        {
            auto v = coek::variable("v").lower(0).upper(1).value(0);
            v.fixed(true);
            coek::Expression e = 2 * (v + v) + v;
            mutable_values(e.repn, fixed_vars, params);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{v.repn};
            static std::unordered_set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};
            REQUIRE(fixed_vars == vbaseline);
            REQUIRE(params == pbaseline);
        }
        WHEN("simple")
        {
            auto p = coek::parameter();
            auto v = coek::variable("v").lower(0).upper(1).value(0);
            v.fixed(true);
            coek::Expression e = 3 * p + 2 * v;
            mutable_values(e.repn, fixed_vars, params);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{v.repn};
            static std::unordered_set<std::shared_ptr<coek::ParameterTerm>> pbaseline{p.repn};
            REQUIRE(fixed_vars == vbaseline);
            REQUIRE(params == pbaseline);
        }
    }

    SECTION("negate")
    {
        WHEN("linear")
        {
            auto p = coek::parameter();
            auto v = coek::variable("v").lower(0).upper(1).value(0).fixed(true);
            coek::Expression e = -(v + 1);
            mutable_values(e.repn, fixed_vars, params);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{v.repn};
            static std::unordered_set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};
            REQUIRE(fixed_vars == vbaseline);
            REQUIRE(params == pbaseline);
        }
    }

    SECTION("times")
    {
        WHEN("lhs constant")
        {
            coek::Model m;
            auto p = coek::parameter().value(2);
            auto v = m.add_variable("v").lower(0).upper(1).value(0).fixed(true);
            coek::Expression e = p * v;
            mutable_values(e.repn, fixed_vars, params);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{v.repn};
            static std::unordered_set<std::shared_ptr<coek::ParameterTerm>> pbaseline{p.repn};
            REQUIRE(fixed_vars == vbaseline);
            REQUIRE(params == pbaseline);
        }
        WHEN("simple quadratic LHS")
        {
            coek::Model m;
            auto v = m.add_variable("v").lower(0).upper(1).value(0).fixed(true);
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression e = v * w;
            mutable_values(e.repn, fixed_vars, params);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{v.repn};
            static std::unordered_set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};
            REQUIRE(fixed_vars == vbaseline);
            REQUIRE(params == pbaseline);
        }
    }

    SECTION("divide")
    {
        WHEN("lhs zero")
        {
            coek::Model m;
            auto p = coek::parameter();
            auto w = m.add_variable("w").lower(0).upper(1).value(0).fixed(true);
            coek::Expression e = p / w;
            mutable_values(e.repn, fixed_vars, params);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{w.repn};
            static std::unordered_set<std::shared_ptr<coek::ParameterTerm>> pbaseline{p.repn};
            REQUIRE(fixed_vars == vbaseline);
            REQUIRE(params == pbaseline);
        }
        WHEN("rhs nonzero")
        {
            coek::Model m;
            auto p = coek::parameter().value(2);
            auto w = m.add_variable("w").lower(0).upper(1).value(0).fixed(true);
            coek::Expression e = w / p;
            mutable_values(e.repn, fixed_vars, params);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{w.repn};
            static std::unordered_set<std::shared_ptr<coek::ParameterTerm>> pbaseline{p.repn};
            REQUIRE(fixed_vars == vbaseline);
            REQUIRE(params == pbaseline);
        }
        WHEN("rhs polynomial")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0).fixed(true);
            coek::Expression e = w / (1 + w);
            mutable_values(e.repn, fixed_vars, params);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{w.repn};
            static std::unordered_set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};
            REQUIRE(fixed_vars == vbaseline);
            REQUIRE(params == pbaseline);
        }
    }

    SECTION("coverage")
    {
        WHEN("variable partial plus monomial - 1")
        {
            coek::Model m;
            auto v = m.add_variable("v").lower(0).upper(1).value(0);
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression e = w * v + v * (2 * w + 1);
            v.fixed(true);
            w.fixed(true);
            mutable_values(e.repn, fixed_vars, params);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{v.repn,
                                                                                     w.repn};
            static std::unordered_set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};
            REQUIRE(fixed_vars == vbaseline);
            REQUIRE(params == pbaseline);
        }
        WHEN("variable partial plus monomial - 2")
        {
            coek::Model m;
            auto v = m.add_variable("v").lower(0).upper(1).value(0).fixed(true);
            auto w = m.add_variable("w").lower(0).upper(1).value(0).fixed(true);
            coek::Expression e = v * (2 * w + 1);
            mutable_values(e.repn, fixed_vars, params);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{v.repn,
                                                                                     w.repn};
            static std::unordered_set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};
            REQUIRE(fixed_vars == vbaseline);
            REQUIRE(params == pbaseline);
        }
        WHEN("constant partial plus monomial")
        {
            coek::Model m;
            auto v = m.add_variable("v").lower(0).upper(1).value(0);
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = 3 * w + 2 * w;
            w.fixed(true);
            coek::Expression e = v * (2 * w + 1);
            mutable_values(e.repn, fixed_vars, params);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{w.repn};
            static std::unordered_set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};
            REQUIRE(fixed_vars == vbaseline);
            REQUIRE(params == pbaseline);
        }
        WHEN("negative monomial")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            w.fixed(true);
            coek::Expression e = -(-w) + (-(-w));
            mutable_values(e.repn, fixed_vars, params);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{w.repn};
            static std::unordered_set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};
            REQUIRE(fixed_vars == vbaseline);
            REQUIRE(params == pbaseline);
        }
    }

    SECTION("intrinsic funcs"){

        // clang-format off
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
        MV_INTRINSIC_TEST2(pow)}  // clang-format on

    SECTION("objective")
    {
        coek::Model m;
        auto w = m.add_variable("w").lower(0).upper(1).value(0);
        auto v = m.add_variable("v").lower(0).upper(1).value(0);
        auto o = m.add_objective(w * v + v * (2 * w + 1));
        v.fixed(true);
        w.fixed(true);
        mutable_values(o.expr().repn, fixed_vars, params);

        static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{v.repn, w.repn};
        static std::unordered_set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};
        REQUIRE(fixed_vars == vbaseline);
        REQUIRE(params == pbaseline);
    }

    SECTION("constraint")
    {
        WHEN("inequality")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            auto v = m.add_variable("v").lower(0).upper(1).value(0);
            auto e = w * v + v * (2 * w + 1) <= 0;
            v.fixed(true);
            w.fixed(true);
            mutable_values(e.repn, fixed_vars, params);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{v.repn,
                                                                                     w.repn};
            static std::unordered_set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};
            REQUIRE(fixed_vars == vbaseline);
            REQUIRE(params == pbaseline);
        }
        WHEN("equality")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            auto v = m.add_variable("v").lower(0).upper(1).value(0);
            auto e = w * v + v * (2 * w + 1) == 0;
            v.fixed(true);
            w.fixed(true);
            mutable_values(e.repn, fixed_vars, params);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{v.repn,
                                                                                     w.repn};
            static std::unordered_set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};
            REQUIRE(fixed_vars == vbaseline);
            REQUIRE(params == pbaseline);
        }
    }
}
