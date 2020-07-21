#include <variant>
#include "../ast/compact_terms.hpp"
#include "../ast/constraint_terms.hpp"
#include "../ast/objective_terms.hpp"
#include "../ast/expr_terms.hpp"
#include "coek/api/expression.hpp"
#include "coek_exprterm.hpp"


namespace coek {

expr_pointer_t get_concrete_var(VariableRefTerm& varref);


namespace visitors {

expr_pointer_t visit_expression(expr_pointer_t expr);
expr_pointer_t visit(SumExpressionTerm&);


expr_pointer_t visit(IndexParameterTerm& arg)
{
if (auto pval = std::get_if<int>(&(arg.value)))
    return CREATE_POINTER(ConstantTerm, *pval);
else if (auto pval = std::get_if<double>(&(arg.value)))
    return CREATE_POINTER(ConstantTerm, *pval);
else
    throw std::runtime_error("Unexpected index parameter in an expression being converted, with a non-numeric value.");
}

expr_pointer_t visit(VariableRefTerm& arg)
{ return get_concrete_var(arg); }

expr_pointer_t visit(InequalityTerm& arg)
{
auto lower = arg.lower ? visit_expression(arg.lower) : 0;
auto body = visit_expression(arg.body);
auto upper = arg.upper ? visit_expression(arg.upper) : 0;
return CREATE_POINTER(InequalityTerm, lower, body, upper);
}

expr_pointer_t visit(EqualityTerm& arg)
{
auto body = visit_expression(arg.body);
auto lower = visit_expression(arg.lower);
return CREATE_POINTER(EqualityTerm, body, lower);
}

expr_pointer_t visit(ObjectiveTerm& arg)
{
auto body = visit_expression(arg.body);
return CREATE_POINTER(ObjectiveTerm, body, arg.sense);
}

expr_pointer_t visit(NegateTerm& arg)
{
auto curr = visit_expression(arg.body);
return CREATE_POINTER(NegateTerm, curr);
}

expr_pointer_t visit(PlusTerm& arg)
{
auto lhs = visit_expression((*(arg.data))[0]);
auto curr = visit_expression((*(arg.data))[1]);
curr = CREATE_POINTER(PlusTerm, lhs, curr, false);
if (arg.n == 2)
    return curr;

PlusTerm* _curr = dynamic_cast<PlusTerm*>(curr);

for (unsigned int i=2; i<arg.n; i++) {
    curr = visit_expression((*(arg.data))[i]);
    _curr->push_back(curr);
    }
return _curr;
}

expr_pointer_t visit(TimesTerm& arg)
{
auto lhs = visit_expression(arg.lhs);
auto rhs = visit_expression(arg.rhs);
return CREATE_POINTER(TimesTerm, lhs, rhs);
}

expr_pointer_t visit(DivideTerm& arg)
{
auto lhs = visit_expression(arg.lhs);
auto rhs = visit_expression(arg.rhs);
return CREATE_POINTER(DivideTerm, lhs, rhs);
}

#define UNARY_VISITOR(TERM)\
expr_pointer_t visit(TERM& arg)\
{\
auto curr = visit_expression(arg.body);\
return CREATE_POINTER(TERM, curr);\
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


expr_pointer_t visit(PowTerm& arg)
{
auto lhs = visit_expression(arg.lhs);
auto rhs = visit_expression(arg.rhs);
return CREATE_POINTER(PowTerm, lhs, rhs);
}


#define VISIT_CASE(TERM)\
    case TERM ## _id:\
        return visit( *dynamic_cast<TERM*>(expr));

expr_pointer_t visit_expression(expr_pointer_t expr)
{
switch (expr->id()) {

    case ConstantTerm_id:
    case ParameterTerm_id:
    case VariableTerm_id:
    case IndexedVariableTerm_id:
    case MonomialTerm_id:
        return expr;

    VISIT_CASE(IndexParameterTerm);
    VISIT_CASE(VariableRefTerm);
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
    VISIT_CASE(SumExpressionTerm);
    };

return 0;
}

}


expr_pointer_t convert_expr_template(expr_pointer_t expr)
{
if (expr == 0)
    throw std::runtime_error("Unexpected null expression");

return visitors::visit_expression(expr);
}

ConstraintTerm* convert_con_template(ConstraintTerm* expr)
{
if (expr == 0)
    throw std::runtime_error("Unexpected null constraint");

auto curr = visitors::visit_expression(expr->body);
if (expr->is_equality()) {
    auto lower = expr->lower ? visitors::visit_expression(expr->lower) : 0;
    return CREATE_POINTER(EqualityTerm, curr, lower);
    }
else {
    auto lower = expr->lower ? visitors::visit_expression(expr->lower) : 0;
    auto upper = expr->upper ? visitors::visit_expression(expr->upper) : 0;
    return CREATE_POINTER(InequalityTerm, lower, curr, upper);
    }
}

}
