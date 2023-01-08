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

#if 0
    /// lower limit for c(x)
    // std::vector<double>     glb;
    /// upper limit for c(x)
    // std::vector<double>     gub;
    /// object that evaluates f(x) and c(x)
    CppAD::ADFun<double> ADfc;

    // ----------------------------------------------------------------------
    // Jacobian information
    // ----------------------------------------------------------------------
    /// Should sparse methods be used to compute Jacobians and Hessians.
    bool sparse_JH;
    /// Sparsity pattern for Jacobian of [f(x), g(x) ].
    /// If sparse_JH is true, this pattern set by constructor and does not change.
    /// Otherwise this vector has size zero.
    CppAD::vectorBool jac_pattern;
    /// Row indices of [f(x), g(x)] for Jacobian of g(x) in row order.
    /// (Set by constructor and not changed.)
    CppAD::vector<size_t> jac_row;
    /// Column indices for Jacobian of g(x), same order as row_jac_.
    /// (Set by constructor and not changed.)
    CppAD::vector<size_t> jac_col;
    /// col_order_jac_ sorts row_jac_ and col_jac_ in column order.
    /// (Set by constructor and not changed.)
    CppAD::vector<size_t> jac_col_order;
    /// Work vector used by SparseJacobian, stored here to avoid recalculation.
    CppAD::sparse_jacobian_work jac_work;

    // ----------------------------------------------------------------------
    // Hessian information
    // ----------------------------------------------------------------------
    /// Sparsity pattern for Hessian of Lagragian
    /// \f[ L(x) = \sigma \sum_i f_i (x) + \sum_i \lambda_i  g_i (x) \f]
    /// If sparse_JH is true, this pattern set by constructor and does not change.
    /// Otherwise this vector has size zero.
    CppAD::vectorBool hes_pattern;
    /// Row indices of Hessian lower left triangle in row order.
    /// (Set by constructor and not changed.)
    CppAD::vector<size_t> hes_row;
    /// Column indices of Hessian left triangle in same order as row_hes_.
    /// (Set by constructor and not changed.)
    CppAD::vector<size_t> hes_col;
    /// Work vector used by SparseJacobian, stored here to avoid recalculation.
    CppAD::sparse_hessian_work hes_work;


    std::vector<double> fcw;

    std::vector<CppAD::AD<double> > dynamic_params;
    std::vector<double> dynamic_param_vals;
#endif

    bool objval_called_with_current_x_;
    bool conval_called_with_current_x_;
    double f_cache;
    std::vector<double> c_cache;
    std::vector<double> currx;

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
