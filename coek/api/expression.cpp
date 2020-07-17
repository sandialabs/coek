#include "../expr/ast_operators.hpp"
#include "../expr/ast_visitors.hpp"
#include "coek/api/expression.hpp"


namespace coek {

expr_pointer_t convert_expr_template(expr_pointer_t expr);


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

Variable::Variable(const Variable& expr)
{
repn = expr.repn;
OWN_POINTER(repn);
}

Variable::Variable(const std::string& name, double lb, double ub, double value, bool binary, bool integer)
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

Variable::~Variable()
{
if (repn)
    DISOWN_POINTER(repn);
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
{ return repn->value; }

void Variable::set_value(double value)
{ repn->value = value; }

double Variable::get_lb() const
{ return repn->lb; }

void Variable::set_lb(double value)
{ repn->lb = value; }

double Variable::get_ub() const
{ return repn->ub; }

void Variable::set_ub(double value)
{ repn->ub = value; }

unsigned int Variable::id() const
{ return repn->index; }

std::string Variable::get_name() const
{ return repn->get_name(); }

bool Variable::is_continuous() const
{ return not (repn->binary or repn->integer); }

bool Variable::is_binary() const
{ return repn->binary; }

bool Variable::is_integer() const
{ return repn->integer; }

std::ostream& operator<<(std::ostream& ostr, const Variable& arg)
{
write_expr(arg.repn, ostr);
return ostr;
}

void Variable::set_fixed(bool _flag)
{ repn->fixed = _flag; }

bool Variable::get_fixed() const
{ return repn->fixed; }

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
{ return repn->is_constant(); }

double Expression::get_value() const
{ return repn->eval(); }

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
{ return convert_expr_template(repn); }

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
expr_pointer_t _repn = repn;
repn = STATIC_CAST(BaseExpressionTerm, CREATE_POINTER(DivideTerm, _repn, arg.repn) );
DISOWN_POINTER(_repn);
OWN_POINTER(repn);
return *this;
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
{
if (lhs == 0)
    return ZEROCONST;
if (lhs == 1)
    return rhs.repn;
return times(lhs, rhs.repn);
}
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
{
if (lhs == 0.0)
    return ZEROCONST;
if (lhs == 1.0)
    return rhs.repn;
return times(lhs, rhs.repn);
}

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
{
if (arg == 0)
    return ZEROCONST;
if (arg == 1)
    return repn;
return times(repn, arg);
}
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
{
if (arg == 0.0)
    return ZEROCONST;
if (arg == 1.0)
    return repn;
return times(repn, arg);
}


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
// TODO - Maybe we should have an expression term for affine expressions.  But for now, we'll
// just create a sum of monomials.
//
Expression affine_expression(const std::vector<double>& coef, const std::vector<Variable>& var, double offset)
{
Expression e(offset);
auto cit = coef.begin();
auto vit = var.begin();
for (; vit != var.end(); ++vit, ++cit)
    e += (*cit)*(*vit);
return e;
}

Expression affine_expression(const std::vector<Variable>& var, double offset)
{
Expression e(offset);
for (auto it=var.begin(); it != var.end(); ++it)
    e += *it;
return e;
}


}
