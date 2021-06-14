#pragma once

#include "../ast/base_terms.hpp"
#include "coek/compact/expression_sequence.hpp"

namespace coek {

class SumExpressionTerm : public BaseExpressionTerm
{
public:

    ExpressionSequence seq;

public:

    SumExpressionTerm(const ExpressionSequence& _seq)
        : seq(_seq)
        {}

    double eval() const
        { throw std::runtime_error("Cannot evaluate a Sum() expression."); }

    bool is_expression() const
        { return false; }

    void accept(Visitor& v)
        { v.visit(*this); }
    term_id id()
        {return SumExpressionTerm_id;}
};

namespace visitors {

expr_pointer_t visitor(SumExpressionTerm&);

}

}
