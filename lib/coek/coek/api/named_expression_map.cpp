#ifdef COEK_WITH_COMPACT_MODEL
#    include "coek/api/expression.hpp"
#    include "coek/model/compact_model.hpp"
#endif
#include "coek/util/io_utils.hpp"
#include "coek/api/named_expression_map.hpp"
#include "coek/api/indexed_container.defs.hpp"
#include "coek/model/model.hpp"
#include "coek/model/model_repn.hpp"

namespace coek {

template class IndexedComponent_Map<NamedExpression>;

/*
void Model::add_named_expression(NamedExpressionMap& exprs)
{
    if (repn->name_generation_policy == Model::NameGeneration::eager)
        exprs.generate_names();
    else if (repn->name_generation_policy == Model::NameGeneration::lazy)
        repn->named_expression_maps.push_back(exprs);
    for (auto& con : exprs.repn->value) add_named_expression(con.second);
}
*/

#ifdef COEK_WITH_COMPACT_MODEL
/*
void CompactModel::add_named_expression(NamedExpressionMap& exprs)
{
    // TODO - name management here
    for (auto& con : exprs.repn->value) add_named_expression(con.second);
}
*/
#endif

}  // namespace coek
