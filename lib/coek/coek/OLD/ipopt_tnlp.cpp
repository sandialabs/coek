#include <cassert>
#include <chrono>

#include "IpIpoptApplication.hpp"
#include "IpSolveStatistics.hpp"
#include "IpTNLP.hpp"
#include "coek/api/constraint.hpp"
#include "coek/api/expression.hpp"
#include "coek/api/objective.hpp"
#include "coek/autograd/autograd.hpp"
#include "ipopt.hpp"

using namespace Ipopt;

namespace coek {

template <typename TYPE>
std::ostream& operator<<(std::ostream& ostr, const std::vector<TYPE>& vec)
{
    for (size_t i = 0; i < vec.size(); i++) ostr << vec[i] << " ";
    return ostr;
}

class IpoptProblem : public TNLP {
   public:
    NLPModel model;

    bool start_from_last_x;
    // double last_objval;
    std::vector<double> last_x;
    std::vector<double> last_zL;
    std::vector<double> last_zU;
    // std::vector<double> last_g;
    std::vector<double> last_lambda;

    std::vector<double> tmp_grad;
    std::vector<double> tmp_c;
    std::vector<double> tmp_j;
    std::vector<double> tmp_h;
    std::vector<double> tmp_hw;

    // default constructor
    IpoptProblem(NLPModel& _model) { model = _model; }

    void build()
    {
        start_from_last_x = false;
        tmp_grad.resize(model.num_variables());
        tmp_c.resize(model.num_constraints());
        tmp_j.resize(model.num_nonzeros_Jacobian());
        tmp_h.resize(model.num_nonzeros_Hessian_Lagrangian());
        tmp_hw.resize(model.num_objectives() + model.num_constraints());
    }

    // default destructor
    virtual ~IpoptProblem() {}

    // Method to return some info about the nlp
    bool get_nlp_info(Index& n, Index& m, Index& nnz_jac_g, Index& nnz_h_lag,
                      IndexStyleEnum& index_style);

    // Method to return the bounds for my problem
    bool get_bounds_info(Index n, Number* x_l, Number* x_u, Index m, Number* g_l, Number* g_u);

    // Method to return the starting point for the algorithm
    bool get_starting_point(Index n, bool init_x, Number* x, bool init_z, Number* z_L, Number* z_U,
                            Index m, bool init_lambda, Number* lambda);

    // Method to return the objective value
    bool eval_f(Index n, const Number* x, bool new_x, Number& obj_value);

    // Method to return the gradient of the objective
    bool eval_grad_f(Index n, const Number* x, bool new_x, Number* grad_f);

    // Method to return the constraint residuals
    bool eval_g(Index n, const Number* x, bool new_x, Index m, Number* g);

    // Method to return:
    //   1) The structure of the jacobian (if "values" is NULL)
    //   2) The values of the jacobian (if "values" is not NULL)
    bool eval_jac_g(Index n, const Number* x, bool new_x, Index m, Index nele_jac, Index* iRow,
                    Index* jCol, Number* values);

    // Method to return:
    //   1) The structure of the hessian of the lagrangian (if "values" is NULL)
    //   2) The values of the hessian of the lagrangian (if "values" is not NULL)
    bool eval_h(Index n, const Number* x, bool new_x, Number obj_factor, Index m,
                const Number* lambda, bool new_lambda, Index nele_hess, Index* iRow, Index* jCol,
                Number* values);

    // This method is called when the algorithm is complete so the TNLP can store/write the solution
    void finalize_solution(SolverReturn status, Index n, const Number* x, const Number* z_L,
                           const Number* z_U, Index m, const Number* g, const Number* lambda,
                           Number obj_value, const IpoptData* ip_data,
                           IpoptCalculatedQuantities* ip_cq);

   private:
    // This method should not be used.
    IpoptProblem(const IpoptProblem&) {}

    // This method should not be used.
    IpoptProblem& operator=(const IpoptProblem&) { return *this; }
};

class IpoptSolverRepn {
   public:
    SmartPtr<IpoptProblem> problem;

    IpoptSolverRepn(NLPModel& model) { problem = new IpoptProblem(model); }

    void build() { problem->build(); }
};

bool IpoptProblem::get_nlp_info(Index& n, Index& m, Index& nnz_jac_g, Index& nnz_h_lag,
                                IndexStyleEnum& index_style)
{
    // The number of variables
    n = model.num_variables();
    // std::cout << "HERE z " << n << std::endl << std::flush;

    // The number of constraints
    m = model.num_constraints();

    // The number of nonzeros in the jacobian
    nnz_jac_g = model.num_nonzeros_Jacobian();

    // The number of nonzeros in the hessian of the lagrangian
    nnz_h_lag = model.num_nonzeros_Hessian_Lagrangian();
    // std::cout << "HERE Z " << nnz_h_lag << std::endl << std::flush;

    // The index style for row/col entries
    // index_style = FORTRAN_STYLE;
    index_style = C_STYLE;

    return true;
}

bool IpoptProblem::get_bounds_info(Index n, Number* x_l, Number* x_u, Index m, Number* g_l,
                                   Number* g_u)
{
    // x_l[i] - the lower bound of variable i
    // x_u[i] - the upper bound of variable i

    // std::cout << "GET BOUNDS " << std::endl << std::flush;
    // std::cout << "  num variables: " << model.num_variables() << std::endl;
    //  Setting bounds to +/- infinity
    for (size_t i = 0; i < model.num_variables(); i++) {
        auto v = model.get_variable(i);
        if (v.is_binary())
            throw std::runtime_error("Cannot apply ipopt to problems with binary variables");
        if (v.is_integer())
            throw std::runtime_error("Cannot apply ipopt to problems with integer variables");

        x_l[i] = v.get_lb();
        x_u[i] = v.get_ub();
        // std::cout << "x " << i << " " << x_l[i] << " " << x_u[i] << std::endl << std::flush;
    }

    // std::cout << "  num constraints: " << model.num_constraints() << std::endl;
    for (size_t j = 0; j < model.repn->model.repn->constraints.size(); j++) {
        if (model.repn->model.repn->constraints[j].is_inequality()) {
            // Inequality constraints are g(x) <= 0
            g_l[j] = -COEK_INFINITY;
            g_u[j] = 0;
            // std::cout << "g " << j << " " << g_l[j] << " " << g_u[j] << std::endl << std::flush;
        }
        else {
            // Equality constraints are g(x) == 0
            g_l[j] = 0;
            g_u[j] = 0;
            // std::cout << "g " << j << " " << g_l[j] << " " << g_u[j] << std::endl << std::flush;
        }
    }

    return true;
}

bool IpoptProblem::get_starting_point(Index n, bool init_x, Number* x, bool init_z, Number* z_L,
                                      Number* z_U, Index m, bool init_lambda, Number* lambda)
{
    // std::cout << "GET STARTING POINT " << init_x << " " << init_z << " " << init_lambda <<
    // std::endl << std::flush;

    // We only have starting values for x
    if (init_x) {
        last_x.resize(n);
        // Initialize the x[i];
        for (size_t i = 0; i < model.num_variables(); i++) {
            auto v = model.get_variable(i);
            if (start_from_last_x)
                x[i] = last_x[i];
            else
                x[i] = v.get_value();
            // std::cout << "x " << i << " " << x[i] << std::endl << std::flush;
        }
    }

    if (init_z) {
        last_zL.resize(n);
        last_zU.resize(n);
        for (int i = 0; i < n; i++) z_L[i] = last_zL[i];
        for (int i = 0; i < n; i++) z_U[i] = last_zU[i];
    }

    if (init_lambda) {
        last_lambda.resize(m);
        for (int i = 0; i < m; i++) lambda[i] = last_lambda[i];
    }

    return true;
}

bool IpoptProblem::eval_f(Index n, const Number* x, bool new_x, Number& obj_value)
{
    // std::cout << "EVAL F " << std::endl << std::flush;
    if (new_x) {
        // std::cout << "Set Vars " << std::endl << std::flush;
        model.set_variables(x, n);
    }

    // std::cout << "Compute F - START" << std::endl << std::flush;
    //  return the value of the objective function
    obj_value = model.compute_f(0);

    // std::cout << "EVAL F - END" << std::endl << std::flush;
    return true;
}

bool IpoptProblem::eval_grad_f(Index n, const Number* x, bool new_x, Number* grad_f)
{
    // std::cout << "EVAL DF " << std::endl << std::flush;
    if (new_x) {
        // std::cout << "Set Vars " << std::endl << std::flush;
        model.set_variables(x, n);
    }

    // std::cout << "Compute DF - START" << std::endl << std::flush;
    //  return the gradient of the objective function grad_{x} f(x)
    double f;
    model.compute_df(f, tmp_grad, 0);
    int i = 0;
    for (std::vector<double>::iterator it = tmp_grad.begin(); it != tmp_grad.end(); it++)
        grad_f[i++] = *it;

    // std::cout << "EVAL DF - END" << std::endl << std::flush;
    return true;
}

bool IpoptProblem::eval_g(Index n, const Number* x, bool new_x, Index m, Number* g)
{
    // std::cout << "EVAL G " << std::endl << std::flush;
    if (new_x) {
        // std::cout << "Set Vars " << std::endl << std::flush;
        model.set_variables(x, n);
    }

    // std::cout << "Compute G - START" << std::endl << std::flush;
    //  return the value of the constraints: g(x)
    model.compute_c(tmp_c);
    int i = 0;
    for (std::vector<double>::iterator it = tmp_c.begin(); it != tmp_c.end(); it++) g[i++] = *it;

    // std::cout << "EVAL G - END" << std::endl << std::flush;
    return true;
}

bool IpoptProblem::eval_jac_g(Index n, const Number* x, bool new_x, Index m, Index nele_jac,
                              Index* jRow, Index* jCol, Number* values)
{
    // std::cout << "EVAL J " << std::endl << std::flush;
    if (values == NULL) {
        // Return the structure of the Jacobian of the constraints
        std::vector<size_t> jrow;
        std::vector<size_t> jcol;
        model.get_J_nonzeros(jrow, jcol);
        for (size_t i = 0; i < jrow.size(); i++) {
            jRow[i] = jrow[i];
            jCol[i] = jcol[i];
        }
        // std::cout << "Get Structure " << std::endl << std::flush;
        // std::cout << jrow << std::endl;
        // std::cout << jcol << std::endl;
    }

    else {
        assert(nele_jac == tmp_j.size());
        // std::cout << "Do Eval - START" << std::endl << std::flush;
        // std::cout << "nele_jac " << nele_jac << std::endl << std::flush;
        //  Return the values of the Jacobian of the constraints
        if (new_x) {
            model.set_variables(x, n);
        }
        model.compute_J(tmp_j);
        for (size_t i = 0; i < tmp_j.size(); i++) values[i] = tmp_j[i];
        // std::cout << "Do Eval - END" << std::endl << std::flush;
    }

    // std::cout << "EVAL J - END" << std::endl << std::flush;
    return true;
}

bool IpoptProblem::eval_h(Index n, const Number* x, bool new_x, Number obj_factor, Index m,
                          const Number* lambda, bool new_lambda, Index nele_hess, Index* hRow,
                          Index* hCol, Number* values)
{
    if (values == NULL) {
        // Return the structure. This is a symmetric matrix, fill the lower left
        // triangle only.
        std::vector<size_t> hrow;
        std::vector<size_t> hcol;
        model.get_H_nonzeros(hrow, hcol);
        // std::cout << "HERE hrow_size " << hrow.size() << std::endl;
        for (size_t i = 0; i < hrow.size(); i++) {
            hRow[i] = hrow[i];
            hCol[i] = hcol[i];
            // std::cout << hRow[i] << " " << hCol[i] << std::endl;
        }
    }

    else {
        // Return the values of the Hessian
        if (new_x) {
            model.set_variables(x, n);
        }
        size_t nf = model.num_objectives();
        size_t nc = model.num_constraints();
        for (size_t i = 0; i < nf; i++) tmp_hw[i] = obj_factor;
        // for (size_t i=nf; i<nc; i++)
        for (size_t i = 0; i < nc; i++) tmp_hw[i + nf] = lambda[i];
        model.compute_H(tmp_hw, tmp_h);
        for (size_t i = 0; i < tmp_h.size(); i++) values[i] = tmp_h[i];
    }

    return true;
}

void IpoptProblem::finalize_solution(SolverReturn status, Index n, const Number* x,
                                     const Number* z_L, const Number* z_U, Index m, const Number* g,
                                     const Number* lambda, Number obj_value,
                                     const IpoptData* ip_data, IpoptCalculatedQuantities* ip_cq)
{
    // std::cout << "FINALIZE " << n << " " << m << std::endl;
    assert(n == model.num_variables());
    assert(m == model.num_constraints());
    last_x.resize(n);
    last_zL.resize(n);
    last_zU.resize(n);
    // last_g.resize(m);
    last_lambda.resize(m);

    // last_objval = obj_value;

    for (int i = 0; i < n; i++) {
        auto v = model.get_variable(i);
        v.set_value(x[i]);
        last_x[i] = x[i];
        last_zL[i] = z_L[i];
        last_zU[i] = z_U[i];
    }

    for (int i = 0; i < m; i++) {
        // last_g[i] = g[i];
        last_lambda[i] = lambda[i];
    }
}

int IpoptSolver::solve(NLPModel& _model)
{
    load(_model);
    if (not initial_solve()) {
        std::cout << "ERROR: must reset the model before solving" << std::endl;
        return -1;
    }
    return perform_solve();
}

int IpoptSolver::resolve()
{
    auto start = std::chrono::high_resolution_clock::now();

    if (not initial_solve()) nlp->problem->model.reset();

    auto it = string_options.find("warm_start_init_point");
    if ((it != string_options.end()) and (it->second == "yes"))
        nlp->problem->start_from_last_x = true;
    else
        nlp->problem->start_from_last_x = false;
    int status = perform_solve();

    auto curr = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = curr - start;
    std::cout << "Time to solve: " << diff.count() << " s\n";

    return status;
}

int IpoptSolver::perform_solve()
{
    SmartPtr<IpoptApplication> app = IpoptApplicationFactory();

    // Initialize the IpoptApplication and process the options
    ApplicationReturnStatus status = app->Initialize();
    if (status != Solve_Succeeded) {
        std::cout << std::endl << std::endl << "*** Error during initialization!" << std::endl;
        return (int)status;
    }

    for (auto it = string_options.begin(); it != string_options.end(); ++it)
        app->Options()->SetStringValue(it->first, it->second);
    for (auto it = integer_options.begin(); it != integer_options.end(); ++it)
        app->Options()->SetIntegerValue(it->first, it->second);
    for (auto it = double_options.begin(); it != double_options.end(); ++it)
        app->Options()->SetNumericValue(it->first, it->second);

    status = app->OptimizeTNLP(nlp->problem);

    if (status == Solve_Succeeded) {
        // Retrieve some statistics about the solve
        Index iter_count = app->Statistics()->IterationCount();
        std::cout << std::endl
                  << std::endl
                  << "*** The problem solved in " << iter_count << " iterations!" << std::endl;

        Number final_obj = app->Statistics()->FinalObjective();
        std::cout << std::endl
                  << std::endl
                  << "*** The final value of the objective function is " << final_obj << '.'
                  << std::endl;
    }

    return (int)status;
}

void IpoptSolver::load(NLPModel& _model)
{
    model = &_model;
    nlp = std::make_shared<IpoptSolverRepn>(_model);
    nlp->build();
}

}  // namespace coek
