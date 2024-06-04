#include "expression_sequence.hpp"

#include "coek/util/cast_utils.hpp"
#include "coek/api/expression.hpp"
#include "coek/compact/coek_exprterm.hpp"
#include "coek_sets.hpp"
#include "sequence_context.hpp"
#include "visitor_exprtemplate.hpp"

namespace coek {

//
// ExpressionSequenceRepn
//

class ExpressionSequenceRepn {
   public:
    Expression expression_template;
    SequenceContext context;

   public:
    ExpressionSequenceRepn(const SequenceContext& context_, const Expression& expr)
        : expression_template(expr), context(context_)
    {
    }
};

//
// ExpressionSeqIteratorRepn
//

class ExpressionSeqIteratorRepn {
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
    ExpressionSeqIteratorRepn(ExpressionSequenceRepn* _seq, bool end) : seq(_seq)
    {
        done = end;
        if (!done) {
            context_iter.resize(seq->context.size());

            // auto cit = seq->context.repn->context.begin();
            size_t i = 0;
            for (auto it = context_iter.begin(); it != context_iter.end(); ++it, ++i) {
                Context& curr = seq->context[i];
                *it = curr.index_set.begin(curr.indices);
            }
            converted_expr = convert_expr_template(seq->expression_template.repn);
        }
        ncontexts = context_iter.size();
    }

    void operator++()
    {
        size_t i_ = 0;
        while (i_ < ncontexts) {
            size_t i = ncontexts - 1 - i_;
            ++context_iter[i];
            if (context_iter[i] == seq->context[i].index_set.end()) {
                context_iter[i] = seq->context[i].index_set.begin(seq->context[i].indices);
                i_++;
            }
            else
                break;
        }
        if (i_ == ncontexts)
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

    reference operator*() { return converted_expr; }

    const_reference operator*() const { return converted_expr; }

    pointer operator->() { return &(converted_expr); }

    const_pointer operator->() const { return &(converted_expr); }
};

//
// ExpressionSeqIterator
//

ExpressionSeqIterator::ExpressionSeqIterator() {}

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

ExpressionSeqIterator::reference ExpressionSeqIterator::operator*() { return repn->operator*(); }

ExpressionSeqIterator::const_reference ExpressionSeqIterator::operator*() const
{
    return repn->operator*();
}

ExpressionSeqIterator::pointer ExpressionSeqIterator::operator->() { return repn->operator->(); }

ExpressionSeqIterator::const_pointer ExpressionSeqIterator::operator->() const
{
    return repn->operator->();
}

//
// ExpressionSequence
//
ExpressionSequence::ExpressionSequence(const std::shared_ptr<ExpressionSequenceRepn>& _repn)
    : repn(_repn)
{
}

ExpressionSequence::ExpressionSequence(const SequenceContext& context_, const Expression& expr)
{
    repn = std::make_shared<ExpressionSequenceRepn>(context_, expr);
}

ExpressionSeqIterator ExpressionSequence::begin()
{
    return ExpressionSeqIterator(repn.get(), false);
}

ExpressionSeqIterator ExpressionSequence::end() { return ExpressionSeqIterator(repn.get(), true); }

const ExpressionSeqIterator ExpressionSequence::begin() const
{
    return ExpressionSeqIterator(repn.get(), false);
}

const ExpressionSeqIterator ExpressionSequence::end() const
{
    return ExpressionSeqIterator(repn.get(), true);
}

namespace convert_expr_visitor {

expr_pointer_t visit_SumExpressionTerm(const expr_pointer_t& expr)
{
    auto tmp = safe_pointer_cast<SumExpressionTerm>(expr);
    auto it = tmp->seq.begin();
    if (it == tmp->seq.end()) {
        return ZEROCONST;
    }

    expr_pointer_t curr;
    {
        Expression e = *it;
        ++it;
        for (; it != tmp->seq.end(); ++it) {
            e += *it;
        }
        curr = e.repn;
    }
    return curr;
}

}  // namespace convert_expr_visitor

//
// Sum
//
Expression Sum(const SequenceContext& context, const Expression& expr)
{
    ExpressionSequence seq(context, expr);
    Expression ans(CREATE_POINTER(SumExpressionTerm, seq));
    return ans;
}

}  // namespace coek
