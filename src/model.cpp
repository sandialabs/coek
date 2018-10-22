
#include <cassert>
#include "model.hpp"


void Model::set_variables(const double* x, int n)
{
assert(n == variables.size());
int j=0;
for (std::vector<Variable*>::iterator it=variables.begin(); it != variables.end(); it++) {
    (*it)->_value = x[j];
    j++;
    }
}


void Model::set_variables(std::vector<double>& x)
{
assert(x.size() == variables.size());
int j=0;
for (std::vector<Variable*>::iterator it=variables.begin(); it != variables.end(); it++) {
    (*it)->_value = x[j];
    j++;
    }
}


void Model::print(std::ostream& ostr, int df)
{
ostr << "MODEL" << std::endl;

ostr << "  Objectives" << std::endl;
for (std::list<Expression*>::iterator it=objectives.begin(); it != objectives.end(); ++it) {
    ostr << "    ";
    (*it)->print(ostr);
    ostr << std::endl;
    }
ostr << std::endl;

ostr << "  Inequality Constraints" << std::endl;
for (std::list<Expression*>::iterator it=inequalities.begin(); it != inequalities.end(); ++it) {
    ostr << "    ";
    (*it)->print(ostr);
    ostr << std::endl;
    }
ostr << std::endl;

ostr << "  Equality Constraints" << std::endl;
for (std::list<Expression*>::iterator it=equalities.begin(); it != equalities.end(); ++it) {
    ostr << "    ";
    (*it)->print(ostr);
    ostr << std::endl;
    }
}

