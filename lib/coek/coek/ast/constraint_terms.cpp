#include "base_terms.hpp"
#include "constraint_terms.hpp"

namespace coek {

//
// ObjectiveTerm
//
unsigned int ObjectiveTerm::count = 0;

ObjectiveTerm::ObjectiveTerm() : body(ZeroConstant), sense(true) { index = count++; }

ObjectiveTerm::ObjectiveTerm(const expr_pointer_t& _body, bool _sense) : body(_body), sense(_sense)
{
    index = count++;
}

//
// ConstraintTerm
//

unsigned int ConstraintTerm::count = 0;

ConstraintTerm::ConstraintTerm() { index = count++; }

ConstraintTerm::ConstraintTerm(const expr_pointer_t& _lower, const expr_pointer_t& _body,
                               const expr_pointer_t& _upper)
    : lower(_lower), body(_body), upper(_upper)
{
    index = count++;
}

ConstraintTerm::ConstraintTerm(const expr_pointer_t& _lower, const expr_pointer_t& _body,
                               int /*_upper*/)
    : lower(_lower), body(_body)
{
    index = count++;
}

ConstraintTerm::ConstraintTerm(int /*_lower*/, const expr_pointer_t& _body,
                               const expr_pointer_t& _upper)
    : body(_body), upper(_upper)
{
    index = count++;
}

//
// EmptyConstraintTerm
//

EmptyConstraintTerm::EmptyConstraintTerm() : ConstraintTerm() {}

std::shared_ptr<EmptyConstraintTerm> EmptyConstraintRepn = std::make_shared<EmptyConstraintTerm>();

}  // namespace coek
