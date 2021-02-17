#include "visitor_fns.hpp"
#include "visitor.hpp"
#include "base_terms.hpp"
#include "constraint_terms.hpp"
#include "objective_terms.hpp"
#include "expr_terms.hpp"
#include "value_terms.hpp"
#ifdef COEK_WITH_COMPACT_MODEL
#include "compact_terms.hpp"
#endif


namespace coek {

namespace {

class WriteExprVisitor : public Visitor
{
public:

    std::ostream& ostr;

public:

    WriteExprVisitor(std::ostream& _ostr)
        : ostr(_ostr) {}

    void visit(ConstantTerm& arg);
    void visit(ParameterTerm& arg);
    void visit(IndexParameterTerm& arg);
    void visit(VariableTerm& arg);
#ifdef COEK_WITH_COMPACT_MODEL
    void visit(VariableRefTerm& arg);
#endif
    void visit(IndexedVariableTerm& arg);
    void visit(MonomialTerm& arg);
    void visit(InequalityTerm& arg);
    void visit(EqualityTerm& arg);
    void visit(ObjectiveTerm& arg);
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
};


void WriteExprVisitor::visit(ConstantTerm& arg)
{
ostr << arg.value;
}

void WriteExprVisitor::visit(ParameterTerm& arg)
{
if (arg.name.size() == 0) {
    char c[256];
    std::snprintf(c, 256, "%.3f", arg.value);
    ostr << c;
    }
else
    ostr << arg.name;
}

void WriteExprVisitor::visit(IndexParameterTerm& arg)
{
ostr << arg.name;
}

void WriteExprVisitor::visit(VariableTerm& arg)
{
if (arg.name.size() == 0) {
    ostr << "x" << arg.index;
    }
else {
    ostr << arg.name;
    }
}

#ifdef COEK_WITH_COMPACT_MODEL
void WriteExprVisitor::visit(VariableRefTerm& arg)
{
bool first=true;
ostr << arg.name << "(";
for (auto it=arg.indices.begin(); it != arg.indices.end(); ++it) {
    if (first)
        first=false;
    else
        ostr << ",";
    auto val = *it;
    if (auto ival = std::get_if<int>(&val)) {
        ostr << *ival;
        }
#if 0
    else if (auto dval = std::get_if<double>(&val)) {
        ostr << *dval;
        }
    else if (auto sval = std::get_if<std::string>(&val)) {
        ostr << "\"" << *sval << "\"";
        }
#endif
    else if (auto eval = std::get_if<expr_pointer_t>(&val)) {
        (*eval)->accept(*this);
        }
    }
ostr << ")";
}
#endif

void WriteExprVisitor::visit(IndexedVariableTerm& arg)
{
ostr << arg.get_name();
}

void WriteExprVisitor::visit(MonomialTerm& arg)
{
if (!(arg.coef == 1.0))
    ostr << arg.coef << "*";
arg.var->accept(*this);
}

void WriteExprVisitor::visit(InequalityTerm& arg)
{
if (arg.lower) {
    arg.lower->accept(*this);
    if (arg.strict)
        ostr << " < ";
    else
        ostr << " <= ";
    }
arg.body->accept(*this);
if (arg.upper) {
    if (arg.strict)
        ostr << " < ";
    else
        ostr << " <= ";
    arg.upper->accept(*this);
    }
}

void WriteExprVisitor::visit(EqualityTerm& arg)
{
arg.body->accept(*this);
ostr << " == ";
arg.lower->accept(*this);
}

void WriteExprVisitor::visit(ObjectiveTerm& arg)
{
if (arg.sense)
   ostr << "min( ";
else
   ostr << "max( ";
arg.body->accept(*this);
ostr << " )";
}

void WriteExprVisitor::visit(NegateTerm& arg)
{
ostr << "- (";
arg.body->accept(*this);
ostr << ")";
}

void WriteExprVisitor::visit(PlusTerm& arg)
{
std::vector<expr_pointer_t>& vec = *(arg.data);
if (vec.size() == 0) {
    ostr << "NULL-SUM";
    return;
    }
vec[0]->accept(*this);

for (size_t i=1; i<arg.n; i++) {
    ostr << " + ";
    vec[i]->accept(*this);
    }
}

void WriteExprVisitor::visit(TimesTerm& arg)
{
ostr << "(";
arg.lhs->accept(*this);
ostr << ")*(";
arg.rhs->accept(*this);
ostr << ")";
}

void WriteExprVisitor::visit(DivideTerm& arg)
{
ostr << "(";
arg.lhs->accept(*this);
ostr << ")/(";
arg.rhs->accept(*this);
ostr << ")";
}

#define WriteExprVisitor_FN(FN, TERM)\
void WriteExprVisitor::visit(TERM& arg)\
{\
ostr << #FN << "(";\
arg.body->accept(*this);\
ostr << ")";\
}

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


void WriteExprVisitor::visit(PowTerm& arg)
{
ostr << "pow(";
arg.lhs->accept(*this);
ostr << ", ";
arg.rhs->accept(*this);
ostr << ")";
}

void WriteExprVisitor::visit(SumExpressionTerm& )
{
ostr << "Sum()";
}

}

void write_expr(expr_pointer_t expr, std::ostream& ostr)
{
if (expr == 0) {
    ostr << "<none>";
    return;
    }
WriteExprVisitor visitor(ostr);
expr->accept(visitor);
}

}
