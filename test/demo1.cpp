//
// Load and JPOF file and illustrate resolves with mutable parameters
//
//#include <chrono>
#include <map>
#include <vector>
#include <iostream>
//#include <random>
//#include <functional>
#include <coek/coek.hpp>

const std::string currdir = COEK_TEST_DIR;


int main(int argc, char** argv)
{
if (argc == 1) {
    std::cout << "demo1 <n-solves>" << std::endl;
    return 0;
    }

int nsolves = atoi(argv[1]);    // Number of solvers

std::map<int,int> vmap;

auto model = coek::read_problem_from_jpof_file(currdir+"jpof/dsrosen.jpof", vmap);

coek::NLPModel nlp(model, "cppad");
coek::NLPSolver opt("ipopt");
opt.set_option("print_level", 0);
opt.load(nlp);

for (int i=0; i<nsolves; i++) {
    opt.resolve();
    std::cout << "Solve: " << 0 << "  Objective: " << nlp.compute_f() << std::endl;

/*
    for (int n=0; n<N; n++)
        c[n].set_value( uniform() );
*/
    }

return 0;
}
