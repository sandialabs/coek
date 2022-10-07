#pragma once

#include <sstream>

#define runtime_assert(flag, descr)               \
    {                                             \
        bool condition = flag;                    \
        if (!condition) {                         \
            std::ostringstream ostr;              \
            ostr << descr;                        \
            throw std::runtime_error(ostr.str()); \
        }                                         \
    }
