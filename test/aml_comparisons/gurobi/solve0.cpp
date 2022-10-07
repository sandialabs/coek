#include <iostream>
#include <string>
#include <vector>

#include "models/gurobi_models.hpp"

void print_help()
{
    std::cout << "gurobi_solve0 [-d] <solver> <model> [<data> ...]" << std::endl;
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
            while (i < args.size()) data.push_back(std::stoul(args[i++]));
        }
    }

    GRBEnv env;
    GRBModel model(env);
    try {
        create_instance(model, model_name, data);
    }
    catch (std::exception& e) {
        std::cout << "ERROR - " << e.what() << std::endl;
        return 1;
    }

    model.set(GRB_IntParam_OutputFlag, debug);
    model.set(GRB_DoubleParam_TimeLimit, 0.0);
    model.optimize();

    return 0;
}
