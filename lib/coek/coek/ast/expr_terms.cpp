#include "expr_terms.hpp"

#include <cassert>

namespace coek {

//
// UnaryTerm
//

UnaryTerm::UnaryTerm(const expr_pointer_t& repn) : body(repn)
{
    non_variable = repn->non_variable;
    OWN_POINTER(body);
}

UnaryTerm::~UnaryTerm() { DISOWN_POINTER(body); }

BinaryTerm::BinaryTerm(const expr_pointer_t& _lhs, const expr_pointer_t& _rhs)
    : lhs(_lhs), rhs(_rhs)
{
    non_variable = lhs->non_variable and rhs->non_variable;
    OWN_POINTER(lhs);
    OWN_POINTER(rhs);
}

BinaryTerm::~BinaryTerm()
{
    DISOWN_POINTER(lhs);
    DISOWN_POINTER(rhs);
}

NAryPrefixTerm::~NAryPrefixTerm()
{
    if (data->size() == n) {
        shared_t::iterator end = data->end();
        for (shared_t::iterator it = data->begin(); it != end; ++it) DISOWN_POINTER(*it);
        data->resize(0);
    }
}

void NAryPrefixTerm::initialize(const expr_pointer_t& lhs, const expr_pointer_t& rhs)
{
    data = std::make_shared<shared_t>();
    data->push_back(lhs);
    data->push_back(rhs);
    non_variable = lhs->non_variable and rhs->non_variable;
    // n = data->size();
    n = 2;
    OWN_POINTER(lhs);
    OWN_POINTER(rhs);
}

void NAryPrefixTerm::initialize(NAryPrefixTerm* lhs, const expr_pointer_t& rhs)
{
    data = lhs->data;
    data->push_back(rhs);
    non_variable = lhs->non_variable and rhs->non_variable;
    n = data->size();
    OWN_POINTER(rhs);
}

void NAryPrefixTerm::push_back(const expr_pointer_t& rhs)
{
    assert(n == data->size());
    non_variable = non_variable and rhs->non_variable;
    data->push_back(rhs);
    n = data->size();
    OWN_POINTER(rhs);
}

//
// PlusTerm
//

PlusTerm::PlusTerm(const expr_pointer_t& lhs, const expr_pointer_t& rhs)
{
    PlusTerm* _lhs = dynamic_cast<PlusTerm*>(lhs);
    if (_lhs && (_lhs->n == _lhs->data->size())) {
        // std::cout << "PLUS-Extend" << std::endl;
        initialize(_lhs, rhs);
    }
    else {
        // std::cout << "PLUS-NEW" << std::endl;
        initialize(lhs, rhs);
    }
}

PlusTerm::PlusTerm(const expr_pointer_t& lhs, const expr_pointer_t& rhs, bool)
{
    initialize(lhs, rhs);
}

}  // namespace coek
