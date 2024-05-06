#include <iostream>

#include "catch2/catch_test_macros.hpp"
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
