#define _GLIBCXX_USE_CxX11_ABI 0
#include "coek_gurobi.hpp"
#include "gurobi_c++.h"



void collect_variables(QuadraticExprRepn& repn, std::map<int,Variable*>& vars)
{
for (std::vector<Variable*>::iterator it=repn.linear_vars.begin(); it != repn.linear_vars.end(); ++it) {
    Variable* tmp = dynamic_cast<Variable*>(*it);
    vars[ tmp->index ] = tmp;
    }
for (std::vector<Variable*>::iterator it=repn.quadratic_lvars.begin(); it != repn.quadratic_lvars.end(); ++it) {
    Variable* tmp = dynamic_cast<Variable*>(*it);
    vars[ tmp->index ] = tmp;
    }
for (std::vector<Variable*>::iterator it=repn.quadratic_rvars.begin(); it != repn.quadratic_rvars.end(); ++it) {
    Variable* tmp = dynamic_cast<Variable*>(*it);
    vars[ tmp->index ] = tmp;
    }
}


int coek::GurobiSolver::solve()
{
GRBEnv env;
GRBModel gmodel(env);
gmodel.getEnv().set(GRB_DoubleParam_TimeLimit, 0);

std::map<int, Variable*> vars;
std::map<int,GRBVar> x;

// Collect repns
QuadraticExprRepn orepn;
model->objectives[0]->collect_terms(orepn);
collect_variables(orepn, vars);

std::vector<QuadraticExprRepn> irepn(model->inequalities.size());
for (size_t i=0; i<model->inequalities.size(); i++) {
    model->inequalities[i]->collect_terms( irepn[i] );
    collect_variables(irepn[i], vars);
    }

std::vector<QuadraticExprRepn> erepn(model->equalities.size());
for (size_t i=0; i<model->equalities.size(); i++) {
    model->equalities[i]->collect_terms( erepn[i] );
    collect_variables(erepn[i], vars);
    }

// Add Gurobi variables
for (std::map<int, Variable*>::iterator it=vars.begin(); it != vars.end(); ++it) {
    Variable* v = it->second;
    if (v->binary)
        x[ it->first ] = gmodel.addVar(v->lb, v->ub, 0, GRB_BINARY);
    else if (v->integer)
        x[ it->first ] = gmodel.addVar(v->lb, v->ub, 0, GRB_INTEGER);
    else {
        if (v->ub >= 1e19) {
            if (v->lb <= -1e19)
                x[ it->first ] = gmodel.addVar(GRB_INFINITY, GRB_INFINITY, 0, GRB_CONTINUOUS);
            else
                x[ it->first ] = gmodel.addVar(v->lb, GRB_INFINITY, 0, GRB_CONTINUOUS);
            }
        else {
            if (v->lb <= -1e19)
                x[ it->first ] = gmodel.addVar(GRB_INFINITY, v->ub, 0, GRB_CONTINUOUS);
            else
                x[ it->first ] = gmodel.addVar(v->lb, v->ub, 0, GRB_CONTINUOUS);
            }
        }
    }

gmodel.update();

// Add Gurobi objective
try {
    GRBLinExpr term1;
    for (size_t i=0; i< orepn.linear_coefs.size(); i++)
        term1 += orepn.linear_coefs[i] * x[orepn.linear_vars[i]->index];
    if (orepn.quadratic_coefs.size() > 0) {
        GRBQuadExpr term2;
        for (size_t i=0; i< orepn.quadratic_coefs.size(); i++)
            term2 += orepn.quadratic_coefs[i] * x[orepn.quadratic_lvars[i]->index] * x[orepn.quadratic_rvars[i]->index];
        gmodel.setObjective(term1 + term2);
        }
    else
        gmodel.setObjective(term1);
    }
catch (GRBException e) {
    std::cerr << "GUROBI Exception: (objective) " << e.getMessage() << std::endl;
    throw;
    }

// Add Gurobi inequality constraints
try {
    for (std::vector<QuadraticExprRepn>::iterator it=irepn.begin(); it != irepn.end(); ++it) {
        QuadraticExprRepn& repn = *it;
        GRBLinExpr term1;
        for (size_t i=0; i< repn.linear_coefs.size(); i++)
            term1 += repn.linear_coefs[i] * x[repn.linear_vars[i]->index];
        if (repn.quadratic_coefs.size() > 0) {
            GRBQuadExpr term2;
            for (size_t i=0; i< repn.quadratic_coefs.size(); i++)
                term2 += repn.quadratic_coefs[i] * x[repn.quadratic_lvars[i]->index] * x[repn.quadratic_rvars[i]->index];
            gmodel.addQConstr(term1 + term2 <= - repn.constval);
            }
        else
            gmodel.addConstr(term1 <= - repn.constval);
        }
    }
catch (GRBException e) {
    std::cerr << "GUROBI Exception: (ineq) " << e.getMessage() << std::endl;
    throw;
    }

// Add Gurobi equality constraints
try {
    for (std::vector<QuadraticExprRepn>::iterator it=erepn.begin(); it != erepn.end(); ++it) {
        QuadraticExprRepn& repn = *it;
        GRBLinExpr term1;
        for (size_t i=0; i< repn.linear_coefs.size(); i++)
            term1 += repn.linear_coefs[i] * x[repn.linear_vars[i]->index];
        if (repn.quadratic_coefs.size() > 0) {
            GRBQuadExpr term2;
            for (size_t i=0; i< repn.quadratic_coefs.size(); i++)
                term2 += repn.quadratic_coefs[i] * x[repn.quadratic_lvars[i]->index] * x[repn.quadratic_rvars[i]->index];
            gmodel.addQConstr(term1 + term2 == - repn.constval);
            }
        else
            gmodel.addConstr(term1 == - repn.constval);
        }
    }
catch (GRBException e) {
    std::cerr << "GUROBI Exception: (eq) " << e.getMessage() << std::endl;
    throw;
    }

gmodel.optimize();

return 0;
}



