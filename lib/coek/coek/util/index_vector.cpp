#include <coek/util/index_vector.hpp>

namespace std {
coek::IndexVector::value_type hash<coek::IndexVector>::operator()(const coek::IndexVector& a) const
{
    hash<coek::IndexVector::value_type> hasher;
    coek::IndexVector::value_type h = static_cast<coek::IndexVector::value_type>(a.size());
    size_t a_size = a.size();
    for (size_t i = 0; i < a_size; ++i) {
        // From Boost
        h ^= hasher(a[i]) + 0x9e3779b9 + (h << 6) + (h >> 2);
    }
    return h;
}
}  // namespace std
