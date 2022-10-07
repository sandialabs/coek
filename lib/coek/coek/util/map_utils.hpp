#pragma once

#include <map>
#include <set>

template <typename Key, typename Value>
std::set<Key> map_keys(std::map<Key, Value> const& input)
{
    std::set<Key> keys;
    for (auto const& element : input) keys.insert(element.first);
    return keys;
}
