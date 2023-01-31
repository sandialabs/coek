#include <fstream>
#include <iomanip>
#include <map>
#ifdef WITH_CALIPER
#    include <caliper/cali.h>
#else
#    define CALI_CXX_MARK_FUNCTION
#    define CALI_MARK_BEGIN(X)
#    define CALI_MARK_END(X)
#endif
#ifdef WITH_FMTLIB
#    include <fmt/compile.h>
#    include <fmt/core.h>
#    include <fmt/os.h>
#endif

#include "../ast/base_terms.hpp"
#include "../ast/constraint_terms.hpp"
#include "../ast/expr_terms.hpp"
#include "../ast/value_terms.hpp"
#include "../ast/visitor.hpp"
#include "../ast/visitor_fns.hpp"
#include "coek/api/constraint.hpp"
#include "coek/api/expression.hpp"
#include "coek/api/expression_visitor.hpp"
#include "coek/api/objective.hpp"
#include "coek/model/model.hpp"
#include "coek/util/sequence.hpp"
#include "model_repn.hpp"

#define EPSILON 1e-12
#define ITYPE size_t

namespace coek {

void to_MutableNLPExpr(const expr_pointer_t& expr, MutableNLPExpr& repn);

void check_that_expression_variables_are_declared(Model& model,
                                                  const std::map<size_t, Variable>& varobj);

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

void format(std::ostream& ostr, double value) { ostr << std::setprecision(16) << value; }

class PrintExpr : public Visitor {
   public:
    std::ostream& ostr;
    const std::unordered_map<ITYPE, ITYPE>& varmap;

   public:
    PrintExpr(std::ostream& _ostr, const std::unordered_map<ITYPE, ITYPE>& _varmap)
        : ostr(_ostr), varmap(_varmap)
    {
    }

    void visit(ConstantTerm& arg);
    void visit(ParameterTerm& arg);
    void visit(IndexParameterTerm& arg);
    void visit(VariableTerm& arg);
#if __cpp_lib_variant
    void visit(ParameterRefTerm& arg);
    void visit(VariableRefTerm& arg);
#endif
    void visit(MonomialTerm& arg);
    void visit(InequalityTerm& arg);
    void visit(EqualityTerm& arg);
    void visit(ObjectiveTerm& arg);
    void visit(SubExpressionTerm& arg);
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
    format(ostr, arg.eval());
    ostr << '\n';
}

// GCOVR_EXCL_START
void PrintExpr::visit(IndexParameterTerm&)
{
    throw std::runtime_error(
        "Encountered an index parameter when printing an expression.  This error should have been "
        "caught earlier!");
}
// GCOVR_EXCL_STOP

void PrintExpr::visit(VariableTerm& arg)
{
    if (arg.fixed)
        ostr << "n" << arg.eval() << '\n';
    else
        ostr << "v" << varmap.at(arg.index) << '\n';
}

#if __cpp_lib_variant
void PrintExpr::visit(ParameterRefTerm&)
{
    throw std::runtime_error("Cannot write an NL file using an abstract expression!");
}

void PrintExpr::visit(VariableRefTerm&)
{
    throw std::runtime_error("Cannot write an NL file using an abstract expression!");
}
#endif

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
void PrintExpr::visit(InequalityTerm&)
{
    throw std::runtime_error(
        "Encountered an inequality constraint when printing an expression.  This error should have "
        "been caught earlier!");
}

void PrintExpr::visit(EqualityTerm&)
{
    throw std::runtime_error(
        "Encountered an equality constraint when printing an expression.  This error should have "
        "been caught earlier!");
}

void PrintExpr::visit(ObjectiveTerm&)
{
    throw std::runtime_error(
        "Encountered an objective when printing an expression.  This error should have been caught "
        "earlier!");
}
// GCOVR_EXCL_STOP

void PrintExpr::visit(SubExpressionTerm& arg) { arg.body->accept(*this); }

void PrintExpr::visit(NegateTerm& arg)
{
    if (arg.body->is_constant()) {
        ostr << "n";
        format(ostr, -arg.body->eval());
        ostr << "\n";
    }
    else {
        ostr << "o16\n";
        arg.body->accept(*this);
    }
}

void PrintExpr::visit(PlusTerm& arg)
{
    if (arg.n == 2)
        ostr << "o0\n";
    else
        ostr << "o54\n" << arg.n << '\n';
    std::vector<expr_pointer_t>& vec = *(arg.data);
    for (size_t i = 0; i < arg.num_expressions(); ++i) vec[i]->accept(*this);
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

#define PrintExpr_FN(FN, TERM)       \
    void PrintExpr::visit(TERM& arg) \
    {                                \
        ostr << #FN << '\n';         \
        arg.body->accept(*this);     \
    }

// clang-format off
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
    // clang-format on

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

class PrintExprFmtlib : public Visitor {
   public:
    fmt::ostream& ostr;
    const std::unordered_map<ITYPE, ITYPE>& varmap;

   public:
    PrintExprFmtlib(fmt::ostream& _ostr, const std::unordered_map<ITYPE, ITYPE>& _varmap)
        : ostr(_ostr), varmap(_varmap)
    {
    }

    void visit(ConstantTerm& arg);
    void visit(ParameterTerm& arg);
    void visit(IndexParameterTerm& arg);
    void visit(VariableTerm& arg);
#    if __cpp_lib_variant
    void visit(ParameterRefTerm& arg);
    void visit(VariableRefTerm& arg);
#    endif
    void visit(MonomialTerm& arg);
    void visit(InequalityTerm& arg);
    void visit(EqualityTerm& arg);
    void visit(ObjectiveTerm& arg);
    void visit(SubExpressionTerm& arg);
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

constexpr auto _fmtstr_value = FMT_COMPILE("{}\n");
constexpr auto _fmtstr_n = FMT_COMPILE("n{}\n");
constexpr auto _fmtstr_v = FMT_COMPILE("v{}\n");
constexpr auto _fmtstr_o54 = FMT_COMPILE("o54\n{}\n");
constexpr auto _fmtstr_o2 = FMT_COMPILE("o2\nn{}\n");
constexpr auto _fmtstr_2vals = FMT_COMPILE("{} {}\n");

void PrintExprFmtlib::visit(ConstantTerm& arg) { ostr.print(fmt::format(_fmtstr_n, arg.value)); }

void PrintExprFmtlib::visit(ParameterTerm& arg) { ostr.print(fmt::format(_fmtstr_n, arg.eval())); }

// GCOVR_EXCL_START
void PrintExprFmtlib::visit(IndexParameterTerm&)
{
    throw std::runtime_error(
        "Encountered an index parameter when printing an expression.  This error should have been "
        "caught earlier!");
}
// GCOVR_EXCL_STOP

void PrintExprFmtlib::visit(VariableTerm& arg)
{
    if (arg.fixed)
        ostr.print(fmt::format(_fmtstr_n, arg.eval()));
    else
        ostr.print(fmt::format(_fmtstr_v, varmap.at(arg.index)));
}

#    if __cpp_lib_variant
void PrintExprFmtlib::visit(ParameterRefTerm&)
{
    throw std::runtime_error("Cannot write an NL file using an abstract expression!");
}

void PrintExprFmtlib::visit(VariableRefTerm&)
{
    throw std::runtime_error("Cannot write an NL file using an abstract expression!");
}
#    endif

void PrintExprFmtlib::visit(MonomialTerm& arg)
{
    ostr.print(fmt::format(_fmtstr_o2, arg.coef));

    if (arg.var->fixed)
        ostr.print(fmt::format(_fmtstr_n, arg.var->value->eval()));
    else
        ostr.print(fmt::format(_fmtstr_v, varmap.at(arg.var->index)));
}

// GCOVR_EXCL_START
void PrintExprFmtlib::visit(InequalityTerm&)
{
    throw std::runtime_error(
        "Encountered an inequality constraint when printing an expression.  This error should have "
        "been caught earlier!");
}

void PrintExprFmtlib::visit(EqualityTerm&)
{
    throw std::runtime_error(
        "Encountered an equality constraint when printing an expression.  This error should have "
        "been caught earlier!");
}

void PrintExprFmtlib::visit(ObjectiveTerm&)
{
    throw std::runtime_error(
        "Encountered an objective when printing an expression.  This error should have been caught "
        "earlier!");
}
// GCOVR_EXCL_STOP

void PrintExprFmtlib::visit(SubExpressionTerm& arg) { arg.body->accept(*this); }

void PrintExprFmtlib::visit(NegateTerm& arg)
{
    if (arg.body->is_constant()) {
        ostr.print(fmt::format(_fmtstr_n, -arg.body->eval()));
    }
    else {
        ostr.print("o16\n");
        arg.body->accept(*this);
    }
}

void PrintExprFmtlib::visit(PlusTerm& arg)
{
    if (arg.n == 2)
        ostr.print("o0\n");
    else
        ostr.print(fmt::format(_fmtstr_o54, arg.n));
    std::vector<expr_pointer_t>& vec = *(arg.data);
    for (size_t i = 0; i < arg.num_expressions(); ++i) vec[i]->accept(*this);
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

#    define PrintExprFmt_FN(FN, TERM)          \
        void PrintExprFmtlib::visit(TERM& arg) \
        {                                      \
            ostr.print(#FN "\n");              \
            arg.body->accept(*this);           \
        }

// clang-format off

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

    // clang-format on

    void PrintExprFmtlib::visit(PowTerm& arg)
{
    ostr.print("o5\n");
    arg.lhs->accept(*this);
    arg.rhs->accept(*this);
}
#endif  // WITH_FMTLIB

void print_expr(std::ostream& ostr, const MutableNLPExpr& repn,
                const std::unordered_map<ITYPE, ITYPE>& varmap, bool objective = false)
{
    bool nonlinear = not repn.nonlinear->is_constant();
    bool quadratic = repn.quadratic_coefs.size() > 0;

    double cval = repn.constval->eval();
    if (not nonlinear) cval += repn.nonlinear->eval();

    std::map<std::pair<size_t, size_t>, double> term;
    if (quadratic) {
        for (size_t i = 0; i < repn.quadratic_coefs.size(); ++i) {
            size_t lhs = varmap.at(repn.quadratic_lvars[i]->index);
            size_t rhs = varmap.at(repn.quadratic_rvars[i]->index);
            if (rhs < lhs) std::swap(lhs, rhs);
            auto key = std::pair<size_t, size_t>(lhs, rhs);
            auto it = term.find(key);
            if (it != term.end())
                it->second += repn.quadratic_coefs[i]->eval();
            else
                term[key] = repn.quadratic_coefs[i]->eval();
        }
    }

    // Compute the number of terms in the sum
    size_t ctr = 0;
    if (objective and (fabs(cval) > EPSILON)) ++ctr;
    if (nonlinear) ++ctr;
    if (quadratic) ctr += term.size();

    // Write the sum header
    if (ctr == 0)
        return;
    else if (ctr == 2)
        ostr << "o0\n";
    else if (ctr > 2)
        ostr << "o54\n" << ctr << '\n';

    // Write terms in the sum
    if (quadratic) {
        for (auto it = term.begin(); it != term.end(); ++it) {
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
        repn.nonlinear->accept(visitor);
    }
    if (objective and (fabs(cval) > EPSILON)) {
        ostr << "n";
        format(ostr, cval);
        ostr << '\n';
    }
}

#ifdef WITH_FMTLIB
void print_expr(fmt::ostream& ostr, const MutableNLPExpr& repn,
                const std::unordered_map<ITYPE, ITYPE>& varmap, bool objective = false)
{
    bool nonlinear = not repn.nonlinear->is_constant();
    bool quadratic = repn.quadratic_coefs.size() > 0;

    double cval = repn.constval->eval();
    if (not nonlinear) cval += repn.nonlinear->eval();

    std::map<std::pair<ITYPE, ITYPE>, double> term;
    if (quadratic) {
        for (size_t i = 0; i < repn.quadratic_coefs.size(); ++i) {
            ITYPE lhs = varmap.at(repn.quadratic_lvars[i]->index);
            ITYPE rhs = varmap.at(repn.quadratic_rvars[i]->index);
            if (rhs < lhs) std::swap(lhs, rhs);
            auto key = std::pair<ITYPE, ITYPE>(lhs, rhs);

            auto it = term.find(key);
            if (it != term.end())
                it->second += repn.quadratic_coefs[i]->eval();
            else
                term[key] = repn.quadratic_coefs[i]->eval();
        }
    }

    // Compute the number of terms in the sum
    size_t ctr = 0;
    if (objective and (fabs(cval) > EPSILON)) ++ctr;
    if (nonlinear) ++ctr;
    if (quadratic) ctr += term.size();

    // Write the sum header
    if (ctr == 0)
        return;
    else if (ctr == 2)
        ostr.print("o0\n");
    else if (ctr > 2)
        ostr.print(fmt::format(_fmtstr_o54, ctr));

    // Write terms in the sum
    constexpr auto _fmtstr_o2_2 = FMT_COMPILE("o2\nv{}\nv{}\n");
    if (quadratic) {
        for (auto it = term.begin(); it != term.end(); ++it) {
            double coef = it->second;
            if (coef != 1) {
                ostr.print(fmt::format(_fmtstr_o2, coef));  // FORMAT
            }
            ostr.print(fmt::format(_fmtstr_o2_2, it->first.first, it->first.second));
        }
    }
    if (nonlinear) {
        PrintExprFmtlib visitor(ostr, varmap);
        repn.nonlinear->accept(visitor);
    }
    if (objective and (fabs(cval) > EPSILON)) {
        ostr.print(fmt::format(_fmtstr_n, cval));  // FORMAT
    }
}
#endif
}  // namespace

class NLWriter {
   public:
    //
    // Process Model to Create NL Header
    //
    std::map<size_t, Variable> varobj;
    std::set<size_t> vars;
    std::set<size_t> nonlinear_vars_obj;
    std::set<size_t> nonlinear_vars_con;
    std::set<size_t> linear_vars;

    size_t num_inequalities = 0;
    size_t num_ranges = 0;
    size_t num_equalities = 0;
    size_t nonl_objectives = 0;
    size_t nonl_constraints = 0;

    size_t num_nonlinear_vars_con = 0;
    size_t num_nonlinear_vars_obj = 0;
    size_t num_nonlinear_vars_both = 0;

    size_t num_linear_binary_vars = 0;
    size_t num_linear_integer_vars = 0;
    size_t num_nonlinear_obj_int_vars = 0;
    size_t num_nonlinear_con_int_vars = 0;
    size_t num_nonlinear_both_int_vars = 0;

    size_t nnz_Jacobian = 0;
    size_t nnz_gradient = 0;

    std::vector<MutableNLPExpr> o_expr;
    std::vector<MutableNLPExpr> c_expr;
    std::vector<int> r;
    std::vector<double> rval;

    std::unordered_map<ITYPE, ITYPE> varmap;
    std::vector<std::set<size_t>> k_count;
    std::vector<std::map<size_t, double>> G;
    std::vector<std::map<size_t, double>> J;

    NLWriter() {}

    void collect_nl_data(Model& model, std::map<size_t, size_t>& invvarmap,
                         std::map<size_t, size_t>& invconmap);

    void write_ostream(Model& model, const std::string& fname);
    void write_fmtlib(Model& model, const std::string& fname);
};

// TODO - Reorder constraints to have nonlinear before linear
void NLWriter::collect_nl_data(Model& model, std::map<size_t, size_t>& invvarmap,
                               std::map<size_t, size_t>& invconmap)
{
    o_expr.resize(model.repn->objectives.size());
    c_expr.resize(model.repn->constraints.size());
    r.resize(model.repn->constraints.size());
    rval.resize(2 * model.repn->constraints.size());

    std::map<std::shared_ptr<SubExpressionTerm>, expr_pointer_t> simplified_subexpressions;

    CALI_CXX_MARK_FUNCTION;

    CALI_MARK_BEGIN("Prepare Objective Expressions");

    // Objectives
    try {
        {
            nnz_gradient = 0;
            size_t ctr = 0;
            for (auto& obj : model.repn->objectives) {
                to_MutableNLPExpr(simplify_expr(obj.repn, simplified_subexpressions), o_expr[ctr]);
                if ((o_expr[ctr].quadratic_coefs.size() > 0)
                    or (not o_expr[ctr].nonlinear->is_constant()))
                    ++nonl_objectives;

                std::set<ITYPE> curr_vars;
                for (auto& var : o_expr[ctr].linear_vars) {
                    auto index = var->index;
                    linear_vars.insert(index);
                    vars.insert(index);
                    curr_vars.insert(index);
                    varobj[index] = var;
                }
                for (auto& var : o_expr[ctr].quadratic_lvars) {
                    auto index = var->index;
                    nonlinear_vars_obj.insert(index);
                    vars.insert(index);
                    curr_vars.insert(index);
                    varobj[index] = var;
                }
                for (auto& var : o_expr[ctr].quadratic_rvars) {
                    auto index = var->index;
                    nonlinear_vars_obj.insert(index);
                    vars.insert(index);
                    curr_vars.insert(index);
                    varobj[index] = var;
                }
                for (auto& var : o_expr[ctr].nonlinear_vars) {
                    auto index = var->index;
                    nonlinear_vars_obj.insert(index);
                    vars.insert(index);
                    curr_vars.insert(index);
                    varobj[index] = var;
                }
                nnz_gradient += curr_vars.size();

                ctr++;
                break;  // TODO - Fix this for multiobjective
            }
        }
        CALI_MARK_END("Prepare Objective Expressions");

        CALI_MARK_BEGIN("Prepare Constraint Expressions");
        // Constraints
        {
            size_t ctr = 0;
            for (auto jt = model.repn->constraints.begin(); jt != model.repn->constraints.end();
                 ++jt, ++ctr) {
                auto& Expr = c_expr[ctr];
                auto& Con = *jt;

                invconmap[ctr] = Con.id();

                // std::cout << "OLD " << Con.body().to_list() << std::endl;
                to_MutableNLPExpr(simplify_expr(Con.repn, simplified_subexpressions), Expr);
                // std::cout << "NEW " << Expr.nonlinear->to_list() << std::endl;

                double bodyconst = Expr.constval->eval();
                if (Con.is_inequality()) {
                    if (Con.repn->lower and Con.repn->upper) {
                        double lower = Con.repn->lower->eval() - bodyconst;
                        double upper = Con.repn->upper->eval() - bodyconst;
                        if (fabs(upper - lower) < EPSILON) {
                            ++num_equalities;
                            r[ctr] = 4;
                            rval[2 * ctr] = lower;
                        }
                        else {
                            ++num_inequalities;
                            ++num_ranges;
                            r[ctr] = 0;
                            rval[2 * ctr] = lower;
                            rval[2 * ctr + 1] = upper;
                        }
                    }
                    else if (Con.repn->lower) {
                        ++num_inequalities;
                        r[ctr] = 2;
                        rval[2 * ctr] = Con.repn->lower->eval() - bodyconst;
                    }
                    else if (Con.repn->upper) {
                        ++num_inequalities;
                        r[ctr] = 1;
                        rval[2 * ctr] = Con.repn->upper->eval() - bodyconst;
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
                    rval[2 * ctr] = Con.repn->lower->eval() - bodyconst;
                }
                if ((Expr.quadratic_coefs.size() > 0) or (not Expr.nonlinear->is_constant()))
                    ++nonl_constraints;

                std::set<ITYPE> curr_vars;

                for (auto& var : Expr.linear_vars) {
                    auto index = var->index;
                    linear_vars.insert(index);
                    vars.insert(index);
                    varobj[index] = var;
                    curr_vars.insert(index);
                }
                for (auto& var : Expr.quadratic_lvars) {
                    auto index = var->index;
                    nonlinear_vars_con.insert(index);
                    vars.insert(index);
                    varobj[index] = var;
                    curr_vars.insert(index);
                }
                for (auto& var : Expr.quadratic_rvars) {
                    auto index = var->index;
                    nonlinear_vars_con.insert(index);
                    vars.insert(index);
                    varobj[index] = var;
                    curr_vars.insert(index);
                }
                for (auto& var : Expr.nonlinear_vars) {
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
    catch (std::exception& e) {
        throw std::runtime_error(std::string("Error writing NL file: ") + e.what());
    }

    CALI_MARK_BEGIN("Misc NL");
    std::set<size_t> nonlinear_vars_both_c;
    std::set<size_t> nonlinear_vars_both_i;
    std::set<size_t> nonlinear_vars_con_c;
    std::set<size_t> nonlinear_vars_con_i;
    std::set<size_t> nonlinear_vars_obj_c;
    std::set<size_t> nonlinear_vars_obj_i;
    std::set<size_t> linear_vars_c;
    std::set<size_t> linear_vars_b;
    std::set<size_t> linear_vars_i;
    //
    // Collect the variables that are in both objectives and constraints
    //
    for (auto& vid : nonlinear_vars_con) {
        if (nonlinear_vars_obj.find(vid) == nonlinear_vars_obj.end()) {
            auto& var = varobj[vid];
            // CON
            if (var.is_binary() or var.is_integer())
                nonlinear_vars_con_i.insert(vid);
            else
                nonlinear_vars_con_c.insert(vid);
        }
        else {
            auto& var = varobj[vid];
            // BOTH
            if (var.is_binary() or var.is_integer())
                nonlinear_vars_both_i.insert(vid);
            else
                nonlinear_vars_both_c.insert(vid);
        }
    }
    for (auto& vid : nonlinear_vars_obj) {
        if (nonlinear_vars_con.find(vid) == nonlinear_vars_con.end()) {
            auto& var = varobj[vid];
            // OBJ
            if (var.is_binary() or var.is_integer())
                nonlinear_vars_obj_i.insert(vid);
            else
                nonlinear_vars_obj_c.insert(vid);
        }
    }
    num_nonlinear_both_int_vars = nonlinear_vars_both_i.size();
    num_nonlinear_con_int_vars = num_nonlinear_con_int_vars + nonlinear_vars_con_i.size();
    num_nonlinear_obj_int_vars = num_nonlinear_con_int_vars + nonlinear_vars_obj_i.size();

    num_nonlinear_vars_both = nonlinear_vars_both_i.size() + nonlinear_vars_both_c.size();
    num_nonlinear_vars_con
        = num_nonlinear_vars_both + nonlinear_vars_con_i.size() + nonlinear_vars_con_c.size();
    num_nonlinear_vars_obj
        = num_nonlinear_vars_con + nonlinear_vars_obj_i.size() + nonlinear_vars_obj_c.size();

    num_linear_binary_vars = 0;
    num_linear_integer_vars = 0;
    for (auto& vid : linear_vars) {
        auto& var = varobj[vid];
        if (var.is_binary())
            num_linear_binary_vars++;
        else if (var.is_integer())
            num_linear_integer_vars++;

        if (nonlinear_vars_obj.find(vid) != nonlinear_vars_obj.end()) continue;
        if (nonlinear_vars_con.find(vid) != nonlinear_vars_con.end()) continue;

        if (var.is_binary())
            linear_vars_b.insert(vid);
        else if (var.is_integer())
            linear_vars_i.insert(vid);
        else
            linear_vars_c.insert(vid);
    }

    // Map Variable index to NL variable ID (0 ... n_vars-1)
    {
        size_t ctr = 0;
        for (auto& vid : nonlinear_vars_both_c) {
            invvarmap[ctr] = vid;
            varmap[vid] = ctr++;
        }
        for (auto& vid : nonlinear_vars_both_i) {
            invvarmap[ctr] = vid;
            varmap[vid] = ctr++;
        }
        for (auto& vid : nonlinear_vars_con_c) {
            invvarmap[ctr] = vid;
            varmap[vid] = ctr++;
        }
        for (auto& vid : nonlinear_vars_con_i) {
            invvarmap[ctr] = vid;
            varmap[vid] = ctr++;
        }
        for (auto& vid : nonlinear_vars_obj_c) {
            invvarmap[ctr] = vid;
            varmap[vid] = ctr++;
        }
        for (auto& vid : nonlinear_vars_obj_i) {
            invvarmap[ctr] = vid;
            varmap[vid] = ctr++;
        }
        for (auto& vid : linear_vars_c) {
            invvarmap[ctr] = vid;
            varmap[vid] = ctr++;
        }
        for (auto& vid : linear_vars_b) {
            invvarmap[ctr] = vid;
            varmap[vid] = ctr++;
        }
        for (auto& vid : linear_vars_i) {
            invvarmap[ctr] = vid;
            varmap[vid] = ctr++;
        }
    }

    // GCOVR_EXCL_START
    if (vars.size() != varmap.size())
        throw std::runtime_error(
            "Error writing NL file: Variables with duplicate index values detected!");
    // GCOVR_EXCL_STOP

    // Compute linear Jacobian and Gradient values
    CALI_MARK_BEGIN("Compute Jacobian/Gradient");
    k_count.resize(vars.size());
    G.resize(o_expr.size());
    J.resize(c_expr.size());

    {
        size_t ctr = 0;
        for (auto it = o_expr.begin(); it != o_expr.end(); ++it, ++ctr) {
            for (auto& var : it->quadratic_lvars) {
                G[ctr][varmap[var->index]] = 0;
            }
            for (auto& var : it->quadratic_rvars) {
                G[ctr][varmap[var->index]] = 0;
            }
            for (auto& var : it->nonlinear_vars) {
                G[ctr][varmap[var->index]] = 0;
            }
            for (size_t j : coek::indices(it->linear_coefs)) {
                auto index = varmap[it->linear_vars[j]->index];
                auto jt = G[ctr].find(index);
                if (jt != G[ctr].end())
                    jt->second += it->linear_coefs[j]->eval();
                else
                    G[ctr][index] = it->linear_coefs[j]->eval();
            }
        }
    }
    {
        size_t ctr = 0;
        for (auto it = c_expr.begin(); it != c_expr.end(); ++it, ++ctr) {
            for (auto& var : it->quadratic_lvars) {
                size_t index = varmap[var->index];
                k_count[index].insert(ctr);
                J[ctr][index] = 0;
            }
            for (auto& var : it->quadratic_rvars) {
                size_t index = varmap[var->index];
                k_count[index].insert(ctr);
                J[ctr][index] = 0;
            }
            for (auto& var : it->nonlinear_vars) {
                size_t index = varmap[var->index];
                k_count[index].insert(ctr);
                J[ctr][index] = 0;
            }
            for (size_t j : coek::indices(it->linear_coefs)) {
                size_t index = varmap[it->linear_vars[j]->index];
                auto jt = J[ctr].find(index);
                if (jt != J[ctr].end())
                    jt->second += it->linear_coefs[j]->eval();
                else {
                    k_count[index].insert(ctr);
                    J[ctr][index] = it->linear_coefs[j]->eval();
                }
            }
        }
    }
    CALI_MARK_END("Compute Jacobian/Gradient");
}

void NLWriter::write_ostream(Model& model, const std::string& fname)
{
    std::ofstream ostr(fname);

    size_t ctr = 0;
    try {
        //
        // Write NL Header
        //
        // This API seems poorly documented.  Is the 2005 paper the defining reference?  Pyomo
        // writes a header that doesn't conform to it...
        //
        ostr << "g3 1 1 0 # unnamed problem generated by COEK\n";
        ostr << " " << vars.size() << " " << (num_inequalities + num_equalities) << " "
             << std::max((size_t)1, o_expr.size()) << " " << num_ranges << " " << num_equalities
             << " 0 # vars, constraints, objectives, ranges, eqns, lcons\n";
        ostr << " " << nonl_constraints << " " << nonl_objectives
             << " # nonlinear constraints, objectives\n";
        ostr << " 0 0 # network constraints: nonlinear, linear\n";
        ostr << " " << num_nonlinear_vars_con << " " << num_nonlinear_vars_obj << " "
             << num_nonlinear_vars_both << " # nonlinear vars in constraints, objectives, both\n";
        ostr << " 0 0 0 1 # linear network variables; functions; arith, flags\n";
        ostr << " " << num_linear_binary_vars << " " << num_linear_integer_vars << " "
             << num_nonlinear_both_int_vars << " " << num_nonlinear_con_int_vars << " "
             << num_nonlinear_obj_int_vars
             << " # discrete variables: binary, integer, nonlinear (b,c,o)\n";
        ostr << " " << nnz_Jacobian << " " << nnz_gradient
             << " # nonzeros in Jacobian, gradients\n";
        ostr << " 0 0 # max name lengths: constraints, variables\n";
        ostr << " 0 0 0 0 0 # common exprs: b,c,o,c1,o1\n";

        //
        // "C" section - nonlinear constraint segments
        //
        ctr = 0;
        for (auto it = c_expr.begin(); it != c_expr.end(); ++it, ++ctr) {
            if ((not it->nonlinear->is_constant()) or (it->quadratic_coefs.size() > 0)) {
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
        if (o_expr.size() > 0) {
            ctr = 0;
            for (auto it = o_expr.begin(); it != o_expr.end(); ++it, ++ctr) {
                bool sense = model.repn->objectives[ctr].sense();
                if (sense == Model::minimize)
                    ostr << "O" << ctr << " 0\n";
                else
                    ostr << "O" << ctr << " 1\n";
                if ((not it->nonlinear->is_constant()) or (it->quadratic_coefs.size() > 0)) {
                    print_expr(ostr, *it, varmap, true);
                }
                else {
                    ostr << "n" << it->constval->eval() << '\n';
                }
            }
        }
        else {
            ostr << "O0 0\nn0\n";
        }

        //
        // "x" section - primal initial values
        //
        {
            std::map<size_t, double> values;
            ctr = 0;
            for (auto it = vars.begin(); it != vars.end(); ++it, ++ctr) {
                auto tmp = varobj[*it].value();
                if (not std::isnan(tmp)) values[ctr] = tmp;
            }
            ostr << "x" << values.size() << '\n';
            if (values.size() > 0) {
                for (auto it = values.begin(); it != values.end(); ++it)
                    ostr << it->first << " " << it->second << '\n';
            }
        }

        //
        // "r" section - bounds on constraints
        //

        if (model.repn->constraints.size() > 0) {
            ostr << "r\n";
            ctr = 0;
            for (auto it = model.repn->constraints.begin(); it != model.repn->constraints.end();
                 ++it, ++ctr) {
                switch (r[ctr]) {
                    case 0:
                        ostr << "0 ";
                        format(ostr, rval[2 * ctr]);
                        ostr << " ";
                        format(ostr, rval[2 * ctr + 1]);
                        break;
                    case 1:
                        ostr << "1 ";
                        format(ostr, rval[2 * ctr]);
                        break;
                    case 2:
                        ostr << "2 ";
                        format(ostr, rval[2 * ctr]);
                        break;
                    // GCOVR_EXCL_START
                    case 3:
                        ostr << "3";
                        break;
                    // GCOVR_EXCL_STOP
                    case 4:
                        ostr << "4 ";
                        format(ostr, rval[2 * ctr]);
                        break;
                };
                ostr << '\n';
            }
        }

        //
        // "b" section - bounds on variables
        //
        ostr << "b\n";
        for (auto it = vars.begin(); it != vars.end(); ++it) {
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
                    if (fabs(ub - lb) < EPSILON) {
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
        if (J.size() > 0) {
            if (k_count.size() > 1) {
                ostr << "k" << (k_count.size() - 1) << '\n';
                ctr = 0;
                for (size_t i = 0; i < (k_count.size() - 1); ++i) {
                    ctr += k_count[i].size();
                    ostr << ctr << '\n';
                }
            }
            else
                ostr << "k0\n";
            }

        //
        // "J" section - Jacobian sparsity, linear terms
        //
        for (size_t i = 0; i < J.size(); ++i) {
            if (J[i].size() == 0) continue;
            ostr << "J" << i << " " << J[i].size() << '\n';
            for (auto it = J[i].begin(); it != J[i].end(); ++it) {
                ostr << it->first << " " << it->second << '\n';
            }
        }

        //
        // "G" section - Gradient sparsity, linear terms
        //
        for (size_t i = 0; i < G.size(); ++i) {
            if (G[i].size() == 0) continue;
            ostr << "G" << i << " " << G[i].size() << '\n';
            for (auto it = G[i].begin(); it != G[i].end(); ++it) {
                ostr << it->first << " " << it->second << '\n';
            }
        }
    }
    // GCOVR_EXCL_START
    catch (std::exception& e) {
        throw std::runtime_error(std::string("Error writing NL file: ") + e.what());
    }
    // GCOVR_EXCL_STOP

    ostr.close();
}

#ifdef WITH_FMTLIB
void NLWriter::write_fmtlib(Model& model, const std::string& fname)
{
    auto ostr = fmt::output_file(fname, fmt::file::WRONLY | fmt::file::CREATE | FMT_POSIX(O_TRUNC));

    //
    // Write NL Header
    //
    // This API seems poorly documented.  Is the 2005 paper the defining reference?  Pyomo writes a
    // header that doesn't conform to it...
    //
    ostr.print("g3 1 1 0 # unnamed problem generated by COEK\n");
    ostr.print(" {} {} {} {} {} 0 # vars, constraints, objectives, ranges, eqns, lcons\n",
               vars.size(), (num_inequalities + num_equalities), std::max((size_t)1, o_expr.size()),
               num_ranges, num_equalities);
    ostr.print(" {} {} # nonlinear constraints, objectives\n", nonl_constraints, nonl_objectives);
    ostr.print(" 0 0 # network constraints: nonlinear, linear\n");
    ostr.print(" {} {} {} # nonlinear vars in constraints, objectives, both\n",
               num_nonlinear_vars_con, num_nonlinear_vars_obj, num_nonlinear_vars_both);
    ostr.print(" 0 0 0 1 # linear network variables; functions; arith, flags\n");
    ostr.print(" {} {} {} {} {} # discrete variables: binary, integer, nonlinear (b,c,o)\n",
               num_linear_binary_vars, num_linear_integer_vars, num_nonlinear_both_int_vars,
               num_nonlinear_con_int_vars, num_nonlinear_obj_int_vars);
    ostr.print(" {} {} # nonzeros in Jacobian, gradients\n", nnz_Jacobian, nnz_gradient);
    ostr.print(" 0 0 # max name lengths: constraints, variables\n");
    ostr.print(" 0 0 0 0 0 # common exprs: b,c,o,c1,o1\n");

    //
    // "C" section - nonlinear constraint segments
    //
    CALI_MARK_BEGIN("C");
    {
        constexpr auto _fmtstr_C = FMT_COMPILE("C{}\n");
        constexpr auto _fmtstr_C_n0 = FMT_COMPILE("C{}\nn0\n");
        int ctr = 0;
        for (auto it = c_expr.begin(); it != c_expr.end(); ++it, ++ctr) {
            if ((not it->nonlinear->is_constant()) or (it->quadratic_coefs.size() > 0)) {
                ostr.print(fmt::format(_fmtstr_C, ctr));
                print_expr(ostr, *it, varmap);
            }
            else {
                ostr.print(fmt::format(_fmtstr_C_n0, ctr));
            }
        }
    }
    CALI_MARK_END("C");

    //
    // "O" section - nonlinear objective segments
    //
    CALI_MARK_BEGIN("O");
    {
        if (o_expr.size() > 0) {
            constexpr auto _fmtstr_O_0 = FMT_COMPILE("O{} 0\n");
            constexpr auto _fmtstr_O_1 = FMT_COMPILE("O{} 1\n");
            size_t ctr = 0;
            for (auto it = o_expr.begin(); it != o_expr.end(); ++it, ++ctr) {
                bool sense = model.repn->objectives[ctr].sense();
                if (sense == Model::minimize)
                    ostr.print(fmt::format(_fmtstr_O_0, ctr));
                else
                    ostr.print(fmt::format(_fmtstr_O_1, ctr));
                if ((not it->nonlinear->is_constant()) or (it->quadratic_coefs.size() > 0)) {
                    print_expr(ostr, *it, varmap, true);
                }
                else {
                    ostr.print(fmt::format(_fmtstr_n, it->constval->eval()));
                }
            }
        }
        else {
            ostr.print("O0 0\nn0\n");
        }
    }
    CALI_MARK_END("O");

    //
    // "x" section - primal initial values
    //
    {
        CALI_MARK_BEGIN("x_str");
        {
            constexpr auto _fmtstr_x = FMT_COMPILE("{} {}\n");
            auto out = std::vector<char>();
            // fmt::memory_buffer out;
            int num = 0;
            int ctr = 0;
            for (auto it = varobj.begin(); it != varobj.end(); ++it, ++ctr) {
                auto tmp = it->second.value();
                if (not std::isnan(tmp)) {
                    num++;
                    fmt::format_to(std::back_inserter(out), _fmtstr_x, ctr, tmp);
                }
            }
            ostr.print("x{}\n", num);
            if (num) {
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
        constexpr auto _fmtstr_r0 = FMT_COMPILE("0 {} {}\n");
        constexpr auto _fmtstr_r1 = FMT_COMPILE("1 {}\n");
        constexpr auto _fmtstr_r2 = FMT_COMPILE("2 {}\n");
        constexpr auto _fmtstr_r4 = FMT_COMPILE("4 {}\n");
        ostr.print("r\n");
        size_t ctr = 0;
        for (auto it = r.begin(); it != r.end(); ++it, ++ctr) {
            switch (*it) {
                case 0:
                    ostr.print(
                        fmt::format(_fmtstr_r0, rval[2 * ctr], rval[2 * ctr + 1]));  // FORMAT
                    break;
                case 1:
                    ostr.print(fmt::format(_fmtstr_r1, rval[2 * ctr]));  // FORMAT
                    break;
                case 2:
                    ostr.print(fmt::format(_fmtstr_r2, rval[2 * ctr]));  // FORMAT
                    break;
                    // GCOVR_EXCL_START
                case 3:
                    ostr.print("3");
                    break;
                    // GCOVR_EXCL_STOP
                case 4:
                    ostr.print(fmt::format(_fmtstr_r4, rval[2 * ctr]));  // FORMAT
                    break;
            };
        }
    }
    CALI_MARK_END("r");

    //
    // "b" section - bounds on variables
    //
    CALI_MARK_BEGIN("b");
    constexpr auto _fmtstr_b1 = FMT_COMPILE("1 {}\n");
    constexpr auto _fmtstr_b2 = FMT_COMPILE("2 {}\n");
    constexpr auto _fmtstr_b4 = FMT_COMPILE("4 {}\n");
    constexpr auto _fmtstr_b0 = FMT_COMPILE("0 {} {}\n");
    ostr.print("b\n");
    for (auto it = vars.begin(); it != vars.end(); ++it) {
        auto& var = varobj[*it];
        double lb = var.lower();
        double ub = var.upper();
        if (lb == -COEK_INFINITY) {
            if (ub == COEK_INFINITY) {
                ostr.print("3\n");
            }
            else {
                ostr.print(fmt::format(_fmtstr_b1, ub));
            }
        }
        else {
            if (ub == COEK_INFINITY) {
                ostr.print(fmt::format(_fmtstr_b2, lb));
            }
            else {
                if (fabs(ub - lb) < EPSILON) {
                    ostr.print(fmt::format(_fmtstr_b4, lb));
                }
                else {
                    ostr.print(fmt::format(_fmtstr_b0, lb, ub));
                }
            }
        }
    }
    CALI_MARK_END("b");

    //
    // "k" section - Jacobian column counts
    //
    CALI_MARK_BEGIN("k");
    if (J.size() > 0) {
        if (k_count.size() > 1) {
            ostr.print("k{}\n", k_count.size() - 1);  // << "k" << (k_count.size()-1) << '\n';
            {
                size_t ctr = 0;
                for (size_t i = 0; i < (k_count.size() - 1); ++i) {
                    ctr += k_count[i].size();
                    ostr.print(fmt::format(_fmtstr_value, ctr));  // << ctr << '\n';
                }
            }
        }
        else
            ostr.print("k0\n");
    }
    CALI_MARK_END("k");

    CALI_MARK_BEGIN("J");
    //
    // "J" section - Jacobian sparsity, linear terms
    //
    {
        constexpr auto _fmtstr_J = FMT_COMPILE("J{} {}\n");
        int ctr = 0;
        for (auto jt = J.begin(); jt != J.end(); ++ctr, ++jt) {
            if (jt->size() == 0) continue;
            ostr.print(fmt::format(_fmtstr_J, ctr,
                                   jt->size()));  // << "J" << i << " " << J[i].size() << '\n';
            for (auto it = jt->begin(); it != jt->end(); ++it) {
                ostr.print(fmt::format(_fmtstr_2vals, it->first,
                                       it->second));  // << it->first << " " << it->second << '\n';
            }
        }
    }
    CALI_MARK_END("J");

    //
    // "G" section - Gradient sparsity, linear terms
    //
    constexpr auto _fmtstr_G = FMT_COMPILE("G{} {}\n");
    CALI_MARK_BEGIN("G");
    for (size_t i = 0; i < G.size(); ++i) {
        if (G[i].size() == 0) continue;
        ostr.print(
            fmt::format(_fmtstr_G, i, G[i].size()));  // << "G" << i << " " << G[i].size() << '\n';
        for (auto it = G[i].begin(); it != G[i].end(); ++it) {
            ostr.print(fmt::format(_fmtstr_2vals, it->first,
                                   it->second));  // << it->first << " " << it->second << '\n';
        }
    }
    CALI_MARK_END("G");

    ostr.close();
}
#endif

void write_nl_problem_ostream(Model& model, const std::string& fname,
                              std::map<size_t, size_t>& invvarmap,
                              std::map<size_t, size_t>& invconmap)
{
    NLWriter writer;
    writer.collect_nl_data(model, invvarmap, invconmap);
    writer.write_ostream(model, fname);
}

#ifdef WITH_FMTLIB
void write_nl_problem_fmtlib(Model& model, const std::string& fname,
                             std::map<size_t, size_t>& invvarmap,
                             std::map<size_t, size_t>& invconmap)
{
    NLWriter writer;
    writer.collect_nl_data(model, invvarmap, invconmap);
    writer.write_fmtlib(model, fname);
}

void write_nl_problem(Model& model, const std::string& fname, std::map<size_t, size_t>& invvarmap,
                      std::map<size_t, size_t>& invconmap)
{
    write_nl_problem_fmtlib(model, fname, invvarmap, invconmap);
}
#else

void write_nl_problem(Model& model, const std::string& fname, std::map<size_t, size_t>& invvarmap,
                      std::map<size_t, size_t>& invconmap)
{
    write_nl_problem_ostream(model, fname, invvarmap, invconmap);
}
#endif

}  // namespace coek
