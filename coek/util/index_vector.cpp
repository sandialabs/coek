#include <coek/util/index_vector.hpp>


typedef typename coek::IndexVector::vecdata_t vecdata_t;


namespace std
{
vecdata_t hash<coek::IndexVector>::operator()(const coek::IndexVector& a) const
{
    hash<vecdata_t> hasher;
    vecdata_t h = static_cast<vecdata_t>(a.size());
    size_t a_size = a.size();
    for (size_t i = 0; i < a_size; ++i)
    {
        // From Boost
        h ^= hasher(a[i]) + 0x9e3779b9 + (h << 6) + (h >> 2);
    }
    return h;
}
}
