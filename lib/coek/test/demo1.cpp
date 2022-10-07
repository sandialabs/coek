//
// Load an JPOF file and illustrate resolves with mutable parameters
//
#include <coek/coek.hpp>
#include <iostream>
#include <map>
#include <vector>

const std::string currdir = COEK_TEST_DIR;

int main(int argc, char** argv)
{
    if (argc == 1) {
        std::cout << "demo1 <jpof-file> <n-solves>" << std::endl;
        return 0;
    }

    char* filename = argv[1];     // Filename
    int nsolves = atoi(argv[2]);  // Number of solves

    std::map<std::string, coek::Parameter> params;

    auto model = coek::read_problem_from_jpof_file(currdir + "/jpof/" + filename, params);
    double p = params["p"].value();

    auto vnames = model.variable_names();

    coek::NLPModel nlp(model, "cppad");
    coek::NLPSolver opt("ipopt");
    opt.set_option("print_level", 0);
    opt.load(nlp);

    for (int i = 0; i < nsolves; i++) {
        opt.resolve();
        std::cout << "Solve: " << 0 << std::endl;
        std::cout << "Objective: " << nlp.compute_f() << std::endl;
        std::cout << "p value: " << params["p"].value() << std::endl;
        std::cout << "Solution:" << std::endl;
        for (auto const& vname : vnames)
            std::cout << "  " << vname << "= " << model.get_variable(vname).value() << std::endl;
        std::cout << std::endl;

        // p = i+1
        params["p"].value(p + i + 1);
    }

    return 0;
}
