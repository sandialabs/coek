//
// Micro tests for Coek
//
#include <coek/coek.hpp>
#include <iostream>
#include <map>
#include <vector>
#include <coek/util/tictoc.hpp>

const std::string currdir = COEK_TEST_DIR;

int main(int argc, char** argv)
{
    if (argc == 1) {
        std::cout << "coek_micro_test <n>" << std::endl;
        return 0;
    }

    int n = atoi(argv[1]);  // Test No

    if (n == 0) {
        // Benchmark loops
        size_t m = atoll(argv[2]);  // Test No
        std::cout << "Timing Loops: " << m << std::endl;
        double val=0.0;
        coek::tic();
        for (size_t i=0; i<m; i++)
            val += 1.1;
        coek::toc("Simple for loop");
        std::cout << val << std::endl;

        val=0.0;
        coek::tic();
        for (size_t i : coek::range<size_t>(m))
            val += 1.1;
        coek::toc("For loop with coek::range<size_t>(m)");
        std::cout << val << std::endl;

        val=0.0;
        coek::tic();
        for (size_t i : coek::range<size_t>(0,m,1))
            val += 1.1;
        coek::toc("For loop with coek::range<size_t>(0,m,1)");
        std::cout << val << std::endl;
        }

    return 0;
}
