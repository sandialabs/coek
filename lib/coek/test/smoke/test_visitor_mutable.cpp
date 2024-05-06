
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

#define MV_INTRINSIC_TEST1(FN)                                                            \
    WHEN(#FN)                                                                             \
    {                                                                                     \
        coek::Model m;                                                                    \
        auto v = m.add_variable("v").lower(0).upper(1).value(0).fixed(true);              \
        coek::Expression e = FN(v + 1);                                                   \
        mutable_values(e.repn, fixed_vars, params, visited_subexpressions);               \
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
        mutable_values(e.repn, fixed_vars, params, visited_subexpressions);               \
        static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{v.repn}; \
        static std::unordered_set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};      \
        REQUIRE(fixed_vars == vbaseline);                                                 \
        REQUIRE(params == pbaseline);                                                     \
    }

TEST_CASE("mutable_values", "[smoke]")
{
    std::unordered_set<std::shared_ptr<coek::VariableTerm>> fixed_vars;
    std::unordered_set<std::shared_ptr<coek::ParameterTerm>> params;
    std::unordered_set<std::shared_ptr<coek::SubExpressionTerm>> visited_subexpressions;

    SECTION("constant")
    {
        coek::Expression e(3);
        mutable_values(e.repn, fixed_vars, params, visited_subexpressions);

        static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{};
        static std::unordered_set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};
        REQUIRE(fixed_vars == vbaseline);
        REQUIRE(params == pbaseline);
    }

    SECTION("param")
    {
        auto p = coek::parameter().value(3);
        coek::Expression e = p;
        mutable_values(e.repn, fixed_vars, params, visited_subexpressions);

        static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{};
        static std::unordered_set<std::shared_ptr<coek::ParameterTerm>> pbaseline{p.repn};
        REQUIRE(fixed_vars == vbaseline);
        REQUIRE(params == pbaseline);
    }

    SECTION("indexparam")
    {
        auto p = coek::set_element("p");
        coek::Expression e = p;
        mutable_values(e.repn, fixed_vars, params, visited_subexpressions);

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
            mutable_values(e.repn, fixed_vars, params, visited_subexpressions);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{v.repn};
            static std::unordered_set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};
            REQUIRE(fixed_vars == vbaseline);
            REQUIRE(params == pbaseline);
        }

        WHEN("same")
        {
            auto v = coek::variable();
            coek::Expression e = v;
            mutable_values(e.repn, fixed_vars, params, visited_subexpressions);

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
            mutable_values(e.repn, fixed_vars, params, visited_subexpressions);

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
            mutable_values(e.repn, fixed_vars, params, visited_subexpressions);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{v.repn};
            static std::unordered_set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};
            REQUIRE(fixed_vars == vbaseline);
            REQUIRE(params == pbaseline);
        }
    }

    SECTION("subexpression")
    {
        WHEN("shared expression")
        {
            auto v = coek::variable("v");
            v.fixed(true);
            auto E = coek::subexpression().value(v + 1);
            coek::Expression e = E + 2 * (E + 1);
            mutable_values(e.repn, fixed_vars, params, visited_subexpressions);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{v.repn};
            static std::unordered_set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};
            REQUIRE(fixed_vars == vbaseline);
            REQUIRE(params == pbaseline);
        }

#ifdef DEBUG
        WHEN("debug walker0")
        {
            auto v = coek::variable("v");
            v.fixed(true);
            auto E = v + 1;
            coek::Expression e = E + 2 * (E + 1);
            size_t num_visits = 0;
            mutable_values_debug(e.repn, fixed_vars, params, visited_subexpressions, num_visits);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{v.repn};
            static std::unordered_set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};
            REQUIRE(fixed_vars == vbaseline);
            REQUIRE(params == pbaseline);
            REQUIRE(num_visits == 10);
        }

        WHEN("debug walker1")
        {
            auto v = coek::variable("v");
            v.fixed(true);
            auto E = coek::subexpression().value(v + 1);
            coek::Expression e = E + 2 * (E + 1);
            size_t num_visits = 0;

            mutable_values_debug(e.repn, fixed_vars, params, visited_subexpressions, num_visits);
            REQUIRE(num_visits == 10);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{v.repn};
            static std::unordered_set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};
            REQUIRE(fixed_vars == vbaseline);
            REQUIRE(params == pbaseline);
        }

        WHEN("debug walker2")
        {
            auto v = coek::variable("v");
            v.fixed(true);
            auto E = coek::subexpression().value(v + 1);
            coek::Expression e1 = E + 2 * (E + 1);
            coek::Expression e2 = E + 2 * (E + 1);

            size_t num_visits = 0;
            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{v.repn};
            static std::unordered_set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};

            mutable_values_debug(e1.repn, fixed_vars, params, visited_subexpressions, num_visits);
            REQUIRE(fixed_vars == vbaseline);
            REQUIRE(params == pbaseline);
            REQUIRE(num_visits == 10);

            mutable_values_debug(e2.repn, fixed_vars, params, visited_subexpressions, num_visits);
            REQUIRE(fixed_vars == vbaseline);
            REQUIRE(params == pbaseline);
            REQUIRE(num_visits == 7);
        }
#endif
    }

    SECTION("plus")
    {
        WHEN("linear")
        {
            auto v = coek::variable("v").lower(0).upper(1).value(0);
            v.fixed(true);
            coek::Expression e = 2 * (v + v) + v;
            mutable_values(e.repn, fixed_vars, params, visited_subexpressions);

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
            mutable_values(e.repn, fixed_vars, params, visited_subexpressions);

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
            mutable_values(e.repn, fixed_vars, params, visited_subexpressions);

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
            mutable_values(e.repn, fixed_vars, params, visited_subexpressions);

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
            mutable_values(e.repn, fixed_vars, params, visited_subexpressions);

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
            mutable_values(e.repn, fixed_vars, params, visited_subexpressions);

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
            mutable_values(e.repn, fixed_vars, params, visited_subexpressions);

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
            mutable_values(e.repn, fixed_vars, params, visited_subexpressions);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{w.repn};
            static std::unordered_set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};
            REQUIRE(fixed_vars == vbaseline);
            REQUIRE(params == pbaseline);
        }
    }

    SECTION("if_else")
    {
        WHEN("parameter")
        {
            coek::Model m;
            auto p = coek::parameter();
            auto w = m.add_variable("w").lower(0).upper(1).value(0).fixed(true);
            auto v = m.add_variable("v").lower(0).upper(1).value(0).fixed(true);
            coek::Expression e = if_else(p, w, v);
            mutable_values(e.repn, fixed_vars, params, visited_subexpressions);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{w.repn,
                                                                                     v.repn};
            static std::unordered_set<std::shared_ptr<coek::ParameterTerm>> pbaseline{p.repn};
            REQUIRE(fixed_vars == vbaseline);
            REQUIRE(params == pbaseline);
        }
        WHEN("conditional")
        {
            coek::Model m;
            auto p = coek::parameter();
            auto w = m.add_variable("w").lower(0).upper(1).value(0).fixed(true);
            auto v = m.add_variable("v").lower(0).upper(1).value(0).fixed(true);
            coek::Expression e = if_else(p > 0, w, v);
            mutable_values(e.repn, fixed_vars, params, visited_subexpressions);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{w.repn,
                                                                                     v.repn};
            static std::unordered_set<std::shared_ptr<coek::ParameterTerm>> pbaseline{p.repn};
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
            mutable_values(e.repn, fixed_vars, params, visited_subexpressions);

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
            mutable_values(e.repn, fixed_vars, params, visited_subexpressions);

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
            mutable_values(e.repn, fixed_vars, params, visited_subexpressions);

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
            mutable_values(e.repn, fixed_vars, params, visited_subexpressions);

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
        mutable_values(o.expr().repn, fixed_vars, params, visited_subexpressions);

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
            mutable_values(e.repn, fixed_vars, params, visited_subexpressions);

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
            mutable_values(e.repn, fixed_vars, params, visited_subexpressions);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{v.repn,
                                                                                     w.repn};
            static std::unordered_set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};
            REQUIRE(fixed_vars == vbaseline);
            REQUIRE(params == pbaseline);
        }
    }
}
