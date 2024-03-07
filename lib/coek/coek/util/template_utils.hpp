#pragma once

#include <map>
#include <set>

namespace coek {

// count_args

template <typename... Args>
constexpr std::size_t count_args(Args...)
{
    return sizeof...(Args);
}

// has_nonintegral_args

template <typename... ARGS>
struct has_nonintegral_args {
    static constexpr bool value = false;
};

template <typename T, typename... Args>
struct has_nonintegral_args<T, Args...> {
    static constexpr bool value
        = !std::is_integral<T>::value || has_nonintegral_args<Args...>::value;
};

// is_std_set

template<typename>
struct is_std_set : std::false_type {};

template<typename T>
struct is_std_set<std::set<T>> : std::true_type {};

// is_std_map

template<typename>
struct is_std_map : std::false_type {};

template<typename K, typename V>
struct is_std_map<std::map<K,V>> : std::true_type {};

// is_std_vector

template<typename>
struct is_std_vector : std::false_type {};

template<typename T>
struct is_std_vector<std::vector<T>> : std::true_type {};

// is_std_tuple

template<typename>
struct is_std_tuple : std::false_type {};

template<typename ...T>
struct is_std_tuple<std::tuple<T...>> : std::true_type {};

}  // namespace coek
