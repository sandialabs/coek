#include <algorithm>
#include <unordered_set>

#include "base_terms.hpp"
#include "constraint_terms.hpp"
#include "expr_terms.hpp"
#include "value_terms.hpp"
#include "visitor.hpp"
#include "visitor_fns.hpp"
#ifdef COEK_WITH_COMPACT_MODEL
#    include "compact_terms.hpp"
#endif

#include "ast_operators.hpp"
#include "coek/api/expression.hpp"
#include "coek/api/expression_visitor.hpp"

namespace coek {

MutableNLPExpr::MutableNLPExpr() : mutable_values(false) {}

MutableNLPExpr::~MutableNLPExpr() {}

namespace {

void visit_expression(expr_pointer_t expr, MutableNLPExpr& repn, double multiplier);

void visit(ConstantTerm& expr, MutableNLPExpr& repn, double multiplier)
{
    repn.constval
        = plus_(repn.constval.repn, CREATE_POINTER(ConstantTerm, multiplier * expr.value));
}

void visit(ParameterTerm& expr, MutableNLPExpr& repn, double multiplier)
{
    if (multiplier == 1)
        repn.constval = plus_(repn.constval.repn, &expr);
    else
        repn.constval
            = plus_(repn.constval.repn, times(CREATE_POINTER(ConstantTerm, multiplier), &expr));
    repn.mutable_values = true;
}

void visit(IndexParameterTerm& /*expr*/, MutableNLPExpr& /*repn*/, double /*multiplier*/)
{
    throw std::runtime_error("Unexpected index parameter.");
}

void visit(VariableTerm& expr, MutableNLPExpr& repn, double multiplier)
{
    // if (! expr.index)
    //     throw std::runtime_error("Unexpected variable not owned by a model.");

    if (expr.fixed) {
        if (multiplier == 1)
            repn.constval = plus_(repn.constval.repn, &expr);
        else
            repn.constval
                = plus_(repn.constval.repn, times(CREATE_POINTER(ConstantTerm, multiplier), &expr));
        repn.mutable_values = true;
    }
    else {
        repn.linear_vars.push_back(&expr);
        if (multiplier == 1) {
            repn.linear_coefs.push_back(ONECONST);
        }
        else {
            repn.linear_coefs.push_back(CREATE_POINTER(ConstantTerm, multiplier));
        }
    }
}

#ifdef COEK_WITH_COMPACT_MODEL
void visit(VariableRefTerm& /*expr*/, MutableNLPExpr& /*repn*/, double /*multiplier*/)
{
    throw std::runtime_error("Unexpected variable reference.");
}
#endif

void visit(MonomialTerm& expr, MutableNLPExpr& repn, double multiplier)
{
    // if (! expr.var->index)
    //     throw std::runtime_error("Unexpected variable not owned by a model.");

    if (expr.var->fixed) {
        repn.constval
            = plus_(repn.constval.repn,
                    times(CREATE_POINTER(ConstantTerm, multiplier * expr.coef), expr.var));
        repn.mutable_values = true;
    }
    else {
        repn.linear_vars.push_back(expr.var);
        repn.linear_coefs.push_back(CREATE_POINTER(ConstantTerm, multiplier * expr.coef));
    }
}

void visit(InequalityTerm& expr, MutableNLPExpr& repn, double multiplier)
{
    visit_expression(expr.body, repn, multiplier);
}

void visit(EqualityTerm& expr, MutableNLPExpr& repn, double multiplier)
{
    visit_expression(expr.body, repn, multiplier);
}

void visit(ObjectiveTerm& expr, MutableNLPExpr& repn, double multiplier)
{
    visit_expression(expr.body, repn, multiplier);
}

void visit(NegateTerm& expr, MutableNLPExpr& repn, double multiplier)
{
    visit_expression(expr.body, repn, -multiplier);
}

void visit(PlusTerm& expr, MutableNLPExpr& repn, double multiplier)
{
    NAryPrefixTerm::shared_t::iterator it = expr.data.get()->begin();
    NAryPrefixTerm::shared_t::iterator end = expr.data.get()->end();

    for (; it != end; ++it) visit_expression(*it, repn, multiplier);
}

void visit(TimesTerm& expr, MutableNLPExpr& repn, double multiplier)
{
    MutableNLPExpr lhs_repn;
    visit_expression(expr.lhs, lhs_repn, multiplier);

    // LHS is a simple constant
    if ((lhs_repn.linear_coefs.size() == 0) and (lhs_repn.quadratic_coefs.size() == 0)
        and (lhs_repn.nonlinear.repn == ZEROCONST) and lhs_repn.constval.is_constant()) {
        if (lhs_repn.constval.repn != ZEROCONST)
            visit_expression(expr.rhs, repn, lhs_repn.constval.value());
        return;
    }

    MutableNLPExpr rhs_repn;
    visit_expression(expr.rhs, rhs_repn, 1.0);

    repn.mutable_values = repn.mutable_values or lhs_repn.mutable_values or rhs_repn.mutable_values;

    // Don't expand expressions with cubic or nonlinear terms
    // Don't expand products of linear terms, unless they are "simple"  (e.g. x*(y+z) )
    int lhs_mindegree = std::max(lhs_repn.linear_coefs.size() == 0 ? 0 : 1,
                                 std::max(lhs_repn.quadratic_coefs.size() == 0 ? 0 : 2,
                                          lhs_repn.nonlinear.repn == ZEROCONST ? 0 : 4));
    int rhs_mindegree = std::max(rhs_repn.linear_coefs.size() == 0 ? 0 : 1,
                                 std::max(rhs_repn.quadratic_coefs.size() == 0 ? 0 : 2,
                                          rhs_repn.nonlinear.repn == ZEROCONST ? 0 : 4));
    if (((lhs_mindegree + rhs_mindegree) > 2) or  // Creating 3rd-degree polynomial
        (std::min(lhs_repn.linear_coefs.size(), rhs_repn.linear_coefs.size())
         > 1)) {  // Creating product of linear terms
        repn.nonlinear = plus_(repn.nonlinear.repn, &expr);
        std::unordered_set<VariableTerm*> exprvars;
        find_variables(&expr, exprvars);
        repn.nonlinear_vars.insert(exprvars.begin(), exprvars.end());
        return;
    }

    // CONSTANT * CONSTANT
    if (not((lhs_repn.constval.repn == ZEROCONST) or (rhs_repn.constval.repn == ZEROCONST)))
        repn.constval = times_(lhs_repn.constval.repn, rhs_repn.constval.repn);

    if (not(lhs_repn.constval.repn == ZEROCONST)) {
        // CONSTANT * LINEAR
        repn.linear_vars.insert(repn.linear_vars.end(), rhs_repn.linear_vars.begin(),
                                rhs_repn.linear_vars.end());
        for (size_t i = 0; i < rhs_repn.linear_coefs.size(); i++)
            repn.linear_coefs.push_back(
                times_(lhs_repn.constval.repn, rhs_repn.linear_coefs[i].repn));
        // CONSTANT * QUADRATIC
        repn.quadratic_lvars.insert(repn.quadratic_lvars.end(), rhs_repn.quadratic_lvars.begin(),
                                    rhs_repn.quadratic_lvars.end());
        repn.quadratic_rvars.insert(repn.quadratic_rvars.end(), rhs_repn.quadratic_rvars.begin(),
                                    rhs_repn.quadratic_rvars.end());
        for (size_t i = 0; i < rhs_repn.quadratic_coefs.size(); i++)
            repn.quadratic_coefs.push_back(
                times_(rhs_repn.quadratic_coefs[i].repn, lhs_repn.constval.repn));
    }
    if (not(rhs_repn.constval.repn == ZEROCONST)) {
        // LINEAR * CONSTANT
        repn.linear_vars.insert(repn.linear_vars.end(), lhs_repn.linear_vars.begin(),
                                lhs_repn.linear_vars.end());
        for (size_t i = 0; i < lhs_repn.linear_coefs.size(); i++)
            repn.linear_coefs.push_back(
                times_(rhs_repn.constval.repn, lhs_repn.linear_coefs[i].repn));
        // QUADRATIC * CONSTANT
        repn.quadratic_lvars.insert(repn.quadratic_lvars.end(), lhs_repn.quadratic_lvars.begin(),
                                    lhs_repn.quadratic_lvars.end());
        repn.quadratic_rvars.insert(repn.quadratic_rvars.end(), lhs_repn.quadratic_rvars.begin(),
                                    lhs_repn.quadratic_rvars.end());
        for (size_t i = 0; i < lhs_repn.quadratic_coefs.size(); i++)
            repn.quadratic_coefs.push_back(
                times_(lhs_repn.quadratic_coefs[i].repn, rhs_repn.constval.repn));
    }

    // LINEAR * LINEAR
    for (size_t i = 0; i < lhs_repn.linear_coefs.size(); i++) {
        for (size_t j = 0; j < rhs_repn.linear_coefs.size(); j++) {
            repn.quadratic_lvars.push_back(lhs_repn.linear_vars[i]);
            repn.quadratic_rvars.push_back(rhs_repn.linear_vars[j]);
            repn.quadratic_coefs.push_back(
                times_(lhs_repn.linear_coefs[i].repn, rhs_repn.linear_coefs[j].repn));
        }
    }
}

void visit(DivideTerm& expr, MutableNLPExpr& repn, double multiplier)
{
    MutableNLPExpr rhs_repn;
    visit_expression(expr.rhs, rhs_repn, 1.0);

    if (((rhs_repn.linear_coefs.size() + rhs_repn.quadratic_coefs.size()) == 0)
        and (rhs_repn.nonlinear.repn == ZEROCONST)) {
        // Dividing by a constant expression
        if (rhs_repn.constval.is_constant()) {
            if (rhs_repn.constval.value() == 0) {
                throw std::runtime_error("Division by zero error.");
            }
            // Dividing by a simple constant
            visit_expression(expr.lhs, repn, multiplier / rhs_repn.constval.value());
        }
        else {
            // Dividing by a constant expression
            MutableNLPExpr lhs_repn;
            visit_expression(expr.lhs, lhs_repn, multiplier);

            repn.mutable_values
                = repn.mutable_values or lhs_repn.mutable_values or rhs_repn.mutable_values;

            repn.constval = plus_(repn.constval.repn,
                                  divide_(lhs_repn.constval.repn, rhs_repn.constval.repn));

            repn.linear_vars.insert(repn.linear_vars.end(), lhs_repn.linear_vars.begin(),
                                    lhs_repn.linear_vars.end());
            for (size_t i = 0; i < lhs_repn.linear_coefs.size(); i++)
                repn.linear_coefs.push_back(
                    divide_(lhs_repn.linear_coefs[i].repn, rhs_repn.constval.repn));

            repn.quadratic_lvars.insert(repn.quadratic_lvars.end(),
                                        lhs_repn.quadratic_lvars.begin(),
                                        lhs_repn.quadratic_lvars.end());
            repn.quadratic_rvars.insert(repn.quadratic_rvars.end(),
                                        lhs_repn.quadratic_rvars.begin(),
                                        lhs_repn.quadratic_rvars.end());
            for (size_t i = 0; i < lhs_repn.quadratic_coefs.size(); i++)
                repn.quadratic_coefs.push_back(
                    divide_(lhs_repn.quadratic_coefs[i].repn, rhs_repn.constval.repn));

            repn.nonlinear = plus_(repn.nonlinear.repn,
                                   divide_(lhs_repn.nonlinear.repn, rhs_repn.constval.repn));
        }
        return;
    }

    // Dividing by a variable expression
    MutableNLPExpr lhs_repn;
    visit_expression(expr.lhs, lhs_repn, multiplier);
    if (((lhs_repn.linear_coefs.size() + lhs_repn.quadratic_coefs.size()) == 0)
        and (lhs_repn.nonlinear.repn == ZEROCONST)) {
        if (lhs_repn.constval.is_constant() and (lhs_repn.constval.value() == 0)) return;
    }
    repn.mutable_values = repn.mutable_values or lhs_repn.mutable_values or rhs_repn.mutable_values;
    repn.nonlinear = plus_(repn.nonlinear.repn, &expr);
    std::unordered_set<VariableTerm*> exprvars;
    find_variables(&expr, exprvars);
    repn.nonlinear_vars.insert(exprvars.begin(), exprvars.end());
}

#define UNARY_VISITOR(TERM, FN)                                                                 \
    void visit(TERM& expr, MutableNLPExpr& repn, double multiplier)                             \
    {                                                                                           \
        MutableNLPExpr body_repn;                                                               \
        visit_expression(expr.body, body_repn, 1.0);                                            \
        if ((body_repn.linear_coefs.size() == 0) and (body_repn.quadratic_coefs.size() == 0)    \
            and (body_repn.nonlinear.repn == ZEROCONST)) {                                      \
            repn.constval = plus(repn.constval.repn, intrinsic_##FN(body_repn.constval.repn));  \
            return;                                                                             \
        }                                                                                       \
        if (multiplier == 1)                                                                    \
            repn.nonlinear = plus(repn.nonlinear.repn, &expr);                                  \
        else                                                                                    \
            repn.nonlinear = plus(repn.nonlinear.repn,                                          \
                                  times(CREATE_POINTER(ConstantTerm, multiplier), &expr));      \
        repn.mutable_values = repn.mutable_values or body_repn.mutable_values;                  \
        repn.nonlinear_vars.insert(body_repn.linear_vars.begin(), body_repn.linear_vars.end()); \
        repn.nonlinear_vars.insert(body_repn.quadratic_lvars.begin(),                           \
                                   body_repn.quadratic_lvars.end());                            \
        repn.nonlinear_vars.insert(body_repn.quadratic_rvars.begin(),                           \
                                   body_repn.quadratic_rvars.end());                            \
        repn.nonlinear_vars.insert(body_repn.nonlinear_vars.begin(),                            \
                                   body_repn.nonlinear_vars.end());                             \
    }

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

#define BINARY_VISITOR(TERM, FN)                                                                  \
    void visit(TERM& expr, MutableNLPExpr& repn, double multiplier)                               \
    {                                                                                             \
        MutableNLPExpr lhs_repn;                                                                  \
        visit_expression(expr.lhs, lhs_repn, 1.0);                                                \
        MutableNLPExpr rhs_repn;                                                                  \
        visit_expression(expr.rhs, rhs_repn, 1.0);                                                \
        if ((lhs_repn.linear_coefs.size() == 0) and (lhs_repn.quadratic_coefs.size() == 0)        \
            and (lhs_repn.nonlinear.repn == ZEROCONST) and (rhs_repn.linear_coefs.size() == 0)    \
            and (rhs_repn.quadratic_coefs.size() == 0)                                            \
            and (rhs_repn.nonlinear.repn == ZEROCONST)) {                                         \
            repn.constval = plus(repn.constval.repn,                                              \
                                 intrinsic_##FN(lhs_repn.constval.repn, rhs_repn.constval.repn)); \
            return;                                                                               \
        }                                                                                         \
        if (multiplier == 1)                                                                      \
            repn.nonlinear = plus(repn.nonlinear.repn, &expr);                                    \
        else                                                                                      \
            repn.nonlinear = plus(repn.nonlinear.repn,                                            \
                                  times(CREATE_POINTER(ConstantTerm, multiplier), &expr));        \
        repn.mutable_values                                                                       \
            = repn.mutable_values or lhs_repn.mutable_values or rhs_repn.mutable_values;          \
        repn.nonlinear_vars.insert(lhs_repn.linear_vars.begin(), lhs_repn.linear_vars.end());     \
        repn.nonlinear_vars.insert(lhs_repn.quadratic_lvars.begin(),                              \
                                   lhs_repn.quadratic_lvars.end());                               \
        repn.nonlinear_vars.insert(lhs_repn.quadratic_rvars.begin(),                              \
                                   lhs_repn.quadratic_rvars.end());                               \
        repn.nonlinear_vars.insert(lhs_repn.nonlinear_vars.begin(),                               \
                                   lhs_repn.nonlinear_vars.end());                                \
                                                                                                  \
        repn.nonlinear_vars.insert(rhs_repn.linear_vars.begin(), rhs_repn.linear_vars.end());     \
        repn.nonlinear_vars.insert(rhs_repn.quadratic_lvars.begin(),                              \
                                   rhs_repn.quadratic_lvars.end());                               \
        repn.nonlinear_vars.insert(rhs_repn.quadratic_rvars.begin(),                              \
                                   rhs_repn.quadratic_rvars.end());                               \
        repn.nonlinear_vars.insert(rhs_repn.nonlinear_vars.begin(),                               \
                                   rhs_repn.nonlinear_vars.end());                                \
    }

BINARY_VISITOR(PowTerm, pow)

#define VISIT_CASE(TERM)                                     \
    case TERM##_id:                                          \
        visit(*dynamic_cast<TERM*>(expr), repn, multiplier); \
        break

void visit_expression(expr_pointer_t expr, MutableNLPExpr& repn, double multiplier)
{
    switch (expr->id()) {
        VISIT_CASE(ConstantTerm);
        VISIT_CASE(ParameterTerm);
        VISIT_CASE(IndexParameterTerm);
        VISIT_CASE(VariableTerm);
        VISIT_CASE(IndexedVariableTerm);
#ifdef COEK_WITH_COMPACT_MODEL
        VISIT_CASE(VariableRefTerm);
#endif
        VISIT_CASE(MonomialTerm);
        VISIT_CASE(InequalityTerm);
        VISIT_CASE(EqualityTerm);
        VISIT_CASE(ObjectiveTerm);
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

void to_MutableNLPExpr(expr_pointer_t expr, MutableNLPExpr& repn)
{
    visit_expression(expr, repn, 1.0);
}

}  // namespace coek
