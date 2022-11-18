//
// The Subrange and ValueGenerator classes are adapted from
// examples in:
//
// Embracing Modern C++ Safely
// J. Lakos, V. Romeo, R. Khlebnikov, A. Meredith
// Addison-Wesley, 2022.
//
// See also https://github.com/klmr/cpp11-range, which implements similar
// functionality.
//
#pragma once

#include <type_traits>

namespace coek {

namespace seq {

template <typename C>
struct has_size {
    template <typename T>
    static auto check(T*) ->
        typename std::is_integral<decltype(std::declval<T const>().size())>::type;

    template <typename>
    static auto check(...) -> std::false_type;

    using type = decltype(check<C>(0));
    static constexpr bool value = type::value;
};

template <typename T>
class SequentialValueGenerator {
   private:
    T value;

   public:
    explicit SequentialValueGenerator(T start) : value(start) {}

    T operator*() const { return value; }

    SequentialValueGenerator& operator++()
    {
        ++value;
        return *this;
    }

    friend bool operator==(const SequentialValueGenerator& a, const SequentialValueGenerator& b)
    {
        return a.value == b.value;
    }

    friend bool operator!=(const SequentialValueGenerator& a, const SequentialValueGenerator& b)
    {
        return not(a == b);
    }
};

template <typename T>
class Subrange {
   private:
    SequentialValueGenerator<T> d_begin, d_end;

   public:
    using iterator = SequentialValueGenerator<T>;

    Subrange(T b, T e) : d_begin(b), d_end(e) {}

    iterator begin() const { return d_begin; }
    iterator end() const { return d_end; }
};

template <typename T>
class ValueGenerator {
   private:
    T value;
    T step;

   public:
    explicit ValueGenerator(T start, T _step) : value(start), step(_step) {}

    T operator*() const { return value; }

    ValueGenerator& operator++()
    {
        value += step;
        return *this;
    }

    bool operator==(const ValueGenerator& b) const
    {
        return step > 0 ? value >= b.value : value < b.value;
    }

    bool operator!=(const ValueGenerator& b) const { return not(*this == b); }
};

template <typename T>
class StepSubrange {
   private:
    ValueGenerator<T> d_begin, d_end;

   public:
    using iterator = ValueGenerator<T>;

    StepSubrange(T b, T e, T s) : d_begin(b, s), d_end(e, 0) {}

    iterator begin() const { return d_begin; }
    iterator end() const { return d_end; }
};

}  // namespace seq

// 0, 1, ..., stop-1
template <class T>
seq::Subrange<T> range(T stop)
{
    return {0, stop};
}

// start, start+1, ..., stop-1
template <class T>
seq::Subrange<T> range(T start, T stop)
{
    return {start, stop};
}

// start, start+step, ..., stop-1
template <class T>
seq::StepSubrange<T> range(T start, T stop, T step)
{
    return {start, stop, step};
}

// 1, 2, ..., stop
template <class T>
seq::Subrange<T> sequence(T stop)
{
    return {1, stop + 1};
}

// start, start+1, ..., stop
template <class T>
seq::Subrange<T> sequence(T start, T stop)
{
    return {start, stop + 1};
}

// start, start+step, ..., stop
template <class T>
seq::StepSubrange<T> sequence(T start, T stop, T step)
{
    return {start, stop + 1, step};
}

// 0, ..., obj.size()-1
template <typename T, typename = typename std::enable_if<seq::has_size<T>::value>>
auto indices(const T& obj) -> seq::Subrange<decltype(obj.size())>
{
    return {0, obj.size()};
}

// 0, ..., obj.size()-1
template <class T>
seq::Subrange<T> indices(std::initializer_list<T>&& obj)
{
    return {0, obj.size()};
}

}  // namespace coek
