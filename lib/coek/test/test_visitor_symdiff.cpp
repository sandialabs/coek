
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

    SECTION("subexpression")
    {
        auto v = coek::variable("v");
        auto E = coek::subexpression().value(v + 1);
        coek::Expression e = E + 2 * (E + v);
        auto ans = e.diff(v);
        static std::list<std::string> baseline = {std::to_string(5.0)};
        REQUIRE(ans.to_list() == baseline);
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
                = {"[", "*", std::to_string(2.0), "[", "exp", "[", "*", std::to_string(2.0), "w", "]", "]", "]"};
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
                                                      "[", "*", std::to_string(2.0),
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
                                                      "[", "*", std::to_string(2.0),
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
                = {"[", "*", std::to_string(2.0),    "[", "pow", "[", "*", std::to_string(2.0),
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
                = {"[", "*", std::to_string(2.0), "[", "cos", "[", "*", std::to_string(2.0), "w", "]", "]", "]"};
            REQUIRE(e.to_list() == baseline);
        }
        WHEN("cos")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = cos(2 * w);
            auto e = f.diff(w);
            static std::list<std::string> baseline = {
                "[", "*", std::to_string(2.0), "[", "-", "[", "sin", "[", "*", std::to_string(2.0), "w", "]", "]",
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
                                                      std::to_string(2.0),
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
                = {"[", "*", std::to_string(2.0), "[", "cosh", "[", "*", std::to_string(2.0), "w", "]", "]", "]"};
            REQUIRE(e.to_list() == baseline);
        }
        WHEN("cosh")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = cosh(2 * w);
            auto e = f.diff(w);
            static std::list<std::string> baseline
                = {"[", "*", std::to_string(2.0), "[", "sinh", "[", "*", std::to_string(2.0), "w", "]", "]", "]"};
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
                                                      std::to_string(2.0),
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
                                                      std::to_string(2.0),
                                                      "w",
                                                      "]",
                                                      "[",
                                                      "*",
                                                      std::to_string(2.0),
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
                                                      std::to_string(2.0),
                                                      "w",
                                                      "]",
                                                      "[",
                                                      "*",
                                                      std::to_string(2.0),
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
                                                      "*", std::to_string(2.0), "w",
                                                      "]", "[", "*",
                                                      std::to_string(2.0), "w", "]",
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
                                                      std::to_string(2.0),
                                                      "w",
                                                      "]",
                                                      "[",
                                                      "*",
                                                      std::to_string(2.0),
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
                                                      "[", "*",    
                                                      std::to_string(2.0),
                                                      "w", "]",    "[",
                                                      "*", std::to_string(2.0),    "w",
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
                                                      std::to_string(2.0),
                                                      "w",
                                                      "]",
                                                      "[",
                                                      "*",
                                                      std::to_string(2.0),
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
                                                      std::to_string(2.0),
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
                                                      "[", "*",   std::to_string(2.0),
                                                      "w", "]",   "]",
                                                      "]", "]"};
            REQUIRE(e.to_list() == baseline);
        }
    }
}
