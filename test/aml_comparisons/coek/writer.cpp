#include <iostream>
#include <string>
#include <vector>

#include "models/coek_models.hpp"

void print_help()
{
    std::cout << "coek_writer [-d] <filename> <model> [<data> ...]" << std::endl;
    std::cout << std::endl << "TEST MODELS" << std::endl;
    print_models(std::cout);
    std::cout << std::endl;
    std::cout << "VALID FILENAME SUFFIXES\n"
                 "  lp     - Canonical LP file, written with FMT library\n"
                 "  fmtlp  - Canonical LP file, written with FMT library\n"
                 "  ostrlp - Canonical LP file, written with C++ ostream\n"
                 "  nl     - Canonical NL file, written with FMT library\n"
                 "  fmtnl  - Canonical NL file, written with FMT library\n"
                 "  ostrnl - Canonical NL file, written with C++ ostream\n"
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

    if (debug)
        std::cout << "Filename: " << filename << " Model: " << model_name << " Data: " << data[0]
                  << std::endl;
    coek::Model model;
    try {
        create_instance(model, model_name, data);
    }
    catch (std::exception& e) {
        std::cout << "ERROR - " << e.what() << std::endl;
        return 1;
    }
    model.write(filename);

    return 0;
}
