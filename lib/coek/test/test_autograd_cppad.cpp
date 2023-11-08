#include <iostream>

#include "catch2/catch.hpp"
#include "coek/ast/base_terms.hpp"
#include "coek/coek.hpp"

const double PI = 3.141592653589793238463;
const double E = exp(1.0);

#define ADNAME "cppad"

TEST_CASE("autograd_cppad", "[smoke]")
{

    SECTION("dense_j")
    {
        WHEN("nx < nc")
        {
            coek::Model model;
            auto a = model.add_variable("a");
            auto b = model.add_variable("b");

            model.add_objective(a);
            model.add_constraint(a <= 0);
            model.add_constraint(a * b <= 0);
            model.add_constraint(b <= 0);

            coek::NLPModel nlp;
            nlp.set_option("sparse_JH", false);
            nlp.initialize(model, ADNAME);
            REQUIRE(nlp.num_nonzeros_Jacobian() == 6);

            std::vector<double> x{0, 1};
            std::vector<double> j(nlp.num_nonzeros_Jacobian());
            nlp.compute_J(x, j);
            REQUIRE(j[0] == 1);
            REQUIRE(j[1] == 0);
            REQUIRE(j[2] == 1);
            REQUIRE(j[3] == 0);
            REQUIRE(j[4] == 0);
            REQUIRE(j[5] == 1);
        }

        WHEN("nx > nc")
        {
            coek::Model model;
            auto a = model.add_variable("a");
            auto b = model.add_variable("b");
            auto c = model.add_variable("c");

            model.add_objective(a);
            model.add_constraint(a + a * b + b <= 0);
            model.add_constraint(b + b * c + c <= 0);

            coek::NLPModel nlp;
            nlp.set_option("sparse_JH", false);
            nlp.initialize(model, ADNAME);
            REQUIRE(nlp.num_nonzeros_Jacobian() == 6);

            std::vector<double> x{0, 1, 2};
            std::vector<double> j(nlp.num_nonzeros_Jacobian());
            nlp.compute_J(x, j);
            REQUIRE(j[0] == 2);
            REQUIRE(j[1] == 1);
            REQUIRE(j[2] == 0);
            REQUIRE(j[3] == 0);
            REQUIRE(j[4] == 3);
            REQUIRE(j[5] == 2);
        }
    }

    SECTION("dense_h")
    {
        WHEN("nx < nc")
        {
            coek::Model model;
            auto a = model.add_variable("a");
            auto b = model.add_variable("b");

            model.add_objective(a * a + b);
            model.add_constraint(a <= 0);
            model.add_constraint(a * b <= 0);
            model.add_constraint(b <= 0);

            coek::NLPModel nlp;
            nlp.set_option("sparse_JH", false);
            nlp.initialize(model, ADNAME);
            REQUIRE(nlp.num_nonzeros_Hessian_Lagrangian() == 3);

            // H = [ [ 2, 1 ]
            //       [ 1, 0 ] ]
            std::vector<double> w{1, 1, 1, 1};
            std::vector<double> x{0, 1};
            std::vector<double> h(nlp.num_nonzeros_Hessian_Lagrangian());
            nlp.compute_H(x, w, h);
            REQUIRE(h[0] == 2);
            REQUIRE(h[1] == 1);
            REQUIRE(h[2] == 0);
        }

        WHEN("nx > nc")
        {
            coek::Model model;
            auto a = model.add_variable("a");
            auto b = model.add_variable("b");
            auto c = model.add_variable("c");
            auto d = model.add_variable("d");

            model.add_objective(d * d * c);
            model.add_constraint(a + a * b + b <= 0);
            model.add_constraint(b + b * c + c <= 0);

            coek::NLPModel nlp;
            nlp.set_option("sparse_JH", false);
            nlp.initialize(model, ADNAME);
            REQUIRE(nlp.num_nonzeros_Hessian_Lagrangian() == 10);

            // H = [ [ 0, 1, 0, 0 ]
            //       [ 1, 0, 1, 0 ]
            //       [ 0, 1, 0, 2d ]
            //       [ 0, 0, 2d, 2c ] ]
            std::vector<double> w{1, 1, 1};
            std::vector<double> x{0, 1, 2, 3};
            std::vector<double> h(nlp.num_nonzeros_Hessian_Lagrangian());
            nlp.compute_H(x, w, h);
            REQUIRE(h[0] == 0);
            REQUIRE(h[1] == 1);
            REQUIRE(h[2] == 0);
            REQUIRE(h[3] == 0);
            REQUIRE(h[4] == 1);
            REQUIRE(h[5] == 0);
            REQUIRE(h[6] == 0);
            REQUIRE(h[7] == 0);
            REQUIRE(h[8] == 6);
            REQUIRE(h[9] == 4);
        }
    }
}

#if 0
TEST_CASE("cppad_diff_tests", "[smoke]")
{
    // TODO - test constant expression

    SECTION("constant")
    {
        coek::Model model;
        coek::Expression f(3);
        auto v = model.add_variable("v");
        model.add_objective(f * v);
        coek::NLPModel nlp(model, ADNAME);

        std::vector<double> x{0};
        std::vector<double> baseline{3};
        std::vector<double> ans(1);
        nlp.compute_df(x, ans);
        REQUIRE(ans == baseline);
    }

    SECTION("param")
    {
        WHEN("simple")
        {
            coek::Model model;
            auto p = coek::parameter().value(3);
            coek::Expression f = p;
            auto v = model.add_variable("v");
            model.add_objective(f * v);
            coek::NLPModel nlp(model, ADNAME);

            std::vector<double> x{0};
            std::vector<double> baseline{3};
            std::vector<double> ans(1);
            nlp.compute_df(x, ans);
            REQUIRE(ans == baseline);

            p.value(4);
            nlp.reset();
            std::vector<double> baseline2{4};
            nlp.compute_df(x, ans);
            REQUIRE(ans == baseline2);
        }
        WHEN("many")
        {
            coek::Model model;
            auto p1 = coek::parameter().value(1);
            auto p2 = coek::parameter().value(2);
            auto p3 = coek::parameter().value(3);
            auto p4 = coek::parameter().value(4);
            auto p5 = coek::parameter().value(5);
            coek::Expression f = p1 + 2 * p2 + 3 * p3 + 4 * p4 + 5 * p5;
            auto v = model.add_variable("v");
            model.add_objective(f * v);
            coek::NLPModel nlp(model, ADNAME);

            std::vector<double> x{0};
            std::vector<double> baseline{1 + 4 + 9 + 16 + 25};
            std::vector<double> ans(1);
            nlp.compute_df(x, ans);
            REQUIRE(ans == baseline);

            p5.value(10);
            nlp.reset();
            std::vector<double> baseline2{1 + 4 + 9 + 16 + 50};
            nlp.compute_df(x, ans);
            REQUIRE(ans == baseline2);
        }
    }

    SECTION("var")
    {
        WHEN("fixed")
        {
            coek::Model model;
            auto v = model.add_variable("v").value(0);
            auto w = model.add_variable("w").value(0);
            v.fixed(true);
            coek::Expression f = v + 2 * w;
            model.add_objective(f);
            coek::NLPModel nlp(model, ADNAME);

            std::vector<double> x{0};
            std::vector<double> baseline{2};
            std::vector<double> ans(1);
            nlp.compute_df(x, ans);
            REQUIRE(ans == baseline);
        }

        WHEN("unfixed")
        {
            coek::Model model;
            auto v = model.add_variable("v");
            auto w = model.add_variable("w");
            coek::Expression f = v;
            model.add_objective(f);
            model.add_objective(w);
            coek::NLPModel nlp(model, ADNAME);

            std::vector<double> x{0, 0};
            std::vector<double> baseline{1, 0};
            std::vector<double> ans(2);
            nlp.compute_df(x, ans, 0);
            REQUIRE(ans == baseline);
            nlp.compute_df(x, ans, 1);
            std::vector<double> baseline2{0, 1};
            REQUIRE(ans == baseline2);
        }
    }

    SECTION("monomial")
    {
        WHEN("other")
        {
            coek::Model m;
            auto v = m.add_variable("v");
            auto w = m.add_variable("w");
            coek::Expression f = 2 * v;
            m.add_objective(f);
            coek::NLPModel nlp(m, ADNAME);

            std::vector<double> x{0};
            std::vector<double> baseline{2};
            std::vector<double> ans(1);
            nlp.compute_df(x, ans);
            REQUIRE(ans == baseline);
        }

        WHEN("fixed")
        {
            coek::Model m;
            auto v = m.add_variable("v").value(0);
            auto w = m.add_variable("w").value(0);
            v.fixed(true);
            coek::Expression f = 2 * v;
            m.add_objective(f + 3 * w);
            coek::NLPModel nlp(m, ADNAME);

            std::vector<double> x{0};
            std::vector<double> baseline{3};
            std::vector<double> ans(1);
            nlp.compute_df(x, ans);
            REQUIRE(ans == baseline);
        }
    }

    SECTION("plus")
    {
        WHEN("linear")
        {
            coek::Model m;
            auto p = coek::parameter();
            auto v = m.add_variable("v");
            coek::Expression f = 2 * (v + v) + v;
            m.add_objective(f);
            coek::NLPModel nlp(m, ADNAME);

            std::vector<double> x{0};
            std::vector<double> baseline{5};
            std::vector<double> ans(1);
            nlp.compute_df(x, ans);
            REQUIRE(ans == baseline);
        }
        WHEN("simple")
        {
            coek::Model m;
            auto p = coek::parameter();
            auto v = m.add_variable("v");
            coek::Expression f = 3 * p + 2 * v;
            m.add_objective(f);
            coek::NLPModel nlp(m, ADNAME);

            std::vector<double> x{0};
            std::vector<double> baseline{2};
            std::vector<double> ans(1);
            nlp.compute_df(x, ans);
            REQUIRE(ans == baseline);
        }
        WHEN("multiple")
        {
            coek::Model m;
            auto v = m.add_variable("v");
            coek::Expression f = 7 * v + v;
            m.add_objective(f);
            coek::NLPModel nlp(m, ADNAME);

            std::vector<double> x{0};
            std::vector<double> baseline{8};
            std::vector<double> ans(1);
            nlp.compute_df(x, ans);
            REQUIRE(ans == baseline);
        }
    }

    SECTION("negate")
    {
        WHEN("linear")
        {
            coek::Model m;
            auto v = m.add_variable("v");
            coek::Expression f = -(v + 1);
            m.add_objective(f);
            coek::NLPModel nlp(m, ADNAME);

            std::vector<double> x{0};
            std::vector<double> baseline{-1};
            std::vector<double> ans(1);
            nlp.compute_df(x, ans);
            REQUIRE(ans == baseline);
        }
    }

    SECTION("times")
    {
        WHEN("lhs zero")
        {
            coek::Model m;
            coek::Expression p;
            auto v = m.add_variable("v");
            coek::Expression f = v + p * v;
            m.add_objective(f);
            coek::NLPModel nlp(m, ADNAME);

            std::vector<double> x{0};
            std::vector<double> baseline{1};
            std::vector<double> ans(1);
            nlp.compute_df(x, ans);
            REQUIRE(ans == baseline);
        }
        WHEN("lhs constant")
        {
            coek::Model m;
            auto p = coek::parameter("p").value(2);
            auto v = m.add_variable("v");
            coek::Expression f = p * v;
            m.add_objective(f);
            coek::NLPModel nlp(m, ADNAME);

            std::vector<double> x{0};
            std::vector<double> baseline{2};
            std::vector<double> ans(1);
            nlp.compute_df(x, ans);
            REQUIRE(ans == baseline);
        }
        WHEN("rhs zero")
        {
            coek::Model m;
            coek::Expression p;
            auto v = m.add_variable("v");
            coek::Expression f = v + v * p;
            m.add_objective(f);
            coek::NLPModel nlp(m, ADNAME);

            std::vector<double> x{0};
            std::vector<double> baseline{1};
            std::vector<double> ans(1);
            nlp.compute_df(x, ans);
            REQUIRE(ans == baseline);
        }
        WHEN("rhs constant")
        {
            coek::Model m;
            auto p = coek::parameter("p").value(2);
            auto v = m.add_variable("v");
            coek::Expression f = v * p;
            m.add_objective(f);
            coek::NLPModel nlp(m, ADNAME);

            std::vector<double> x{0};
            std::vector<double> baseline{2};
            std::vector<double> ans(1);
            nlp.compute_df(x, ans);
            REQUIRE(ans == baseline);
        }
        WHEN("simple quadratic")
        {
            coek::Model m;
            auto v = m.add_variable("v");
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = v * w;
            m.add_objective(f);
            coek::NLPModel nlp(m, ADNAME);

            std::vector<double> x{2, 3};
            std::vector<double> baseline{3, 2};
            std::vector<double> ans(2);
            nlp.compute_df(x, ans);
            REQUIRE(ans == baseline);
        }
    }

    SECTION("divide")
    {
        WHEN("lhs zero parameter")
        {
            coek::Model m;
            auto p = coek::parameter("p");
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = w + (2 * p) / w;
            m.add_objective(f);
            m.add_constraint(2 * w <= 0);
            coek::NLPModel nlp(m, ADNAME);

            std::vector<double> x{2};
            std::vector<double> baseline{1};
            std::vector<double> ans{999.0};
            nlp.compute_df(x, ans);
            REQUIRE(ans == baseline);
        }
        WHEN("lhs zero fixed-variable")
        {
            coek::Model m;
            auto p = m.add_variable("p").lower(0).upper(1).value(0).fixed(true);
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = w + (2 * p) / w;
            m.add_objective(f);
            m.add_constraint(2 * w <= 0);
            coek::NLPModel nlp(m, ADNAME);

            std::vector<double> x{2};
            std::vector<double> baseline{1};
            std::vector<double> ans{999.0};
            nlp.compute_df(x, ans);
            REQUIRE(ans == baseline);
        }
        WHEN("lhs zero subexpression")
        {
            coek::Model m;
            auto p = coek::subexpression();
            auto w = m.add_variable("W").lower(0).upper(1).value(0);
            coek::Expression f = w + (2 * p) / w;
            m.add_objective(f);
            coek::NLPModel nlp(m, ADNAME);

            std::vector<double> x{0};
            std::vector<double> baseline{1};
            std::vector<double> ans{999.0};
            nlp.compute_df(x, ans);
            REQUIRE(ans == baseline);
        }
        WHEN("rhs nonzero")
        {
            coek::Model m;
            auto p = coek::parameter("p").value(2);
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = w / p;
            m.add_objective(f);
            coek::NLPModel nlp(m, ADNAME);

            std::vector<double> x{1};
            std::vector<double> baseline{0.5};
            std::vector<double> ans{999.0};
            nlp.compute_df(x, ans);
            REQUIRE(ans == baseline);
        }
        WHEN("rhs polynomial")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = w / (1 + w);
            m.add_objective(f);
            coek::NLPModel nlp(m, ADNAME);

            std::vector<double> x{1};
            std::vector<double> baseline{0.25};
            std::vector<double> ans{999.0};
            nlp.compute_df(x, ans);
            REQUIRE(ans == baseline);
        }
    }

    SECTION("coverage")
    {
        WHEN("variable partial plus monomial - 1")
        {
            coek::Model m;
            auto v = m.add_variable("v");
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = w * v + v * (2 * w + 1);
            m.add_objective(f);
            coek::NLPModel nlp(m, ADNAME);

            std::vector<double> x{2, 3};
            std::vector<double> baseline{10, 6};
            std::vector<double> ans(2);
            nlp.compute_df(x, ans);
            REQUIRE(ans == baseline);
        }
        WHEN("variable partial plus monomial - 2")
        {
            coek::Model m;
            auto v = m.add_variable("v");
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = v * (2 * w + 1);
            m.add_objective(f);
            coek::NLPModel nlp(m, ADNAME);

            std::vector<double> x{2, 3};
            std::vector<double> baseline{7, 4};
            std::vector<double> ans(2);
            nlp.compute_df(x, ans);
            REQUIRE(ans == baseline);
        }
        WHEN("constant partial plus monomial")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = 3 * w + 2 * w;
            m.add_objective(f);
            coek::NLPModel nlp(m, ADNAME);

            std::vector<double> x{2};
            std::vector<double> baseline{5};
            std::vector<double> ans(1);
            nlp.compute_df(x, ans);
            REQUIRE(ans == baseline);
        }
        WHEN("negative monomial")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = -(-w) + (-(-w));
            m.add_objective(f);
            coek::NLPModel nlp(m, ADNAME);

            std::vector<double> x{2};
            std::vector<double> baseline{2};
            std::vector<double> ans(1);
            nlp.compute_df(x, ans);
            REQUIRE(ans == baseline);
        }
        WHEN("shared subexpr")
        {
            coek::Model m;
            auto v = m.add_variable("v");
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = v + 2 * w;
            m.add_objective(2 * f + 3 * f);
            coek::NLPModel nlp(m, ADNAME);

            std::vector<double> x{10, 11};
            std::vector<double> baseline{5, 10};
            std::vector<double> ans(2);
            nlp.compute_df(x, ans);
            REQUIRE(ans == baseline);
        }
    }

    SECTION("intrinsic funcs")
    {
        WHEN("exp")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = abs(2 * w);
            m.add_objective(f);
            coek::NLPModel nlp(m, ADNAME);

            std::vector<double> x{-1};
            std::vector<double> baseline{-2};
            std::vector<double> ans(1);
            nlp.compute_df(x, ans);
            REQUIRE(ans[0] == Approx(baseline[0]));
        }
        // REPN_INTRINSIC_TEST1(ceil)
        // REPN_INTRINSIC_TEST1(floor)
        WHEN("exp")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = exp(2 * w);
            m.add_objective(f);
            coek::NLPModel nlp(m, ADNAME);

            std::vector<double> x{1};
            std::vector<double> baseline{2 * pow(E, 2.0)};
            std::vector<double> ans(1);
            nlp.compute_df(x, ans);
            REQUIRE(ans[0] == Approx(baseline[0]));
        }
        WHEN("log")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = log(2 * w);
            m.add_objective(f);
            coek::NLPModel nlp(m, ADNAME);

            std::vector<double> x{2};
            std::vector<double> baseline{0.5};
            std::vector<double> ans(1);
            nlp.compute_df(x, ans);
            REQUIRE(ans[0] == Approx(baseline[0]));
        }
        WHEN("log10")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = log10(2 * w);
            m.add_objective(f);
            coek::NLPModel nlp(m, ADNAME);

            std::vector<double> x{2};
            std::vector<double> baseline{0.5 / log(10.0)};
            std::vector<double> ans(1);
            nlp.compute_df(x, ans);
            REQUIRE(ans[0] == Approx(baseline[0]));
        }
        WHEN("sqrt")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = sqrt(2 * w);
            m.add_objective(f);
            coek::NLPModel nlp(m, ADNAME);

            std::vector<double> x{2};
            std::vector<double> baseline{0.5};
            std::vector<double> ans(1);
            nlp.compute_df(x, ans);
            REQUIRE(ans[0] == Approx(baseline[0]));
        }
        WHEN("sin")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = sin(2 * w);
            m.add_objective(f);
            coek::NLPModel nlp(m, ADNAME);

            std::vector<double> x{2};
            std::vector<double> baseline{2 * cos(4)};
            std::vector<double> ans(1);
            nlp.compute_df(x, ans);
            REQUIRE(ans[0] == Approx(baseline[0]));
        }
        WHEN("cos")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = cos(2 * w);
            m.add_objective(f);
            coek::NLPModel nlp(m, ADNAME);

            std::vector<double> x{2};
            std::vector<double> baseline{-2 * sin(4)};
            std::vector<double> ans(1);
            nlp.compute_df(x, ans);
            REQUIRE(ans[0] == Approx(baseline[0]));
        }
        WHEN("tan")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = tan(2 * w);
            m.add_objective(f);
            coek::NLPModel nlp(m, ADNAME);

            std::vector<double> x{2};
            std::vector<double> baseline{2 / pow(cos(4), 2)};
            std::vector<double> ans(1);
            nlp.compute_df(x, ans);
            REQUIRE(ans[0] == Approx(baseline[0]));
        }
        WHEN("sinh")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = sinh(2 * w);
            m.add_objective(f);
            coek::NLPModel nlp(m, ADNAME);

            std::vector<double> x{2};
            std::vector<double> baseline{2 * cosh(4)};
            std::vector<double> ans(1);
            nlp.compute_df(x, ans);
            REQUIRE(ans[0] == Approx(baseline[0]));
            // static std::list<std::string> baseline = { "[", "*", "2.000", "[", "cosh", "[", "*",
            // "2", "w", "]", "]", "]" };
        }
        WHEN("cosh")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = cosh(2 * w);
            m.add_objective(f);
            coek::NLPModel nlp(m, ADNAME);

            std::vector<double> x{2};
            std::vector<double> baseline{2 * sinh(4)};
            std::vector<double> ans(1);
            nlp.compute_df(x, ans);
            REQUIRE(ans[0] == Approx(baseline[0]));
            // static std::list<std::string> baseline = { "[", "*", "2.000", "[", "sinh", "[", "*",
            // "2", "w", "]", "]", "]" };
        }
        WHEN("tanh")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = tanh(2 * w);
            m.add_objective(f);
            coek::NLPModel nlp(m, ADNAME);

            std::vector<double> x{2};
            std::vector<double> baseline{2 * (1 - pow(tanh(4), 2))};
            std::vector<double> ans(1);
            nlp.compute_df(x, ans);
            REQUIRE(ans[0] == Approx(baseline[0]));
            // static std::list<std::string> baseline = { "[", "*", "2.000", "[", "+", "1.000", "[",
            // "*", "-1.000", "[", "pow", "[", "tan", "[", "*", "2", "w", "]", "]", "2.000", "]",
            // "]", "]", "]" };
        }
        WHEN("asin")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = asin(2 * w);
            m.add_objective(f);
            coek::NLPModel nlp(m, ADNAME);

            std::vector<double> x{0.25};
            std::vector<double> baseline{2 / sqrt(3.0 / 4.0)};
            std::vector<double> ans(1);
            nlp.compute_df(x, ans);
            REQUIRE(ans[0] == Approx(baseline[0]));
            // static std::list<std::string> baseline = { "[", "*", "2.000", "[", "/", "1.000", "[",
            // "sqrt", "[", "+", "1.000", "[", "-", "[", "*", "[", "*", "2", "w", "]", "[", "*",
            // "2", "w", "]", "]", "]", "]", "]", "]", "]" };
        }
        WHEN("acos")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = acos(2 * w);
            m.add_objective(f);
            coek::NLPModel nlp(m, ADNAME);

            std::vector<double> x{0.25};
            std::vector<double> baseline{-2 / sqrt(3.0 / 4.0)};
            std::vector<double> ans(1);
            nlp.compute_df(x, ans);
            REQUIRE(ans[0] == Approx(baseline[0]));

            // static std::list<std::string> baseline = { "[", "*", "2.000", "[", "-", "[", "/",
            // "1.000", "[", "sqrt", "[", "+", "1.000", "[", "-", "[", "*", "[", "*", "2", "w", "]",
            // "[", "*", "2", "w", "]", "]", "]", "]", "]", "]", "]", "]" };
        }
        WHEN("atan")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = atan(2 * w);
            m.add_objective(f);
            coek::NLPModel nlp(m, ADNAME);

            std::vector<double> x{0.25};
            std::vector<double> baseline{2 / (5.0 / 4.0)};
            std::vector<double> ans(1);
            nlp.compute_df(x, ans);
            REQUIRE(ans[0] == Approx(baseline[0]));
            // static std::list<std::string> baseline = { "[", "*", "2.000", "[", "/", "1.000", "[",
            // "+", "1.000", "[", "*", "[", "*", "2", "w", "]", "[", "*", "2", "w", "]", "]", "]",
            // "]", "]" };
        }
        WHEN("asinh")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = asinh(2 * w);
            m.add_objective(f);
            coek::NLPModel nlp(m, ADNAME);

            std::vector<double> x{0.25};
            std::vector<double> baseline{2 / sqrt(5.0 / 4.0)};
            std::vector<double> ans(1);
            nlp.compute_df(x, ans);
            REQUIRE(ans[0] == Approx(baseline[0]));

            // static std::list<std::string> baseline = { "[", "*", "2.000", "[", "/", "1.000", "[",
            // "sqrt", "[", "+", "1.000", "[", "*", "[", "*", "2", "w", "]", "[", "*", "2", "w",
            // "]", "]", "]", "]", "]", "]" };
        }
        WHEN("acosh")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = acosh(2 * w);
            m.add_objective(f);
            coek::NLPModel nlp(m, ADNAME);

            std::vector<double> x{1};
            std::vector<double> baseline{2 / sqrt(3.0)};
            std::vector<double> ans(1);
            nlp.compute_df(x, ans);
            REQUIRE(ans[0] == Approx(baseline[0]));

            // static std::list<std::string> baseline = { "[", "*", "2.000", "[", "/", "1.000", "[",
            // "sqrt", "[", "+", "[", "*", "[", "*", "2", "w", "]", "[", "*", "2", "w", "]", "]",
            // "-1.000", "]", "]", "]", "]" };
        }
        WHEN("atanh")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = atanh(2 * w);
            m.add_objective(f);
            coek::NLPModel nlp(m, ADNAME);

            std::vector<double> x{0.25};
            std::vector<double> baseline{2 / (3.0 / 4.0)};
            std::vector<double> ans(1);
            nlp.compute_df(x, ans);
            REQUIRE(ans[0] == Approx(baseline[0]));

            // static std::list<std::string> baseline = { "[", "*", "2.000", "[", "/", "1.000", "[",
            // "+", "[", "*", "[", "*", "2", "w", "]", "[", "*", "2", "w", "]", "]", "-1.000", "]",
            // "]", "]" };
        }
        WHEN("pow - 1")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = pow(w, 3);
            m.add_objective(f);
            coek::NLPModel nlp(m, ADNAME);

            std::vector<double> x{2};
            std::vector<double> baseline{3 * 4};
            std::vector<double> ans(1);
            nlp.compute_df(x, ans);
            REQUIRE(ans[0] == Approx(baseline[0]));

            // static std::list<std::string> baseline = { "[", "*", "2.000", "[", "*", "3.000", "[",
            // "pow", "[", "*", "2", "w", "]", "[", "+", "3.000", "-1.000", "]", "]", "]", "]" };
        }
        WHEN("pow - 2")
        {
            coek::Model m;
            auto w = m.add_variable("w").lower(0).upper(1).value(0);
            coek::Expression f = pow(3, 2 * w);
            m.add_objective(f);
            coek::NLPModel nlp(m, ADNAME);

            std::vector<double> x{2};
            std::vector<double> baseline{2 * log(3) * pow(3, 4)};
            std::vector<double> ans(1);
            nlp.compute_df(x, ans);
            REQUIRE(ans[0] == Approx(baseline[0]));

            // static std::list<std::string> baseline = { "[", "*", "2.000", "[", "*", "1.099", "[",
            // "pow", "3.000", "[", "*", "2", "w", "]", "]", "]", "]" };
        }
    }
}
#endif
