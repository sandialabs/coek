#include <unordered_map>

#include "../ast/base_terms.hpp"
#include "../ast/constraint_terms.hpp"
#include "../ast/expr_terms.hpp"
#include "../ast/value_terms.hpp"
#include "../ast/visitor_fns.hpp"
#include "coek/api/expression.hpp"
#include "coek/api/constraint.hpp"
#include "coek/api/objective.hpp"
#include "coek/model/model.hpp"
#include "coek/model/model_repn.hpp"
#include "cppad_repn.hpp"

namespace coek {

CppAD_Repn::CppAD_Repn(Model& model) : NLPModelRepn(model)
{
    nx = 0;
    nf = 0;
    nc = 0;
    invalid_fc = true;
    sparse_JH = true;
}

size_t CppAD_Repn::num_variables() const { return nx; }

size_t CppAD_Repn::num_objectives() const { return nf; }

size_t CppAD_Repn::num_constraints() const { return nc; }

size_t CppAD_Repn::num_nonzeros_Jacobian() const { return jac_row.size(); }

size_t CppAD_Repn::num_nonzeros_Hessian_Lagrangian() const { return hes_row.size(); }

void CppAD_Repn::set_variables(std::vector<double>& x)
{
    assert(x.size() == currx.size());
    auto cit = currx.begin();
    auto xit = x.begin();
    for (; cit != currx.end(); ++cit, ++xit)
        *cit = *xit;

    invalid_fc = true;
}

void CppAD_Repn::set_variables(const double* x, size_t n)
{
    assert(n == currx.size());
    for (size_t i = 0; i < n; i++)
        currx[i] = x[i];

    invalid_fc = true;
}

void CppAD_Repn::get_J_nonzeros(std::vector<size_t>& jrow, std::vector<size_t>& jcol)
{
    jrow.resize(jac_row.size());
    jcol.resize(jac_col.size());

    for (size_t i = 0; i < jac_row.size(); i++) {
        jrow[i] = jac_row[i] - 1;
        jcol[i] = jac_col[i];
    }
}

void CppAD_Repn::get_H_nonzeros(std::vector<size_t>& hrow, std::vector<size_t>& hcol)
{
    hrow.resize(hes_row.size());
    hcol.resize(hes_col.size());

    for (size_t i = 0; i < hes_row.size(); i++) {
        hrow[i] = hes_row[i];
        hcol[i] = hes_col[i];
    }
}

bool CppAD_Repn::column_major_hessian() { return false; }

void CppAD_Repn::print_equations(std::ostream& ostr) const { NLPModelRepn::print_equations(ostr); }

void CppAD_Repn::print_values(std::ostream& ostr) const { NLPModelRepn::print_values(ostr); }

double CppAD_Repn::compute_f(size_t i)
{
    assert(i < nf);
    if (invalid_fc) {
        fc_cache = ADfc.Forward(0, currx);
        invalid_fc = false;
    }
    return fc_cache[i];
}

void CppAD_Repn::compute_df(double& f, std::vector<double>& df, size_t i)
{
    assert(df.size() == nx);

    f = compute_f(i);
    fcw[i] = 1;
    auto dy = ADfc.Reverse(1, fcw);
    fcw[i] = 0;
    for (size_t j = 0; j < df.size(); j++)
        df[j] = dy[j];
}

void CppAD_Repn::compute_c(std::vector<double>& c)
{
    assert(c.size() == nc);
    if (invalid_fc) {
        fc_cache = ADfc.Forward(0, currx);
        invalid_fc = false;
    }
    for (size_t i = 0; i < c.size(); i++)
        c[i] = fc_cache[nf + i];
}

void CppAD_Repn::compute_dc(std::vector<double>& dc, size_t i)
{
    assert(i < fcw.size());
    assert(dc.size() == nx);

    if (invalid_fc) {
        fc_cache = ADfc.Forward(0, currx);
        invalid_fc = false;
    }
    fcw[nf + i] = 1;
    auto dy = ADfc.Reverse(1, fcw);
    fcw[nf + i] = 0;
    for (size_t j = 0; j < dc.size(); j++)
        dc[j] = dy[j];
}

void CppAD_Repn::compute_H(std::vector<double>& w, std::vector<double>& H)
{
#if 0
if (invalid_fc) {
    fc_cache = ADfc.Forward(0, currx);
    invalid_fc = false;
    }
#endif
    if (sparse_JH) {
        //
        // Sparse Hessian
        //
        ADfc.SparseHessian(currx, w, hes_pattern, hes_row, hes_col, H, hes_work);
    }
    else {
        //
        // Dense Hessian
        //
        auto hes = ADfc.Hessian(currx, w);
        for (size_t k = 0; k < hes_row.size(); k++) {
            size_t i = hes_row[k];
            size_t j = hes_col[k];
            H[k] = hes[i * nx + j];
        }
    }
}

void CppAD_Repn::compute_J(std::vector<double>& J)
{
    if (sparse_JH) {
        //
        // Sparse Jacobian calculation
        //
        if (nx < nc) {
            // Forward
            ADfc.SparseJacobianForward(currx, jac_pattern, jac_row, jac_col, J, jac_work);
        }
        else {
            // Reverse
            ADfc.SparseJacobianReverse(currx, jac_pattern, jac_row, jac_col, J, jac_work);
        }
    }

    else {
        //
        // Dense Jacobian
        //
        if (invalid_fc) {
            //
            // Unlike the sparse case, we need to explicitly initialize CppAD with its
            // forward function calculation.
            //
            fc_cache = ADfc.Forward(0, currx);
            invalid_fc = false;
        }
        if (nx < nc) {
            // Forward
            std::vector<double> x1(nx), fg1(nf + nc);
            for (size_t j = 0; j < nx; j++)
                x1[j] = 0.0;
            // index in col_order_jac_ of next entry
            size_t ell = 0;
            size_t k = jac_col_order[ell];
            size_t nk = jac_col.size();
            for (size_t j = 0; j < nx; j++) {
                // compute j-th column of Jacobian of g(x)
                x1[j] = 1.0;
                fg1 = ADfc.Forward(1, x1);
                while (ell < nk && jac_col[k] <= j) {
                    CPPAD_ASSERT_UNKNOWN(jac_col[k] == j);
                    size_t i = jac_row[k];
                    CPPAD_ASSERT_UNKNOWN(i >= nf)
                    J[k] = fg1[i];
                    ell++;
                    if (ell < nk)
                        k = jac_col_order[ell];
                }
                x1[j] = 0.0;
            }
        }
        else {
            // Reverse
            size_t nfc = nf + nc;
            // user reverse mode
            std::vector<double> w(nfc), dw(nx);
            for (size_t i = 0; i < nfc; i++)
                w[i] = 0.0;
            // index in jac_row of next entry
            size_t k = 0;
            size_t nk = jac_row.size();
            for (size_t i = nf; i < nfc; i++) {
                // compute i-th row of Jacobian of g(x)
                w[i] = 1.0;
                dw = ADfc.Reverse(1, w);
                while (k < nk && jac_row[k] <= i) {
                    CPPAD_ASSERT_UNKNOWN(jac_row[k] == i);
                    size_t j = jac_col[k];
                    J[k] = dw[j];
                    k++;
                }
                w[i] = 0.0;
            }
        }
    }
}

void CppAD_Repn::create_CppAD_function()
{
    if (simplify_expressions) {
        dynamic_params.resize(0);
        dynamic_param_vals.resize(0);
    }
    else {
        dynamic_params.resize(fixed_variables.size() + parameters.size());
        dynamic_param_vals.resize(fixed_variables.size() + parameters.size());
    }

    //
    // Create the CppAD function
    //
    std::unordered_map<VariableRepn, size_t> _used_variables;
    for (auto& it : used_variables)
        _used_variables[it.second] = it.first;

    std::vector<CppAD::AD<double> > ADvars(nx);
    std::vector<CppAD::AD<double> > ADrange(nf + nc);
    if (dynamic_params.size() > 0)
        CppAD::Independent(ADvars, 0, false, dynamic_params);
    else
        CppAD::Independent(ADvars);

    try {
        if (simplify_expressions) {
            std::map<std::shared_ptr<SubExpressionTerm>, expr_pointer_t> cache;

            size_t nb = 0;
            for (auto& it : model.repn->objectives) {
                if (it.active()) {
                    build_expression(simplify_expr(it.repn, cache), ADvars, ADrange[nb],
                                     _used_variables);
                    nb++;
                }
            }

            nb = 0;
            for (auto& it : model.repn->constraints) {
                if (it.active()) {
                    build_expression(simplify_expr(it.repn, cache), ADvars, ADrange[nf + nb],
                                     _used_variables);
                    nb++;
                }
            }
        }
        else {
            size_t nb = 0;
            for (auto& it : model.repn->objectives) {
                if (it.active()) {
                    build_expression(it.repn, ADvars, ADrange[nb], _used_variables);
                    nb++;
                }
            }

            nb = 0;
            for (auto& it : model.repn->constraints) {
                if (it.active()) {
                    build_expression(it.repn, ADvars, ADrange[nf + nb], _used_variables);
                    nb++;
                }
            }
        }
    }
    catch (std::runtime_error& err) {
        ADfc.Dependent(ADvars, ADrange);
        throw err;
    }
    ADfc.Dependent(ADvars, ADrange);
    ADfc.optimize();
}

void CppAD_Repn::initialize()
{
    //
    // Find all variables used in the NLP model
    //
    find_used_variables();
    nx = used_variables.size();
    nf = model.num_active_objectives();
    nc = model.num_active_constraints();

    if (nx == 0)
        return;

    create_CppAD_function();

    //
    // Setup temporary arrays used during computations
    //
    size_t nfc = nf + nc;
    fc_cache.resize(nfc);
    currx.resize(nx);
    fcw.assign(nfc, 0.0);

    if (nc > 0) {
        //
        // Setup Jacobian calculations
        //
        if (sparse_JH) {
            //
            // Sparse Jacobian
            //
            // Compute jac_pattern
            //
            jac_pattern.resize(nfc * nx);
            if (nx <= nfc) {
                //
                // Use forward mode to compute sparsity
                //

#if 0
            // number of bits that are packed into one unit in vectorBool
            size_t n_column = CppAD::vectorBool::bit_per_unit();

            // sparsity patterns for current columns
            CppAD::vectorBool r(nx * n_column), s(nc * n_column);

            // compute the sparsity pattern n_column columns at a time
            size_t n_loop = (nx - 1) / n_column + 1;
            for(size_t i_loop = 0; i_loop < n_loop; i_loop++) {   // starting column index for this iteration
                size_t i_column = i_loop * n_column;

                // pattern that picks out the appropriate columns
                for(size_t i = 0; i < nx; i++) {
                    for(size_t j = 0; j < n_column; j++)
                        r[i*n_column + j] = (i == i_column + j);
                    }

                s = ADfc.ForSparseJac(n_column, r);

                // fill in the corresponding columns of total_sparsity
                for(size_t i = 0; i < nc; i++) {
                    for(size_t j = 0; j < n_column; j++) {
                        if( i_column + j < nx  )
                            jac_pattern[i*nx  + i_column + j] = s[i*n_column + j];
                        }
                    }
                }
#else
                // Identity marix
                CppAD::vectorBool r(nx * nx);
                for (size_t i = 0; i < nx; i++)
                    for (size_t j = 0; j < nx; j++)
                        if (i == j)
                            r[i * nx + j] = true;
                        else
                            r[i * nx + j] = false;
                auto s = ADfc.ForSparseJac(nx, r);

                // fill in the corresponding columns of total_sparsity
                for (size_t i = 0; i < nf; i++) {
                    for (size_t j = 0; j < nx; j++)
                        jac_pattern[i * nx + j] = 0;
                }
                for (size_t i = nf; i < nfc; i++) {
                    for (size_t j = 0; j < nx; j++)
                        jac_pattern[i * nx + j] = s[i * nx + j];
                }
#endif
            }
            else {
                //
                // Use reverse mode to compute sparsity
                //

#if 0
            // number of bits that are packed into one unit in vectorBool
            size_t n_row = CppAD::vectorBool::bit_per_unit();

            // sparsity patterns for current rows
            CppAD::vectorBool r(n_row * nc), s(n_row * nx);

            // compute the sparsity pattern n_row row at a time
            size_t n_loop = (nc - 1) / n_row + 1;
            for(size_t i_loop = 0; i_loop < n_loop; i_loop++) {   // starting row index for this iteration
                size_t i_row = i_loop * n_row;

                // pattern that picks out the appropriate rows
                for(size_t i = 0; i < n_row; i++) {
                    for(size_t j = 0; j < nc; j++)
                        r[i*nc + j] = (i_row + i ==  j);
                    }
                s = ADfc.RevSparseJac(n_row, r);

                // fill in correspoding rows of total sparsity
                for(size_t i = 0; i < n_row; i++) {
                    for(size_t j = 0; j < nx; j++)
                        if( i_row + i < nc )
                            jac_pattern[(i_row + i)*nx + j] = s[i*nx + j];
                    }
                }
#else
                // sparsity patterns for current rows
                CppAD::vectorBool r(nfc * nfc);  //, s(nx * nc);

                // R is the identity matrix
                for (size_t i = 0; i < nfc * nfc; i++)
                    r[i] = false;
                for (size_t i = nf; i < nfc; i++)
                    r[i * nfc + i] = true;
                auto s = ADfc.RevSparseJac(nfc, r);

                // fill in correspoding rows of total sparsity
                for (size_t i = 0; i < nf; i++) {
                    for (size_t j = 0; j < nx; j++)
                        jac_pattern[i * nx + j] = 0;
                }
                for (size_t i = nf; i < nfc; i++) {
                    for (size_t j = 0; j < nx; j++)
                        jac_pattern[i * nx + j] = s[i * nx + j];
                }
#endif
            }
            //
            // Row-major indices for Jacobian of c(x).
            //
            for (size_t i = nf; i < nfc; i++) {
                for (size_t j = 0; j < nx; j++) {
                    if (jac_pattern[i * nx + j]) {
                        jac_row.push_back(i);
                        jac_col.push_back(j);
                    }
                }
            }
        }
        else {
            //
            // Dense Jacobian
            //
            // Row-major indices for Jacobian of c(x).
            //
            for (size_t i = nf; i < nfc; i++) {
                for (size_t j = 0; j < nx; j++) {
                    jac_row.push_back(i);
                    jac_col.push_back(j);
                }
            }
        }

        // Column order indirect sort of the Jacobian indices
        jac_col_order.resize(jac_col.size());
        index_sort(jac_col, jac_col_order);
    }

    if (sparse_JH) {
        //
        // Sparse Hessian
        //
        // Compute hes_pattern
        //
        size_t nfc = nf + nc;
        hes_pattern.resize(nx * nx);

#if 0
    // sparsity patterns for current columns
    CppAD::vectorBool r(nx * n_column), h(nx * n_column);

    // number of bits that are packed into one unit in vectorBool
    size_t n_column = CppAD::vectorBool::bit_per_unit();

    // sparsity pattern for range space of function
    CppAD::vectorBool s(m);
    for(size_t i = 0; i < m; i++)
        s[i] = true;

    // compute the sparsity pattern n_column columns at a time
    size_t n_loop = (nx - 1) / n_column + 1;
    for(size_t i_loop = 0; i_loop < n_loop; i_loop++) {
        // starting column index for this iteration
        size_t i_column = i_loop * n_column;

        // pattern that picks out the appropriate columns
        for(size_t i = 0; i < nx; i++) {
            for(size_t j = 0; j < n_column; j++)
                r[i * n_column + j] = (i == i_column + j);
            }
        ////adfun_.ForSparseJac(n_column, r);

        // sparsity pattern corresponding to paritls w.r.t. (theta, u)
        // of partial w.r.t. the selected columns
        bool transpose = true;
        ////h = adfun_.RevSparseHes(n_column, s, transpose);

        // fill in the corresponding columns of total_sparsity
        for(size_t i = 0; i < nx; i++) {
            for(size_t j = 0; j < n_column; j++) {
                if( i_column + j < nx )
                    hes_pattern[i * nx + i_column + j] = h[i * n_column + j];
                }
            }
        }
#else
        // Identity matrix
        CppAD::vectorBool r(nx * nx);  //, h(nx * n_column);
        for (size_t i = 0; i < nx; i++)
            for (size_t j = 0; j < nx; j++)
                if (i == j)
                    r[i * nx + j] = true;
                else
                    r[i * nx + j] = false;
        ADfc.ForSparseJac(nx, r);

        // sparsity pattern corresponding to parials w.r.t. (theta, u)
        // of partial w.r.t. the selected columns
        bool transpose = true;
        // sparsity pattern for range space of function
        CppAD::vectorBool s(nfc);
        for (size_t i = 0; i < nfc; i++)
            s[i] = true;
        auto h = ADfc.RevSparseHes(nx, s, transpose);

        // fill in the corresponding columns of total_sparsity
        for (size_t i = 0; i < nx; i++) {
            for (size_t j = 0; j <= i; j++)
                hes_pattern[i * nx + j] = h[i * nx + j];
            // TODO - Why is CppAD looking at the pattern in a non-symmetric manner?
            for (size_t j = i + 1; j < nx; j++)
                hes_pattern[i * nx + j] = 0;
        }
#endif
        //
        // Set row and column indices for Lower triangle of Hessian
        // of Lagragian.  These indices are in row major order.
        //
        for (size_t i = 0; i < nx; i++) {
            for (size_t j = 0; j <= i; j++) {
                if (hes_pattern[i * nx + j]) {
                    hes_row.push_back(i);
                    hes_col.push_back(j);
                }
            }
        }
    }

    else {
        //
        // Dense Hessian
        //
        // Row-major indices for lower triangular Hessian.
        //
        for (size_t i = 0; i < nx; i++) {
            for (size_t j = 0; j <= i; j++) {
                hes_row.push_back(i);
                hes_col.push_back(j);
            }
        }
    }

    if (sparse_JH)
        hes_work.color_method = "cppad.symmetric";

    reset();
}

void CppAD_Repn::reset(void)
{
    if (nx == 0)
        return;

    //
    // Initialize the CppAD dynamic parameters
    //
    if (not simplify_expressions) {
        for (auto& it : fixed_variables)
            dynamic_param_vals[it.second] = it.first->value->eval();
        for (auto& it : parameters)
            dynamic_param_vals[it.second] = it.first->value->eval();
        ADfc.new_dynamic(dynamic_param_vals);
    }
    else {
        // Regenerate CppAD data structures when simplifying the expressions
        create_CppAD_function();
    }

    //
    // Setup initial value
    //
    xlb.resize(used_variables.size());
    xub.resize(used_variables.size());
    for (auto& it : used_variables) {
        currx[it.first] = it.second->value->eval();
        xlb[it.first] = it.second->lb->eval();
        xub[it.first] = it.second->ub->eval();
    }
    set_variables(currx);
}

bool CppAD_Repn::get_option(const std::string& option, bool& value) const
{
    if (option == "sparse_JH") {
        value = sparse_JH;
        return true;
    }
    else if (option == "simplify_expressions") {
        value = simplify_expressions;
        return true;
    }
    return false;
}

bool CppAD_Repn::get_option(const std::string& option, int& value) const
{
    if (option == "sparse_JH") {
        value = sparse_JH;
        return true;
    }
    else if (option == "simplify_expressions") {
        value = simplify_expressions;
        return true;
    }
    return false;
}

void CppAD_Repn::set_option(const std::string& option, bool value)
{
    if (option == "sparse_JH")
        sparse_JH = value;
    else if (option == "simplify_expressions")
        simplify_expressions = value;
}

void CppAD_Repn::set_option(const std::string& option, int value)
{
    if (option == "sparse_JH")
        sparse_JH = (value == 1);
    else if (option == "simplify_expressions")
        simplify_expressions = (value == 1);
}

//
// This empty namespace contains functions used to walk the COEK
// expression tree.  The CppAD expression is accumulated in the 'ans'
// value.
//
namespace {

class VisitorData {
   public:
    std::unordered_map<expr_pointer_t, CppAD::AD<double> > cache;

    std::vector<CppAD::AD<double> >& ADvars;
    std::unordered_map<VariableRepn, size_t>& used_variables;
    std::map<VariableRepn, size_t>& fixed_variables;
    std::map<ParameterRepn, size_t>& parameters;
    std::vector<CppAD::AD<double> >& dynamic_params;

    VisitorData(std::vector<CppAD::AD<double> >& ADvars_,
                std::unordered_map<VariableRepn, size_t>& used_variables_,
                std::map<VariableRepn, size_t>& fixed_variables_,
                std::map<ParameterRepn, size_t>& parameters_,
                std::vector<CppAD::AD<double> >& dynamic_params_)
        : ADvars(ADvars_),
          used_variables(used_variables_),
          fixed_variables(fixed_variables_),
          parameters(parameters_),
          dynamic_params(dynamic_params_)
    {
    }
};

void visit_expression(expr_pointer_t& expr, VisitorData& data, CppAD::AD<double>& ans);

void visit_ConstantTerm(expr_pointer_t& expr, VisitorData& /*data*/, CppAD::AD<double>& ans)
{
    auto tmp = std::dynamic_pointer_cast<ConstantTerm>(expr);
    if (tmp->value != 0)
        ans += tmp->value;
}

void visit_ParameterTerm(expr_pointer_t& expr, VisitorData& data, CppAD::AD<double>& ans)
{
    auto tmp = std::dynamic_pointer_cast<ParameterTerm>(expr);
    ans += data.dynamic_params[data.parameters[tmp]];
}

void visit_VariableTerm(expr_pointer_t& expr, VisitorData& data, CppAD::AD<double>& ans)
{
    auto tmp = std::dynamic_pointer_cast<VariableTerm>(expr);
    if (tmp->fixed)
        ans += data.dynamic_params[data.fixed_variables[tmp]];
    else
        ans += data.ADvars[data.used_variables[tmp]];
}

void visit_MonomialTerm(expr_pointer_t& expr, VisitorData& data, CppAD::AD<double>& ans)
{
    auto tmp = std::dynamic_pointer_cast<MonomialTerm>(expr);
    if (tmp->var->fixed)
        ans += tmp->coef * data.dynamic_params[data.fixed_variables[tmp->var]];
    else
        ans += tmp->coef * data.ADvars[data.used_variables[tmp->var]];
}

#define FROM_BODY(TERM)                                                                \
    void visit_##TERM(expr_pointer_t& expr, VisitorData& data, CppAD::AD<double>& ans) \
    {                                                                                  \
        auto tmp = std::dynamic_pointer_cast<TERM>(expr);                              \
        visit_expression(tmp->body, data, ans);                                        \
    }

FROM_BODY(InequalityTerm)
FROM_BODY(EqualityTerm)
FROM_BODY(ObjectiveTerm)

void visit_SubExpressionTerm(expr_pointer_t& expr, VisitorData& data, CppAD::AD<double>& ans)
{
    auto tmp = std::dynamic_pointer_cast<SubExpressionTerm>(expr);
    CppAD::AD<double> body;
    visit_expression(tmp->body, data, body);
    ans += body;
}

void visit_NegateTerm(expr_pointer_t& expr, VisitorData& data, CppAD::AD<double>& ans)
{
    auto tmp = std::dynamic_pointer_cast<NegateTerm>(expr);
    CppAD::AD<double> body;
    visit_expression(tmp->body, data, body);
    ans += -body;
}

void visit_PlusTerm(expr_pointer_t& expr, VisitorData& data, CppAD::AD<double>& ans)
{
    auto tmp = std::dynamic_pointer_cast<PlusTerm>(expr);
    std::vector<expr_pointer_t>& vec = *(tmp->data);
    auto n = tmp->num_expressions();
    for (size_t i = 0; i < n; i++) {
        CppAD::AD<double> next;
        visit_expression(vec[i], data, next);
        ans += next;
    }
}

void visit_TimesTerm(expr_pointer_t& expr, VisitorData& data, CppAD::AD<double>& ans)
{
    auto tmp = std::dynamic_pointer_cast<TimesTerm>(expr);
    CppAD::AD<double> lhs;
    visit_expression(tmp->lhs, data, lhs);
    CppAD::AD<double> rhs;
    visit_expression(tmp->rhs, data, rhs);
    ans += lhs * rhs;
}

void visit_DivideTerm(expr_pointer_t& expr, VisitorData& data, CppAD::AD<double>& ans)
{
    auto tmp = std::dynamic_pointer_cast<DivideTerm>(expr);
    CppAD::AD<double> lhs;
    visit_expression(tmp->lhs, data, lhs);
    CppAD::AD<double> rhs;
    visit_expression(tmp->rhs, data, rhs);

    ans += lhs / rhs;
}

void visit_IfThenElseTerm(expr_pointer_t& expr, VisitorData& data, CppAD::AD<double>& ans)
{
    auto tmp = std::dynamic_pointer_cast<IfThenElseTerm>(expr);
    CppAD::AD<double> cond_;
    visit_expression(tmp->cond_expr, data, cond_);
    CppAD::AD<double> then_;
    visit_expression(tmp->then_expr, data, then_);
    CppAD::AD<double> else_;
    visit_expression(tmp->else_expr, data, else_);
    CppAD::AD<double> zero(0.);

    ans += CppAD::CondExpGt(cond_, zero, then_, else_);
}

#define UNARY_VISITOR(TERM, FN)                                                        \
    void visit_##TERM(expr_pointer_t& expr, VisitorData& data, CppAD::AD<double>& ans) \
    {                                                                                  \
        auto tmp = std::dynamic_pointer_cast<TERM>(expr);                              \
        CppAD::AD<double> body;                                                        \
        visit_expression(tmp->body, data, body);                                       \
        ans += CppAD::FN(body);                                                        \
    }

UNARY_VISITOR(AbsTerm, abs)
// UNARY_VISITOR(CeilTerm, ceil)
// UNARY_VISITOR(FloorTerm, floor)
UNARY_VISITOR(ExpTerm, exp)
UNARY_VISITOR(LogTerm, log)
UNARY_VISITOR(Log10Term, log10)
UNARY_VISITOR(SqrtTerm, sqrt)
UNARY_VISITOR(SinTerm, sin)
UNARY_VISITOR(CosTerm, cos)
UNARY_VISITOR(TanTerm, tan)
UNARY_VISITOR(SinhTerm, sinh)
UNARY_VISITOR(CoshTerm, cosh)
UNARY_VISITOR(TanhTerm, tanh)
UNARY_VISITOR(ASinTerm, asin)
UNARY_VISITOR(ACosTerm, acos)
UNARY_VISITOR(ATanTerm, atan)
UNARY_VISITOR(ASinhTerm, asinh)
UNARY_VISITOR(ACoshTerm, acosh)
UNARY_VISITOR(ATanhTerm, atanh)

void visit_PowTerm(expr_pointer_t& expr, VisitorData& data, CppAD::AD<double>& ans)
{
    auto tmp = std::dynamic_pointer_cast<PowTerm>(expr);
    CppAD::AD<double> lhs;
    visit_expression(tmp->lhs, data, lhs);
    if (tmp->rhs->is_constant()) {
        double val = tmp->rhs->eval();
        if (fabs(val - int(val)) < 1e-12) {
            ans += CppAD::pow(lhs, int(val));
            return;
        }
    }

    CppAD::AD<double> rhs;
    visit_expression(tmp->rhs, data, rhs);
    ans += CppAD::pow(lhs, rhs);
}

#define VISIT_CASE(TERM)               \
    case TERM##_id:                    \
        visit_##TERM(expr, data, ans); \
        break

void visit_expression(expr_pointer_t& expr, VisitorData& data, CppAD::AD<double>& ans)
{
    auto curr = data.cache.find(expr);
    if (curr != data.cache.end()) {
        ans += curr->second;
        return;
    }

    switch (expr->id()) {
        VISIT_CASE(ConstantTerm);
        VISIT_CASE(ParameterTerm);
        VISIT_CASE(VariableTerm);
        VISIT_CASE(MonomialTerm);
        VISIT_CASE(InequalityTerm);
        VISIT_CASE(EqualityTerm);
        VISIT_CASE(ObjectiveTerm);
        VISIT_CASE(NegateTerm);
        VISIT_CASE(SubExpressionTerm);
        VISIT_CASE(PlusTerm);
        VISIT_CASE(TimesTerm);
        VISIT_CASE(DivideTerm);
        VISIT_CASE(IfThenElseTerm);
        VISIT_CASE(AbsTerm);
        // VISIT_CASE(CeilTerm);
        // VISIT_CASE(FloorTerm);
        VISIT_CASE(ExpTerm);
        VISIT_CASE(LogTerm);
        VISIT_CASE(Log10Term);
        VISIT_CASE(SqrtTerm);
        VISIT_CASE(SinTerm);
        VISIT_CASE(CosTerm);
        VISIT_CASE(TanTerm);
        VISIT_CASE(SinhTerm);
        VISIT_CASE(CoshTerm);
        VISIT_CASE(TanhTerm);
        VISIT_CASE(ASinTerm);
        VISIT_CASE(ACosTerm);
        VISIT_CASE(ATanTerm);
        VISIT_CASE(ASinhTerm);
        VISIT_CASE(ACoshTerm);
        VISIT_CASE(ATanhTerm);
        VISIT_CASE(PowTerm);

        default:
            throw std::runtime_error(
                "Error in CppAD_Repn visitor!  Visiting unexpected expression term "
                + std::to_string(expr->id()));
    };

    data.cache[expr] = ans;
}

}  // namespace

void CppAD_Repn::build_expression(expr_pointer_t root, std::vector<CppAD::AD<double> >& ADvars,
                                  CppAD::AD<double>& ans,
                                  std::unordered_map<VariableRepn, size_t>& _used_variables)
{
    VisitorData data(ADvars, _used_variables, fixed_variables, parameters, dynamic_params);
    visit_expression(root, data, ans);
}

}  // namespace coek
