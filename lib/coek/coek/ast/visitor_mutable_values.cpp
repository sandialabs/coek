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

class MutableValuesData {
   public:
    std::unordered_set<std::shared_ptr<VariableTerm>>& fixed_vars;
    std::unordered_set<std::shared_ptr<ParameterTerm>>& params;

    std::unordered_set<std::shared_ptr<SubExpressionTerm>>& visited_subexpressions;

#ifdef DEBUG
    size_t num_visits = 0;
#endif

   public:
    MutableValuesData(
        std::unordered_set<std::shared_ptr<VariableTerm>>& _fixed_vars,
        std::unordered_set<std::shared_ptr<ParameterTerm>>& _params,
        std::unordered_set<std::shared_ptr<SubExpressionTerm>>& _visited_subexpressions)
        : fixed_vars(_fixed_vars), params(_params), visited_subexpressions(_visited_subexpressions)
    {
    }
};

void visit_expression(const expr_pointer_t& expr, MutableValuesData& data);

#define FROM_BODY(TERM)                                                    \
    void visit_##TERM(const expr_pointer_t& expr, MutableValuesData& data) \
    {                                                                      \
        auto tmp = safe_pointer_cast<TERM>(expr);                          \
        visit_expression(tmp->body, data);                                 \
    }

#define FROM_LHS_RHS(TERM)                                                 \
    void visit_##TERM(const expr_pointer_t& expr, MutableValuesData& data) \
    {                                                                      \
        auto tmp = safe_pointer_cast<TERM>(expr);                          \
        visit_expression(tmp->lhs, data);                                  \
        visit_expression(tmp->rhs, data);                                  \
    }

// -----------------------------------------------------------------------------------------

void visit_ConstantTerm(const expr_pointer_t& /*expr*/, MutableValuesData& /*data*/) {}

void visit_ParameterTerm(const expr_pointer_t& expr, MutableValuesData& data)
{
    auto tmp = safe_pointer_cast<ParameterTerm>(expr);
    data.params.insert(tmp);
}

void visit_IndexParameterTerm(const expr_pointer_t& /*expr*/, MutableValuesData& /*data*/) {}

void visit_VariableTerm(const expr_pointer_t& expr, MutableValuesData& data)
{
    auto tmp = safe_pointer_cast<VariableTerm>(expr);
    if (tmp->fixed) data.fixed_vars.insert(tmp);
}

#if __cpp_lib_variant
void visit_ParameterRefTerm(const expr_pointer_t& /*expr*/, MutableValuesData& /*data*/) {}

void visit_VariableRefTerm(const expr_pointer_t& /*expr*/, MutableValuesData& /*data*/) {}
#endif

void visit_MonomialTerm(const expr_pointer_t& expr, MutableValuesData& data)
{
    auto tmp = safe_pointer_cast<MonomialTerm>(expr);
    if (tmp->var->fixed) data.fixed_vars.insert(tmp->var);
}

// clang-format off
FROM_BODY(InequalityTerm)
FROM_BODY(EqualityTerm)
FROM_BODY(ObjectiveTerm)
FROM_BODY(NegateTerm)
// clang-format on

void visit_SubExpressionTerm(const expr_pointer_t& expr, MutableValuesData& data)
{
    auto tmp = safe_pointer_cast<SubExpressionTerm>(expr);
    if (data.visited_subexpressions.find(tmp) == data.visited_subexpressions.end()) {
        data.visited_subexpressions.insert(tmp);
        visit_expression(tmp->body, data);
    }
}

void visit_PlusTerm(const expr_pointer_t& expr, MutableValuesData& data)
{
    auto tmp = safe_pointer_cast<PlusTerm>(expr);
    auto& vec = *(tmp->data);
    auto n = tmp->num_expressions();
    for (size_t i = 0; i < n; i++) visit_expression(vec[i], data);
}

// clang-format off
FROM_LHS_RHS(TimesTerm)
FROM_LHS_RHS(DivideTerm)

FROM_BODY(AbsTerm)
FROM_BODY(CeilTerm)
FROM_BODY(FloorTerm)
FROM_BODY(ExpTerm)
FROM_BODY(LogTerm)
FROM_BODY(Log10Term)
FROM_BODY(SqrtTerm)
FROM_BODY(SinTerm)
FROM_BODY(CosTerm)
FROM_BODY(TanTerm)
FROM_BODY(SinhTerm)
FROM_BODY(CoshTerm)
FROM_BODY(TanhTerm)
FROM_BODY(ASinTerm)
FROM_BODY(ACosTerm)
FROM_BODY(ATanTerm)
FROM_BODY(ASinhTerm)
FROM_BODY(ACoshTerm)
FROM_BODY(ATanhTerm)

FROM_LHS_RHS(PowTerm)
// clang-format on

#define VISIT_CASE(TERM)          \
    case TERM##_id:               \
        visit_##TERM(expr, data); \
        break

void visit_expression(const expr_pointer_t& expr, MutableValuesData& data)
{
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
                "Error in mutable_values visitor!  Visiting unexpected expression term "
                + std::to_string(expr->id()));
            // GCOVR_EXCL_STOP
    };

#ifdef DEBUG
    data.num_visits++;
#endif
}

}  // namespace

#ifdef DEBUG
void mutable_values_debug(
    const expr_pointer_t& expr, std::unordered_set<std::shared_ptr<VariableTerm>>& fixed_vars,
    std::unordered_set<std::shared_ptr<ParameterTerm>>& params,
    std::unordered_set<std::shared_ptr<SubExpressionTerm>>& visited_subexpressions,
    size_t& num_visits)
{
    num_visits = 0;

    // GCOVR_EXCL_START
    if (not expr) return;
    // GCOVR_EXCL_STOP

    MutableValuesData data(fixed_vars, params, visited_subexpressions);
    visit_expression(expr, data);

    num_visits = data.num_visits;
}
#endif

void mutable_values(const expr_pointer_t& expr,
                    std::unordered_set<std::shared_ptr<VariableTerm>>& fixed_vars,
                    std::unordered_set<std::shared_ptr<ParameterTerm>>& params,
                    std::unordered_set<std::shared_ptr<SubExpressionTerm>>& visited_subexpressions)
{
    // GCOVR_EXCL_START
    if (not expr) return;
    // GCOVR_EXCL_STOP

    MutableValuesData data(fixed_vars, params, visited_subexpressions);
    visit_expression(expr, data);
}

}  // namespace coek
