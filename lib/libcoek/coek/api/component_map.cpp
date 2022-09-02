#include "coek/model/model.hpp"
#ifdef COEK_WITH_COMPACT_MODEL
#include "coek/api/objective.hpp"
#include "coek/api/constraint.hpp"
#include "coek/model/compact_model.hpp"
#endif
#include "coek/api/component_map.hpp"
#include "coek/api/indexed_container.defs.hpp"


namespace coek {

template class IndexedComponent_Map<Constraint>;


void Model::add_constraint(ConstraintMap& cons)
{
for (auto& con: cons.repn->value)
    add_constraint(con.second);
}

#ifdef COEK_WITH_COMPACT_MODEL
void CompactModel::add_constraint(ConstraintMap& cons)
{
for (auto& con: cons.repn->value)
    add_constraint(con.second);
}
#endif

}
