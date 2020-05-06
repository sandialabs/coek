#pragma once

#include <string>
#include <vector>
#include "coek_model.hpp"

namespace coek {

class Model;


class NLPModelRepn
{
public:

    Model model;
    std::map<int,VariableTerm*> used_variables;
    std::map<VariableTerm*,int> fixed_variables;
    std::map<ParameterTerm*,int> parameters;

public:

    NLPModelRepn(Model& _model)
        : model(_model) {}
    virtual ~NLPModelRepn() {}

    virtual void initialize(bool sparse_JH=true) = 0;
    virtual void reset(void) = 0;

    virtual size_t num_variables() const = 0;
    virtual size_t num_objectives() const = 0;
    virtual size_t num_constraints() const = 0;
    virtual size_t num_nonzeros_Jacobian() const = 0;
    virtual size_t num_nonzeros_Hessian_Lagrangian() const = 0;

    virtual VariableTerm* get_variable(int i);
    virtual void set_variable(int i, const VariableTerm* v);

    virtual void set_variables(std::vector<double>& x) = 0;
    virtual void set_variables(const double* x, size_t n) = 0;

    virtual Constraint get_constraint(int i);

    virtual void print_summary(std::ostream& ostr) const = 0;

    virtual double compute_f(unsigned int i) = 0;
    virtual void compute_df(double& f, std::vector<double>& df, unsigned int i) = 0;
    virtual void compute_H(std::vector<double>& w, std::vector<double>& H) = 0;
    virtual void compute_c(std::vector<double>& c) = 0;
    virtual void compute_dc(std::vector<double>& dc, unsigned int i) = 0;
    virtual void compute_J(std::vector<double>& J) = 0;

    virtual void get_J_nonzeros(std::vector<size_t>& jrow, std::vector<size_t>& jcol) = 0;
    // NOTE: Should we always assume column- or row-major order?
    virtual void get_H_nonzeros(std::vector<size_t>& hrow, std::vector<size_t>& hcol) = 0;

public:

    void find_used_variables();
};


NLPModelRepn* create_NLPModelRepn(Model& model, std::string& name);

}
