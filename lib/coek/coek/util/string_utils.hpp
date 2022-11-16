#pragma once

#include <string>

namespace coek {

inline bool ends_with(const std::string& str, const std::string& suffix)
{
    return str.size() >= suffix.size()
           && 0 == str.compare(str.size() - suffix.size(), suffix.size(), suffix);
}

inline bool starts_with(const std::string& str, const std::string& prefix)
{
    return str.size() >= prefix.size() && (str.rfind(prefix, 0) == 0);
}

}  // namespace coek
