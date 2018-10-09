#include "IpTNLP.hpp"
#include "model.h"

using namespace Ipopt;


class IpoptNLP : public TNLP
{
public:

    Model* model;

    /** default constructor */
    IpoptNLP(Model* _model)
        {model=_model;}

    /** default destructor */
    virtual ~IpoptNLP()
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
    IpoptNLP(const IpoptNLP&)
        {}

    /** This method should not be used. */
    IpoptNLP& operator=(const IpoptNLP&)
        { return *this; }

};



bool IpoptNLP::get_nlp_info(Index& n, Index& m, Index& nnz_jac_g,
                         Index& nnz_h_lag, IndexStyleEnum& index_style)
{
// The number of variables
n = model->num_variables();

// The number of constraints
m = model->equalities.size() + model->inequalities.size();

// The number of nonzeros in the jacobian
nnz_jac_g = 0;

// The number of nonzeros in the hessian of the lagrangian
nnz_h_lag = 0;

// The index style for row/col entries
index_style = FORTRAN_STYLE;

return true;
}


bool IpoptNLP::get_bounds_info(Index n, Number* x_l, Number* x_u,
                            Index m, Number* g_l, Number* g_u)
{
// x_l[i] - the lower bound of variable i
// x_u[i] - the upper bound of variable i

// Setting bounds to +/- infinity
Index i=0;
for (ordered_variable_iterator_t it=model->variables.begin(); it != model->variables.end(); it++) {
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


bool IpoptNLP::get_starting_point(Index n, bool init_x, Number* x,
                               bool init_z, Number* z_L, Number* z_U,
                               Index m, bool init_lambda, Number* lambda)
{
// Here, we assume we only have starting values for x
assert(init_x == true);
assert(init_z == false);
assert(init_lambda == false);

// Initialize the x[i];
Index i=0;
for (ordered_variable_iterator_t it=model->variables.begin(); it != model->variables.end(); it++) {
    x[i++] = (*it)->_value;
    }

return true;
}



