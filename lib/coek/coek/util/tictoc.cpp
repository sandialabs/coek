#include <iomanip>
#include <sstream>
#include <iostream>
#include <coek/util/tictoc.hpp>

namespace coek {

namespace {

TicTocTimer global_timer;

}

void TicTocTimer::tic() { t_start = Time::now(); }

void TicTocTimer::tic(const std::string& msg)
{
    std::cout << msg << std::endl;
    t_start = Time::now();
    std::cout << "[";
    std::cout.width(10);
    std::cout << std::fixed << std::setprecision(2) << 0.0 << "] " << msg << std::endl;
}

double TicTocTimer::toc()
{
    auto t_end = Time::now();
    std::chrono::duration<double> diff = t_end - t_start;
    t_start = t_end;
    return diff.count();  // sec
}

double TicTocTimer::toc(const std::string& msg)
{
    auto t_end = Time::now();
    std::chrono::duration<double> diff = t_end - t_start;
    double sec = diff.count();
    t_start = t_end;
    std::cout << "[";
    std::cout.width(10);
    std::cout << std::fixed << std::setprecision(2) << sec << "] " << msg << std::endl;
    return sec;
}

void tic() { global_timer.tic(); }

void tic(const std::string& msg) { global_timer.tic(msg); }

double toc() { return global_timer.toc(); }

double toc(const std::string& msg) { return global_timer.toc(msg); }

}  // namespace coek
