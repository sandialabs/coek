#pragma once

#include <vector>

namespace coek {

class Variable;
class VariableSequenceRepn;
class VariableSeqIteratorRepn;
class SequenceContext;


class VariableSeqIterator
{
public:

    std::shared_ptr<VariableSeqIteratorRepn> repn;

    typedef Variable* pointer;
    typedef const Variable* const_pointer;
    typedef Variable& reference;
    typedef const Variable& const_reference;

public:

    VariableSeqIterator();
    VariableSeqIterator(VariableSequenceRepn* seq, bool end=false);

    VariableSeqIterator& operator++();

    bool operator==(const VariableSeqIterator& other) const;
    bool operator!=(const VariableSeqIterator& other) const;

    reference operator*();
    const_reference operator*() const;
    pointer operator->();
    const_pointer operator->() const;
};


class VariableSequence
{
public:

  std::shared_ptr<VariableSequenceRepn> repn;

public:

    VariableSequence(const std::shared_ptr<VariableSequenceRepn>& _repn);
    VariableSequence(const SequenceContext& context, const Variable& var);
    VariableSequence(const SequenceContext& context, const Expression& lb, const Expression& ub, const Expression& value, bool binary, bool integer);

    VariableSeqIterator begin();
    VariableSeqIterator end();
};
}
