#pragma once

#include <vector>
#include <cppad/cppad.hpp>
#include "model.hpp"
#include "model/simple.hpp"


//
// An extension model that uses the CppAD library for autograd.
//
class CppAD_ADModel : public ADModel, public Simple_ExprModel
{
public:

    int num_variables();

    int num_contraints();

    void set_variables(std::vector<double>& x);

    //void set_variables(const double* x, int n);

    void print(std::ostream& ostr);

    void build(void);

    void compute_adjoints(unsigned int i);

protected:

    double _compute_f(unsigned int i);

    void _compute_df(double& f, std::vector<double>& df, unsigned int i);

    void _compute_c(std::vector<double>& c);

    void _compute_dc(std::vector<double>& dc, unsigned int i);

    void _compute_Hv(std::vector<double>& v, std::vector<double>& Hv, unsigned int i);

    std::vector< CppAD::AD<double> > ax;
};

