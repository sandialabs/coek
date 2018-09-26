
#include <cassert>
#include "model.h"


///
/// MODEL
///


void Model::set_variables(std::vector<double>& x)
{
///std::cout << "x=" << x.size() << "  vars=" << variables.size() << std::endl << std::flush;
assert(x.size() == variables.size());
int j=0;
for (variables_iterator_type it=variables.begin(); it != variables.end(); it++) {
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



///
/// MODEL1
///

void Model1::build()
{
builds_f.resize(objectives.size() + inequalities.size() + equalities.size());
int nb=0;

for (std::list<Expression*>::iterator it=objectives.begin(); it != objectives.end(); ++it) {
    build_expression(*it, builds_f[nb], variables);
    nb++;
    }

for (std::list<Expression*>::iterator it=inequalities.begin(); it != inequalities.end(); ++it) {
    build_expression(*it, builds_f[nb], variables);
    nb++;
    }

for (std::list<Expression*>::iterator it=equalities.begin(); it != equalities.end(); ++it) {
    build_expression(*it, builds_f[nb], variables);
    nb++;
    }
}

double Model1::_compute_f(unsigned int i)
{
assert(i < builds_f.size());
std::list<NumericValue*>& tmp = builds_f[i];

///std::cout << "NVAR " << variables.size() << std::endl;
double ans = 0.0;
///std::cout << "HERE " << tmp.size() << std::endl << std::endl;
for (std::list<NumericValue*>::iterator it=tmp.begin(); it != tmp.end(); it++) {
    ans = (*it)->compute_value();
    ///std::cout << "ANS " << ans << std::endl;
    ///(*it)->print(std::cout);
    ///std::cout << std::endl;
    }

return ans;
}

void Model1::_compute_df(double& f, std::vector<double>& df, unsigned int i)
{
assert(i < builds_f.size());
assert(variables.size() == df.size());
std::list<NumericValue*>& build = builds_f[i];

/// _compute_f + set adjoint=0
double ans = 0.0;
for (std::list<NumericValue*>::iterator it=build.begin(); it != build.end(); it++) {
    NumericValue* tmp = *it;
    ans = tmp->compute_value();
    tmp->adjoint = 0;
    }
f = ans;

/// Set adjoint for variables
for (variables_iterator_type it=variables.begin(); it != variables.end(); it++)
    (*it)->adjoint = 0;

/// Compute reverse AD
std::list<NumericValue*>::reverse_iterator rit=build.rbegin();
(*rit)->adjoint = 1;    // SEED VALUE FOR AD
for ( ; rit != build.rend(); rit++)
    (*rit)->compute_adjoint();

/// Retrieve adjoint from variables
int j=0;
for (variables_iterator_type it=variables.begin(); it != variables.end(); it++)
    df[j++] = (*it)->adjoint;
}


void Model1::_compute_c(std::vector<double>& c)
{
assert((inequalities.size() + equalities.size()) == c.size());

int i=objectives.size();
for (unsigned int j=0; j<c.size(); j++, i++) {
    std::list<NumericValue*>& build = builds_f[i];
    double ans = 0.0;
    for (std::list<NumericValue*>::iterator it=build.begin(); it != build.end(); it++) {
        NumericValue* tmp = *it;
        ans = tmp->compute_value();
        }
    c[j] = ans;
    }
}

void Model1::_compute_dc(std::vector<double>& dc, unsigned int i)
{
assert(variables.size() == dc.size());
assert(i < (inequalities.size() + equalities.size()));
std::list<NumericValue*>& build = builds_f[objectives.size() + i];

/// _compute_c + set adjoint=0
double ans = 0.0;
for (std::list<NumericValue*>::iterator it=build.begin(); it != build.end(); it++) {
    NumericValue* tmp = *it;
    ans = tmp->compute_value();
    tmp->adjoint = 0;
    }
//c[i] = ans;

/// Set adjoint for variables
for (variables_iterator_type it=variables.begin(); it != variables.end(); it++)
    (*it)->adjoint = 0;

/// Compute reverse AD
std::list<NumericValue*>::reverse_iterator rit=build.rbegin();
(*rit)->adjoint = 1;    // SEED VALUE FOR AD
for ( ; rit != build.rend(); rit++)
    (*rit)->compute_adjoint();

/// Retrieve adjoint from variables
int j=0;
for (variables_iterator_type it=variables.begin(); it != variables.end(); it++)
    dc[j++] = (*it)->adjoint;
}


void Model1::_compute_Hv(std::vector<double>& v, std::vector<double>& Hv, unsigned int i)
{
assert(i < builds_f.size());
assert(variables.size() == v.size());
assert(variables.size() == Hv.size());
std::list<NumericValue*>& build = builds_f[i];

/// Set adjoint for variables
int j=0;
for (variables_iterator_type it=variables.begin(); it != variables.end(); it++) {
    (*it)->adjoint = 0;
    (*it)->dO = v[j++];
    std::cout << "Value " << (*it)->_value << std::endl;
    std::cout << "  v[" << (j-1) << "] = " << v[j-1] << std::endl;
    }

/// Compute forward AD
for (std::list<NumericValue*>::iterator it=build.begin(); it != build.end(); it++) {
    (*it)->compute_hv_fwd();
    (*it)->print(std::cout);
    std::cout << std::endl << (*it)->dO << std::endl;
    }

/// Compute reverse AD
std::list<NumericValue*>::reverse_iterator rit=build.rbegin();
(*rit)->adjoint = 1;    // SEED VALUE FOR AD
(*rit)->adO = 1;    // SEED VALUE FOR AD
for ( ; rit != build.rend(); rit++) {
    (*rit)->print(std::cout);
    std::cout << std::endl << "adjoint=" << (*rit)->adjoint << " adO=" << (*rit)->adO << std::endl;
    (*rit)->compute_hv_back();
    }

/// Retrieve adjoint from variables
j=0;
for (variables_iterator_type it=variables.begin(); it != variables.end(); it++) {
    Hv[j++] = (*it)->adjoint;
    std::cout << "x[" << (*it)->index << "]  adjoint=" << (*it)->adjoint << " adO=" << (*it)->adO << std::endl;
    }
}


void Model1::print(std::ostream& ostr, int df)
{
Model::print(ostr, df);

if ((variables.size() > 0) && df) {
    ostr << std::endl;

    ostr << "  DF" << std::endl;
    int k = 0;
    for (std::list<Expression*>::iterator it=objectives.begin(); it != objectives.end(); ++it) {
        std::map<Variable*, NumericValue*> diff;
        symbolic_diff_all(*it,  diff);
        ostr << "    (Objective " << k << ")" << std::endl;
        for (variables_iterator_type IT=variables.begin(); IT != variables.end(); IT++) {
            ostr << "    ";
            (*IT)->print(ostr);
            ostr << " :  ";
            if (diff.find(*IT) == diff.end())
                ostr << "0" << std::endl;
            else {
                diff[*IT]->print(ostr);
                ostr << std::endl;
                }
            }
        ostr << std::endl;
        k++;
        }

    ostr << "  DC" << std::endl;
    k = 0;
    for (std::list<Expression*>::iterator it=inequalities.begin(); it != inequalities.end(); ++it) {
        std::map<Variable*, NumericValue*> diff;
        InequalityExpression* tmp = static_cast<InequalityExpression*>(*it);
        symbolic_diff_all(tmp->body,  diff);
        ostr << "    (Inequality " << k << ")" << std::endl;
        for (variables_iterator_type IT=variables.begin(); IT != variables.end(); IT++) {
            ostr << "    ";
            (*IT)->print(ostr);
            ostr << " :  ";
            if (diff.find(*IT) == diff.end())
                ostr << "0" << std::endl;
            else {
                diff[*IT]->print(ostr);
                ostr << std::endl;
                }
            }
        ostr << std::endl;
        k++;
        }

    ostr << "  DC" << std::endl;
    k = 0;
    for (std::list<Expression*>::iterator it=equalities.begin(); it != equalities.end(); ++it) {
        std::map<Variable*, NumericValue*> diff;
        EqualityExpression* tmp = static_cast<EqualityExpression*>(*it);
        symbolic_diff_all(tmp->body,  diff);
        ostr << "    (Equality " << k << ")" << std::endl;
        for (variables_iterator_type IT=variables.begin(); IT != variables.end(); IT++) {
            ostr << "    ";
            (*IT)->print(ostr);
            ostr << " :  ";
            if (diff.find(*IT) == diff.end())
                ostr << "0" << std::endl;
            else {
                diff[*IT]->print(ostr);
                ostr << std::endl;
                }
            }
        ostr << std::endl;
        k++;
        }

    }
}

