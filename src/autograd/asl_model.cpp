#include "asl_model.hpp"

void ASL_Model::build()
{}

double ASL_Model::compute_f(unsigned int i)
{
return 0.0;
}

void ASL_Model::compute_df(double& f, std::vector<double>& df, unsigned int i)
{}

void ASL_Model::compute_c(std::vector<double>& c)
{}

void ASL_Model::compute_dc(std::vector<double>& dc, unsigned int i)
{}

void ASL_Model::compute_adjoints(unsigned int i)
{}

void ASL_Model::compute_Hv(std::vector<double>& v, std::vector<double>& Hv, unsigned int i)
{}

void ASL_Model::print(std::ostream& ostr, int df)
{}

