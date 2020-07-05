#include "coek/expr/ast_term.hpp"
#include "coek/api/expression.hpp"
#include "coek/api/constraint.hpp"
#include "ast_set.hpp"
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

    std::vector<Context> context;
    Constraint constraint_template;

public:

    ConstraintSequenceRepn(const Constraint& con)
        : constraint_template(con)
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

            auto cit = seq->context.begin();
            for (auto it = context_iter.begin(); it != context_iter.end(); ++it, ++cit) {
                *it = cit->index_set.begin(cit->indices);
                }
            converted_con = convert_con_template(seq->constraint_template.repn);
            }
        ncontexts = context_iter.size();
        }

    void operator++()
        {
        int i = ncontexts-1;
        while (i >= 0) {
            ++context_iter[i];
            if (context_iter[i] == seq->context[i].index_set.end())
                {
                context_iter[i] = seq->context[i].index_set.begin(seq->context[i].indices);
                i--;
                }
            else
                break;
            }
        if (i < 0)
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
// Constraint
//

ConstraintSequenceAux Constraint::Forall(const std::vector<IndexParameter>& params)
{
auto repn = std::make_shared<ConstraintSequenceRepn>(*this);
repn->context.emplace_back(params);
return repn;
}

//
// ConstraintSequenceAux
//
ConstraintSequenceAux::ConstraintSequenceAux(const std::shared_ptr<ConstraintSequenceRepn>& _repn)
    : repn(_repn)
{}

ConstraintSequence ConstraintSequenceAux::In(const ConcreteSet& _index_set)
{
Context& curr = repn->context.back();
curr.index_set = _index_set;
return repn;
}

//
// ConstraintSequence
//
ConstraintSequence::ConstraintSequence(const std::shared_ptr<ConstraintSequenceRepn>& _repn)
    : repn(_repn)
{}

ConstraintSequenceAux ConstraintSequence::Forall(const std::vector<IndexParameter>& params)
{
repn->context.emplace_back(params);
return repn;
}

ConstraintSequence ConstraintSequence::ST(const Constraint& con)
{
auto curr = repn->context.back();
curr.index_constraints.push_back(con);
return repn;
}

ConstraintSequence ConstraintSequence::Where(const Constraint& con)
{
// TODO - parse these constraints here and use a more explicit data structure
auto curr = repn->context.back();
curr.index_values.push_back(con);
return repn;
}

ConstraintSeqIterator ConstraintSequence::begin()
{
return ConstraintSeqIterator(repn.get(), false);
}

ConstraintSeqIterator ConstraintSequence::end()
{
return ConstraintSeqIterator(repn.get(), true);
}

}
