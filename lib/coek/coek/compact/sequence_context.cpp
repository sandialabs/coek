
#include "ast_set.hpp"
#include "coek/api/constraint.hpp"
#include "coek/api/expression.hpp"
#include "coek_exprterm.hpp"
#include "coek_sets.hpp"
#include "sequence_context.hpp"

namespace coek {

//
// Context
//

Context::Context(const std::vector<IndexParameter>& _indices) : indices(_indices) {}

//
// SequenceContextRepn
//

class SequenceContextRepn {
   public:
    std::vector<Context> context;
};

//
// SequenceContext
//

SequenceContext::SequenceContext(const std::shared_ptr<SequenceContextRepn>& _repn) : repn(_repn) {}

SequenceContext SequenceContext::In(const ConcreteSet& _index_set)
{
    Context& curr = repn->context.back();
    curr.index_set = _index_set;
    return repn;
}

SequenceContext SequenceContext::ST(const Constraint& con)
{
    Context& curr = repn->context.back();
    curr.index_values.emplace_back(con);
    return repn;
}

SequenceContext SequenceContext::Where(const Constraint& con)
{
    Context& curr = repn->context.back();
    curr.index_constraints.emplace_back(con);
    return repn;
}

size_t SequenceContext::size() const { return repn->context.size(); }

ConcreteSet SequenceContext::index_set() const
{
    if (repn->context.size() == 0) {
        ConcreteSet tmp;
        return tmp;
    }
    else if (repn->context.size() == 1)
        return repn->context[0].index_set;
    else if (repn->context.size() == 2)
        return repn->context[0].index_set * repn->context[1].index_set;
    else {
        auto tmp = repn->context[0].index_set * repn->context[1].index_set;
        for (size_t i = 2; i < repn->context.size(); i++)
            tmp *= repn->context[i].index_set;
        return tmp;
    }
}

Context& SequenceContext::operator[](size_t i) { return repn->context[i]; }

SequenceContext SequenceContext::Forall(const std::vector<IndexParameter>& indices)
{
    repn->context.emplace_back(indices);
    return repn;
}

//
// Forall
//

SequenceContext Forall(const std::vector<IndexParameter>& indices)
{
    auto repn = std::make_shared<SequenceContextRepn>();
    repn->context.emplace_back(indices);
    return repn;
}

}  // namespace coek
