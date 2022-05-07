#include "coek/model/model.hpp"
#include "coek/api/component_map.hpp"


namespace coek {

ConstraintMap& Model::add_constraint(ConstraintMap& cons)
{
for (auto& con: cons.repn->value)
    add_constraint(con.second);
return cons;
}

}
