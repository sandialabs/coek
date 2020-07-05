#include "coek/expr/coek_exprterm.hpp"
#include "coek/api/expression.hpp"
#include "coek/api/constraint.hpp"

#include "ast_set.hpp"
#include "coek_sets.hpp"
#include "sequence_context.hpp"


namespace coek {

Context::Context(const std::vector<IndexParameter>& _indices)
    : indices(_indices)
{}

}
