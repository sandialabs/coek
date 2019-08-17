#include <map>
#include "expr/ast_visitors.hpp"
#include "coek_model.hpp"

namespace coek {

namespace {

void print_repn(std::ostream& ostr, QuadraticExpr& repn)
{
if (repn.linear_coefs.size() > 0) {
    std::map<int,double> vval;
    int i=0;
    for (std::vector<VariableTerm*>::iterator it=repn.linear_vars.begin(); it != repn.linear_vars.end(); ++it, i++) {
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

}

void write_lp_problem(Model& model, std::ostream& ostr)
{
if (model.objectives.size() == 0) {
    std::cerr << "Error writing LP file: No objectives specified!" << std::endl;
    return;
    }
if (model.objectives.size() > 1) {
    std::cerr << "Error writing LP file: More than one objective defined!" << std::endl;
    return;
    }

// Create file

ostr << "\\* LP File *\\" << std::endl << std::endl;
ostr << std::endl << "minimize" << std::endl << std::endl;

ostr << "obj:" << std::endl;
{
QuadraticExpr expr;
expr.collect_terms(model.objectives[0]);
//write_expr(objectives[0].repn, std::cout);
//std::cout << std::endl;
//std::cout << expr << std::endl;
print_repn(ostr, expr);
}

ostr << std::endl << "subject to" << std::endl << std::endl;

int ctr=0;
for (std::vector<Constraint>::iterator it=model.constraints.begin(); it != model.constraints.end(); ++it) {
    ostr << "c" << ctr++ << ":" << std::endl;
    QuadraticExpr expr;
    expr.collect_terms(*it);
    //write_expr(it->repn, std::cout);
    //std::cout << std::endl;
    //std::cout << expr << std::endl;
    print_repn(ostr, expr);
    if (it->is_inequality())
        ostr << "<= ";
    else
        ostr << "= ";
    double tmp = expr.constval;
    if (tmp == 0)
        ostr << "0" << std::endl << std::endl;
    else
        ostr << (-tmp) << std::endl << std::endl;
    }

std::map<int,VariableTerm*> bvars;
std::map<int,VariableTerm*> ivars;
ostr << std::endl << "bounds" << std::endl;
for(std::vector<Variable>::iterator it=model.variables.begin(); it != model.variables.end(); ++it) {
    VariableTerm* v = it->repn;
    if (v->lb <= -COEK_INFINITY)
        ostr << "-inf";
    else
        ostr << v->lb;
    ostr << " <= x(" << v->index << ") <= ";
    if (v->ub >= COEK_INFINITY)
        ostr << "inf" << std::endl;
    else
        ostr << v->ub << std::endl;
    if (v->binary)
        bvars[v->index] = v;
    if (v->integer)
        ivars[v->index] = v;
    }

if (bvars.size() > 0) {
    ostr << std::endl << "binary" << std::endl;
    for(std::map<int,VariableTerm*>::iterator it=bvars.begin(); it != bvars.end(); ++it)
        ostr << "x(" << it->first << ")" << std::endl;
    }

if (ivars.size() > 0) {
    ostr << std::endl << "integer" << std::endl;
    for(std::map<int,VariableTerm*>::iterator it=ivars.begin(); it != ivars.end(); ++it)
        ostr << "x(" << it->first << ")" << std::endl;
    }

ostr << std::endl << "end" << std::endl;
}

}
