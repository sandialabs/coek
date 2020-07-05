#ifndef COEK_COMPACT_CONSTRAINT_SEQUENCE_H
#define COEK_COMPACT_CONSTRAINT_SEQUENCE_H
#pragma once

#include <vector>
#include "coek_indexed.hpp"
#include "coek/api/constraint.hpp"
#include "coek/api/objective.hpp"

namespace coek {

class Constraint;
class IndexParameter;
class ConstraintSeqIteratorRepn;


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

    template <typename... TYPES>
    ConstraintSequenceAux Forall(const TYPES&... args)
        {
        std::vector<IndexParameter> arg;
        collect_args(args..., arg);
        return Forall(arg);
        }
    
    ConstraintSequenceAux Forall(const std::vector<IndexParameter>& params);
    ConstraintSequence ST(const Constraint& con);
    ConstraintSequence Where(const Constraint& con);

    ConstraintSeqIterator begin();
    ConstraintSeqIterator end();

protected:
    
    void collect_args(const IndexParameter& arg, std::vector<IndexParameter>& _arg)
        {
        _arg.emplace_back(arg);
        }

    template <typename... TYPES>
    void collect_args(const IndexParameter& arg, const TYPES&... args, std::vector<IndexParameter>&  _arg)
        {
        _arg.emplace_back(arg);
        collect_args(args..., _arg);
        }
};

}
#endif
