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

void CppAD_ADModel::build(void)
{}


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


