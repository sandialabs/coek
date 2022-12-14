#pragma once

#include <iostream>
#include <list>
#include <vector>
#include <string>

namespace std {

template <typename TYPE>
inline std::ostream& operator<<(std::ostream& ostr, const std::vector<TYPE>& vals)
{
    for (auto& v : vals) ostr << v << " ";
    return ostr;
}

template <typename TYPE>
inline std::ostream& operator<<(std::ostream& ostr, const std::list<TYPE>& vals)
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

}  // namespace std
