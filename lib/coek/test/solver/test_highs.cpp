#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_approx.hpp"
#include "catch2/generators/catch_generators.hpp"

#include "coek/coek.hpp"
#include "test/solver/TestModel.hpp"

//
// Highs Checks
//

TEST_CASE("highs_checks", "[solver][highs]")
{
    coek::Solver solver("highs");
    solver.set_option("output_flag", false);

    if (solver.available()) {
        SECTION("empty")
        {
            auto test = test::model("empty");
            auto m = test->model;
            REQUIRE(m.name() == "empty");
            auto res = solver.solve(m);
            REQUIRE(res->termination_condition == coek::TerminationCondition::empty_model);
        }

        SECTION("simplelp1")
        {
            auto test = test::model("simplelp1");
            auto m = test->model;
            REQUIRE(m.name() == "simplelp1");
            auto res = solver.solve(m);
            REQUIRE(test->check_results(m, res) == true);
        }
        SECTION("lp_bounds")
        {
            auto test = test::model("lp_bounds");
            auto m = test->model;
            REQUIRE(m.name() == "lp_bounds");
            auto res = solver.solve(m);
            REQUIRE(test->check_results(m, res) == true);
        }
        SECTION("qp_bounds")
        {
            auto test = test::model("qp_bounds");
            auto m = test->model;
            REQUIRE(m.name() == "qp_bounds");
            auto res = solver.solve(m);
            REQUIRE(test->check_results(m, res)
                    == false);  // Expected failure (Our HIGHS interface doesn't yet support QIPs)
        }
    }
    else {
        SECTION("simplelp1")
        {
            auto test = test::model("simplelp1");
            auto m = test->model;
            REQUIRE(m.name() == "simplelp1");
            auto res = solver.solve(m);
            REQUIRE(res->termination_condition == coek::TerminationCondition::solver_not_available);
        }
    }
}
