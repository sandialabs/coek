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
unsigned int ObjectiveTerm::count = 0;

ObjectiveTerm::ObjectiveTerm() : body(ZeroConstant), sense(true)
{
    ObjectiveTerm_mtx.lock();
    index = count++;
    ObjectiveTerm_mtx.unlock();
}

ObjectiveTerm::ObjectiveTerm(const expr_pointer_t& _body, bool _sense) : body(_body), sense(_sense)
{
    ObjectiveTerm_mtx.lock();
    index = count++;
    ObjectiveTerm_mtx.unlock();
}

//
// ConstraintTerm
//

unsigned int ConstraintTerm::count = 0;

ConstraintTerm::ConstraintTerm()
{
    ConstraintTerm_mtx.lock();
    index = count++;
    ConstraintTerm_mtx.unlock();
}

ConstraintTerm::ConstraintTerm(const expr_pointer_t& _lower, const expr_pointer_t& _body,
                               const expr_pointer_t& _upper)
    : lower(_lower), body(_body), upper(_upper)
{
    ConstraintTerm_mtx.lock();
    index = count++;
    ConstraintTerm_mtx.unlock();
}

ConstraintTerm::ConstraintTerm(const expr_pointer_t& _lower, const expr_pointer_t& _body,
                               int /*_upper*/)
    : lower(_lower), body(_body)
{
    ConstraintTerm_mtx.lock();
    index = count++;
    ConstraintTerm_mtx.unlock();
}

ConstraintTerm::ConstraintTerm(int /*_lower*/, const expr_pointer_t& _body,
                               const expr_pointer_t& _upper)
    : body(_body), upper(_upper)
{
    ConstraintTerm_mtx.lock();
    index = count++;
    ConstraintTerm_mtx.unlock();
}

//
// EmptyConstraintTerm
//

EmptyConstraintTerm::EmptyConstraintTerm() : ConstraintTerm() {}

std::shared_ptr<EmptyConstraintTerm> EmptyConstraintRepn = std::make_shared<EmptyConstraintTerm>();

}  // namespace coek
