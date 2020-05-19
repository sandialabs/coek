#include <cmath>
#include <cstdio>
//#include <sstream>
#include <map>
#include "expr/ast_operators.hpp"
#include "expr/ast_visitors.hpp"
#include "coek_expr.hpp"

namespace coek {

expr_pointer_t convert_expr_template(expr_pointer_t expr);
ConstraintTerm* convert_con_template(ConstraintTerm* con);


double nan_value = std::nan("");

//
// Parameter
//

Parameter::Parameter()
{
repn = CREATE_POINTER(ParameterTerm, 0);
OWN_POINTER(repn);
}

Parameter::Parameter(double value)
{
repn = CREATE_POINTER(ParameterTerm, value);
OWN_POINTER(repn);
}

Parameter::Parameter(double value, const std::string& name)
{
repn = CREATE_POINTER(ParameterTerm, value);
OWN_POINTER(repn);
repn->name = name;
}

Parameter::Parameter(const Parameter& expr)
{
repn = expr.repn;
OWN_POINTER(repn);
}

Parameter::~Parameter()
{
DISOWN_POINTER(repn);
}

Parameter& Parameter::operator=(const Parameter& expr)
{
DISOWN_POINTER(repn);
repn = expr.repn;
OWN_POINTER(repn);
return *this;
}

void Parameter::set_value(double value)
{
// Parameters are non-constant
//if (repn->is_constant())
//    throw std::runtime_error("Cannot set the value of a constant Parameter!");
repn->value = value;
}

double Parameter::get_value() const
{
return repn->value;
}

std::string Parameter::get_name() const
{
return repn->name;
}

std::ostream& operator<<(std::ostream& ostr, const Parameter& arg)
{
write_expr(arg.repn, ostr);
return ostr;
}

//
// IndexParameter
//

// TODO - Does it make sense to allow an empty IndexParameter?

IndexParameter::IndexParameter()
    : repn(0)
{}

IndexParameter::IndexParameter(const std::string& name)
{
repn = CREATE_POINTER(IndexParameterTerm, name);
OWN_POINTER(repn);
}

IndexParameter::IndexParameter(const IndexParameter& expr)
{
repn = expr.repn;
if (repn)
    OWN_POINTER(repn);
}

IndexParameter::~IndexParameter()
{
if (repn)
    DISOWN_POINTER(repn);
}

IndexParameter& IndexParameter::operator=(const IndexParameter& expr)
{
if (repn)
    DISOWN_POINTER(repn);
repn = expr.repn;
OWN_POINTER(repn);
return *this;
}

void IndexParameter::set_value(double value)
{
if (repn)
    repn->value = value;
}

void IndexParameter::set_value(int value)
{
if (repn)
    repn->value = value;
}

void IndexParameter::set_value(const std::string& value)
{
if (repn)
    repn->value = value;
}

void IndexParameter::get_value(double& value) const
{
if (!repn)
    throw std::runtime_error("No double value stored in index parameter.");

if (auto pval = std::get_if<double>(&(repn->value))) {
    value = *pval;
}
else {
    throw std::runtime_error("No double value stored in index parameter.");
    }
}

void IndexParameter::get_value(int& value) const
{
if (!repn)
    throw std::runtime_error("No integer value stored in index parameter.");

if (auto pval = std::get_if<int>(&(repn->value))) {
    value = *pval;
}
else {
    throw std::runtime_error("No integer value stored in index parameter.");
    }
}

void IndexParameter::get_value(std::string& value) const
{
if (!repn)
    throw std::runtime_error("No string value stored in index parameter.");

if (auto pval = std::get_if<std::string>(&(repn->value))) {
    value = *pval;
}
else {
    throw std::runtime_error("No string value stored in index parameter.");
    }
}

std::string IndexParameter::get_name() const
{
if (repn)
    return repn->name;
return "";
}

std::ostream& operator<<(std::ostream& ostr, const IndexParameter& arg)
{
if (arg.repn)
    write_expr(arg.repn, ostr);
return ostr;
}

//
// Variable
//
Variable::Variable(const VariableRepn& _repn)
{
repn = _repn;
OWN_POINTER(repn);
}

Variable::~Variable()
{
if (repn)
    DISOWN_POINTER(repn);
}

Variable::Variable(const Variable& expr)
{
repn = expr.repn;
OWN_POINTER(repn);
}

Variable::Variable(double lb, double ub, double value)
{
repn = CREATE_POINTER(VariableTerm, lb, ub, value, false, false);
OWN_POINTER(repn);
}

Variable::Variable(double lb, double ub, double value, const std::string& name)
{
repn = CREATE_POINTER(VariableTerm, lb, ub, value, false, false);
OWN_POINTER(repn);
repn->name = name;
}

Variable::Variable(double lb, double ub, double value, bool binary, bool integer, const std::string& name)
{
repn = CREATE_POINTER(VariableTerm, lb, ub, value, binary, integer);
OWN_POINTER(repn);
repn->name = name;
}

Variable::Variable(double lb, double ub, double value, bool binary, bool integer)
{
repn = CREATE_POINTER(VariableTerm, lb, ub, value, binary, integer);
OWN_POINTER(repn);
}

void Variable::initialize(double lb, double ub, double value, bool binary, bool integer, bool fixed)
{
repn->lb = lb;
repn->ub = ub;
repn->value = value;
repn->binary = binary;
repn->integer = integer;
repn->fixed = fixed;
}

void Variable::initialize(double lb, double ub, double value, bool binary, bool integer, bool fixed, const std::string& name)
{
initialize(lb, ub, value, binary, integer, fixed);
repn->name = name;
}

Variable& Variable::operator=(const Variable& expr)
{
DISOWN_POINTER(repn);
repn = expr.repn;
OWN_POINTER(repn);
return *this;
}

double Variable::get_value() const
{
return repn->value;
}

void Variable::set_value(double value)
{
repn->value = value;
}

double Variable::get_lb() const
{
return repn->lb;
}

void Variable::set_lb(double value)
{
repn->lb = value;
}

double Variable::get_ub() const
{
return repn->ub;
}

void Variable::set_ub(double value)
{
repn->ub = value;
}

unsigned int Variable::id() const
{
return repn->index;
}

std::string Variable::get_name() const
{
return repn->get_name();
}

bool Variable::is_continuous() const
{
return not (repn->binary or repn->integer);
}

bool Variable::is_binary() const
{
return repn->binary;
}

bool Variable::is_integer() const
{
return repn->integer;
}

std::ostream& operator<<(std::ostream& ostr, const Variable& arg)
{
write_expr(arg.repn, ostr);
return ostr;
}

void Variable::set_fixed(bool _flag)
{
repn->fixed = _flag;
}

bool Variable::get_fixed() const
{
return repn->fixed;
}

//
// Expression
//

Expression::Expression()
    : repn(ZEROCONST)
{
OWN_POINTER(repn);
}

Expression::Expression(const ExpressionRepn& _repn)
    : repn(_repn)
{
if (repn)
    OWN_POINTER(repn);
}

Expression::Expression(double value)
{
repn = STATIC_CAST(BaseExpressionTerm, CREATE_POINTER(ConstantTerm, value));
OWN_POINTER(repn);
}

Expression::Expression(int value)
{
repn = STATIC_CAST(BaseExpressionTerm, CREATE_POINTER(ConstantTerm, value));
OWN_POINTER(repn);
}

Expression::Expression(const Parameter& param)
{
repn = STATIC_CAST(BaseExpressionTerm, param.repn);
OWN_POINTER(repn);
}

Expression::Expression(const IndexParameter& param)
{
repn = STATIC_CAST(BaseExpressionTerm, param.repn);
OWN_POINTER(repn);
}

Expression::Expression(const Variable& var)
{
repn = STATIC_CAST(BaseExpressionTerm, var.repn);
OWN_POINTER(repn);
}

Expression::~Expression()
{
if (repn)
    DISOWN_POINTER(repn);
}

Expression::Expression(const Expression& expr)
{
repn = expr.repn;
OWN_POINTER(repn);
}

Expression& Expression::operator=(const Expression& expr)
{
DISOWN_POINTER(repn);
repn = expr.repn;
OWN_POINTER(repn);
return *this;
}

bool Expression::is_constant() const
{
return repn->is_constant();
}

double Expression::get_value() const
{
return repn->eval();
}

std::list<std::string> Expression::to_list() const
{
std::list<std::string> tmp;
expr_to_list(repn, tmp);
return tmp;
}

Expression Expression::diff(const Variable& var) const
{
std::map<VariableTerm*, expr_pointer_t> ans;
symbolic_diff_all(repn, ans);
Expression e;
if (ans.find(var.repn) != ans.end())
    e = ans[var.repn];
// Discarding all pointers that are not owned 
for (std::map<VariableTerm*, expr_pointer_t>::iterator it=ans.begin(); it != ans.end(); ++it)
    DISCARD_POINTER(it->second);
return e;
}

Expression Expression::expand()
{
return convert_expr_template(repn);
}

std::ostream& operator<<(std::ostream& ostr, const Expression& arg)
{
write_expr(arg.repn, ostr);
return ostr;
}

Expression& Expression::operator+=(int arg)
{ Expression e(arg); *this += e; return *this; }

Expression& Expression::operator+=(double arg)
{ Expression e(arg); *this += e; return *this; }

Expression& Expression::operator+=(const Parameter& arg)
{ Expression e(arg); *this += e; return *this; }

Expression& Expression::operator+=(const IndexParameter& arg)
{ Expression e(arg); *this += e; return *this; }

Expression& Expression::operator+=(const Variable& arg)
{ Expression e(arg); *this += e; return *this; }

Expression& Expression::operator+=(const Expression& arg)
{
// SHARED_PTR
//repn = CREATE_POINTER(PlusTerm, repn, arg.repn);
expr_pointer_t _repn = repn;
repn = CREATE_POINTER(PlusTerm, _repn, arg.repn);
DISOWN_POINTER(_repn);
OWN_POINTER(repn);
return *this;
}

Expression& Expression::operator-=(int arg)
{ Expression e(arg); *this -= e; return *this; }

Expression& Expression::operator-=(double arg)
{ Expression e(arg); *this -= e; return *this; }

Expression& Expression::operator-=(const Parameter& arg)
{ Expression e(arg); *this -= e; return *this; }

Expression& Expression::operator-=(const IndexParameter& arg)
{ Expression e(arg); *this -= e; return *this; }

Expression& Expression::operator-=(const Variable& arg)
{ Expression e(arg); *this -= e; return *this; }

Expression& Expression::operator-=(const Expression& arg)
{
expr_pointer_t tmp = CREATE_POINTER(NegateTerm, arg.repn);
// SHARED_PTR
//repn = STATIC_CAST(BaseExpressionTerm, CREATE_POINTER(PlusTerm, repn, tmp) );
expr_pointer_t _repn = repn;
repn = STATIC_CAST(BaseExpressionTerm, CREATE_POINTER(PlusTerm, _repn, tmp) );
DISOWN_POINTER(_repn);
OWN_POINTER(repn);
return *this;
}

Expression& Expression::operator*=(int arg)
{ Expression e(arg); *this *= e; return *this; }

Expression& Expression::operator*=(double arg)
{ Expression e(arg); *this *= e; return *this; }

Expression& Expression::operator*=(const Parameter& arg)
{ Expression e(arg); *this *= e; return *this; }

Expression& Expression::operator*=(const IndexParameter& arg)
{ Expression e(arg); *this *= e; return *this; }

Expression& Expression::operator*=(const Variable& arg)
{ Expression e(arg); *this *= e; return *this; }

Expression& Expression::operator*=(const Expression& arg)
{
// SHARED_PTR
//repn = STATIC_CAST(BaseExpressionTerm, CREATE_POINTER(TimesTerm, repn, arg.repn) );
expr_pointer_t _repn = repn;
repn = STATIC_CAST(BaseExpressionTerm, times(_repn, arg.repn) );
DISOWN_POINTER(_repn);
OWN_POINTER(repn);
return *this;
}

Expression& Expression::operator/=(int arg)
{ Expression e(arg); *this /= e; return *this; }

Expression& Expression::operator/=(double arg)
{ Expression e(arg); *this /= e; return *this; }

Expression& Expression::operator/=(const Parameter& arg)
{ Expression e(arg); *this /= e; return *this; }

Expression& Expression::operator/=(const IndexParameter& arg)
{ Expression e(arg); *this /= e; return *this; }

Expression& Expression::operator/=(const Variable& arg)
{ Expression e(arg); *this /= e; return *this; }

Expression& Expression::operator/=(const Expression& arg)
{
// SHARED_PTR
//repn = STATIC_CAST(BaseExpressionTerm, CREATE_POINTER(DivideTerm, repn, arg.repn) );
expr_pointer_t _repn = repn;
repn = STATIC_CAST(BaseExpressionTerm, CREATE_POINTER(DivideTerm, _repn, arg.repn) );
DISOWN_POINTER(_repn);
OWN_POINTER(repn);
return *this;
}

//
// Constraint
//

Constraint::Constraint()
    : repn(DUMMYCONSTRAINT)
{
OWN_POINTER(repn);
}

Constraint::Constraint(const ConstraintRepn& _repn)
    : repn(_repn)
{
OWN_POINTER(repn);
}

Constraint::Constraint(const Constraint& expr)
{
repn = expr.repn;
OWN_POINTER(repn);
}

Constraint::~Constraint()
{
DISOWN_POINTER(repn);
}

Constraint& Constraint::operator=(const Constraint& expr)
{
DISOWN_POINTER(repn);
repn = expr.repn;
OWN_POINTER(repn);
return *this;
}

unsigned int Constraint::id() const
{
if (repn)
    return repn->index;
// TODO - throw an exception here?
return 0;
}

bool Constraint::is_inequality() const
{ return repn->is_inequality(); }

bool Constraint::is_equality() const
{ return repn->is_equality(); }

bool Constraint::is_feasible() const
{ return repn->is_feasible(); }

Expression Constraint::lower() const
{ return repn->lower; }

Expression Constraint::body() const
{ return repn->body; }

Expression Constraint::upper() const
{ return repn->upper; }

double Constraint::get_lb() const
{
if (repn->lower)
    return repn->lower->eval();
return -COEK_INFINITY;
}

double Constraint::get_ub() const
{
if (repn->upper)
    return repn->upper->eval();
return COEK_INFINITY;
}

Constraint Constraint::expand()
{ return convert_con_template(repn); }

std::list<std::string> Constraint::to_list() const
{
std::list<std::string> tmp;
expr_to_list(repn, tmp);
return tmp;
}

std::ostream& operator<<(std::ostream& ostr, const Constraint& arg)
{
write_expr(arg.repn, ostr);
return ostr;
}

//
// QuadraticExpr
//

void to_QuadraticExpr(expr_pointer_t expr, QuadraticExpr& repn);

void QuadraticExpr::collect_terms(const Expression& expr)
{
to_QuadraticExpr(expr.repn, *this);
}

void QuadraticExpr::collect_terms(const Constraint& expr)
{
to_QuadraticExpr(expr.repn, *this);
}

//
// MutableNLPExpr
//

bool MutableNLPExpr::varterm_compare::operator() (const VariableTerm* lhs, const VariableTerm* rhs) const
{ return lhs->index < rhs->index; }

void to_MutableNLPExpr(expr_pointer_t expr,
                    MutableNLPExpr& repn);

void MutableNLPExpr::collect_terms(Expression& expr)
{
to_MutableNLPExpr(expr.repn, *this);
}

void MutableNLPExpr::collect_terms(Constraint& expr)
{
to_MutableNLPExpr(expr.repn, *this);
}

//
// -------------------------------------------------------------------------------------
// OPERATORS
// -------------------------------------------------------------------------------------

//
// operator+
//
Expression operator+(const Parameter& param)
{ return STATIC_CAST(BaseExpressionTerm, param.repn); }
Expression operator+(const IndexParameter& param)
{ return STATIC_CAST(BaseExpressionTerm, param.repn); }
Expression operator+(const Variable& var)
{ return STATIC_CAST(BaseExpressionTerm, var.repn); }
Expression operator+(const Expression& expr)
{ return expr; }

Expression operator+(int lhs, const Parameter& rhs)
{ return plus(lhs, rhs.repn); }
Expression operator+(int lhs, const IndexParameter& rhs)
{ return plus(lhs, rhs.repn); }
Expression operator+(int lhs, const Variable& rhs)
{ return plus(lhs, rhs.repn); }
Expression operator+(int lhs, const Expression& rhs)
{ return plus(lhs, rhs.repn); }
Expression operator+(double lhs, const Parameter& rhs)
{return plus(lhs, rhs.repn);}
Expression operator+(double lhs, const IndexParameter& rhs)
{return plus(lhs, rhs.repn);}
Expression operator+(double lhs, const Variable& rhs)
{return plus(lhs, rhs.repn);}
Expression operator+(double lhs, const Expression& rhs)
{return plus(lhs, rhs.repn);}

Expression Parameter::operator+(int arg) const
{ return plus(repn, arg); }
Expression IndexParameter::operator+(int arg) const
{ return plus(repn, arg); }
Expression Variable::operator+(int arg) const
{ return plus(repn, arg); }
Expression Expression::operator+(int arg) const
{ return plus(repn, arg); }
Expression Parameter::operator+(double arg) const
{ return plus(repn, arg); }
Expression IndexParameter::operator+(double arg) const
{ return plus(repn, arg); }
Expression Variable::operator+(double arg) const
{ return plus(repn, arg); }
Expression Expression::operator+(double arg) const
{ return plus(repn, arg); }

Expression Parameter::operator+(const Parameter& arg) const
{ return STATIC_CAST(BaseExpressionTerm, CREATE_POINTER(PlusTerm, repn, arg.repn) ); }
Expression Parameter::operator+(const IndexParameter& arg) const
{ return STATIC_CAST(BaseExpressionTerm, CREATE_POINTER(PlusTerm, repn, arg.repn) ); }
Expression Parameter::operator+(const Variable& arg) const
{ return STATIC_CAST(BaseExpressionTerm, CREATE_POINTER(PlusTerm, repn, arg.repn) ); }
Expression Parameter::operator+(const Expression& arg) const
{ return STATIC_CAST(BaseExpressionTerm, CREATE_POINTER(PlusTerm, repn, arg.repn) ); }

Expression IndexParameter::operator+(const Parameter& arg) const
{ return STATIC_CAST(BaseExpressionTerm, CREATE_POINTER(PlusTerm, repn, arg.repn) ); }
Expression IndexParameter::operator+(const IndexParameter& arg) const
{ return STATIC_CAST(BaseExpressionTerm, CREATE_POINTER(PlusTerm, repn, arg.repn) ); }
Expression IndexParameter::operator+(const Variable& arg) const
{ return STATIC_CAST(BaseExpressionTerm, CREATE_POINTER(PlusTerm, repn, arg.repn) ); }
Expression IndexParameter::operator+(const Expression& arg) const
{ return STATIC_CAST(BaseExpressionTerm, CREATE_POINTER(PlusTerm, repn, arg.repn) ); }

Expression Variable::operator+(const Parameter& arg) const
{ return STATIC_CAST(BaseExpressionTerm, CREATE_POINTER(PlusTerm, repn, arg.repn) ); }
Expression Variable::operator+(const IndexParameter& arg) const
{ return STATIC_CAST(BaseExpressionTerm, CREATE_POINTER(PlusTerm, repn, arg.repn) ); }
Expression Variable::operator+(const Variable& arg) const
{ return STATIC_CAST(BaseExpressionTerm, CREATE_POINTER(PlusTerm, repn, arg.repn) ); }
Expression Variable::operator+(const Expression& arg) const
{ return STATIC_CAST(BaseExpressionTerm, CREATE_POINTER(PlusTerm, repn, arg.repn) ); }

Expression Expression::operator+(const Parameter& arg) const
{
//if (repn == ZeroConstant.repn)
//    return arg.repn;
return STATIC_CAST(BaseExpressionTerm, CREATE_POINTER(PlusTerm, repn, arg.repn) );
}
Expression Expression::operator+(const IndexParameter& arg) const
{
//if (repn == ZeroConstant.repn)
//    return arg.repn;
return STATIC_CAST(BaseExpressionTerm, CREATE_POINTER(PlusTerm, repn, arg.repn) );
}
Expression Expression::operator+(const Variable& arg) const
{
//if (repn == ZeroConstant.repn)
//    return arg.repn;
return STATIC_CAST(BaseExpressionTerm, CREATE_POINTER(PlusTerm, repn, arg.repn) );
}
Expression Expression::operator+(const Expression& arg) const
{
//if (repn == ZeroConstant.repn)
//    return arg.repn;
//if (arg.repn == ZeroConstant.repn)
//    return repn;
return STATIC_CAST(BaseExpressionTerm, CREATE_POINTER(PlusTerm, repn, arg.repn) );
}

//
// operator-
//
Expression operator-(const Parameter& param)
{ return param.repn->negate(param.repn); }
Expression operator-(const IndexParameter& param)
{ return param.repn->negate(param.repn); }
Expression operator-(const Variable& var)
{ return var.repn->negate(var.repn); }
Expression operator-(const Expression& expr)
{ return expr.repn->negate(expr.repn); }

Expression operator-(int lhs, const Parameter& rhs)
{return minus(lhs, rhs.repn);}
Expression operator-(int lhs, const IndexParameter& rhs)
{return minus(lhs, rhs.repn);}
Expression operator-(int lhs, const Variable& rhs)
{return minus(lhs, rhs.repn);}
Expression operator-(int lhs, const Expression& rhs)
{
//if (rhs.repn == ZeroConstant.repn)
//    return repn;
return minus(lhs, rhs.repn);
}
Expression operator-(double lhs, const Parameter& rhs)
{return minus(lhs, rhs.repn);}
Expression operator-(double lhs, const IndexParameter& rhs)
{return minus(lhs, rhs.repn);}
Expression operator-(double lhs, const Variable& rhs)
{return minus(lhs, rhs.repn);}
Expression operator-(double lhs, const Expression& rhs)
{
//if (rhs.repn == ZeroConstant.repn)
//    return repn;
return minus(lhs, rhs.repn);
}

Expression Parameter::operator-(int arg) const
{ return plus(repn, -arg); }
Expression IndexParameter::operator-(int arg) const
{ return plus(repn, -arg); }
Expression Variable::operator-(int arg) const
{ return plus(repn, -arg); }
Expression Expression::operator-(int arg) const
{ return plus(repn, -arg); }
Expression Parameter::operator-(double arg) const
{ return plus(repn, -arg); }
Expression IndexParameter::operator-(double arg) const
{ return plus(repn, -arg); }
Expression Variable::operator-(double arg) const
{ return plus(repn, -arg); }
Expression Expression::operator-(double arg) const
{ return plus(repn, -arg); }

Expression Parameter::operator-(const Parameter& arg) const
{
return STATIC_CAST(BaseExpressionTerm,
        CREATE_POINTER(PlusTerm, repn, arg.repn->negate(arg.repn) ) );
}
Expression Parameter::operator-(const IndexParameter& arg) const
{
return STATIC_CAST(BaseExpressionTerm,
        CREATE_POINTER(PlusTerm, repn, arg.repn->negate(arg.repn) ) );
}
Expression Parameter::operator-(const Variable& arg) const
{
return STATIC_CAST(BaseExpressionTerm,
        CREATE_POINTER(PlusTerm, repn, arg.repn->negate(arg.repn) ) );
}
Expression Parameter::operator-(const Expression& arg) const
{
//if (rhs.repn == ZeroConstant.repn)
//    return repn;
return STATIC_CAST(BaseExpressionTerm,
        CREATE_POINTER(PlusTerm, repn, arg.repn->negate(arg.repn) ) );
}

Expression IndexParameter::operator-(const Parameter& arg) const
{
return STATIC_CAST(BaseExpressionTerm,
        CREATE_POINTER(PlusTerm, repn, arg.repn->negate(arg.repn) ) );
}
Expression IndexParameter::operator-(const IndexParameter& arg) const
{
return STATIC_CAST(BaseExpressionTerm,
        CREATE_POINTER(PlusTerm, repn, arg.repn->negate(arg.repn) ) );
}
Expression IndexParameter::operator-(const Variable& arg) const
{
return STATIC_CAST(BaseExpressionTerm,
        CREATE_POINTER(PlusTerm, repn, arg.repn->negate(arg.repn) ) );
}
Expression IndexParameter::operator-(const Expression& arg) const
{
//if (rhs.repn == ZeroConstant.repn)
//    return repn;
return STATIC_CAST(BaseExpressionTerm,
        CREATE_POINTER(PlusTerm, repn, arg.repn->negate(arg.repn) ) );
}

Expression Variable::operator-(const Parameter& arg) const
{
return STATIC_CAST(BaseExpressionTerm,
        CREATE_POINTER(PlusTerm, repn, arg.repn->negate(arg.repn) ) );
}
Expression Variable::operator-(const IndexParameter& arg) const
{
return STATIC_CAST(BaseExpressionTerm,
        CREATE_POINTER(PlusTerm, repn, arg.repn->negate(arg.repn) ) );
}
Expression Variable::operator-(const Variable& arg) const
{
return STATIC_CAST(BaseExpressionTerm,
        CREATE_POINTER(PlusTerm, repn, arg.repn->negate(arg.repn) ) );
}
Expression Variable::operator-(const Expression& arg) const
{
return STATIC_CAST(BaseExpressionTerm,
        CREATE_POINTER(PlusTerm, repn, arg.repn->negate(arg.repn) ) );
}

Expression Expression::operator-(const Parameter& arg) const
{
return STATIC_CAST(BaseExpressionTerm,
        CREATE_POINTER(PlusTerm, repn, arg.repn->negate(arg.repn) ) );
}
Expression Expression::operator-(const IndexParameter& arg) const
{
return STATIC_CAST(BaseExpressionTerm,
        CREATE_POINTER(PlusTerm, repn, arg.repn->negate(arg.repn) ) );
}
Expression Expression::operator-(const Variable& arg) const
{
return STATIC_CAST(BaseExpressionTerm,
        CREATE_POINTER(PlusTerm, repn, arg.repn->negate(arg.repn) ) );
}
Expression Expression::operator-(const Expression& arg) const
{
//if (rhs.repn == ZeroConstant.repn)
//    return repn;
//if (arg.repn == ZeroConstant.repn)
//    return repn;

return STATIC_CAST(BaseExpressionTerm,
        CREATE_POINTER(PlusTerm, repn, arg.repn->negate(arg.repn) ) );
}

//
// operator*
//
Expression operator*(int lhs, const Parameter& rhs)
{return times(lhs, rhs.repn);}
Expression operator*(int lhs, const IndexParameter& rhs)
{return times(lhs, rhs.repn);}
Expression operator*(int lhs, const Variable& rhs)
{
if (lhs == 0)
    return ZEROCONST;
if (lhs == 1)
    return rhs.repn;
return STATIC_CAST(BaseExpressionTerm,
        rhs.repn->const_mult(lhs, rhs.repn) );
}
Expression operator*(int lhs, const Expression& rhs)
{return times(lhs, rhs.repn);}
Expression operator*(double lhs, const Parameter& rhs)
{return times(lhs, rhs.repn);}
Expression operator*(double lhs, const IndexParameter& rhs)
{return times(lhs, rhs.repn);}
Expression operator*(double lhs, const Variable& rhs)
{
if (lhs == 0.0)
    return ZEROCONST;
if (lhs == 1.0)
    return rhs.repn;
return STATIC_CAST(BaseExpressionTerm,
        rhs.repn->const_mult(lhs, rhs.repn) );
}
Expression operator*(double lhs, const Expression& rhs)
{return times(lhs, rhs.repn);}

Expression Parameter::operator*(int arg) const
{ return times(repn, arg); }
Expression IndexParameter::operator*(int arg) const
{ return times(repn, arg); }
Expression Variable::operator*(int arg) const
{
if (arg == 0)
    return ZEROCONST;
if (arg == 1)
    return repn;
return STATIC_CAST(BaseExpressionTerm,
        repn->const_mult(arg, repn) );
}
Expression Expression::operator*(int arg) const
{ return times(repn, arg); }
Expression Parameter::operator*(double arg) const
{ return times(repn, arg); }
Expression IndexParameter::operator*(double arg) const
{ return times(repn, arg); }
Expression Variable::operator*(double arg) const
{
if (arg == 0.0)
    return ZEROCONST;
if (arg == 1.0)
    return repn;
return STATIC_CAST(BaseExpressionTerm,
        repn->const_mult(arg, repn) );
}
Expression Expression::operator*(double arg) const
{ return times(repn, arg); }


Expression Parameter::operator*(const Parameter& arg) const
{ return times(repn, arg.repn); }
Expression Parameter::operator*(const IndexParameter& arg) const
{ return times(repn, arg.repn); }
Expression Parameter::operator*(const Variable& arg) const
{ return times(repn, arg.repn); }
Expression Parameter::operator*(const Expression& arg) const
{ return times(repn, arg.repn); }

Expression IndexParameter::operator*(const Parameter& arg) const
{ return times(repn, arg.repn); }
Expression IndexParameter::operator*(const IndexParameter& arg) const
{ return times(repn, arg.repn); }
Expression IndexParameter::operator*(const Variable& arg) const
{ return times(repn, arg.repn); }
Expression IndexParameter::operator*(const Expression& arg) const
{ return times(repn, arg.repn); }

Expression Variable::operator*(const Parameter& arg) const
{ return times(repn, arg.repn); }
Expression Variable::operator*(const IndexParameter& arg) const
{ return times(repn, arg.repn); }
Expression Variable::operator*(const Variable& arg) const
{ return times(repn, arg.repn); }
Expression Variable::operator*(const Expression& arg) const
{ return times(repn, arg.repn); }

Expression Expression::operator*(const Parameter& arg) const
{ return times(repn, arg.repn); }
Expression Expression::operator*(const IndexParameter& arg) const
{ return times(repn, arg.repn); }
Expression Expression::operator*(const Variable& arg) const
{ return times(repn, arg.repn); }
Expression Expression::operator*(const Expression& arg) const
{ return times(repn, arg.repn); }

//
// operator/
//
Expression operator/(int lhs, const Parameter& rhs)
{return divide(lhs, rhs.repn);}
Expression operator/(int lhs, const IndexParameter& rhs)
{return divide(lhs, rhs.repn);}
Expression operator/(int lhs, const Variable& rhs)
{return divide(lhs, rhs.repn);}
Expression operator/(int lhs, const Expression& rhs)
{return divide(lhs, rhs.repn);}
Expression operator/(double lhs, const Parameter& rhs)
{return divide(lhs, rhs.repn);}
Expression operator/(double lhs, const IndexParameter& rhs)
{return divide(lhs, rhs.repn);}
Expression operator/(double lhs, const Variable& rhs)
{return divide(lhs, rhs.repn);}
Expression operator/(double lhs, const Expression& rhs)
{return divide(lhs, rhs.repn);}

Expression Parameter::operator/(int arg) const
{ return divide(repn, arg); }
Expression IndexParameter::operator/(int arg) const
{ return divide(repn, arg); }
Expression Variable::operator/(int arg) const
{
if (arg == 1)
    return repn;
if (arg == 0)
    throw std::domain_error("Division by zero.");
return STATIC_CAST(BaseExpressionTerm,
        repn->const_mult(1.0/arg, repn) );
}
Expression Expression::operator/(int arg) const
{ return divide(repn, arg); }
Expression Parameter::operator/(double arg) const
{ return divide(repn, arg); }
Expression IndexParameter::operator/(double arg) const
{ return divide(repn, arg); }
Expression Variable::operator/(double arg) const
{
if (arg == 1.0)
    return repn;
if (arg == 0.0)
    throw std::domain_error("Division by zero.");
return STATIC_CAST(BaseExpressionTerm,
        repn->const_mult(1.0/arg, repn) );
}
Expression Expression::operator/(double arg) const
{ return divide(repn, arg); }

Expression Parameter::operator/(const Parameter& arg) const
{ return divide(repn, arg.repn); }
Expression Parameter::operator/(const IndexParameter& arg) const
{ return divide(repn, arg.repn); }
Expression Parameter::operator/(const Variable& arg) const
{ return divide(repn, arg.repn); }
Expression Parameter::operator/(const Expression& arg) const
{ return divide(repn, arg.repn); }

Expression IndexParameter::operator/(const Parameter& arg) const
{ return divide(repn, arg.repn); }
Expression IndexParameter::operator/(const IndexParameter& arg) const
{ return divide(repn, arg.repn); }
Expression IndexParameter::operator/(const Variable& arg) const
{ return divide(repn, arg.repn); }
Expression IndexParameter::operator/(const Expression& arg) const
{ return divide(repn, arg.repn); }

Expression Variable::operator/(const Parameter& arg) const
{ return divide(repn, arg.repn); }
Expression Variable::operator/(const IndexParameter& arg) const
{ return divide(repn, arg.repn); }
Expression Variable::operator/(const Variable& arg) const
{ return divide(repn, arg.repn); }
Expression Variable::operator/(const Expression& arg) const
{ return divide(repn, arg.repn); }

Expression Expression::operator/(const Parameter& arg) const
{ return divide(repn, arg.repn); }
Expression Expression::operator/(const IndexParameter& arg) const
{ return divide(repn, arg.repn); }
Expression Expression::operator/(const Variable& arg) const
{ return divide(repn, arg.repn); }
Expression Expression::operator/(const Expression& arg) const
{ return divide(repn, arg.repn); }

//
// operator<
//
Constraint operator<(int lhs, const Parameter& rhs)
{return less_than(lhs, rhs.repn, true);}
Constraint operator<(int lhs, const IndexParameter& rhs)
{return less_than(lhs, rhs.repn, true);}
Constraint operator<(int lhs, const Variable& rhs)
{return less_than(lhs, rhs.repn, true);}
Constraint operator<(int lhs, const Expression& rhs)
{return less_than(lhs, rhs.repn, true);}
Constraint operator<(double lhs, const Parameter& rhs)
{return less_than(lhs, rhs.repn, true);}
Constraint operator<(double lhs, const IndexParameter& rhs)
{return less_than(lhs, rhs.repn, true);}
Constraint operator<(double lhs, const Variable& rhs)
{return less_than(lhs, rhs.repn, true);}
Constraint operator<(double lhs, const Expression& rhs)
{return less_than(lhs, rhs.repn, true);}

Constraint Parameter::operator<(double rhs) const
{return less_than(repn, rhs, true);}
Constraint Parameter::operator<(int rhs) const
{return less_than(repn, rhs, true);}
Constraint Parameter::operator<(const Parameter& rhs) const
{return less_than(repn, rhs.repn, true);}
Constraint Parameter::operator<(const IndexParameter& rhs) const
{return less_than(repn, rhs.repn, true);}
Constraint Parameter::operator<(const Variable& rhs) const
{return less_than(repn, rhs.repn, true);}
Constraint Parameter::operator<(const Expression& rhs) const
{return less_than(repn, rhs.repn, true);}

Constraint IndexParameter::operator<(double rhs) const
{return less_than(repn, rhs, true);}
Constraint IndexParameter::operator<(int rhs) const
{return less_than(repn, rhs, true);}
Constraint IndexParameter::operator<(const Parameter& rhs) const
{return less_than(repn, rhs.repn, true);}
Constraint IndexParameter::operator<(const IndexParameter& rhs) const
{return less_than(repn, rhs.repn, true);}
Constraint IndexParameter::operator<(const Variable& rhs) const
{return less_than(repn, rhs.repn, true);}
Constraint IndexParameter::operator<(const Expression& rhs) const
{return less_than(repn, rhs.repn, true);}

Constraint Variable::operator<(double rhs) const
{return less_than(repn, rhs, true);}
Constraint Variable::operator<(int rhs) const
{return less_than(repn, rhs, true);}
Constraint Variable::operator<(const Parameter& rhs) const
{return less_than(repn, rhs.repn, true);}
Constraint Variable::operator<(const IndexParameter& rhs) const
{return less_than(repn, rhs.repn, true);}
Constraint Variable::operator<(const Variable& rhs) const
{return less_than(repn, rhs.repn, true);}
Constraint Variable::operator<(const Expression& rhs) const
{return less_than(repn, rhs.repn, true);}

Constraint Expression::operator<(double rhs) const
{return less_than(repn, rhs, true);}
Constraint Expression::operator<(int rhs) const
{return less_than(repn, rhs, true);}
Constraint Expression::operator<(const Parameter& rhs) const
{return less_than(repn, rhs.repn, true);}
Constraint Expression::operator<(const IndexParameter& rhs) const
{return less_than(repn, rhs.repn, true);}
Constraint Expression::operator<(const Variable& rhs) const
{return less_than(repn, rhs.repn, true);}
Constraint Expression::operator<(const Expression& rhs) const
{return less_than(repn, rhs.repn, true);}

//
// operator<=
//
Constraint operator<=(int lhs, const Parameter& rhs)
{return less_than(lhs, rhs.repn, false);}
Constraint operator<=(int lhs, const IndexParameter& rhs)
{return less_than(lhs, rhs.repn, false);}
Constraint operator<=(int lhs, const Variable& rhs)
{return less_than(lhs, rhs.repn, false);}
Constraint operator<=(int lhs, const Expression& rhs)
{return less_than(lhs, rhs.repn, false);}
Constraint operator<=(double lhs, const Parameter& rhs)
{return less_than(lhs, rhs.repn, false);}
Constraint operator<=(double lhs, const IndexParameter& rhs)
{return less_than(lhs, rhs.repn, false);}
Constraint operator<=(double lhs, const Variable& rhs)
{return less_than(lhs, rhs.repn, false);}
Constraint operator<=(double lhs, const Expression& rhs)
{return less_than(lhs, rhs.repn, false);}

Constraint Parameter::operator<=(double rhs) const
{return less_than(repn, rhs, false);}
Constraint Parameter::operator<=(int rhs) const
{return less_than(repn, rhs, false);}
Constraint Parameter::operator<=(const Parameter& rhs) const
{return less_than(repn, rhs.repn, false);}
Constraint Parameter::operator<=(const IndexParameter& rhs) const
{return less_than(repn, rhs.repn, false);}
Constraint Parameter::operator<=(const Variable& rhs) const
{return less_than(repn, rhs.repn, false);}
Constraint Parameter::operator<=(const Expression& rhs) const
{return less_than(repn, rhs.repn, false);}

Constraint IndexParameter::operator<=(double rhs) const
{return less_than(repn, rhs, false);}
Constraint IndexParameter::operator<=(int rhs) const
{return less_than(repn, rhs, false);}
Constraint IndexParameter::operator<=(const Parameter& rhs) const
{return less_than(repn, rhs.repn, false);}
Constraint IndexParameter::operator<=(const IndexParameter& rhs) const
{return less_than(repn, rhs.repn, false);}
Constraint IndexParameter::operator<=(const Variable& rhs) const
{return less_than(repn, rhs.repn, false);}
Constraint IndexParameter::operator<=(const Expression& rhs) const
{return less_than(repn, rhs.repn, false);}

Constraint Variable::operator<=(double rhs) const
{return less_than(repn, rhs, false);}
Constraint Variable::operator<=(int rhs) const
{return less_than(repn, rhs, false);}
Constraint Variable::operator<=(const Parameter& rhs) const
{return less_than(repn, rhs.repn, false);}
Constraint Variable::operator<=(const IndexParameter& rhs) const
{return less_than(repn, rhs.repn, false);}
Constraint Variable::operator<=(const Variable& rhs) const
{return less_than(repn, rhs.repn, false);}
Constraint Variable::operator<=(const Expression& rhs) const
{return less_than(repn, rhs.repn, false);}

Constraint Expression::operator<=(double rhs) const
{return less_than(repn, rhs, false);}
Constraint Expression::operator<=(int rhs) const
{return less_than(repn, rhs, false);}
Constraint Expression::operator<=(const Parameter& rhs) const
{return less_than(repn, rhs.repn, false);}
Constraint Expression::operator<=(const IndexParameter& rhs) const
{return less_than(repn, rhs.repn, false);}
Constraint Expression::operator<=(const Variable& rhs) const
{return less_than(repn, rhs.repn, false);}
Constraint Expression::operator<=(const Expression& rhs) const
{return less_than(repn, rhs.repn, false);}

//
// operator>
//
Constraint operator>(int lhs, const Parameter& rhs)
{return less_than(rhs.repn, lhs, true);}
Constraint operator>(int lhs, const IndexParameter& rhs)
{return less_than(rhs.repn, lhs, true);}
Constraint operator>(int lhs, const Variable& rhs)
{return less_than(rhs.repn, lhs, true);}
Constraint operator>(int lhs, const Expression& rhs)
{return less_than(rhs.repn, lhs, true);}
Constraint operator>(double lhs, const Parameter& rhs)
{return less_than(rhs.repn, lhs, true);}
Constraint operator>(double lhs, const IndexParameter& rhs)
{return less_than(rhs.repn, lhs, true);}
Constraint operator>(double lhs, const Variable& rhs)
{return less_than(rhs.repn, lhs, true);}
Constraint operator>(double lhs, const Expression& rhs)
{return less_than(rhs.repn, lhs, true);}

Constraint Parameter::operator>(double rhs) const
{return less_than(rhs, repn, true);}
Constraint Parameter::operator>(int rhs) const
{return less_than(rhs, repn, true);}
Constraint Parameter::operator>(const Parameter& rhs) const
{return less_than(rhs.repn, repn, true);}
Constraint Parameter::operator>(const IndexParameter& rhs) const
{return less_than(rhs.repn, repn, true);}
Constraint Parameter::operator>(const Variable& rhs) const
{return less_than(rhs.repn, repn, true);}
Constraint Parameter::operator>(const Expression& rhs) const
{return less_than(rhs.repn, repn, true);}

Constraint IndexParameter::operator>(double rhs) const
{return less_than(rhs, repn, true);}
Constraint IndexParameter::operator>(int rhs) const
{return less_than(rhs, repn, true);}
Constraint IndexParameter::operator>(const Parameter& rhs) const
{return less_than(rhs.repn, repn, true);}
Constraint IndexParameter::operator>(const IndexParameter& rhs) const
{return less_than(rhs.repn, repn, true);}
Constraint IndexParameter::operator>(const Variable& rhs) const
{return less_than(rhs.repn, repn, true);}
Constraint IndexParameter::operator>(const Expression& rhs) const
{return less_than(rhs.repn, repn, true);}

Constraint Variable::operator>(double rhs) const
{return less_than(rhs, repn, true);}
Constraint Variable::operator>(int rhs) const
{return less_than(rhs, repn, true);}
Constraint Variable::operator>(const Parameter& rhs) const
{return less_than(rhs.repn, repn, true);}
Constraint Variable::operator>(const IndexParameter& rhs) const
{return less_than(rhs.repn, repn, true);}
Constraint Variable::operator>(const Variable& rhs) const
{return less_than(rhs.repn, repn, true);}
Constraint Variable::operator>(const Expression& rhs) const
{return less_than(rhs.repn, repn, true);}

Constraint Expression::operator>(double rhs) const
{return less_than(rhs, repn, true);}
Constraint Expression::operator>(int rhs) const
{return less_than(rhs, repn, true);}
Constraint Expression::operator>(const Parameter& rhs) const
{return less_than(rhs.repn, repn, true);}
Constraint Expression::operator>(const IndexParameter& rhs) const
{return less_than(rhs.repn, repn, true);}
Constraint Expression::operator>(const Variable& rhs) const
{return less_than(rhs.repn, repn, true);}
Constraint Expression::operator>(const Expression& rhs) const
{return less_than(rhs.repn, repn, true);}

//
// operator>=
//
Constraint operator>=(int lhs, const Parameter& rhs)
{return less_than(rhs.repn, lhs, false);}
Constraint operator>=(int lhs, const IndexParameter& rhs)
{return less_than(rhs.repn, lhs, false);}
Constraint operator>=(int lhs, const Variable& rhs)
{return less_than(rhs.repn, lhs, false);}
Constraint operator>=(int lhs, const Expression& rhs)
{return less_than(rhs.repn, lhs, false);}
Constraint operator>=(double lhs, const Parameter& rhs)
{return less_than(rhs.repn, lhs, false);}
Constraint operator>=(double lhs, const IndexParameter& rhs)
{return less_than(rhs.repn, lhs, false);}
Constraint operator>=(double lhs, const Variable& rhs)
{return less_than(rhs.repn, lhs, false);}
Constraint operator>=(double lhs, const Expression& rhs)
{return less_than(rhs.repn, lhs, false);}

Constraint Parameter::operator>=(double rhs) const
{return less_than(rhs, repn, false);}
Constraint Parameter::operator>=(int rhs) const
{return less_than(rhs, repn, false);}
Constraint Parameter::operator>=(const Parameter& rhs) const
{return less_than(rhs.repn, repn, false);}
Constraint Parameter::operator>=(const IndexParameter& rhs) const
{return less_than(rhs.repn, repn, false);}
Constraint Parameter::operator>=(const Variable& rhs) const
{return less_than(rhs.repn, repn, false);}
Constraint Parameter::operator>=(const Expression& rhs) const
{return less_than(rhs.repn, repn, false);}

Constraint IndexParameter::operator>=(double rhs) const
{return less_than(rhs, repn, false);}
Constraint IndexParameter::operator>=(int rhs) const
{return less_than(rhs, repn, false);}
Constraint IndexParameter::operator>=(const Parameter& rhs) const
{return less_than(rhs.repn, repn, false);}
Constraint IndexParameter::operator>=(const IndexParameter& rhs) const
{return less_than(rhs.repn, repn, false);}
Constraint IndexParameter::operator>=(const Variable& rhs) const
{return less_than(rhs.repn, repn, false);}
Constraint IndexParameter::operator>=(const Expression& rhs) const
{return less_than(rhs.repn, repn, false);}

Constraint Variable::operator>=(double rhs) const
{return less_than(rhs, repn, false);}
Constraint Variable::operator>=(int rhs) const
{return less_than(rhs, repn, false);}
Constraint Variable::operator>=(const Parameter& rhs) const
{return less_than(rhs.repn, repn, false);}
Constraint Variable::operator>=(const IndexParameter& rhs) const
{return less_than(rhs.repn, repn, false);}
Constraint Variable::operator>=(const Variable& rhs) const
{return less_than(rhs.repn, repn, false);}
Constraint Variable::operator>=(const Expression& rhs) const
{return less_than(rhs.repn, repn, false);}

Constraint Expression::operator>=(double rhs) const
{return less_than(rhs, repn, false);}
Constraint Expression::operator>=(int rhs) const
{return less_than(rhs, repn, false);}
Constraint Expression::operator>=(const Parameter& rhs) const
{return less_than(rhs.repn, repn, false);}
Constraint Expression::operator>=(const IndexParameter& rhs) const
{return less_than(rhs.repn, repn, false);}
Constraint Expression::operator>=(const Variable& rhs) const
{return less_than(rhs.repn, repn, false);}
Constraint Expression::operator>=(const Expression& rhs) const
{return less_than(rhs.repn, repn, false);}

//
// operator==
//
Constraint operator==(int lhs, const Parameter& rhs)
{return equal(lhs, rhs.repn);}
Constraint operator==(int lhs, const IndexParameter& rhs)
{return equal(lhs, rhs.repn);}
Constraint operator==(int lhs, const Variable& rhs)
{return equal(lhs, rhs.repn);}
Constraint operator==(int lhs, const Expression& rhs)
{return equal(lhs, rhs.repn);}
Constraint operator==(double lhs, const Parameter& rhs)
{return equal(lhs, rhs.repn);}
Constraint operator==(double lhs, const IndexParameter& rhs)
{return equal(lhs, rhs.repn);}
Constraint operator==(double lhs, const Variable& rhs)
{return equal(lhs, rhs.repn);}
Constraint operator==(double lhs, const Expression& rhs)
{return equal(lhs, rhs.repn);}

Constraint Parameter::operator==(double rhs) const
{return equal(repn, rhs);}
Constraint Parameter::operator==(int rhs) const
{return equal(repn, rhs);}
Constraint Parameter::operator==(const Parameter& rhs) const
{return equal(repn, rhs.repn);}
Constraint Parameter::operator==(const IndexParameter& rhs) const
{return equal(repn, rhs.repn);}
Constraint Parameter::operator==(const Variable& rhs) const
{return equal(repn, rhs.repn);}
Constraint Parameter::operator==(const Expression& rhs) const
{return equal(repn, rhs.repn);}

Constraint IndexParameter::operator==(double rhs) const
{return equal(repn, rhs);}
Constraint IndexParameter::operator==(int rhs) const
{return equal(repn, rhs);}
Constraint IndexParameter::operator==(const Parameter& rhs) const
{return equal(repn, rhs.repn);}
Constraint IndexParameter::operator==(const IndexParameter& rhs) const
{return equal(repn, rhs.repn);}
Constraint IndexParameter::operator==(const Variable& rhs) const
{return equal(repn, rhs.repn);}
Constraint IndexParameter::operator==(const Expression& rhs) const
{return equal(repn, rhs.repn);}

Constraint Variable::operator==(double rhs) const
{return equal(repn, rhs);}
Constraint Variable::operator==(int rhs) const
{return equal(repn, rhs);}
Constraint Variable::operator==(const Parameter& rhs) const
{return equal(repn, rhs.repn);}
Constraint Variable::operator==(const IndexParameter& rhs) const
{return equal(repn, rhs.repn);}
Constraint Variable::operator==(const Variable& rhs) const
{return equal(repn, rhs.repn);}
Constraint Variable::operator==(const Expression& rhs) const
{return equal(repn, rhs.repn);}

Constraint Expression::operator==(double rhs) const
{return equal(repn, rhs);}
Constraint Expression::operator==(int rhs) const
{return equal(repn, rhs);}
Constraint Expression::operator==(const Parameter& rhs) const
{return equal(repn, rhs.repn);}
Constraint Expression::operator==(const IndexParameter& rhs) const
{return equal(repn, rhs.repn);}
Constraint Expression::operator==(const Variable& rhs) const
{return equal(repn, rhs.repn);}
Constraint Expression::operator==(const Expression& rhs) const
{return equal(repn, rhs.repn);}

Constraint inequality(int lower, const Expression& body, int upper, bool strict)
{
return CREATE_POINTER(InequalityTerm, CREATE_POINTER(ConstantTerm, lower), body.repn, CREATE_POINTER(ConstantTerm, upper), strict);
}

Constraint inequality(double lower, const Expression& body, double upper, bool strict)
{
return CREATE_POINTER(InequalityTerm, CREATE_POINTER(ConstantTerm, lower), body.repn, CREATE_POINTER(ConstantTerm, upper), strict);
}

Constraint inequality(const Expression& lower, const Expression& body, const Expression& upper, bool strict)
{
return CREATE_POINTER(InequalityTerm, lower.repn, body.repn, upper.repn, strict);
}


//
// intrinsic functions
//
Expression abs(const Expression& body)
{
if (body.repn->is_constant()) {
    ConstantTerm* _body = dynamic_cast<ConstantTerm*>(body.repn);
    return ::fabs(_body->value);
    }
return intrinsic_abs(body.repn);
}

#define INTRINSIC_DEF1(FN)\
Expression FN(const Expression& body)\
{\
if (body.repn->is_constant()) {\
    ConstantTerm* _body = dynamic_cast<ConstantTerm*>(body.repn);\
    return ::FN(_body->value);\
    }\
return intrinsic_ ## FN(body.repn);\
}

INTRINSIC_DEF1(ceil);
INTRINSIC_DEF1(floor);
INTRINSIC_DEF1(exp);
INTRINSIC_DEF1(log);
INTRINSIC_DEF1(log10);
INTRINSIC_DEF1(sqrt);
INTRINSIC_DEF1(sin);
INTRINSIC_DEF1(cos);
INTRINSIC_DEF1(tan);
INTRINSIC_DEF1(sinh);
INTRINSIC_DEF1(cosh);
INTRINSIC_DEF1(tanh);
INTRINSIC_DEF1(asin);
INTRINSIC_DEF1(acos);
INTRINSIC_DEF1(atan);
INTRINSIC_DEF1(asinh);
INTRINSIC_DEF1(acosh);
INTRINSIC_DEF1(atanh);

#define INTRINSIC_DEF2(FN)\
Expression FN(const Expression& lhs, const Expression& rhs)\
{\
if (lhs.is_constant() and rhs.is_constant()) {\
    ConstantTerm* _lhs = dynamic_cast<ConstantTerm*>(lhs.repn);\
    ConstantTerm* _rhs = dynamic_cast<ConstantTerm*>(rhs.repn);\
    return ::FN(_lhs->value, _rhs->value);\
    }\
return intrinsic_ ## FN(lhs.repn, rhs.repn);\
}

INTRINSIC_DEF2(pow);


//
// TODO - Maybe we should have an expression term for affine expressions.  But for now, we'll
// just create a sum of monomials.
//
Expression affine_expression(std::vector<double>& coef, std::vector<Variable>& var, double offset)
{
Expression e = offset;
for (size_t i=0; i<coef.size(); i++) {
    e += coef[i]*var[i];
    }
return e;
}

Expression affine_expression(std::vector<Variable>& var, double offset)
{
Expression e = offset;
for (size_t i=0; i<var.size(); i++) {
    e += var[i];
    }
return e;
}


}
