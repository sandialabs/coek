#include <iomanip>
#include <sstream>
#include <iostream>
#include <coek/util/tictoc.hpp>

namespace coek {

namespace {

TicTocTimer global_timer;

}

void TicTocTimer::tic(const std::string& msg)
{
    std::cout << msg << std::endl;
    t_start = std::chrono::high_resolution_clock::now();
    std::cout << "[";
    std::cout.width(10);
    std::cout << std::fixed << std::setprecision(2) << 0.0 << "] " << msg << std::endl;
}

double TicTocTimer::toc(const std::string& msg)
{
    auto t_end = std::chrono::high_resolution_clock::now();
    auto sec = (t_end - t_start).count() * 1E-9;
    std::cout << "[";
    std::cout.width(10);
    std::cout << std::fixed << std::setprecision(2) << sec << "] " << msg << std::endl;
    t_start = t_end;
    return sec;
}

void tic(const std::string& msg) { global_timer.tic(msg); }

double toc(const std::string& msg) { return global_timer.toc(msg); }

}  // namespace coek
