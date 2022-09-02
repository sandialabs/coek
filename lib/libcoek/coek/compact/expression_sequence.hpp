#pragma once

#include <vector>

namespace coek {

class ExpressionSeqIteratorRepn;
class Expression;
class ExpressionSequenceRepn;
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
    ExpressionSequence(const SequenceContext& context, const Expression& expr);

    ExpressionSeqIterator begin();
    ExpressionSeqIterator end();
    const ExpressionSeqIterator begin() const;
    const ExpressionSeqIterator end() const;
};


Expression Sum(const SequenceContext& context, const Expression& expr);
}
