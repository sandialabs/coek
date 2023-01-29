#pragma once

#include <cppad/cppad.hpp>
#include <map>
#include <vector>

#include "autograd.hpp"

namespace coek {

class VariableTerm;

//
// An extension model that uses the CppAD library for autograd.
//
class CppAD_Repn : public NLPModelRepn {
   public:
    // ----------------------------------------------------------------------
    // Problem information
    // ----------------------------------------------------------------------
    /// dimension of the range space for f(x).
    size_t nf;
    /// dimension of the domain space
    size_t nx;
    /// dimension of the range space for c(x)
    size_t nc;
    /// initial value for x
    std::vector<double> xi;
    /// lower limit for x
    std::vector<double> xlb;
    /// upper limit for x
    std::vector<double> xub;
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

    bool invalid_fc;
    std::vector<double> fc_cache;

    std::vector<double> currx;
    std::vector<double> fcw;

    std::vector<CppAD::AD<double> > dynamic_params;
    std::vector<double> dynamic_param_vals;

   public:
    CppAD_Repn(Model& model);

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
    bool column_major_hessian();

    void print_equations(std::ostream& ostr) const;
    void print_values(std::ostream& ostr) const;

   public:
    double compute_f(size_t i);

    void compute_df(double& f, std::vector<double>& df, size_t i);

    void compute_c(std::vector<double>& c);

    void compute_dc(std::vector<double>& dc, size_t i);

    void compute_H(std::vector<double>& w, std::vector<double>& H);

    void compute_J(std::vector<double>& J);

   public:
    void build_expression(expr_pointer_t root, std::vector<CppAD::AD<double> >& ADvars,
                          CppAD::AD<double>& range,
                          std::unordered_map<VariableRepn, size_t>& _used_variables);
};

}  // namespace coek
