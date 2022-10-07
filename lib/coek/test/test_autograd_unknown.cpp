#include <iostream>
#include <vector>

#include "catch2/catch.hpp"
#include "coek/ast/base_terms.hpp"
#include "coek/coek.hpp"

// These are code coverage tests for coek/autograd/unknownad_repn.hpp
TEST_CASE("unknown_ad", "[smoke]")
{
    {
        SECTION("error1")
        {
            coek::NLPModel nlp;

            // No errors here
            nlp.reset();

            REQUIRE_THROWS_WITH(nlp.num_variables(), "Error accessing uninitialized NLPModel");
            REQUIRE_THROWS_WITH(nlp.num_objectives(), "Error accessing uninitialized NLPModel");
            REQUIRE_THROWS_WITH(nlp.num_constraints(), "Error accessing uninitialized NLPModel");
            REQUIRE_THROWS_WITH(nlp.num_nonzeros_Jacobian(),
                                "Error accessing uninitialized NLPModel");
            REQUIRE_THROWS_WITH(nlp.num_nonzeros_Hessian_Lagrangian(),
                                "Error accessing uninitialized NLPModel");

            std::vector<double> tmp1;
            REQUIRE_THROWS_WITH(nlp.set_variable_view(tmp1),
                                "Error accessing uninitialized NLPModel");
            REQUIRE_THROWS_WITH(nlp.set_variable_view(&tmp1[0], 0),
                                "Error accessing uninitialized NLPModel");

            std::vector<size_t> tmp2;
            REQUIRE_THROWS_WITH(nlp.get_J_nonzeros(tmp2, tmp2),
                                "Error accessing uninitialized NLPModel");
            REQUIRE_THROWS_WITH(nlp.get_H_nonzeros(tmp2, tmp2),
                                "Error accessing uninitialized NLPModel");

            REQUIRE_THROWS_WITH(nlp.print_equations(), "Error accessing uninitialized NLPModel");
            REQUIRE_THROWS_WITH(nlp.print_values(), "Error accessing uninitialized NLPModel");

            REQUIRE_THROWS_WITH(nlp.compute_f(), "Error accessing uninitialized NLPModel");
            double tmp3;
            REQUIRE_THROWS_WITH(nlp.compute_df(tmp3, tmp1, 0),
                                "Error accessing uninitialized NLPModel");
            REQUIRE_THROWS_WITH(nlp.compute_c(tmp1), "Error accessing uninitialized NLPModel");
            REQUIRE_THROWS_WITH(nlp.compute_dc(tmp1, 0), "Error accessing uninitialized NLPModel");
            REQUIRE_THROWS_WITH(nlp.compute_H(tmp1, tmp1),
                                "Error accessing uninitialized NLPModel");
            REQUIRE_THROWS_WITH(nlp.compute_J(tmp1), "Error accessing uninitialized NLPModel");
        }
    }
#ifdef DEBUG
    REQUIRE(coek::env.check_memory() == true);
#endif
}
