
#include <iostream>

#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_approx.hpp"
#include "catch2/generators/catch_generators.hpp"

#include "coek/coek.hpp"

//
// EXAMPLES
//

// rosenbr
coek::Model rosenbr();
std::vector<double> rosenbr_soln{1, 1};

// simplelp1
coek::Model simplelp1();
std::vector<double> simplelp1_soln{375, 250};
void simplelp1_solve();

// quad
coek::Model quad_vector(std::vector<coek::Parameter>& p);
std::vector<double> quad_soln1{0.5, 0.5, 0.5, 0.5, 0.5};
std::vector<double> quad_soln2{-0.5, -0.5, -0.5, -0.5, -0.5};

// invquad
coek::Model invquad_vector(std::vector<coek::Parameter>& p);
#if __cpp_lib_variant
coek::Model invquad_array(std::vector<coek::Parameter>& p);
void invquad_array_solve();
void invquad_array_resolve();
#endif
std::vector<double> invquad_soln_5{-10, -10, -10, -10, -10};

// bounds_check
coek::Model check_bounds()
{
    coek::Model model;

    auto a = model.add_variable().value(2.0);
    auto b = model.add_variable().value(-2.0);
    auto c = model.add_variable().value(2.0);
    auto d = model.add_variable().value(-2.0);
    auto e = model.add_variable().value(2.0);

    auto aa = model.add_variable().value(2.0);
    auto bb = model.add_variable().value(-2.0);
    auto cc = model.add_variable().value(2.0);
    auto dd = model.add_variable().value(-2.0);
    auto ee = model.add_variable().value(2.0);

    model.add_objective(a * a + b * b + c * c + d * d + e * e + aa * aa + bb * bb + cc * cc
                        + dd * dd + ee * ee);

    model.add(a >= 1);
    model.add(b <= -1);
    model.add(aa - 1 >= 0);
    model.add(bb + 1 <= 0);
    model.add(inequality(1, c, 3));
    model.add(inequality(-3, d, -1));
    model.add(inequality(0, cc - 1, 2));
    model.add(inequality(-2, dd + 1, 0));
    model.add(e == 1);
    model.add(ee - 1 == 0);

    return model;
}
std::vector<double> check_bounds_soln{1, -1, 1, -1, 1, 1, -1, 1, -1, 1};

void check(std::vector<coek::Variable>& variables, std::vector<double>& soln)
{
    REQUIRE(variables.size() == soln.size());
    for (size_t i = 0; i < variables.size(); i++) {
        // CAPTURE(i);
        REQUIRE(variables[i].value() == Catch::Approx(soln[i]));
    }
}

namespace {
std::initializer_list<const char*> adnames{
#ifdef WITH_CPPAD
    "cppad",
#endif
#ifdef WITH_ASL
    "asl",
#endif
};
}  // namespace

TEST_CASE("ipopt_examples", "[solvers][ipopt]")
{
    INFO("TEST_CASE ipopt_examples");

    // TODO - use SKIP after upgrading to Catch2 v3
    auto adname = GENERATE(values(adnames));
#if defined(WITH_ASL) | defined(WITH_CPPAD)
    CAPTURE(adname);

    coek::NLPSolver solver("ipopt");
    if (solver.available()) {
        solver.set_option("print_level", 0);

        SECTION("rosenbr")
        {
            auto m = rosenbr();

            coek::NLPModel nlp(m, adname);
            auto res = solver.solve(nlp);
            REQUIRE(coek::check_optimal_termination(res));

            check(m.get_variables(), rosenbr_soln);
        }

        SECTION("check_bounds")
        {
            auto m = check_bounds();

            coek::NLPModel nlp(m, adname);
            auto res = solver.solve(nlp);
            REQUIRE(coek::check_optimal_termination(res));

            check(m.get_variables(), check_bounds_soln);
        }

        SECTION("quad_vector")
        {
            std::vector<coek::Parameter> p(5);
            for (auto& param : p)
                param.value(0.5);

            WHEN("solve")
            {
                auto m = quad_vector(p);

                coek::NLPModel nlp(m, adname);
                auto res = solver.solve(nlp);
                REQUIRE(coek::check_optimal_termination(res));

                check(m.get_variables(), quad_soln1);
            }
            WHEN("resolve - Same start")
            {
                auto m = quad_vector(p);

                coek::NLPModel nlp(m, adname);
                auto res = solver.solve(nlp);
                REQUIRE(coek::check_optimal_termination(res));

                for (auto& param : p)
                    param.value(-0.5);

                for (size_t i = 0; i < nlp.num_variables(); i++)
                    nlp.get_variable(i).value(0);
                // solver.set_option("print_level", 0);
                res = solver.resolve();
                REQUIRE(coek::check_optimal_termination(res));

                check(m.get_variables(), quad_soln2);
            }
            WHEN("resolve - Current point")
            {
                auto m = quad_vector(p);

                coek::NLPModel nlp(m, adname);
                auto res = solver.solve(nlp);
                REQUIRE(coek::check_optimal_termination(res));

                for (auto& param : p)
                    param.value(-0.5);

                res = solver.resolve();
                REQUIRE(coek::check_optimal_termination(res));

                check(m.get_variables(), quad_soln2);
            }
        }

        SECTION("invquad_vector")
        {
            std::vector<coek::Parameter> p(5);
            for (auto& param : p)
                param.value(0.5);

            WHEN("solve")
            {
                auto m = invquad_vector(p);

                coek::NLPModel nlp(m, adname);
                auto res = solver.solve(nlp);
                REQUIRE(coek::check_optimal_termination(res));

                check(m.get_variables(), invquad_soln_5);
            }
            WHEN("resolve - Same start")
            {
                auto m = invquad_vector(p);

                coek::NLPModel nlp(m, adname);
                auto res = solver.solve(nlp);
                REQUIRE(coek::check_optimal_termination(res));

                for (auto& param : p)
                    param.value(-0.5);

                for (size_t i = 0; i < nlp.num_variables(); i++)
                    nlp.get_variable(i).value(0);
                solver.set_option("print_level", 0);
                res = solver.resolve();
                REQUIRE(coek::check_optimal_termination(res));

                std::vector<double> invquad_resolve_5{10, 10, 10, 10, 10};
                check(m.get_variables(), invquad_resolve_5);
            }
            WHEN("resolve - Current point")
            {
                auto m = invquad_vector(p);

                coek::NLPModel nlp(m, adname);
                auto res = solver.solve(nlp);
                REQUIRE(coek::check_optimal_termination(res));

                for (auto& param : p)
                    param.value(0.5);

                res = solver.resolve();
                REQUIRE(coek::check_optimal_termination(res));

                check(m.get_variables(), invquad_soln_5);
            }
        }

#    if __cpp_lib_variant
        SECTION("invquad_array")
        {
            std::vector<coek::Parameter> p(5);
            for (auto& param : p)
                param.value(0.5);

            WHEN("solve")
            {
                auto m = invquad_array(p);

                coek::NLPModel nlp(m, adname);
                auto res = solver.solve(nlp);
                REQUIRE(coek::check_optimal_termination(res));

                check(m.get_variables(), invquad_soln_5);
            }
            WHEN("resolve - Same start")
            {
                auto m = invquad_array(p);

                coek::NLPModel nlp(m, adname);
                auto res = solver.solve(nlp);
                REQUIRE(coek::check_optimal_termination(res));

                for (auto& param : p)
                    param.value(-0.5);

                for (size_t i = 0; i < nlp.num_variables(); i++)
                    nlp.get_variable(i).value(0);
                solver.set_option("print_level", 0);
                res = solver.resolve();
                REQUIRE(coek::check_optimal_termination(res));

                std::vector<double> invquad_resolve_5{10, 10, 10, 10, 10};
                check(m.get_variables(), invquad_resolve_5);
            }
            WHEN("resolve - Current point")
            {
                auto m = invquad_array(p);

                coek::NLPModel nlp(m, adname);
                auto res = solver.solve(nlp);
                REQUIRE(coek::check_optimal_termination(res));

                for (auto& param : p)
                    param.value(0.5);

                res = solver.resolve();
                REQUIRE(coek::check_optimal_termination(res));

                check(m.get_variables(), invquad_soln_5);
            }

#        ifdef WITH_CPPAD
            WHEN("invquad_solve") { invquad_array_solve(); }
            WHEN("invquad_resolve") { invquad_array_resolve(); }
#        endif
        }
#    endif
    }
    else {
        // GCOVR_EXCL_START
        SECTION("rosenbr")
        {
            auto m = rosenbr();
            coek::NLPModel nlp(m, adname);
            auto res = solver.solve(nlp);
            REQUIRE(res->termination_condition == coek::TerminationCondition::solver_not_available);
        }
        // GCOVR_EXCL_STOP
    }
#endif
}

TEST_CASE("gurobi_examples", "[solvers][gurobi]")
{
    coek::Solver solver("gurobi");
    solver.set_option("OutputFlag", 0);

    if (solver.available()) {
        SECTION("simplelp1")
        {
            auto m = simplelp1();
            auto res = solver.solve(m);
            REQUIRE(coek::check_optimal_termination(res));

            REQUIRE(res->objective_value == Catch::Approx(28750.0));
            REQUIRE(res->objective_bound == Catch::Approx(28750.0));
            check(m.get_variables(), simplelp1_soln);
        }
    }
    else {
        // GCOVR_EXCL_START
        SECTION("simplelp1")
        {
            auto m = simplelp1();
            auto res = solver.solve(m);
            REQUIRE(res->termination_condition == coek::TerminationCondition::solver_not_available);
        }
        // GCOVR_EXCL_STOP
    }
}

TEST_CASE("highs_examples", "[solvers][highs]")
{
    coek::Solver solver("highs");
    solver.set_option("output_flag", false);

    if (solver.available()) {
        SECTION("simplelp1")
        {
            auto m = simplelp1();
            auto res = solver.solve(m);
            REQUIRE(coek::check_optimal_termination(res));

            REQUIRE(res->objective_value == Catch::Approx(28750.0));
            REQUIRE(res->objective_bound == Catch::Approx(28750.0));
            check(m.get_variables(), simplelp1_soln);
        }
    }
    else {
        // GCOVR_EXCL_START
        SECTION("simplelp1")
        {
            auto m = simplelp1();
            auto res = solver.solve(m);
            REQUIRE(res->termination_condition == coek::TerminationCondition::solver_not_available);
        }
        // GCOVR_EXCL_STOP
    }
}
