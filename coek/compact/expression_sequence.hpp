#ifndef COEK_COMPACT_EXPRESSION_SEQUENCE_H
#define COEK_COMPACT_EXPRESSION_SEQUENCE_H
#pragma once

#include <vector>

namespace coek {

class ExpressionSequenceRepn;
class ExpressionSeqIteratorRepn;
class SequenceContext;


class ExpressionSeqIterator
{
public:

    std::shared_ptr<ExpressionSeqIteratorRepn> repn;

    typedef Expression* pointer;
    typedef const Expression* const_pointer;
    typedef Expression& reference;
    typedef const Expression& const_reference;

public:

    ExpressionSeqIterator();
    ExpressionSeqIterator(ExpressionSequenceRepn* seq, bool end=false);

    ExpressionSeqIterator& operator++();

    bool operator==(const ExpressionSeqIterator& other) const;
    bool operator!=(const ExpressionSeqIterator& other) const;

    reference operator*();
    const_reference operator*() const;
    pointer operator->();
    const_pointer operator->() const;
};


class ExpressionSequence
{
public:

  std::shared_ptr<ExpressionSequenceRepn> repn;

public:

    ExpressionSequence(const std::shared_ptr<ExpressionSequenceRepn>& _repn);
    ExpressionSequence(const Expression& expr, const SequenceContext& context);

    ExpressionSeqIterator begin();
    ExpressionSeqIterator end();
};


Expression Sum(const Expression& expr, const SequenceContext& context);
}
#endif
