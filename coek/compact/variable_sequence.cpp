#include "coek/api/expression.hpp"
#include "coek/compact/coek_exprterm.hpp"

#include "coek_sets.hpp"
#include "sequence_context.hpp"
#include "variable_sequence.hpp"


namespace coek {

expr_pointer_t convert_expr_template(expr_pointer_t expr);


//
// VariableSequenceRepn
//

class VariableSequenceRepn
{
public:

    Variable expression_template;
    SequenceContext context;

public:

    VariableSequenceRepn(const SequenceContext& context_, const Variable& expr)
        : expression_template(expr), context(context_)
        {}
    
    VariableSequenceRepn(const SequenceContext& context_, const Expression& lb, const Expression& ub, const Expression& value, bool binary, bool integer)
        : expression_template(lb, ub, value, binary, integer), context(context_)
        {}
    
};


//
// VariableSeqIteratorRepn
//

class VariableSeqIteratorRepn
{
public:

    VariableSequenceRepn* seq;
    std::vector<SetIterator> context_iter;
    size_t ncontexts;
    Variable converted_expr;
    bool done;

    typedef Variable* pointer;
    typedef const Variable* const_pointer;
    typedef Variable& reference;
    typedef const Variable& const_reference;

public:

    VariableSeqIteratorRepn(VariableSequenceRepn* _seq, bool end)
        : seq(_seq)
        {
        done = end;
        if (!done) {
            context_iter.resize(seq->context.size());
            
            size_t i=0;
            for (auto it = context_iter.begin(); it != context_iter.end(); ++it, ++i) {
                Context& curr = seq->context[i];
                *it = curr.index_set.begin(curr.indices);
                }
            coek::Variable tmp;
            tmp.initialize( 
                    convert_expr_template( seq->expression_template.get_lb_expression().repn ),
                    convert_expr_template( seq->expression_template.get_ub_expression().repn ),
                    convert_expr_template( seq->expression_template.get_value_expression().repn ),
                    seq->expression_template.is_binary(),
                    seq->expression_template.is_integer(),
                    seq->expression_template.get_fixed());
            converted_expr = tmp;
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
            coek::Variable tmp;
            tmp.initialize( 
                    convert_expr_template( seq->expression_template.get_lb_expression().repn ),
                    convert_expr_template( seq->expression_template.get_ub_expression().repn ),
                    convert_expr_template( seq->expression_template.get_value_expression().repn ),
                    seq->expression_template.is_binary(),
                    seq->expression_template.is_integer(),
                    seq->expression_template.get_fixed());
            converted_expr = tmp;
            }
        }

    bool operator==(const VariableSeqIteratorRepn* other) const
        {
        if (done != other->done)
            return false;
        if (done)
            return true;
        // BAD - Other comparisons here?
        return true;
        }

    bool operator!=(const VariableSeqIteratorRepn* other) const
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
// VariableSeqIterator
//

VariableSeqIterator::VariableSeqIterator()
{}

VariableSeqIterator::VariableSeqIterator(VariableSequenceRepn* _seq, bool end)
{
repn = std::make_shared<VariableSeqIteratorRepn>(_seq, end);
}

VariableSeqIterator& VariableSeqIterator::operator++()
{
repn->operator++();
return *this;
}

bool VariableSeqIterator::operator==(const VariableSeqIterator& other) const
{
return repn->operator==(other.repn.get());
}

bool VariableSeqIterator::operator!=(const VariableSeqIterator& other) const
{
return repn->operator!=(other.repn.get());
}

VariableSeqIterator::reference VariableSeqIterator::operator*()
{
return repn->operator*();
}

VariableSeqIterator::const_reference VariableSeqIterator::operator*() const
{
return repn->operator*();
}

VariableSeqIterator::pointer VariableSeqIterator::operator->()
{
return repn->operator->();
}

VariableSeqIterator::const_pointer VariableSeqIterator::operator->() const
{
return repn->operator->();
}


//
// VariableSequence
//
VariableSequence::VariableSequence(const std::shared_ptr<VariableSequenceRepn>& _repn)
    : repn(_repn)
{}

VariableSequence::VariableSequence(const SequenceContext& context_, const Variable& var)
{ repn = std::make_shared<VariableSequenceRepn>(context_,var); }

VariableSequence::VariableSequence(const SequenceContext& context_, const Expression& lb, const Expression& ub, const Expression& value, bool binary, bool integer)
{ repn = std::make_shared<VariableSequenceRepn>(context_,lb,ub,value,binary,integer); }

VariableSeqIterator VariableSequence::begin()
{ return VariableSeqIterator(repn.get(), false); }

VariableSeqIterator VariableSequence::end()
{ return VariableSeqIterator(repn.get(), true); }

}
