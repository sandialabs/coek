#pragma once

#include <memory>
#include <vector>

namespace coek {

class Variable;
enum VariableTypes : unsigned int;
class VariableSequenceRepn;
class VariableSeqIteratorRepn;
class SequenceContext;

class VariableSeqIterator {
   public:
    std::shared_ptr<VariableSeqIteratorRepn> repn;

    typedef Variable* pointer;
    typedef const Variable* const_pointer;
    typedef Variable& reference;
    typedef const Variable& const_reference;

   public:
    VariableSeqIterator();
    VariableSeqIterator(VariableSequenceRepn* seq, bool end = false);

    VariableSeqIterator& operator++();

    bool operator==(const VariableSeqIterator& other) const;
    bool operator!=(const VariableSeqIterator& other) const;

    reference operator*();
    const_reference operator*() const;
    pointer operator->();
    const_pointer operator->() const;
};

class VariableSequence {
   public:
    std::shared_ptr<VariableSequenceRepn> repn;

   public:
    VariableSequence(const std::shared_ptr<VariableSequenceRepn>& _repn);
    VariableSequence(const SequenceContext& context, const Variable& var);

    VariableSeqIterator begin();
    VariableSeqIterator end();
};
}  // namespace coek
