#include <sstream>
#include "ast_visitors.hpp"


std::ostream& operator<<(std::ostream& ostr, const std::list<std::string>& vals)
{
auto it = vals.begin();
auto end = vals.end();
if (it == end)
    return ostr;
ostr << *it;
++it;

for (; it != end; ++it) {
    ostr << " " << *it;
    }
return ostr;
}


namespace coek {

std::ostream& operator<<(std::ostream& ostr, const std::list<std::string>& vals)
{
auto it = vals.begin();
auto end = vals.end();
if (it == end)
    return ostr;
ostr << *it;
++it;

for (; it != end; ++it) {
    ostr << " " << *it;
    }
return ostr;
}


namespace {

class ToListVisitor : public Visitor
{
public:

    std::list<std::string>& repr;

public:

    ToListVisitor(std::list<std::string>& _repr)
        : repr(_repr) {}

    void visit(ConstantTerm& arg);
    void visit(ParameterTerm& arg);
    void visit(VariableTerm& arg);
    void visit(MonomialTerm& arg);
    void visit(InequalityTerm& arg);
    void visit(EqualityTerm& arg);
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
};


void ToListVisitor::visit(ConstantTerm& arg)
{
std::stringstream sstr;
char c[256];
std::snprintf(c, 256, "%.3f", arg.value);
repr.push_back( c );
}

void ToListVisitor::visit(ParameterTerm& arg)
{
std::stringstream sstr;
write_expr(&arg, sstr);
repr.push_back( sstr.str() );
}

void ToListVisitor::visit(VariableTerm& arg)
{
std::stringstream sstr;
write_expr(&arg, sstr);
repr.push_back( sstr.str() );
}

void ToListVisitor::visit(MonomialTerm& arg)
{
repr.push_back("[");
repr.push_back("*");
{
std::stringstream sstr;
sstr << arg.coef;
repr.push_back( sstr.str() );
}
if (arg.var->name.size() == 0) {
    std::stringstream sstr;
    sstr << "x" << arg.var->index;
    repr.push_back( sstr.str() );
    }
else {
    repr.push_back( arg.var->name );
    }
repr.push_back("]");
}

void ToListVisitor::visit(InequalityTerm& arg)
{
repr.push_back( "[" );
if (arg.strict)
    repr.push_back( "<" );
else
    repr.push_back( "<=" );
arg.body->accept(*this);
repr.push_back( "0" );
repr.push_back( "]" );
}

void ToListVisitor::visit(EqualityTerm& arg)
{
repr.push_back( "[" );
repr.push_back( "==" );
arg.body->accept(*this);
repr.push_back( "0" );
repr.push_back( "]" );
}

void ToListVisitor::visit(NegateTerm& arg)
{
repr.push_back( "[" );
repr.push_back( "-" );
arg.body->accept(*this);
repr.push_back( "]" );
}

void ToListVisitor::visit(PlusTerm& arg)
{
repr.push_back( "[" );
repr.push_back( "+" );
std::vector<expr_pointer_t>& vec = *(arg.data);
for (size_t i=0; i<arg.n; i++)
    vec[i]->accept(*this);
repr.push_back( "]" );
}

void ToListVisitor::visit(TimesTerm& arg)
{
repr.push_back( "[" );
repr.push_back( "*" );
arg.lhs->accept(*this);
arg.rhs->accept(*this);
repr.push_back( "]" );
}

void ToListVisitor::visit(DivideTerm& arg)
{
repr.push_back( "[" );
repr.push_back( "/" );
arg.lhs->accept(*this);
arg.rhs->accept(*this);
repr.push_back( "]" );
}

#define ToListVisitor_FN(FN, TERM)\
void ToListVisitor::visit(TERM& arg)\
{\
repr.push_back( "[" );\
repr.push_back( #FN );\
arg.body->accept(*this);\
repr.push_back( "]" );\
}

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

void ToListVisitor::visit(PowTerm& arg)
{
repr.push_back( "[" );
repr.push_back( "pow" );
arg.lhs->accept(*this);
arg.rhs->accept(*this);
repr.push_back( "]" );
}

}


void expr_to_list(expr_pointer_t expr, std::list<std::string>& repr)
{
ToListVisitor visitor(repr);
expr->accept(visitor);
}

std::list<std::string> to_list(expr_pointer_t expr)
{
std::list<std::string> repr;
ToListVisitor visitor(repr);
expr->accept(visitor);
return repr;
}

}
