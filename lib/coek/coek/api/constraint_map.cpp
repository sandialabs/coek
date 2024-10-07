#include "coek/api/expression.hpp"
#include "coek/util/io_utils.hpp"
#include "coek/api/constraint_map.hpp"
#include "coek/api/indexed_container.defs.hpp"
#include "coek/model/model.hpp"
#include "coek/model/model_repn.hpp"

namespace coek {

template class IndexedComponent_Map<Constraint>;

void Model::add_constraint(ConstraintMap& cons)
{
    if (repn->name_generation_policy == Model::NameGeneration::eager)
        cons.generate_names();
    repn->constraint_maps.push_back(cons);
    for (auto& con : cons.repn->value)
        add_constraint(con.second);
}

}  // namespace coek
