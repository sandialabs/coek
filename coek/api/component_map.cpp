#include "coek/model/model.hpp"
#include "coek/api/component_map.hpp"
#include "coek/api/indexed_container.defs.hpp"


namespace coek {

template class IndexedComponent_Map<Constraint>;


void Model::add_constraint(ConstraintMap& cons)
{
for (auto& con: cons.repn->value)
    add_constraint(con.second);
}

}
