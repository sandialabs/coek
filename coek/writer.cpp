#include <vector>
#include <iostream>
#include <string>
#include <vector>
#include <coek/coek.hpp>
#include "models/coek_models.hpp"

int main(int argc, char* argv[])
{
if (argc <= 3) {
    std::cout << "coek_writer [-d] <filename> <model> [<data> ...]" << std::endl;
    std::cout << std::endl << "TEST MODELS" << std::endl;
    print_models(std::cout);
    std::cout << std::endl;
    return 1;
    }

bool debug = false;
std::string filename;
std::string model_name;
std::vector<int> data;

std::vector<std::string> args(argv + 1, argv + argc);

// Loop over command-line args
size_t i=0;
while (i<args.size()) {
    if (args[i] == "-h" || args[i] == "--help") {
        std::cout << "coek_writer [-d] <filename> <model> [<data> ...]" << std::endl;
        std::cout << std::endl << "TEST MODELS" << std::endl;
        print_models(std::cout);
        std::cout << std::endl;
        return 0;
    } else if (args[i] == "-d") {
        debug = true;
        i++;
    } else {
        filename = args[i++];
        model_name = args[i++];
        while (i<args.size()) 
            data.push_back(std::stoi(args[i++]));
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
model.write(filename);

return 0;
}

