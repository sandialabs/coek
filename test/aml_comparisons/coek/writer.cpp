#include <vector>
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
std::cout <<
"VALID FILENAME SUFFIXES\n"
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
size_t i=0;
while (i<args.size()) {
    if (args[i] == "-h" || args[i] == "--help") {
        print_help();
        return 0;
    } else if (args[i] == "-d") {
        debug = true;
        i++;
    } else {
        filename = args[i++];
        model_name = args[i++];
        while (i<args.size()) 
            data.push_back(std::stoul(args[i++]));
    }
}

bool constructed = false;
coek::Model model;
try {
    constructed = create_instance(model, model_name, data);
    }
catch (std::exception& e) {
    std::cout << "ERROR - " << e.what() << std::endl;
    return 1;
    }
if (constructed) {
    model.write(filename);
    return 0;
    }

#ifdef COEK_WITH_COMPACT_MODEL
coek::CompactModel cmodel;
try {
    constructed = create_instance(cmodel, model_name, data);
    }
catch (std::exception& e) {
    std::cout << "ERROR - " << e.what() << std::endl;
    return 1;
    }
if (constructed) {
    cmodel.write(filename);
    return 0;
    }
#endif

return 1;
}

