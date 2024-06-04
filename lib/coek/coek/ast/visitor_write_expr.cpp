#include "base_terms.hpp"
#include "constraint_terms.hpp"
#include "expr_terms.hpp"
#include "value_terms.hpp"
#include "visitor.hpp"
#include "visitor_fns.hpp"
#ifdef COEK_WITH_COMPACT_MODEL
#    include "compact_terms.hpp"
#endif
#include "coek/util/cast_utils.hpp"

namespace coek {

namespace {

void visit_expression(const expr_pointer_t& expr, std::ostream& data);

void visit_ConstantTerm(const expr_pointer_t& expr, std::ostream& ostr)
{
    auto tmp = safe_pointer_cast<ConstantTerm>(expr);
    ostr << tmp->value;
}

void visit_ParameterTerm(const expr_pointer_t& expr, std::ostream& ostr)
{
    auto tmp = safe_pointer_cast<ParameterTerm>(expr);
    if (tmp->name.size() > 0)
        ostr << tmp->name;
    else
        ostr << std::to_string(tmp->eval());
}

void visit_IndexParameterTerm(const expr_pointer_t& expr, std::ostream& ostr)
{
    auto tmp = safe_pointer_cast<IndexParameterTerm>(expr);
    ostr << tmp->name;
}

void visit_VariableTerm(const expr_pointer_t& expr, std::ostream& ostr)
{
    auto tmp = safe_pointer_cast<VariableTerm>(expr);
    auto name = tmp->get_name();
    if (name.size() == 0) {
        ostr << "x" << tmp->index;
    }
    else {
        ostr << name;
    }
}

#ifdef COEK_WITH_COMPACT_MODEL
void visit_ParameterRefTerm(const expr_pointer_t& expr, std::ostream& ostr)
{
    auto tmp = safe_pointer_cast<ParameterRefTerm>(expr);
    bool first = true;
    ostr << tmp->name << "[";
    for (auto& val : tmp->indices) {
        if (first)
            first = false;
        else
            ostr << ",";
        if (auto ival = std::get_if<int>(&val)) {
            ostr << *ival;
        }
        else if (auto eval = std::get_if<expr_pointer_t>(&val)) {
            visit_expression(*eval, ostr);
        }
    }
    ostr << "]";
}

void visit_VariableRefTerm(const expr_pointer_t& expr, std::ostream& ostr)
{
    auto tmp = safe_pointer_cast<VariableRefTerm>(expr);
    bool first = true;
    ostr << tmp->name << "[";
    for (auto& val : tmp->indices) {
        if (first)
            first = false;
        else
            ostr << ",";
        if (auto ival = std::get_if<int>(&val)) {
            ostr << *ival;
        }
        else if (auto eval = std::get_if<expr_pointer_t>(&val)) {
            visit_expression(*eval, ostr);
        }
    }
    ostr << "]";
}
#endif

void visit_MonomialTerm(const expr_pointer_t& expr, std::ostream& ostr)
{
    auto tmp = safe_pointer_cast<MonomialTerm>(expr);
    if (!(tmp->coef == 1.0))
        ostr << tmp->coef << "*";
    visit_expression(tmp->var, ostr);
}

void visit_InequalityTerm(const expr_pointer_t& expr, std::ostream& ostr)
{
    auto tmp = safe_pointer_cast<InequalityTerm>(expr);
    if (tmp->lower) {
        visit_expression(tmp->lower, ostr);
        ostr << " <= ";
    }
    visit_expression(tmp->body, ostr);
    if (tmp->upper) {
        ostr << " <= ";
        visit_expression(tmp->upper, ostr);
    }
}

void visit_StrictInequalityTerm(const expr_pointer_t& expr, std::ostream& ostr)
{
    auto tmp = safe_pointer_cast<StrictInequalityTerm>(expr);
    if (tmp->lower) {
        visit_expression(tmp->lower, ostr);
        ostr << " < ";
    }
    visit_expression(tmp->body, ostr);
    if (tmp->upper) {
        ostr << " < ";
        visit_expression(tmp->upper, ostr);
    }
}

void visit_EqualityTerm(const expr_pointer_t& expr, std::ostream& ostr)
{
    auto tmp = safe_pointer_cast<EqualityTerm>(expr);
    visit_expression(tmp->body, ostr);
    ostr << " == ";
    visit_expression(tmp->lower, ostr);
}

void visit_ObjectiveTerm(const expr_pointer_t& expr, std::ostream& ostr)
{
    auto tmp = safe_pointer_cast<ObjectiveTerm>(expr);
    if (tmp->sense)
        ostr << "min( ";
    else
        ostr << "max( ";
    visit_expression(tmp->body, ostr);
    ostr << " )";
}

void visit_SubExpressionTerm(const expr_pointer_t& expr, std::ostream& ostr)
{
    auto tmp = safe_pointer_cast<SubExpressionTerm>(expr);
    visit_expression(tmp->body, ostr);
}

void visit_NegateTerm(const expr_pointer_t& expr, std::ostream& ostr)
{
    auto tmp = safe_pointer_cast<NegateTerm>(expr);
    ostr << "- (";
    visit_expression(tmp->body, ostr);
    ostr << ")";
}

void visit_PlusTerm(const expr_pointer_t& expr, std::ostream& ostr)
{
    auto tmp = safe_pointer_cast<PlusTerm>(expr);
    std::vector<expr_pointer_t>& vec = *(tmp->data);
    // GCOVR_EXCL_START
    if (vec.size() == 0) {
        ostr << "NULL-SUM";
        return;
    }
    // GCOVR_EXCL_STOP
    visit_expression(vec[0], ostr);

    for (size_t i = 1; i < tmp->num_expressions(); i++) {
        ostr << " + ";
        visit_expression(vec[i], ostr);
    }
}

void visit_TimesTerm(const expr_pointer_t& expr, std::ostream& ostr)
{
    auto tmp = safe_pointer_cast<TimesTerm>(expr);
    ostr << "(";
    visit_expression(tmp->lhs, ostr);
    ostr << ")*(";
    visit_expression(tmp->rhs, ostr);
    ostr << ")";
}

void visit_DivideTerm(const expr_pointer_t& expr, std::ostream& ostr)
{
    auto tmp = safe_pointer_cast<DivideTerm>(expr);
    ostr << "(";
    visit_expression(tmp->lhs, ostr);
    ostr << ")/(";
    visit_expression(tmp->rhs, ostr);
    ostr << ")";
}

#define WriteExprVisitor_FN(FN, TERM)                                 \
    void visit_##TERM(const expr_pointer_t& expr, std::ostream& ostr) \
    {                                                                 \
        auto tmp = safe_pointer_cast<TERM>(expr);                     \
        ostr << #FN << "(";                                           \
        visit_expression(tmp->body, ostr);                            \
        ostr << ")";                                                  \
    }

// clang-format off

WriteExprVisitor_FN(abs, AbsTerm)
WriteExprVisitor_FN(ceil, CeilTerm)
WriteExprVisitor_FN(floor, FloorTerm)
WriteExprVisitor_FN(exp, ExpTerm)
WriteExprVisitor_FN(log, LogTerm)
WriteExprVisitor_FN(log10, Log10Term)
WriteExprVisitor_FN(sqrt, SqrtTerm)
WriteExprVisitor_FN(sin, SinTerm)
WriteExprVisitor_FN(cos, CosTerm)
WriteExprVisitor_FN(tan, TanTerm)
WriteExprVisitor_FN(sinh, SinhTerm)
WriteExprVisitor_FN(cosh, CoshTerm)
WriteExprVisitor_FN(tanh, TanhTerm)
WriteExprVisitor_FN(asin, ASinTerm)
WriteExprVisitor_FN(acos, ACosTerm)
WriteExprVisitor_FN(atan, ATanTerm)
WriteExprVisitor_FN(asinh, ASinhTerm)
WriteExprVisitor_FN(acosh, ACoshTerm)
WriteExprVisitor_FN(atanh, ATanhTerm)

    // clang-format on

    void visit_PowTerm(const expr_pointer_t& expr, std::ostream& ostr)
{
    auto tmp = safe_pointer_cast<PowTerm>(expr);
    ostr << "pow(";
    visit_expression(tmp->lhs, ostr);
    ostr << ", ";
    visit_expression(tmp->rhs, ostr);
    ostr << ")";
}

void visit_SumExpressionTerm(const expr_pointer_t&, std::ostream& ostr) { ostr << "Sum()"; }

void visit_EmptyConstraintTerm(const expr_pointer_t&, std::ostream& ostr)
{
    ostr << "EmptyConstraint()";
}

void visit_IfThenElseTerm(const expr_pointer_t& expr, std::ostream& ostr)
{
    auto tmp = safe_pointer_cast<IfThenElseTerm>(expr);
    ostr << "if_else(";
    visit_expression(tmp->cond_expr, ostr);
    ostr << ", ";
    visit_expression(tmp->then_expr, ostr);
    ostr << ", ";
    visit_expression(tmp->else_expr, ostr);
    ostr << ")";
}

#define VISIT_CASE(TERM) \
    case TERM##_id:      \
        return visit_##TERM(expr, ostr);

void visit_expression(const expr_pointer_t& expr, std::ostream& ostr)
{
    switch (expr->id()) {
        VISIT_CASE(ConstantTerm);
        VISIT_CASE(ParameterTerm);
        VISIT_CASE(IndexParameterTerm);
        VISIT_CASE(VariableTerm);
#ifdef COEK_WITH_COMPACT_MODEL
        VISIT_CASE(ParameterRefTerm);
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
        VISIT_CASE(EmptyConstraintTerm);
        VISIT_CASE(IfThenElseTerm);

        // GCOVR_EXCL_START
        default:
            throw std::runtime_error(
                "Error in write_expr visitor!  Visiting unexpected expression term "
                + std::to_string(expr->id()));
            // GCOVR_EXCL_STOP
    };
}

}  // namespace

// NOTE: This function is defined with raw pointers to allow for the convenient
// writing of raw AST term objects.  The pointer passed-in here is owned by a
// shared_ptr object.
void write_expr(BaseExpressionTerm* expr, std::ostream& ostr)
{
    // GCOVR_EXCL_START
    if (expr == 0) {
        ostr << "<none>";
        return;
    }
    // GCOVR_EXCL_STOP
    visit_expression(std::shared_ptr<BaseExpressionTerm>(expr, [](BaseExpressionTerm*) {}), ostr);
}

void write_expr(const expr_pointer_t& expr, std::ostream& ostr) { visit_expression(expr, ostr); }

}  // namespace coek
