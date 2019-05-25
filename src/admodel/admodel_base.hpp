#pragma once

#include <vector>
#include <iostream>

namespace coek {

namespace base {

//
// The base class of a model that uses AD
//
class ADModel
{
public:

    virtual ~ADModel() {}

    virtual int num_variables() = 0;

    virtual int num_objectives() = 0;

    virtual int num_constraints() = 0;

    virtual int num_nonzeros_Jacobian() = 0;

    virtual void set_variables(std::vector<double>& x) = 0;

    virtual void set_variables(const double* x, int n) =0;

    virtual void print(std::ostream& ostr) = 0;

    /// COMPUTE F

    double compute_f(std::vector<double>& x)
        {
        set_variables(x);
        return _compute_f(0);
        }

    double compute_f(std::vector<double>& x, unsigned int i)
        {
        set_variables(x);
        return _compute_f(i);
        }

    double compute_f(unsigned int i)
        { return _compute_f(i); }

    /// COMPUTE DF

    void compute_df(std::vector<double>& x, std::vector<double>& df)
        {
        set_variables(x);
        double f;
        _compute_df(f, df, 0);
        }

    void compute_df(std::vector<double>& x, std::vector<double>& df, unsigned int i)
        {
        set_variables(x);
        double f;
        _compute_df(f, df, i);
        }

    void compute_df(double& f, std::vector<double>& df)
        { _compute_df(f, df, 0); }

    void compute_df(double& f, std::vector<double>& df, unsigned int i)
        { _compute_df(f, df, i); }

    void compute_df(std::vector<double>& df)
        { double f; _compute_df(f, df, 0); }

    void compute_df(std::vector<double>& df, unsigned int i)
        { double f; _compute_df(f, df, i); }

    /// COMPUTE Hv

    void compute_Hv(std::vector<double>& x, std::vector<double>& v, std::vector<double>& Hv)
        {
        set_variables(x);
        _compute_Hv(v, Hv, 0);
        }

    void compute_Hv(std::vector<double>& x, std::vector<double>& v, std::vector<double>& Hv, unsigned int i)
        {
        set_variables(x);
        _compute_Hv(v, Hv, i);
        }

    void compute_Hv(std::vector<double>& v, std::vector<double>& Hv, unsigned int i)
        { _compute_Hv(v, Hv, i); }

    /// COMPUTE C

    void compute_c(std::vector<double>& x, std::vector<double>& c)
        {
        set_variables(x);
        _compute_c(c);
        }

    void compute_c(std::vector<double>& c)
        { _compute_c(c); }

    /// COMPUTE DC

    void compute_dc(std::vector<double>& x, std::vector<double>& dc, unsigned int i)
        {
        set_variables(x);
        _compute_dc(dc, i);
        }

    void compute_dc(std::vector<double>& dc, unsigned int i)
        { _compute_dc(dc, i); }

    virtual void compute_adjoints(unsigned int i) = 0;

    /// COMPUTE J

    //void compute_J(std::vector<double>& x, std::vector<double>& J)

protected:

    virtual double _compute_f(unsigned int i) = 0;

    virtual void _compute_df(double& f, std::vector<double>& df, unsigned int i) = 0;

    virtual void _compute_Hv(std::vector<double>& v, std::vector<double>& Hv, unsigned int i) = 0;

    virtual void _compute_c(std::vector<double>& c) = 0;

    virtual void _compute_dc(std::vector<double>& dc, unsigned int i) = 0;

};

}

}
