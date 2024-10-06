#include <memory>
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

namespace coek {

namespace {

class VisitorData {
   public:
    std::map<std::shared_ptr<SubExpressionTerm>, expr_pointer_t>& subexpr_value;

    double last_value;
    expr_pointer_t last_expr;
    bool is_value;

    VisitorData(std::map<std::shared_ptr<SubExpressionTerm>, expr_pointer_t>& _subexpr_value)
        : subexpr_value(_subexpr_value), last_value(0.0), is_value(true)
    {
    }
};

void visit_expression(const expr_pointer_t& expr, VisitorData& data);

#define FROM_BODY_FN(TERM, FN)                                       \
    void visit_##TERM(const expr_pointer_t& expr, VisitorData& data) \
    {                                                                \
        auto tmp = safe_pointer_cast<TERM>(expr);                    \
        visit_expression(tmp->body, data);                           \
        if (data.is_value)                                           \
            data.last_value = FN(data.last_value);                   \
        else {                                                       \
            data.last_expr = std::make_shared<TERM>(data.last_expr); \
        }                                                            \
    }

// -----------------------------------------------------------------------------------------

void visit_ConstantTerm(const expr_pointer_t& expr, VisitorData& data)
{
    auto tmp = safe_pointer_cast<ConstantTerm>(expr);
    data.last_value = tmp->value;
    data.is_value = true;
}

void visit_ParameterTerm(const expr_pointer_t& expr, VisitorData& data)
{
    auto tmp = safe_pointer_cast<ParameterTerm>(expr);
    data.last_value = tmp->eval();
    data.is_value = true;
}

void visit_IndexParameterTerm(const expr_pointer_t& expr, VisitorData& data)
{
    data.last_expr = expr;
    data.is_value = false;
}

void visit_VariableTerm(const expr_pointer_t& expr, VisitorData& data)
{
    auto tmp = safe_pointer_cast<VariableTerm>(expr);
    if (tmp->fixed) {
        data.last_value = tmp->eval();
        data.is_value = true;
    }
    else {
        data.last_expr = expr;
        data.is_value = false;
    }
}

#ifdef COEK_WITH_COMPACT_MODEL
void visit_VariableRefTerm(const expr_pointer_t& expr, VisitorData& data)
{
    data.last_expr = expr;
    data.is_value = false;
}

void visit_ParameterRefTerm(const expr_pointer_t& expr, VisitorData& data)
{
    data.last_expr = expr;
    data.is_value = false;
}

void visit_DataRefTerm(const expr_pointer_t& expr, VisitorData& data)
{
    data.last_expr = expr;
    data.is_value = false;
}
#endif

void visit_MonomialTerm(const expr_pointer_t& expr, VisitorData& data)
{
    auto tmp = safe_pointer_cast<MonomialTerm>(expr);
    if (tmp->var->fixed) {
        data.last_value = tmp->coef * tmp->var->eval();
        data.is_value = true;
    }
    else {
        data.last_expr = expr;
        data.is_value = false;
    }
}

void visit_ObjectiveTerm(const expr_pointer_t& expr, VisitorData& data)
{
    auto tmp = safe_pointer_cast<ObjectiveTerm>(expr);
    visit_expression(tmp->body, data);
    if (data.is_value)
        data.last_expr = std::make_shared<ObjectiveTerm>(
            std::make_shared<ConstantTerm>(data.last_value), tmp->sense);
    else
        data.last_expr = std::make_shared<ObjectiveTerm>(data.last_expr, tmp->sense);
}

void visit_InequalityTerm(const expr_pointer_t& expr, VisitorData& data)
{
    auto tmp = safe_pointer_cast<InequalityTerm>(expr);
    visit_expression(tmp->body, data);
    if (data.is_value)
        // TODO - ignore constraints with a constant body
        data.last_expr = std::make_shared<InequalityTerm>(
            tmp->lower, std::make_shared<ConstantTerm>(data.last_value), tmp->upper);
    else
        data.last_expr = std::make_shared<InequalityTerm>(tmp->lower, data.last_expr, tmp->upper);
}

void visit_StrictInequalityTerm(const expr_pointer_t& expr, VisitorData& data)
{
    auto tmp = safe_pointer_cast<StrictInequalityTerm>(expr);
    visit_expression(tmp->body, data);
    if (data.is_value)
        // TODO - ignore constraints with a constant body
        data.last_expr = std::make_shared<StrictInequalityTerm>(
            tmp->lower, std::make_shared<ConstantTerm>(data.last_value), tmp->upper);
    else
        data.last_expr
            = std::make_shared<StrictInequalityTerm>(tmp->lower, data.last_expr, tmp->upper);
}

void visit_EqualityTerm(const expr_pointer_t& expr, VisitorData& data)
{
    auto tmp = safe_pointer_cast<EqualityTerm>(expr);
    visit_expression(tmp->body, data);
    if (data.is_value)
        // TODO - ignore constraints with a constant body
        data.last_expr = std::make_shared<EqualityTerm>(
            std::make_shared<ConstantTerm>(data.last_value), tmp->lower);
    else
        data.last_expr = std::make_shared<EqualityTerm>(data.last_expr, tmp->lower);
}

void visit_NegateTerm(const expr_pointer_t& expr, VisitorData& data)
{
    auto tmp = safe_pointer_cast<NegateTerm>(expr);
    visit_expression(tmp->body, data);
    if (data.is_value)
        data.last_value *= -1;
    else
        data.last_expr = std::make_shared<NegateTerm>(data.last_expr);
}

void visit_SubExpressionTerm(const expr_pointer_t& expr, VisitorData& data)
{
    auto tmp = safe_pointer_cast<SubExpressionTerm>(expr);
    auto it = data.subexpr_value.find(tmp);
    if (it == data.subexpr_value.end()) {
        visit_expression(tmp->body, data);
        if (data.is_value)
            data.subexpr_value[tmp] = std::make_shared<ConstantTerm>(data.last_value);
        else
            data.subexpr_value[tmp] = data.last_expr;
    }
    else {
        if (it->second->is_constant()) {
            data.last_value = it->second->eval();
            data.is_value = true;
        }
        else {
            data.last_expr = it->second;
            data.is_value = false;
        }
    }
}

void visit_PlusTerm(const expr_pointer_t& expr, VisitorData& data)
{
    auto tmp = safe_pointer_cast<PlusTerm>(expr);
    auto& vec = *(tmp->data);
    auto n = tmp->num_expressions();

    double offset = 0.0;
    bool first_term = true;
    expr_pointer_t sum_of_terms;

    for (size_t i = 0; i < n; i++) {
        visit_expression(vec[i], data);
        if (data.is_value)
            offset += data.last_value;
        else if (first_term) {
            first_term = false;
            sum_of_terms = data.last_expr;
        }
        else
            sum_of_terms = std::make_shared<PlusTerm>(sum_of_terms, data.last_expr);
    }

    if (first_term) {
        data.last_value = offset;
        data.is_value = true;
    }
    else {
        if (offset != 0.0)
            data.last_expr
                = std::make_shared<PlusTerm>(sum_of_terms, std::make_shared<ConstantTerm>(offset));
        else
            data.last_expr = sum_of_terms;
        data.is_value = false;
    }
}

void visit_TimesTerm(const expr_pointer_t& expr, VisitorData& data)
{
    auto tmp = safe_pointer_cast<TimesTerm>(expr);
    visit_expression(tmp->lhs, data);
    if (data.is_value) {
        if (data.last_value == 0.0)
            return;

        else if (data.last_value == 1.0) {
            visit_expression(tmp->rhs, data);
        }

        else {
            double val = data.last_value;
            visit_expression(tmp->rhs, data);
            if (data.is_value)
                data.last_value *= val;
            else
                data.last_expr = std::make_shared<TimesTerm>(std::make_shared<ConstantTerm>(val),
                                                             data.last_expr);
        }
    }
    else {
        expr_pointer_t val = data.last_expr;
        visit_expression(tmp->rhs, data);
        if (data.is_value) {
            if (data.last_value == 0.0)
                return;
            else if (data.last_value == 1.0) {
                data.last_expr = val;
                data.is_value = false;
            }
            else {
                data.last_expr = std::make_shared<TimesTerm>(
                    val, std::make_shared<ConstantTerm>(data.last_value));
                data.is_value = false;
            }
        }
        else
            data.last_expr = std::make_shared<TimesTerm>(val, data.last_expr);
    }
}

void visit_DivideTerm(const expr_pointer_t& expr, VisitorData& data)
{
    auto tmp = safe_pointer_cast<DivideTerm>(expr);
    visit_expression(tmp->lhs, data);
    if (data.is_value) {
        if (data.last_value == 0.0)
            return;

        else {
            double val = data.last_value;
            visit_expression(tmp->rhs, data);
            if (data.is_value)
                data.last_value = val / data.last_value;  // TODO - check for zero
            else
                data.last_expr = std::make_shared<DivideTerm>(std::make_shared<ConstantTerm>(val),
                                                              data.last_expr);
        }
    }
    else {
        expr_pointer_t val = data.last_expr;
        visit_expression(tmp->rhs, data);
        if (data.is_value) {
            if (data.last_value == 0.0)  // TODO - check for zero
                return;
            else if (data.last_value == 1.0) {
                data.last_expr = val;
                data.is_value = false;
            }
            else {
                data.last_expr = std::make_shared<DivideTerm>(
                    val, std::make_shared<ConstantTerm>(data.last_value));
                data.is_value = false;
            }
        }
        else
            data.last_expr = std::make_shared<DivideTerm>(val, data.last_expr);
    }
}

void visit_IfThenElseTerm(const expr_pointer_t& expr, VisitorData& data)
{
    auto tmp = safe_pointer_cast<IfThenElseTerm>(expr);
    visit_expression(tmp->cond_expr, data);
    if (data.is_value) {
        if (data.last_value > (1.0 - 1e-7))
            visit_expression(tmp->then_expr, data);
        else
            visit_expression(tmp->else_expr, data);
    }
    else {
        auto cond_expr = data.last_expr;

        expr_pointer_t then_expr;
        visit_expression(tmp->then_expr, data);
        if (data.is_value)
            then_expr = std::make_shared<ConstantTerm>(data.last_value);
        else
            then_expr = data.last_expr;

        expr_pointer_t else_expr;
        visit_expression(tmp->else_expr, data);
        if (data.is_value)
            else_expr = std::make_shared<ConstantTerm>(data.last_value);
        else
            else_expr = data.last_expr;

        data.last_expr = std::make_shared<IfThenElseTerm>(cond_expr, then_expr, else_expr);
        data.is_value = false;
    }
}

// clang-format off
FROM_BODY_FN(AbsTerm, std::fabs)
FROM_BODY_FN(CeilTerm, std::ceil)
FROM_BODY_FN(FloorTerm, std::floor)
FROM_BODY_FN(ExpTerm, std::exp)
FROM_BODY_FN(LogTerm, std::log)
FROM_BODY_FN(Log10Term, std::log10)
FROM_BODY_FN(SqrtTerm, std::sqrt)
FROM_BODY_FN(SinTerm, std::sin)
FROM_BODY_FN(CosTerm, std::cos)
FROM_BODY_FN(TanTerm, std::tan)
FROM_BODY_FN(SinhTerm, std::sinh)
FROM_BODY_FN(CoshTerm, std::cosh)
FROM_BODY_FN(TanhTerm, std::tanh)
FROM_BODY_FN(ASinTerm, std::asin)
FROM_BODY_FN(ACosTerm, std::acos)
FROM_BODY_FN(ATanTerm, std::atan)
FROM_BODY_FN(ASinhTerm, std::asinh)
FROM_BODY_FN(ACoshTerm, std::acosh)
FROM_BODY_FN(ATanhTerm, std::atanh)
// clang-format on

void visit_PowTerm(const expr_pointer_t& expr, VisitorData& data)
{
    auto tmp = safe_pointer_cast<PowTerm>(expr);
    visit_expression(tmp->lhs, data);
    if (data.is_value) {
        if ((data.last_value == 0.0) or (data.last_value == 1.0))
            return;

        double val = data.last_value;
        visit_expression(tmp->rhs, data);
        if (data.is_value)
            data.last_value = std::pow(val, data.last_value);
        else
            data.last_expr
                = std::make_shared<PowTerm>(std::make_shared<ConstantTerm>(val), data.last_expr);
    }
    else {
        expr_pointer_t val = data.last_expr;
        visit_expression(tmp->rhs, data);
        if (data.is_value) {
            if (data.last_value == 0.0)
                data.last_value = 1.0;
            else if (data.last_value == 1.0) {
                data.last_expr = val;
                data.is_value = false;
            }
            else {
                data.last_expr = std::make_shared<PowTerm>(
                    val, std::make_shared<ConstantTerm>(data.last_value));
                data.is_value = false;
            }
        }
        else {
            data.last_expr = std::make_shared<PowTerm>(val, data.last_expr);
            data.is_value = false;
        }
    }
}

void visit_SumExpressionTerm(const expr_pointer_t& expr, VisitorData& data)
{
    data.last_expr = expr;
    data.is_value = false;
}

#define VISIT_CASE(TERM) \
    case TERM##_id:      \
        return visit_##TERM(expr, data);

void visit_expression(const expr_pointer_t& expr, VisitorData& data)
{
    switch (expr->id()) {
        VISIT_CASE(ConstantTerm);
        VISIT_CASE(ParameterTerm);
        VISIT_CASE(IndexParameterTerm);
        VISIT_CASE(VariableTerm);
#ifdef COEK_WITH_COMPACT_MODEL
        VISIT_CASE(VariableRefTerm);
        VISIT_CASE(ParameterRefTerm);
        VISIT_CASE(DataRefTerm);
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
        VISIT_CASE(SumExpressionTerm);

        // GCOVR_EXCL_START
        default:
            throw std::runtime_error(
                "Error in simplify_expr visitor!  Visiting unexpected expression term "
                + std::to_string(expr->id()));
            // GCOVR_EXCL_STOP
    };
}

}  // namespace

expr_pointer_t simplify_expr(
    const expr_pointer_t& expr,
    std::map<std::shared_ptr<SubExpressionTerm>, expr_pointer_t>& subexpr_value)
{
    // GCOVR_EXCL_START
    if (not expr)
        return expr;
    // GCOVR_EXCL_STOP

    VisitorData data(subexpr_value);
    visit_expression(expr, data);

    if (data.is_value)
        return std::make_shared<ConstantTerm>(data.last_value);
    else
        return data.last_expr;
}

expr_pointer_t simplify_expr(const expr_pointer_t& expr)
{
    std::map<std::shared_ptr<SubExpressionTerm>, expr_pointer_t> simplified_subexpressions;
    return simplify_expr(expr, simplified_subexpressions);
}

}  // namespace coek
