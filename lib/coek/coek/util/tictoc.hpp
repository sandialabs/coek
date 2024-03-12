#pragma once

#include <chrono>


namespace coek {

class TicTocTimer
{
protected:

    std::chrono::_V2::system_clock::time_point t_start;

public:

    void tic(const std::string& msg);
    void tic() {tic("");}

    double toc(const std::string& msg);
    double toc() {return toc("");}
};

void tic(const std::string& msg);
inline void tic() {tic("");}

double toc(const std::string& msg);
inline double toc() {return toc("");}

}
