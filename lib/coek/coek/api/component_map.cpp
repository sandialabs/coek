#ifdef COEK_WITH_COMPACT_MODEL
#    include "coek/api/constraint.hpp"
#    include "coek/api/objective.hpp"
#    include "coek/model/compact_model.hpp"
#endif
#include "coek/util/io_utils.hpp"
#include "coek/api/component_map.hpp"
#include "coek/api/indexed_container.defs.hpp"
#include "coek/model/model.hpp"
#include "coek/model/model_repn.hpp"

namespace coek {

template class IndexedComponent_Map<Constraint>;

void Model::add_constraint(ConstraintMap& cons)
{
    if (repn->name_generation_policy == Model::NameGeneration::eager)
        cons.generate_names();
    else if (repn->name_generation_policy == Model::NameGeneration::lazy)
        repn->constraint_maps.push_back(cons);
    for (auto& con : cons.repn->value) add_constraint(con.second);
}

#ifdef COEK_WITH_COMPACT_MODEL
void CompactModel::add_constraint(ConstraintMap& cons)
{
    // TODO - name management here
    for (auto& con : cons.repn->value) add_constraint(con.second);
}
#endif

}  // namespace coek
