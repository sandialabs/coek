#pragma once

namespace coek {

template <typename... Args>
constexpr std::size_t count_args(Args...)
{
    return sizeof...(Args);
}

template <typename... ARGS>
struct has_nonintegral_args {
    static constexpr bool value = false;
};

template <typename T, typename... Args>
struct has_nonintegral_args<T, Args...> {
    static constexpr bool value
        = !std::is_integral<T>::value || has_nonintegral_args<Args...>::value;
};

}  // namespace coek
