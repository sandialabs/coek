
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

    SECTION("subexpression")
    {
        auto v = coek::variable("v");
        auto E = coek::subexpression().value(v + 1);
        coek::Expression e = E + 2 * (E + 1);
        std::stringstream sstr;
        sstr << e;
        REQUIRE(sstr.str() == "v + 1 + (2)*(v + 1 + 1)");
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
