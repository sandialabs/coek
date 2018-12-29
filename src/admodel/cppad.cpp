#include "model/simple.hpp"
#include "admodel/cppad.hpp"

#include <cppad/cppad.hpp>


int CppAD_ADModel::num_variables()
{ return ax.size(); }

void CppAD_ADModel::set_variables(std::vector<double>& x)
{
std::vector<double>::iterator xit = x.begin();
std::vector< CppAD::AD<double> >::iterator axit = ax.begin();
for ( ; xit != x.end(); xit++, axit++)
    *axit = *xit;
}

/*
void CppAD_ADModel::set_variables(const double* x, int n)
{}
*/

void CppAD_ADModel::print(std::ostream& ostr)
{}

void CppAD_ADModel::build_expression(NumericValue* root, int nb, ordered_variableset_t& vars, std::map<int,CppAD::AD<double> >& vmap)
{
// Walk the expression tree and recreate it using CppAD operators.

std::list<NumericValue*> stack;
stack.push_back(root);
}

void CppAD_ADModel::build(void)
{
builds_f.resize(objectives.size() + inequalities.size() + equalities.size());
int nb=0;
ordered_variableset_t vset(variable_comparator);
std::map<int, CppAD::AD<double> > vmap;

for (std::list<NumericValue*>::iterator it=objectives.begin(); it != objectives.end(); ++  it) {
    build_expression(*it, nb, vset, vmap);
    nb++;
    }

std::vector< ordered_variableset_t* > vars;
vars.resize(inequalities.size() + equalities.size());
size_t i=0;
for (std::list<NumericValue*>::iterator it=inequalities.begin(); it != inequalities.end(); ++it) {
    vars[i] = new ordered_variableset_t(variable_comparator);
    build_expression(*it, nb, *vars[i], vmap);
    i++;
    nb++;
    }

for (std::list<NumericValue*>::iterator it=equalities.begin(); it != equalities.end(); ++  it) {
    vars[i] = new ordered_variableset_t(variable_comparator);
    build_expression(*it, nb, *vars[i], vmap);
    i++;
    nb++;
    }

// Add variables from Jacobian
for (size_t i = 0; i<vars.size(); i++)
    for (ordered_variableset_iterator_t it=vars[i]->begin(); it != vars[i]->end(); it++) {
        vset.insert( *it );
        }

// Create the vector of all variables
std::map<int,int> index_to_id;
variables.resize(vset.size());
std::vector<CppAD::AD<double> >::iterator IT=variables.begin();
i=0;
for (ordered_variableset_iterator_t it=vset.begin(); it != vset.end(); it++) {
    index_to_id[ (*it)->index ] = i++;
    *IT = vmap[(*it)->index];
    IT++;
    }

// Setup Jacobian data structures
J_rc.resize(inequalities.size() + equalities.size());
J.resize(inequalities.size() + equalities.size());
for (size_t i = 0; i<vars.size(); i++) {
    size_t j=0;
    J_rc[i].resize( vars[i]->size() );
    J[i].resize( vars[i]->size() );
    for (ordered_variableset_iterator_t it=vars[i]->begin(); it != vars[i]->end(); it++) {
        J_rc[i][j] = index_to_id[ (*it)->index ];
        J[i][j] = (*it)->_value;
        //std::cout << "J i,j " << i << " " << j << " " << *it << std::endl;
        j++;
        }
    }

// Free memory
for (size_t i=0; i<vars.size(); i++)
    delete vars[i];
}


double CppAD_ADModel::_compute_f(unsigned int i)
{
return 0.0;
}

void CppAD_ADModel::_compute_df(double& f, std::vector<double>& df, unsigned int i)
{}

void CppAD_ADModel::_compute_c(std::vector<double>& c)
{}

void CppAD_ADModel::_compute_dc(std::vector<double>& dc, unsigned int i)
{}

void CppAD_ADModel::_compute_Hv(std::vector<double>& v, std::vector<double>& Hv, unsigned int i)
{}

void CppAD_ADModel::compute_adjoints(unsigned int i)
{}


