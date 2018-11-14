#include "model/simple.hpp"
#include "admodel/cppad.hpp"


template <>
void initialize_admodel(CppAD_ADModel& admodel, Simple_ExprModel& model)
{
// TODO: Walk the coek expression tree, and create a CPPAD expression
}


int CppAD_ADModel::num_variables()
{ return 0; }

void CppAD_ADModel::set_variables(std::vector<double>& x)
{}

void CppAD_ADModel::set_variables(const double* x, int n)
{}

void CppAD_ADModel::print(std::ostream& ostr)
{}

void CppAD_ADModel::build(void)
{}


double CppAD_ADModel::compute_f(unsigned int i)
{
return 0.0;
}

void CppAD_ADModel::compute_df(double& f, std::vector<double>& df, unsigned int i)
{}

void CppAD_ADModel::compute_c(std::vector<double>& c)
{}

void CppAD_ADModel::compute_dc(std::vector<double>& dc, unsigned int i)
{}

void CppAD_ADModel::compute_adjoints(unsigned int i)
{}

void CppAD_ADModel::compute_Hv(std::vector<double>& v, std::vector<double>& Hv, unsigned int i)
{}


