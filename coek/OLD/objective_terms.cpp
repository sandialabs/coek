#include "objective_terms.hpp"

namespace coek {

//
// ObjectiveTerm
//
unsigned int ObjectiveTerm::count = 0;

ObjectiveTerm::ObjectiveTerm()
    : body(0), sense(true)
{
index = count++;
}

ObjectiveTerm::ObjectiveTerm(const expr_pointer_t& _body, bool _sense)
    : body(_body), sense(_sense)
{
index = count++;
if (body)
    OWN_POINTER(body);
}

ObjectiveTerm::~ObjectiveTerm()
{
if (body)
    DISOWN_POINTER(body);
}

}
