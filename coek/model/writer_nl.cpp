#include <fstream>
#include <iomanip>
#include <map>
#ifdef WITH_CALIPER
#include <caliper/cali.h>
#else
#define CALI_CXX_MARK_FUNCTION
#define CALI_MARK_BEGIN(X)
#define CALI_MARK_END(X)
#endif
#ifdef WITH_FMTLIB
#include <fmt/core.h>
#include <fmt/os.h>
#endif

#include "../ast/visitor_fns.hpp"
#include "../ast/visitor.hpp"
#include "../ast/value_terms.hpp"
#include "../ast/constraint_terms.hpp"
#include "../ast/expr_terms.hpp"
#include "coek/api/expression.hpp"
#include "coek/api/objective.hpp"
#include "coek/api/constraint.hpp"
#include "coek/api/expression_visitor.hpp"
#include "coek/model/model.hpp"
#include "model_repn.hpp"

#define EPSILON 1e-12
#define ITYPE size_t


namespace coek {

void check_that_expression_variables_are_declared(Model& model, const std::map<size_t, Variable>& varobj);


namespace {

#if 0
struct UnknownVariable : public std::exception
{
    const char * what () const throw ()
        { return "Unknown variable encountered"; }
};
#endif



//
//
// Print expressions with ostream
//
//

void format(std::ostream& ostr, double value)
{ ostr << std::setprecision(16) << value; }

class PrintExpr : public Visitor
{
public:

    std::ostream& ostr;
    const std::unordered_map<ITYPE,ITYPE>& varmap;

public:

    PrintExpr(std::ostream& _ostr, const std::unordered_map<ITYPE,ITYPE>& _varmap)
        : ostr(_ostr), varmap(_varmap) {}

    void visit(ConstantTerm& arg);
    void visit(ParameterTerm& arg);
    void visit(IndexParameterTerm& arg);
    void visit(VariableTerm& arg);
#if __cpp_lib_variant
    void visit(VariableRefTerm& arg);
#endif
    void visit(IndexedVariableTerm& arg);
    void visit(MonomialTerm& arg);
    void visit(InequalityTerm& arg);
    void visit(EqualityTerm& arg);
    void visit(ObjectiveTerm& arg);
    void visit(NegateTerm& arg);
    void visit(PlusTerm& arg);
    void visit(TimesTerm& arg);
    void visit(DivideTerm& arg);
    void visit(AbsTerm& arg);
    void visit(CeilTerm& arg);
    void visit(FloorTerm& arg);
    void visit(ExpTerm& arg);
    void visit(LogTerm& arg);
    void visit(Log10Term& arg);
    void visit(SqrtTerm& arg);
    void visit(SinTerm& arg);
    void visit(CosTerm& arg);
    void visit(TanTerm& arg);
    void visit(SinhTerm& arg);
    void visit(CoshTerm& arg);
    void visit(TanhTerm& arg);
    void visit(ASinTerm& arg);
    void visit(ACosTerm& arg);
    void visit(ATanTerm& arg);
    void visit(ASinhTerm& arg);
    void visit(ACoshTerm& arg);
    void visit(ATanhTerm& arg);
    void visit(PowTerm& arg);
};


void PrintExpr::visit(ConstantTerm& arg)
{
ostr << "n";
format(ostr, arg.value);
ostr << '\n';
}

void PrintExpr::visit(ParameterTerm& arg)
{
ostr << "n";
format(ostr, arg.value);
ostr << '\n';
}

// GCOVR_EXCL_START
void PrintExpr::visit(IndexParameterTerm& )
{ throw std::runtime_error("Encountered an index parameter when printing an expression.  This error should have been caught earlier!"); }
// GCOVR_EXCL_STOP

void PrintExpr::visit(VariableTerm& arg)
{ 
if (arg.fixed)
    ostr << "n" << arg.value->eval() << '\n';
else
    ostr << "v" << varmap.at(arg.index) << '\n';
}

#if __cpp_lib_variant
void PrintExpr::visit(VariableRefTerm& )
{ throw std::runtime_error("Cannot write an NL file using an abstract expression!"); }
#endif

void PrintExpr::visit(IndexedVariableTerm& arg)
{ ostr << "v" << varmap.at(arg.index) << '\n'; }

void PrintExpr::visit(MonomialTerm& arg)
{
ostr << "o2" << '\n';
ostr << "n";
format(ostr, arg.coef);
ostr << '\n';
if (arg.var->fixed)
    ostr << "n" << arg.var->value->eval() << '\n';
else
    ostr << "v" << varmap.at(arg.var->index) << '\n';
}

// GCOVR_EXCL_START
void PrintExpr::visit(InequalityTerm& )
{ throw std::runtime_error("Encountered an inequality constraint when printing an expression.  This error should have been caught earlier!"); }

void PrintExpr::visit(EqualityTerm& )
{ throw std::runtime_error("Encountered an equality constraint when printing an expression.  This error should have been caught earlier!"); }

void PrintExpr::visit(ObjectiveTerm& )
{ throw std::runtime_error("Encountered an objective when printing an expression.  This error should have been caught earlier!"); }
// GCOVR_EXCL_STOP

void PrintExpr::visit(NegateTerm& arg)
{
ostr << "o16\n";
arg.body->accept(*this);
}

void PrintExpr::visit(PlusTerm& arg)
{
if (arg.n == 2)
    ostr << "o0\n";
else
    ostr << "o54\n" << arg.n << '\n';
std::vector<expr_pointer_t>& vec = *(arg.data);
for (size_t i=0; i<arg.n; ++i)
    vec[i]->accept(*this);
}

void PrintExpr::visit(TimesTerm& arg)
{
ostr << "o2\n";
arg.lhs->accept(*this);
arg.rhs->accept(*this);
}

void PrintExpr::visit(DivideTerm& arg)
{
ostr << "o3\n";
arg.lhs->accept(*this);
arg.rhs->accept(*this);
}

#define PrintExpr_FN(FN, TERM)\
void PrintExpr::visit(TERM& arg)\
{\
ostr << #FN << '\n';\
arg.body->accept(*this);\
}

PrintExpr_FN(o15, AbsTerm)
PrintExpr_FN(o14, CeilTerm)
PrintExpr_FN(o13, FloorTerm)
PrintExpr_FN(o44, ExpTerm)
PrintExpr_FN(o43, LogTerm)
PrintExpr_FN(o42, Log10Term)
PrintExpr_FN(o39, SqrtTerm)
PrintExpr_FN(o41, SinTerm)
PrintExpr_FN(o46, CosTerm)
PrintExpr_FN(o38, TanTerm)
PrintExpr_FN(o40, SinhTerm)
PrintExpr_FN(o45, CoshTerm)
PrintExpr_FN(o37, TanhTerm)
PrintExpr_FN(o51, ASinTerm)
PrintExpr_FN(o53, ACosTerm)
PrintExpr_FN(o49, ATanTerm)
PrintExpr_FN(o50, ASinhTerm)
PrintExpr_FN(o52, ACoshTerm)
PrintExpr_FN(o47, ATanhTerm)

void PrintExpr::visit(PowTerm& arg)
{
ostr << "o5\n";
arg.lhs->accept(*this);
arg.rhs->accept(*this);
}


//
//
// Print expressions with fmtlib
//
//

#ifdef WITH_FMTLIB

class PrintExprFmtlib : public Visitor
{
public:

    fmt::ostream& ostr;
    const std::unordered_map<ITYPE,ITYPE>& varmap;

public:

    PrintExprFmtlib(fmt::ostream& _ostr, const std::unordered_map<ITYPE,ITYPE>& _varmap)
        : ostr(_ostr), varmap(_varmap) {}

    void visit(ConstantTerm& arg);
    void visit(ParameterTerm& arg);
    void visit(IndexParameterTerm& arg);
    void visit(VariableTerm& arg);
#if __cpp_lib_variant
    void visit(VariableRefTerm& arg);
#endif
    void visit(IndexedVariableTerm& arg);
    void visit(MonomialTerm& arg);
    void visit(InequalityTerm& arg);
    void visit(EqualityTerm& arg);
    void visit(ObjectiveTerm& arg);
    void visit(NegateTerm& arg);
    void visit(PlusTerm& arg);
    void visit(TimesTerm& arg);
    void visit(DivideTerm& arg);
    void visit(AbsTerm& arg);
    void visit(CeilTerm& arg);
    void visit(FloorTerm& arg);
    void visit(ExpTerm& arg);
    void visit(LogTerm& arg);
    void visit(Log10Term& arg);
    void visit(SqrtTerm& arg);
    void visit(SinTerm& arg);
    void visit(CosTerm& arg);
    void visit(TanTerm& arg);
    void visit(SinhTerm& arg);
    void visit(CoshTerm& arg);
    void visit(TanhTerm& arg);
    void visit(ASinTerm& arg);
    void visit(ACosTerm& arg);
    void visit(ATanTerm& arg);
    void visit(ASinhTerm& arg);
    void visit(ACoshTerm& arg);
    void visit(ATanhTerm& arg);
    void visit(PowTerm& arg);
};


void PrintExprFmtlib::visit(ConstantTerm& arg)
{ ostr.print("n{}\n", arg.value); }

void PrintExprFmtlib::visit(ParameterTerm& arg)
{ ostr.print("n{}\n", arg.value); }

// GCOVR_EXCL_START
void PrintExprFmtlib::visit(IndexParameterTerm& )
{ throw std::runtime_error("Encountered an index parameter when printing an expression.  This error should have been caught earlier!"); }
// GCOVR_EXCL_STOP

void PrintExprFmtlib::visit(VariableTerm& arg)
{ 
if (arg.fixed)
    ostr.print("n{}\n", arg.value->eval());
else
    ostr.print("v{}\n", varmap.at(arg.index));
}


#if __cpp_lib_variant
void PrintExprFmtlib::visit(VariableRefTerm& )
{ throw std::runtime_error("Cannot write an NL file using an abstract expression!"); }
#endif

void PrintExprFmtlib::visit(IndexedVariableTerm& arg)
{ ostr.print("v{}\n", varmap.at(arg.index)); }

void PrintExprFmtlib::visit(MonomialTerm& arg)
{
ostr.print("o2\nn{}\n", arg.coef);

if (arg.var->fixed)
    ostr.print("n{}\n", arg.var->value->eval());
else
    ostr.print("v{}\n", varmap.at(arg.var->index));
}

// GCOVR_EXCL_START
void PrintExprFmtlib::visit(InequalityTerm& )
{ throw std::runtime_error("Encountered an inequality constraint when printing an expression.  This error should have been caught earlier!"); }

void PrintExprFmtlib::visit(EqualityTerm& )
{ throw std::runtime_error("Encountered an equality constraint when printing an expression.  This error should have been caught earlier!"); }

void PrintExprFmtlib::visit(ObjectiveTerm& )
{ throw std::runtime_error("Encountered an objective when printing an expression.  This error should have been caught earlier!"); }
// GCOVR_EXCL_STOP

void PrintExprFmtlib::visit(NegateTerm& arg)
{
ostr.print("o16\n");
arg.body->accept(*this);
}

void PrintExprFmtlib::visit(PlusTerm& arg)
{
if (arg.n == 2)
    ostr.print("o0\n");
else
    ostr.print("o54\n{}\n", arg.n);
std::vector<expr_pointer_t>& vec = *(arg.data);
for (size_t i=0; i<arg.n; ++i)
    vec[i]->accept(*this);
}

void PrintExprFmtlib::visit(TimesTerm& arg)
{
ostr.print("o2\n");
arg.lhs->accept(*this);
arg.rhs->accept(*this);
}

void PrintExprFmtlib::visit(DivideTerm& arg)
{
ostr.print("o3\n");
arg.lhs->accept(*this);
arg.rhs->accept(*this);
}

#define PrintExprFmt_FN(FN, TERM)\
void PrintExprFmtlib::visit(TERM& arg)\
{\
ostr.print( #FN "\n" );\
arg.body->accept(*this);\
}

PrintExprFmt_FN(o15, AbsTerm)
PrintExprFmt_FN(o14, CeilTerm)
PrintExprFmt_FN(o13, FloorTerm)
PrintExprFmt_FN(o44, ExpTerm)
PrintExprFmt_FN(o43, LogTerm)
PrintExprFmt_FN(o42, Log10Term)
PrintExprFmt_FN(o39, SqrtTerm)
PrintExprFmt_FN(o41, SinTerm)
PrintExprFmt_FN(o46, CosTerm)
PrintExprFmt_FN(o38, TanTerm)
PrintExprFmt_FN(o40, SinhTerm)
PrintExprFmt_FN(o45, CoshTerm)
PrintExprFmt_FN(o37, TanhTerm)
PrintExprFmt_FN(o51, ASinTerm)
PrintExprFmt_FN(o53, ACosTerm)
PrintExprFmt_FN(o49, ATanTerm)
PrintExprFmt_FN(o50, ASinhTerm)
PrintExprFmt_FN(o52, ACoshTerm)
PrintExprFmt_FN(o47, ATanhTerm)

void PrintExprFmtlib::visit(PowTerm& arg)
{
ostr.print("o5\n");
arg.lhs->accept(*this);
arg.rhs->accept(*this);
}
#endif // WITH_FMTLIB


void print_expr(std::ostream& ostr, const MutableNLPExpr& repn, const std::unordered_map<ITYPE,ITYPE>& varmap, bool objective=false)
{
bool nonlinear = not repn.nonlinear.is_constant();
bool quadratic = repn.quadratic_coefs.size() > 0;

double cval = repn.constval.value();
if (not nonlinear)
    cval += repn.nonlinear.value();

std::map<std::pair<size_t,size_t>,double> term;
if (quadratic) {
    for (size_t i=0; i<repn.quadratic_coefs.size(); ++i) {
        size_t lhs = varmap.at(repn.quadratic_lvars[i]->index);
        size_t rhs = varmap.at(repn.quadratic_rvars[i]->index);
        if (rhs < lhs)
            std::swap(lhs,rhs);
        auto key = std::pair<size_t,size_t>(lhs, rhs);
        auto it = term.find(key);
        if (it != term.end())
            it->second += repn.quadratic_coefs[i].value();
        else
            term[key] = repn.quadratic_coefs[i].value();
        }
    }

// Compute the number of terms in the sum
size_t ctr=0;
if (objective and (fabs(cval) > EPSILON))
    ++ctr;
if (nonlinear)
    ++ctr;
if (quadratic)
    ctr += term.size();

// Write the sum header
if (ctr == 0)
    return;
else if (ctr == 2)
    ostr << "o0\n";
else if (ctr > 2)
    ostr << "o54\n" << ctr << '\n';

// Write terms in the sum
if (quadratic) {
    for (auto it=term.begin(); it != term.end(); ++it) {
        double coef = it->second;
        if (coef != 1) {
            ostr << "o2\n";
            ostr << "n";
            format(ostr, coef);
            ostr << '\n';
            }
        ostr << "o2\n";
        ostr << "v" << it->first.first << '\n';
        ostr << "v" << it->first.second << '\n';
        }
    }
if (nonlinear) {
    PrintExpr visitor(ostr, varmap);
    repn.nonlinear.repn->accept(visitor);
    }
if (objective and (fabs(cval) > EPSILON)) {
    ostr << "n";
    format(ostr, cval);
    ostr << '\n';
    }
}

#ifdef WITH_FMTLIB
void print_expr(fmt::ostream& ostr, const MutableNLPExpr& repn, const std::unordered_map<ITYPE,ITYPE>& varmap, bool objective=false)
{
bool nonlinear = not repn.nonlinear.is_constant();
bool quadratic = repn.quadratic_coefs.size() > 0;

double cval = repn.constval.value();
if (not nonlinear)
    cval += repn.nonlinear.value();

std::map<std::pair<ITYPE,ITYPE>,double> term;
if (quadratic) {
    for (size_t i=0; i<repn.quadratic_coefs.size(); ++i) {
        ITYPE lhs = varmap.at(repn.quadratic_lvars[i]->index);
        ITYPE rhs = varmap.at(repn.quadratic_rvars[i]->index);
        if (rhs < lhs)
            std::swap(lhs,rhs);
        auto key = std::pair<ITYPE,ITYPE>(lhs, rhs);

        auto it = term.find(key);
        if (it != term.end())
            it->second += repn.quadratic_coefs[i].value();
        else
            term[key] = repn.quadratic_coefs[i].value();
/*
        if (auto it{ term.find(key) };  it != term.end() )
            it->second += repn.quadratic_coefs[i].value();
        else
            term[key] = repn.quadratic_coefs[i].value();
*/
        }
    }

// Compute the number of terms in the sum
size_t ctr=0;
if (objective and (fabs(cval) > EPSILON))
    ++ctr;
if (nonlinear)
    ++ctr;
if (quadratic)
    ctr += term.size();

// Write the sum header
if (ctr == 0)
    return;
else if (ctr == 2)
    ostr.print("o0\n");
else if (ctr > 2)
    ostr.print("o54\n{}\n", ctr); 

// Write terms in the sum
if (quadratic) {
    for (auto it=term.begin(); it != term.end(); ++it) {
        double coef = it->second;
        if (coef != 1) {
            ostr.print("o2\nn{}\n", coef);      // FORMAT
            }
        ostr.print("o2\nv{}\nv{}\n", it->first.first, it->first.second);
        }
    }
if (nonlinear) {
    PrintExprFmtlib visitor(ostr, varmap);
    repn.nonlinear.repn->accept(visitor);
    }
if (objective and (fabs(cval) > EPSILON)) {
    ostr.print("n{}\n", cval);      // FORMAT
    }
}
#endif
}


class NLWriter
{
public:

    //
    // Process Model to Create NL Header
    //
    std::map<size_t,Variable> varobj;
    std::set<size_t> vars;
    std::set<size_t> nonlinear_vars_obj;
    std::set<size_t> nonlinear_vars_con;
    size_t num_inequalities;
    size_t num_ranges;
    size_t num_equalities;
    size_t nonl_objectives;
    size_t nonl_constraints;
    size_t nonlinear_vars_both;

    std::set<size_t> linear_vars;
    size_t num_linear_binary_vars;
    size_t num_linear_integer_vars;
    size_t num_nonlinear_obj_int_vars;
    size_t num_nonlinear_con_int_vars;
    size_t num_nonlinear_both_int_vars;

    size_t nnz_Jacobian;
    size_t nnz_gradient;

    std::vector<MutableNLPExpr> o_expr;
    std::vector<MutableNLPExpr> c_expr;
    std::vector<int> r;
    std::vector<double> rval;

    std::unordered_map<ITYPE,ITYPE> varmap;
    std::vector<std::set<size_t>> k_count;
    std::vector<std::map<size_t,double>> G;
    std::vector<std::map<size_t,double>> J;

    NLWriter()
    {
    num_inequalities=0;
    num_ranges=0;
    num_equalities=0;
    nonl_objectives=0;
    nonl_constraints=0;
    nonlinear_vars_both=0;

    num_linear_binary_vars=0;
    num_linear_integer_vars=0;
    num_nonlinear_obj_int_vars=0;
    num_nonlinear_con_int_vars=0;
    num_nonlinear_both_int_vars=0;

    nnz_Jacobian=0;
    nnz_gradient=0;
    }

    void collect_nl_data(Model& model, std::map<size_t,size_t>& invvarmap, std::map<size_t,size_t>& invconmap);

    void write_ostream(Model& model, std::string& fname);
    void write_fmtlib(Model& model, std::string& fname);
};


// TODO - Reorder constraints to have nonlinear before linear
// TODO - Reorder variables per the AMPL solver hookup logic
void NLWriter::collect_nl_data(Model& model, std::map<size_t,size_t>& invvarmap, std::map<size_t,size_t>& invconmap)
{
o_expr.resize(model.repn->objectives.size());
c_expr.resize(model.repn->constraints.size());
r.resize(model.repn->constraints.size());
rval.resize(2*model.repn->constraints.size());

CALI_CXX_MARK_FUNCTION;

CALI_MARK_BEGIN("Prepare Objective Expressions");
if (model.repn->objectives.size() == 0) {
    throw std::runtime_error("Error writing NL file: No objectives specified!");
    }
if (model.repn->objectives.size() > 1) {
    throw std::runtime_error("Error writing NL file: More than one objective defined!");
    }

// Objectives
try {
    {
    size_t ctr=0;
    for (auto it=model.repn->objectives.begin(); it != model.repn->objectives.end(); ++it, ++ctr) {
        o_expr[ctr].collect_terms(*it);
        if ((o_expr[ctr].quadratic_coefs.size() > 0) or (not o_expr[ctr].nonlinear.is_constant()))
            ++nonl_objectives;
        for (auto it=o_expr[ctr].linear_vars.begin(); it != o_expr[ctr].linear_vars.end(); ++it) {
            auto var = *it;
            linear_vars.insert(var->index);
            vars.insert(var->index);
            varobj[var->index] = var;
            }
        for (auto it=o_expr[ctr].quadratic_lvars.begin(); it != o_expr[ctr].quadratic_lvars.end(); ++it) {
            auto var = *it;
            nonlinear_vars_obj.insert(var->index);
            vars.insert(var->index);
            varobj[var->index] = var;
            }
        for (auto it=o_expr[ctr].quadratic_rvars.begin(); it != o_expr[ctr].quadratic_rvars.end(); ++it) {
            auto var = *it;
            nonlinear_vars_obj.insert(var->index);
            vars.insert(var->index);
            varobj[var->index] = var;
            }
        for (auto it=o_expr[ctr].nonlinear_vars.begin(); it != o_expr[ctr].nonlinear_vars.end(); ++it) {
            auto var = *it;
            nonlinear_vars_obj.insert(var->index);
            vars.insert(var->index);
            varobj[var->index] = var;
            }
        }
    }
    CALI_MARK_END("Prepare Objective Expressions");

    // Since we have just one objective, the # of variables is the # of nonzeros in gradients
    nnz_gradient=vars.size();

    CALI_MARK_BEGIN("Prepare Constraint Expressions");
    // Constraints
    {
    size_t ctr=0;
    for (auto jt=model.repn->constraints.begin(); jt != model.repn->constraints.end(); ++jt, ++ctr) {
        auto& Expr = c_expr[ctr];
        auto& Con = *jt;

        invconmap[ctr] = Con.id();

        Expr.collect_terms(Con);

        double bodyconst = Expr.constval.value();
        if (Con.is_inequality()) {
            if (Con.repn->lower and Con.repn->upper) {
                double lower = Con.repn->lower->eval() - bodyconst;
                double upper = Con.repn->upper->eval() - bodyconst;
                if (fabs(upper-lower) < EPSILON) {
                    ++num_equalities;
                    r[ctr] = 4;
                    rval[2*ctr] = lower;
                    }
                else {
                    ++num_inequalities;
                    ++num_ranges;
                    r[ctr] = 0;
                    rval[2*ctr] = lower;
                    rval[2*ctr+1] = upper;
                    }
                }
            else if (Con.repn->lower) {
                ++num_inequalities;
                r[ctr] = 2;
                rval[2*ctr] = Con.repn->lower->eval() - bodyconst;
                }
            else if (Con.repn->upper) {
                ++num_inequalities;
                r[ctr] = 1;
                rval[2*ctr] = Con.repn->upper->eval() - bodyconst;
                }
            else {
                // TODO - test unbounded expressions like this
                ++num_inequalities;
                r[ctr] = 3;
                }
            }
        else {
            ++num_equalities;
            r[ctr] = 4;
            rval[2*ctr] = Con.repn->lower->eval() - bodyconst;
            }
        if ((Expr.quadratic_coefs.size() > 0) or (not Expr.nonlinear.is_constant()))
            ++nonl_constraints;

        std::set<ITYPE> curr_vars;

        for (auto it=Expr.linear_vars.begin(); it != Expr.linear_vars.end(); ++it) {
            auto var = *it;
            auto index = var->index;
            linear_vars.insert(index);
            vars.insert(index);
            varobj[index] = var;
            curr_vars.insert(index);
            }
        for (auto it=Expr.quadratic_lvars.begin(); it != Expr.quadratic_lvars.end(); ++it) {
            auto var = *it;
            auto index = var->index;
            nonlinear_vars_con.insert(index);
            vars.insert(index);
            varobj[index] = var;
            curr_vars.insert(index);
            }
        for (auto it=Expr.quadratic_rvars.begin(); it != Expr.quadratic_rvars.end(); ++it) {
            auto var = *it;
            auto index = var->index;
            nonlinear_vars_con.insert(index);
            vars.insert(index);
            varobj[index] = var;
            curr_vars.insert(index);
            }
        for (auto it=Expr.nonlinear_vars.begin(); it != Expr.nonlinear_vars.end(); ++it) {
            auto var = *it;
            auto index = var->index;
            nonlinear_vars_con.insert(index);
            vars.insert(index);
            varobj[index] = var;
            curr_vars.insert(index);
            }

        // Add Jacobian terms for each constraint
        nnz_Jacobian += curr_vars.size();
        }
    }
    CALI_MARK_END("Prepare Constraint Expressions");

    check_that_expression_variables_are_declared(model, varobj);
    }
catch (std::exception& e)
    {
    throw std::runtime_error(std::string("Error writing NL file: ") + e.what());
    }

CALI_MARK_BEGIN("Misc NL");
for (auto it=linear_vars.begin(); it != linear_vars.end(); ++it) {
    auto& var = varobj[*it];
    if (var.is_binary())
        ++num_linear_binary_vars;
    else if (var.is_integer())
        ++num_linear_integer_vars;
    }

for (auto it=nonlinear_vars_obj.begin(); it != nonlinear_vars_obj.end(); ++it) {
    auto& var = varobj[*it];
    bool flag = var.is_binary() or var.is_integer();
    if (flag)
       ++num_nonlinear_obj_int_vars;
    if (nonlinear_vars_con.find(*it) != nonlinear_vars_con.end()) {
        ++nonlinear_vars_both;
        if (flag)
           ++num_nonlinear_both_int_vars;
        }
    }
for (auto it=nonlinear_vars_con.begin(); it != nonlinear_vars_con.end(); ++it) {
    auto& var = varobj[*it];
    if (var.is_binary() or var.is_integer())
       ++num_nonlinear_con_int_vars;
    }

// Map Variable index to NL variable ID (0 ... n_vars-1)
{
size_t ctr = 0;
for (auto it=vars.begin(); it != vars.end(); ++it) {
    invvarmap[ctr] = *it;
    varmap[*it] = ctr;
    ++ctr;
    }
CALI_MARK_END("Misc NL");

// GCOVR_EXCL_START
if (vars.size() != varmap.size())
    throw std::runtime_error("Error writing NL file: Variables with duplicate index values detected!");
// GCOVR_EXCL_STOP
}

// Compute linear Jacobian and Gradient values
CALI_MARK_BEGIN("Compute Jacobian/Gradient");
k_count.resize(vars.size());
G.resize(o_expr.size());
J.resize(c_expr.size());

{
size_t ctr=0;
for (auto it=o_expr.begin(); it != o_expr.end(); ++it, ++ctr) {
    for (auto jt=it->quadratic_lvars.begin(); jt!= it->quadratic_lvars.end(); ++jt) {
        G[ctr][ varmap[(*jt)->index] ] = 0;
        }
    for (auto jt=it->quadratic_rvars.begin(); jt!= it->quadratic_rvars.end(); ++jt) {
        G[ctr][ varmap[(*jt)->index] ] = 0;
        }
    for (auto jt=it->nonlinear_vars.begin(); jt!= it->nonlinear_vars.end(); ++jt) {
        G[ctr][ varmap[(*jt)->index] ] = 0;
        }
    for (size_t j=0; j<it->linear_coefs.size(); ++j) {
        auto index = varmap[it->linear_vars[j]->index];
        auto jt = G[ctr].find(index);
        if (jt != G[ctr].end())
            jt->second += it->linear_coefs[j].value();
        else
            G[ctr][index] = it->linear_coefs[j].value();
/*
        if (auto jt{ G[ctr].find(index) };  jt != G[ctr].end() )
            jt->second += it->linear_coefs[j].value();
        else
            G[ctr][index] = it->linear_coefs[j].value();
*/
        }
    }
}
{
size_t ctr=0;
for (auto it=c_expr.begin(); it != c_expr.end(); ++it, ++ctr) {
    for (auto jt=it->quadratic_lvars.begin(); jt!= it->quadratic_lvars.end(); ++jt) {
        size_t index = varmap[(*jt)->index];
        k_count[ index ].insert(ctr);
        J[ctr][ index ] = 0;
        }
    for (auto jt=it->quadratic_rvars.begin(); jt!= it->quadratic_rvars.end(); ++jt) {
        size_t index = varmap[(*jt)->index];
        k_count[ index ].insert(ctr);
        J[ctr][ index ] = 0;
        }
    for (auto jt=it->nonlinear_vars.begin(); jt!= it->nonlinear_vars.end(); ++jt) {
        size_t index = varmap[(*jt)->index];
        k_count[ index ].insert(ctr);
        J[ctr][ index ] = 0;
        }
    for (size_t j=0; j<it->linear_coefs.size(); ++j) {
        size_t index = varmap[it->linear_vars[j]->index];
        auto jt = J[ctr].find(index);
        if (jt != J[ctr].end())
            jt->second += it->linear_coefs[j].value();
        else {
            k_count[ index ].insert(ctr);
            J[ctr][index] = it->linear_coefs[j].value();
            }
/*
        if (auto jt{ J[ctr].find(index) };  jt != J[ctr].end() )
            jt->second += it->linear_coefs[j].value();
        else {
            k_count[ index ].insert(ctr);
            J[ctr][index] = it->linear_coefs[j].value();
            }
*/
        }
    }
}
CALI_MARK_END("Compute Jacobian/Gradient");
}


void NLWriter::write_ostream(Model& model, std::string& fname)
{
std::ofstream ostr(fname);

size_t ctr=0;
try {
    //
    // Write NL Header
    //
    // This API seems poorly documented.  Is the 2005 paper the defining reference?  Pyomo writes a header that doesn't conform to it...
    //
    ostr << "g3 1 1 0 # unnamed problem generated by COEK\n";
    ostr << " " << vars.size() << " " << (num_inequalities+num_equalities) << " 1 " << num_ranges << " " << num_equalities << " 0 # vars, constraints, objectives, ranges, eqns, lcons\n";
    ostr << " " << nonl_constraints << " " << nonl_objectives << " # nonlinear constraints, objectives\n";
    ostr << " 0 0 # network constraints: nonlinear, linear\n";
    ostr << " " << nonlinear_vars_con.size() << " " << nonlinear_vars_obj.size() << " " << nonlinear_vars_both << " # nonlinear vars in constraints, objectives, both\n";
    ostr << " 0 0 0 1 # linear network variables; functions; arith, flags\n";
    ostr << " " << num_linear_binary_vars << " " << num_linear_integer_vars << " " << num_nonlinear_both_int_vars << " " << num_nonlinear_con_int_vars << " " << num_nonlinear_obj_int_vars << " # discrete variables: binary, integer, nonlinear (b,c,o)\n";
    ostr << " " << nnz_Jacobian << " " << nnz_gradient << " # nonzeros in Jacobian, gradients\n";
    ostr << " 0 0 # max name lengths: constraints, variables\n";
    ostr << " 0 0 0 0 0 # common exprs: b,c,o,c1,o1\n";

    //
    // "C" section - nonlinear constraint segments
    //
    ctr = 0;
    for (auto it=c_expr.begin(); it != c_expr.end(); ++it, ++ctr) {
        if ((not it->nonlinear.is_constant()) or (it->quadratic_coefs.size() > 0)) {
            ostr << "C" << ctr << '\n';
            print_expr(ostr, *it, varmap);
            }
        else {
            ostr << "C" << ctr << '\n';
            ostr << "n0\n";
            }
        }

    //
    // "O" section - nonlinear objective segments
    //
    ctr=0;
    for (auto it=o_expr.begin(); it != o_expr.end(); ++it, ++ctr) {
        bool sense = model.repn->objectives[ctr].sense();
        if (sense == Model::minimize)
            ostr << "O" << ctr << " 0\n";
        else
            ostr << "O" << ctr << " 1\n";
        if ((not it->nonlinear.is_constant()) or (it->quadratic_coefs.size() > 0)) {
            print_expr(ostr, *it, varmap, true);
            }
        else {
            ostr << "n" << it->constval.value() << '\n';
            }
        }

    //
    // "x" section - primal initial values
    //
    {
    std::map<size_t, double> values;
    ctr=0;
    for (auto it=vars.begin(); it != vars.end(); ++it, ++ctr) {
        auto tmp = varobj[*it].value();
        if (not std::isnan(tmp))
            values[ctr] = tmp;
        }
    if (values.size() > 0) {
        ostr << "x" << values.size() << '\n';
        for (auto it=values.begin(); it != values.end(); ++it)
            ostr << it->first << " " << it->second << '\n';
        }
    }

    //
    // "r" section - bounds on constraints
    //

    if (model.repn->constraints.size() > 0) {
        ostr << "r\n";
        ctr = 0;
        for (auto it=model.repn->constraints.begin(); it != model.repn->constraints.end(); ++it, ++ctr) {
            switch (r[ctr]) {
                case 0:
                    ostr << "0 ";
                    format(ostr, rval[2*ctr]);
                    ostr << " ";
                    format(ostr, rval[2*ctr+1]);
                    break;
                case 1:
                    ostr << "1 ";
                    format(ostr, rval[2*ctr]);
                    break;
                case 2:
                    ostr << "2 ";
                    format(ostr, rval[2*ctr]);
                    break;
                // GCOVR_EXCL_START
                case 3:
                    ostr << "3";
                    break;
                // GCOVR_EXCL_STOP
                case 4:
                    ostr << "4 ";
                    format(ostr, rval[2*ctr]);
                    break;
                };
            ostr << '\n';
            }
        }

    //
    // "b" section - bounds on variables
    //
    ostr << "b\n";
    for (auto it=vars.begin(); it != vars.end(); ++it) {
        auto var = varobj[*it];
        double lb = var.lower();
        double ub = var.upper();
        if (lb == -COEK_INFINITY) {
            if (ub == COEK_INFINITY) {
                ostr << "3\n";
                }
            else {
                ostr << "1 ";
                format(ostr, ub);
                ostr << '\n';
                }
            }
        else {
            if (ub == COEK_INFINITY) {
                ostr << "2 ";
                format(ostr, lb);
                ostr << '\n';
                }
            else {
                if (fabs(ub-lb) < EPSILON) {
                    ostr << "4 ";
                    format(ostr, lb);
                    }
                else {
                    ostr << "0 ";
                    format(ostr, lb);
                    ostr << " ";
                    format(ostr, ub);
                    }
                ostr << '\n';
                }
            }
        }

    //
    // "k" section - Jacobian column counts
    //
    ostr << "k" << (k_count.size()-1) << '\n';
    ctr = 0;
    for (size_t i=0; i<(k_count.size()-1); ++i) {
        ctr += k_count[i].size();
        ostr << ctr << '\n';
        }

    //
    // "J" section - Jacobian sparsity, linear terms
    //
    for (size_t i=0; i<J.size(); ++i) {
        if (J[i].size() == 0) continue;
        ostr << "J" << i << " " << J[i].size() << '\n';
        for (auto it=J[i].begin(); it!=J[i].end(); ++it) {
            ostr << it->first << " " << it->second << '\n';
            }
        }

    //
    // "G" section - Gradient sparsity, linear terms
    //
    for (size_t i=0; i<G.size(); ++i) {
        if (G[i].size() == 0) continue;
        ostr << "G" << i << " " << G[i].size() << '\n';
        for (auto it=G[i].begin(); it!=G[i].end(); ++it) {
            ostr << it->first << " " << it->second << '\n';
            }
        }
    }
// GCOVR_EXCL_START
catch (std::exception& e)
    {
    throw std::runtime_error(std::string("Error writing NL file: ") + e.what());
    }
// GCOVR_EXCL_STOP


ostr.close();
}


#ifdef WITH_FMTLIB
void NLWriter::write_fmtlib(Model& model, std::string& fname)
{
auto ostr = fmt::output_file(fname, fmt::file::WRONLY | fmt::file::CREATE | FMT_POSIX(O_TRUNC));

//
// Write NL Header
//
// This API seems poorly documented.  Is the 2005 paper the defining reference?  Pyomo writes a header that doesn't conform to it...
//
ostr.print("g3 1 1 0 # unnamed problem generated by COEK\n");
ostr.print(" {} {} 1 {} {} 0 # vars, constraints, objectives, ranges, eqns, lcons\n", vars.size(), (num_inequalities+num_equalities), num_ranges, num_equalities);
ostr.print(" {} {} # nonlinear constraints, objectives\n", nonl_constraints, nonl_objectives);
ostr.print(" 0 0 # network constraints: nonlinear, linear\n");
ostr.print(" {} {} {} # nonlinear vars in constraints, objectives, both\n", nonlinear_vars_con.size(), nonlinear_vars_obj.size(), nonlinear_vars_both);
ostr.print(" 0 0 0 1 # linear network variables; functions; arith, flags\n");
ostr.print(" {} {} {} {} {} # discrete variables: binary, integer, nonlinear (b,c,o)\n", num_linear_binary_vars, num_linear_integer_vars, num_nonlinear_both_int_vars, num_nonlinear_con_int_vars, num_nonlinear_obj_int_vars);
ostr.print(" {} {} # nonzeros in Jacobian, gradients\n", nnz_Jacobian, nnz_gradient);
ostr.print(" 0 0 # max name lengths: constraints, variables\n");
ostr.print(" 0 0 0 0 0 # common exprs: b,c,o,c1,o1\n");

//
// "C" section - nonlinear constraint segments
//
CALI_MARK_BEGIN("C");
{
int ctr = 0;
for (auto it=c_expr.begin(); it != c_expr.end(); ++it, ++ctr) {
    if ((not it->nonlinear.is_constant()) or (it->quadratic_coefs.size() > 0)) {
        ostr.print("C{}\n", ctr);
        print_expr(ostr, *it, varmap);
        }
    else {
        ostr.print("C{}\nn0\n", ctr);
        }
    }
}
CALI_MARK_END("C");

//
// "O" section - nonlinear objective segments
//
CALI_MARK_BEGIN("O");
{
size_t ctr=0;
for (auto it=o_expr.begin(); it != o_expr.end(); ++it, ++ctr) {
    bool sense = model.repn->objectives[ctr].sense();
    if (sense == Model::minimize)
        ostr.print("O{} 0\n", ctr);
    else
        ostr.print("O{} 1\n", ctr);
    if ((not it->nonlinear.is_constant()) or (it->quadratic_coefs.size() > 0)) {
        print_expr(ostr, *it, varmap, true);
        }
    else {
        ostr.print("n{}\n", it->constval.value());
        }
    }
}
CALI_MARK_END("O");

//
// "x" section - primal initial values
//
{
CALI_MARK_BEGIN("x_str");
{
auto out = std::vector<char>();
//fmt::memory_buffer out;
int num=0;
int ctr=0;
for (auto it=varobj.begin(); it != varobj.end(); ++it, ++ctr) {
    auto tmp = it->second.value();
    if (not std::isnan(tmp)) {
        num++;
        fmt::format_to(std::back_inserter(out), "{} {}\n", ctr, tmp);
        }
    }
if (num) {
    ostr.print("x{}\n", num);
    out.push_back(0);
    ostr.print("{}", out.data());
    }
}
CALI_MARK_END("x_str");
}

//
// "r" section - bounds on constraints
//
CALI_MARK_BEGIN("r");
if (model.repn->constraints.size() > 0) {
    ostr.print("r\n");
    size_t ctr = 0;
    for (auto it=r.begin(); it != r.end(); ++it, ++ctr) {
        switch (*it) {
            case 0:
                ostr.print("0 {} {}\n", rval[2*ctr], rval[2*ctr+1]);  // FORMAT
                break;
            case 1:
                ostr.print("1 {}\n", rval[2*ctr]);  // FORMAT
                break;
            case 2:
                ostr.print("2 {}\n", rval[2*ctr]);  // FORMAT
                break;
                // GCOVR_EXCL_START
            case 3:
                ostr.print("3");
                break;
                // GCOVR_EXCL_STOP
            case 4:
                ostr.print("4 {}\n", rval[2*ctr]);  // FORMAT
                break;
            };
        }
    }
CALI_MARK_END("r");

//
// "b" section - bounds on variables
//
CALI_MARK_BEGIN("b");
ostr.print("b\n");
for (auto it=vars.begin(); it != vars.end(); ++it) {
    auto& var = varobj[*it];
    double lb = var.lower();
    double ub = var.upper();
    if (lb == -COEK_INFINITY) {
        if (ub == COEK_INFINITY) {
            ostr.print("3\n");
            }
        else {
            ostr.print("1 {}\n", ub);
            }
        }
    else {
        if (ub == COEK_INFINITY) {
            ostr.print("2 {}\n", lb);
            }
        else {
            if (fabs(ub-lb) < EPSILON) {
                ostr.print("4 {}\n", lb);
                }
            else {
                ostr.print("0 {} {}\n", lb, ub);
                }
            }
        }
    }
CALI_MARK_END("b");

//
// "k" section - Jacobian column counts
//
CALI_MARK_BEGIN("k");
ostr.print("k{}\n", k_count.size()-1);      // << "k" << (k_count.size()-1) << '\n';
{
size_t ctr = 0;
for (size_t i=0; i<(k_count.size()-1); ++i) {
    ctr += k_count[i].size();
    ostr.print("{}\n", ctr);        // << ctr << '\n';
    }
}
CALI_MARK_END("k");

CALI_MARK_BEGIN("J");
//
// "J" section - Jacobian sparsity, linear terms
//
{
int ctr=0;
for (auto jt=J.begin(); jt != J.end(); ++ctr, ++jt) {
    if (jt->size() == 0) continue;
    ostr.print("J{} {}\n", ctr, jt->size());                 // << "J" << i << " " << J[i].size() << '\n';
    for (auto it=jt->begin(); it != jt->end(); ++it) {
        ostr.print("{} {}\n", it->first, it->second);       // << it->first << " " << it->second << '\n';
        }
    }
}
CALI_MARK_END("J");

//
// "G" section - Gradient sparsity, linear terms
//
CALI_MARK_BEGIN("G");
for (size_t i=0; i<G.size(); ++i) {
    if (G[i].size() == 0) continue;
    ostr.print("G{} {}\n", i, G[i].size());                 // << "G" << i << " " << G[i].size() << '\n';
    for (auto it=G[i].begin(); it!=G[i].end(); ++it) {
        ostr.print("{} {}\n", it->first, it->second);       // << it->first << " " << it->second << '\n';
        }
    }
CALI_MARK_END("G");

ostr.close();
}
#endif


void write_nl_problem_ostream(Model& model, std::string& fname, std::map<size_t,size_t>& invvarmap, std::map<size_t,size_t>& invconmap)
{
NLWriter writer;
writer.collect_nl_data(model, invvarmap, invconmap);
writer.write_ostream(model, fname);
}

#ifdef WITH_FMTLIB
void write_nl_problem_fmtlib(Model& model, std::string& fname, std::map<size_t,size_t>& invvarmap, std::map<size_t,size_t>& invconmap)
{
NLWriter writer;
writer.collect_nl_data(model, invvarmap, invconmap);
writer.write_fmtlib(model, fname);
}

void write_nl_problem(Model& model, std::string& fname, std::map<size_t,size_t>& invvarmap, std::map<size_t,size_t>& invconmap)
{ write_nl_problem_fmtlib(model, fname, invvarmap, invconmap); }
#else

void write_nl_problem(Model& model, std::string& fname, std::map<size_t,size_t>& invvarmap, std::map<size_t,size_t>& invconmap)
{ write_nl_problem_ostream(model, fname, invvarmap, invconmap); }
#endif

}
