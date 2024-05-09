#include <coek/util/string_utils.hpp>

namespace coek {

std::string indent_string(const std::string& str, char c, unsigned int count)
{
    if (count == 0)
        return str;

    std::string indent = std::string(count, c);

    std::string ans(count, c);
    for (const char curr : str) {
        if (curr == '\n') {
            ans.push_back(curr);
            ans.append(indent);
        }
        else
            ans.push_back(curr);
    }
    return ans;
}

}  // namespace coek
