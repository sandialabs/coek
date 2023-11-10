
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

#define INTRINSIC_TEST1(FN)                                          \
    WHEN(#FN)                                                        \
    {                                                                \
        {                                                            \
            auto v = coek::variable("v").lower(0).upper(1).value(0); \
            coek::Expression e = FN(v + 1);                          \
            double tmp = 1.0 * FN(1.0);                              \
            REQUIRE(evaluate_expr(e.repn) == tmp);                   \
        }                                                            \
    }

#define INTRINSIC_TEST2(FN)                                          \
    WHEN(#FN " 2")                                                   \
    {                                                                \
        {                                                            \
            auto v = coek::variable("v").lower(0).upper(1).value(0); \
            coek::Expression e = FN(v + 1, v);                       \
            double tmp = 1.0 * FN(1.0, 0.0);                         \
            REQUIRE(evaluate_expr(e.repn) == tmp);                   \
        }                                                            \
    }

TEST_CASE("evaluate_expr", "[smoke]")
{
    SECTION("constant")
    {
        {
            auto p = coek::parameter("p").value(1);
            coek::Expression e(3 + p);

            REQUIRE(evaluate_expr(e.repn) == 4.0);
        }
    }

    SECTION("param")
    {
        WHEN("simple")
        {
            {
                auto p = coek::parameter("p").value(3);
                coek::Expression e = p;

                REQUIRE(evaluate_expr(e.repn) == 3.0);
            }
        }
        WHEN("nontrivial multiplier")
        {
            {
                auto p = coek::parameter("p").value(3);
                coek::Expression e = p / 2;

                REQUIRE(evaluate_expr(e.repn) == 1.5);
            }
        }
    }

    SECTION("var")
    {
        WHEN("unfixed")
        {
            {
                auto v = coek::variable("v").lower(0).upper(1).value(3);
                coek::Expression e = v;

                REQUIRE(evaluate_expr(e.repn) == 3.0);
            }
        }
        WHEN("fixed")
        {
            {
                auto v = coek::variable("v").lower(0).upper(1).value(3).fixed(true);
                coek::Expression e = v;

                REQUIRE(evaluate_expr(e.repn) == 3.0);
            }
        }
        WHEN("fixed - nontrivial multiplier")
        {
            {
                auto v = coek::variable("v").lower(0).upper(1).value(3);
                coek::Expression f(2);
                coek::Expression e = v / f;

                REQUIRE(evaluate_expr(e.repn) == 1.5);
            }
        }
    }

    SECTION("monomial")
    {
        WHEN("unfixed")
        {
            {
                auto v = coek::variable("v").lower(0).upper(1).value(3);
                coek::Expression e = 2 * v;

                REQUIRE(evaluate_expr(e.repn) == 6.0);
            }
        }
        WHEN("fixed")
        {
            {
                auto v = coek::variable("v").lower(0).upper(1).value(3).fixed(true);
                coek::Expression e = 2 * v;

                REQUIRE(evaluate_expr(e.repn) == 6.0);
            }
        }
    }

    SECTION("negate")
    {
        {
            auto v = coek::variable("v").lower(0).upper(1).value(3).fixed(true);
            coek::Expression e = -(v + 1);

            REQUIRE(evaluate_expr(e.repn) == -4.0);
        }
    }

    SECTION("subexpression")
    {
        {
            coek::Model m;
            auto v = m.add_variable("v").lower(0).upper(1).value(3);
            auto E = coek::subexpression().value(v + 1);
            coek::Expression e = E + 2 * (E + 1);

            REQUIRE(evaluate_expr(e.repn) == 14.0);
        }
    }

    SECTION("plus")
    {
        WHEN("2 terms")
        {
            {
                auto v = coek::variable("v").lower(0).upper(1).value(3);
                coek::Expression e = v + 1;

                REQUIRE(evaluate_expr(e.repn) == 4.0);
            }
        }
        WHEN("combine sums")
        {
            {
                auto v = coek::variable("v").lower(0).upper(1).value(3);
                coek::Expression e = (v + 1) + (-2 - v);

                REQUIRE(evaluate_expr(e.repn) == -1.0);
            }
        }
    }

    SECTION("times")
    {
        WHEN("lhs - param zero")
        {
            {
                auto p = coek::parameter("p");
                auto v = coek::variable("v").lower(0).upper(1).value(3);
                coek::Expression e = p * v;

                REQUIRE(evaluate_expr(e.repn) == 0.0);
            }
        }
        WHEN("lhs - param one")
        {
            {
                auto p = coek::parameter("p").value(1);
                auto v = coek::variable("v").lower(0).upper(1).value(3);
                coek::Expression e = p * v;

                REQUIRE(evaluate_expr(e.repn) == 3.0);
            }
        }
        WHEN("lhs - param other")
        {
            {
                auto p = coek::parameter("p").value(2.0);
                auto v = coek::variable("v").lower(0).upper(1).value(3);
                coek::Expression e = p * v;

                REQUIRE(evaluate_expr(e.repn) == 6.0);
            }
        }
        WHEN("rhs - param zero")
        {
            {
                auto p = coek::parameter("p");
                auto v = coek::variable("v").lower(0).upper(1).value(3);
                coek::Expression e = v * p;

                REQUIRE(evaluate_expr(e.repn) == 0.0);
            }
        }
        WHEN("rhs - param one")
        {
            {
                auto p = coek::parameter("p").value(1);
                auto v = coek::variable("v").lower(0).upper(1).value(3);
                coek::Expression e = v * p;

                REQUIRE(evaluate_expr(e.repn) == 3.0);
            }
        }
        WHEN("rhs - param other")
        {
            {
                auto p = coek::parameter("p").value(2.0);
                auto v = coek::variable("v").lower(0).upper(1).value(3);
                coek::Expression e = v * p;

                REQUIRE(evaluate_expr(e.repn) == 6.0);
            }
        }
        WHEN("complex quadratic 1a")
        {
            {
                auto v = coek::variable("v").lower(0).upper(1).value(0);
                auto w = coek::variable("w").lower(0).upper(1).value(1).fixed(true);
                coek::Expression e = (2 + 3 * w + v) * 4;

                REQUIRE(evaluate_expr(e.repn) == 20.0);
            }
        }
    }

    SECTION("divide")
    {
        WHEN("lhs parameter - zero")
        {
            {
                auto p = coek::parameter("p");
                auto w = coek::variable("w").lower(0).upper(1).value(0);
                coek::Expression e = p / w;

                REQUIRE(evaluate_expr(e.repn) == 0.0);
            }
        }
        WHEN("lhs constant - zero AND rhs constant")
        {
            {
                auto p = coek::parameter("p").value(2.0);
                auto w = coek::variable("w").lower(0).upper(1).value(1).fixed(true);
                coek::Expression e = p / (w + 1);

                REQUIRE(evaluate_expr(e.repn) == 1.0);
            }
        }
        WHEN("rhs constant - rhs 1.0")
        {
            {
                auto p = coek::parameter("p").value(1.0);
                auto w = coek::variable("w").lower(0).upper(1).value(1);
                coek::Expression e = w / p;

                REQUIRE(evaluate_expr(e.repn) == 1.0);
            }
        }
        WHEN("rhs constant")
        {
            {
                auto p = coek::parameter("p").value(2.0);
                auto w = coek::variable("w").lower(0).upper(1).value(1);
                coek::Expression e = w / p;

                REQUIRE(evaluate_expr(e.repn) == 0.5);
            }
        }
    }

    SECTION("if_else")
    {
        WHEN("if_else - parameter - zero")
        {
            {
                auto p = coek::parameter("p");
                auto w = coek::variable("w").lower(0).upper(1).value(2);
                coek::Expression e = if_else(p, w);

                REQUIRE(evaluate_expr(e.repn) == 0.0);
            }
        }
        WHEN("if_else - parameter - zero")
        {
            {
                auto p = coek::parameter("p");
                auto w = coek::variable("w").lower(0).upper(1).value(2);
                coek::Expression e = if_else(p, w, 3);

                REQUIRE(evaluate_expr(e.repn) == 3.0);
            }
        }
        WHEN("parameter - one")
        {
            {
                auto p = coek::parameter("p").value(1.0);
                auto w = coek::variable("w").lower(0).upper(1).value(2);
                coek::Expression e = if_else(p, w);

                REQUIRE(evaluate_expr(e.repn) == 2.0);
            }
        }
        WHEN("conditional - true")
        {
            {
                auto p = coek::parameter("p").value(1.0);
                auto w = coek::variable("w").lower(0).upper(1).value(2);
                coek::Expression e = if_else(p > 0.5, w);

                REQUIRE(evaluate_expr(e.repn) == 2.0);
            }
        }
    }

    SECTION("constraints")
    {
        WHEN("inequality")
        {
            {
                auto p = coek::parameter("p");
                auto w = coek::variable("w").lower(0).upper(1).value(3);
                auto e = w + p <= 2;

                REQUIRE(evaluate_expr(e.repn) == 3.0);
            }
        }
        WHEN("equality")
        {
            {
                auto p = coek::parameter("p");
                auto w = coek::variable("w").lower(0).upper(1).value(3);
                auto e = w + p == 2;

                REQUIRE(evaluate_expr(e.repn) == 3.0);
            }
        }
    }

    SECTION("intrinsic funcs"){
        INTRINSIC_TEST1(abs) INTRINSIC_TEST1(ceil) INTRINSIC_TEST1(floor) INTRINSIC_TEST1(exp)
            INTRINSIC_TEST1(log) INTRINSIC_TEST1(log10) INTRINSIC_TEST1(sqrt) INTRINSIC_TEST1(sin)
                INTRINSIC_TEST1(cos) INTRINSIC_TEST1(tan) INTRINSIC_TEST1(sinh)
                    INTRINSIC_TEST1(cosh) INTRINSIC_TEST1(tanh) INTRINSIC_TEST1(asin)
                        INTRINSIC_TEST1(acos) INTRINSIC_TEST1(atan) INTRINSIC_TEST1(asinh)
                            INTRINSIC_TEST1(acosh) INTRINSIC_TEST1(atanh) INTRINSIC_TEST2(pow)}

    SECTION("pow func")
    {
        WHEN("lhs constant - 0")
        {
            {
                auto p = coek::parameter("p");
                auto w = coek::variable("w").lower(0).upper(1).value(3);
                auto e = coek::pow(p, w);

                REQUIRE(evaluate_expr(e.repn) == 0.0);
            }
        }
        WHEN("lhs constant - 1")
        {
            {
                auto p = coek::parameter("p").value(1.0);
                auto w = coek::variable("w").lower(0).upper(1).value(3);
                auto e = coek::pow(p, w);

                REQUIRE(evaluate_expr(e.repn) == 1.0);
            }
        }
        WHEN("lhs constant - other")
        {
            {
                auto p = coek::parameter("p").value(2.0);
                auto w = coek::variable("w").lower(0).upper(1).value(3);
                auto e = coek::pow(p, w);

                REQUIRE(evaluate_expr(e.repn) == 8.0);
            }
        }
        WHEN("rhs constant - 0")
        {
            {
                auto p = coek::parameter("p").value(0.0);
                auto w = coek::variable("w").lower(0).upper(1).value(3);
                auto e = coek::pow(w, p);

                REQUIRE(evaluate_expr(e.repn) == 1.0);
            }
        }
        WHEN("rhs constant - 1")
        {
            {
                auto p = coek::parameter("p").value(1.0);
                auto w = coek::variable("w").lower(0).upper(1).value(3);
                auto e = coek::pow(w, p);

                REQUIRE(evaluate_expr(e.repn) == 3.0);
            }
        }
        WHEN("rhs constant - other")
        {
            {
                auto p = coek::parameter("p").value(2.0);
                auto w = coek::variable("w").lower(0).upper(1).value(3);
                auto e = coek::pow(w, p);

                REQUIRE(evaluate_expr(e.repn) == 9.0);
            }
        }
        WHEN("general")
        {
            {
                auto w = coek::variable("w").lower(0).upper(1).value(3);
                auto e = coek::pow(w, w - 1);

                REQUIRE(evaluate_expr(e.repn) == 9.0);
            }
        }
    }
}
