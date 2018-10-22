#include "cppad_model.hpp"

void CppAD_Model::build()
{}

double CppAD_Model::compute_f(unsigned int i)
{
return 0.0;
}

void CppAD_Model::compute_df(double& f, std::vector<double>& df, unsigned int i)
{}

void CppAD_Model::compute_c(std::vector<double>& c)
{}

void CppAD_Model::compute_dc(std::vector<double>& dc, unsigned int i)
{}

void CppAD_Model::compute_adjoints(unsigned int i)
{}

void CppAD_Model::compute_Hv(std::vector<double>& v, std::vector<double>& Hv, unsigned int i)
{}

void CppAD_Model::print(std::ostream& ostr, int df)
{}

