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

namespace coek {

namespace {

class ToListVisitor : public Visitor {
   public:
    std::list<std::string>& repr;

   public:
    ToListVisitor(std::list<std::string>& _repr) : repr(_repr) {}

    void visit(ConstantTerm& arg);
    void visit(ParameterTerm& arg);
    void visit(IndexParameterTerm& arg);
    void visit(VariableTerm& arg);
#if __cpp_lib_variant
    void visit(ParameterRefTerm& arg);
    void visit(VariableRefTerm& arg);
#endif
    void visit(MonomialTerm& arg);
    void visit(InequalityTerm& arg);
    void visit(EqualityTerm& arg);
    void visit(ObjectiveTerm& arg);
    void visit(SubExpressionTerm& arg);
    void visit(NegateTerm& arg);
    void visit(PlusTerm& arg);
    void visit(TimesTerm& arg);
    void visit(DivideTerm& arg);
    void visit(AbsTerm& arg);
    void visit(CeilTerm& arg);
    void visit(FloorTerm& arg);
    void visit(ExpTerm& arg);
    void visit(LogTerm& arg);
    void visit(Log10Term& arg);
    void visit(SqrtTerm& arg);
    void visit(SinTerm& arg);
    void visit(CosTerm& arg);
    void visit(TanTerm& arg);
    void visit(SinhTerm& arg);
    void visit(CoshTerm& arg);
    void visit(TanhTerm& arg);
    void visit(ASinTerm& arg);
    void visit(ACosTerm& arg);
    void visit(ATanTerm& arg);
    void visit(ASinhTerm& arg);
    void visit(ACoshTerm& arg);
    void visit(ATanhTerm& arg);
    void visit(PowTerm& arg);
    void visit(SumExpressionTerm& arg);
    void visit(IfThenElseTerm& arg);
};

void ToListVisitor::visit(ConstantTerm& arg)
{
    // std::stringstream sstr;
    // char c[256];
    // std::snprintf(c, 256, "%.3f", arg.value);
    repr.push_back(std::to_string(arg.value));
}

void ToListVisitor::visit(ParameterTerm& arg)
{
    std::stringstream sstr;
    write_expr(&arg, sstr);
    repr.push_back(sstr.str());
}

void ToListVisitor::visit(IndexParameterTerm& arg)
{
    std::stringstream sstr;
    write_expr(&arg, sstr);
    repr.push_back(sstr.str());
}

void ToListVisitor::visit(VariableTerm& arg)
{
    std::stringstream sstr;
    write_expr(&arg, sstr);
    repr.push_back(sstr.str());
}

#if __cpp_lib_variant
void ToListVisitor::visit(ParameterRefTerm& arg)
{
    std::stringstream sstr;
    write_expr(&arg, sstr);
    repr.push_back(sstr.str());
}

void ToListVisitor::visit(VariableRefTerm& arg)
{
    std::stringstream sstr;
    write_expr(&arg, sstr);
    repr.push_back(sstr.str());
}
#endif

void ToListVisitor::visit(MonomialTerm& arg)
{
    repr.push_back("[");
    repr.push_back("*");
    {
        std::stringstream sstr;
        sstr << arg.coef;
        repr.push_back(sstr.str());
    }
    if (arg.var->name.size() == 0) {
        // WEH - It's hard to test this logic, since the variable index is dynamically generated
        // GCOVR_EXCL_START
        std::stringstream sstr;
        sstr << "x" << arg.var->index;
        repr.push_back(sstr.str());
        // GCOVR_EXCL_STOP
    }
    else {
        repr.push_back(arg.var->name);
    }
    repr.push_back("]");
}

void ToListVisitor::visit(InequalityTerm& arg)
{
    repr.push_back("[");
    if (arg.strict)
        repr.push_back("<");
    else
        repr.push_back("<=");
    if (arg.lower)
        arg.lower->accept(*this);
    else
        repr.push_back("-Inf");
    arg.body->accept(*this);
    if (arg.upper)
        arg.upper->accept(*this);
    else
        repr.push_back("Inf");
    repr.push_back("]");
}

void ToListVisitor::visit(EqualityTerm& arg)
{
    repr.push_back("[");
    repr.push_back("==");
    arg.body->accept(*this);
    arg.lower->accept(*this);
    repr.push_back("]");
}

void ToListVisitor::visit(ObjectiveTerm& arg)
{
    repr.push_back("[");
    if (arg.sense)
        repr.push_back("min");
    else
        repr.push_back("max");
    arg.body->accept(*this);
    repr.push_back("]");
}

void ToListVisitor::visit(SubExpressionTerm& arg)
{
    repr.push_back("[");
    repr.push_back("_");
    arg.body->accept(*this);
    repr.push_back("]");
}

void ToListVisitor::visit(NegateTerm& arg)
{
    repr.push_back("[");
    repr.push_back("-");
    arg.body->accept(*this);
    repr.push_back("]");
}

void ToListVisitor::visit(PlusTerm& arg)
{
    repr.push_back("[");
    repr.push_back("+");
    std::vector<expr_pointer_t>& vec = *(arg.data);
    for (size_t i = 0; i < arg.num_expressions(); i++) vec[i]->accept(*this);
    repr.push_back("]");
}

void ToListVisitor::visit(TimesTerm& arg)
{
    repr.push_back("[");
    repr.push_back("*");
    arg.lhs->accept(*this);
    arg.rhs->accept(*this);
    repr.push_back("]");
}

void ToListVisitor::visit(DivideTerm& arg)
{
    repr.push_back("[");
    repr.push_back("/");
    arg.lhs->accept(*this);
    arg.rhs->accept(*this);
    repr.push_back("]");
}

#define ToListVisitor_FN(FN, TERM)       \
    void ToListVisitor::visit(TERM& arg) \
    {                                    \
        repr.push_back("[");             \
        repr.push_back(#FN);             \
        arg.body->accept(*this);         \
        repr.push_back("]");             \
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

    void ToListVisitor::visit(PowTerm& arg)
{
    repr.push_back("[");
    repr.push_back("pow");
    arg.lhs->accept(*this);
    arg.rhs->accept(*this);
    repr.push_back("]");
}

void ToListVisitor::visit(SumExpressionTerm&)
{
    repr.push_back("[");
    repr.push_back("Sum");
    repr.push_back("]");
}

void ToListVisitor::visit(IfThenElseTerm& arg)
{
    repr.push_back("[");
    repr.push_back("If");
    arg.cond_expr->accept(*this);
    arg.then_expr->accept(*this);
    arg.else_expr->accept(*this);
    repr.push_back("]");
}

}  // namespace

void expr_to_list(BaseExpressionTerm* expr, std::list<std::string>& repr)
{
    ToListVisitor visitor(repr);
    expr->accept(visitor);
}

}  // namespace coek
