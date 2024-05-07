#include <fmt/core.h>
#include "test/solver/TestModel.hpp"
#include "coek/solvers/solver_results.hpp"
#include <coek/util/io_utils.hpp>

namespace test {

bool TestModel::check_results(coek::Model& model, std::shared_ptr<coek::SolverResults>& results)
{
    if (results->termination_condition
        != coek::TerminationCondition::convergence_criteria_satisfied) {
        std::cout << fmt::format(
            "TEST ERROR: Unexpected termination condition ({} != {})",
            to_string(results->termination_condition),
            to_string(coek::TerminationCondition::convergence_criteria_satisfied))
                  << std::endl;
        std::cout << to_string(*results, 4) << std::endl;
        return false;
    }

    std::vector<double> primal;
    for (auto& v : model.get_variables())
        primal.push_back(v.value());
    if (primal_solution.size() != primal.size()) {
        std::cout << "TEST ERROR: Difference in number of primal variables" << std::endl;
        std::cout << "\t" << primal << std::endl;
        std::cout << "\t" << primal_solution << std::endl;
        std::cout << to_string(*results, 4) << std::endl;
        return false;
    }

    for (size_t i = 0; i < primal.size(); i++) {
        if (std::fabs(primal[i] - primal_solution[i]) > 1e-7) {
            std::cout << fmt::format("TEST ERROR: Difference primal solution at {} ({} != {})", i,
                                     primal[i], primal_solution[i])
                      << std::endl;
            std::cout << "\t" << primal << std::endl;
            std::cout << "\t" << primal_solution << std::endl;
            std::cout << to_string(*results, 4) << std::endl;
            return false;
        }
    }

    if (std::fabs(results->objective_value - optimal_objective) > 1e-7) {
        std::cout << fmt::format("TEST ERROR: Unexpected objective values ({} != {})",
                                 results->objective_value, optimal_objective)
                  << std::endl;
        std::cout << to_string(*results, 4) << std::endl;
        return false;
    }

    return true;
}

class EmptyModel : public TestModel {
   public:
    EmptyModel() { model.name("empty"); }
};

class Rosenbrock : public TestModel {
   public:
    Rosenbrock()
    {
        primal_solution = {1, 1};
        optimal_objective = 0.0;

        // Model
        model.name("rosenbr");
        auto x0 = model.add(coek::variable().value(1));
        auto x1 = model.add(coek::variable().value(2));

        model.add_objective(100 * pow(x1 - pow(x0, 2), 2) + pow(x0 - 1, 2));
    }
};

class SimpleLP1 : public TestModel {
   public:
    SimpleLP1()
    {
        primal_solution = {375, 250};
        optimal_objective = 28750.0;

        // Model
        model.name("simplelp1");
        auto& m = model;
        auto x = m.add(coek::variable("x").bounds(0, m.inf));
        auto y = m.add(coek::variable("y").bounds(0, m.inf));

        m.add_objective(50 * x + 40 * y).sense(m.maximize);
        m.add_constraint(2 * x + 3 * y <= 1500);
        m.add_constraint(2 * x + y <= 1000);
    }
};

class SimpleQP1 : public TestModel {
   public:
    SimpleQP1()
    {
        primal_solution = {375, 250};
        optimal_objective = 9531250;

        // Model
        model.name("simpleqp1");
        auto& m = model;
        auto x = m.add(coek::variable("x").bounds(0, m.inf));
        auto y = m.add(coek::variable("y").bounds(0, m.inf));

        m.add_objective(50 * x * x + 40 * y * y);
        m.add_constraint(2 * x + 3 * y >= 1500);
        m.add_constraint(2 * x + y >= 1000);
    }
};

class SimpleQP2 : public TestModel {
   public:
    SimpleQP2()
    {
        primal_solution = {375, 250};
        optimal_objective = 2875000;

        // Model
        model.name("simpleqp2");
        auto& m = model;
        auto x = m.add(coek::variable("x").bounds(0, m.inf));
        auto y = m.add(coek::variable("y").bounds(0, m.inf));

        m.add_objective(10 * x * x + 5 * x * y + 4 * y * x + 10 * y * y);
        m.add_constraint(2 * x + 3 * y >= 1500);
        m.add_constraint(2 * x + y >= 1000);
    }
};

class LP_bounds1 : public TestModel {
   public:
    LP_bounds1()
    {
        primal_solution = {1, -1, 1, -1, 1, 1, -1, 1, -1, 1};
        optimal_objective = 10.0;

        // Model
        model.name("lp_bounds1");
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

        model.add_objective(a - b + c - d + e + aa - bb + cc - dd + ee);

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
    }
};

class LP_bounds2 : public TestModel {
   public:
    LP_bounds2()
    {
        primal_solution = {0, -2, 0, -2, 0, 0, -2, 0, -2, 0, 1};
        optimal_objective = 8.0;

        // Model
        model.name("lp_bounds2");
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

        auto x = model.add_variable().value(1.0);
        x.fix();

        model.add_objective(a - b + c - d + e + aa - bb + cc - dd + ee);

        model.add(a + x >= 1);
        model.add(b + x <= -1);
        model.add(aa + x - 1 >= 0);
        model.add(bb + x + 1 <= 0);
        model.add(inequality(1, c + x, 3));
        model.add(inequality(-3, d + x, -1));
        model.add(inequality(0, cc + x - 1, 2));
        model.add(inequality(-2, dd + x + 1, 0));
        model.add(e + x == 1);
        model.add(ee + x - 1 == 0);
    }
};

class QP_bounds : public TestModel {
   public:
    QP_bounds()
    {
        primal_solution = {1, -1, 1, -1, 1, 1, -1, 1, -1, 1};
        optimal_objective = 10.0;

        // Model
        model.name("qp_bounds");
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
    }
};

std::shared_ptr<TestModel> model(const std::string& name)
{
    if (name == "rosenbr")
        return std::make_shared<Rosenbrock>();
    if (name == "simplelp1")
        return std::make_shared<SimpleLP1>();
    if (name == "simpleqp1")
        return std::make_shared<SimpleQP1>();
    if (name == "simpleqp2")
        return std::make_shared<SimpleQP2>();
    if (name == "qp_bounds")
        return std::make_shared<QP_bounds>();
    if (name == "lp_bounds1")
        return std::make_shared<LP_bounds1>();
    if (name == "lp_bounds2")
        return std::make_shared<LP_bounds2>();

    return std::make_shared<EmptyModel>();
}

}  // namespace test

#if 0
// quad
coek::Model quad_vector(std::vector<coek::Parameter>& p);
std::vector<double> quad_soln1{0.5, 0.5, 0.5, 0.5, 0.5};
std::vector<double> quad_soln2{-0.5, -0.5, -0.5, -0.5, -0.5};

// invquad
coek::Model invquad_vector(std::vector<coek::Parameter>& p);
#    if __cpp_lib_variant
coek::Model invquad_array(std::vector<coek::Parameter>& p);
void invquad_array_solve();
void invquad_array_resolve();
#    endif
std::vector<double> invquad_soln_5{-10, -10, -10, -10, -10};

    REQUIRE(variables.size() == soln.size());
    for (size_t i = 0; i < variables.size(); i++) {
        REQUIRE(variables[i].value() == Catch::Approx(soln[i]));
    }
}
#endif
