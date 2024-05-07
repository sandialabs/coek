#pragma once

#include <cmath>
#include <vector>
#include <memory>

#include "coek/coek.hpp"

namespace test {

class TestModel {
   public:
    std::vector<double> primal_solution = {};

    std::vector<double> dual_solution = {};

    std::vector<double> slacks = {};

    double optimal_objective = std::nan("");

    coek::Model model;

    virtual bool check_results(coek::Model& model, std::shared_ptr<coek::SolverResults>& results);
};

std::shared_ptr<TestModel> model(const std::string& name);

}  // namespace test
