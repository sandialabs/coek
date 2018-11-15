#pragma once

#include <vector>
#include <iostream>


//
// Model base class used to manage global data
//
class Model
{
protected:

  virtual ~Model() {}

};


//
// The base class of models defined with expressions
//
template <class ExprManager>
class ExprModel : public Model
{
public:

  typedef typename ExprManager::expr_t expr_t;

  ExprManager manager;
  
  virtual ~ExprModel() {}

  virtual void add_objective(expr_t expr) = 0;

  virtual void add_inequality(expr_t expr) = 0;

  virtual void add_equality(expr_t expr) = 0;

  virtual void print(std::ostream& ostr) = 0;

};



//
// The base class of a model that uses AD
//
class ADModel : public Model
{
public:

    virtual ~ADModel() {}

    virtual int num_variables() = 0;

    virtual void set_variables(std::vector<double>& x) = 0;

    virtual void set_variables(const double* x, int n) =0;

    virtual void print(std::ostream& ostr) = 0;

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

};

