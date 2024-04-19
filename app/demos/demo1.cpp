#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <random>
#include <coek/coek.hpp>


coek::Model knapsack(size_t N)
{
    N *= 1000;
    double W = N / 10.0;
    
    std::mt19937 rng(10000);
    std::uniform_real_distribution<double> distribution(0, 1);
    auto uniform = std::bind(distribution, rng);
    
    std::vector<double> v(N);
    std::vector<double> w(N);
    for (size_t n : coek::range(N)) {
        v[n] = uniform();
        w[n] = uniform();
    }
    
    auto model = coek::Model();

    auto x = coek::variable(N).bounds(0, 1).value(0);
    model.add(x);
    
    // obj
    auto obj = coek::expression();
    for (size_t n : coek::range(N)) obj += v[n] * x(n);
    model.add_objective(obj);

    // con
    auto con = coek::expression();
    for (size_t n : coek::range(N)) con += w[n] * x(n);
    model.add(con <= W);

    return model;
}


int main(int argc, char* argv[])
{
    if (argc != 3) {
        std::cout << "demo1 <solver> <N>" << std::endl;
        return 1;
    }

    std::string solver_name = argv[1];
    size_t N = std::stoul(argv[2]);

    auto model = knapsack(N);

    if (solver_name == "gurobi") {
        coek::Solver solver;
        solver.initialize(solver_name);
        solver.set_option("OutputFlag", 1);
        //solver.set_option("TimeLimit", 0.0);

        auto res = solver.solve(model);
        if (not check_optimal_termination(res)) {
            std::cout << "ERROR - failed to execute solver '" << solver_name << "'" << std::endl;
            std::cout << "MESSAGE - " << res->error_message << std::endl;
            return 2;
        }
    }
    else if (solver_name == "ipopt") {
        coek::NLPModel nlpmodel(model, "cppad");
        coek::NLPSolver solver;
        solver.initialize(solver_name);
        //solver.set_option("max_iter", 1);
        //solver.set_option("print_level", 1);

        auto res = solver.solve(nlpmodel);
        if (not check_optimal_termination(res)) {
            std::cout << "ERROR - failed to execute solver '" << solver_name << "'" << std::endl;
            std::cout << "MESSAGE - " << res->error_message << std::endl;
            return 2;
        }
    }

    return 0;
}
