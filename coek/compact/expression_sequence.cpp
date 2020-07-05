//#include <cstdio>
//#include <memory>
//#include <sstream>
//#include <fstream>
//#include <map>

#include "coek/expr/ast_term.hpp"
#include "coek/api/expression.hpp"
#include "coek/compact/coek_exprterm.hpp"

#include "coek_sets.hpp"
#include "sequence_context.hpp"
#include "expression_sequence.hpp"


namespace coek {

expr_pointer_t convert_expr_template(expr_pointer_t expr);


//
// ExpressionSequenceRepn
//

class ExpressionSequenceRepn
{
public:

    Expression expression_template;
    SequenceContext context;

public:

    ExpressionSequenceRepn(const Expression& expr, const SequenceContext& context_)
        : expression_template(expr), context(context_)
        {}
    
};


//
// ExpressionSeqIteratorRepn
//

class ExpressionSeqIteratorRepn
{
public:

    ExpressionSequenceRepn* seq;
    std::vector<SetIterator> context_iter;
    size_t ncontexts;
    Expression converted_expr;
    bool done;

    typedef Expression* pointer;
    typedef const Expression* const_pointer;
    typedef Expression& reference;
    typedef const Expression& const_reference;

public:

    ExpressionSeqIteratorRepn(ExpressionSequenceRepn* _seq, bool end)
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
            converted_expr = convert_expr_template( seq->expression_template.repn );
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
            converted_expr = convert_expr_template(seq->expression_template.repn);
        }

    bool operator==(const ExpressionSeqIteratorRepn* other) const
        {
        if (done != other->done)
            return false;
        if (done)
            return true;
        // BAD - Other comparisons here?
        return true;
        }

    bool operator!=(const ExpressionSeqIteratorRepn* other) const
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
// ExpressionSeqIterator
//

ExpressionSeqIterator::ExpressionSeqIterator()
{}

ExpressionSeqIterator::ExpressionSeqIterator(ExpressionSequenceRepn* _seq, bool end)
{
repn = std::make_shared<ExpressionSeqIteratorRepn>(_seq, end);
}

ExpressionSeqIterator& ExpressionSeqIterator::operator++()
{
repn->operator++();
return *this;
}

bool ExpressionSeqIterator::operator==(const ExpressionSeqIterator& other) const
{
return repn->operator==(other.repn.get());
}

bool ExpressionSeqIterator::operator!=(const ExpressionSeqIterator& other) const
{
return repn->operator!=(other.repn.get());
}

ExpressionSeqIterator::reference ExpressionSeqIterator::operator*()
{
return repn->operator*();
}

ExpressionSeqIterator::const_reference ExpressionSeqIterator::operator*() const
{
return repn->operator*();
}

ExpressionSeqIterator::pointer ExpressionSeqIterator::operator->()
{
return repn->operator->();
}

ExpressionSeqIterator::const_pointer ExpressionSeqIterator::operator->() const
{
return repn->operator->();
}


//
// ExpressionSequence
//
ExpressionSequence::ExpressionSequence(const std::shared_ptr<ExpressionSequenceRepn>& _repn)
    : repn(_repn)
{}

ExpressionSequence::ExpressionSequence(const Expression& expr, const SequenceContext& context_)
{ repn = std::make_shared<ExpressionSequenceRepn>(expr,context_); }

ExpressionSeqIterator ExpressionSequence::begin()
{ return ExpressionSeqIterator(repn.get(), false); }

ExpressionSeqIterator ExpressionSequence::end()
{ return ExpressionSeqIterator(repn.get(), true); }



namespace visitors {

expr_pointer_t visit(SumExpressionTerm& arg)
{
auto it=arg.seq.begin();
if (it == arg.seq.end()) {
    return ZEROCONST;
    }

expr_pointer_t curr;
{
Expression e = *it;
++it;
for (; it != arg.seq.end(); ++it) {
    e += *it;
    }
curr = e.repn;
OWN_POINTER(curr);
}
FREE_POINTER(curr);
return curr;
}

}



//
// Sum
//
Expression Sum(const Expression& expr, const SequenceContext& context)
{
ExpressionSequence seq(expr, context);
Expression ans( CREATE_POINTER(SumExpressionTerm, seq) );
return ans;
}

}
