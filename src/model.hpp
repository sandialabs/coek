#pragma once

#include <vector>
#include <list>
#include <map>
#include <set>
#include <iostream>

#include "expr_types.hpp"
#include "context.hpp"


//
// The base model class that defines the API used by Python
//
class Model 
{
public:

    ExpressionContext* context;

    std::list<Expression*> objectives;
    std::list<Expression*> inequalities;
    std::list<Expression*> equalities;

    std::vector<Variable*> variables;
    typedef std::set<Variable*, bool(*)(const Variable*, const Variable*)>::iterator variables_iterator_type;
    std::vector<std::vector<int> > J_rc;
    std::vector<std::vector<Variable*> > J;

    Model(void) {}

    int num_variables() {return variables.size();}

    virtual void build() = 0;

    virtual void set_variables(std::vector<double>& x);

    virtual void set_variables(const double* x, int n);

    /// COMPUTE F

    virtual double compute_f(std::vector<double>& x)
        {return compute_f(x, 0);}

    virtual double compute_f(std::vector<double>& x, unsigned int i)
        {
        set_variables(x);
        return compute_f(i);
        }

    virtual double compute_f(unsigned int i) = 0;

    /// COMPUTE DF

    virtual void compute_df(std::vector<double>& x, std::vector<double>& df)
        {compute_df(x, df, 0);}

    virtual void compute_df(std::vector<double>& x, std::vector<double>& df, unsigned int i)
        {
        set_variables(x);
        double f;
        compute_df(f, df, i);
        }

    virtual void compute_df(double& f, std::vector<double>& df, unsigned int i) = 0;

    /// COMPUTE Hv

    virtual void compute_Hv(std::vector<double>& x, std::vector<double>& v, std::vector<double>& Hv)
        {compute_Hv(x, v, Hv, 0);}

    virtual void compute_Hv(std::vector<double>& x, std::vector<double>& v, std::vector<double>& Hv, unsigned int i)
        {
        set_variables(x);
        compute_Hv(v, Hv, 0);
        }

    /// COMPUTE C

    virtual void compute_c(std::vector<double>& x, std::vector<double>& c)
        {
        set_variables(x);
        compute_c(c);
        }

    virtual void compute_c(std::vector<double>& c) = 0;

    /// COMPUTE DC

    virtual void compute_dc(std::vector<double>& x, std::vector<double>& dc, unsigned int i)
        {
        set_variables(x);
        compute_dc(dc, i);
        }

    virtual void compute_dc(std::vector<double>& dc, unsigned int i) = 0;

    virtual void compute_adjoints(unsigned int i) = 0;

    /// COMPUTE J

    //void compute_J(std::vector<double>& x, std::vector<double>& J)

    virtual void compute_Hv(std::vector<double>& v, std::vector<double>& Hv, unsigned int i) = 0;

    virtual void print(std::ostream& ostr, int df);

};

