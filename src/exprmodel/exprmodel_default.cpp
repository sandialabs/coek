#include "exprmodel/exprmodel_default.hpp"

namespace coek {

namespace exprmodel_default {


void ExprModel::print(std::ostream& ostr)
{
ostr << "MODEL" << std::endl;

ostr << "  Objectives" << std::endl;
for (std::vector<NumericValue*>::iterator it=objectives.begin(); it != objectives.end(); ++it) {
    ostr << "    ";
    (*it)->print(ostr);
    ostr << std::endl;
    }
ostr << std::endl;

ostr << "  Inequality Constraints" << std::endl;
for (std::vector<NumericValue*>::iterator it=inequalities.begin(); it != inequalities.end(); ++it) {
    ostr << "    ";
    (*it)->print(ostr);
    ostr << std::endl;
    }
ostr << std::endl;

ostr << "  Equality Constraints" << std::endl;
for (std::vector<NumericValue*>::iterator it=equalities.begin(); it != equalities.end(); ++it) {
    ostr << "    ";
    (*it)->print(ostr);
    ostr << std::endl;
    }
}

}

}
