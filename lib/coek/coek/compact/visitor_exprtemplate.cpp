#include <variant>

#include "../ast/compact_terms.hpp"
#include "../ast/constraint_terms.hpp"
#include "../ast/expr_terms.hpp"
#include "../util/cast_utils.hpp"
#include "coek/api/expression.hpp"
#include "coek_exprterm.hpp"

namespace coek {

namespace convert_expr_visitor {

expr_pointer_t visit_expression(const expr_pointer_t& expr);
expr_pointer_t visit_SumExpressionTerm(const expr_pointer_t& expr);

// ConstantTerm ignored

// ParameterTerm ignored

expr_pointer_t visit_IndexParameterTerm(const expr_pointer_t& expr)
{
    auto tmp = safe_pointer_cast<IndexParameterTerm>(expr);

    //  TODO - embed this logic in the IndexParameterTerm class
    if (tmp->type == 1)
        return std::make_shared<ConstantTerm>(tmp->double_value);
    else if (tmp->type == 2)
        return std::make_shared<ConstantTerm>(tmp->int_value);
    else
        throw std::runtime_error(
            "Unexpected index parameter in an expression being converted, with a non-numeric "
            "value.");
}

expr_pointer_t visit_VariableRefTerm(const expr_pointer_t& expr)
{
    auto tmp = safe_pointer_cast<VariableRefTerm>(expr);
    return tmp->get_concrete_variable();
}

expr_pointer_t visit_ParameterRefTerm(const expr_pointer_t& expr)
{
    auto tmp = safe_pointer_cast<ParameterRefTerm>(expr);
    return tmp->get_concrete_parameter();
}

expr_pointer_t visit_DataRefTerm(const expr_pointer_t& expr)
{
    auto tmp = safe_pointer_cast<DataRefTerm>(expr);
    return tmp->get_concrete_data();
}

expr_pointer_t visit_ObjectiveTerm(const expr_pointer_t& expr)
{
    auto tmp = safe_pointer_cast<ObjectiveTerm>(expr);
    auto body = visit_expression(tmp->body);
    if (body->id() == tmp->id())
        return expr;
    return std::make_shared<ObjectiveTerm>(body, tmp->sense);
}

expr_pointer_t visit_InequalityTerm(const expr_pointer_t& expr)
{
    auto tmp = safe_pointer_cast<InequalityTerm>(expr);
    auto lower = tmp->lower ? visit_expression(tmp->lower) : tmp->lower;
    auto body = visit_expression(tmp->body);
    auto upper = tmp->upper ? visit_expression(tmp->upper) : tmp->upper;
    if ((not tmp->lower or (lower->id() == tmp->lower->id())) and (body->id() == tmp->body->id())
        and (not tmp->upper or (upper->id() == tmp->upper->id())))
        return expr;
    return std::make_shared<InequalityTerm>(lower, body, upper);
}

expr_pointer_t visit_StrictInequalityTerm(const expr_pointer_t& expr)
{
    auto tmp = safe_pointer_cast<StrictInequalityTerm>(expr);
    auto lower = tmp->lower ? visit_expression(tmp->lower) : tmp->lower;
    auto body = visit_expression(tmp->body);
    auto upper = tmp->upper ? visit_expression(tmp->upper) : tmp->upper;
    if ((not tmp->lower or (lower->id() == tmp->lower->id())) and (body->id() == tmp->body->id())
        and (not tmp->upper or (upper->id() == tmp->upper->id())))
        return expr;
    return std::make_shared<StrictInequalityTerm>(lower, body, upper);
}

expr_pointer_t visit_EqualityTerm(const expr_pointer_t& expr)
{
    auto tmp = safe_pointer_cast<EqualityTerm>(expr);
    return std::make_shared<EqualityTerm>(visit_expression(tmp->body),
                                          visit_expression(tmp->lower));
}

expr_pointer_t visit_SubExpressionTerm(const expr_pointer_t& expr)
{
    auto tmp = safe_pointer_cast<SubExpressionTerm>(expr);
    return visit_expression(tmp->body);
}

expr_pointer_t visit_NegateTerm(const expr_pointer_t& expr)
{
    auto tmp = safe_pointer_cast<NegateTerm>(expr);
    auto body = visit_expression(tmp->body);
    if (body->id() == tmp->body->id())
        return expr;
    return std::make_shared<NegateTerm>(body);
}

expr_pointer_t visit_PlusTerm(const expr_pointer_t& expr)
{
    auto tmp = safe_pointer_cast<PlusTerm>(expr);

    auto data = *(tmp->data);
    auto lhs = visit_expression(data[0]);
    auto curr = visit_expression(data[1]);
    if (tmp->n == 2) {
        if ((lhs->id() == data[0]->id()) and (curr->id() == data[1]->id()))
            return expr;
        return std::make_shared<PlusTerm>(lhs, curr, false);
    }

    auto _curr = std::make_shared<PlusTerm>(lhs, curr, false);
    bool flag = (lhs->id() == data[0]->id()) and (curr->id() == data[1]->id());

    for (size_t i = 2; i < tmp->num_expressions(); i++) {
        auto curr = visit_expression(data[i]);
        _curr->push_back(curr);
        flag = flag and (curr->id() == data[i]->id());
    }
    if (flag)
        return expr;
    return _curr;
}

expr_pointer_t visit_TimesTerm(const expr_pointer_t& expr)
{
    auto tmp = safe_pointer_cast<TimesTerm>(expr);
    auto lhs = visit_expression(tmp->lhs);
    auto rhs = visit_expression(tmp->rhs);
    if ((lhs->id() == tmp->lhs->id()) and (rhs->id() == tmp->rhs->id()))
        return expr;
    return std::make_shared<TimesTerm>(lhs, rhs);
}

expr_pointer_t visit_DivideTerm(const expr_pointer_t& expr)
{
    auto tmp = safe_pointer_cast<DivideTerm>(expr);
    auto lhs = visit_expression(tmp->lhs);
    auto rhs = visit_expression(tmp->rhs);
    if ((lhs->id() == tmp->lhs->id()) and (rhs->id() == tmp->rhs->id()))
        return expr;
    return std::make_shared<DivideTerm>(lhs, rhs);
}

expr_pointer_t visit_IfThenElseTerm(const expr_pointer_t& expr)
{
    auto tmp = safe_pointer_cast<IfThenElseTerm>(expr);
    auto cond_expr = visit_expression(tmp->cond_expr);
    auto then_expr = visit_expression(tmp->then_expr);
    auto else_expr = visit_expression(tmp->else_expr);
    if ((cond_expr->id() == tmp->cond_expr->id()) and (then_expr->id() == tmp->then_expr->id())
        and (else_expr->id() == tmp->else_expr->id()))
        return expr;
    return std::make_shared<IfThenElseTerm>(cond_expr, then_expr, else_expr);
}

#define UNARY_VISITOR(TERM)                                 \
    expr_pointer_t visit_##TERM(const expr_pointer_t& expr) \
    {                                                       \
        auto tmp = safe_pointer_cast<TERM>(expr);           \
        auto body = visit_expression(tmp->body);            \
        if (body->id() == tmp->body->id())                  \
            return expr;                                    \
        return std::make_shared<TERM>(body);                \
    }

UNARY_VISITOR(AbsTerm)
UNARY_VISITOR(CeilTerm)
UNARY_VISITOR(FloorTerm)
UNARY_VISITOR(ExpTerm)
UNARY_VISITOR(LogTerm)
UNARY_VISITOR(Log10Term)
UNARY_VISITOR(SqrtTerm)
UNARY_VISITOR(SinTerm)
UNARY_VISITOR(CosTerm)
UNARY_VISITOR(TanTerm)
UNARY_VISITOR(ASinTerm)
UNARY_VISITOR(ACosTerm)
UNARY_VISITOR(ATanTerm)
UNARY_VISITOR(SinhTerm)
UNARY_VISITOR(CoshTerm)
UNARY_VISITOR(TanhTerm)
UNARY_VISITOR(ASinhTerm)
UNARY_VISITOR(ACoshTerm)
UNARY_VISITOR(ATanhTerm)

expr_pointer_t visit_PowTerm(const expr_pointer_t& expr)
{
    auto tmp = safe_pointer_cast<PowTerm>(expr);
    auto lhs = visit_expression(tmp->lhs);
    auto rhs = visit_expression(tmp->rhs);
    if ((lhs->id() == tmp->lhs->id()) and (rhs->id() == tmp->rhs->id()))
        return expr;
    return std::make_shared<PowTerm>(lhs, rhs);
}

#define VISIT_CASE(TERM) \
    case TERM##_id:      \
        return visit_##TERM(expr);

expr_pointer_t visit_expression(const expr_pointer_t& expr)
{
    switch (expr->id()) {
        case ConstantTerm_id:
        case ParameterTerm_id:
        case VariableTerm_id:
        case MonomialTerm_id:
            return expr;

            // VISIT_CASE(ConstantTerm);
            // VISIT_CASE(ParameterTerm);
            VISIT_CASE(IndexParameterTerm);
            // VISIT_CASE(VariableTerm);
            VISIT_CASE(VariableRefTerm);
            VISIT_CASE(ParameterRefTerm);
            VISIT_CASE(DataRefTerm);
            // VISIT_CASE(MonomialTerm);
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
            VISIT_CASE(SumExpressionTerm);

        // GCOVR_EXCL_START
        default:
            throw std::runtime_error(
                "Error in convert_expr or convert_con visitor!  Visiting unexpected expression "
                "term "
                + std::to_string(expr->id()));
            // GCOVR_EXCL_STOP
    };

    return 0;
}

}  // namespace convert_expr_visitor

expr_pointer_t convert_expr_template(const expr_pointer_t& expr)
{
    if (expr == 0)
        throw std::runtime_error("Unexpected null expression");

    return convert_expr_visitor::visit_expression(expr);
}

std::shared_ptr<ConstraintTerm> convert_con_template(const std::shared_ptr<ConstraintTerm>& expr)
{
    if (expr == 0)
        throw std::runtime_error("Unexpected null constraint");

    auto ans = convert_expr_visitor::visit_expression(expr);
    return safe_pointer_cast<ConstraintTerm>(ans);
}

}  // namespace coek
