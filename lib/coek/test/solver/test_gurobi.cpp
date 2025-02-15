#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_approx.hpp"
#include "catch2/generators/catch_generators.hpp"

#include "coek/coek.hpp"
#include "test/solver/TestModel.hpp"

//
// Gurobi Checks
//

TEST_CASE("gurobi_checks", "[solvers][gurobi]")
{
    coek::Solver solver("gurobi");
    solver.set_option("OutputFlag", 0);

    if (solver.available()) {
        SECTION("empty")
        {
            auto test = test::model("empty");
            auto m = test->model;
            REQUIRE(m.name() == "empty");
            auto res = solver.solve(m);
            REQUIRE(res->termination_condition == coek::TerminationCondition::empty_model);
        }
        SECTION("rosenbr")
        {
            auto test = test::model("rosenbr");
            auto m = test->model;
            REQUIRE(m.name() == "rosenbr");
            auto res = solver.solve(m);
            REQUIRE(res->termination_condition
                    == coek::TerminationCondition::invalid_model_for_solver);
        }
        SECTION("simplelp1")
        {
            auto test = test::model("simplelp1");
            auto m = test->model;
            REQUIRE(m.name() == "simplelp1");
            try {
                auto res = solver.solve(m);
                REQUIRE(test->check_results(m, res) == true);
                }
            catch (std::exception& e) {
                std::cout << "TESTING ERROR: exception - " << e.what() << std::endl;
                throw;
                }
        }
        SECTION("simpleqp1")
        {
            auto test = test::model("simpleqp1");
            auto m = test->model;
            REQUIRE(m.name() == "simpleqp1");
            auto res = solver.solve(m);
            REQUIRE(test->check_results(m, res) == true);
        }
        SECTION("simpleqp2")
        {
            auto test = test::model("simpleqp2");
            auto m = test->model;
            REQUIRE(m.name() == "simpleqp2");
            auto res = solver.solve(m);
            REQUIRE(test->check_results(m, res) == true);
        }
        SECTION("simpleqp3")
        {
            auto test = test::model("simpleqp3");
            auto m = test->model;
            REQUIRE(m.name() == "simpleqp3");
            auto res = solver.solve(m);
            REQUIRE(test->check_results(m, res) == true);
        }
        SECTION("simpleqp4")
        {
            auto test = test::model("simpleqp4");
            auto m = test->model;
            REQUIRE(m.name() == "simpleqp4");
            auto res = solver.solve(m);
            REQUIRE(test->check_results(m, res) == true);
        }
        SECTION("lp_bounds1")
        {
            auto test = test::model("lp_bounds1");
            auto m = test->model;
            REQUIRE(m.name() == "lp_bounds1");
            auto res = solver.solve(m);
            REQUIRE(test->check_results(m, res) == true);
        }
        SECTION("lp_bounds2")
        {
            auto test = test::model("lp_bounds2");
            auto m = test->model;
            REQUIRE(m.name() == "lp_bounds2");
            auto res = solver.solve(m);
            REQUIRE(test->check_results(m, res) == true);
        }
        SECTION("qp_bounds")
        {
            auto test = test::model("qp_bounds");
            auto m = test->model;
            REQUIRE(m.name() == "qp_bounds");
            auto res = solver.solve(m);
            REQUIRE(test->check_results(m, res) == true);
        }
    }
    else {
        // GCOVR_EXCL_START
        SECTION("simplelp1 - missing license")
        {
            auto test = test::model("simplelp1");
            auto m = test->model;
            REQUIRE(m.name() == "simplelp1");
            auto res = solver.solve(m);
            auto ok
                = (res->termination_condition == coek::TerminationCondition::solver_not_available)
                  or (res->termination_condition == coek::TerminationCondition::license_problems);
            if (not ok) {
                std::cout << "WARNING: Unexpected termination condition: " << coek::to_string(res->termination_condition) << std::endl;
                std::cout << "Error message: " << res->error_message << std::endl;
                }
            // We cannot assume the gurobi license is unavailable b.c. after the call to available(), the license may have been freed up
            //
            //REQUIRE(ok == true);
        }
        // GCOVR_EXCL_STOP
    }
}
