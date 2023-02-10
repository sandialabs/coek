
#include <iostream>

#include "catch2/catch.hpp"
#include "coek/ast/base_terms.hpp"
#include "coek/coek.hpp"

//
// EXAMPLES
//
coek::Model rosenbr();
std::vector<double> rosenbr_soln{1, 1};

coek::Model simplelp1();
std::vector<double> simplelp1_soln{375, 250};
void simplelp1_solve();

coek::Model invquad_vector(std::vector<coek::Parameter>& p);
#if __cpp_lib_variant
coek::Model invquad_array(std::vector<coek::Parameter>& p);
void invquad_array_solve();
void invquad_array_resolve();
#endif
std::vector<double> invquad_soln_5{-10, -10, -10, -10, -10};

void check(std::vector<coek::Variable>& variables, std::vector<double>& soln)
{
    for (size_t i = 0; i < variables.size(); i++) REQUIRE(variables[i].value() == Approx(soln[i]));
}

TEST_CASE("ipopt_cppad", "[smoke]")
{
    coek::NLPSolver solver("ipopt");
    if (solver.available()) {
        REQUIRE(not solver.error_status());
        REQUIRE(solver.error_code() == 0);

        solver.set_option("print_level", 0);

        SECTION("rosenbr")
        {
            auto m = rosenbr();

            coek::NLPModel nlp(m, "cppad");
            solver.solve(nlp);

            check(m.get_variables(), rosenbr_soln);
        }

        SECTION("invquad_vector")
        {
            std::vector<coek::Parameter> p(5);
            for (auto& param : p) param.value(0.5);

            WHEN("solve")
            {
                auto m = invquad_vector(p);

                coek::NLPModel nlp(m, "cppad");
                solver.solve(nlp);

                check(m.get_variables(), invquad_soln_5);
            }
            WHEN("resolve - Same start")
            {
                auto m = invquad_vector(p);

                coek::NLPModel nlp(m, "cppad");
                solver.solve(nlp);

                for (auto& param : p) param.value(-0.5);

                for (size_t i = 0; i < nlp.num_variables(); i++) nlp.get_variable(i).value(0);
                solver.set_option("print_level", 0);
                solver.resolve();

                std::vector<double> invquad_resolve_5{10, 10, 10, 10, 10};
                check(m.get_variables(), invquad_resolve_5);
            }
            WHEN("resolve - Current point")
            {
                auto m = invquad_vector(p);

                coek::NLPModel nlp(m, "cppad");
                solver.solve(nlp);

                for (auto& param : p) param.value(0.5);

                solver.resolve();

                check(m.get_variables(), invquad_soln_5);
            }
            /*
                    WHEN( "resolve - Warm Start" ) {
                        auto m = invquad_vector(p);

                        coek::NLPModel nlp(m, "cppad");
                        solver.solve(nlp);

                        for (auto& param : p)
                            param.value(-0.5);

                        // Even though we set the value of the initial point,
                        // the warm starting option should ignore this and
                        // restart the solve from where it ended last time.
                        for (size_t i=0; i<nlp.num_variables(); i++)
                            nlp.get_variable(i).value(0);

                        solver.set_option("warm_start_init_point", "yes");
                        //solver.set_option("print_level", 0);
                        solver.resolve();

                        check(m.get_variables(), invquad_soln_5);
                    }
            */
        }

#if __cpp_lib_variant
        SECTION("invquad_array")
        {
            std::vector<coek::Parameter> p(5);
            for (auto& param : p) param.value(0.5);

            WHEN("solve")
            {
                auto m = invquad_array(p);

                coek::NLPModel nlp(m, "cppad");
                solver.solve(nlp);

                check(m.get_variables(), invquad_soln_5);
            }
            WHEN("resolve - Same start")
            {
                auto m = invquad_array(p);

                coek::NLPModel nlp(m, "cppad");
                solver.solve(nlp);

                for (auto& param : p) param.value(-0.5);

                for (size_t i = 0; i < nlp.num_variables(); i++) nlp.get_variable(i).value(0);
                solver.set_option("print_level", 0);
                solver.resolve();

                std::vector<double> invquad_resolve_5{10, 10, 10, 10, 10};
                check(m.get_variables(), invquad_resolve_5);
            }
            WHEN("resolve - Current point")
            {
                auto m = invquad_array(p);

                coek::NLPModel nlp(m, "cppad");
                solver.solve(nlp);

                for (auto& param : p) param.value(0.5);

                solver.resolve();

                check(m.get_variables(), invquad_soln_5);
            }
            /*
                    WHEN( "resolve - Warm Start" ) {
                        auto m = invquad_array(p);

                        coek::NLPModel nlp(m, "cppad");
                        solver.solve(nlp);

                        for (auto& param : p)
                            param.value(-0.5);

                        // Even though we set the value of the initial point,
                        // the warm starting option should ignore this and
                        // restart the solve from where it ended last time.
                        for (size_t i=0; i<nlp.num_variables(); i++)
                            nlp.get_variable(i).value(0);

                        solver.set_option("warm_start_init_point", "yes");
                        //solver.set_option("print_level", 0);
                        solver.resolve();

                        check(m.get_variables(), invquad_soln_5);
                    }
            */

            WHEN("invquad_solve") { invquad_array_solve(); }
            WHEN("invquad_resolve") { invquad_array_resolve(); }
        }
#endif
    }
    else {
        REQUIRE(solver.error_status());
        REQUIRE(solver.error_code() != 0);
        std::cerr << solver.error_message() << std::endl;
    }
}

TEST_CASE("ipopt_asl", "[smoke]")
{
    coek::NLPSolver solver("ipopt");
    if (solver.available()) {
        REQUIRE(not solver.error_status());
        REQUIRE(solver.error_code() == 0);

        solver.set_option("print_level", 0);

        SECTION("rosenbr")
        {
            auto m = rosenbr();

            coek::NLPModel nlp(m, "asl");
            solver.solve(nlp);

            check(m.get_variables(), rosenbr_soln);
        }

        SECTION("invquad_vector")
        {
            std::vector<coek::Parameter> p(5);
            for (auto& param : p) param.value(0.5);

            WHEN("solve")
            {
                auto m = invquad_vector(p);

                coek::NLPModel nlp(m, "asl");
                solver.solve(nlp);

                check(m.get_variables(), invquad_soln_5);
            }
            WHEN("resolve - Same start")
            {
                auto m = invquad_vector(p);

                coek::NLPModel nlp(m, "asl");
                solver.solve(nlp);

                for (auto& param : p) param.value(-0.5);

                for (size_t i = 0; i < nlp.num_variables(); i++) nlp.get_variable(i).value(0);
                solver.set_option("print_level", 0);
                solver.resolve();

                std::vector<double> invquad_resolve_5{10, 10, 10, 10, 10};
                check(m.get_variables(), invquad_resolve_5);
            }
            WHEN("resolve - Current point")
            {
                auto m = invquad_vector(p);

                coek::NLPModel nlp(m, "asl");
                solver.solve(nlp);

                for (auto& param : p) param.value(0.5);

                solver.resolve();

                check(m.get_variables(), invquad_soln_5);
            }
            /*
                    WHEN( "resolve - Warm Start" ) {
                        auto m = invquad_vector(p);

                        coek::NLPModel nlp(m, "asl");
                        solver.solve(nlp);

                        for (auto& param : p)
                            param.value(-0.5);

                        // Even though we set the value of the initial point,
                        // the warm starting option should ignore this and
                        // restart the solve from where it ended last time.
                        for (size_t i=0; i<nlp.num_variables(); i++)
                            nlp.get_variable(i).value(0);

                        solver.set_option("warm_start_init_point", "yes");
                        //solver.set_option("print_level", 0);
                        solver.resolve();

                        check(m.get_variables(), invquad_soln_5);
                    }
            */
        }

#if __cpp_lib_variant
        SECTION("invquad_array")
        {
            std::vector<coek::Parameter> p(5);
            for (auto& param : p) param.value(0.5);

            WHEN("solve")
            {
                auto m = invquad_array(p);

                coek::NLPModel nlp(m, "asl");
                solver.solve(nlp);

                check(m.get_variables(), invquad_soln_5);
            }
            WHEN("resolve - Same start")
            {
                auto m = invquad_array(p);

                coek::NLPModel nlp(m, "asl");
                solver.solve(nlp);

                for (auto& param : p) param.value(-0.5);

                for (size_t i = 0; i < nlp.num_variables(); i++) nlp.get_variable(i).value(0);
                solver.set_option("print_level", 0);
                solver.resolve();

                std::vector<double> invquad_resolve_5{10, 10, 10, 10, 10};
                check(m.get_variables(), invquad_resolve_5);
            }
            WHEN("resolve - Current point")
            {
                auto m = invquad_array(p);

                coek::NLPModel nlp(m, "asl");
                solver.solve(nlp);

                for (auto& param : p) param.value(0.5);

                solver.resolve();

                check(m.get_variables(), invquad_soln_5);
            }
            /*
                    WHEN( "resolve - Warm Start" ) {
                        auto m = invquad_array(p);

                        coek::NLPModel nlp(m, "asl");
                        solver.solve(nlp);

                        for (auto& param : p)
                            param.value(-0.5);

                        // Even though we set the value of the initial point,
                        // the warm starting option should ignore this and
                        // restart the solve from where it ended last time.
                        for (size_t i=0; i<nlp.num_variables(); i++)
                            nlp.get_variable(i).value(0);

                        solver.set_option("warm_start_init_point", "yes");
                        //solver.set_option("print_level", 0);
                        solver.resolve();

                        check(m.get_variables(), invquad_soln_5);
                    }
            */

            WHEN("invquad_solve") { invquad_array_solve(); }
            WHEN("invquad_resolve") { invquad_array_resolve(); }
        }
#endif
    }
    else {
        REQUIRE(solver.error_status());
        REQUIRE(solver.error_code() != 0);
        std::cerr << solver.error_message() << std::endl;
    }
}

#ifdef WITH_GUROBI
TEST_CASE("gurobi", "[smoke]")
{
    SECTION("simplelp1")
    {
        auto m = simplelp1();

        coek::Solver solver("gurobi");

        solver.set_option("OutputFlag", 0);
        solver.solve(m);

        check(m.get_variables(), simplelp1_soln);
    }
    SECTION("simplelp1_solve") { simplelp1_solve(); }
}
#endif
