#pragma once

#ifdef WITH_ADMODEL_SIMPLEAD
#include "admodel/simplead.hpp"

namespace coek {

typedef coek::simplead::ADModel ADModel;

}
#endif

#ifdef WITH_ADMODEL_CPPAD
#include "admodel/cppad.hpp"

namespace coek {

typedef coek::cppad::ADModel ADModel;

}
#endif
