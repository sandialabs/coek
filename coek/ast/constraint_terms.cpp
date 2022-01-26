#include "base_terms.hpp"
#include "constraint_terms.hpp"


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


//
// ConstraintTerm
//

unsigned int ConstraintTerm::count = 0;

ConstraintTerm::ConstraintTerm()
    : lower(0), body(0), upper(0)
{
index = count++;
}

ConstraintTerm::ConstraintTerm(const expr_pointer_t& _lower, const expr_pointer_t& _body, const expr_pointer_t& _upper)
    : lower(_lower), body(_body), upper(_upper)
{
index = count++;
if (lower)
    OWN_POINTER(lower);
if (body)
    OWN_POINTER(body);
if (upper)
    OWN_POINTER(upper);
}

ConstraintTerm::~ConstraintTerm()
{
if (lower)
    DISOWN_POINTER(lower);
if (body)
    DISOWN_POINTER(body);
if (upper)
    DISOWN_POINTER(upper);
}

//
// DummyConstraintTerm
//

DummyConstraintTerm::DummyConstraintTerm()
    : ConstraintTerm()
{
refcount=2;
body = this;
#ifdef WITH_AST_ENV
env.cache(this);
#endif
}

}
