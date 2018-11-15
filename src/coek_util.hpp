#pragma once


#define runtime_assert(flag, descr)\
{\
bool condition= flag ;\
if (!condition)\
    throw std::runtime_error(descr);\
}

