#ifndef COEK_COMPACT_CONSTRAINT_SEQUENCE_H
#define COEK_COMPACT_CONSTRAINT_SEQUENCE_H
#pragma once

#include <vector>
//#include "coek_indexed.hpp"
//#include "coek/api/constraint.hpp"
//#include "coek/api/objective.hpp"

namespace coek {

class Constraint;
class IndexParameter;
class ConstraintSequenceRepn;
class ConstraintSeqIteratorRepn;
class SequenceContext;


class ConstraintSeqIterator
{
public:

    std::shared_ptr<ConstraintSeqIteratorRepn> repn;

    typedef Constraint* pointer;
    typedef const Constraint* const_pointer;
    typedef Constraint& reference;
    typedef const Constraint& const_reference;

public:

    ConstraintSeqIterator();
    ConstraintSeqIterator(ConstraintSequenceRepn* seq, bool end=false);

    ConstraintSeqIterator& operator++();

    bool operator==(const ConstraintSeqIterator& other) const;
    bool operator!=(const ConstraintSeqIterator& other) const;

    reference operator*();
    const_reference operator*() const;
    pointer operator->();
    const_pointer operator->() const;
};


class ConstraintSequence
{
public:

  std::shared_ptr<ConstraintSequenceRepn> repn;

public:

    ConstraintSequence(const std::shared_ptr<ConstraintSequenceRepn>& _repn);
    ConstraintSequence(const Constraint& con, const SequenceContext& context);

    ConstraintSeqIterator begin();
    ConstraintSeqIterator end();
};
}
#endif
