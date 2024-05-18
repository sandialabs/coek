#pragma once

#include <functional>
#include <cassert>
#include <cstddef>
#include <map>
#include <iostream>

namespace coek {

// class BaseExpressionTerm;
class IndexVectorCache;

// typedef BaseExpressionTerm* expr_pointer_t;
// typedef std::variant<int,expr_pointer_t> refarg_types;

class IndexVector {
   public:
    typedef int vecdata_t;
    size_t len;
    vecdata_t* data;

   public:
    IndexVector() : len(0), data(0) {}

    explicit IndexVector(vecdata_t* _data) : len(static_cast<size_t>(_data[0])), data(_data + 1) {}

    explicit IndexVector(vecdata_t* _data, size_t _len) : len(_len), data(_data) {}

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

    vecdata_t& operator[](size_t i) { return data[i]; }

    const vecdata_t& operator[](size_t i) const { return data[i]; }

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
        vecdata_t* curr = data;
        vecdata_t* end = data + len;
        vecdata_t* _other = other.data;

        for (; curr != end; ++curr, ++_other) {
            if (*curr != *_other)
                return false;
        }
        return true;
    }
};

class IndexVectorCache {
   public:
    typedef typename IndexVector::vecdata_t vecdata_t;
    size_t len;
    vecdata_t* data;
    size_t curr;

   public:
    IndexVectorCache() : len(0), data(0), curr(0) {}
    IndexVectorCache(size_t _len) : len(_len), curr(0) { data = new vecdata_t[len]; }

    IndexVectorCache(const IndexVectorCache& other) = delete;

    ~IndexVectorCache()
    {
        if (len)
            delete[] data;
    }

    void resize(size_t _len)
    {
        assert(len == 0);
        len = _len;
        data = new vecdata_t[len];
    }

    IndexVector alloc(size_t _len)
    {
        assert((curr + _len + 1) <= len);
        auto tmp = data + curr;
        *tmp = static_cast<vecdata_t>(_len);
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

}  // namespace coek

namespace std {
template <>
struct hash<coek::IndexVector> {
    coek::IndexVector::vecdata_t operator()(const coek::IndexVector& a) const;
};

inline std::ostream& operator<<(std::ostream& ostr, const coek::IndexVector& vec)
{
    for (size_t i = 0; i < vec.size(); i++) {
        ostr << vec[i] << " ";
    }
    return ostr;
}

}  // namespace std
