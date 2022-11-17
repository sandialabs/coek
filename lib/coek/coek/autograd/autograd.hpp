#pragma once

#include <string>
#include <vector>

#include "coek/api/expression.hpp"
#include "coek/model/model.hpp"

namespace coek {

class Model;

class NLPModelRepn {
   public:
    Model model;
    std::map<size_t, VariableRepn> used_variables;
    std::map<VariableRepn, size_t> fixed_variables;
    std::map<ParameterRepn, size_t> parameters;

   public:
    NLPModelRepn() {}
    NLPModelRepn(Model& _model) : model(_model) {}
    virtual ~NLPModelRepn() {}

    virtual void initialize(bool sparse_JH = true) = 0;
    virtual void reset(void) = 0;

    virtual size_t num_variables() const = 0;
    virtual size_t num_objectives() const = 0;
    virtual size_t num_constraints() const = 0;
    virtual size_t num_nonzeros_Jacobian() const = 0;
    virtual size_t num_nonzeros_Hessian_Lagrangian() const = 0;

    virtual VariableRepn get_variable(size_t i);
    virtual void set_variable(size_t i, const VariableRepn v);

    virtual void set_variables(std::vector<double>& x) = 0;
    virtual void set_variables(const double* x, size_t n) = 0;

    virtual Objective get_objective(size_t i);
    virtual Constraint get_constraint(size_t i);

    virtual void print_equations(std::ostream& ostr) const = 0;
    virtual void print_values(std::ostream& ostr) const = 0;

    virtual double compute_f(size_t i) = 0;
    virtual void compute_df(double& f, std::vector<double>& df, size_t i) = 0;
    virtual void compute_H(std::vector<double>& w, std::vector<double>& H) = 0;
    virtual void compute_c(std::vector<double>& c) = 0;
    virtual void compute_dc(std::vector<double>& dc, size_t i) = 0;
    virtual void compute_J(std::vector<double>& J) = 0;

    virtual void get_J_nonzeros(std::vector<size_t>& jrow, std::vector<size_t>& jcol) = 0;
    // NOTE: Should we always assume column- or row-major order?
    virtual void get_H_nonzeros(std::vector<size_t>& hrow, std::vector<size_t>& hcol) = 0;

   public:
    void find_used_variables();
};

NLPModelRepn* create_NLPModelRepn(Model& model, const std::string& name);
NLPModelRepn* create_NLPModelRepn(const std::string& name);

}  // namespace coek
