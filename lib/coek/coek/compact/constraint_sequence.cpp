#include "coek/api/expression.hpp"
#include "coek/api/constraint.hpp"

#include "coek_sets.hpp"
#include "sequence_context.hpp"
#include "constraint_sequence.hpp"


namespace coek {

ConstraintTerm* convert_con_template(ConstraintTerm* expr);


//
// ConstraintSequenceRepn
//

class ConstraintSequenceRepn
{
public:

    Constraint constraint_template;
    SequenceContext context;
    std::string name;

public:

    ConstraintSequenceRepn(const SequenceContext& context_, const Constraint& con)
        : constraint_template(con), context(context_)
        {}

    ConstraintSequenceRepn(const std::string& name_, const SequenceContext& context_, const Constraint& con)
        : constraint_template(con), context(context_), name(name_)
        {}
};


//
// ConstraintSeqIteratorRepn
//

class ConstraintSeqIteratorRepn
{
public:

    ConstraintSequenceRepn* seq;
    std::vector<SetIterator> context_iter;
    size_t ncontexts;
    Constraint converted_con;
    bool done;

    typedef Constraint* pointer;
    typedef const Constraint* const_pointer;
    typedef Constraint& reference;
    typedef const Constraint& const_reference;

public:

    ConstraintSeqIteratorRepn(ConstraintSequenceRepn* _seq, bool end)
        : seq(_seq)
        {
        done = end;
        if (!done) {
            context_iter.resize(seq->context.size());

            //auto cit = seq->context.begin();
            size_t i=0;
            for (auto it = context_iter.begin(); it != context_iter.end(); ++it, ++i) {
                Context& curr = seq->context[i];
                *it = curr.index_set.begin(curr.indices);
                }
            converted_con = convert_con_template(seq->constraint_template.repn);
            }
        ncontexts = context_iter.size();
        }

    void operator++()
        {
        size_t i_=0;
        while (i_ < ncontexts) {
            size_t i = ncontexts-1 - i_;
            ++context_iter[i];
            if (context_iter[i] == seq->context[i].index_set.end())
                {
                context_iter[i] = seq->context[i].index_set.begin(seq->context[i].indices);
                i_++;
                }
            else
                break;
            }
        if (i_ == ncontexts)
            done = true;
        else
            converted_con = convert_con_template(seq->constraint_template.repn);
        }

    bool operator==(const ConstraintSeqIteratorRepn* other) const
        {
        if (done != other->done)
            return false;
        if (done)
            return true;
        // BAD - Other comparisons here?
        return true;
        }

    bool operator!=(const ConstraintSeqIteratorRepn* other) const
        {
        if (done == other->done)
            return false;
        if (other->done)
            return true;
        // BAD - Other comparisons here?
        return true;
        }

    reference operator*()
        { return converted_con; }

    const_reference operator*() const
        { return converted_con; }

    pointer operator->()
        { return &(converted_con); }

    const_pointer operator->() const
        { return &(converted_con); }
};

//
// ConstraintSeqIterator
//

ConstraintSeqIterator::ConstraintSeqIterator()
{}

ConstraintSeqIterator::ConstraintSeqIterator(ConstraintSequenceRepn* _seq, bool end)
{
repn = std::make_shared<ConstraintSeqIteratorRepn>(_seq, end);
}

ConstraintSeqIterator& ConstraintSeqIterator::operator++()
{
repn->operator++();
return *this;
}

bool ConstraintSeqIterator::operator==(const ConstraintSeqIterator& other) const
{
return repn->operator==(other.repn.get());
}

bool ConstraintSeqIterator::operator!=(const ConstraintSeqIterator& other) const
{
return repn->operator!=(other.repn.get());
}

ConstraintSeqIterator::reference ConstraintSeqIterator::operator*()
{
return repn->operator*();
}

ConstraintSeqIterator::const_reference ConstraintSeqIterator::operator*() const
{
return repn->operator*();
}

ConstraintSeqIterator::pointer ConstraintSeqIterator::operator->()
{
return repn->operator->();
}

ConstraintSeqIterator::const_pointer ConstraintSeqIterator::operator->() const
{
return repn->operator->();
}

//
// ConstraintSequence
//
ConstraintSequence::ConstraintSequence(const std::shared_ptr<ConstraintSequenceRepn>& _repn)
    : repn(_repn)
{}

ConstraintSequence::ConstraintSequence(const SequenceContext& context_, const Constraint& expr)
{ repn = std::make_shared<ConstraintSequenceRepn>(context_,expr); }

ConstraintSequence::ConstraintSequence(const std::string& name, const SequenceContext& context_, const Constraint& expr)
{ repn = std::make_shared<ConstraintSequenceRepn>(name,context_,expr); }

ConstraintSeqIterator ConstraintSequence::begin()
{ return ConstraintSeqIterator(repn.get(), false); }

ConstraintSeqIterator ConstraintSequence::end()
{ return ConstraintSeqIterator(repn.get(), true); }

const ConstraintSeqIterator ConstraintSequence::begin() const
{ return ConstraintSeqIterator(repn.get(), false); }

const ConstraintSeqIterator ConstraintSequence::end() const
{ return ConstraintSeqIterator(repn.get(), true); }

}
