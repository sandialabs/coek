
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

#define FVP_INTRINSIC_TEST1(FN)                                                         \
    WHEN(#FN)                                                                           \
    {                                                                                   \
        coek::Model m;                                                                  \
        auto v = m.add_variable("v").lower(0).upper(1).value(0).fixed(true);            \
        coek::Expression e = FN(v + 1);                                                 \
        find_vars_and_params(e.repn, vars, fixed_vars, params, visited_subexpressions); \
        static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{};     \
        static std::set<std::shared_ptr<coek::VariableTerm>> fvbaseline{v.repn};        \
        static std::set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};              \
        REQUIRE(vars == vbaseline);                                                     \
        REQUIRE(fixed_vars == fvbaseline);                                              \
        REQUIRE(params == pbaseline);                                                   \
    }

#define FVP_INTRINSIC_TEST2(FN)                                                           \
    WHEN(#FN)                                                                             \
    {                                                                                     \
        coek::Model m;                                                                    \
        auto w = m.add_variable("w").lower(0).upper(1).value(0);                          \
        auto v = m.add_variable("v").lower(0).upper(1).value(0).fixed(true);              \
        coek::Expression e = FN(v + 1, w);                                                \
        find_vars_and_params(e.repn, vars, fixed_vars, params, visited_subexpressions);   \
        static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{w.repn}; \
        static std::set<std::shared_ptr<coek::VariableTerm>> fvbaseline{v.repn};          \
        static std::set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};                \
        REQUIRE(vars == vbaseline);                                                       \
        REQUIRE(fixed_vars == fvbaseline);                                                \
        REQUIRE(params == pbaseline);                                                     \
    }

TEST_CASE("find_vars_and_params", "[smoke]")
{
    std::unordered_set<std::shared_ptr<coek::VariableTerm>> vars;
    std::set<std::shared_ptr<coek::VariableTerm>> fixed_vars;
    std::set<std::shared_ptr<coek::ParameterTerm>> params;
    std::set<std::shared_ptr<coek::SubExpressionTerm>> visited_subexpressions;

    SECTION("constant")
    {
        coek::Expression e(3);
        find_vars_and_params(e.repn, vars, fixed_vars, params, visited_subexpressions);

        static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{};
        static std::set<std::shared_ptr<coek::VariableTerm>> fvbaseline{};
        static std::set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};
        REQUIRE(vars == vbaseline);
        REQUIRE(fixed_vars == fvbaseline);
        REQUIRE(params == pbaseline);
    }

    SECTION("param")
    {
        auto p = coek::parameter().value(3);
        coek::Expression e = p;
        find_vars_and_params(e.repn, vars, fixed_vars, params, visited_subexpressions);

        static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{};
        static std::set<std::shared_ptr<coek::VariableTerm>> fvbaseline{};
        static std::set<std::shared_ptr<coek::ParameterTerm>> pbaseline{p.repn};
        REQUIRE(vars == vbaseline);
        REQUIRE(fixed_vars == fvbaseline);
        REQUIRE(params == pbaseline);
    }

    SECTION("indexparam")
    {
        auto p = coek::set_element("p");
        coek::Expression e = p;
        find_vars_and_params(e.repn, vars, fixed_vars, params, visited_subexpressions);

        static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{};
        static std::set<std::shared_ptr<coek::VariableTerm>> fvbaseline{};
        static std::set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};
        REQUIRE(vars == vbaseline);
        REQUIRE(fixed_vars == fvbaseline);
        REQUIRE(params == pbaseline);
    }

    SECTION("var")
    {
        WHEN("fixed")
        {
            auto v = coek::variable();
            v.fixed(true);
            coek::Expression e = v;
            find_vars_and_params(e.repn, vars, fixed_vars, params, visited_subexpressions);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{};
            static std::set<std::shared_ptr<coek::VariableTerm>> fvbaseline{v.repn};
            static std::set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};
            REQUIRE(vars == vbaseline);
            REQUIRE(fixed_vars == fvbaseline);
            REQUIRE(params == pbaseline);
        }

        WHEN("same")
        {
            auto v = coek::variable();
            coek::Expression e = v;
            find_vars_and_params(e.repn, vars, fixed_vars, params, visited_subexpressions);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{v.repn};
            static std::set<std::shared_ptr<coek::VariableTerm>> fvbaseline{};
            static std::set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};
            REQUIRE(vars == vbaseline);
            REQUIRE(fixed_vars == fvbaseline);
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
            find_vars_and_params(e.repn, vars, fixed_vars, params, visited_subexpressions);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{w.repn};
            static std::set<std::shared_ptr<coek::VariableTerm>> fvbaseline{};
            static std::set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};
            REQUIRE(vars == vbaseline);
            REQUIRE(fixed_vars == fvbaseline);
            REQUIRE(params == pbaseline);
        }

        WHEN("fixed")
        {
            coek::Model m;
            auto v = m.add_variable("v").lower(0).upper(1).value(0);
            v.fixed(true);
            coek::Expression e = 2 * v;
            find_vars_and_params(e.repn, vars, fixed_vars, params, visited_subexpressions);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{};
            static std::set<std::shared_ptr<coek::VariableTerm>> fvbaseline{v.repn};
            static std::set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};
            REQUIRE(vars == vbaseline);
            REQUIRE(fixed_vars == fvbaseline);
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
            find_vars_and_params(e.repn, vars, fixed_vars, params, visited_subexpressions);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{};
            static std::set<std::shared_ptr<coek::VariableTerm>> fvbaseline{v.repn};
            static std::set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};
            REQUIRE(vars == vbaseline);
            REQUIRE(fixed_vars == fvbaseline);
            REQUIRE(params == pbaseline);
        }

        WHEN("debug walker0")
        {
#ifdef DEBUG
            auto v = coek::variable("v");
            auto E = v + 1;
            coek::Expression e = E + 2 * (E + 1);
            size_t num_visits = 0;
            find_vars_and_params_debug(e.repn, vars, fixed_vars, params, visited_subexpressions,
                                       num_visits);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{v.repn};
            static std::set<std::shared_ptr<coek::VariableTerm>> fvbaseline{};
            static std::set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};
            REQUIRE(vars == vbaseline);
            REQUIRE(fixed_vars == fvbaseline);
            REQUIRE(params == pbaseline);
            REQUIRE(num_visits == 10);
#endif
        }

        WHEN("debug walker1")
        {
#ifdef DEBUG
            auto v = coek::variable("v");
            v.fixed(true);
            auto E = coek::subexpression().value(v + 1);
            coek::Expression e = E + 2 * (E + 1);
            size_t num_visits = 0;

            find_vars_and_params_debug(e.repn, vars, fixed_vars, params, visited_subexpressions,
                                       num_visits);
            REQUIRE(num_visits == 10);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{};
            static std::set<std::shared_ptr<coek::VariableTerm>> fvbaseline{v.repn};
            static std::set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};
            REQUIRE(vars == vbaseline);
            REQUIRE(fixed_vars == fvbaseline);
            REQUIRE(params == pbaseline);
#endif
        }

        WHEN("debug walker2")
        {
#ifdef DEBUG
            auto v = coek::variable("v");
            v.fixed(true);
            auto E = coek::subexpression().value(v + 1);
            coek::Expression e1 = E + 2 * (E + 1);
            coek::Expression e2 = E + 2 * (E + 1);

            size_t num_visits = 0;
            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{};
            static std::set<std::shared_ptr<coek::VariableTerm>> fvbaseline{v.repn};
            static std::set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};

            find_vars_and_params_debug(e1.repn, vars, fixed_vars, params, visited_subexpressions,
                                       num_visits);
            REQUIRE(vars == vbaseline);
            REQUIRE(fixed_vars == fvbaseline);
            REQUIRE(params == pbaseline);
            REQUIRE(num_visits == 10);

            find_vars_and_params_debug(e2.repn, vars, fixed_vars, params, visited_subexpressions,
                                       num_visits);
            REQUIRE(vars == vbaseline);
            REQUIRE(fixed_vars == fvbaseline);
            REQUIRE(params == pbaseline);
            REQUIRE(num_visits == 7);
#endif
        }
    }

    SECTION("plus")
    {
        WHEN("linear")
        {
            auto v = coek::variable("v").lower(0).upper(1).value(0);
            v.fixed(true);
            coek::Expression e = 2 * (v + v) + v;
            find_vars_and_params(e.repn, vars, fixed_vars, params, visited_subexpressions);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{};
            static std::set<std::shared_ptr<coek::VariableTerm>> fvbaseline{v.repn};
            static std::set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};
            REQUIRE(vars == vbaseline);
            REQUIRE(fixed_vars == fvbaseline);
            REQUIRE(params == pbaseline);
        }
        WHEN("simple")
        {
            auto p = coek::parameter();
            auto v = coek::variable("v").lower(0).upper(1).value(0);
            v.fixed(true);
            coek::Expression e = 3 * p + 2 * v;
            find_vars_and_params(e.repn, vars, fixed_vars, params, visited_subexpressions);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{};
            static std::set<std::shared_ptr<coek::VariableTerm>> fvbaseline{v.repn};
            static std::set<std::shared_ptr<coek::ParameterTerm>> pbaseline{p.repn};
            REQUIRE(vars == vbaseline);
            REQUIRE(fixed_vars == fvbaseline);
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
            find_vars_and_params(e.repn, vars, fixed_vars, params, visited_subexpressions);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{};
            static std::set<std::shared_ptr<coek::VariableTerm>> fvbaseline{v.repn};
            static std::set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};
            REQUIRE(vars == vbaseline);
            REQUIRE(fixed_vars == fvbaseline);
            REQUIRE(params == pbaseline);
        }
    }

    SECTION("times")
    {
        WHEN("lhs constant")
        {
            coek::Model m;
            auto p = coek::parameter().value(2);
            auto v = m.add_variable("v").lower(0).upper(1).value(0);
            coek::Expression e = p * v;
            find_vars_and_params(e.repn, vars, fixed_vars, params, visited_subexpressions);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{v.repn};
            static std::set<std::shared_ptr<coek::VariableTerm>> fvbaseline{};
            static std::set<std::shared_ptr<coek::ParameterTerm>> pbaseline{p.repn};
            REQUIRE(vars == vbaseline);
            REQUIRE(fixed_vars == fvbaseline);
            REQUIRE(params == pbaseline);
        }
        WHEN("simple quadratic LHS")
        {
            coek::Model m;
            auto v = m.add_variable("v").lower(0).upper(1).value(0).fixed(true);
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression e = v * w;
            find_vars_and_params(e.repn, vars, fixed_vars, params, visited_subexpressions);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{w.repn};
            static std::set<std::shared_ptr<coek::VariableTerm>> fvbaseline{v.repn};
            static std::set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};
            REQUIRE(vars == vbaseline);
            REQUIRE(fixed_vars == fvbaseline);
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
            find_vars_and_params(e.repn, vars, fixed_vars, params, visited_subexpressions);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{};
            static std::set<std::shared_ptr<coek::VariableTerm>> fvbaseline{w.repn};
            static std::set<std::shared_ptr<coek::ParameterTerm>> pbaseline{p.repn};
            REQUIRE(vars == vbaseline);
            REQUIRE(fixed_vars == fvbaseline);
            REQUIRE(params == pbaseline);
        }
        WHEN("rhs nonzero")
        {
            coek::Model m;
            auto p = coek::parameter().value(2);
            auto w = m.add_variable("w").lower(0).upper(1).value(0).fixed(true);
            coek::Expression e = w / p;
            find_vars_and_params(e.repn, vars, fixed_vars, params, visited_subexpressions);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{};
            static std::set<std::shared_ptr<coek::VariableTerm>> fvbaseline{w.repn};
            static std::set<std::shared_ptr<coek::ParameterTerm>> pbaseline{p.repn};
            REQUIRE(vars == vbaseline);
            REQUIRE(fixed_vars == fvbaseline);
            REQUIRE(params == pbaseline);
        }
        WHEN("rhs polynomial")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression e = w / (1 + w);
            find_vars_and_params(e.repn, vars, fixed_vars, params, visited_subexpressions);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{w.repn};
            static std::set<std::shared_ptr<coek::VariableTerm>> fvbaseline{};
            static std::set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};
            REQUIRE(vars == vbaseline);
            REQUIRE(fixed_vars == fvbaseline);
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
            w.fixed(true);
            find_vars_and_params(e.repn, vars, fixed_vars, params, visited_subexpressions);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{v.repn};
            static std::set<std::shared_ptr<coek::VariableTerm>> fvbaseline{w.repn};
            static std::set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};
            REQUIRE(vars == vbaseline);
            REQUIRE(fixed_vars == fvbaseline);
            REQUIRE(params == pbaseline);
        }
        WHEN("variable partial plus monomial - 2")
        {
            coek::Model m;
            auto v = m.add_variable("v").lower(0).upper(1).value(0);
            auto w = m.add_variable("w").lower(0).upper(1).value(0).fixed(true);
            coek::Expression e = v * (2 * w + 1);
            find_vars_and_params(e.repn, vars, fixed_vars, params, visited_subexpressions);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{v.repn};
            static std::set<std::shared_ptr<coek::VariableTerm>> fvbaseline{w.repn};
            static std::set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};
            REQUIRE(vars == vbaseline);
            REQUIRE(fixed_vars == fvbaseline);
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
            find_vars_and_params(e.repn, vars, fixed_vars, params, visited_subexpressions);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{v.repn};
            static std::set<std::shared_ptr<coek::VariableTerm>> fvbaseline{w.repn};
            static std::set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};
            REQUIRE(vars == vbaseline);
            REQUIRE(fixed_vars == fvbaseline);
            REQUIRE(params == pbaseline);
        }
        WHEN("negative monomial")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            w.fixed(true);
            coek::Expression e = -(-w) + (-(-w));
            find_vars_and_params(e.repn, vars, fixed_vars, params, visited_subexpressions);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{};
            static std::set<std::shared_ptr<coek::VariableTerm>> fvbaseline{w.repn};
            static std::set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};
            REQUIRE(vars == vbaseline);
            REQUIRE(fixed_vars == fvbaseline);
            REQUIRE(params == pbaseline);
        }
    }

    SECTION("intrinsic funcs"){

        // clang-format off
        FVP_INTRINSIC_TEST1(abs)
        FVP_INTRINSIC_TEST1(ceil)
        FVP_INTRINSIC_TEST1(floor)
        FVP_INTRINSIC_TEST1(exp)
        FVP_INTRINSIC_TEST1(log)
        FVP_INTRINSIC_TEST1(log10)
        FVP_INTRINSIC_TEST1(sqrt)
        FVP_INTRINSIC_TEST1(sin)
        FVP_INTRINSIC_TEST1(cos)
        FVP_INTRINSIC_TEST1(tan)
        FVP_INTRINSIC_TEST1(sinh)
        FVP_INTRINSIC_TEST1(cosh)
        FVP_INTRINSIC_TEST1(tanh)
        FVP_INTRINSIC_TEST1(asin)
        FVP_INTRINSIC_TEST1(acos)
        FVP_INTRINSIC_TEST1(atan)
        FVP_INTRINSIC_TEST1(asinh)
        FVP_INTRINSIC_TEST1(acosh)
        FVP_INTRINSIC_TEST1(atanh)
        FVP_INTRINSIC_TEST2(pow)}
    // clang-format on

    SECTION("objective")
    {
        coek::Model m;
        auto w = m.add_variable("w").lower(0).upper(1).value(0);
        auto v = m.add_variable("v").lower(0).upper(1).value(0);
        auto o = m.add_objective(w * v + v * (2 * w + 1));
        find_vars_and_params(o.expr().repn, vars, fixed_vars, params, visited_subexpressions);

        static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{v.repn, w.repn};
        static std::set<std::shared_ptr<coek::VariableTerm>> fvbaseline{};
        static std::set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};
        REQUIRE(vars == vbaseline);
        REQUIRE(fixed_vars == fvbaseline);
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
            w.fixed(true);
            find_vars_and_params(e.repn, vars, fixed_vars, params, visited_subexpressions);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{v.repn};
            static std::set<std::shared_ptr<coek::VariableTerm>> fvbaseline{w.repn};
            static std::set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};
            REQUIRE(vars == vbaseline);
            REQUIRE(fixed_vars == fvbaseline);
            REQUIRE(params == pbaseline);
        }
        WHEN("equality")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            auto v = m.add_variable("v").lower(0).upper(1).value(0);
            auto e = w * v + v * (2 * w + 1) == 0;
            w.fixed(true);
            find_vars_and_params(e.repn, vars, fixed_vars, params, visited_subexpressions);

            static std::unordered_set<std::shared_ptr<coek::VariableTerm>> vbaseline{v.repn};
            static std::set<std::shared_ptr<coek::VariableTerm>> fvbaseline{w.repn};
            static std::set<std::shared_ptr<coek::ParameterTerm>> pbaseline{};
            REQUIRE(vars == vbaseline);
            REQUIRE(fixed_vars == fvbaseline);
            REQUIRE(params == pbaseline);
        }
    }
}
