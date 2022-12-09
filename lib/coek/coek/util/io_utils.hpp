#pragma once

#include <iostream>
#include <list>
#include <vector>
#include <string>

namespace coek {

template <typename TYPE>
std::ostream& operator<<(std::ostream& ostr, const std::vector<TYPE>& vec)
{
    for (size_t i = 0; i < vec.size(); i++) ostr << vec[i] << " ";
    return ostr;
}

inline std::ostream& operator<<(std::ostream& ostr, const std::list<std::string>& vals)
{
    auto it = vals.begin();
    auto end = vals.end();
    if (it == end) return ostr;
    ostr << *it;
    ++it;

    for (; it != end; ++it) {
        ostr << " " << *it;
    }
    return ostr;
}

}  // namespace coek
