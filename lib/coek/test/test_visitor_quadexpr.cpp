
#include <iostream>
#include <sstream>

#include "catch2/catch.hpp"
#include "coek/ast/base_terms.hpp"
#include "coek/ast/constraint_terms.hpp"
#include "coek/ast/value_terms.hpp"
#include "coek/ast/expr_terms.hpp"
#include "coek/ast/visitor_fns.hpp"
#include "coek/coek.hpp"
#include "coek/util/io_utils.hpp"

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

    SECTION("subexpression")
    {
        {
            coek::Model m;
            auto v = m.add_variable("v").lower(0).upper(1).value(3);
            auto E = coek::subexpression().value(v + 1);
            coek::Expression e = E + 2 * (E + 1);
            REQUIRE(e.value() == 14);

            coek::QuadraticExpr repn;
            repn.collect_terms(e);

            REQUIRE(repn.constval == 5);
            REQUIRE(repn.linear_coefs.size() == 2);
            REQUIRE(repn.linear_coefs[0] == 1);
            REQUIRE(repn.linear_coefs[1] == 2);
            REQUIRE(repn.linear_vars[0] == v.repn);
            REQUIRE(repn.linear_vars[1] == v.repn);
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
}
