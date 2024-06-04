#include "variable_sequence.hpp"

#include "coek/api/expression.hpp"
#include "coek/compact/coek_exprterm.hpp"
#include "coek_sets.hpp"
#include "sequence_context.hpp"
#include "visitor_exprtemplate.hpp"

namespace coek {

//
// VariableSequenceRepn
//

class VariableSequenceRepn {
   public:
    Variable expression_template;
    SequenceContext context;

   public:
    VariableSequenceRepn(const SequenceContext& context_, const Variable& expr)
        : expression_template(expr), context(context_)
    {
    }

    VariableSequenceRepn(const SequenceContext& context_, const Expression& lb,
                         const Expression& ub, const Expression& value, VariableTypes vtype)
        : expression_template(), context(context_)
    {
        expression_template.lower(lb).upper(ub).value(value).within(vtype);
    }
};

//
// VariableSeqIteratorRepn
//

class VariableSeqIteratorRepn {
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
    VariableSeqIteratorRepn(VariableSequenceRepn* _seq, bool end) : seq(_seq)
    {
        done = end;
        if (!done) {
            context_iter.resize(seq->context.size());

            size_t i = 0;
            for (auto it = context_iter.begin(); it != context_iter.end(); ++it, ++i) {
                Context& curr = seq->context[i];
                *it = curr.index_set.begin(curr.indices);
            }
            auto tmp
                = variable()
                      .lower(
                          convert_expr_template(seq->expression_template.lower_expression().repn))
                      .upper(
                          convert_expr_template(seq->expression_template.upper_expression().repn))
                      .value(
                          convert_expr_template(seq->expression_template.value_expression().repn))
                      .within(seq->expression_template.within())
                      .fixed(seq->expression_template.fixed());
            converted_expr = tmp;
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
        else {
            auto tmp
                = variable()
                      .lower(
                          convert_expr_template(seq->expression_template.lower_expression().repn))
                      .upper(
                          convert_expr_template(seq->expression_template.upper_expression().repn))
                      .value(
                          convert_expr_template(seq->expression_template.value_expression().repn))
                      .within(seq->expression_template.within())
                      .fixed(seq->expression_template.fixed());
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

    reference operator*() { return converted_expr; }

    const_reference operator*() const { return converted_expr; }

    pointer operator->() { return &(converted_expr); }

    const_pointer operator->() const { return &(converted_expr); }
};

//
// VariableSeqIterator
//

VariableSeqIterator::VariableSeqIterator() {}

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

VariableSeqIterator::reference VariableSeqIterator::operator*() { return repn->operator*(); }

VariableSeqIterator::const_reference VariableSeqIterator::operator*() const
{
    return repn->operator*();
}

VariableSeqIterator::pointer VariableSeqIterator::operator->() { return repn->operator->(); }

VariableSeqIterator::const_pointer VariableSeqIterator::operator->() const
{
    return repn->operator->();
}

//
// VariableSequence
//
VariableSequence::VariableSequence(const std::shared_ptr<VariableSequenceRepn>& _repn) : repn(_repn)
{
}

VariableSequence::VariableSequence(const SequenceContext& context_, const Variable& var)
{
    repn = std::make_shared<VariableSequenceRepn>(context_, var);
}

VariableSeqIterator VariableSequence::begin() { return VariableSeqIterator(repn.get(), false); }

VariableSeqIterator VariableSequence::end() { return VariableSeqIterator(repn.get(), true); }

}  // namespace coek
