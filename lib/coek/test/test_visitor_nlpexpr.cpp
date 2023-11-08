
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

#define NLP_INTRINSIC_TEST1(FN)                                                   \
    WHEN(#FN " 1")                                                                \
    {                                                                             \
        {                                                                         \
            coek::Model m;                                                        \
            auto v = m.add_variable("v").lower(0).upper(1).value(0);              \
            coek::Expression e = FN(v + 1);                                       \
            coek::MutableNLPExpr repn;                                            \
            repn.collect_terms(e);                                                \
            static std::list<std::string> constval = {std::to_string(0.0)};       \
            static std::list<std::string> nonlinear                               \
                = {"[", #FN, "[", "+", "v", std::to_string(1.0), "]", "]"};       \
            REQUIRE(repn.constval->to_list() == constval);                        \
            REQUIRE(repn.linear_coefs.size() + repn.quadratic_coefs.size() == 0); \
            REQUIRE(repn.nonlinear_vars.size() == 1);                             \
        }                                                                         \
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
            REQUIRE(repn.nonlinear_vars.size() == 0);                             \
        }                                                                         \
    }

#define NLP_INTRINSIC_TEST2(FN)                                                   \
    WHEN(#FN " 1")                                                                \
    {                                                                             \
        {                                                                         \
            coek::Model m;                                                        \
            auto v = m.add_variable("v").lower(0).upper(1).value(0);              \
            auto w = m.add_variable("w").lower(0).upper(1).value(0);              \
            coek::Expression e = FN(v + 1, w);                                    \
            coek::MutableNLPExpr repn;                                            \
            repn.collect_terms(e);                                                \
            static std::list<std::string> constval = {std::to_string(0.0)};       \
            static std::list<std::string> nonlinear                               \
                = {"[", #FN, "[", "+", "v", std::to_string(1.0), "]", "w", "]"};  \
            REQUIRE(repn.constval->to_list() == constval);                        \
            REQUIRE(repn.linear_coefs.size() + repn.quadratic_coefs.size() == 0); \
            REQUIRE(repn.nonlinear_vars.size() == 2);                             \
        }                                                                         \
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
            REQUIRE(repn.nonlinear_vars.size() == 0);                             \
        }                                                                         \
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
            REQUIRE(repn.nonlinear_vars.size() == 0);
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
                REQUIRE(repn.nonlinear_vars.size() == 0);
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
                REQUIRE(repn.nonlinear_vars.size() == 0);
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
                REQUIRE(repn.nonlinear_vars.size() == 0);
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
                REQUIRE(repn.nonlinear_vars.size() == 0);
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
                REQUIRE(repn.nonlinear_vars.size() == 0);
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
                REQUIRE(repn.nonlinear_vars.size() == 0);
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
                REQUIRE(repn.nonlinear_vars.size() == 0);
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
            REQUIRE(repn.nonlinear_vars.size() == 0);
        }
    }

    SECTION("subexpression")
    {
        WHEN("linear")
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
            REQUIRE(repn.nonlinear_vars.size() == 0);
        }
        WHEN("nonlinear")
        {
            coek::Model m;
            auto v = m.add_variable("v").lower(0).upper(1).value(3);
            auto w = m.add_variable("v").lower(0).upper(1).value(1);
            auto E = coek::subexpression().value(2 * v + w + 1);
            coek::Expression e = E + (E + 1) * (E + 1);
            REQUIRE(e.value() == 89);

            coek::MutableNLPExpr repn;
            repn.collect_terms(e);

            static std::list<std::string> constval = {std::to_string(1.0)};
            static std::list<std::string> coefval0 = {std::to_string(2.0)};
            static std::list<std::string> coefval1 = {std::to_string(1.0)};
            REQUIRE(repn.mutable_values == false);
            REQUIRE(repn.constval->to_list() == constval);
            REQUIRE(repn.linear_coefs.size() == 2);
            REQUIRE(repn.linear_coefs[0]->to_list() == coefval0);
            REQUIRE(repn.linear_coefs[1]->to_list() == coefval1);
            REQUIRE(repn.linear_vars[0] == v.repn);
            REQUIRE(repn.linear_vars[1] == w.repn);
            REQUIRE(repn.quadratic_coefs.size() == 0);
            REQUIRE(repn.nonlinear_vars.size() == 2);
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
                REQUIRE(repn.nonlinear_vars.size() == 0);
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
                REQUIRE(repn.nonlinear_vars.size() == 0);
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
                REQUIRE(repn.nonlinear_vars.size() == 0);
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
                REQUIRE(repn.nonlinear_vars.size() == 0);
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
                REQUIRE(repn.nonlinear_vars.size() == 0);
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
                REQUIRE(repn.nonlinear_vars.size() == 2);
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
                REQUIRE(repn.nonlinear_vars.size() == 0);
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
                REQUIRE(repn.nonlinear_vars.size() == 0);
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

                REQUIRE(repn.nonlinear_vars.size() == 0);
            }
        }
        WHEN("complex quadratic 4")
        {
            {
                coek::Model m;
                auto w = m.add_variable("w").lower(0).upper(1).value(0);
                coek::Expression e = (w - 3) * (w - 3);
                coek::MutableNLPExpr repn;
                repn.collect_terms(e);

                static std::list<std::string> constval = {std::to_string(9.0)};
                static std::list<std::string> lcoefval = {std::to_string(-3.0)};
                static std::list<std::string> qcoefval = {std::to_string(1.0)};
                REQUIRE(repn.constval->to_list() == constval);

                REQUIRE(repn.linear_coefs.size() == 2);
                REQUIRE(repn.linear_coefs[0]->to_list() == lcoefval);
                REQUIRE(repn.linear_coefs[1]->to_list() == lcoefval);

                REQUIRE(repn.quadratic_coefs.size() == 1);
                REQUIRE(repn.quadratic_coefs[0]->to_list() == qcoefval);
                REQUIRE(repn.quadratic_lvars[0] == w.repn);
                REQUIRE(repn.quadratic_rvars[0] == w.repn);

                REQUIRE(repn.nonlinear_vars.size() == 0);
            }
        }
        WHEN("complex quadratic 5")
        {
            {
                coek::Model m;
                auto w = m.add_variable("w").lower(0).upper(1).value(0);
                coek::Expression e = (w - 3) * (w - 3) + (w - 5) * (w - 5);
                coek::MutableNLPExpr repn;
                repn.collect_terms(e);

                static std::list<std::string> constval = {std::to_string(34.0)};
                static std::list<std::string> lcoefval1 = {std::to_string(-3.0)};
                static std::list<std::string> lcoefval2 = {std::to_string(-5.0)};
                static std::list<std::string> qcoefval = {std::to_string(1.0)};
                REQUIRE(repn.constval->to_list() == constval);

                REQUIRE(repn.linear_coefs.size() == 4);
                REQUIRE(repn.linear_coefs[0]->to_list() == lcoefval1);
                REQUIRE(repn.linear_coefs[1]->to_list() == lcoefval1);
                REQUIRE(repn.linear_coefs[2]->to_list() == lcoefval2);
                REQUIRE(repn.linear_coefs[3]->to_list() == lcoefval2);

                REQUIRE(repn.quadratic_coefs.size() == 2);
                REQUIRE(repn.quadratic_coefs[0]->to_list() == qcoefval);
                REQUIRE(repn.quadratic_coefs[1]->to_list() == qcoefval);
                REQUIRE(repn.quadratic_lvars[0] == w.repn);
                REQUIRE(repn.quadratic_rvars[0] == w.repn);

                REQUIRE(repn.nonlinear_vars.size() == 0);
            }
        }
        WHEN("complex nonlinear")
        {
            {
                // Force use of ceil and floor functions within a nested product.
                // Force expression of multiplication between constant parameter and quadratic term
                // Force inclusion of multipliers in nonlinear terms
                coek::Model m;
                auto w = m.add_variable("w").lower(0).upper(1).value(0);
                auto p = coek::parameter("p");
                coek::Expression e = ceil(w) * floor(w) + p * (w * w) - floor(w) * floor(w);
                coek::MutableNLPExpr repn;
                repn.collect_terms(e);

                REQUIRE(repn.linear_coefs.size() == 0);
                REQUIRE(repn.quadratic_coefs.size() == 1);
                static std::list<std::string> baseline
                    = {"[", "+",     "[", "*",     "[", "ceil", "w", "]",
                       "[", "floor", "w", "]",     "]", "[",    "*", std::to_string(-1.0),
                       "[", "*",     "[", "floor", "w", "]",    "[", "floor",
                       "w", "]",     "]", "]",     "]"};
                REQUIRE(repn.nonlinear->to_list() == baseline);
                REQUIRE(repn.nonlinear_vars.size() == 1);
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
                REQUIRE(repn.nonlinear_vars.size() == 1);
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
                REQUIRE(repn.nonlinear_vars.size() == 0);
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
                REQUIRE(repn.nonlinear_vars.size() == 0);
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
                REQUIRE(repn.nonlinear_vars.size() == 0);
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
                REQUIRE(repn.nonlinear_vars.size() == 1);
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
                REQUIRE(repn.nonlinear_vars.size() == 0);
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
                REQUIRE(repn.nonlinear_vars.size() == 0);
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
                REQUIRE(repn.nonlinear_vars.size() == 0);
            }
        }
    }

    SECTION("intrinsic funcs")
    {
        NLP_INTRINSIC_TEST1(abs)
        NLP_INTRINSIC_TEST1(ceil)
        NLP_INTRINSIC_TEST1(floor)
        NLP_INTRINSIC_TEST1(exp)
        NLP_INTRINSIC_TEST1(log)
        NLP_INTRINSIC_TEST1(log10)
        NLP_INTRINSIC_TEST1(sqrt)
        NLP_INTRINSIC_TEST1(sin)
        NLP_INTRINSIC_TEST1(cos)
        NLP_INTRINSIC_TEST1(tan)
        NLP_INTRINSIC_TEST1(sinh)
        NLP_INTRINSIC_TEST1(cosh)
        NLP_INTRINSIC_TEST1(tanh)
        NLP_INTRINSIC_TEST1(asin)
        NLP_INTRINSIC_TEST1(acos)
        NLP_INTRINSIC_TEST1(atan)
        NLP_INTRINSIC_TEST1(asinh)
        NLP_INTRINSIC_TEST1(acosh)
        NLP_INTRINSIC_TEST1(atanh)
        NLP_INTRINSIC_TEST2(pow)
    }
}
