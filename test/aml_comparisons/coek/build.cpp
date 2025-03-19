#include <iostream>
#include <string>
#include <vector>

#include "models/coek_models.hpp"

void print_help()
{
    std::cout << "coek_build [-d] <model> [<data> ...]" << std::endl;
    std::cout << std::endl << "TEST MODELS" << std::endl;
    print_models(std::cout);
    std::cout << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc <= 2) {
        print_help();
        return 1;
    }

    bool debug = false;
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
            model_name = args[i++];
            while (i < args.size())
                data.push_back(std::stoul(args[i++]));
        }
    }

    if (debug)
        std::cout << " Model: " << model_name << " Data: " << data[0] << std::endl;
    coek::Model model;
    try {
        create_instance(model, model_name, data);
    }
    catch (std::exception& e) {
        std::cout << "ERROR - " << e.what() << std::endl;
        return 1;
    }

#ifdef COEK_WITH_COMPACT_MODEL
    coek::CompactModel cmodel;
    try {
        create_instance(cmodel, model_name, data);
    }
    catch (std::exception& e) {
        std::cout << "ERROR - " << e.what() << std::endl;
        return 1;
    }
#endif

    std::cout << "coek_build - DONE" << std::endl;
    return 0;
}
