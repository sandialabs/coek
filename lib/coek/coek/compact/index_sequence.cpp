#include "index_sequence.hpp"

#include "coek/util/cast_utils.hpp"
#include "coek/api/expression.hpp"
#include "coek/compact/coek_exprterm.hpp"
#include "coek_sets.hpp"
#include "sequence_context.hpp"
//#include "visitor_exprtemplate.hpp"

namespace coek {

//
// IndexSequenceRepn
//

class IndexSequenceRepn {
   public:
    SequenceContext context;

   public:
    IndexSequenceRepn(const SequenceContext& context_) : context(context_) { }
};

//
// IndexSeqIteratorRepn
//

class IndexSeqIteratorRepn {
   public:
    IndexSequenceRepn* seq;
    std::vector<SetIterator> context_iter;
    size_t ncontexts;
    //Index converted_expr;
    std::vector<IndexParameter> indices;
    bool done;

    typedef std::vector<IndexParameter>* pointer;
    typedef const std::vector<IndexParameter>* const_pointer;
    typedef std::vector<IndexParameter>& reference;
    typedef const std::vector<IndexParameter>& const_reference;

   public:
    IndexSeqIteratorRepn(IndexSequenceRepn* _seq, bool end) : seq(_seq)
    {
        done = end;
        if (!done) {
            // Collect indices
            for (auto& context: seq->context.repn->context) {
                for (auto& iparam: context.indices)
                    indices.push_back(iparam);
            }
            //std::cout << "HERE " << indices.size() << std::endl;
            //std::cout << "HERE " << _seq->context.repn->dim << std::endl;
            assert(indices.size() == _seq->context.repn->dim);

            // Setup context iters
            context_iter.resize(seq->context.size());

            // auto cit = seq->context.repn->context.begin();
            size_t i = 0;
            for (auto it = context_iter.begin(); it != context_iter.end(); ++it, ++i) {
                Context& curr = seq->context[i];
                *it = curr.index_set.begin(curr.indices);
            }
            //converted_expr = convert_expr_template(seq->expression_template.repn);
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
        //else
        //    converted_expr = convert_expr_template(seq->expression_template.repn);
    }

    bool operator==(const IndexSeqIteratorRepn* other) const
    {
        if (done != other->done)
            return false;
        if (done)
            return true;
        // BAD - Other comparisons here?
        return true;
    }

    bool operator!=(const IndexSeqIteratorRepn* other) const
    {
        if (done == other->done)
            return false;
        if (other->done)
            return true;
        // BAD - Other comparisons here?
        return true;
    }

    reference operator*() { return indices; }

    const_reference operator*() const { return indices; }

    pointer operator->() { return &(indices); }

    const_pointer operator->() const { return &(indices); }
};

//
// IndexSeqIterator
//

IndexSeqIterator::IndexSeqIterator() {}

IndexSeqIterator::IndexSeqIterator(IndexSequenceRepn* _seq, bool end)
{
    repn = std::make_shared<IndexSeqIteratorRepn>(_seq, end);
}

IndexSeqIterator& IndexSeqIterator::operator++()
{
    repn->operator++();
    return *this;
}

bool IndexSeqIterator::operator==(const IndexSeqIterator& other) const
{
    return repn->operator==(other.repn.get());
}

bool IndexSeqIterator::operator!=(const IndexSeqIterator& other) const
{
    return repn->operator!=(other.repn.get());
}

IndexSeqIterator::reference IndexSeqIterator::operator*() { return repn->operator*(); }

IndexSeqIterator::const_reference IndexSeqIterator::operator*() const
{
    return repn->operator*();
}

IndexSeqIterator::pointer IndexSeqIterator::operator->() { return repn->operator->(); }

IndexSeqIterator::const_pointer IndexSeqIterator::operator->() const
{
    return repn->operator->();
}

//
// IndexSequence
//
IndexSequence::IndexSequence(const std::shared_ptr<IndexSequenceRepn>& _repn)
    : repn(_repn)
{
}

IndexSequence::IndexSequence(const SequenceContext& context_)
{
    repn = std::make_shared<IndexSequenceRepn>(context_);
}

IndexSeqIterator IndexSequence::begin()
{
    return IndexSeqIterator(repn.get(), false);
}

IndexSeqIterator IndexSequence::end() { return IndexSeqIterator(repn.get(), true); }

const IndexSeqIterator IndexSequence::begin() const
{
    return IndexSeqIterator(repn.get(), false);
}

const IndexSeqIterator IndexSequence::end() const
{
    return IndexSeqIterator(repn.get(), true);
}

}  // namespace coek
