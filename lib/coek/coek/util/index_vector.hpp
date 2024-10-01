#pragma once

#include <functional>
#include <cassert>
#include <cstddef>
#include <map>
#include <iostream>
#ifndef CUSTOM_INDEXVECTOR
#    include <vector>
#endif

namespace coek {

#ifdef CUSTOM_INDEXVECTOR

// class BaseExpressionTerm;
class IndexVectorCache;

// typedef BaseExpressionTerm* expr_pointer_t;
// typedef std::variant<int,expr_pointer_t> refarg_types;

class IndexVector {
   public:
    typedef int value_type;
    size_t len;
    value_type* data;

   public:
    IndexVector() : len(0), data(0) {}

    explicit IndexVector(value_type* _data) : len(static_cast<size_t>(_data[0])), data(_data + 1) {}

    explicit IndexVector(value_type* _data, size_t _len) : len(_len), data(_data) {}

    IndexVector(const IndexVector& other) : len(other.len), data(other.data) {}

    IndexVector& operator=(const IndexVector& other)
    {
        if (this == &other)
            return *this;

        len = other.len;
        data = other.data;
        return *this;
    }

    size_t size() const { return len; }

    value_type& operator[](size_t i) { return data[i]; }

    const value_type& operator[](size_t i) const { return data[i]; }

    bool operator<(const IndexVector& other) const
    {
        if (len == 1)
            return data[0] < other.data[0];

        for (size_t i = 0; i < len; i++) {
            if (data[i] < other.data[i])
                return true;
            else if (data[i] > other.data[i])
                return false;
        }
        return false;
    }

    bool operator==(const IndexVector& other) const
    {
        if (len == 1)
            return data[0] == other.data[0];
        else if (len == 2) {
            return (data[0] == other.data[0]) and (data[1] == other.data[1]);
        }
        value_type* curr = data;
        value_type* end = data + len;
        value_type* _other = other.data;

        for (; curr != end; ++curr, ++_other) {
            if (*curr != *_other)
                return false;
        }
        return true;
    }
};

class IndexVectorCache {
   public:
    typedef typename IndexVector::value_type value_type;
    size_t len;
    value_type* data;
    size_t curr;

   public:
    IndexVectorCache() : len(0), data(0), curr(0) {}
    IndexVectorCache(size_t _len) : len(_len), curr(0) { data = new value_type[len]; }

    IndexVectorCache(const IndexVectorCache& other) = delete;

    ~IndexVectorCache()
    {
        if (data)
            delete[] data;
    }

    void resize(size_t _len)
    {
        assert(len == 0);
        len = _len;
        if (data)
            delete[] data;
        data = new value_type[len];
    }

    IndexVector alloc(size_t _len)
    {
        // std::cout << "HERE" << curr << " " << _len << " " << len << std::endl;
        assert((curr + _len + 1) <= len);
        auto tmp = data + curr;
        *tmp = static_cast<value_type>(_len);
        curr += _len + 1;
        return IndexVector(tmp);
    }

    IndexVector clone(const IndexVector& x)
    {
        auto tmp = alloc(x.size());
        for (size_t i = 0; i < x.size(); ++i)
            tmp[i] = x[i];
        return tmp;
    }
};

#else

using IndexVector = std::vector<int>;

#endif
}  // namespace coek

namespace std {
template <>
struct hash<coek::IndexVector> {
    coek::IndexVector::value_type operator()(const coek::IndexVector& a) const;
};

inline std::ostream& operator<<(std::ostream& ostr, const coek::IndexVector& vec)
{
    for (size_t i = 0; i < vec.size(); i++) {
        ostr << vec[i] << " ";
    }
    return ostr;
}

}  // namespace std
