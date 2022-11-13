#pragma once

#include <memory>
#include <vector>

namespace coek {

class Constraint;
class IndexParameter;
class ConstraintSequenceRepn;
class ConstraintSeqIteratorRepn;
class SequenceContext;

class ConstraintSeqIterator {
   public:
    std::shared_ptr<ConstraintSeqIteratorRepn> repn;

    typedef Constraint* pointer;
    typedef const Constraint* const_pointer;
    typedef Constraint& reference;
    typedef const Constraint& const_reference;

   public:
    ConstraintSeqIterator();
    ConstraintSeqIterator(ConstraintSequenceRepn* seq, bool end = false);

    ConstraintSeqIterator& operator++();

    bool operator==(const ConstraintSeqIterator& other) const;
    bool operator!=(const ConstraintSeqIterator& other) const;

    reference operator*();
    const_reference operator*() const;
    pointer operator->();
    const_pointer operator->() const;
};

class ConstraintSequence {
   public:
    std::shared_ptr<ConstraintSequenceRepn> repn;

   public:
    ConstraintSequence(const std::shared_ptr<ConstraintSequenceRepn>& _repn);
    ConstraintSequence(const SequenceContext& context, const Constraint& con);
    ConstraintSequence(const std::string& name, const SequenceContext& context,
                       const Constraint& con);

    ConstraintSeqIterator begin();
    ConstraintSeqIterator end();
    const ConstraintSeqIterator begin() const;
    const ConstraintSeqIterator end() const;
};

inline ConstraintSequence constraint(const Constraint& expr, const SequenceContext& context)
{
    ConstraintSequence seq(context, expr);
    return seq;
}

inline ConstraintSequence constraint(const std::string& name, const Constraint& expr,
                                     const SequenceContext& context)
{
    ConstraintSequence seq(name, context, expr);
    return seq;
}

}  // namespace coek
