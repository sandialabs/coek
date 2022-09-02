
#include <iostream>
#include "catch2/catch.hpp"
#include "coek/ast/base_terms.hpp"
#include "coek/coek.hpp"

//
// EXAMPLES
//
void rosenbr_example(coek::Model& model);
std::vector<double> rosenbr_soln {1,1};

void simplelp1_example(coek::Model& model);
std::vector<double> simplelp1_soln {375,250};

void invquad_example(coek::Model& model, std::vector<coek::Parameter>& p);
std::vector<double> invquad_soln_5 {-10,-10,-10,-10,-10};


void check(std::vector<coek::Variable>& variables, std::vector<double>& soln)
{
for (size_t i=0; i<variables.size(); i++)
    REQUIRE(variables[i].value() == Approx(soln[i]));
}


TEST_CASE( "ipopt", "[smoke]" )
{
{
coek::NLPSolver solver("ipopt");
if (solver.available()) {
    REQUIRE( not solver.error_status());
    REQUIRE(solver.error_code() == 0);

    solver.set_option("print_level", 0);

    SECTION( "rosenbr" ) {
        coek::Model m;
        rosenbr_example(m);

        coek::NLPModel nlp(m, "cppad");
        solver.solve(nlp);

        check(m.get_variables(), rosenbr_soln);
        }

    SECTION( "invquad" ) {
        std::vector<coek::Parameter> p(5);
        for (auto it=p.begin(); it != p.end(); ++it)
            it->value(0.5);

        WHEN( "solve" ) {
            //
            // Solve for p_i=0.5
            // Initial x_i=0
            // Final x_i=-10
            //
            coek::Model m;
            invquad_example(m, p);

            coek::NLPModel nlp(m, "cppad");
            solver.solve(nlp);

            check(m.get_variables(), invquad_soln_5);
        }
        WHEN( "resolve - Same start" ) {
            //
            // Solve for p_i=0.5
            // Initial x_i=0
            // Final x_i=-10
            //
            coek::Model m;
            invquad_example(m, p);

            coek::NLPModel nlp(m, "cppad");
            solver.solve(nlp);

            //
            // Resolve for p_i=-0.5
            // Initial x_i=0
            // Final x_i=-10
            //
            for (auto it=p.begin(); it != p.end(); ++it)
                it->value(-0.5);
            for (size_t i=0; i<nlp.num_variables(); i++)
                nlp.get_variable(i).value(0);
            solver.set_option("print_level", 0);
            solver.resolve();

            std::vector<double> invquad_resolve_5 {10,10,10,10,10};
            check(m.get_variables(), invquad_resolve_5);
        }
        WHEN( "resolve - Current point" ) {
            //
            // Solve for p_i=0.5
            // Initial x_i=0
            // Final x_i=-10
            //
            coek::Model m;
            invquad_example(m, p);

            coek::NLPModel nlp(m, "cppad");
            solver.solve(nlp);

            //
            // Solve for p_i=-0.5
            // Initial x_i=-10
            // Final x_i=-10
            //
            for (auto it=p.begin(); it != p.end(); ++it)
                it->value(-0.5);
            solver.resolve();

            check(m.get_variables(), invquad_soln_5);
        }
#if 0
WEH - This is broken.

        WHEN( "resolve - Warm Start" ) {
            //
            // Solve for p_i=0.5
            // Initial x_i=0
            // Final x_i=-10
            //
            coek::Model m;
            invquad_example(m, p);

            coek::NLPModel nlp(m, "cppad");
            solver.solve(nlp);

            //
            // Solve for p_i=-0.5
            // Initial x_i=-10
            // Final x_i=-10
            //
            for (auto it=p.begin(); it != p.end(); ++it)
                it->value(-0.5);
            // Even though we set the value of the initial point,
            // the warm starting option should ignore this and 
            // restart the solve from where it ended last time.
            for (size_t i=0; i<nlp.num_variables(); i++)
                nlp.get_variable(i).value(0);

            solver.set_option("warm_start_init_point", "yes");
            solver.set_option("print_level", 10);
            solver.resolve();

            check(m.get_variables(), invquad_soln_5);
        }
#endif
        }
    }
else {
    REQUIRE(solver.error_status());
    REQUIRE(solver.error_code() != 0);
    std::cerr << solver.error_message() << std::endl;
    }
}

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}


#ifdef WITH_GUROBI
TEST_CASE( "gurobi", "[smoke]" ) {

  SECTION( "simplelp1" ) {
    coek::Model m;
    simplelp1_example(m);

    coek::Solver solver("gurobi");

    solver.set_option("OutputFlag", 0);
    solver.solve(m);

    check(m.get_variables(), simplelp1_soln);
  }

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}
#endif
