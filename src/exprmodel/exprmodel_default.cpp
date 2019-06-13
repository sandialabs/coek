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


void print_repn(std::ostream& ostr, QuadraticExprRepn& repn)
{
if (repn.linear_coefs.size() > 0) {
    std::map<int,double> vval;
    int i=0;
    for (std::vector<Variable*>::iterator it=repn.linear_vars.begin(); it != repn.linear_vars.end(); ++it, i++) {
        //Variable* tmp = dynamic_cast<Variable*>(*it);
        //vars[ tmp->index ] = tmp;
        int index = (*it)->index;

        std::map<int,double>::iterator curr = vval.find(index);
        if (curr == vval.end())
            vval[index ] = repn.linear_coefs[i];
        else
            vval[index ] += repn.linear_coefs[i];
        }

    for (std::map<int,double>::iterator it=vval.begin(); it != vval.end(); ++it) {
        i = it->first;
        double tmp = it->second;
        if (tmp > 0)
            ostr << "+" << tmp << " x(" << i << ")" << std::endl;
        else if (tmp < 0)
            ostr << tmp << " x(" << i << ")" << std::endl;
        }
    }

if (repn.quadratic_coefs.size() > 0) {
    ostr << "+ [" << std::endl;
    std::map<std::pair<int,int>,double> qval;
    for (size_t ii=0; ii<repn.quadratic_coefs.size(); ii++) {
        //Variable* lvar = dynamic_cast<Variable*>(repn.quadratic_lvars[ii]);
        //vars[ lvar->index ] = lvar;
        //Variable* rvar = dynamic_cast<Variable*>(repn.quadratic_rvars[ii]);
        //vars[ rvar->index ] = rvar;
        int lindex = repn.quadratic_lvars[ii]->index;
        int rindex = repn.quadratic_rvars[ii]->index;

        std::pair<int,int> tmp;
        if (lindex < rindex)
            tmp = std::pair<int,int>(lindex, rindex);
        else
            tmp = std::pair<int,int>(rindex, lindex);

        std::map<std::pair<int,int>,double>::iterator curr = qval.find(tmp);
        if (curr == qval.end())
            qval[ tmp ] = repn.quadratic_coefs[ii];
        else
            qval[ tmp ] += repn.quadratic_coefs[ii];
        }

    for (std::map<std::pair<int,int>,double>::iterator it=qval.begin(); it != qval.end(); ++it) {
        const std::pair<int,int>& tmp = it->first;
        double val = it->second;
        if (tmp.first == tmp.second) {
            if (val > 0)
                ostr << "+" << val << " x(" << tmp.first << ") ^ 2" << std::endl;
            else if (val < 0)
                ostr << val << " x(" << tmp.first << ") ^ 2" << std::endl;
            }
        else {
            if (val > 0)
                ostr << "+" << val << " x(" << tmp.first << ") * x(" << tmp.second << ")" << std::endl;
            else if (val < 0)
                ostr << val << " x(" << tmp.first << ") * x(" << tmp.second << ")" << std::endl;
            }
        }
    ostr << "]" << std::endl;
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

vars_t vars;
std::ofstream ofstr(filename);

ofstr << "\\* LP File *\\" << std::endl << std::endl;
ofstr << std::endl << "minimize" << std::endl << std::endl;

ofstr << "obj:" << std::endl;
{
QuadraticExprRepn repn;
objectives[0]->collect_terms(repn, vars);
print_repn(ofstr, repn);
}

ofstr << std::endl << "subject to" << std::endl << std::endl;

int ctr=0;
for (std::vector<NumericValue*>::iterator it=inequalities.begin(); it != inequalities.end(); ++it) {
    ofstr << "c" << ctr++ << ":" << std::endl;
    QuadraticExprRepn repn;
    (*it)->collect_terms(repn, vars);
    print_repn(ofstr, repn);
    double tmp = repn.constval;
    if (tmp == 0)
        ofstr << "<= 0" << std::endl << std::endl;
    else
        ofstr << "<= " << (-tmp) << std::endl << std::endl;
    }

for (std::vector<NumericValue*>::iterator it=equalities.begin(); it != equalities.end(); ++it) {
    ofstr << "c" << ctr++ << ":" << std::endl;
    QuadraticExprRepn repn;
    (*it)->collect_terms(repn, vars);
    print_repn(ofstr, repn);
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
    if (it->second->lb < -1.0e18)
        ofstr << "-inf";
    else
        ofstr << it->second->lb;
    ofstr << " <= x(" << it->first << ") <= ";
    if (it->second->ub > 1.0e18)
        ofstr << "inf" << std::endl;
    else
        ofstr << it->second->ub << std::endl;
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
