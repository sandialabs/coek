#include <iostream>
#include <string>
#include <vector>
#include "gurobi_c++.h"

#include "models/gurobi_models.hpp"

void print_help()
{
    std::cout << "gurobi_writer [-d] <filename> <model> [<data> ...]" << std::endl;
    std::cout << std::endl << "TEST MODELS" << std::endl;
    print_models(std::cout);
    std::cout << std::endl;
    std::cout << "VALID FILENAME SUFFIXES\n"
                 "  lp     - Canonical LP file, written with GUROBI library\n"
                 "\n";
}

int main(int argc, char* argv[])
{
    if (argc <= 3) {
        print_help();
        return 1;
    }

    bool debug = false;
    std::string filename;
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
            filename = args[i++];
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
    try {
        model.write(filename);
    }
    catch (std::exception& e) {
        std::cout << "ERROR - " << e.what() << std::endl;
        return 2;
    }
    catch (GRBException& e) {
        std::cout << "ERROR - " << e.getMessage() << std::endl;
        return 3;
    }

    return 0;
}
