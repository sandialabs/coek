#pragma once

#include "model.hpp"
#include "model/simple.hpp"


//
// An extension model that uses the CppAD library for autograd.
//
class CppAD_ADModel : public ADModel
{
public:

    int num_variables();

    void set_variables(std::vector<double>& x);

    void set_variables(const double* x, int n);

    void print(std::ostream& ostr);


    double compute_f(unsigned int i);

    void compute_df(double& f, std::vector<double>& df, unsigned int i);

    void compute_c(std::vector<double>& c);

    void compute_dc(std::vector<double>& dc, unsigned int i);

    void compute_adjoints(unsigned int i);

    void compute_Hv(std::vector<double>& v, std::vector<double>& Hv, unsigned int i);

};

template <>
void initialize_admodel(CppAD_ADModel& admodel, Simple_ExprModel& model);

