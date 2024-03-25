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
        size_t m = atoll(argv[2]);  // Test Num
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

    else if (n == 1) {
        // Benchmark var initialization
        size_t m = atoll(argv[2]);  // Test Num
        std::cout << "Timing var initialization: " << m << std::endl;

        {
        coek::tic();
        std::vector<coek::Variable> x(m);
        for (size_t i=0; i<m; i++)
            x[i].value(1.0).lower(0.0).upper(1.0);
        double val = x[0].value();
        coek::toc("Loop over indices of std::vector<coek::Variable>");
        std::cout << x.size() << " " << val << std::endl;
        }

        {
        coek::tic();
        std::vector<coek::Variable> x(m);
        for (auto& var : x)
            var.value(1.0).lower(0.0).upper(1.0);
        double val = x[0].value();
        coek::toc("Range loop over std::vector<coek::Variable>");
        std::cout << x.size() << " " << val << std::endl;
        }

#ifdef __cpp_lib_variant
        {
        coek::tic();
        auto x = coek::variable(m);
        for (size_t i=0; i<m; i++)
            x(i).value(1.0).lower(0.0).upper(1.0);
        double val = x(0).value();
        coek::toc("Loop over indices of VariableArray");
        std::cout << x.size() << " " << val << std::endl;
        }

        {
        coek::tic();
        auto x = coek::variable(m).value(1.0).lower(0.0).upper(1.0);
        double val = x(0).value();
        coek::toc("Loop over indices of VariableArray");
        std::cout << x.size() << " " << val << std::endl;
        }
#endif
        }

    return 0;
}
