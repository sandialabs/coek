#pragma once

#include <map>
#include <vector>

#include "autograd.hpp"

extern "C" {
struct ASL_pfgh;
}

namespace coek {

class VariableTerm;

//
// An extension model that uses the ASL library for autograd.
//
class ASL_Repn : public NLPModelRepn {
   public:
    // The main ASL structure
    ASL_pfgh* asl_;

    // ----------------------------------------------------------------------
    // Problem information
    // ----------------------------------------------------------------------
    /// Dimension of the range space for f(x).
    size_t nf;
    /// Dimension of the domain space
    size_t nx;
    /// Dimension of the range space for c(x)
    size_t nc;
    /// Number of nonzeros in the Jacobian
    size_t nnz_jac_g;
    /// Number of nonzeros in Hessian of the Lagrangian
    size_t nnz_lag_h;

    /// initial value for x
    std::vector<double> xi;
    /// lower limit for x
    std::vector<double> xlb;
    /// upper limit for x
    std::vector<double> xub;

    bool objval_called_with_current_x_;
    bool conval_called_with_current_x_;
    double f_cache;
    std::vector<double> c_cache;
    std::vector<double> currx;

   public:
    std::map<size_t, size_t> varmap;  // ASL index -> NLPModelRepn index
    VariableRepn get_variable(size_t i);

   public:
    ASL_Repn(Model& model);
    ~ASL_Repn();

    void initialize(bool sparse_JH = true);

    void reset(void);

    size_t num_variables() const;

    size_t num_objectives() const;

    size_t num_constraints() const;

    size_t num_nonzeros_Jacobian() const;

    size_t num_nonzeros_Hessian_Lagrangian() const;

    void set_variables(std::vector<double>& x);
    void set_variables(const double* x, size_t n);

    void get_J_nonzeros(std::vector<size_t>& jrow, std::vector<size_t>& jcol);
    void get_H_nonzeros(std::vector<size_t>& hrow, std::vector<size_t>& hcol);

    void print_equations(std::ostream& ostr) const;
    void print_values(std::ostream& ostr) const;

   public:
    double compute_f(size_t i);

    void compute_df(double& f, std::vector<double>& df, size_t i);

    void compute_c(std::vector<double>& c);

    void compute_dc(std::vector<double>& dc, size_t i);

    void compute_H(std::vector<double>& w, std::vector<double>& H);

    void compute_J(std::vector<double>& J);

   protected:
    void* nerror_;

    void free_asl();
    void alloc_asl();
    bool nerror_ok;
    bool check_asl_status(void* nerror);
    void call_hesset();
};

}  // namespace coek
