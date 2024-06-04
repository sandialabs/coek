#include <mutex>
#include "base_terms.hpp"
#include "constraint_terms.hpp"

namespace coek {

namespace {

//
// Mutex objects used to lock the modifications of the count value
//
std::mutex ObjectiveTerm_mtx;
std::mutex ConstraintTerm_mtx;

}  // namespace

//
// ObjectiveTerm
//
size_t ObjectiveTerm::count = 0;

ObjectiveTerm::ObjectiveTerm() : sense(true), active(true), body(ZeroConstant)
{
    ObjectiveTerm_mtx.lock();
    index = count++;
    ObjectiveTerm_mtx.unlock();
}

ObjectiveTerm::ObjectiveTerm(const expr_pointer_t& body_, bool sense_)
    : sense(sense_), active(true), body(body_)
{
    ObjectiveTerm_mtx.lock();
    index = count++;
    ObjectiveTerm_mtx.unlock();
}

//
// ConstraintTerm
//

size_t ConstraintTerm::count = 0;

ConstraintTerm::ConstraintTerm()
{
    ConstraintTerm_mtx.lock();
    index = count++;
    ConstraintTerm_mtx.unlock();
}

ConstraintTerm::ConstraintTerm(const expr_pointer_t& _lower, const expr_pointer_t& _body,
                               const expr_pointer_t& _upper)
    : active(true), lower(_lower), body(_body), upper(_upper)
{
    ConstraintTerm_mtx.lock();
    index = count++;
    ConstraintTerm_mtx.unlock();
}

ConstraintTerm::ConstraintTerm(const expr_pointer_t& _lower, const expr_pointer_t& _body,
                               int /*_upper*/)
    : active(true), lower(_lower), body(_body)
{
    ConstraintTerm_mtx.lock();
    index = count++;
    ConstraintTerm_mtx.unlock();
}

ConstraintTerm::ConstraintTerm(int /*_lower*/, const expr_pointer_t& _body,
                               const expr_pointer_t& _upper)
    : active(true), body(_body), upper(_upper)
{
    ConstraintTerm_mtx.lock();
    index = count++;
    ConstraintTerm_mtx.unlock();
}

//
// EmptyConstraintTerm
//

EmptyConstraintTerm::EmptyConstraintTerm() : ConstraintTerm() { active = false; }

std::shared_ptr<EmptyConstraintTerm> EmptyConstraintRepn = std::make_shared<EmptyConstraintTerm>();

}  // namespace coek
