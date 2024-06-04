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

    coek::NLPModel nlp_model;

   public:
    virtual ~TestModel() {}

    virtual bool check_results(coek::Model& model, std::shared_ptr<coek::SolverResults>& results,
                               double tol = 1e-7);
    // virtual bool check_results(coek::NLPModel& model, std::shared_ptr<coek::SolverResults>&
    // results, double tol=1e-7);
};

std::shared_ptr<TestModel> model(const std::string& name);
std::shared_ptr<TestModel> nlp_model(const std::string& name, const std::string& adname);

}  // namespace test
