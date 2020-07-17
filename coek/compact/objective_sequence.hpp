#ifndef COEK_COMPACT_OBJECTIVE_SEQUENCE_H
#define COEK_COMPACT_OBJECTIVE_SEQUENCE_H
#pragma once

#include <vector>

namespace coek {

class Objective;
class ObjectiveSequenceRepn;
class ObjectiveSeqIteratorRepn;
class SequenceContext;


class ObjectiveSeqIterator
{
public:

    std::shared_ptr<ObjectiveSeqIteratorRepn> repn;

    typedef Objective* pointer;
    typedef const Objective* const_pointer;
    typedef Objective& reference;
    typedef const Objective& const_reference;

public:

    ObjectiveSeqIterator();
    ObjectiveSeqIterator(ObjectiveSequenceRepn* seq, bool end=false);

    ObjectiveSeqIterator& operator++();

    bool operator==(const ObjectiveSeqIterator& other) const;
    bool operator!=(const ObjectiveSeqIterator& other) const;

    reference operator*();
    const_reference operator*() const;
    pointer operator->();
    const_pointer operator->() const;
};


class ObjectiveSequence
{
public:

  std::shared_ptr<ObjectiveSequenceRepn> repn;

public:

    ObjectiveSequence(const std::shared_ptr<ObjectiveSequenceRepn>& _repn);
    ObjectiveSequence(const Objective& expr, const SequenceContext& context);
    ObjectiveSequence(const Expression& expr, const SequenceContext& context, bool sense);

    ObjectiveSeqIterator begin();
    ObjectiveSeqIterator end();
};
}
#endif
