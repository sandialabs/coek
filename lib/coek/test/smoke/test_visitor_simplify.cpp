
#include <iostream>
#include <sstream>

#include "catch2/catch_test_macros.hpp"
#include "coek/ast/base_terms.hpp"
#include "coek/ast/constraint_terms.hpp"
#include "coek/ast/value_terms.hpp"
#include "coek/ast/expr_terms.hpp"
#include "coek/ast/visitor_fns.hpp"
#include "coek/coek.hpp"
#include "coek/util/io_utils.hpp"

#define INTRINSIC_TEST1(FN)                                                      \
    WHEN(#FN " 1")                                                               \
    {                                                                            \
        {                                                                        \
            auto v = coek::variable("v").lower(0).upper(1).value(0);             \
            coek::Expression e = FN(v + 1);                                      \
            static std::list<std::string> baseline                               \
                = {"[", #FN, "[", "+", "v", std::to_string(1.0), "]", "]"};      \
            REQUIRE(simplify_expr(e.repn)->to_list() == baseline);               \
        }                                                                        \
    }                                                                            \
    WHEN(#FN " 2")                                                               \
    {                                                                            \
        {                                                                        \
            auto v = coek::variable("v").lower(0).upper(1).value(0).fixed(true); \
            coek::Expression e = FN(v + 1);                                      \
            double tmp = 1.0 * FN(1.0);                                          \
            static std::list<std::string> baseline = {std::to_string(tmp)};      \
            REQUIRE(simplify_expr(e.repn)->to_list() == baseline);               \
        }                                                                        \
    }

#define INTRINSIC_TEST2(FN)                                                      \
    WHEN(#FN " 1")                                                               \
    {                                                                            \
        {                                                                        \
            auto v = coek::variable("v").lower(0).upper(1).value(0);             \
            coek::Expression e = FN(v + 1, v);                                   \
            static std::list<std::string> baseline                               \
                = {"[", #FN, "[", "+", "v", std::to_string(1.0), "]", "v", "]"}; \
            REQUIRE(simplify_expr(e.repn)->to_list() == baseline);               \
        }                                                                        \
    }                                                                            \
    WHEN(#FN " 2")                                                               \
    {                                                                            \
        {                                                                        \
            auto v = coek::variable("v").lower(0).upper(1).value(0).fixed(true); \
            coek::Expression e = FN(v + 1, v);                                   \
            double tmp = 1.0 * FN(1.0, 0.0);                                     \
            static std::list<std::string> baseline = {std::to_string(tmp)};      \
            REQUIRE(simplify_expr(e.repn)->to_list() == baseline);               \
        }                                                                        \
    }

TEST_CASE("simplify_expr", "[smoke]")
{
    SECTION("constant")
    {
        {
            auto p = coek::parameter("p").value(1);
            coek::Expression e(3 + p);

            static std::list<std::string> baseline = {std::to_string(4.0)};
            REQUIRE(simplify_expr(e.repn)->to_list() == baseline);
        }
    }

    SECTION("param")
    {
        WHEN("simple")
        {
            {
                auto p = coek::parameter("p").value(3);
                coek::Expression e = p;

                static std::list<std::string> baseline = {std::to_string(3.0)};
                REQUIRE(simplify_expr(e.repn)->to_list() == baseline);
            }
        }
        WHEN("nontrivial multiplier")
        {
            {
                auto p = coek::parameter("p").value(3);
                coek::Expression e = p / 2;

                static std::list<std::string> baseline = {std::to_string(1.5)};
                REQUIRE(simplify_expr(e.repn)->to_list() == baseline);
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

                static std::list<std::string> baseline = {"v"};
                REQUIRE(simplify_expr(e.repn)->to_list() == baseline);
            }
        }
        WHEN("fixed")
        {
            {
                auto v = coek::variable("v").lower(0).upper(1).value(3).fixed(true);
                coek::Expression e = v;

                static std::list<std::string> baseline = {std::to_string(3.0)};
                REQUIRE(simplify_expr(e.repn)->to_list() == baseline);
            }
        }
        WHEN("fixed - nontrivial multiplier")
        {
            {
                auto v = coek::variable("v").lower(0).upper(1).value(3);
                coek::Expression f(2);
                coek::Expression e = v / f;

                static std::list<std::string> baseline = {"[", "/", "v", std::to_string(2.0), "]"};
                REQUIRE(simplify_expr(e.repn)->to_list() == baseline);
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

                static std::list<std::string> baseline = {"[", "*", std::to_string(2), "v", "]"};
                REQUIRE(simplify_expr(e.repn)->to_list() == baseline);
            }
        }
        WHEN("fixed")
        {
            {
                auto v = coek::variable("v").lower(0).upper(1).value(3).fixed(true);
                coek::Expression e = 2 * v;

                static std::list<std::string> baseline = {std::to_string(6.0)};
                REQUIRE(simplify_expr(e.repn)->to_list() == baseline);
            }
        }
    }

    SECTION("negate")
    {
        {
            auto v = coek::variable("v").lower(0).upper(1).value(3).fixed(true);
            coek::Expression e = -(v + 1);

            static std::list<std::string> baseline = {std::to_string(-4.0)};
            REQUIRE(simplify_expr(e.repn)->to_list() == baseline);
        }
    }

#if 0
    SECTION("subexpression")
    {
        {
            coek::Model m;
            auto v = m.add_variable("v").lower(0).upper(1).value(3);
            auto E = coek::subexpression().value(v + 1);
            coek::Expression e = E + 2 * (E + 1);
            REQUIRE(e.value() == 14);

            coek::MutableNLPExpr repn;
            repn.collect_terms(e);

            static std::list<std::string> constval = {std::to_string(5.0)};
            static std::list<std::string> coefval0 = {std::to_string(1.0)};
            static std::list<std::string> coefval1 = {std::to_string(2.0)};
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
#endif

    SECTION("plus")
    {
        WHEN("2 terms")
        {
            {
                auto v = coek::variable("v").lower(0).upper(1).value(3);
                coek::Expression e = v + 1;

                static std::list<std::string> baseline = {"[", "+", "v", std::to_string(1.0), "]"};
                REQUIRE(simplify_expr(e.repn)->to_list() == baseline);
            }
        }
        WHEN("combine sums")
        {
            {
                auto v = coek::variable("v").lower(0).upper(1).value(3);
                coek::Expression e = (v + 1) + (-2 - v);

                static std::list<std::string> baseline = {"[",
                                                          "+",
                                                          "v",
                                                          "[",
                                                          "+",
                                                          "[",
                                                          "*",
                                                          std::to_string(-1),
                                                          "v",
                                                          "]",
                                                          std::to_string(-2.0),
                                                          "]",
                                                          std::to_string(1.0),
                                                          "]"};
                REQUIRE(simplify_expr(e.repn)->to_list() == baseline);
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

                static std::list<std::string> baseline = {std::to_string(0.0)};
                REQUIRE(simplify_expr(e.repn)->to_list() == baseline);
            }
        }
        WHEN("lhs - param one")
        {
            {
                auto p = coek::parameter("p").value(1);
                auto v = coek::variable("v").lower(0).upper(1).value(3);
                coek::Expression e = p * v;

                static std::list<std::string> baseline = {"v"};
                REQUIRE(simplify_expr(e.repn)->to_list() == baseline);
            }
        }
        WHEN("lhs - param other")
        {
            {
                auto p = coek::parameter("p").value(2.0);
                auto v = coek::variable("v").lower(0).upper(1).value(3);
                coek::Expression e = p * v;

                static std::list<std::string> baseline = {"[", "*", std::to_string(2.0), "v", "]"};
                REQUIRE(simplify_expr(e.repn)->to_list() == baseline);
            }
        }
        WHEN("rhs - param zero")
        {
            {
                auto p = coek::parameter("p");
                auto v = coek::variable("v").lower(0).upper(1).value(3);
                coek::Expression e = v * p;

                static std::list<std::string> baseline = {std::to_string(0.0)};
                REQUIRE(simplify_expr(e.repn)->to_list() == baseline);
            }
        }
        WHEN("rhs - param one")
        {
            {
                auto p = coek::parameter("p").value(1);
                auto v = coek::variable("v").lower(0).upper(1).value(3);
                coek::Expression e = v * p;

                static std::list<std::string> baseline = {"v"};
                REQUIRE(simplify_expr(e.repn)->to_list() == baseline);
            }
        }
        WHEN("rhs - param other")
        {
            {
                auto p = coek::parameter("p").value(2.0);
                auto v = coek::variable("v").lower(0).upper(1).value(3);
                coek::Expression e = v * p;

                static std::list<std::string> baseline = {"[", "*", "v", std::to_string(2.0), "]"};
                REQUIRE(simplify_expr(e.repn)->to_list() == baseline);
            }
        }
        WHEN("complex quadratic 1a")
        {
            {
                auto v = coek::variable("v").lower(0).upper(1).value(0);
                auto w = coek::variable("w").lower(0).upper(1).value(1).fixed(true);
                coek::Expression e = (2 + 3 * w + v) * 4;

                static std::list<std::string> baseline
                    = {"[", "*", "[", "+", "v", std::to_string(5.0), "]", std::to_string(4.0), "]"};
                REQUIRE(simplify_expr(e.repn)->to_list() == baseline);
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

                static std::list<std::string> baseline = {std::to_string(0.0)};
                REQUIRE(simplify_expr(e.repn)->to_list() == baseline);
            }
        }
        WHEN("lhs constant - zero AND rhs constant")
        {
            {
                auto p = coek::parameter("p").value(2.0);
                auto w = coek::variable("w").lower(0).upper(1).value(1).fixed(true);
                coek::Expression e = p / (w + 1);

                static std::list<std::string> baseline = {std::to_string(1.0)};
                REQUIRE(simplify_expr(e.repn)->to_list() == baseline);
            }
        }
        WHEN("rhs constant - rhs 1.0")
        {
            {
                auto p = coek::parameter("p").value(1.0);
                auto w = coek::variable("w").lower(0).upper(1).value(1);
                coek::Expression e = w / p;

                static std::list<std::string> baseline = {"w"};
                REQUIRE(simplify_expr(e.repn)->to_list() == baseline);
            }
        }
        WHEN("rhs constant")
        {
            {
                auto p = coek::parameter("p").value(2.0);
                auto w = coek::variable("w").lower(0).upper(1).value(1);
                coek::Expression e = w / p;

                static std::list<std::string> baseline = {"[", "/", "w", std::to_string(2.0), "]"};
                REQUIRE(simplify_expr(e.repn)->to_list() == baseline);
            }
        }
    }

    SECTION("if_else")
    {
        WHEN("cond parameter")
        {
            auto p = coek::parameter("p").value(2.0);
            auto w = coek::variable("w").lower(0).upper(1).value(1);
            coek::Expression e = if_else(p >= 2.0, w, 2 * w);

            static std::list<std::string> baseline = {"w"};
            REQUIRE(simplify_expr(e.repn)->to_list() == baseline);
        }
        WHEN("cond parameter - nonzero")
        {
            auto p = coek::parameter("p").value(2.0);
            auto w = coek::variable("w").lower(0).upper(1).value(1);
            coek::Expression e = if_else(p, w, 2 * w);

            static std::list<std::string> baseline = {"w"};
            REQUIRE(simplify_expr(e.repn)->to_list() == baseline);
        }
        WHEN("cond parameter - zero")
        {
            auto p = coek::parameter("p").value(0.0);
            auto w = coek::variable("w").lower(0).upper(1).value(1);
            coek::Expression e = if_else(p, w, 2 * w);

            static std::list<std::string> baseline = {"[", "*", std::to_string(2), "w", "]"};
            REQUIRE(simplify_expr(e.repn)->to_list() == baseline);
        }
        WHEN("cond variable 1")
        {
            auto w = coek::variable("w").lower(0).upper(1).value(1);
            coek::Expression e = if_else(w, w, 2 * w);

            static std::list<std::string> baseline
                = {"[", "If", "w", "w", "[", "*", std::to_string(2), "w", "]", "]"};
            REQUIRE(simplify_expr(e.repn)->to_list() == baseline);
        }
        WHEN("cond variable 2")
        {
            auto p = coek::parameter("p").value(1.0);
            auto w = coek::variable("w").lower(0).upper(1).value(1);
            coek::Expression e = if_else(w, p, 2 * p);

            static std::list<std::string> baseline
                = {"[", "If", "w", std::to_string(1.0), std::to_string(2.0), "]"};
            REQUIRE(simplify_expr(e.repn)->to_list() == baseline);
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

                static std::list<std::string> baseline
                    = {"[", "<=", "-Inf", "w", std::to_string(2.0), "]"};
                REQUIRE(simplify_expr(e.repn)->to_list() == baseline);
            }
        }
        WHEN("equality")
        {
            {
                auto p = coek::parameter("p");
                auto w = coek::variable("w").lower(0).upper(1).value(3);
                auto e = w + p == 2;

                static std::list<std::string> baseline
                    = {"[", "==", "w", std::to_string(2.000), "]"};
                REQUIRE(simplify_expr(e.repn)->to_list() == baseline);
            }
        }
    }

    SECTION("intrinsic funcs"){
        // INTRINSIC_TEST1(abs)
        INTRINSIC_TEST1(ceil) INTRINSIC_TEST1(floor) INTRINSIC_TEST1(exp) INTRINSIC_TEST1(log)
            INTRINSIC_TEST1(log10) INTRINSIC_TEST1(sqrt) INTRINSIC_TEST1(sin) INTRINSIC_TEST1(cos)
                INTRINSIC_TEST1(tan) INTRINSIC_TEST1(sinh) INTRINSIC_TEST1(cosh)
                    INTRINSIC_TEST1(tanh) INTRINSIC_TEST1(asin) INTRINSIC_TEST1(acos)
                        INTRINSIC_TEST1(atan) INTRINSIC_TEST1(asinh) INTRINSIC_TEST1(acosh)
                            INTRINSIC_TEST1(atanh) INTRINSIC_TEST2(pow)}

    SECTION("pow func")
    {
        WHEN("lhs constant - 0")
        {
            {
                auto p = coek::parameter("p");
                auto w = coek::variable("w").lower(0).upper(1).value(3);
                auto e = coek::pow(p, w);

                static std::list<std::string> baseline = {std::to_string(0.0)};
                REQUIRE(simplify_expr(e.repn)->to_list() == baseline);
            }
        }
        WHEN("lhs constant - 1")
        {
            {
                auto p = coek::parameter("p").value(1.0);
                auto w = coek::variable("w").lower(0).upper(1).value(3);
                auto e = coek::pow(p, w);

                static std::list<std::string> baseline = {std::to_string(1.0)};
                REQUIRE(simplify_expr(e.repn)->to_list() == baseline);
            }
        }
        WHEN("lhs constant - other")
        {
            {
                auto p = coek::parameter("p").value(2.0);
                auto w = coek::variable("w").lower(0).upper(1).value(3);
                auto e = coek::pow(p, w);

                static std::list<std::string> baseline
                    = {"[", "pow", std::to_string(2.0), "w", "]"};
                REQUIRE(simplify_expr(e.repn)->to_list() == baseline);
            }
        }
        WHEN("rhs constant - 0")
        {
            {
                auto p = coek::parameter("p").value(0.0);
                auto w = coek::variable("w").lower(0).upper(1).value(3);
                auto e = coek::pow(w, p);

                static std::list<std::string> baseline = {std::to_string(1.0)};
                REQUIRE(simplify_expr(e.repn)->to_list() == baseline);
            }
        }
        WHEN("rhs constant - 1")
        {
            {
                auto p = coek::parameter("p").value(1.0);
                auto w = coek::variable("w").lower(0).upper(1).value(3);
                auto e = coek::pow(w, p);

                static std::list<std::string> baseline = {"w"};
                REQUIRE(simplify_expr(e.repn)->to_list() == baseline);
            }
        }
        WHEN("rhs constant - other")
        {
            {
                auto p = coek::parameter("p").value(2.0);
                auto w = coek::variable("w").lower(0).upper(1).value(3);
                auto e = coek::pow(w, p);

                static std::list<std::string> baseline
                    = {"[", "pow", "w", std::to_string(2.0), "]"};
                REQUIRE(simplify_expr(e.repn)->to_list() == baseline);
            }
        }
        WHEN("general")
        {
            {
                auto w = coek::variable("w").lower(0).upper(1).value(3);
                auto e = coek::pow(w, 2 + w);

                static std::list<std::string> baseline
                    = {"[", "pow", "w", "[", "+", "w", std::to_string(2.0), "]", "]"};
                REQUIRE(simplify_expr(e.repn)->to_list() == baseline);
            }
        }
    }
}
