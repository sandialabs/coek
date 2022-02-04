#include "visitor_fns.hpp"
#include "visitor.hpp"
#include "base_terms.hpp"
#include "constraint_terms.hpp"
#include "expr_terms.hpp"
#include "value_terms.hpp"
#ifdef COEK_WITH_COMPACT_MODEL
#include "compact_terms.hpp"
#endif

#include "coek/api/expression.hpp"
#include "coek/api/expression_visitor.hpp"


namespace coek {

void write_expr(expr_pointer_t, std::ostream& );


std::ostream& operator<<(std::ostream& ostr, const QuadraticExpr& arg)
{
ostr << "Constant: " << arg.constval << std::endl;
ostr << "Linear: " << std::endl;
for (size_t i=0; i<arg.linear_coefs.size(); i++) {
    ostr << arg.linear_coefs[i] << " ";
    write_expr(arg.linear_vars[i], ostr);
    ostr << std::endl;
    }
ostr << "Quadratic: " << std::endl;
for (size_t i=0; i<arg.quadratic_coefs.size(); i++) {
    ostr << arg.quadratic_coefs[i] << " ";
    write_expr(arg.quadratic_lvars[i], ostr);
    ostr << " ";
    write_expr(arg.quadratic_rvars[i], ostr);
    ostr << std::endl;
    }
return ostr;
}


namespace {

void visit_expression(expr_pointer_t expr, QuadraticExpr& repn, double multiplier);


void visit(ConstantTerm& expr, QuadraticExpr& repn, double multiplier)
{
repn.constval += multiplier * expr.value;
}

void visit(ParameterTerm& expr, QuadraticExpr& repn, double multiplier)
{
repn.constval += multiplier * expr.value;
}

void visit(IndexParameterTerm& /*expr*/, 
           QuadraticExpr& /*repn*/, 
           double /*multiplier*/)
{
throw std::runtime_error("Unexpected index parameter.");
}

void visit(VariableTerm& expr, QuadraticExpr& repn, double multiplier)
{
//if (! expr.index)
//    throw std::runtime_error("Unexpected variable not owned by a model.");

if (expr.fixed)  {
    repn.constval += multiplier * expr.value->eval();
    }
else {
    repn.linear_vars.push_back(&expr);
    repn.linear_coefs.push_back(multiplier);
    }
}

#ifdef COEK_WITH_COMPACT_MODEL
void visit(VariableRefTerm& /*expr*/,
           QuadraticExpr& /*repn*/,
           double /*multiplier*/)
{
throw std::runtime_error("Unexpected variable reference.");
}
#endif

void visit(IndexedVariableTerm& expr, QuadraticExpr& repn, double multiplier)
{
//if (! expr.index)
//    throw std::runtime_error("Unexpected variable not owned by a model.");

if (expr.fixed)  {
    repn.constval += multiplier * expr.value->eval();
    }
else {
    repn.linear_vars.push_back(&expr);
    repn.linear_coefs.push_back(multiplier);
    }
}

void visit(MonomialTerm& expr, QuadraticExpr& repn, double multiplier)
{
//if (! expr.var->index)
//    throw std::runtime_error("Unexpected variable not owned by a model.");

if (expr.var->fixed) {
    repn.constval += multiplier * expr.coef * expr.var->value->eval();
    }
else {
    repn.linear_vars.push_back(expr.var);
    repn.linear_coefs.push_back(multiplier * expr.coef);
    }
}

void visit(InequalityTerm& expr, QuadraticExpr& repn, double multiplier)
{
visit_expression(expr.body, repn, multiplier);
}

void visit(EqualityTerm& expr, QuadraticExpr& repn, double multiplier)
{
visit_expression(expr.body, repn, multiplier);
}

void visit(ObjectiveTerm& expr, QuadraticExpr& repn, double multiplier)
{
visit_expression(expr.body, repn, multiplier);
}

void visit(NegateTerm& expr, QuadraticExpr& repn, double multiplier)
{
visit_expression(expr.body, repn, - multiplier);
}

void visit(PlusTerm& expr, QuadraticExpr& repn, double multiplier)
{
NAryPrefixTerm::shared_t::iterator it = expr.data.get()->begin();
NAryPrefixTerm::shared_t::iterator end = expr.data.get()->end();

for( ; it != end; ++it)
    visit_expression(*it, repn, multiplier);
}

void visit(TimesTerm& expr, QuadraticExpr& repn, double multiplier)
{
QuadraticExpr lhs_repn;
visit_expression(expr.lhs, lhs_repn, 1.0);

if (lhs_repn.linear_coefs.size()+lhs_repn.quadratic_coefs.size() == 0) {
    //
    // LHS is a constant
    //
    if (lhs_repn.constval == 0.0)
        return;

    visit_expression(expr.rhs, repn, lhs_repn.constval * multiplier);
    return;
    }

QuadraticExpr rhs_repn;
visit_expression(expr.rhs, rhs_repn, 1.0);

if (rhs_repn.linear_coefs.size()+rhs_repn.quadratic_coefs.size() == 0) {
    //
    // RHS is a constant
    //
    if (rhs_repn.constval == 0.0)
        return;

    multiplier *= rhs_repn.constval;

    repn.constval += multiplier * lhs_repn.constval;
    for (size_t i=0; i<lhs_repn.linear_coefs.size(); i++) {
        repn.linear_vars.push_back( lhs_repn.linear_vars[i] );
        repn.linear_coefs.push_back( lhs_repn.linear_coefs[i] * multiplier );
        }
    for (size_t i=0; i<lhs_repn.quadratic_coefs.size(); i++) {
        repn.quadratic_lvars.push_back( lhs_repn.quadratic_lvars[i] );
        repn.quadratic_rvars.push_back( lhs_repn.quadratic_rvars[i] );
        repn.quadratic_coefs.push_back( lhs_repn.quadratic_coefs[i] * multiplier );
        }
    return;
    }

//
// LHS and RHS are non-constant
//
repn.constval += multiplier * lhs_repn.constval * rhs_repn.constval;

if (lhs_repn.constval != 0.0) {
    for (size_t i=0; i<rhs_repn.linear_coefs.size(); i++) {
        repn.linear_vars.push_back(rhs_repn.linear_vars[i]);
        repn.linear_coefs.push_back(multiplier * lhs_repn.constval * rhs_repn.linear_coefs[i]);
        }
    /*
    for (size_t i=0; i<rhs_repn.quadratic_coefs.size(); i++) {
        repn.quadratic_lvars.push_back(rhs_repn.quadratic_lvars[i]);
        repn.quadratic_rvars.push_back(rhs_repn.quadratic_rvars[i]);
        repn.quadratic_coefs.push_back(multiplier * lhs_repn.constval * rhs_repn.quadratic_coefs[i]);
        }
    */
    }
if (rhs_repn.constval != 0.0) {
    for (size_t i=0; i<lhs_repn.linear_coefs.size(); i++) {
        repn.linear_vars.push_back(lhs_repn.linear_vars[i]);
        repn.linear_coefs.push_back(multiplier * lhs_repn.linear_coefs[i] * rhs_repn.constval);
        }
    /*
    for (size_t i=0; i<lhs_repn.quadratic_coefs.size(); i++) {
        repn.quadratic_lvars.push_back(lhs_repn.quadratic_lvars[i]);
        repn.quadratic_rvars.push_back(lhs_repn.quadratic_rvars[i]);
        repn.quadratic_coefs.push_back(multiplier * rhs_repn.constval * lhs_repn.quadratic_coefs[i]);
        }
    */
    }
for (size_t i=0; i<lhs_repn.linear_coefs.size(); i++) {
    for (size_t j=0; j<rhs_repn.linear_coefs.size(); j++) {
        repn.quadratic_lvars.push_back(lhs_repn.linear_vars[i]);
        repn.quadratic_rvars.push_back(rhs_repn.linear_vars[j]);
        repn.quadratic_coefs.push_back(multiplier * lhs_repn.linear_coefs[i]*rhs_repn.linear_coefs[j]);
        }
    }

if ( (lhs_repn.quadratic_coefs.size() > 0) and ((rhs_repn.linear_coefs.size()+rhs_repn.quadratic_coefs.size()) > 0) )
    throw std::runtime_error("Non-quadratic expressions cannot be expressed in a QuadraticExpr object.");
if ( (rhs_repn.quadratic_coefs.size() > 0) and (lhs_repn.linear_coefs.size() > 0) )
    throw std::runtime_error("Non-quadratic expressions cannot be expressed in a QuadraticExpr object.");
}

void visit(DivideTerm& expr, QuadraticExpr& repn, double multiplier)
{
visit_expression(expr.lhs, repn, multiplier);

// LHS is zero, so we ignore the RHS
if ( (repn.constval == 0.0) and ((repn.linear_coefs.size()+repn.quadratic_coefs.size()) == 0) )
    return;

QuadraticExpr rhs_repn;
visit_expression(expr.rhs, rhs_repn, 1.0);

if ((rhs_repn.linear_coefs.size()+rhs_repn.quadratic_coefs.size()) > 0)
    throw std::runtime_error("Non-constant expressions cannot appear in the denominator of quadratic expressions.");
if (rhs_repn.constval == 0.0)
    throw std::runtime_error("Division by zero error.");

// Divide the the rhs value
repn.constval /= rhs_repn.constval;
for (size_t i=0; i<repn.linear_coefs.size(); i++)
    repn.linear_coefs[i] /= rhs_repn.constval;
for (size_t i=0; i<repn.quadratic_coefs.size(); i++)
    repn.quadratic_coefs[i] /= rhs_repn.constval;
}

#define UNARY_VISITOR(TERM, FN)\
void visit(TERM& expr, QuadraticExpr& repn, double multiplier)\
{\
QuadraticExpr body_repn;\
visit_expression(expr.body, body_repn, 1.0);\
\
if ((body_repn.linear_coefs.size() + body_repn.quadratic_coefs.size()) > 0)\
    throw std::runtime_error("Nonlinear expressions are not supported for QuadraticExpr: " + std::string(#FN) + " term.");\
\
repn.constval += multiplier * ::FN(body_repn.constval);\
}

UNARY_VISITOR(AbsTerm, fabs)
UNARY_VISITOR(CeilTerm, ceil)
UNARY_VISITOR(FloorTerm, floor)
UNARY_VISITOR(ExpTerm, exp)
UNARY_VISITOR(LogTerm, log)
UNARY_VISITOR(Log10Term, log10)
UNARY_VISITOR(SqrtTerm, sqrt)
UNARY_VISITOR(SinTerm, sin)
UNARY_VISITOR(CosTerm, cos)
UNARY_VISITOR(TanTerm, tan)
UNARY_VISITOR(SinhTerm, sinh)
UNARY_VISITOR(CoshTerm, cosh)
UNARY_VISITOR(TanhTerm, tanh)
UNARY_VISITOR(ASinTerm, asin)
UNARY_VISITOR(ACosTerm, acos)
UNARY_VISITOR(ATanTerm, atan)
UNARY_VISITOR(ASinhTerm, asinh)
UNARY_VISITOR(ACoshTerm, acosh)
UNARY_VISITOR(ATanhTerm, atanh)

#define BINARY_VISITOR(TERM, FN)\
void visit(TERM& expr, QuadraticExpr& repn, double multiplier)\
{\
QuadraticExpr lhs_repn;\
visit_expression(expr.lhs, lhs_repn, 1.0);\
if ((lhs_repn.linear_coefs.size() + lhs_repn.quadratic_coefs.size()) > 0)\
    throw std::runtime_error("Nonlinear expressions are not supported for QuadraticExpr: " + std::string(#FN) + " term.");\
\
QuadraticExpr rhs_repn;\
visit_expression(expr.rhs, rhs_repn, 1.0);\
if ((rhs_repn.linear_coefs.size() + rhs_repn.quadratic_coefs.size()) > 0)\
    throw std::runtime_error("Nonlinear expressions are not supported for QuadraticExpr: " + std::string(#FN) + " term.");\
\
repn.constval += multiplier * ::FN(lhs_repn.constval, rhs_repn.constval);\
}

void visit(PowTerm& expr, QuadraticExpr& repn, double multiplier)
{
QuadraticExpr rhs_repn;
visit_expression(expr.rhs, rhs_repn, 1.0);
if (not rhs_repn.is_constant())
    throw std::runtime_error("Nonlinear expressions are not supported for QuadraticExpr: pow term with non-constant exponent.");

if (rhs_repn.constval == 0) {
    repn.constval += multiplier;
    }

else if (rhs_repn.constval == 1) {
    visit_expression(expr.lhs, repn, multiplier);
    }

else {
    QuadraticExpr lhs_repn;
    visit_expression(expr.lhs, lhs_repn, 1.0);
    if (lhs_repn.is_constant())
        // A**B - A and B constant
        repn.constval += multiplier * ::pow(lhs_repn.constval, rhs_repn.constval);

    else if (lhs_repn.is_linear() and (rhs_repn.constval == 2)) {
        // A**B - A linear and B=2
        // Quadratic
        for (std::size_t i=0; i<lhs_repn.linear_coefs.size(); i++)
            for (std::size_t j=0; j<lhs_repn.linear_coefs.size(); j++) {
                repn.quadratic_coefs.push_back( multiplier * lhs_repn.linear_coefs[i] * lhs_repn.linear_coefs[j] );
                repn.quadratic_lvars.push_back( lhs_repn.linear_vars[i] );
                repn.quadratic_rvars.push_back( lhs_repn.linear_vars[j] );
                }
        // Linear
        for (std::size_t i=0; i<lhs_repn.linear_coefs.size(); i++) {
            repn.linear_coefs.push_back( 2 * multiplier * lhs_repn.linear_coefs[i] * lhs_repn.constval );
            repn.linear_vars.push_back( lhs_repn.linear_vars[i] );
            }
        // Constant
        repn.constval += multiplier * lhs_repn.constval * lhs_repn.constval;
        }

    else {
        throw std::runtime_error("Nonlinear expressions are not supported for QuadraticExpr: pow term with nonlinear base or constant exponent other than 2.");
        }
    }
}

//BINARY_VISITOR(PowTerm, pow)


#define VISIT_CASE(TERM)\
    case TERM ## _id:\
        visit( *dynamic_cast<TERM*>(expr), repn, multiplier);\
        break

void visit_expression(expr_pointer_t expr, QuadraticExpr& repn, double multiplier)
{
switch (expr->id()) {

    VISIT_CASE(ConstantTerm);
    VISIT_CASE(ParameterTerm);
    VISIT_CASE(IndexParameterTerm);
    VISIT_CASE(VariableTerm);
#ifdef COEK_WITH_COMPACT_MODEL
    VISIT_CASE(VariableRefTerm);
#endif
    VISIT_CASE(IndexedVariableTerm);
    VISIT_CASE(MonomialTerm);
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

    // GCOVR_EXCL_START
    default:
        throw std::runtime_error("Error in QuadraticExpr visitor!  Visiting unexpected expression term " + std::to_string(expr->id()));
    // GCOVR_EXCL_STOP
    };
}

}

void to_QuadraticExpr(expr_pointer_t expr, QuadraticExpr& repn)
{
visit_expression(expr, repn, 1.0);
}

}
