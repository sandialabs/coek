#include <algorithm>
#include <unordered_set>

#include "base_terms.hpp"
#include "constraint_terms.hpp"
#include "expr_terms.hpp"
#include "value_terms.hpp"
#include "visitor.hpp"
#include "visitor_fns.hpp"
#include "../util/cast_utils.hpp"
#include "../util/io_utils.hpp"
#ifdef COEK_WITH_COMPACT_MODEL
#    include "compact_terms.hpp"
#endif

#include "ast_operators.hpp"
#include "coek/api/expression.hpp"
#include "coek/api/expression_visitor.hpp"

namespace std {

std::ostream& operator<<(std::ostream& ostr, const coek::MutableNLPExpr& arg)
{
    ostr << "Constant: ";
    write_expr(arg.constval, ostr);
    ostr << std::endl;
    ostr << "Linear: " << std::endl;
    for (size_t i = 0; i < arg.linear_coefs.size(); i++) {
        write_expr(arg.linear_coefs[i], ostr);
        ostr << " ";
        write_expr(arg.linear_vars[i], ostr);
        ostr << std::endl;
    }
    ostr << "Quadratic: " << std::endl;
    for (size_t i = 0; i < arg.quadratic_coefs.size(); i++) {
        write_expr(arg.quadratic_coefs[i], ostr);
        ostr << " ";
        write_expr(arg.quadratic_lvars[i], ostr);
        ostr << " ";
        write_expr(arg.quadratic_rvars[i], ostr);
        ostr << std::endl;
    }
    ostr << "Nonlinear: " << std::endl;
    ostr << arg.nonlinear->to_list() << std::endl;
    return ostr;
}

}  // namespace std

namespace coek {

MutableNLPExpr::MutableNLPExpr()
    : constval(ZeroConstant), nonlinear(ZeroConstant), mutable_values(false)
{
}

namespace {

void visit_expression(const expr_pointer_t& expr, MutableNLPExpr& repn, double multiplier);

void visit(std::shared_ptr<ConstantTerm>& expr, MutableNLPExpr& repn, double multiplier)
{
    repn.constval = plus_(repn.constval, CREATE_POINTER(ConstantTerm, multiplier * expr->value));
}

void visit(std::shared_ptr<ParameterTerm>& expr, MutableNLPExpr& repn, double multiplier)
{
    if (multiplier == 1)
        repn.constval = plus_(repn.constval, expr);
    else
        repn.constval = plus_(repn.constval, times(CREATE_POINTER(ConstantTerm, multiplier), expr));
    repn.mutable_values = true;
}

void visit(std::shared_ptr<IndexParameterTerm>& /*expr*/, MutableNLPExpr& /*repn*/,
           double /*multiplier*/)
{
    throw std::runtime_error("Unexpected index parameter.");
}

void visit(std::shared_ptr<VariableTerm>& expr, MutableNLPExpr& repn, double multiplier)
{
    // if (! expr->index)
    //     throw std::runtime_error("Unexpected variable not owned by a model.");

    if (expr->fixed) {
        if (multiplier == 1)
            repn.constval = plus_(repn.constval, expr);
        else
            repn.constval
                = plus_(repn.constval, times(CREATE_POINTER(ConstantTerm, multiplier), expr));
        repn.mutable_values = true;
    }
    else {
        repn.linear_vars.push_back(expr);
        if (multiplier == 1) {
            repn.linear_coefs.push_back(ONECONST);
        }
        else {
            repn.linear_coefs.push_back(CREATE_POINTER(ConstantTerm, multiplier));
        }
    }
}

#ifdef COEK_WITH_COMPACT_MODEL
void visit(std::shared_ptr<VariableRefTerm>& /*expr*/, MutableNLPExpr& /*repn*/,
           double /*multiplier*/)
{
    throw std::runtime_error("Unexpected variable reference.");
}
#endif

void visit(std::shared_ptr<MonomialTerm>& expr, MutableNLPExpr& repn, double multiplier)
{
    // if (! expr->var->index)
    //     throw std::runtime_error("Unexpected variable not owned by a model.");

    if (expr->var->fixed) {
        repn.constval = plus_(
            repn.constval, times(CREATE_POINTER(ConstantTerm, multiplier * expr->coef), expr->var));
        repn.mutable_values = true;
    }
    else {
        repn.linear_vars.push_back(expr->var);
        repn.linear_coefs.push_back(CREATE_POINTER(ConstantTerm, multiplier * expr->coef));
    }
}

void visit(std::shared_ptr<InequalityTerm>& expr, MutableNLPExpr& repn, double multiplier)
{
    visit_expression(expr->body, repn, multiplier);
}

void visit(std::shared_ptr<EqualityTerm>& expr, MutableNLPExpr& repn, double multiplier)
{
    visit_expression(expr->body, repn, multiplier);
}

void visit(std::shared_ptr<ObjectiveTerm>& expr, MutableNLPExpr& repn, double multiplier)
{
    visit_expression(expr->body, repn, multiplier);
}

void visit(std::shared_ptr<SubExpressionTerm>& expr, MutableNLPExpr& repn, double multiplier)
{
    visit_expression(expr->body, repn, multiplier);
}

void visit(std::shared_ptr<NegateTerm>& expr, MutableNLPExpr& repn, double multiplier)
{
    visit_expression(expr->body, repn, -multiplier);
}

void visit(std::shared_ptr<PlusTerm>& expr, MutableNLPExpr& repn, double multiplier)
{
    std::vector<expr_pointer_t>& vec = *(expr->data.get());
    auto n = expr->num_expressions();
    for (size_t i = 0; i < n; i++) visit_expression(vec[i], repn, multiplier);
}

void visit(std::shared_ptr<TimesTerm>& expr, MutableNLPExpr& repn, double multiplier)
{
    MutableNLPExpr lhs_repn;
    visit_expression(expr->lhs, lhs_repn, multiplier);

    // LHS is a simple constant
    if ((lhs_repn.linear_coefs.size() == 0) and (lhs_repn.quadratic_coefs.size() == 0)
        and (lhs_repn.nonlinear == ZEROCONST) and lhs_repn.constval->is_constant()) {
        if (lhs_repn.constval != ZEROCONST)
            visit_expression(expr->rhs, repn, lhs_repn.constval->eval());
        return;
    }

    MutableNLPExpr rhs_repn;
    visit_expression(expr->rhs, rhs_repn, 1.0);

    repn.mutable_values = repn.mutable_values or lhs_repn.mutable_values or rhs_repn.mutable_values;

    // Don't expand expressions with cubic or nonlinear terms
    // Don't expand products of linear terms, unless they are "simple"  (e.g. x*(y+z) )
    int lhs_mindegree = std::max(lhs_repn.linear_coefs.size() == 0 ? 0 : 1,
                                 std::max(lhs_repn.quadratic_coefs.size() == 0 ? 0 : 2,
                                          lhs_repn.nonlinear == ZEROCONST ? 0 : 4));
    int rhs_mindegree = std::max(rhs_repn.linear_coefs.size() == 0 ? 0 : 1,
                                 std::max(rhs_repn.quadratic_coefs.size() == 0 ? 0 : 2,
                                          rhs_repn.nonlinear == ZEROCONST ? 0 : 4));
    if (((lhs_mindegree + rhs_mindegree) > 2) or  // Creating 3rd-degree polynomial
        (std::min(lhs_repn.linear_coefs.size(), rhs_repn.linear_coefs.size())
         > 1)) {  // Creating product of linear terms
        repn.nonlinear = plus_(repn.nonlinear, expr);
        std::unordered_set<std::shared_ptr<VariableTerm>> exprvars;
        find_variables(expr, exprvars);
        repn.nonlinear_vars.insert(exprvars.begin(), exprvars.end());
        return;
    }

    // CONSTANT * CONSTANT
    if (not((lhs_repn.constval == ZEROCONST) or (rhs_repn.constval == ZEROCONST)))
        repn.constval = times_(lhs_repn.constval, rhs_repn.constval);

    if (not(lhs_repn.constval == ZEROCONST)) {
        // CONSTANT * LINEAR
        repn.linear_vars.insert(repn.linear_vars.end(), rhs_repn.linear_vars.begin(),
                                rhs_repn.linear_vars.end());
        for (size_t i = 0; i < rhs_repn.linear_coefs.size(); i++)
            repn.linear_coefs.push_back(times_(lhs_repn.constval, rhs_repn.linear_coefs[i]));
        // CONSTANT * QUADRATIC
        repn.quadratic_lvars.insert(repn.quadratic_lvars.end(), rhs_repn.quadratic_lvars.begin(),
                                    rhs_repn.quadratic_lvars.end());
        repn.quadratic_rvars.insert(repn.quadratic_rvars.end(), rhs_repn.quadratic_rvars.begin(),
                                    rhs_repn.quadratic_rvars.end());
        for (size_t i = 0; i < rhs_repn.quadratic_coefs.size(); i++)
            repn.quadratic_coefs.push_back(times_(rhs_repn.quadratic_coefs[i], lhs_repn.constval));
    }
    if (not(rhs_repn.constval == ZEROCONST)) {
        // LINEAR * CONSTANT
        repn.linear_vars.insert(repn.linear_vars.end(), lhs_repn.linear_vars.begin(),
                                lhs_repn.linear_vars.end());
        for (size_t i = 0; i < lhs_repn.linear_coefs.size(); i++)
            repn.linear_coefs.push_back(times_(rhs_repn.constval, lhs_repn.linear_coefs[i]));
        // QUADRATIC * CONSTANT
        repn.quadratic_lvars.insert(repn.quadratic_lvars.end(), lhs_repn.quadratic_lvars.begin(),
                                    lhs_repn.quadratic_lvars.end());
        repn.quadratic_rvars.insert(repn.quadratic_rvars.end(), lhs_repn.quadratic_rvars.begin(),
                                    lhs_repn.quadratic_rvars.end());
        for (size_t i = 0; i < lhs_repn.quadratic_coefs.size(); i++)
            repn.quadratic_coefs.push_back(times_(lhs_repn.quadratic_coefs[i], rhs_repn.constval));
    }

    // LINEAR * LINEAR
    for (size_t i = 0; i < lhs_repn.linear_coefs.size(); i++) {
        for (size_t j = 0; j < rhs_repn.linear_coefs.size(); j++) {
            repn.quadratic_lvars.push_back(lhs_repn.linear_vars[i]);
            repn.quadratic_rvars.push_back(rhs_repn.linear_vars[j]);
            repn.quadratic_coefs.push_back(
                times_(lhs_repn.linear_coefs[i], rhs_repn.linear_coefs[j]));
        }
    }
}

void visit(std::shared_ptr<DivideTerm>& expr, MutableNLPExpr& repn, double multiplier)
{
    MutableNLPExpr rhs_repn;
    visit_expression(expr->rhs, rhs_repn, 1.0);

    if (((rhs_repn.linear_coefs.size() + rhs_repn.quadratic_coefs.size()) == 0)
        and (rhs_repn.nonlinear == ZEROCONST)) {
        // Dividing by a constant expression
        if (rhs_repn.constval->is_constant()) {
            if (rhs_repn.constval->eval() == 0) {
                throw std::runtime_error("Division by zero error.");
            }
            // Dividing by a simple constant
            visit_expression(expr->lhs, repn, multiplier / rhs_repn.constval->eval());
        }
        else {
            // Dividing by a constant expression
            MutableNLPExpr lhs_repn;
            visit_expression(expr->lhs, lhs_repn, multiplier);

            repn.mutable_values
                = repn.mutable_values or lhs_repn.mutable_values or rhs_repn.mutable_values;

            repn.constval = plus_(repn.constval, divide_(lhs_repn.constval, rhs_repn.constval));

            repn.linear_vars.insert(repn.linear_vars.end(), lhs_repn.linear_vars.begin(),
                                    lhs_repn.linear_vars.end());
            for (size_t i = 0; i < lhs_repn.linear_coefs.size(); i++)
                repn.linear_coefs.push_back(divide_(lhs_repn.linear_coefs[i], rhs_repn.constval));

            repn.quadratic_lvars.insert(repn.quadratic_lvars.end(),
                                        lhs_repn.quadratic_lvars.begin(),
                                        lhs_repn.quadratic_lvars.end());
            repn.quadratic_rvars.insert(repn.quadratic_rvars.end(),
                                        lhs_repn.quadratic_rvars.begin(),
                                        lhs_repn.quadratic_rvars.end());
            for (size_t i = 0; i < lhs_repn.quadratic_coefs.size(); i++)
                repn.quadratic_coefs.push_back(
                    divide_(lhs_repn.quadratic_coefs[i], rhs_repn.constval));

            repn.nonlinear = plus_(repn.nonlinear, divide_(lhs_repn.nonlinear, rhs_repn.constval));
        }
        return;
    }

    // Dividing by a variable expression
    MutableNLPExpr lhs_repn;
    visit_expression(expr->lhs, lhs_repn, multiplier);
    if (((lhs_repn.linear_coefs.size() + lhs_repn.quadratic_coefs.size()) == 0)
        and (lhs_repn.nonlinear == ZEROCONST)) {
        if (lhs_repn.constval->is_constant() and (lhs_repn.constval->eval() == 0)) return;
    }
    repn.mutable_values = repn.mutable_values or lhs_repn.mutable_values or rhs_repn.mutable_values;
    repn.nonlinear = plus_(repn.nonlinear, expr);
    std::unordered_set<std::shared_ptr<VariableTerm>> exprvars;
    find_variables(expr, exprvars);
    repn.nonlinear_vars.insert(exprvars.begin(), exprvars.end());
}

#define UNARY_VISITOR(TERM, FN)                                                                 \
    void visit(std::shared_ptr<TERM>& expr, MutableNLPExpr& repn, double multiplier)            \
    {                                                                                           \
        MutableNLPExpr body_repn;                                                               \
        visit_expression(expr->body, body_repn, 1.0);                                           \
        if ((body_repn.linear_coefs.size() == 0) and (body_repn.quadratic_coefs.size() == 0)    \
            and (body_repn.nonlinear == ZEROCONST)) {                                           \
            repn.constval = plus(repn.constval, intrinsic_##FN(body_repn.constval));            \
            return;                                                                             \
        }                                                                                       \
        if (multiplier == 1)                                                                    \
            repn.nonlinear = plus(repn.nonlinear, expr);                                        \
        else                                                                                    \
            repn.nonlinear                                                                      \
                = plus(repn.nonlinear, times(CREATE_POINTER(ConstantTerm, multiplier), expr));  \
        repn.mutable_values = repn.mutable_values or body_repn.mutable_values;                  \
        repn.nonlinear_vars.insert(body_repn.linear_vars.begin(), body_repn.linear_vars.end()); \
        repn.nonlinear_vars.insert(body_repn.quadratic_lvars.begin(),                           \
                                   body_repn.quadratic_lvars.end());                            \
        repn.nonlinear_vars.insert(body_repn.quadratic_rvars.begin(),                           \
                                   body_repn.quadratic_rvars.end());                            \
        repn.nonlinear_vars.insert(body_repn.nonlinear_vars.begin(),                            \
                                   body_repn.nonlinear_vars.end());                             \
    }

// clang-format off
UNARY_VISITOR(AbsTerm, abs)
UNARY_VISITOR(CeilTerm, ceil)
UNARY_VISITOR(FloorTerm, floor)
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
// clang-format on

#define BINARY_VISITOR(TERM, FN)                                                                \
    void visit(std::shared_ptr<TERM>& expr, MutableNLPExpr& repn, double multiplier)            \
    {                                                                                           \
        MutableNLPExpr lhs_repn;                                                                \
        visit_expression(expr->lhs, lhs_repn, 1.0);                                             \
        MutableNLPExpr rhs_repn;                                                                \
        visit_expression(expr->rhs, rhs_repn, 1.0);                                             \
        if ((lhs_repn.linear_coefs.size() == 0) and (lhs_repn.quadratic_coefs.size() == 0)      \
            and (lhs_repn.nonlinear == ZEROCONST) and (rhs_repn.linear_coefs.size() == 0)       \
            and (rhs_repn.quadratic_coefs.size() == 0) and (rhs_repn.nonlinear == ZEROCONST)) { \
            repn.constval                                                                       \
                = plus(repn.constval, intrinsic_##FN(lhs_repn.constval, rhs_repn.constval));    \
            return;                                                                             \
        }                                                                                       \
        if (multiplier == 1)                                                                    \
            repn.nonlinear = plus(repn.nonlinear, expr);                                        \
        else                                                                                    \
            repn.nonlinear                                                                      \
                = plus(repn.nonlinear, times(CREATE_POINTER(ConstantTerm, multiplier), expr));  \
        repn.mutable_values                                                                     \
            = repn.mutable_values or lhs_repn.mutable_values or rhs_repn.mutable_values;        \
        repn.nonlinear_vars.insert(lhs_repn.linear_vars.begin(), lhs_repn.linear_vars.end());   \
        repn.nonlinear_vars.insert(lhs_repn.quadratic_lvars.begin(),                            \
                                   lhs_repn.quadratic_lvars.end());                             \
        repn.nonlinear_vars.insert(lhs_repn.quadratic_rvars.begin(),                            \
                                   lhs_repn.quadratic_rvars.end());                             \
        repn.nonlinear_vars.insert(lhs_repn.nonlinear_vars.begin(),                             \
                                   lhs_repn.nonlinear_vars.end());                              \
                                                                                                \
        repn.nonlinear_vars.insert(rhs_repn.linear_vars.begin(), rhs_repn.linear_vars.end());   \
        repn.nonlinear_vars.insert(rhs_repn.quadratic_lvars.begin(),                            \
                                   rhs_repn.quadratic_lvars.end());                             \
        repn.nonlinear_vars.insert(rhs_repn.quadratic_rvars.begin(),                            \
                                   rhs_repn.quadratic_rvars.end());                             \
        repn.nonlinear_vars.insert(rhs_repn.nonlinear_vars.begin(),                             \
                                   rhs_repn.nonlinear_vars.end());                              \
    }

BINARY_VISITOR(PowTerm, pow)

#define VISIT_CASE(TERM)                          \
    case TERM##_id: {                             \
        auto tmp = safe_pointer_cast<TERM>(expr); \
        visit(tmp, repn, multiplier);             \
    } break

void visit_expression(const expr_pointer_t& expr, MutableNLPExpr& repn, double multiplier)
{
    switch (expr->id()) {
        VISIT_CASE(ConstantTerm);
        VISIT_CASE(ParameterTerm);
        VISIT_CASE(IndexParameterTerm);
        VISIT_CASE(VariableTerm);
#ifdef COEK_WITH_COMPACT_MODEL
        VISIT_CASE(VariableRefTerm);
#endif
        VISIT_CASE(MonomialTerm);
        VISIT_CASE(InequalityTerm);
        VISIT_CASE(EqualityTerm);
        VISIT_CASE(ObjectiveTerm);
        VISIT_CASE(SubExpressionTerm);
        VISIT_CASE(NegateTerm);
        VISIT_CASE(PlusTerm);
        VISIT_CASE(TimesTerm);
        VISIT_CASE(DivideTerm);
        VISIT_CASE(AbsTerm);
        VISIT_CASE(CeilTerm);
        VISIT_CASE(FloorTerm);
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

        // GCOVR_EXCL_START
        default:
            throw std::runtime_error(
                "Error in MutableNLPExpr visitor!  Visiting unexpected expression term "
                + std::to_string(expr->id()));
            // GCOVR_EXCL_STOP
    };
}

}  // namespace

void to_MutableNLPExpr(const expr_pointer_t& expr, MutableNLPExpr& repn)
{
    visit_expression(expr, repn, 1.0);
}

}  // namespace coek
