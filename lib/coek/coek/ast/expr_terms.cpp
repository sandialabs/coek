#include "expr_terms.hpp"

#include <cassert>

namespace coek {

//
// UnaryTerm
//

UnaryTerm::UnaryTerm(const expr_pointer_t& repn) : body(repn) { non_variable = repn->non_variable; }

BinaryTerm::BinaryTerm(const expr_pointer_t& _lhs, const expr_pointer_t& _rhs)
    : lhs(_lhs), rhs(_rhs)
{
    non_variable = lhs->non_variable and rhs->non_variable;
}

NAryPrefixTerm::~NAryPrefixTerm()
{
    if (data->size() == n) {
        data->resize(0);
    }
}

void NAryPrefixTerm::initialize(const expr_pointer_t& lhs, const expr_pointer_t& rhs)
{
    data = std::make_shared<shared_t>();
    data->push_back(lhs);
    data->push_back(rhs);
    non_variable = lhs->non_variable and rhs->non_variable;
    n = 2;
}

void NAryPrefixTerm::initialize(NAryPrefixTerm* lhs, const expr_pointer_t& rhs)
{
    data = lhs->data;
    data->push_back(rhs);
    non_variable = lhs->non_variable and rhs->non_variable;
    n = data->size();
}

void NAryPrefixTerm::push_back(const expr_pointer_t& rhs)
{
    assert(n == data->size());
    non_variable = non_variable and rhs->non_variable;
    data->push_back(rhs);
    n = data->size();
}

//
// PlusTerm
//

PlusTerm::PlusTerm(const expr_pointer_t& lhs, const expr_pointer_t& rhs)
{
    auto _lhs = std::dynamic_pointer_cast<PlusTerm>(lhs);
    if (_lhs.get() && (_lhs->n == _lhs->data->size())) {
        initialize(_lhs.get(), rhs);
    }
    else {
        initialize(lhs, rhs);
    }
}

PlusTerm::PlusTerm(const expr_pointer_t& lhs, const expr_pointer_t& rhs, bool)
{
    initialize(lhs, rhs);
}

}  // namespace coek
