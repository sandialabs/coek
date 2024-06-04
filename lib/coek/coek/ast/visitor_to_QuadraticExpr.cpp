#include "base_terms.hpp"
#include "constraint_terms.hpp"
#include "expr_terms.hpp"
#include "value_terms.hpp"
#include "visitor.hpp"
#include "visitor_fns.hpp"
#include "../util/cast_utils.hpp"
#ifdef COEK_WITH_COMPACT_MODEL
#    include "compact_terms.hpp"
#endif

#include "coek/api/exceptions.hpp"
#include "coek/api/expression.hpp"
#include "coek/api/expression_visitor.hpp"

namespace std {

std::ostream& operator<<(std::ostream& ostr, const coek::QuadraticExpr& arg)
{
    ostr << "Constant: " << arg.constval << std::endl;
    ostr << "Linear: " << std::endl;
    for (size_t i = 0; i < arg.linear_coefs.size(); i++) {
        ostr << arg.linear_coefs[i] << " ";
        write_expr(arg.linear_vars[i], ostr);
        ostr << std::endl;
    }
    ostr << "Quadratic: " << std::endl;
    for (size_t i = 0; i < arg.quadratic_coefs.size(); i++) {
        ostr << arg.quadratic_coefs[i] << " ";
        write_expr(arg.quadratic_lvars[i], ostr);
        ostr << " ";
        write_expr(arg.quadratic_rvars[i], ostr);
        ostr << std::endl;
    }
    return ostr;
}

}  // namespace std

namespace coek {

namespace {

void visit_expression(const expr_pointer_t& expr, QuadraticExpr& repn, double multiplier);

void visit(std::shared_ptr<ConstantTerm>& expr, QuadraticExpr& repn, double multiplier)
{
    repn.constval += multiplier * expr->value;
}

void visit(std::shared_ptr<ParameterTerm>& expr, QuadraticExpr& repn, double multiplier)
{
    repn.constval += multiplier * expr->value->eval();
}

void visit(std::shared_ptr<IndexParameterTerm>& /*expr*/, QuadraticExpr& /*repn*/,
           double /*multiplier*/)
{
    throw std::runtime_error("Unexpected index parameter.");
}

void visit(std::shared_ptr<VariableTerm>& expr, QuadraticExpr& repn, double multiplier)
{
    if (expr->fixed) {
        repn.constval += multiplier * expr->value->eval();
    }
    else {
        repn.linear_vars.push_back(expr);
        repn.linear_coefs.push_back(multiplier);
    }
}

#ifdef COEK_WITH_COMPACT_MODEL
void visit(std::shared_ptr<VariableRefTerm>& /*expr*/, QuadraticExpr& /*repn*/,
           double /*multiplier*/)
{
    throw std::runtime_error("Unexpected variable reference.");
}
#endif

void visit(std::shared_ptr<MonomialTerm>& expr, QuadraticExpr& repn, double multiplier)
{
    if (expr->var->fixed) {
        repn.constval += multiplier * expr->coef * expr->var->value->eval();
    }
    else {
        repn.linear_vars.push_back(expr->var);
        repn.linear_coefs.push_back(multiplier * expr->coef);
    }
}

void visit(std::shared_ptr<InequalityTerm>& expr, QuadraticExpr& repn, double multiplier)
{
    visit_expression(expr->body, repn, multiplier);
}

void visit(std::shared_ptr<StrictInequalityTerm>& expr, QuadraticExpr& repn, double multiplier)
{
    visit_expression(expr->body, repn, multiplier);
}

void visit(std::shared_ptr<EqualityTerm>& expr, QuadraticExpr& repn, double multiplier)
{
    visit_expression(expr->body, repn, multiplier);
}

void visit(std::shared_ptr<ObjectiveTerm>& expr, QuadraticExpr& repn, double multiplier)
{
    visit_expression(expr->body, repn, multiplier);
}

void visit(std::shared_ptr<SubExpressionTerm>& expr, QuadraticExpr& repn, double multiplier)
{
    visit_expression(expr->body, repn, multiplier);
}

void visit(std::shared_ptr<NegateTerm>& expr, QuadraticExpr& repn, double multiplier)
{
    visit_expression(expr->body, repn, -multiplier);
}

void visit(std::shared_ptr<PlusTerm>& expr, QuadraticExpr& repn, double multiplier)
{
    std::vector<expr_pointer_t>& vec = *(expr->data.get());
    auto n = expr->num_expressions();
    for (size_t i = 0; i < n; i++)
        visit_expression(vec[i], repn, multiplier);
}

void visit(std::shared_ptr<TimesTerm>& expr, QuadraticExpr& repn, double multiplier)
{
    QuadraticExpr lhs_repn;
    visit_expression(expr->lhs, lhs_repn, 1.0);

    if (lhs_repn.linear_coefs.size() + lhs_repn.quadratic_coefs.size() == 0) {
        //
        // LHS is a constant
        //
        if (lhs_repn.constval == 0.0)
            return;

        visit_expression(expr->rhs, repn, lhs_repn.constval * multiplier);
        return;
    }

    QuadraticExpr rhs_repn;
    visit_expression(expr->rhs, rhs_repn, 1.0);

    if (rhs_repn.linear_coefs.size() + rhs_repn.quadratic_coefs.size() == 0) {
        //
        // RHS is a constant
        //
        if (rhs_repn.constval == 0.0)
            return;

        multiplier *= rhs_repn.constval;

        repn.constval += multiplier * lhs_repn.constval;
        repn.linear_vars.insert(repn.linear_vars.end(), lhs_repn.linear_vars.begin(),
                                lhs_repn.linear_vars.end());
        repn.quadratic_lvars.insert(repn.quadratic_lvars.end(), lhs_repn.quadratic_lvars.begin(),
                                    lhs_repn.quadratic_lvars.end());
        repn.quadratic_rvars.insert(repn.quadratic_rvars.end(), lhs_repn.quadratic_rvars.begin(),
                                    lhs_repn.quadratic_rvars.end());
        for (size_t i = 0; i < lhs_repn.linear_coefs.size(); i++)
            repn.linear_coefs.push_back(lhs_repn.linear_coefs[i] * multiplier);
        for (size_t i = 0; i < lhs_repn.quadratic_coefs.size(); i++)
            repn.quadratic_coefs.push_back(lhs_repn.quadratic_coefs[i] * multiplier);
        return;
    }

    //
    // LHS and RHS are non-constant
    //
    repn.constval += multiplier * lhs_repn.constval * rhs_repn.constval;

    if (lhs_repn.constval != 0.0) {
        repn.linear_vars.insert(repn.linear_vars.end(), rhs_repn.linear_vars.begin(),
                                rhs_repn.linear_vars.end());
        for (size_t i = 0; i < rhs_repn.linear_coefs.size(); i++)
            repn.linear_coefs.push_back(multiplier * lhs_repn.constval * rhs_repn.linear_coefs[i]);
    }
    if (rhs_repn.constval != 0.0) {
        repn.linear_vars.insert(repn.linear_vars.end(), lhs_repn.linear_vars.begin(),
                                lhs_repn.linear_vars.end());
        for (size_t i = 0; i < lhs_repn.linear_coefs.size(); i++)
            repn.linear_coefs.push_back(multiplier * lhs_repn.linear_coefs[i] * rhs_repn.constval);
    }
    for (size_t i = 0; i < lhs_repn.linear_coefs.size(); i++) {
        for (size_t j = 0; j < rhs_repn.linear_coefs.size(); j++) {
            repn.quadratic_lvars.push_back(lhs_repn.linear_vars[i]);
            repn.quadratic_rvars.push_back(rhs_repn.linear_vars[j]);
            repn.quadratic_coefs.push_back(multiplier * lhs_repn.linear_coefs[i]
                                           * rhs_repn.linear_coefs[j]);
        }
    }

    if ((lhs_repn.quadratic_coefs.size() > 0)
        and ((rhs_repn.linear_coefs.size() + rhs_repn.quadratic_coefs.size()) > 0))
        throw exceptions::NonquadraticExpression(
            "Non-quadratic expressions cannot be expressed in a QuadraticExpr object.");
    if ((rhs_repn.quadratic_coefs.size() > 0) and (lhs_repn.linear_coefs.size() > 0))
        throw exceptions::NonquadraticExpression(
            "Non-quadratic expressions cannot be expressed in a QuadraticExpr object.");
}

void visit(std::shared_ptr<DivideTerm>& expr, QuadraticExpr& repn, double multiplier)
{
    visit_expression(expr->lhs, repn, multiplier);

    // LHS is zero, so we ignore the RHS
    if ((repn.constval == 0.0) and ((repn.linear_coefs.size() + repn.quadratic_coefs.size()) == 0))
        return;

    QuadraticExpr rhs_repn;
    visit_expression(expr->rhs, rhs_repn, 1.0);

    if ((rhs_repn.linear_coefs.size() + rhs_repn.quadratic_coefs.size()) > 0)
        throw exceptions::NonquadraticExpression(
            "Non-constant expressions cannot appear in the denominator of quadratic expressions.");
    if (rhs_repn.constval == 0.0)
        throw std::runtime_error("Division by zero error.");

    // Divide the the rhs value
    repn.constval /= rhs_repn.constval;
    for (size_t i = 0; i < repn.linear_coefs.size(); i++)
        repn.linear_coefs[i] /= rhs_repn.constval;
    for (size_t i = 0; i < repn.quadratic_coefs.size(); i++)
        repn.quadratic_coefs[i] /= rhs_repn.constval;
}

void visit(std::shared_ptr<IfThenElseTerm>& expr, QuadraticExpr& repn, double multiplier)
{
    QuadraticExpr cond_repn;
    visit_expression(expr->cond_expr, cond_repn, 1.0);
    if ((cond_repn.linear_coefs.size() + cond_repn.quadratic_coefs.size()) > 0)
        throw exceptions::NonquadraticExpression(
            "Non-constant expressions in the condition of an if-then-else expression are "
            "non-quadratic.");

    if (cond_repn.constval > (1.0 - 1e-7))
        visit_expression(expr->then_expr, repn, multiplier);
    else
        visit_expression(expr->else_expr, repn, multiplier);
}

#define UNARY_VISITOR(TERM, FN)                                                                  \
    void visit(std::shared_ptr<TERM>& expr, QuadraticExpr& repn, double multiplier)              \
    {                                                                                            \
        QuadraticExpr body_repn;                                                                 \
        visit_expression(expr->body, body_repn, 1.0);                                            \
                                                                                                 \
        if ((body_repn.linear_coefs.size() + body_repn.quadratic_coefs.size()) > 0)              \
            throw exceptions::NonquadraticExpression(                                            \
                "Nonlinear expressions are not supported for QuadraticExpr: " + std::string(#FN) \
                + " term.");                                                                     \
                                                                                                 \
        repn.constval += multiplier * ::FN(body_repn.constval);                                  \
    }

// clang-format off
UNARY_VISITOR(AbsTerm, fabs)
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

#define BINARY_VISITOR(TERM, FN)                                                                 \
    void visit(std::shared_ptr<TERM>& expr, QuadraticExpr& repn, double multiplier)              \
    {                                                                                            \
        QuadraticExpr lhs_repn;                                                                  \
        visit_expression(expr.lhs, lhs_repn, 1.0);                                               \
        if ((lhs_repn.linear_coefs.size() + lhs_repn.quadratic_coefs.size()) > 0)                \
            throw exceptions::NonquadraticExpression(                                            \
                "Nonlinear expressions are not supported for QuadraticExpr: " + std::string(#FN) \
                + " term.");                                                                     \
                                                                                                 \
        QuadraticExpr rhs_repn;                                                                  \
        visit_expression(expr.rhs, rhs_repn, 1.0);                                               \
        if ((rhs_repn.linear_coefs.size() + rhs_repn.quadratic_coefs.size()) > 0)                \
            throw exceptions::NonquadraticExpression(                                            \
                "Nonlinear expressions are not supported for QuadraticExpr: " + std::string(#FN) \
                + " term.");                                                                     \
                                                                                                 \
        repn.constval += multiplier * ::FN(lhs_repn.constval, rhs_repn.constval);                \
    }

void visit(std::shared_ptr<PowTerm>& expr, QuadraticExpr& repn, double multiplier)
{
    QuadraticExpr rhs_repn;
    visit_expression(expr->rhs, rhs_repn, 1.0);
    if (not rhs_repn.is_constant())
        throw exceptions::NonquadraticExpression(
            "Nonlinear expressions are not supported for QuadraticExpr: pow term with non-constant "
            "exponent.");

    if (rhs_repn.constval == 0) {
        repn.constval += multiplier;
    }

    else if (rhs_repn.constval == 1) {
        visit_expression(expr->lhs, repn, multiplier);
    }

    else {
        QuadraticExpr lhs_repn;
        visit_expression(expr->lhs, lhs_repn, 1.0);
        if (lhs_repn.is_constant())
            // A**B - A and B constant
            repn.constval += multiplier * ::pow(lhs_repn.constval, rhs_repn.constval);

        else if (lhs_repn.is_linear() and (rhs_repn.constval == 2)) {
            // A**B - A linear and B=2
            // Quadratic
            for (std::size_t i = 0; i < lhs_repn.linear_coefs.size(); i++)
                for (std::size_t j = 0; j < lhs_repn.linear_coefs.size(); j++) {
                    repn.quadratic_coefs.push_back(multiplier * lhs_repn.linear_coefs[i]
                                                   * lhs_repn.linear_coefs[j]);
                    repn.quadratic_lvars.push_back(lhs_repn.linear_vars[i]);
                    repn.quadratic_rvars.push_back(lhs_repn.linear_vars[j]);
                }
            // Linear
            repn.linear_vars.insert(repn.linear_vars.end(), lhs_repn.linear_vars.begin(),
                                    lhs_repn.linear_vars.end());
            for (std::size_t i = 0; i < lhs_repn.linear_coefs.size(); i++)
                repn.linear_coefs.push_back(2 * multiplier * lhs_repn.linear_coefs[i]
                                            * lhs_repn.constval);
            // Constant
            repn.constval += multiplier * lhs_repn.constval * lhs_repn.constval;
        }

        else {
            throw exceptions::NonquadraticExpression(
                "Nonlinear expressions are not supported for QuadraticExpr: pow term with "
                "nonlinear base or constant exponent other than 2.");
        }
    }
}

// BINARY_VISITOR(PowTerm, pow)

#define VISIT_CASE(TERM)                          \
    case TERM##_id: {                             \
        auto tmp = safe_pointer_cast<TERM>(expr); \
        visit(tmp, repn, multiplier);             \
    } break

void visit_expression(const expr_pointer_t& expr, QuadraticExpr& repn, double multiplier)
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
        VISIT_CASE(StrictInequalityTerm);
        VISIT_CASE(EqualityTerm);
        VISIT_CASE(ObjectiveTerm);
        VISIT_CASE(SubExpressionTerm);
        VISIT_CASE(NegateTerm);
        VISIT_CASE(PlusTerm);
        VISIT_CASE(TimesTerm);
        VISIT_CASE(DivideTerm);
        VISIT_CASE(IfThenElseTerm);
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
                "Error in QuadraticExpr visitor!  Visiting unexpected expression term "
                + std::to_string(expr->id()));
            // GCOVR_EXCL_STOP
    };
}

}  // namespace

void to_QuadraticExpr(const expr_pointer_t& expr, QuadraticExpr& repn)
{
    visit_expression(expr, repn, 1.0);
}

}  // namespace coek
