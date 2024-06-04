#pragma once

#include <chrono>

namespace coek {

class TicTocTimer {
   protected:
    typedef std::chrono::steady_clock Time;

    Time::time_point t_start;

   public:
    void tic(const std::string& msg);
    void tic();

    double toc(const std::string& msg);
    double toc();
};

void tic(const std::string& msg);
void tic();

double toc(const std::string& msg);
double toc();

}  // namespace coek
