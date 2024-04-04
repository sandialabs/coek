#include <iostream>
#include <string>
#include <vector>

#include "models/coek_models.hpp"

void print_help()
{
    std::cout << "coek_solve0 [-d] <solver> <model> [<data> ...]" << std::endl;
    std::cout << std::endl << "TEST MODELS" << std::endl;
    print_models(std::cout);
    std::cout << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc <= 3) {
        print_help();
        return 1;
    }

    bool debug = false;
    std::string solver_name;
    std::string model_name;
    std::vector<size_t> data;

    std::vector<std::string> args(argv + 1, argv + argc);

    // Loop over command-line args
    size_t i = 0;
    while (i < args.size()) {
        if (args[i] == "-h" || args[i] == "--help") {
            print_help();
            return 0;
        }
        else if (args[i] == "-d") {
            debug = true;
            i++;
        }
        else {
            solver_name = args[i++];
            model_name = args[i++];
            while (i < args.size())
                data.push_back(std::stoul(args[i++]));
        }
    }

    coek::Model model;
    try {
        create_instance(model, model_name, data);
    }
    catch (std::exception& e) {
        std::cout << "ERROR - " << e.what() << std::endl;
        return 1;
    }

    if (solver_name == "gurobi") {
        coek::Solver solver;
        solver.initialize(solver_name);
        solver.set_option("OutputFlag", debug);
        solver.set_option("TimeLimit", 0.0);

        auto res = solver.solve(model);
        if (not check_optimal_termination(res)) {
            std::cout << "ERROR - failed to execute solver '" << solver_name << "'" << std::endl;
            std::cout << "MESSAGE - " << res->error_message << std::endl;
            return 2;
        }
    }
    else if (solver_name == "ipopt") {
        coek::NLPModel Model(model, "cppad");
        coek::NLPSolver solver;
        solver.initialize(solver_name);

        solver.set_option("max_iter", 1);
        solver.set_option("print_level", 1);

        auto res = solver.solve(Model);
        if (not check_optimal_termination(res)) {
            std::cout << "ERROR - failed to execute solver '" << solver_name << "'" << std::endl;
            std::cout << "MESSAGE - " << res->error_message << std::endl;
            return 2;
        }
    }

    return 0;
}
