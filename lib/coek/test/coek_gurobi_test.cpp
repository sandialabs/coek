// #include "catch2/catch_test_macros.hpp"
// #include "catch2/catch_approx.hpp"
// #include "catch2/generators/catch_generators.hpp"

#include <cassert>
#include "coek/coek.hpp"
#include "test/solver/TestModel.hpp"

coek::Solver test_solver()
{
    coek::Solver solver("gurobi");
    solver.set_option("OutputFlag", 0);
    return solver;
}

int main()
{
    //
    // Gurobi Checks
    //

    auto solver = test_solver();
    if (solver.available()) {
        // SECTION("empty")
        {
            std::cout << "empty" << std::endl;
            auto solver = test_solver();
            auto test = test::model("empty");
            auto m = test->model;
            assert(m.name() == "empty");
            auto res = solver.solve(m);
            assert(res->termination_condition == coek::TerminationCondition::empty_model);
        }
        // SECTION("rosenbr")
        {
            std::cout << "rosenbr" << std::endl;
            auto solver = test_solver();
            auto test = test::model("rosenbr");
            auto m = test->model;
            assert(m.name() == "rosenbr");
            auto res = solver.solve(m);
            assert(res->termination_condition
                   == coek::TerminationCondition::invalid_model_for_solver);
        }
        // SECTION("simplelp1")
        {
            std::cout << "simplelp1" << std::endl;
            auto solver = test_solver();
            auto test = test::model("simplelp1");
            auto m = test->model;
            assert(m.name() == "simplelp1");
            auto res = solver.solve(m);
            assert(test->check_results(m, res) == true);
        }
        // SECTION("simpleqp1")
        {
            std::cout << "simpleqp1" << std::endl;
            auto solver = test_solver();
            auto test = test::model("simpleqp1");
            auto m = test->model;
            assert(m.name() == "simpleqp1");
            auto res = solver.solve(m);
            assert(test->check_results(m, res) == true);
        }
        // SECTION("simpleqp2")
        {
            std::cout << "simpleqp2" << std::endl;
            auto solver = test_solver();
            auto test = test::model("simpleqp2");
            auto m = test->model;
            assert(m.name() == "simpleqp2");
            auto res = solver.solve(m);
            assert(test->check_results(m, res) == true);
        }
        // SECTION("simpleqp3")
        {
            std::cout << "simpleqp3" << std::endl;
            auto solver = test_solver();
            auto test = test::model("simpleqp3");
            auto m = test->model;
            assert(m.name() == "simpleqp3");
            auto res = solver.solve(m);
            assert(test->check_results(m, res) == true);
        }
        // SECTION("simpleqp4")
        {
            std::cout << "simpleqp4" << std::endl;
            auto solver = test_solver();
            auto test = test::model("simpleqp4");
            auto m = test->model;
            assert(m.name() == "simpleqp4");
            auto res = solver.solve(m);
            assert(test->check_results(m, res) == true);
        }
        // SECTION("lp_bounds1")
        {
            std::cout << "lp_bounds" << std::endl;
            auto solver = test_solver();
            auto test = test::model("lp_bounds1");
            auto m = test->model;
            assert(m.name() == "lp_bounds1");
            auto res = solver.solve(m);
            assert(test->check_results(m, res) == true);
        }
        // SECTION("lp_bounds2")
        {
            std::cout << "lp_bounds2" << std::endl;
            auto solver = test_solver();
            auto test = test::model("lp_bounds2");
            auto m = test->model;
            assert(m.name() == "lp_bounds2");
            auto res = solver.solve(m);
            assert(test->check_results(m, res) == true);
        }
        // SECTION("qp_bounds")
        {
            std::cout << "qp_bounds" << std::endl;
            auto solver = test_solver();
            auto test = test::model("qp_bounds");
            auto m = test->model;
            assert(m.name() == "qp_bounds");
            auto res = solver.solve(m);
            assert(test->check_results(m, res) == true);
        }
    }
    else {
        // SECTION("simplelp1")
        {
            std::cout << "simplelp1 - license issue" << std::endl;
            auto solver = test_solver();
            auto test = test::model("simplelp1");
            auto m = test->model;
            assert(m.name() == "simplelp1");
            auto res = solver.solve(m);
            auto ok = (res->termination_condition == coek::TerminationCondition::solver_not_available)
                   or (res->termination_condition == coek::TerminationCondition::license_problems);
            std::cout << "Unexpected termination condition: " << coek::to_string(res->termination_condition) << std::endl;
            std::cout << "Error message: " << res->error_message << std::endl;
            assert(ok == true);
        }
    }
}
