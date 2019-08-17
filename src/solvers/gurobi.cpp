#define _GLIBCXX_USE_CxX11_ABI 0
#include <cassert>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include "gurobi_c++.h"

#include "coek_gurobi.hpp"
#include "expr/ast_term.hpp"
#include "coek_model.hpp"

namespace coek {

#if 0
namespace {

void write(std::ostream& ostr, const std::list<std::string>& slist)
{
bool flag=false;
ostr << "{ ";
for (std::list<std::string>::const_iterator it=slist.begin(); it != slist.end(); ++it) {
    if (flag) {
        ostr << "  ";
        }
    flag=true;
    ostr << *it;
    }
ostr << "} ";
ostr << std::endl;
}

}
#endif


GurobiSolver::~GurobiSolver()
{
if (gmodel)
    delete gmodel;
if (env)
    delete env;
}


int GurobiSolver::solve(Model& model)
{
std::cout << "STARTING GUROBI" << std::endl << std::flush;

env = new GRBEnv();
gmodel = new GRBModel(*env);

std::cout << "COLLECTING REPNS/VARS" << std::endl << std::flush;
assert(model.objectives.size() == 1);

// Collect repns
coek::QuadraticExpr orepn;
orepn.collect_terms(model.objectives[0]);

std::vector<coek::QuadraticExpr> crepn(model.constraints.size());
for (size_t i=0; i<model.constraints.size(); i++)
    crepn[i].collect_terms(model.constraints[i]);

std::cout << "BUILDING GUROBI MODEL" << std::endl << std::flush;

// Add Gurobi variables
for (std::vector<coek::Variable>::iterator it=model.variables.begin(); it != model.variables.end(); ++it) {
    coek::VariableTerm* v = it->repn;
    if (v->binary)
        x[ v->index ] = gmodel->addVar(v->lb, v->ub, 0, GRB_BINARY);
    else if (v->integer)
        x[ v->index ] = gmodel->addVar(v->lb, v->ub, 0, GRB_INTEGER);
    else {
        if (v->ub >= 1e19) {
            if (v->lb <= -1e19)
                x[ v->index ] = gmodel->addVar(GRB_INFINITY, GRB_INFINITY, 0, GRB_CONTINUOUS);
            else
                x[ v->index ] = gmodel->addVar(v->lb, GRB_INFINITY, 0, GRB_CONTINUOUS);
            }
        else {
            if (v->lb <= -1e19)
                x[ v->index ] = gmodel->addVar(GRB_INFINITY, v->ub, 0, GRB_CONTINUOUS);
            else
                x[ v->index ] = gmodel->addVar(v->lb, v->ub, 0, GRB_CONTINUOUS);
            }
        }
    }

gmodel->update();

// Add Gurobi objective
try {
    if (orepn.linear_coefs.size() + orepn.quadratic_coefs.size() > 0) {
        GRBLinExpr term1;
        for (size_t i=0; i< orepn.linear_coefs.size(); i++)
            term1 += orepn.linear_coefs[i] * x[orepn.linear_vars[i]->index];
        term1 += orepn.constval;
        if (orepn.quadratic_coefs.size() > 0) {
            GRBQuadExpr term2;
            for (size_t i=0; i< orepn.quadratic_coefs.size(); i++)
                term2 += orepn.quadratic_coefs[i] * x[orepn.quadratic_lvars[i]->index] * x[orepn.quadratic_rvars[i]->index];
            gmodel->setObjective(term1 + term2);
            }
        else
            gmodel->setObjective(term1);
        }

    if (model.sense[0])
        gmodel->set(GRB_IntAttr_ModelSense, GRB_MINIMIZE);
    else
        gmodel->set(GRB_IntAttr_ModelSense, GRB_MAXIMIZE);
    }
catch (GRBException e) {
    std::cerr << "GUROBI Exception: (objective) " << e.getMessage() << std::endl;
    throw;
    }

// Add Gurobi constraints
try {
    int ii=0;
    for (std::vector<coek::QuadraticExpr>::iterator it=crepn.begin(); it != crepn.end(); ++it, ii++) {
        coek::QuadraticExpr& repn = *it;
        if (repn.linear_coefs.size() + repn.quadratic_coefs.size() > 0) {
            GRBLinExpr term1;
#if 0
            std::vector<GRBVar> vars1(repn.linear_coefs.size());  
            for (size_t i=0; i< vars1.size(); i++)
                vars1[i] = x[repn.linear_vars[i]->index];
            term1.addTerms(&(repn.linear_coefs[0]), &(vars1[0]), vars1.size());
#else
            for (size_t i=0; i< repn.linear_coefs.size(); i++)
                term1 += repn.linear_coefs[i] * x[repn.linear_vars[i]->index];
#endif
            if (repn.quadratic_coefs.size() > 0) {
                GRBQuadExpr term2;
#if 0
                std::vector<GRBVar> vars2a(repn.quadratic_coefs.size());  
                std::vector<GRBVar> vars2b(repn.quadratic_coefs.size());  
                for (size_t i=0; i< vars2a.size(); i++) {
                    vars2a[i] = x[repn.quadratic_lvars[i]->index];
                    vars2b[i] = x[repn.quadratic_rvars[i]->index];
                    }
                term2.addTerms(&(repn.quadratic_coefs[0]), &(vars2a[0]), &(vars2b[0]), vars2a.size());
#else
                for (size_t i=0; i< repn.quadratic_coefs.size(); i++)
                    term2 += repn.quadratic_coefs[i] * x[repn.quadratic_lvars[i]->index] * x[repn.quadratic_rvars[i]->index];
#endif

                if (model.constraints[ii].is_inequality())
                    gmodel->addQConstr(term1 + term2, GRB_LESS_EQUAL, - repn.constval);
                else
                    gmodel->addQConstr(term1 + term2, GRB_EQUAL, - repn.constval);
                }
            else {
                if (model.constraints[ii].is_inequality())
                    gmodel->addConstr(term1, GRB_LESS_EQUAL, - repn.constval);
                else
                    gmodel->addConstr(term1, GRB_EQUAL, - repn.constval);
                }
            }
        }
    }
catch (GRBException e) {
    std::cerr << "GUROBI Exception: (constraint) " << e.getMessage() << std::endl;
    throw;
    }

std::cout << "OPTIMIZING GUROBI MODEL" << std::endl << std::flush;
// All options are converted to strings for Gurobi
for (auto it=string_options.begin(); it != string_options.end(); ++it)
    gmodel->set(it->first, it->second);
gmodel->optimize();

// Collect values of Gurobi variables
for (std::vector<coek::Variable>::iterator it=model.variables.begin(); it != model.variables.end(); ++it) {
    coek::VariableTerm* v = it->repn;
    v->value = x[v->index].get(GRB_DoubleAttr_X);
    }

delete gmodel;
gmodel = 0;
delete env;
env = 0;

return 0;
}


int GurobiSolver::resolve()
{
if (initial_solve()) {
    std::cout << "STARTING GUROBI" << std::endl << std::flush;

    env = new GRBEnv();
    gmodel = new GRBModel(*env);

    std::cout << "BUILDING GUROBI MODEL" << std::endl << std::flush;
    assert(model->objectives.size() == 1);

    // Add Gurobi variables
    for (std::vector<coek::Variable>::iterator it=model->variables.begin(); it != model->variables.end(); ++it) {
        coek::VariableTerm* v = it->repn;
        if (v->binary)
            x[ v->index ] = gmodel->addVar(v->lb, v->ub, 0, GRB_BINARY);
        else if (v->integer)
            x[ v->index ] = gmodel->addVar(v->lb, v->ub, 0, GRB_INTEGER);
        else {
            if (v->ub >= 1e19) {
                if (v->lb <= -1e19)
                    x[ v->index ] = gmodel->addVar(GRB_INFINITY, GRB_INFINITY, 0, GRB_CONTINUOUS);
                else
                    x[ v->index ] = gmodel->addVar(v->lb, GRB_INFINITY, 0, GRB_CONTINUOUS);
                }
            else {
                if (v->lb <= -1e19)
                    x[ v->index ] = gmodel->addVar(GRB_INFINITY, v->ub, 0, GRB_CONTINUOUS);
                else
                    x[ v->index ] = gmodel->addVar(v->lb, v->ub, 0, GRB_CONTINUOUS);
                }
            }
        }

    gmodel->update();

    size_t ii=0;
    if (model->objectives.size() != 1)
        throw std::runtime_error("GUROBI Exception: multiple objectives declared");
    
    // Add Gurobi objective
    try {
        coek::MutableNLPExpr& _repn = repn[ii];
        GRBLinExpr term1;
        for (size_t i=0; i< _repn.linear_coefs.size(); i++)
            term1 += _repn.linear_coefs[i].get_value() * x[_repn.linear_vars[i]->index];
        term1 += _repn.constval.get_value();
        if (_repn.quadratic_coefs.size() > 0) {
            GRBQuadExpr term2;
            for (size_t i=0; i< _repn.quadratic_coefs.size(); i++)
                term2 += _repn.quadratic_coefs[i].get_value() * x[_repn.quadratic_lvars[i]->index] * x[_repn.quadratic_rvars[i]->index];
            gmodel->setObjective(term1 + term2);
            }
        else
            gmodel->setObjective(term1);

        if (model->sense[0])
            gmodel->set(GRB_IntAttr_ModelSense, GRB_MINIMIZE);
        else
            gmodel->set(GRB_IntAttr_ModelSense, GRB_MAXIMIZE);
        }
    catch (GRBException e) {
        std::cerr << "GUROBI Exception: (objective) " << e.getMessage() << std::endl;
        throw;
        }

    // Add Gurobi constraints
    try {
        size_t nobjectives = model->objectives.size();
        for (ii=1; ii<repn.size(); ii++) {
            coek::MutableNLPExpr& _repn = repn[ii];
            if (_repn.linear_coefs.size() + _repn.quadratic_coefs.size() > 0) {
                GRBLinExpr term1;
                for (size_t i=0; i< _repn.linear_coefs.size(); i++)
                    term1 += _repn.linear_coefs[i].get_value() * x[_repn.linear_vars[i]->index];
                if (_repn.quadratic_coefs.size() > 0) {
                    GRBQuadExpr term2;
                    for (size_t i=0; i< _repn.quadratic_coefs.size(); i++)
                        term2 += _repn.quadratic_coefs[i].get_value() * x[_repn.quadratic_lvars[i]->index] * x[_repn.quadratic_rvars[i]->index];

                    if (model->constraints[ii-nobjectives].is_inequality())
                        gmodel->addQConstr(term1 + term2, GRB_LESS_EQUAL, - _repn.constval.get_value());
                    else
                        gmodel->addQConstr(term1 + term2, GRB_EQUAL, - _repn.constval.get_value());
                    }
                else {
                    if (model->constraints[ii-nobjectives].is_inequality())
                        gmodel->addConstr(term1, GRB_LESS_EQUAL, - _repn.constval.get_value());
                    else
                        gmodel->addConstr(term1, GRB_EQUAL, - _repn.constval.get_value());
                    }
                }
            }
        }
    catch (GRBException e) {
        std::cerr << "GUROBI Exception: (constraint) " << e.getMessage() << std::endl;
        throw;
        }

    std::cout << "OPTIMIZING GUROBI MODEL" << std::endl << std::flush;
    // All options are converted to strings for Gurobi
    for (auto it=string_options.begin(); it != string_options.end(); ++it)
        gmodel->set(it->first, it->second);
    gmodel->optimize();
    }

else {
    std::cout << "UPDATING GUROBI MODEL" << std::endl << std::flush;

    for (auto it=updated_coefs.begin(); it != updated_coefs.end(); ++it) {
        int i=std::get<0>(*it);
        int where=std::get<1>(*it);
        int j=std::get<2>(*it);

        switch (where) {
            case 0:     // Constant Value
                if (i > 0)
                    gmodel->getConstr(i-1).set(GRB_DoubleAttr_RHS, repn[i].constval.get_value() );
                else
                    gmodel->set(GRB_DoubleAttr_ObjCon, repn[0].constval.get_value());
                break;

            case 1:     // Linear Coef
                if (i > 0)
                    gmodel->chgCoeff( gmodel->getConstr(i-1), x[ repn[i].linear_vars[j]->index ], repn[i].linear_coefs[j].get_value() );
                else
                    x[ repn[0].linear_vars[j]->index ].set(GRB_DoubleAttr_Obj, repn[0].linear_coefs[j].get_value());
                break;

            case 2:     // Quadratic Coef
                break;

            case 3:     // Nonlinear terms
                throw std::runtime_error("Gurobi cannot optimize models with nonlinear terms.");
                break;
            };
        }

    std::cout << "OPTIMIZING GUROBI MODEL" << std::endl << std::flush;
    //gmodel->getEnv().set(GRB_DoubleParam_TimeLimit, 0);
    gmodel->optimize();
    }
    
// Collect values of Gurobi variables
for (std::vector<coek::Variable>::iterator it=model->variables.begin(); it != model->variables.end(); ++it) {
    coek::VariableTerm* v = it->repn;
    v->value = x[v->index].get(GRB_DoubleAttr_X);
    }

return 0;
}

bool GurobiSolver::get_option(const std::string& option, int& value) const
{
auto curr = string_options.find(option);
if (curr == string_options.end())
    return false;
value = atoi(curr->second.c_str());
return true;
}

bool GurobiSolver::get_option(const std::string& option, double& value) const
{
auto curr = string_options.find(option);
if (curr == string_options.end())
    return false;
value = atoi(curr->second.c_str());
return true;
}

void GurobiSolver::set_option(const std::string& option, int value)
{
std::stringstream ostr;
ostr << value;
string_options[option] = ostr.str();
}

void GurobiSolver::set_option(const std::string& option, double value)
{
std::stringstream ostr;
ostr << value;
string_options[option] = ostr.str();
}

}
