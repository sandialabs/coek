#include <mutex>
#include <cassert>
#include "expr_terms.hpp"

namespace coek {

namespace {

//
// Mutex objects used to lock the modifications of the count value
//
std::mutex SubExpressionTerm_mtx;

}  // namespace

//
// SubExpressionTerm
//

unsigned int SubExpressionTerm::count = 0;

SubExpressionTerm::SubExpressionTerm(const expr_pointer_t& body) : UnaryTerm(body)
{
    SubExpressionTerm_mtx.lock();
    index = count++;
    SubExpressionTerm_mtx.unlock();
}

//
// UnaryTerm
//

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

//
// IfThenElseTerm
//

IfThenElseTerm::IfThenElseTerm(const expr_pointer_t& _cond, const expr_pointer_t& _then,
                               const expr_pointer_t& _else)
    : cond_expr(_cond), then_expr(_then), else_expr(_else)
{
}

}  // namespace coek
