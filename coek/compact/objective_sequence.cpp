#include "coek/api/expression.hpp"
#include "coek/api/objective.hpp"
#include "coek/compact/coek_exprterm.hpp"

#include "coek_sets.hpp"
#include "sequence_context.hpp"
#include "objective_sequence.hpp"


namespace coek {

expr_pointer_t convert_expr_template(expr_pointer_t expr);


//
// ObjectiveSequenceRepn
//

class ObjectiveSequenceRepn
{
public:

    Objective expression_template;
    SequenceContext context;

public:

    ObjectiveSequenceRepn(const Objective& expr, const SequenceContext& context_)
        : expression_template(expr), context(context_)
        {}
    
    ObjectiveSequenceRepn(const Expression& expr, const SequenceContext& context_, bool sense)
        : expression_template(expr, sense), context(context_)
        {}
    
};


//
// ObjectiveSeqIteratorRepn
//

class ObjectiveSeqIteratorRepn
{
public:

    ObjectiveSequenceRepn* seq;
    std::vector<SetIterator> context_iter;
    size_t ncontexts;
    Objective converted_expr;
    bool done;

    typedef Objective* pointer;
    typedef const Objective* const_pointer;
    typedef Objective& reference;
    typedef const Objective& const_reference;

public:

    ObjectiveSeqIteratorRepn(ObjectiveSequenceRepn* _seq, bool end)
        : seq(_seq)
        {
        done = end;
        if (!done) {
            context_iter.resize(seq->context.size());
            
            //auto cit = seq->context.repn->context.begin();
            size_t i=0;
            for (auto it = context_iter.begin(); it != context_iter.end(); ++it, ++i) {
                Context& curr = seq->context[i];
                *it = curr.index_set.begin(curr.indices);
                }
            converted_expr.set_body( convert_expr_template( seq->expression_template.body().repn ) );
            converted_expr.set_sense( seq->expression_template.sense() );
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
        else {
            converted_expr.set_body( convert_expr_template(seq->expression_template.body().repn ) );
            converted_expr.set_sense( seq->expression_template.sense() );
            }
        }

    bool operator==(const ObjectiveSeqIteratorRepn* other) const
        {
        if (done != other->done)
            return false;
        if (done)
            return true;
        // BAD - Other comparisons here?
        return true;
        }

    bool operator!=(const ObjectiveSeqIteratorRepn* other) const
        {
        if (done == other->done)
            return false;
        if (other->done)
            return true;
        // BAD - Other comparisons here?
        return true;
        }

    reference operator*()
        { return converted_expr; }

    const_reference operator*() const
        { return converted_expr; }

    pointer operator->()
        { return &(converted_expr); }

    const_pointer operator->() const
        { return &(converted_expr); }
};

//
// ObjectiveSeqIterator
//

ObjectiveSeqIterator::ObjectiveSeqIterator()
{}

ObjectiveSeqIterator::ObjectiveSeqIterator(ObjectiveSequenceRepn* _seq, bool end)
{
repn = std::make_shared<ObjectiveSeqIteratorRepn>(_seq, end);
}

ObjectiveSeqIterator& ObjectiveSeqIterator::operator++()
{
repn->operator++();
return *this;
}

bool ObjectiveSeqIterator::operator==(const ObjectiveSeqIterator& other) const
{
return repn->operator==(other.repn.get());
}

bool ObjectiveSeqIterator::operator!=(const ObjectiveSeqIterator& other) const
{
return repn->operator!=(other.repn.get());
}

ObjectiveSeqIterator::reference ObjectiveSeqIterator::operator*()
{
return repn->operator*();
}

ObjectiveSeqIterator::const_reference ObjectiveSeqIterator::operator*() const
{
return repn->operator*();
}

ObjectiveSeqIterator::pointer ObjectiveSeqIterator::operator->()
{
return repn->operator->();
}

ObjectiveSeqIterator::const_pointer ObjectiveSeqIterator::operator->() const
{
return repn->operator->();
}


//
// ObjectiveSequence
//
ObjectiveSequence::ObjectiveSequence(const std::shared_ptr<ObjectiveSequenceRepn>& _repn)
    : repn(_repn)
{}

ObjectiveSequence::ObjectiveSequence(const Objective& expr, const SequenceContext& context_)
{ repn = std::make_shared<ObjectiveSequenceRepn>(expr,context_); }

ObjectiveSequence::ObjectiveSequence(const Expression& expr, const SequenceContext& context_, bool sense)
{ repn = std::make_shared<ObjectiveSequenceRepn>(expr,context_,sense); }

ObjectiveSeqIterator ObjectiveSequence::begin()
{ return ObjectiveSeqIterator(repn.get(), false); }

ObjectiveSeqIterator ObjectiveSequence::end()
{ return ObjectiveSeqIterator(repn.get(), true); }

}
