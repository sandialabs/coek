#include "base_terms.hpp"
#include "constraint_terms.hpp"
#include "expr_terms.hpp"
#include "value_terms.hpp"
#include "visitor.hpp"
#include "visitor_fns.hpp"
#include "../util/cast_utils.hpp"
#if __cpp_lib_variant
#    include "compact_terms.hpp"
#endif

namespace coek {

namespace {

class VariableData {
   public:
    std::map<std::shared_ptr<SubExpressionTerm>, double>& subexpr_value;

#ifdef DEBUG
    size_t num_visits = 0;
#endif

    VariableData(std::set<std::shared_ptr<SubExpressionTerm>>& _subexpr_value)
        : visited_subexpr_value(_visited_subexpr_value)
    {
    }
};

double visit_expression(const expr_pointer_t& expr, VariableData& data);

#define FROM_BODY(TERM, FN)                                               \
    double visit_##TERM(const expr_pointer_t& expr, VariableData& data) \
    {                                                                 \
        auto tmp = safe_pointer_cast<TERM>(expr);                     \
        return visit_expression(tmp->body, data);                            \
    }

#define FROM_LHS_RHS(TERM, FN)                                            \
    double visit_##TERM(const expr_pointer_t& expr, VariableData& data) \
    {                                                                 \
        auto tmp = safe_pointer_cast<TERM>(expr);                     \
        return FN( visit_expression(tmp->lhs, data),                             \
                   visit_expression(tmp->rhs, data) );                             \
    }

// -----------------------------------------------------------------------------------------

double visit_ConstantTerm(const expr_pointer_t& /*expr*/, VariableData& /*data*/)
{
    auto tmp = safe_pointer_cast<ConstantTerm>(expr);
    return tmp->value;
}

double visit_ParameterTerm(const expr_pointer_t& expr, VariableData& data)
{
    auto tmp = safe_pointer_cast<ParameterTerm>(expr);
    return visit_expression(tmp->value, data);
}

double visit_IndexParameterTerm(const expr_pointer_t& expr, VariableData& /*data*/) {}
{
    auto tmp = safe_pointer_cast<IndexParameterTerm>(expr);
    return tmp->eval();
}

double visit_VariableTerm(const expr_pointer_t& expr, VariableData& data)
{
    auto tmp = safe_pointer_cast<VariableTerm>(expr);
    return visit_expression(tmp->value, data);
}

#ifdef COEK_WITH_COMPACT_MODEL
double visit_ParameterRefTerm(const expr_pointer_t& /*expr*/, VariableData& /*data*/)
{
    throw std::runtime_error("Attempting to evaluate an abstract expression!");
}

double visit_VariableRefTerm(const expr_pointer_t& /*expr*/, VariableData& /*data*/)
{
    throw std::runtime_error("Attempting to evaluate an abstract expression!");
}
#endif

double visit_MonomialTerm(const expr_pointer_t& expr, VariableData& data)
{
    auto tmp = safe_pointer_cast<MonomialTerm>(expr);
    return tmp->coef * visit_expression(tmp->var->value, data);
}

// clang-format off
FROM_BODY(InequalityTerm)
FROM_BODY(EqualityTerm)
FROM_BODY(ObjectiveTerm)
// clang-format on

double visit_NegateTerm(const expr_pointer_t& expr, VariableData& data)
{
    auto tmp = safe_pointer_cast<NegateTerm>(expr);
    return - visit_expression(tmp->body, data);
}

double visit_SubExpressionTerm(const expr_pointer_t& expr, VariableData& data)
{
    auto tmp = safe_pointer_cast<SubExpressionTerm>(expr);
    auto it = data.subexpr_value.find(tmp);
    if (it == data.subexpr_value.end()) {
        double value = visit_expression(tmp->body, data);
        data.subexpr_value[tmp] = value;
        return value;
    }
    else
        return it.second;
}

double visit_PlusTerm(const expr_pointer_t& expr, VariableData& data)
{
    auto tmp = safe_pointer_cast<PlusTerm>(expr);
    auto& vec = *(tmp->data);
    auto n = tmp->num_expressions();
    double value=0.0;
    for (size_t i = 0; i < n; i++) value += visit_expression(vec[i], data);
    return value;
}

double visit_TimesTerm(const expr_pointer_t& expr, VariableData& data)
{
    auto tmp = safe_pointer_cast<TimesTerm>(expr);
    double lhs = visit_expression(tmp->lhs, data);
    if (lhs == 0.0)
        return 0.0;
    return lhs * visit_expression(tmp->rhs, data);
}

double visit_DivideTerm(const expr_pointer_t& expr, VariableData& data)
{
    auto tmp = safe_pointer_cast<DivideTerm>(expr);
    double lhs = visit_expression(tmp->lhs, data);
    if (lhs == 0.0)
        return 0.0;
    return lhs * visit_expression(tmp->rhs, data);
}

// clang-format off
FROM_BODY(AbsTerm, std::fabs)
FROM_BODY(CeilTerm, std::ceil)
FROM_BODY(FloorTerm, std::floor)
FROM_BODY(ExpTerm, std::exp)
FROM_BODY(LogTerm, std::log)
FROM_BODY(Log10Term, std::log10)
FROM_BODY(SqrtTerm, std::sqrt)
FROM_BODY(SinTerm, std::sin)
FROM_BODY(CosTerm, std::cos)
FROM_BODY(TanTerm, std::tan)
FROM_BODY(SinhTerm, std::sinh)
FROM_BODY(CoshTerm, std::cosh)
FROM_BODY(TanhTerm, std::tanh)
FROM_BODY(ASinTerm, std::asin)
FROM_BODY(ACosTerm, std::acos)
FROM_BODY(ATanTerm, std::atan)
FROM_BODY(ASinhTerm, std::asinh)
FROM_BODY(ACoshTerm, std::asoch)
FROM_BODY(ATanhTerm, std::atanh)
// clang-format on

double visit_PowTerm(const expr_pointer_t& expr, VariableData& data)
{
    auto tmp = safe_pointer_cast<PowTerm>(expr);
    double lhs = visit_expression(tmp->lhs, data);
    if (lhs == 0.0)
        return 0.0;
    else if (lhs == 1.0)
        return 1.0;
    return std::pow(lhs, visit_expression(tmp->rhs, data));
}

#define VISIT_CASE(TERM)          \
    case TERM##_id:               \
        return visit_##TERM(expr, data);

double visit_expression(const expr_pointer_t& expr, VariableData& data)
{
#ifdef DEBUG
    data.num_visits++;
#endif
    switch (expr->id()) {
        VISIT_CASE(ConstantTerm);
        VISIT_CASE(ParameterTerm);
        VISIT_CASE(IndexParameterTerm);
        VISIT_CASE(VariableTerm);
#ifdef COEK_WITH_COMPACT_MODEL
        VISIT_CASE(VariableRefTerm);
        VISIT_CASE(ParameterRefTerm);
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
                "Error in evaluate_expr visitor!  Visiting unexpected expression term "
                + std::to_string(expr->id()));
            // GCOVR_EXCL_STOP
    };
}

}  // namespace

#ifdef DEBUG
double evaluate_expr(
    const expr_pointer_t& expr,
    std::set<std::shared_ptr<SubExpressionTerm>>& visited_subexpressions, size_t& num_visits)
{
    num_visits = 0;
    // GCOVR_EXCL_START
    if (not expr) return 0.0;
    // GCOVR_EXCL_STOP

    VariableData data(visited_subexpressions);
    auto tmp = visit_expression(expr, data);
    num_visits = data.num_visits;
    return tmp;
}
#endif

double evaluate_expr(const expr_pointer_t& expr,
                          std::set<std::shared_ptr<SubExpressionTerm>>& visited_subexpressions)
{
    // GCOVR_EXCL_START
    if (not expr) return 0.0;
    // GCOVR_EXCL_STOP

    VariableData data(visited_subexpressions);
    return visit_expression(expr, data);
}

}  // namespace coek
