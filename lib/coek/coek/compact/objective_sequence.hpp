#pragma once

#include <vector>

namespace coek {

class Objective;
class ObjectiveSequenceRepn;
class ObjectiveSeqIteratorRepn;
class SequenceContext;

class ObjectiveSeqIterator {
   public:
    std::shared_ptr<ObjectiveSeqIteratorRepn> repn;

    typedef Objective* pointer;
    typedef const Objective* const_pointer;
    typedef Objective& reference;
    typedef const Objective& const_reference;

   public:
    ObjectiveSeqIterator();
    ObjectiveSeqIterator(ObjectiveSequenceRepn* seq, bool end = false);

    ObjectiveSeqIterator& operator++();

    bool operator==(const ObjectiveSeqIterator& other) const;
    bool operator!=(const ObjectiveSeqIterator& other) const;

    reference operator*();
    const_reference operator*() const;
    pointer operator->();
    const_pointer operator->() const;
};

class ObjectiveSequence {
   public:
    std::shared_ptr<ObjectiveSequenceRepn> repn;

   public:
    ObjectiveSequence(const std::shared_ptr<ObjectiveSequenceRepn>& _repn);
    ObjectiveSequence(const SequenceContext& context, const Objective& expr);
    ObjectiveSequence(const SequenceContext& context, const Expression& expr, bool sense);

    ObjectiveSeqIterator begin();
    ObjectiveSeqIterator end();
};
}  // namespace coek
