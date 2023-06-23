#include <cstdio>
#include <cassert>
#include <cmath>
#include <unordered_map>

#include "coek/util/sequence.hpp"
#include "coek/autograd/asl_repn.hpp"
#include "coek/ast/value_terms.hpp"

// AMPL includes
#include "asl.h"
#include "asl_pfgh.h"
#include "getstub.h"
#ifdef range
#    undef range
#endif

namespace coek {

// TODO - Put this declaration in a header
void write_nl_problem(Model& model, const std::string& fname, std::map<size_t, size_t>& invvarmap,
                      std::map<size_t, size_t>& invconmap);

VariableRepn ASL_Repn::get_variable(size_t i) { return used_variables[varmap[i]]; }

ASL_Repn::ASL_Repn(Model& model) : NLPModelRepn(model)
{
    nx = 0;
    nf = 0;
    nc = 0;
    objval_called_with_current_x_ = false;
    conval_called_with_current_x_ = false;
    asl_ = 0;
    nnz_jac_g = 0;
    nnz_lag_h = 0;

    // First assume that we don't want to halt on error (default)
    nerror_ = (void*)new fint;
    *(fint*)nerror_ = 0;

    temp_directory = "/tmp/";
}

ASL_Repn::~ASL_Repn()
{
    free_asl();

    delete (fint*)nerror_;
    nerror_ = 0;
}

size_t ASL_Repn::num_variables() const { return nx; }

size_t ASL_Repn::num_objectives() const { return nf; }

size_t ASL_Repn::num_constraints() const { return nc; }

size_t ASL_Repn::num_nonzeros_Jacobian() const { return nnz_jac_g; }

size_t ASL_Repn::num_nonzeros_Hessian_Lagrangian() const { return nnz_lag_h; }

void ASL_Repn::set_variables(std::vector<double>& x) { set_variables(&(x[0]), x.size()); }

void ASL_Repn::set_variables(const double* x, size_t n)
{
    assert(n == currx.size());
    for (size_t i = 0; i < n; i++) currx[i] = x[i];

    objval_called_with_current_x_ = false;
    conval_called_with_current_x_ = false;
}

void ASL_Repn::get_J_nonzeros(std::vector<size_t>& jrow, std::vector<size_t>& jcol)
{
    ASL_pfgh* asl = asl_;

    jrow.resize(nnz_jac_g);
    jcol.resize(nnz_jac_g);
    size_t curr_nz = 0;
    for (size_t i : coek::range(nc)) {
        for (cgrad* cg = Cgrad[i]; cg; cg = cg->next) {
            jrow[cg->goff] = i;
            jcol[cg->goff] = cg->varno;
            curr_nz++;
        }
    }

    // The number of nonzeros should match what the value in the ASL data
    assert(curr_nz == nnz_jac_g);
}

void ASL_Repn::get_H_nonzeros(std::vector<size_t>& hrow, std::vector<size_t>& hcol)
{
    ASL_pfgh* asl = asl_;

    hrow.resize(nnz_lag_h);
    hcol.resize(nnz_lag_h);
    size_t curr_nz = 0;
    for (size_t i : coek::range(nx)) {
        for (size_t j = sputinfo->hcolstarts[i]; j < sputinfo->hcolstarts[i + 1]; j++) {
            hrow[curr_nz] = i;
            hcol[curr_nz] = sputinfo->hrownos[j];
            curr_nz++;
        }
    }

    // The number of nonzeros should match what the value in the ASL data
    assert(curr_nz == nnz_lag_h);
}

bool ASL_Repn::column_major_hessian() { return false; }

void ASL_Repn::print_equations(std::ostream& ostr) const { NLPModelRepn::print_equations(ostr); }

void ASL_Repn::print_values(std::ostream& ostr) const { NLPModelRepn::print_values(ostr); }

// TODO - Should COEK capture errors and return an error flag?
// TODO - Generalize caching for multiple objectives
double ASL_Repn::compute_f(size_t i)
{
    assert(i == 0);

    if (nf == 0) {
        objval_called_with_current_x_ = true;
        return 0;
    }
    else if (not objval_called_with_current_x_) {
        ASL_pfgh* asl = asl_;
        f_cache = objval(static_cast<int>(i), &(currx[0]), (fint*)nerror_);
        nerror_ok = check_asl_status(nerror_);
        if (nerror_ok) {
            objval_called_with_current_x_ = true;
        }
        else {
            objval_called_with_current_x_ = false;
            f_cache = nan("");
        }
    }

    return f_cache;
}

void ASL_Repn::compute_df(double& f, std::vector<double>& df, size_t i)
{
    assert(df.size() == nx);

    if (nf == 0) {
        f = 0.0;
        for (double& df_x : df) df_x = 0.0;
    }
    else {
        f = compute_f(i);

        ASL_pfgh* asl = asl_;
        objgrd(static_cast<int>(i), &(currx[0]), &(df[0]), (fint*)nerror_);
        nerror_ok = check_asl_status(nerror_);
    }
}

void ASL_Repn::compute_c(std::vector<double>& c)
{
    assert(c.size() == nc);

    if (not conval_called_with_current_x_) {
        ASL_pfgh* asl = asl_;
        conval(&(currx[0]), &(c_cache[0]), (fint*)nerror_);
        nerror_ok = check_asl_status(nerror_);
        if (nerror_ok)
            conval_called_with_current_x_ = true;
        else {
            conval_called_with_current_x_ = false;
            for (size_t i : coek::range(nc)) c[i] = nan("");
            return;
        }
    }
    for (size_t i : coek::range(nc)) c[i] = c_cache[i];
}

void ASL_Repn::compute_dc(std::vector<double>& dc, size_t i)
{
    assert(dc.size() == nx);

    ASL_pfgh* asl = asl_;
    congrd(i, &(currx[0]), &(dc[0]), (fint*)nerror_);
    nerror_ok = check_asl_status(nerror_);
    if (not nerror_ok) {
        for (size_t j : coek::indices(dc)) dc[j] = nan("");
    }
}

void ASL_Repn::compute_H(std::vector<double>& w, std::vector<double>& H)
{
    ASL_pfgh* asl = asl_;

    if (!objval_called_with_current_x_) {
        f_cache = compute_f(0);  // TODO - Extend API for multiple objectives
    }
    if (!conval_called_with_current_x_) {
        compute_c(c_cache);
    }
    sphes(&(H[0]), -1, &(w[0]), &(w[nf]));
}

void ASL_Repn::compute_J(std::vector<double>& J)
{
    ASL_pfgh* asl = asl_;

    jacval(&(currx[0]), &(J[0]), (fint*)nerror_);

    nerror_ok = check_asl_status(nerror_);
}

void ASL_Repn::initialize()
{
    //
    // Initialize the NLPModelRepn data
    //
    find_used_variables();

    //
    // Create the ASL object
    //
    alloc_asl();
    ASL_pfgh* asl = asl_;

    // Must have at least one continuous variable.
    assert(n_var > 0);
    // API does not support discrete variables
    assert((nbv == 0 && niv == 0 && nlvbi == 0 && nlvci == 0 && nlvoi == 0));
    // API does not support complementary constraints
    assert(n_cc == 0);
    // API does not support linear arc variables
    assert(nwv == 0);
    // API does not support nonlinear network constraints
    assert(nlnc == 0);
    // API does not support linear network constraints
    assert(lnc == 0);

    //
    // Set options in the asl structure
    //
    // allocate initial values for primal and dual if available
    want_xpi0 = 1 | 2;
    assert((want_xpi0 & 1) == 1 && (want_xpi0 & 2) == 2);

    call_hesset();

    //
    // Resize and initialize the ASL_Repn data
    //
    nx = static_cast<size_t>(n_var);
    nf = static_cast<size_t>(n_obj);
    nc = static_cast<size_t>(n_con);
    nnz_jac_g = static_cast<size_t>(nzc);

    currx.resize(nx);
    xlb.resize(nx);
    xub.resize(nx);
    c_cache.resize(nc);

    //
    // Setup initial values
    //
    for (size_t i : coek::range(nx)) {
        currx[i] = havex0[i] != 0 ? X0[i] : std::max(LUv[2 * i], std::min(LUv[2 * i + 1], 0.0));
        xlb[i] = LUv[2 * i];
        xub[i] = LUv[2 * i + 1];
    }
    set_variables(currx);
}

void ASL_Repn::reset(void)
{
    //
    // We re-generate the NL file and parse it, using new parameter values and new
    // fixed variables.
    //
    initialize();
}

bool ASL_Repn::check_asl_status(void* nerror)
{
    if (nerror == NULL || *((fint*)nerror) == 0) return true;

    std::cerr << "Error in an ASL evaluation." << std::endl;
    std::cerr << "nerror = " << *((fint*)nerror) << std::endl;
    return false;
}

void ASL_Repn::alloc_asl()
{
    free_asl();
    //
    // Create the ASL structure
    //
    ASL_pfgh* asl = reinterpret_cast<ASL_pfgh*>(ASL_alloc(ASL_read_pfgh));
    asl_ = asl;
    //
    // Create a temporary filename
    //
    std::string fname = temp_directory + "coek_XXXXXX";
    std::string tmp = mktemp(&fname[0]);
    if (tmp.size() == 0)
        throw std::runtime_error("Failure to create temporary file for ASL interface");
    fname += ".nl";
    //
    // Write the NL file
    //
    {
        std::map<size_t, size_t> invvarmap;  // ASL index -> Var ID
        std::map<size_t, size_t> invconmap;  // Ignore
        write_nl_problem(model, fname, invvarmap, invconmap);
        std::map<size_t, size_t> tmpvarmap;  // Var ID -> Coek index
        for (auto& it : used_variables) tmpvarmap[it.second->index] = it.first;
        for (auto& it : invvarmap)
            varmap[it.first] = tmpvarmap[it.second];  // ASL index -> Coek -> index
    }
    //
    // Read the NL file with the ASL library
    //
    return_nofile = 1;  // A hack to prevent the ASL from calling exit()
    FILE* nlfile = jac0dim(&(fname[0]), 16);
    if (!nlfile) {
        throw std::runtime_error(
            "ASL_Repn::alloc_asl - Cannot create ASL interface for model with no variables.");
    }
    //
    // allocate space for initial values
    //
    X0 = new real[n_var];
    havex0 = new char[n_var];
    //
    // Load model expressions
    //
    int retcode = pfgh_read(nlfile, ASL_return_read_err | ASL_findgroups);
    //
    // Close and remove the file
    //
    remove(fname.c_str());

    //
    // No errors, so return
    //
    if ((retcode == ASL_readerr_none) or (retcode == ASL_readerr_nonlin)) return;

    free_asl();
    if (retcode == ASL_readerr_nofile)
        throw std::runtime_error("ASL_Repn::alloc_asl - Error opening file " + fname);

    else if (retcode == ASL_readerr_argerr)
        throw std::runtime_error("ASL_Repn::alloc_asl - User-defined function with bad arguments");

    else if (retcode == ASL_readerr_unavail)
        throw std::runtime_error("ASL_Repn::alloc_asl - User-defined function not available");

    else if (retcode == ASL_readerr_corrupt)
        throw std::runtime_error("ASL_Repn::alloc_asl - Corrupt NL file");

    else if (retcode == ASL_readerr_bug)
        throw std::runtime_error("ASL_Repn::alloc_asl - Bug in ASL NL reader");

    else if (retcode == ASL_readerr_CLP)
        throw std::runtime_error(
            "ASL_Repn::alloc_asl - NL file contains a constraint without \"=\", \">=\", or "
            "\"<=\"");

    else
        throw std::runtime_error("ASL_Repn::alloc_asl - Unknown error in ASL file reader");
}

void ASL_Repn::free_asl()
{
    ASL_pfgh* asl = asl_;
    if (asl) {
        if (X0) {
            delete[] X0;
            X0 = 0;
        }

        if (havex0) {
            delete[] havex0;
            havex0 = 0;
        }

        ASL* asl_to_free = (ASL*)asl_;
        ASL_free(&asl_to_free);
        asl_ = 0;
    }
}

void ASL_Repn::call_hesset()
{
    ASL_pfgh* asl = asl_;

    if (n_obj == 0) {
        hesset(1, 0, 0, 0, nlc);
    }
    else {
        // TODO - rethink how this is setup, since the ASL data structures
        // are being optimized for a specific objective here.
        obj_no = 0;
        // see "changes" in solvers directory of ampl code...
        hesset(1, obj_no, 1, 0, nlc);
    }

    // find the nonzero structure for the hessian parameters to
    // sphsetup:
    int coeff_obj = 1;
    int mult_supplied = 1;  // multipliers will be supplied
    int uptri = 1;          // only need the upper triangular part
    nnz_lag_h = static_cast<size_t>(sphsetup(-1, coeff_obj, mult_supplied, uptri));
}

bool ASL_Repn::get_option(const std::string& option, std::string& value) const
{
if (option == "temp_directory") {
    value = temp_directory;
    return true;
    }
return false;
}

void ASL_Repn::set_option(const std::string& option, const std::string value)
{
if (option == "temp_directory")
    temp_directory = value;
}

}  // namespace coek
