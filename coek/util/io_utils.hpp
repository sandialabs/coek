#pragma once

#include <iostream>
#include <list>
#include <string>

namespace coek
{

std::ostream& operator<<(std::ostream& ostr, const std::list<std::string>& vals)
{
auto it = vals.begin();
auto end = vals.end();
if (it == end)
    return ostr;
ostr << *it;
++it;

for (; it != end; ++it) {
    ostr << " " << *it;
    }
return ostr;
}


}


