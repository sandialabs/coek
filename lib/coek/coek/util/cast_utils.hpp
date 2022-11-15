#pragma once

#include <memory>

namespace coek {

#include <cassert>

// If we have a pointer to a base type and know the type, then we can safely use static_pointer_cast.  But
// when debugging we use dynamic_pointer_cast to be cautious.

#if defined(DEBUG)

template <class T, class U>
inline std::shared_ptr<T> safe_pointer_cast(const std::shared_ptr<U>& ptr)
{
T ret = std::dynamic_pointer_cast<T>(ptr);
assert(ret);
return ret;
}

#else

template <class T, class U>
inline std::shared_ptr<T> safe_pointer_cast(const std::shared_ptr<U>& ptr)
{
return std::static_pointer_cast<T>(ptr);
}

#endif

}  // namespace coek
