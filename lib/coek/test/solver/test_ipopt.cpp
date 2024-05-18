#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_approx.hpp"
#include "catch2/generators/catch_generators.hpp"

#include "coek/coek.hpp"
#include "test/solver/TestModel.hpp"

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

//
// Ipopt Checks
//

TEST_CASE("ipopt_checks", "[solvers][ipopt]")
{
    // TODO - use SKIP after upgrading to Catch2 v3
    auto adname = GENERATE(values(adnames));
#if defined(WITH_ASL) | defined(WITH_CPPAD)
    CAPTURE(adname);

    coek::NLPSolver solver("ipopt");
    solver.set_option("print_level", 0);

    if (solver.available()) {
        SECTION("empty")
        {
            auto test = test::nlp_model("empty", adname);
            auto m = test->nlp_model;
            REQUIRE(m.name() == "empty");
            auto res = solver.solve(m);
            REQUIRE(res->termination_condition == coek::TerminationCondition::empty_model);
        }
        SECTION("rosenbr")
        {
            auto test = test::nlp_model("rosenbr", adname);
            auto m = test->nlp_model;
            REQUIRE(m.name() == "rosenbr");
            auto res = solver.solve(m);
            REQUIRE(test->check_results(test->model, res, 1e-5) == true);
        }
        SECTION("simplelp1")
        {
            auto test = test::nlp_model("simplelp1", adname);
            auto m = test->nlp_model;
            REQUIRE(m.name() == "simplelp1");
            auto res = solver.solve(m);
            REQUIRE(test->check_results(test->model, res, 1e-3) == true);
        }
        SECTION("simpleqp1")
        {
            auto test = test::nlp_model("simpleqp1", adname);
            auto m = test->nlp_model;
            REQUIRE(m.name() == "simpleqp1");
            auto res = solver.solve(m);
            REQUIRE(test->check_results(test->model, res, 1e-4) == true);
        }
        SECTION("simpleqp2")
        {
            auto test = test::nlp_model("simpleqp2", adname);
            auto m = test->nlp_model;
            REQUIRE(m.name() == "simpleqp2");
            auto res = solver.solve(m);
            REQUIRE(test->check_results(test->model, res, 1e-4) == true);
        }
        SECTION("simpleqp3")
        {
            auto test = test::nlp_model("simpleqp3", adname);
            auto m = test->nlp_model;
            REQUIRE(m.name() == "simpleqp3");
            auto res = solver.solve(m);
            REQUIRE(test->check_results(test->model, res, 1e-4) == true);
        }
        SECTION("simpleqp4")
        {
            auto test = test::nlp_model("simpleqp4", adname);
            auto m = test->nlp_model;
            REQUIRE(m.name() == "simpleqp4");
            auto res = solver.solve(m);
            REQUIRE(test->check_results(test->model, res, 1e-4) == true);
        }
        SECTION("lp_bounds1")
        {
            auto test = test::nlp_model("lp_bounds1", adname);
            auto m = test->nlp_model;
            REQUIRE(m.name() == "lp_bounds1");
            auto res = solver.solve(m);
            REQUIRE(test->check_results(test->model, res) == true);
        }
        SECTION("lp_bounds2")
        {
            auto test = test::nlp_model("lp_bounds2", adname);
            auto m = test->nlp_model;
            REQUIRE(m.name() == "lp_bounds2");
            auto res = solver.solve(m);
            REQUIRE(test->check_results(test->model, res, 1e-4) == true);
        }
        SECTION("qp_bounds")
        {
            auto test = test::nlp_model("qp_bounds", adname);
            auto m = test->nlp_model;
            REQUIRE(m.name() == "qp_bounds");
            auto res = solver.solve(m);
            REQUIRE(test->check_results(test->model, res, 1e-5) == true);
        }
    }
    else {
        // GCOVR_EXCL_START
        SECTION("simplelp1")
        {
            auto test = test::nlp_model("simplelp1", adname);
            auto m = test->nlp_model;
            REQUIRE(m.name() == "simplelp1");
            auto res = solver.solve(m);
            REQUIRE(res->termination_condition == coek::TerminationCondition::solver_not_available);
        }
        // GCOVR_EXCL_STOP
    }
#endif
}
