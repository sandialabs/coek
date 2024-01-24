#include <sstream>

#include "base_terms.hpp"
#include "constraint_terms.hpp"
#include "expr_terms.hpp"
#include "value_terms.hpp"
#include "visitor.hpp"
#include "visitor_fns.hpp"
#if __cpp_lib_variant
#    include "compact_terms.hpp"
#endif
#include "coek/util/cast_utils.hpp"

namespace coek {

namespace {

void visit_expression(const expr_pointer_t& expr, std::list<std::string>& repr);

void visit_ConstantTerm(const expr_pointer_t& expr, std::list<std::string>& repr)
{
    auto tmp = safe_pointer_cast<ConstantTerm>(expr);
    repr.push_back(std::to_string(tmp->value));
}

void visit_ParameterTerm(const expr_pointer_t& expr, std::list<std::string>& repr)
{
    auto tmp = safe_pointer_cast<ParameterTerm>(expr);
    std::stringstream sstr;
    write_expr(tmp, sstr);
    repr.push_back(sstr.str());
}

void visit_IndexParameterTerm(const expr_pointer_t& expr, std::list<std::string>& repr)
{
    auto tmp = safe_pointer_cast<IndexParameterTerm>(expr);
    std::stringstream sstr;
    write_expr(tmp, sstr);
    repr.push_back(sstr.str());
}

void visit_VariableTerm(const expr_pointer_t& expr, std::list<std::string>& repr)
{
    auto tmp = safe_pointer_cast<VariableTerm>(expr);
    std::stringstream sstr;
    write_expr(tmp, sstr);
    repr.push_back(sstr.str());
}

#if __cpp_lib_variant
void visit_ParameterRefTerm(const expr_pointer_t& expr, std::list<std::string>& repr)
{
    auto tmp = safe_pointer_cast<ParameterRefTerm>(expr);
    std::stringstream sstr;
    write_expr(tmp, sstr);
    repr.push_back(sstr.str());
}

void visit_VariableRefTerm(const expr_pointer_t& expr, std::list<std::string>& repr)
{
    auto tmp = safe_pointer_cast<VariableRefTerm>(expr);
    std::stringstream sstr;
    write_expr(tmp, sstr);
    repr.push_back(sstr.str());
}
#endif

void visit_MonomialTerm(const expr_pointer_t& expr, std::list<std::string>& repr)
{
    auto tmp = safe_pointer_cast<MonomialTerm>(expr);
    repr.push_back("[");
    repr.push_back("*");
    {
        //std::stringstream sstr;
        //sstr << tmp->coef;
        //repr.push_back(sstr.str());
        repr.push_back(std::to_string(tmp->coef));
    }
    if (tmp->var->name.size() == 0) {
        // WEH - It's hard to test this logic, since the variable index is dynamically generated
        // GCOVR_EXCL_START
        std::stringstream sstr;
        sstr << "x" << tmp->var->index;
        repr.push_back(sstr.str());
        // GCOVR_EXCL_STOP
    }
    else {
        repr.push_back(tmp->var->name);
    }
    repr.push_back("]");
}

void visit_InequalityTerm(const expr_pointer_t& expr, std::list<std::string>& repr)
{
    auto tmp = safe_pointer_cast<InequalityTerm>(expr);
    repr.push_back("[");
    if (tmp->strict)
        repr.push_back("<");
    else
        repr.push_back("<=");
    if (tmp->lower)
        visit_expression(tmp->lower, repr);
    else
        repr.push_back("-Inf");
    visit_expression(tmp->body, repr);
    if (tmp->upper)
        visit_expression(tmp->upper, repr);
    else
        repr.push_back("Inf");
    repr.push_back("]");
}

void visit_EqualityTerm(const expr_pointer_t& expr, std::list<std::string>& repr)
{
    auto tmp = safe_pointer_cast<EqualityTerm>(expr);
    repr.push_back("[");
    repr.push_back("==");
    visit_expression(tmp->body, repr);
    visit_expression(tmp->lower, repr);
    repr.push_back("]");
}

void visit_ObjectiveTerm(const expr_pointer_t& expr, std::list<std::string>& repr)
{
    auto tmp = safe_pointer_cast<ObjectiveTerm>(expr);
    repr.push_back("[");
    if (tmp->sense)
        repr.push_back("min");
    else
        repr.push_back("max");
    visit_expression(tmp->body, repr);
    repr.push_back("]");
}

void visit_SubExpressionTerm(const expr_pointer_t& expr, std::list<std::string>& repr)
{
    auto tmp = safe_pointer_cast<SubExpressionTerm>(expr);
    repr.push_back("[");
    repr.push_back("_");
    visit_expression(tmp->body, repr);
    repr.push_back("]");
}

void visit_DefinedValueTerm(const expr_pointer_t& expr, std::list<std::string>& repr)
{
    auto tmp = safe_pointer_cast<DefinedValueTerm>(expr);
    std::stringstream sstr;
    sstr << "V" << tmp->index;
    repr.push_back(sstr.str());
}

void visit_NegateTerm(const expr_pointer_t& expr, std::list<std::string>& repr)
{
    auto tmp = safe_pointer_cast<NegateTerm>(expr);
    repr.push_back("[");
    repr.push_back("-");
    visit_expression(tmp->body, repr);
    repr.push_back("]");
}

void visit_PlusTerm(const expr_pointer_t& expr, std::list<std::string>& repr)
{
    auto tmp = safe_pointer_cast<PlusTerm>(expr);
    repr.push_back("[");
    repr.push_back("+");
    std::vector<expr_pointer_t>& vec = *(tmp->data);
    for (size_t i = 0; i < tmp->num_expressions(); i++) visit_expression(vec[i], repr);
    repr.push_back("]");
}

void visit_TimesTerm(const expr_pointer_t& expr, std::list<std::string>& repr)
{
    auto tmp = safe_pointer_cast<TimesTerm>(expr);
    repr.push_back("[");
    repr.push_back("*");
    visit_expression(tmp->lhs, repr);
    visit_expression(tmp->rhs, repr);
    repr.push_back("]");
}

void visit_DivideTerm(const expr_pointer_t& expr, std::list<std::string>& repr)
{
    auto tmp = safe_pointer_cast<DivideTerm>(expr);
    repr.push_back("[");
    repr.push_back("/");
    visit_expression(tmp->lhs, repr);
    visit_expression(tmp->rhs, repr);
    repr.push_back("]");
}

#define ToListVisitor_FN(FN, TERM)                                              \
    void visit_##TERM(const expr_pointer_t& expr, std::list<std::string>& repr) \
    {                                                                           \
        auto tmp = safe_pointer_cast<TERM>(expr);                               \
        repr.push_back("[");                                                    \
        repr.push_back(#FN);                                                    \
        visit_expression(tmp->body, repr);                                      \
        repr.push_back("]");                                                    \
    }

// clang-format off
ToListVisitor_FN(abs, AbsTerm)
ToListVisitor_FN(ceil, CeilTerm)
ToListVisitor_FN(floor, FloorTerm)
ToListVisitor_FN(exp, ExpTerm)
ToListVisitor_FN(log, LogTerm)
ToListVisitor_FN(log10, Log10Term)
ToListVisitor_FN(sqrt, SqrtTerm)
ToListVisitor_FN(sin, SinTerm)
ToListVisitor_FN(cos, CosTerm)
ToListVisitor_FN(tan, TanTerm)
ToListVisitor_FN(sinh, SinhTerm)
ToListVisitor_FN(cosh, CoshTerm)
ToListVisitor_FN(tanh, TanhTerm)
ToListVisitor_FN(asin, ASinTerm)
ToListVisitor_FN(acos, ACosTerm)
ToListVisitor_FN(atan, ATanTerm)
ToListVisitor_FN(asinh, ASinhTerm)
ToListVisitor_FN(acosh, ACoshTerm)
ToListVisitor_FN(atanh, ATanhTerm)
    // clang-format on
    //

    void visit_PowTerm(const expr_pointer_t& expr, std::list<std::string>& repr)
{
    auto tmp = safe_pointer_cast<PowTerm>(expr);
    repr.push_back("[");
    repr.push_back("pow");
    visit_expression(tmp->lhs, repr);
    visit_expression(tmp->rhs, repr);
    repr.push_back("]");
}

void visit_SumExpressionTerm(const expr_pointer_t&, std::list<std::string>& repr)
{
    // auto tmp = safe_pointer_cast<SumExpressionTerm>(expr);
    repr.push_back("[");
    repr.push_back("Sum");
    repr.push_back("]");
}

void visit_EmptyConstraintTerm(const expr_pointer_t&, std::list<std::string>&) {}

void visit_IfThenElseTerm(const expr_pointer_t& expr, std::list<std::string>& repr)
{
    auto tmp = safe_pointer_cast<IfThenElseTerm>(expr);
    repr.push_back("[");
    repr.push_back("If");
    visit_expression(tmp->cond_expr, repr);
    visit_expression(tmp->then_expr, repr);
    visit_expression(tmp->else_expr, repr);
    repr.push_back("]");
}

#define VISIT_CASE(TERM) \
    case TERM##_id:      \
        return visit_##TERM(expr, repr);

void visit_expression(const expr_pointer_t& expr, std::list<std::string>& repr)
{
    switch (expr->id()) {
        VISIT_CASE(ConstantTerm);
        VISIT_CASE(ParameterTerm);
        VISIT_CASE(IndexParameterTerm);
        VISIT_CASE(VariableTerm);
#if __cpp_lib_variant
        VISIT_CASE(VariableRefTerm);
        VISIT_CASE(ParameterRefTerm);
#endif
        VISIT_CASE(MonomialTerm);
        VISIT_CASE(InequalityTerm);
        VISIT_CASE(EqualityTerm);
        VISIT_CASE(ObjectiveTerm);
        VISIT_CASE(SubExpressionTerm);
        VISIT_CASE(DefinedValueTerm);
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
                "Error in expr_to_list visitor!  Visiting unexpected expression term "
                + std::to_string(expr->id()));
            // GCOVR_EXCL_STOP
    };
}

}  // namespace

void expr_to_list(BaseExpressionTerm* expr, std::list<std::string>& repr)
{
    visit_expression(std::shared_ptr<BaseExpressionTerm>(expr, [](BaseExpressionTerm*) {}), repr);
}

void expr_to_list(const expr_pointer_t& expr, std::list<std::string>& repr)
{
    visit_expression(expr, repr);
}

}  // namespace coek
