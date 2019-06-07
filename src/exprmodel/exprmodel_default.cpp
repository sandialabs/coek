#include <fstream>
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


void print_repn(std::ostream& ostr, ExprRepn& repn, std::map<int,Variable*>& vars)
{
std::map<int,int> vindex;
int i=0;
for (std::vector<NumericValue*>::iterator it=repn.linear_vars.begin(); it != repn.linear_vars.end(); ++it, i++) {
    Variable* tmp = dynamic_cast<Variable*>(*it);
    vars[ tmp->index ] = tmp;
    vindex[ tmp->index ] = i;
    }

for (std::map<int,int>::iterator it=vindex.begin(); it != vindex.end(); ++it) {
    i = it->first;
    double tmp = repn.linear_coefs[it->second];
    if (tmp > 0)
        ostr << "+" << tmp << " x(" << i << ")" << std::endl;
    else if (tmp < 0)
        ostr << tmp << " x(" << i << ")" << std::endl;
    }
}

void ExprModel::write(std::string& filename)
{
if (objectives.size() == 0) {
    std::cerr << "Error writing LP file: No objectives specified!" << std::endl;
    return;
    }
if (objectives.size() > 1) {
    std::cerr << "Error writing LP file: More than one objective defined!" << std::endl;
    return;
    }

// Create file

std::map<int,Variable*> vars;
std::ofstream ofstr(filename);

ofstr << "\\* LP File *\\" << std::endl << std::endl;
ofstr << std::endl << "minimize" << std::endl << std::endl;

ofstr << "obj:" << std::endl;
{
ExprRepn repn;
objectives[0]->collect_terms(repn);
print_repn(ofstr, repn, vars);
}

ofstr << std::endl << "subject to" << std::endl << std::endl;

int ctr=0;
for (std::vector<NumericValue*>::iterator it=inequalities.begin(); it != inequalities.end(); ++it) {
    ofstr << "c" << ctr++ << ":" << std::endl;
    ExprRepn repn;
    (*it)->collect_terms(repn);
    print_repn(ofstr, repn, vars);
    double tmp = repn.constval;
    if (tmp == 0)
        ofstr << "<= 0" << std::endl << std::endl;
    else
        ofstr << "<= " << (-tmp) << std::endl << std::endl;
    }

for (std::vector<NumericValue*>::iterator it=equalities.begin(); it != equalities.end(); ++it) {
    ofstr << "c" << ctr++ << ":" << std::endl;
    ExprRepn repn;
    (*it)->collect_terms(repn);
    print_repn(ofstr, repn, vars);
    double tmp = repn.constval;
    if (tmp == 0)
        ofstr << "= 0" << std::endl << std::endl;
    else
        ofstr << "= " << (-tmp) << std::endl << std::endl;
    }

std::map<int,Variable*> bvars;
std::map<int,Variable*> ivars;
ofstr << std::endl << "bounds" << std::endl;
for(std::map<int,Variable*>::iterator it=vars.begin(); it != vars.end(); ++it) {
    ofstr << it->second->lb << " <= x(" << it->first << ") <= " << it->second->ub << std::endl;
    //std::cerr << it->first << " " << it->second->binary << std::endl;
    if (it->second->binary)
        bvars[it->first] = it->second;
    if (it->second->integer)
        ivars[it->first] = it->second;
    }

if (bvars.size() > 0) {
    ofstr << std::endl << "binary" << std::endl;
    for(std::map<int,Variable*>::iterator it=bvars.begin(); it != bvars.end(); ++it)
        ofstr << "x(" << it->first << ")" << std::endl;
    }

if (ivars.size() > 0) {
    ofstr << std::endl << "integer" << std::endl;
    for(std::map<int,Variable*>::iterator it=ivars.begin(); it != ivars.end(); ++it)
        ofstr << "x(" << it->first << ")" << std::endl;
    }

ofstr << std::endl << "end" << std::endl;
ofstr.close();
}

}

}
