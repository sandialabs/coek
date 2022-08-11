#pragma once

namespace coek {

//
// The Subrange and ValueGenerator classes are adapted from 
// examples in:
// 
// Embracing Modern C++ Safely
// J. Lakos, V. Romeo, R. Khlebnikov, A. Meredith
// Addison-Wesley, 2022.
//
template <typename Iter>
class Subrange
{
private:

    Iter d_begin, d_end;

public:

    using iterator = Iter;

    Subrange(Iter b, Iter e)
        : d_begin(b), d_end(e) {}

    iterator begin() const { return d_begin; }
    iterator end() const { return d_end; }
};


template <typename T>
class ValueGenerator
{
private:

    T value;
    T stop;
    T step;
    bool status;

public:

    explicit ValueGenerator(const T& start, const T& _stop, const T& _step) 
                : value(start), stop(_stop), step(_step), status(false) {}

    T operator*() const
        {return value;}

    ValueGenerator& operator++()
        {
        value += step;
        status = value >= stop;
        return *this;
        }

    friend bool operator!=(const ValueGenerator& a, const ValueGenerator& b)
        { return a.status == b.status; }
};


template <class T>
Subrange<ValueGenerator<T>> range(T stop)
{ return {ValueGenerator<T>(0,stop,1), ValueGenerator<T>(0,0,0)}; }

template <class T>
Subrange<ValueGenerator<T>> range(T start, T stop)
{ return {ValueGenerator<T>(start,stop,1), ValueGenerator<T>(0,0,0)}; }

template <class T>
Subrange<ValueGenerator<T>> range(T start, T stop, T step)
{ return {ValueGenerator<T>(start,stop,step), ValueGenerator<T>(0,0,0)}; }

template <class T>
Subrange<ValueGenerator<T>> sequence(T stop)
{ return {ValueGenerator<T>(0,stop+1,1), ValueGenerator<T>(0,0,0)}; }

template <class T>
Subrange<ValueGenerator<T>> sequence(T start, T stop)
{ return {ValueGenerator<T>(start,stop+1,1), ValueGenerator<T>(0,0,0)}; }

template <class T>
Subrange<ValueGenerator<T>> sequence(T start, T stop, T step)
{ return {ValueGenerator<T>(start,stop+1,step), ValueGenerator<T>(0,0,0)}; }

}
