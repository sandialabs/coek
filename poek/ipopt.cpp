#include "IpIpoptApplication.hpp"
#include "IpSolveStatistics.hpp"
#include "IpTNLP.hpp"
#include "solver.h"

using namespace Ipopt;


class IpoptProblem : public TNLP
{
public:

    Model* model;
    std::vector<double> tmp_grad;
    std::vector<double> tmp_c;

    /** default constructor */
    IpoptProblem(Model* _model)
        {
        model=_model;
        }

    void build()
        {
        tmp_grad.resize(model->variables.size());
        tmp_c.resize(model->inequalities.size() + model->equalities.size());
        }

    /** default destructor */
    virtual ~IpoptProblem()
        {}

    /** Method to return some info about the nlp */
    virtual bool get_nlp_info(Index& n, Index& m, Index& nnz_jac_g,
                    Index& nnz_h_lag, IndexStyleEnum& index_style);
  
    /** Method to return the bounds for my problem */
    virtual bool get_bounds_info(Index n, Number* x_l, Number* x_u, Index m, Number* g_l, Number* g_u);
  
    /** Method to return the starting point for the algorithm */
    virtual bool get_starting_point(Index n, bool init_x, Number* x, 
                    bool init_z, Number* z_L, Number* z_U,
                    Index m, bool init_lambda,
                    Number* lambda);
  
    /** Method to return the objective value */
    virtual bool eval_f(Index n, const Number* x, bool new_x, Number& obj_value);
  
    /** Method to return the gradient of the objective */
    virtual bool eval_grad_f(Index n, const Number* x, bool new_x, Number* grad_f);
  
    /** Method to return the constraint residuals */
    virtual bool eval_g(Index n, const Number* x, bool new_x, Index m, Number* g);

  /** Method to return:
    *   1) The structure of the jacobian (if "values" is NULL)
    *   2) The values of the jacobian (if "values" is not NULL)
    */
  virtual bool eval_jac_g(Index n, const Number* x, bool new_x,
                          Index m, Index nele_jac, Index* iRow, Index *jCol,
                          Number* values);

  /** Method to return:
    *   1) The structure of the hessian of the lagrangian (if "values" is NULL)
    *   2) The values of the hessian of the lagrangian (if "values" is not NULL)
    */
  virtual bool eval_h(Index n, const Number* x, bool new_x,
                      Number obj_factor, Index m, const Number* lambda,
                      bool new_lambda, Index nele_hess, Index* iRow,
                      Index* jCol, Number* values);

  /** This method is called when the algorithm is complete so the TNLP can store/write the solution */
  virtual void finalize_solution(SolverReturn status,
                 Index n, const Number* x, const Number* z_L, const Number* z_U,
                 Index m, const Number* g, const Number* lambda,
                 Number obj_value,
                 const IpoptData* ip_data,
                 IpoptCalculatedQuantities* ip_cq);

private:

    /** This method should not be used. */
    IpoptProblem(const IpoptProblem&)
        {}

    /** This method should not be used. */
    IpoptProblem& operator=(const IpoptProblem&)
        { return *this; }

};



bool IpoptProblem::get_nlp_info(Index& n, Index& m, Index& nnz_jac_g,
                         Index& nnz_h_lag, IndexStyleEnum& index_style)
{
// The number of variables
n = model->num_variables();

// The number of constraints
m = model->equalities.size() + model->inequalities.size();

// The number of nonzeros in the jacobian
nnz_jac_g = model->J.size();

// The number of nonzeros in the hessian of the lagrangian
nnz_h_lag = 0;

// The index style for row/col entries
index_style = C_STYLE;

return true;
}


bool IpoptProblem::get_bounds_info(Index n, Number* x_l, Number* x_u,
                            Index m, Number* g_l, Number* g_u)
{
// x_l[i] - the lower bound of variable i
// x_u[i] - the upper bound of variable i

// Setting bounds to +/- infinity
Index i=0;
for (std::vector<Variable*>::iterator it=model->variables.begin(); it != model->variables.end(); it++) {
    x_l[i] = (*it)->lb;
    x_u[i] = (*it)->ub;
    i++;
    }

// Equality constraints are g(x) == 0
for (Index j=0; j<model->inequalities.size(); j++) {
    g_l[j] = 0;
    g_u[j] = 0; 
    }
// Inequality constraints are g(x) <= 0
for (Index j=model->inequalities.size(); j<m; j++) {
    g_l[j] = NEGATIVE_INFINITY;
    g_u[j] = 0; 
    }

return true;
}


bool IpoptProblem::get_starting_point(Index n, bool init_x, Number* x,
                               bool init_z, Number* z_L, Number* z_U,
                               Index m, bool init_lambda, Number* lambda)
{
// Here, we assume we only have starting values for x
assert(init_x == true);
assert(init_z == false);
assert(init_lambda == false);

// Initialize the x[i];
Index i=0;
for (std::vector<Variable*>::iterator it=model->variables.begin(); it != model->variables.end(); it++) {
    x[i++] = (*it)->_value;
    }

return true;
}


bool IpoptProblem::eval_f(Index n, const Number* x, bool new_x, Number& obj_value)
{
if (new_x) {
    model->set_variables(x, n);
    }

// return the value of the objective function
obj_value = model->compute_f(0);
return true;
}


bool IpoptProblem::eval_grad_f(Index n, const Number* x, bool new_x, Number* grad_f)
{
if (new_x) {
    model->set_variables(x, n);
    }

// return the gradient of the objective function grad_{x} f(x)
double f;
model->compute_df(f, tmp_grad, 0);
int i=0;
for (std::vector<double>::iterator it=tmp_grad.begin(); it != tmp_grad.end(); it++)
    grad_f[i++] = *it;
return true;
}


bool IpoptProblem::eval_g(Index n, const Number* x, bool new_x, Index m, Number* g)
{ 
if (new_x) {
    model->set_variables(x, n);
    }

// return the value of the constraints: g(x)
model->compute_c(tmp_c);
int i=0;
for (std::vector<double>::iterator it=tmp_c.begin(); it != tmp_c.end(); it++)
    g[i++] = *it;
return true;
}


bool IpoptProblem::eval_jac_g(Index n, const Number* x, bool new_x,
                       Index m, Index nele_jac, Index* iRow, Index *jCol,
                       Number* values)
{ 
if (values == NULL) {
    // Return the structure of the Jacobian of the constraints
    int i=0;
    for (size_t j=0; j<model->J_rc.size(); j++) {
        for (size_t k=0; k<model->J_rc[j].size(); k++) {
            iRow[i] = j;
            jCol[i] = model->J_rc[j][k];
            i++;
            }
        }
    }

else {
    // Return the values of the Jacobian of the constraints
    if (new_x) {
        model->set_variables(x, n);
        }
    int i=0;
    for (size_t j=0; j<model->J.size(); j++) {
        model->compute_dc(tmp_grad, j);
        for (size_t k=0; k<model->J[j].size(); k++) {
            values[i++] = model->J[j][k]->adjoint;
            }
        }
    }

return true;
}


bool IpoptProblem::eval_h(Index n, const Number* x, bool new_x,
                   Number obj_factor, Index m, const Number* lambda,
                   bool new_lambda, Index nele_hess, Index* iRow,
                   Index* jCol, Number* values)
{
assert(false);
if (values == NULL) {
    // Return the structure. This is a symmetric matrix, fill the lower left
    // triangle only.
    }

else {
    // Return the values of the Hessian
    if (new_x) {
        model->set_variables(x, n);
        }
    }

return true;
}


void IpoptProblem::finalize_solution(SolverReturn status,
                Index n, const Number* x, const Number* z_L, const Number* z_U,
                Index m, const Number* g, const Number* lambda,
                Number obj_value,
                const IpoptData* ip_data,
                IpoptCalculatedQuantities* ip_cq)
{
Index i=0;
for (std::vector<Variable*>::iterator it=model->variables.begin(); it != model->variables.end(); it++)
    (*it)->_value = x[i++];
}



int IpoptSolver::solve()
{
IpoptProblem nlp(model);
nlp.build();

// Create an instance of the IpoptApplication
//
// We are using the factory, since this allows us to compile this
// example with an Ipopt Windows DLL
SmartPtr<IpoptApplication> app = IpoptApplicationFactory();

// Initialize the IpoptApplication and process the options
ApplicationReturnStatus status = app->Initialize();
if (status != Solve_Succeeded) {
    std::cout << std::endl << std::endl << "*** Error during initialization!" << std::endl;
    return (int) status;
    }

status = app->OptimizeTNLP(&nlp);

if (status == Solve_Succeeded) {
    // Retrieve some statistics about the solve
    Index iter_count = app->Statistics()->IterationCount();
    std::cout << std::endl << std::endl << "*** The problem solved in " << iter_count << " iterations!" << std::endl;

    Number final_obj = app->Statistics()->FinalObjective();
    std::cout << std::endl << std::endl << "*** The final value of the objective function is " << final_obj << '.' << std::endl;
}

return (int) status;
}
