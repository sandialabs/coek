#pragma once

namespace coek {

namespace {
//
// The Subrange and ValueGenerator classes are adapted from
// examples in:
//
// Embracing Modern C++ Safely
// J. Lakos, V. Romeo, R. Khlebnikov, A. Meredith
// Addison-Wesley, 2022.
//
template <typename Iter>
class Subrange {
   private:
    Iter d_begin, d_end;

   public:
    using iterator = Iter;

    Subrange(Iter b, Iter e) : d_begin(b), d_end(e) {}

    iterator begin() const { return d_begin; }
    iterator end() const { return d_end; }
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
        value++;
        return *this;
    }

    friend bool operator!=(const SequentialValueGenerator& a, const SequentialValueGenerator& b)
    {
        return a.value < b.value;
    }
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

    friend bool operator!=(const ValueGenerator& a, const ValueGenerator& b)
    {
        return a.value < b.value;
    }
};

}  // namespace

// 0, 1, ..., stop-1
template <class T>
Subrange<SequentialValueGenerator<T>> range(T stop)
{
    return {SequentialValueGenerator<T>(0), SequentialValueGenerator<T>(stop)};
}

// start, start+1, ..., stop-1
template <class T>
Subrange<SequentialValueGenerator<T>> range(T start, T stop)
{
    return {SequentialValueGenerator<T>(start), SequentialValueGenerator<T>(stop)};
}

// start, start+step, ..., stop-1
template <class T>
Subrange<ValueGenerator<T>> range(T start, T stop, T step)
{
    return {ValueGenerator<T>(start, step), ValueGenerator<T>(stop, 0)};
}

// 1, 2, ..., stop
template <class T>
Subrange<SequentialValueGenerator<T>> sequence(T stop)
{
    return {SequentialValueGenerator<T>(1), SequentialValueGenerator<T>(stop + 1)};
}

// start, start+1, ..., stop
template <class T>
Subrange<SequentialValueGenerator<T>> sequence(T start, T stop)
{
    return {SequentialValueGenerator<T>(start), SequentialValueGenerator<T>(stop + 1)};
}

// start, start+step, ..., stop
template <class T>
Subrange<ValueGenerator<T>> sequence(T start, T stop, T step)
{
    return {ValueGenerator<T>(start, step), ValueGenerator<T>(stop + 1, 0)};
}

}  // namespace coek
