#include <memory>
#include <cassert>

#include "coek/api/expression.hpp"
#include "coek/api/objective.hpp"
#include "coek/api/constraint.hpp"
#include "coek/model/model_repn.hpp"
#include "coek/autograd/autograd.hpp"
#include "coek/solvers/loadlib.h"
#include "ipopt_solver.hpp"
#include "IpStdCInterfaceTypes.h"


extern "C" {

static libHandle_t ipopt_handle=NULL;
static CreateIpoptProblem_func_t CreateIpoptProblem_func_ptr=0;
static FreeIpoptProblem_func_t FreeIpoptProblem_func_ptr=0;
static AddIpoptStrOption_func_t AddIpoptStrOption_func_ptr=0;
static AddIpoptNumOption_func_t AddIpoptNumOption_func_ptr=0;
static AddIpoptIntOption_func_t AddIpoptIntOption_func_ptr=0;
/* OpenIpoptOutputFile_func_t OpenIpoptOutputFile_func_ptr=0; */
/* SetIpoptProblemScaling_func_t SetIpoptProblemScaling_func_ptr=0; */
static SetIntermediateCallback_func_t SetIntermediateCallback_func_ptr=0;
static IpoptSolve_func_t IpoptSolve_func_ptr=0;

}

namespace coek {

int load_ipopt_library(const char* libname, std::string& error_message )
{
char buf[256];
ipopt_handle = loadlib(libname, buf, 256);
if (ipopt_handle == NULL) {
    error_message = buf;
    return 1;
    }

CreateIpoptProblem_func_ptr = (CreateIpoptProblem_func_t)getsym(ipopt_handle, "CreateIpoptProblem", buf, 256);
FreeIpoptProblem_func_ptr = (FreeIpoptProblem_func_t)getsym(ipopt_handle, "FreeIpoptProblem", buf, 256);
AddIpoptStrOption_func_ptr = (AddIpoptStrOption_func_t)getsym(ipopt_handle, "AddIpoptStrOption", buf, 256);
AddIpoptNumOption_func_ptr = (AddIpoptNumOption_func_t)getsym(ipopt_handle, "AddIpoptNumOption", buf, 256);
AddIpoptIntOption_func_ptr = (AddIpoptIntOption_func_t)getsym(ipopt_handle, "AddIpoptIntOption", buf, 256);
//OpenIpoptOutputFile_func_ptr = (OpenIpoptOutputFile_func_t)getsym(ipopt_handle, "OpenIpoptOutputFile", buf, 256);
//SetIpoptProblemScaling_func_ptr = (SetIpoptProblemScaling_func_t)getsym(ipopt_handle, "SetIpoptProblemScaling", buf, 256);
SetIntermediateCallback_func_ptr = (SetIntermediateCallback_func_t)getsym(ipopt_handle, "SetIntermediateCallback", buf, 256);
IpoptSolve_func_ptr = (IpoptSolve_func_t)getsym(ipopt_handle, "IpoptSolve", buf, 256);

return 0;
}


class IpoptModel
{
public:

    IpoptProblem app;
    NLPModel model;

    bool start_from_last_x;
    Number last_objval;
    Index last_iter_count;
    std::vector<Number> last_x;
    std::vector<Number> last_g;
    std::vector<Number> last_zL;
    std::vector<Number> last_zU;
    std::vector<Number> last_lambda;

    std::vector<double> tmp_grad;
    std::vector<double> tmp_c;
    std::vector<double> tmp_j;
    std::vector<double> tmp_h;
    std::vector<double> tmp_hw;

    // default constructor
    IpoptModel(NLPModel& _model)
        {
        app = 0;
        model = _model;
        }

    // default destructor
    virtual ~IpoptModel()
        {
        if (app)
            (*FreeIpoptProblem_func_ptr)(app);
        }

    // initialize using the COEK model
    void build();

    // Method to return some info about the nlp
    bool get_nlp_info(Index& n, Index& m, Index& nnz_jac_g,
                    Index& nnz_h_lag, int& index_style);
  
    // Method to return the bounds for my problem
    bool get_bounds_info(Index n, Number* x_l, Number* x_u, Index m, Number* g_l, Number* g_u);
  
/*
    // Method to return the starting point for the algorithm
    bool get_starting_point(Index n, bool init_x, Number* x, 
                    bool init_z, Number* z_L, Number* z_U,
                    Index m, bool init_lambda,
                    Number* lambda);
*/ 

    // Method to return the objective value
    bool eval_f(Index n, const Number* x, bool new_x, Number& obj_value);
  
    // Method to return the gradient of the objective
    bool eval_grad_f(Index n, const Number* x, bool new_x, Number* grad_f);
  
    // Method to return the constraint residuals
    bool eval_g(Index n, const Number* x, bool new_x, Index m, Number* g);

    // Method to return:
    //   1) The structure of the jacobian (if "values" is NULL)
    //   2) The values of the jacobian (if "values" is not NULL)
    bool eval_jac_g(Index n, const Number* x, bool new_x,
                          Index m, Index nele_jac, Index* iRow, Index *jCol,
                          Number* values);

    // Method to return:
    //   1) The structure of the hessian of the lagrangian (if "values" is NULL)
    //   2) The values of the hessian of the lagrangian (if "values" is not NULL)
    bool eval_h(Index n, const Number* x, bool new_x,
                      Number obj_factor, Index m, const Number* lambda,
                      bool new_lambda, Index nele_hess, Index* iRow,
                      Index* jCol, Number* values);

    int perform_solve();

/*
    // This method is called when the algorithm is complete so the TNLP can store/write the solution
    void finalize_solution(SolverReturn status,
                 Index n, const Number* x, const Number* z_L, const Number* z_U,
                 Index m, const Number* g, const Number* lambda,
                 Number obj_value,
                 const IpoptData* ip_data,
                 IpoptCalculatedQuantities* ip_cq);
*/

private:

    // This method should not be used.
    IpoptModel(const IpoptModel&)
        {}

    // This method should not be used.
    IpoptModel& operator=(const IpoptModel&)
        { return *this; }
};


bool IpoptModel::get_nlp_info(Index& n, Index& m, Index& nnz_jac_g,
                         Index& nnz_h_lag, int& index_style)
{
// The number of variables
n = static_cast<Index>(model.num_variables());
//std::cout << "HERE z " << n << std::endl << std::flush;

// The number of constraints
m = static_cast<Index>(model.num_constraints());

// The number of nonzeros in the jacobian
nnz_jac_g = static_cast<Index>(model.num_nonzeros_Jacobian());

// The number of nonzeros in the hessian of the lagrangian
nnz_h_lag = static_cast<Index>(model.num_nonzeros_Hessian_Lagrangian());
//std::cout << "HERE Z " << nnz_h_lag << std::endl << std::flush;

// The index style for row/col entries
//index_style = FORTRAN_STYLE;
index_style = 0;  /* C_STYLE */

return true;
}


bool IpoptModel::get_bounds_info(Index /*n*/, Number* x_l, Number* x_u,
                            Index /*m*/, Number* g_l, Number* g_u)
{
// x_l[i] - the lower bound of variable i
// x_u[i] - the upper bound of variable i

//std::cout << "GET BOUNDS " << std::endl << std::flush;
//std::cout << "  num variables: " << model.num_variables() << std::endl;
// Setting bounds to +/- infinity
for (size_t i=0; i<model.num_variables(); i++) {
    auto v = model.get_variable(i);
    if (v.is_binary())
        throw std::runtime_error("Cannot apply ipopt to problems with binary variables");
    if (v.is_integer())
        throw std::runtime_error("Cannot apply ipopt to problems with integer variables");

    x_l[i] = v.lower();
    x_u[i] = v.upper();
    //std::cout << "x " << i << " " << x_l[i] << " " << x_u[i] << std::endl << std::flush;
    }

//std::cout << "  num constraints: " << model.num_constraints() << std::endl;
for (size_t j=0; j<model.repn->model.repn->constraints.size(); j++) {
    if (model.repn->model.repn->constraints[j].is_inequality()) {
        // Inequality constraints are g(x) <= 0
        g_l[j] = - COEK_INFINITY;
        g_u[j] = 0; 
        //std::cout << "g " << j << " " << g_l[j] << " " << g_u[j] << std::endl << std::flush;
        }
    else {
        // Equality constraints are g(x) == 0
        g_l[j] = 0;
        g_u[j] = 0; 
        //std::cout << "g " << j << " " << g_l[j] << " " << g_u[j] << std::endl << std::flush;
        }
    }

return true;
}


/*
bool IpoptModel::get_starting_point(Index n, bool init_x, Number* x,
                               bool init_z, Number* z_L, Number* z_U,
                               Index m, bool init_lambda, Number* lambda)
{
//std::cout << "GET STARTING POINT " << init_x << " " << init_z << " " << init_lambda << std::endl << std::flush;

// We only have starting values for x
if (init_x) {
    size_t n_ = static_cast<size_t>(n);
    last_x.resize(n_);
    // Initialize the x[i];
    for (size_t i=0; i<model.num_variables(); i++) {
        if (start_from_last_x)
            x[i] = last_x[i];
        else {
            auto v = model.get_variable(i);
            x[i] = v.get_value();
            }
        //std::cout << "x " << i << " " << x[i] << std::endl << std::flush;
        }
    }

if (init_z) {
    size_t n_ = static_cast<size_t>(n);
    last_zL.resize(n_);
    last_zU.resize(n_);
    for (size_t i=0; i<n_; i++)
        z_L[i] = last_zL[i];
    for (size_t i=0; i<n_; i++)
        z_U[i] = last_zU[i];
    }

if (init_lambda) {
    size_t m_ = static_cast<size_t>(m);
    last_lambda.resize(m_);
    for (size_t i=0; i<m_; i++)
        lambda[i] = last_lambda[i];
    }

return true;
}
*/


bool IpoptModel::eval_f(Index n, const Number* x, bool new_x, Number& obj_value)
{
//std::cout << "EVAL F " << std::endl << std::flush;
if (new_x) {
    //std::cout << "Set Vars " << std::endl << std::flush;
    model.set_variable_view(x, static_cast<size_t>(n));
    }

//std::cout << "Compute F - START" << std::endl << std::flush;
// return the value of the objective function
obj_value = model.compute_f(0);

//std::cout << "EVAL F - END" << std::endl << std::flush;
return true;
}


bool IpoptModel::eval_grad_f(Index n, const Number* x, bool new_x, Number* grad_f)
{
//std::cout << "EVAL DF " << std::endl << std::flush;
if (new_x) {
    //std::cout << "Set Vars " << std::endl << std::flush;
    model.set_variable_view(x, static_cast<size_t>(n));
    }

//std::cout << "Compute DF - START" << std::endl << std::flush;
// return the gradient of the objective function grad_{x} f(x)
double f;
model.compute_df(f, tmp_grad, 0);
size_t i=0;
for (std::vector<double>::iterator it=tmp_grad.begin(); it != tmp_grad.end(); it++)
    grad_f[i++] = *it;

//std::cout << "EVAL DF - END" << std::endl << std::flush;
return true;
}


bool IpoptModel::eval_g(Index n, const Number* x, bool new_x, Index /*m*/, Number* g)
{ 
//std::cout << "EVAL G " << std::endl << std::flush;
if (new_x) {
    //std::cout << "Set Vars " << std::endl << std::flush;
    model.set_variable_view(x, static_cast<size_t>(n));
    }

//std::cout << "Compute G - START" << std::endl << std::flush;
// return the value of the constraints: g(x)
model.compute_c(tmp_c);
size_t i=0;
for (std::vector<double>::iterator it=tmp_c.begin(); it != tmp_c.end(); it++)
    g[i++] = *it;

//std::cout << "EVAL G - END" << std::endl << std::flush;
return true;
}


bool IpoptModel::eval_jac_g(Index n, const Number* x, bool new_x,
                        Index /*m*/,
                        Index nele_jac, Index* jRow, Index *jCol,
                        Number* values)
{ 
//std::cout << "EVAL J " << std::endl << std::flush;
if (values == NULL) {
    // Return the structure of the Jacobian of the constraints
    std::vector<size_t> jrow;
    std::vector<size_t> jcol;
    model.get_J_nonzeros(jrow, jcol);
    for (size_t i=0; i<jrow.size(); i++) {
        jRow[i] = static_cast<Index>(jrow[i]);
        jCol[i] = static_cast<Index>(jcol[i]);
        }
    //std::cout << "Get Structure " << std::endl << std::flush;
    //std::cout << jrow << std::endl;
    //std::cout << jcol << std::endl;
    }

else {
    assert(static_cast<size_t>(nele_jac) == tmp_j.size());
    //std::cout << "Do Eval - START" << std::endl << std::flush;
    //std::cout << "nele_jac " << nele_jac << std::endl << std::flush;
    // Return the values of the Jacobian of the constraints
    if (new_x) {
        model.set_variable_view(x, static_cast<size_t>(n));
        }
    model.compute_J(tmp_j);
    for (size_t i=0; i<tmp_j.size(); i++)
        values[i] = tmp_j[i];
    //std::cout << "Do Eval - END" << std::endl << std::flush;
    }

//std::cout << "EVAL J - END" << std::endl << std::flush;
return true;
}


bool IpoptModel::eval_h(Index n, const Number* x, bool new_x,
                    Number obj_factor, 
                    Index /*m*/,
                    const Number* lambda,
                    bool /*new_lambda*/, 
                    Index /*nele_hess*/,
                    Index* hRow,
                    Index* hCol, Number* values)
{
if (values == NULL) {
    // Return the structure. This is a symmetric matrix, fill the lower left
    // triangle only.
    std::vector<size_t> hrow;
    std::vector<size_t> hcol;
    model.get_H_nonzeros(hrow, hcol);
    //std::cout << "HERE hrow_size " << hrow.size() << std::endl;
    for (size_t i=0; i<hrow.size(); i++) {
        hRow[i] = static_cast<Index>(hrow[i]);
        hCol[i] = static_cast<Index>(hcol[i]);
        //std::cout << hRow[i] << " " << hCol[i] << std::endl;
        }
    }

else {
    // Return the values of the Hessian
    if (new_x) {
        model.set_variable_view(x, static_cast<size_t>(n));
        }
    size_t nf = model.num_objectives();
    size_t nc = model.num_constraints();
    for (size_t i=0; i<nf; i++)
        tmp_hw[i] = obj_factor;
    //for (size_t i=nf; i<nc; i++)
    for (size_t i=0; i<nc; i++)
        tmp_hw[i+nf] = lambda[i];
    model.compute_H(tmp_hw, tmp_h);
    for (size_t i=0; i<tmp_h.size(); i++)
        values[i] = tmp_h[i];
    }

return true;
}


Number* array_ptr(std::vector<Number>& v)
{
if (v.size() == 0)
    return 0;
return &(v[0]);
}


int IpoptModel::perform_solve()
{
enum ApplicationReturnStatus status;

if (start_from_last_x) {
    status = (*IpoptSolve_func_ptr)(
        app,
        array_ptr(last_x),
        array_ptr(last_g),
        &last_objval,
        array_ptr(last_lambda),
        array_ptr(last_zL),
        array_ptr(last_zU),
        this);
    }
else {
    for (size_t i=0; i<model.num_variables(); i++) {
        auto v = model.get_variable(i);
        last_x[i] = v.value();
        }
    status = (*IpoptSolve_func_ptr)(
        app,
        array_ptr(last_x),
        array_ptr(last_g),
        &last_objval,
        0,
        0,
        0,
        this);
    }
 
if (status == Solve_Succeeded) {
    std::cout << std::endl << std::endl << "*** The problem solved in " << last_iter_count << " iterations!" << std::endl;

    std::cout << std::endl << std::endl << "*** The final value of the objective function is " << last_objval << '.' << std::endl;

    for (size_t i=0; i<model.num_variables(); i++) {
        auto v = model.get_variable(i);
        v.value( last_x[i] );
        }
    }
else {
    std::cout << std::endl << std::endl << "*** The problem was not solved successfully.  Status = " << status << std::endl;
    }

return (int) status;
}



/*
void IpoptModel::finalize_solution(SolverReturn status,
                Index n, const Number* x, const Number* z_L, const Number* z_U,
                Index m, const Number* g, const Number* lambda,
                Number obj_value,
                const IpoptData* ip_data,
                IpoptCalculatedQuantities* ip_cq)
{
//std::cout << "FINALIZE " << n << " " << m << std::endl;
assert(n == model.num_variables());
assert(m == model.num_constraints());
last_x.resize(n);
last_zL.resize(n);
last_zU.resize(n);
//last_g.resize(m);
last_lambda.resize(m);

//last_objval = obj_value;

for (int i=0; i<n; i++) {
    auto v = model.get_variable(i);
    v.set_value( x[i] );
    last_x[i] = x[i];
    last_zL[i] = z_L[i];
    last_zU[i] = z_U[i];
    }

for (int i=0; i<m; i++) {
    //last_g[i] = g[i];
    last_lambda[i] = lambda[i];
    }
}
*/

}

extern "C" {

/* Functions used to interface with IPOPT */
Bool ipopt_capi_eval_f(
    Index       n,
    Number*     x,
    Bool        new_x,
    Number*     obj_value,
    UserDataPtr user_data
    )
{
coek::IpoptModel* nlp = reinterpret_cast<coek::IpoptModel*>(user_data);
Number ovalue;
Bool retval = nlp->eval_f(n, x, new_x, ovalue);
*obj_value = ovalue;
return retval;
}

Bool ipopt_capi_eval_grad_f(
    Index       n,
    Number*     x,
    Bool        new_x,
    Number*     grad_f,
    UserDataPtr user_data
    )
{
coek::IpoptModel* nlp = reinterpret_cast<coek::IpoptModel*>(user_data);
return nlp->eval_grad_f(n, x, new_x, grad_f);
}

Bool ipopt_capi_eval_g(
    Index       n,
    Number*     x,
    Bool        new_x,
    Index       m,
    Number*     g,
    UserDataPtr user_data
    )
{
coek::IpoptModel* nlp = reinterpret_cast<coek::IpoptModel*>(user_data);
return nlp->eval_g(n, x, new_x, m, g);
}

Bool ipopt_capi_eval_jac_g(
    Index       n,
    Number*     x,
    Bool        new_x,
    Index       m,
    Index       nele_jac,
    Index*      iRow,
    Index*      jCol,
    Number*     values,
    UserDataPtr user_data
    )
{
coek::IpoptModel* nlp = reinterpret_cast<coek::IpoptModel*>(user_data);
return nlp->eval_jac_g(n, x, new_x, m, nele_jac, iRow, jCol, values);
}

Bool ipopt_capi_eval_h(
    Index       n,
    Number*     x,
    Bool        new_x,
    Number      obj_factor,
    Index       m,
    Number*     lambda,
    Bool        new_lambda,
    Index       nele_hess,
    Index*      iRow,
    Index*      jCol,
    Number*     values,
    UserDataPtr user_data
    )
{
coek::IpoptModel* nlp = reinterpret_cast<coek::IpoptModel*>(user_data);
return nlp->eval_h(n, x, new_x, obj_factor, m, lambda, new_lambda, nele_hess, iRow, jCol, values);
}

Bool ipopt_capi_intermediate_cb(
    Index       /*alg_mod*/,
    Index       iter_count,
    Number      /*obj_value*/,
    Number      /*inf_pr*/,
    Number      /*inf_du*/,
    Number      /*mu*/,
    Number      /*d_norm*/,
    Number      /*regularization_size*/,
    Number      /*alpha_du*/,
    Number      /*alpha_pr*/,
    Index       /*ls_trials*/,
    UserDataPtr user_data
    )
{
coek::IpoptModel* nlp = reinterpret_cast<coek::IpoptModel*>(user_data);
// Track the last iteration count
nlp->last_iter_count = iter_count;
return TRUE;
}
 
}


namespace coek {

void IpoptModel::build()
{
Index n;
Index m;
Index nnz_jac_g;
Index nnz_h_lag;
int index_style;
get_nlp_info(n, m, nnz_jac_g, nnz_h_lag, index_style);
size_t n_ = static_cast<size_t>(n);
size_t m_ = static_cast<size_t>(m);

start_from_last_x=false;

last_x.resize(n_);
last_g.resize(m_);
last_zL.resize(n_);
last_zU.resize(n_);
last_lambda.resize(m_);
tmp_grad.resize(n_);
tmp_c.resize(m_);
tmp_j.resize(static_cast<size_t>(nnz_jac_g));
tmp_h.resize(static_cast<size_t>(nnz_h_lag));
tmp_hw.resize(model.num_objectives() + m_);

std::vector<Number> x_L(n_);
std::vector<Number> x_U(n_);
std::vector<Number> g_L(m_);
std::vector<Number> g_U(m_);
get_bounds_info(n, array_ptr(x_L), array_ptr(x_U), m, array_ptr(g_L), array_ptr(g_U));

app = (*CreateIpoptProblem_func_ptr)(
                    n,
                    array_ptr(x_L),
                    array_ptr(x_U),
                    m,
                    array_ptr(g_L),
                    array_ptr(g_U),
                    nnz_jac_g,
                    nnz_h_lag,
                    index_style,
                    &ipopt_capi_eval_f,
                    &ipopt_capi_eval_g,
                    &ipopt_capi_eval_grad_f,
                    &ipopt_capi_eval_jac_g,
                    &ipopt_capi_eval_h
                    );
(*SetIntermediateCallback_func_ptr)(app, &ipopt_capi_intermediate_cb);
}


class IpoptSolverRepn_CAPI : public IpoptSolverRepn
{
public:

    std::shared_ptr<IpoptModel> nlp;

    IpoptSolverRepn_CAPI(NLPModel& model)
        { nlp = std::make_shared<IpoptModel>(model); }

    int perform_solve()
        { return nlp->perform_solve(); }

    void set_start_from_last_x(bool flag)
        { nlp->start_from_last_x = flag; }

    void set_options(
                    std::map<std::string,std::string>& string_options,
                    std::map<std::string,int>& integer_options,
                    std::map<std::string,double>& double_options
                    );
};


void IpoptSolverRepn_CAPI::set_options(
                    std::map<std::string,std::string>& string_options,
                    std::map<std::string,int>& integer_options,
                    std::map<std::string,double>& double_options
                    )
{
for (auto it=string_options.begin(); it != string_options.end(); ++it) {
    char* tmp1 = const_cast<char*>(it->first.c_str());
    char* tmp2 = const_cast<char*>(it->second.c_str());
    (*AddIpoptStrOption_func_ptr)(nlp->app, tmp1, tmp2);
    }
for (auto it=integer_options.begin(); it != integer_options.end(); ++it) {
    char* tmp1 = const_cast<char*>(it->first.c_str());
    (*AddIpoptIntOption_func_ptr)(nlp->app, tmp1, it->second);
    }
for (auto it=double_options.begin(); it != double_options.end(); ++it) {
    char* tmp1 = const_cast<char*>(it->first.c_str());
    (*AddIpoptNumOption_func_ptr)(nlp->app, tmp1, it->second);
    }
}


void IpoptSolver::initialize()
{
#ifdef _MSC_VER
error_code = load_ipopt_library("libipopt-3.dll", error_message);
#else
error_code = load_ipopt_library("libipopt.so", error_message);
#endif
if (error_code == 1) {
    error_code = load_ipopt_library("libipopt.dylib", error_message);
}
if (error_code == 1) {
    error_code = NonIpopt_Exception_Thrown;
    error_occurred = true;
    }
available_ = error_code == 0;
}


void IpoptSolver::load(NLPModel& _model)
{
auto repn_capi = std::make_shared<IpoptSolverRepn_CAPI>(_model);
repn_capi->nlp->build();
model = &_model;
repn = std::dynamic_pointer_cast<IpoptSolverRepn>(repn_capi);
}

}
