#include "../ast/base_terms.hpp"
#include "../ast/constraint_terms.hpp"
#include "../ast/expr_terms.hpp"
#include "../ast/value_terms.hpp"
#include "../ast/visitor_fns.hpp"
#include "../ast/ast_operators.hpp"
#include "coek/api/expression.hpp"

namespace coek {

#ifdef COEK_WITH_COMPACT_MODEL
expr_pointer_t convert_expr_template(expr_pointer_t expr);
#endif

//
// Parameter
//

Parameter::Parameter() { repn = CREATE_POINTER(ParameterTerm); }

Parameter::Parameter(const std::string& name)
{
    repn = CREATE_POINTER(ParameterTerm);
    repn->name = name;
}

Parameter::Parameter(const ParameterRepn& _repn) : repn(_repn) {}

Parameter& Parameter::value(double value)
{
    repn->set_value(value);
    return *this;
}

Parameter& Parameter::value(const Expression& value)
{
    repn->set_value(value.repn);
    return *this;
}

double Parameter::value() const { return repn->eval(); }

Expression Parameter::value_expression() const { return repn->value; }

std::string Parameter::name() const { return repn->get_name(); }

Parameter& Parameter::name(const std::string& name)
{
    repn->name = name;
    return *this;
}

std::ostream& operator<<(std::ostream& ostr, const Parameter& arg)
{
    write_expr(arg.repn, ostr);
    return ostr;
}

Parameter parameter() { return Parameter(); }

Parameter parameter(const std::string& name) { return Parameter(name); }

//
// IndexParameter
//

// TODO - Does it make sense to allow an empty IndexParameter?
IndexParameter::IndexParameter() { repn = CREATE_POINTER(IndexParameterTerm, "unknown"); }

IndexParameter::IndexParameter(const std::string& name)
{
    repn = CREATE_POINTER(IndexParameterTerm, name);
}

IndexParameter::IndexParameter(const IndexParameter& expr) : repn(expr.repn) {}

IndexParameter& IndexParameter::operator=(const IndexParameter& expr)
{
    repn = expr.repn;
    return *this;
}

IndexParameter& IndexParameter::value(double value)
{
    repn->set_value(value);
    return *this;
}

IndexParameter& IndexParameter::value(int value)
{
    repn->set_value(value);
    return *this;
}

IndexParameter& IndexParameter::value(const std::string& value)
{
    repn->set_value(value);
    return *this;
}

bool IndexParameter::get_value(double& value) const { return repn->get_value(value); }

bool IndexParameter::get_value(int& value) const { return repn->get_value(value); }

bool IndexParameter::get_value(std::string& value) const { return repn->get_value(value); }

std::string IndexParameter::name() const { return repn->name; }

IndexParameter& IndexParameter::name(const std::string& name)
{
    repn->name = name;
    return *this;
}

std::ostream& operator<<(std::ostream& ostr, const IndexParameter& arg)
{
    write_expr(arg.repn, ostr);
    return ostr;
}

IndexParameter set_element() { return IndexParameter(); }

IndexParameter set_element(const std::string& name) { return IndexParameter(name); }

//
// Variable
//

Variable::Variable(const VariableRepn& _repn) : repn(_repn) {}

Variable::Variable()
{
    repn = CREATE_POINTER(VariableTerm, VariableTerm::negative_infinity,
                          VariableTerm::positive_infinity, VariableTerm::nan, false, false);
}

Variable::Variable(const std::string& name)
{
    repn = CREATE_POINTER(VariableTerm, VariableTerm::negative_infinity,
                          VariableTerm::positive_infinity, VariableTerm::nan, false, false);
    repn->name = name;
}

Variable& Variable::value(double value)
{
    repn->set_value(value);
    return *this;
}

Variable& Variable::value(const Expression& value)
{
    repn->set_value(value.repn);
    return *this;
}

double Variable::value() const { return repn->value->eval(); }

Expression Variable::value_expression() const { return repn->value; }

Variable& Variable::lower(double value)
{
    repn->set_lb(value);
    return *this;
}

Variable& Variable::lower(const Expression& value)
{
    repn->set_lb(value.repn);
    return *this;
}

double Variable::lower() const { return repn->lb->eval(); }

Expression Variable::lower_expression() const { return repn->lb; }

Variable& Variable::upper(double value)
{
    repn->set_ub(value);
    return *this;
}

Variable& Variable::upper(const Expression& value)
{
    repn->set_ub(value.repn);
    return *this;
}

double Variable::upper() const { return repn->ub->eval(); }

Expression Variable::upper_expression() const { return repn->ub; }

Variable& Variable::bounds(double lb, double ub)
{
    repn->set_lb(lb);
    repn->set_ub(ub);
    return *this;
}

Variable& Variable::bounds(const Expression& lb, double ub)
{
    repn->set_lb(lb.repn);
    repn->set_ub(ub);
    return *this;
}

Variable& Variable::bounds(double lb, const Expression& ub)
{
    repn->set_lb(lb);
    repn->set_ub(ub.repn);
    return *this;
}

Variable& Variable::bounds(const Expression& lb, const Expression& ub)
{
    repn->set_lb(lb.repn);
    repn->set_ub(ub.repn);
    return *this;
}

Variable& Variable::fix(double value)
{
    repn->set_value(value);
    repn->fixed = true;
    return *this;
}

Variable& Variable::fixed(bool _flag)
{
    repn->fixed = _flag;
    return *this;
}

bool Variable::fixed() const { return repn->fixed; }

Variable& Variable::name(const std::string& name)
{
    repn->name = name;
    return *this;
}

std::string Variable::name() const { return repn->get_name(); }

unsigned int Variable::id() const { return repn->index; }

Variable& Variable::within(VariableTypes vtype)
{
    if (vtype == Reals) {
        repn->binary = false;
        repn->integer = false;
    }
    else if (vtype == Integers) {
        repn->binary = false;
        repn->integer = true;
    }
    else {
        repn->binary = true;
        repn->integer = false;
    }
    return *this;
}

VariableTypes Variable::within() const
{
    if (repn->binary)
        return Binary;
    else if (repn->integer)
        return Integers;
    else
        return Reals;
}

bool Variable::is_continuous() const { return not(repn->binary or repn->integer); }

bool Variable::is_binary() const { return repn->binary; }

bool Variable::is_integer() const { return repn->integer; }

std::ostream& operator<<(std::ostream& ostr, const Variable& arg)
{
    write_expr(arg.repn, ostr);
    return ostr;
}

Variable variable()
{
    Variable tmp;
    return tmp;
}

Variable variable(const std::string& name)
{
    Variable tmp(name);
    return tmp;
}

//
// Expression
//

Expression::Expression() : repn(ZEROCONST) {}

Expression::Expression(const ExpressionRepn& _repn) : repn(_repn) {}

Expression::Expression(ExpressionRepn&& _repn) : repn(_repn) {}

Expression::Expression(double value) { repn = CREATE_POINTER(ConstantTerm, value); }

Expression::Expression(int value) { repn = CREATE_POINTER(ConstantTerm, value); }

Expression::Expression(const Parameter& param) : repn(param.repn) {}

Expression::Expression(const IndexParameter& param) : repn(param.repn) {}

Expression::Expression(const Variable& var) : repn(var.repn) {}


bool Expression::is_constant() const { return repn->is_constant(); }

double Expression::value() const { return repn->eval(); }

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
    if (ans.find(var.repn.get()) != ans.end()) e = ans[var.repn.get()];
    return e;
}

Expression Expression::expand()
{
#ifdef COEK_WITH_COMPACT_MODEL
    return convert_expr_template(repn);
#else
    return *this;
#endif
}

std::ostream& operator<<(std::ostream& ostr, const Expression& arg)
{
    write_expr(arg.repn, ostr);
    return ostr;
}

Expression& Expression::operator+=(int arg)
{
    Expression e(arg);
    *this += e;
    return *this;
}

Expression& Expression::operator+=(double arg)
{
    Expression e(arg);
    *this += e;
    return *this;
}

Expression& Expression::operator+=(const Parameter& arg)
{
    Expression e(arg);
    *this += e;
    return *this;
}

Expression& Expression::operator+=(const IndexParameter& arg)
{
    Expression e(arg);
    *this += e;
    return *this;
}

Expression& Expression::operator+=(const Variable& arg)
{
    Expression e(arg);
    *this += e;
    return *this;
}

Expression& Expression::operator+=(const Expression& arg)
{
    // expr_pointer_t _repn = repn;
    repn = CREATE_POINTER(PlusTerm, repn, arg.repn);
    return *this;
}

Expression& Expression::operator-=(int arg)
{
    Expression e(arg);
    *this -= e;
    return *this;
}

Expression& Expression::operator-=(double arg)
{
    Expression e(arg);
    *this -= e;
    return *this;
}

Expression& Expression::operator-=(const Parameter& arg)
{
    Expression e(arg);
    *this -= e;
    return *this;
}

Expression& Expression::operator-=(const IndexParameter& arg)
{
    Expression e(arg);
    *this -= e;
    return *this;
}

Expression& Expression::operator-=(const Variable& arg)
{
    Expression e(arg);
    *this -= e;
    return *this;
}

Expression& Expression::operator-=(const Expression& arg)
{
    expr_pointer_t tmp = CREATE_POINTER(NegateTerm, arg.repn);
    // expr_pointer_t _repn = repn;
    repn = CREATE_POINTER(PlusTerm, repn, tmp);
    return *this;
}

Expression& Expression::operator*=(int arg)
{
    Expression e(arg);
    *this *= e;
    return *this;
}

Expression& Expression::operator*=(double arg)
{
    Expression e(arg);
    *this *= e;
    return *this;
}

Expression& Expression::operator*=(const Parameter& arg)
{
    Expression e(arg);
    *this *= e;
    return *this;
}

Expression& Expression::operator*=(const IndexParameter& arg)
{
    Expression e(arg);
    *this *= e;
    return *this;
}

Expression& Expression::operator*=(const Variable& arg)
{
    Expression e(arg);
    *this *= e;
    return *this;
}

Expression& Expression::operator*=(const Expression& arg)
{
    // expr_pointer_t _repn = repn;
    repn = times(repn, arg.repn);
    return *this;
}

Expression& Expression::operator/=(int arg)
{
    Expression e(arg);
    *this /= e;
    return *this;
}

Expression& Expression::operator/=(double arg)
{
    Expression e(arg);
    *this /= e;
    return *this;
}

Expression& Expression::operator/=(const Parameter& arg)
{
    Expression e(arg);
    *this /= e;
    return *this;
}

Expression& Expression::operator/=(const IndexParameter& arg)
{
    Expression e(arg);
    *this /= e;
    return *this;
}

Expression& Expression::operator/=(const Variable& arg)
{
    Expression e(arg);
    *this /= e;
    return *this;
}

Expression& Expression::operator/=(const Expression& arg)
{
    // expr_pointer_t _repn = repn;
    repn = CREATE_POINTER(DivideTerm, repn, arg.repn);
    return *this;
}

Expression expression() { return coek::Expression(); }

Expression expression(double arg) { return coek::Expression(arg); }

Expression expression(int arg) { return coek::Expression(arg); }

Expression expression(const Parameter& arg) { return coek::Expression(arg); }

Expression expression(const Variable& arg) { return coek::Expression(arg); }

//
// -------------------------------------------------------------------------------------
// OPERATORS
// -------------------------------------------------------------------------------------

//
// operator+
//
Expression operator+(const Parameter& param)
{
    // return std::dynamic_pointer_cast<BaseExpressionTerm>(param.repn);
    expr_pointer_t tmp = param.repn;
    return tmp;
}
Expression operator+(const IndexParameter& param)
{
    // return std::dynamic_pointer_cast<BaseExpressionTerm>(param.repn);
    expr_pointer_t tmp = param.repn;
    return tmp;
}
Expression operator+(const Variable& var)
{
    // return std::dynamic_pointer_cast<BaseExpressionTerm>(var.repn);
    expr_pointer_t tmp = var.repn;
    return tmp;
}
Expression operator+(const Expression& expr) { return expr; }

Expression operator+(int lhs, const Parameter& rhs) { return plus(lhs, rhs.repn); }
Expression operator+(int lhs, const IndexParameter& rhs) { return plus(lhs, rhs.repn); }
Expression operator+(int lhs, const Variable& rhs) { return plus(lhs, rhs.repn); }
Expression operator+(int lhs, const Expression& rhs) { return plus(lhs, rhs.repn); }
Expression operator+(double lhs, const Parameter& rhs) { return plus(lhs, rhs.repn); }
Expression operator+(double lhs, const IndexParameter& rhs) { return plus(lhs, rhs.repn); }
Expression operator+(double lhs, const Variable& rhs) { return plus(lhs, rhs.repn); }
Expression operator+(double lhs, const Expression& rhs) { return plus(lhs, rhs.repn); }

Expression Parameter::operator+(int arg) const { return plus(repn, arg); }
Expression IndexParameter::operator+(int arg) const { return plus(repn, arg); }
Expression Variable::operator+(int arg) const { return plus(repn, arg); }
Expression Expression::operator+(int arg) const { return plus(repn, arg); }
Expression Parameter::operator+(double arg) const { return plus(repn, arg); }
Expression IndexParameter::operator+(double arg) const { return plus(repn, arg); }
Expression Variable::operator+(double arg) const { return plus(repn, arg); }
Expression Expression::operator+(double arg) const { return plus(repn, arg); }

Expression Parameter::operator+(const Parameter& arg) const
{
    // return std::dynamic_pointer_cast<BaseExpressionTerm>(CREATE_POINTER(PlusTerm, repn,
    // arg.repn));
    expr_pointer_t tmp = CREATE_POINTER(PlusTerm, repn, arg.repn);
    return tmp;
}
Expression Parameter::operator+(const IndexParameter& arg) const
{
    // return std::dynamic_pointer_cast<BaseExpressionTerm>(CREATE_POINTER(PlusTerm, repn,
    // arg.repn));
    expr_pointer_t tmp = CREATE_POINTER(PlusTerm, repn, arg.repn);
    return tmp;
}
Expression Parameter::operator+(const Variable& arg) const
{
    // return std::dynamic_pointer_cast<BaseExpressionTerm>(CREATE_POINTER(PlusTerm, repn,
    // arg.repn));
    expr_pointer_t tmp = CREATE_POINTER(PlusTerm, repn, arg.repn);
    return tmp;
}
Expression Parameter::operator+(const Expression& arg) const
{
    // return std::dynamic_pointer_cast<BaseExpressionTerm>(CREATE_POINTER(PlusTerm, repn,
    // arg.repn));
    expr_pointer_t tmp = CREATE_POINTER(PlusTerm, repn, arg.repn);
    return tmp;
}

Expression IndexParameter::operator+(const Parameter& arg) const
{
    // return std::dynamic_pointer_cast<BaseExpressionTerm>(CREATE_POINTER(PlusTerm, repn,
    // arg.repn));
    expr_pointer_t tmp = CREATE_POINTER(PlusTerm, repn, arg.repn);
    return tmp;
}
Expression IndexParameter::operator+(const IndexParameter& arg) const
{
    // return std::dynamic_pointer_cast<BaseExpressionTerm>(CREATE_POINTER(PlusTerm, repn,
    // arg.repn));
    expr_pointer_t tmp = CREATE_POINTER(PlusTerm, repn, arg.repn);
    return tmp;
}
Expression IndexParameter::operator+(const Variable& arg) const
{
    // return std::dynamic_pointer_cast<BaseExpressionTerm>(CREATE_POINTER(PlusTerm, repn,
    // arg.repn));
    expr_pointer_t tmp = CREATE_POINTER(PlusTerm, repn, arg.repn);
    return tmp;
}
Expression IndexParameter::operator+(const Expression& arg) const
{
    // return std::dynamic_pointer_cast<BaseExpressionTerm>(CREATE_POINTER(PlusTerm, repn,
    // arg.repn));
    expr_pointer_t tmp = CREATE_POINTER(PlusTerm, repn, arg.repn);
    return tmp;
}

Expression Variable::operator+(const Parameter& arg) const
{
    // return std::dynamic_pointer_cast<BaseExpressionTerm>(CREATE_POINTER(PlusTerm, repn,
    // arg.repn));
    expr_pointer_t tmp = CREATE_POINTER(PlusTerm, repn, arg.repn);
    return tmp;
}
Expression Variable::operator+(const IndexParameter& arg) const
{
    // return std::dynamic_pointer_cast<BaseExpressionTerm>(CREATE_POINTER(PlusTerm, repn,
    // arg.repn));
    expr_pointer_t tmp = CREATE_POINTER(PlusTerm, repn, arg.repn);
    return tmp;
}
Expression Variable::operator+(const Variable& arg) const
{
    // return std::dynamic_pointer_cast<BaseExpressionTerm>(CREATE_POINTER(PlusTerm, repn,
    // arg.repn));
    expr_pointer_t tmp = CREATE_POINTER(PlusTerm, repn, arg.repn);
    return tmp;
}
Expression Variable::operator+(const Expression& arg) const
{
    // return std::dynamic_pointer_cast<BaseExpressionTerm>(CREATE_POINTER(PlusTerm, repn,
    // arg.repn));
    expr_pointer_t tmp = CREATE_POINTER(PlusTerm, repn, arg.repn);
    return tmp;
}

Expression Expression::operator+(const Parameter& arg) const
{
    // if (repn == ZeroConstant.repn)
    //     return arg.repn;
    // return std::dynamic_pointer_cast<BaseExpressionTerm>(CREATE_POINTER(PlusTerm, repn,
    // arg.repn));
    expr_pointer_t tmp = CREATE_POINTER(PlusTerm, repn, arg.repn);
    return tmp;
}
Expression Expression::operator+(const IndexParameter& arg) const
{
    // if (repn == ZeroConstant.repn)
    //     return arg.repn;
    // return std::dynamic_pointer_cast<BaseExpressionTerm>(CREATE_POINTER(PlusTerm, repn,
    // arg.repn));
    expr_pointer_t tmp = CREATE_POINTER(PlusTerm, repn, arg.repn);
    return tmp;
}
Expression Expression::operator+(const Variable& arg) const
{
    // if (repn == ZeroConstant.repn)
    //     return arg.repn;
    // return std::dynamic_pointer_cast<BaseExpressionTerm>(CREATE_POINTER(PlusTerm, repn,
    // arg.repn));
    expr_pointer_t tmp = CREATE_POINTER(PlusTerm, repn, arg.repn);
    return tmp;
}
Expression Expression::operator+(const Expression& arg) const
{
    // if (repn == ZeroConstant.repn)
    //     return arg.repn;
    // if (arg.repn == ZeroConstant.repn)
    //     return repn;
    // return std::dynamic_pointer_cast<BaseExpressionTerm>(CREATE_POINTER(PlusTerm, repn,
    // arg.repn));
    expr_pointer_t tmp = CREATE_POINTER(PlusTerm, repn, arg.repn);
    return tmp;
}

//
// operator-
//
Expression operator-(const Parameter& param) { return param.repn->negate(param.repn); }
Expression operator-(const IndexParameter& param) { return param.repn->negate(param.repn); }
Expression operator-(const Variable& var) { return var.repn->negate(var.repn); }
Expression operator-(const Expression& expr) { return expr.repn->negate(expr.repn); }

Expression operator-(int lhs, const Parameter& rhs) { return minus(lhs, rhs.repn); }
Expression operator-(int lhs, const IndexParameter& rhs) { return minus(lhs, rhs.repn); }
Expression operator-(int lhs, const Variable& rhs) { return minus(lhs, rhs.repn); }
Expression operator-(int lhs, const Expression& rhs)
{
    // if (rhs.repn == ZeroConstant.repn)
    //     return repn;
    return minus(lhs, rhs.repn);
}
Expression operator-(double lhs, const Parameter& rhs) { return minus(lhs, rhs.repn); }
Expression operator-(double lhs, const IndexParameter& rhs) { return minus(lhs, rhs.repn); }
Expression operator-(double lhs, const Variable& rhs) { return minus(lhs, rhs.repn); }
Expression operator-(double lhs, const Expression& rhs)
{
    // if (rhs.repn == ZeroConstant.repn)
    //     return repn;
    return minus(lhs, rhs.repn);
}

Expression Parameter::operator-(int arg) const { return plus(repn, -arg); }
Expression IndexParameter::operator-(int arg) const { return plus(repn, -arg); }
Expression Variable::operator-(int arg) const { return plus(repn, -arg); }
Expression Expression::operator-(int arg) const { return plus(repn, -arg); }
Expression Parameter::operator-(double arg) const { return plus(repn, -arg); }
Expression IndexParameter::operator-(double arg) const { return plus(repn, -arg); }
Expression Variable::operator-(double arg) const { return plus(repn, -arg); }
Expression Expression::operator-(double arg) const { return plus(repn, -arg); }

Expression Parameter::operator-(const Parameter& arg) const
{
    return std::dynamic_pointer_cast<BaseExpressionTerm>(
        CREATE_POINTER(PlusTerm, repn, arg.repn->negate(arg.repn)));
}
Expression Parameter::operator-(const IndexParameter& arg) const
{
    return std::dynamic_pointer_cast<BaseExpressionTerm>(
        CREATE_POINTER(PlusTerm, repn, arg.repn->negate(arg.repn)));
}
Expression Parameter::operator-(const Variable& arg) const
{
    return std::dynamic_pointer_cast<BaseExpressionTerm>(
        CREATE_POINTER(PlusTerm, repn, arg.repn->negate(arg.repn)));
}
Expression Parameter::operator-(const Expression& arg) const
{
    // if (rhs.repn == ZeroConstant.repn)
    //     return repn;
    return std::dynamic_pointer_cast<BaseExpressionTerm>(
        CREATE_POINTER(PlusTerm, repn, arg.repn->negate(arg.repn)));
}

Expression IndexParameter::operator-(const Parameter& arg) const
{
    return std::dynamic_pointer_cast<BaseExpressionTerm>(
        CREATE_POINTER(PlusTerm, repn, arg.repn->negate(arg.repn)));
}
Expression IndexParameter::operator-(const IndexParameter& arg) const
{
    return std::dynamic_pointer_cast<BaseExpressionTerm>(
        CREATE_POINTER(PlusTerm, repn, arg.repn->negate(arg.repn)));
}
Expression IndexParameter::operator-(const Variable& arg) const
{
    return std::dynamic_pointer_cast<BaseExpressionTerm>(
        CREATE_POINTER(PlusTerm, repn, arg.repn->negate(arg.repn)));
}
Expression IndexParameter::operator-(const Expression& arg) const
{
    // if (rhs.repn == ZeroConstant.repn)
    //     return repn;
    return std::dynamic_pointer_cast<BaseExpressionTerm>(
        CREATE_POINTER(PlusTerm, repn, arg.repn->negate(arg.repn)));
}

Expression Variable::operator-(const Parameter& arg) const
{
    return std::dynamic_pointer_cast<BaseExpressionTerm>(
        CREATE_POINTER(PlusTerm, repn, arg.repn->negate(arg.repn)));
}
Expression Variable::operator-(const IndexParameter& arg) const
{
    return std::dynamic_pointer_cast<BaseExpressionTerm>(
        CREATE_POINTER(PlusTerm, repn, arg.repn->negate(arg.repn)));
}
Expression Variable::operator-(const Variable& arg) const
{
    return std::dynamic_pointer_cast<BaseExpressionTerm>(
        CREATE_POINTER(PlusTerm, repn, arg.repn->negate(arg.repn)));
}
Expression Variable::operator-(const Expression& arg) const
{
    return std::dynamic_pointer_cast<BaseExpressionTerm>(
        CREATE_POINTER(PlusTerm, repn, arg.repn->negate(arg.repn)));
}

Expression Expression::operator-(const Parameter& arg) const
{
    return std::dynamic_pointer_cast<BaseExpressionTerm>(
        CREATE_POINTER(PlusTerm, repn, arg.repn->negate(arg.repn)));
}
Expression Expression::operator-(const IndexParameter& arg) const
{
    return std::dynamic_pointer_cast<BaseExpressionTerm>(
        CREATE_POINTER(PlusTerm, repn, arg.repn->negate(arg.repn)));
}
Expression Expression::operator-(const Variable& arg) const
{
    return std::dynamic_pointer_cast<BaseExpressionTerm>(
        CREATE_POINTER(PlusTerm, repn, arg.repn->negate(arg.repn)));
}
Expression Expression::operator-(const Expression& arg) const
{
    // if (rhs.repn == ZeroConstant.repn)
    //     return repn;
    // if (arg.repn == ZeroConstant.repn)
    //     return repn;

    return std::dynamic_pointer_cast<BaseExpressionTerm>(
        CREATE_POINTER(PlusTerm, repn, arg.repn->negate(arg.repn)));
}

//
// operator*
//
Expression operator*(int lhs, const Parameter& rhs) { return times(lhs, rhs.repn); }
Expression operator*(int lhs, const IndexParameter& rhs) { return times(lhs, rhs.repn); }
Expression operator*(int lhs, const Variable& rhs)
{
    if (lhs == 0) return std::dynamic_pointer_cast<BaseExpressionTerm>(ZEROCONST);
    if (lhs == 1) return std::dynamic_pointer_cast<BaseExpressionTerm>(rhs.repn);
    return std::dynamic_pointer_cast<BaseExpressionTerm>(rhs.repn->const_mult(lhs, rhs.repn));
}
Expression operator*(int lhs, const Expression& rhs)
{
    if (lhs == 0) return std::dynamic_pointer_cast<BaseExpressionTerm>(ZEROCONST);
    if (lhs == 1) return rhs.repn;
    return times(lhs, rhs.repn);
}
Expression operator*(double lhs, const Parameter& rhs) { return times(lhs, rhs.repn); }
Expression operator*(double lhs, const IndexParameter& rhs) { return times(lhs, rhs.repn); }
Expression operator*(double lhs, const Variable& rhs)
{
    if (lhs == 0.0) return std::dynamic_pointer_cast<BaseExpressionTerm>(ZEROCONST);
    if (lhs == 1.0) return std::dynamic_pointer_cast<BaseExpressionTerm>(rhs.repn);
    return std::dynamic_pointer_cast<BaseExpressionTerm>(rhs.repn->const_mult(lhs, rhs.repn));
}
Expression operator*(double lhs, const Expression& rhs)
{
    if (lhs == 0.0) return std::dynamic_pointer_cast<BaseExpressionTerm>(ZEROCONST);
    if (lhs == 1.0) return rhs.repn;
    return times(lhs, rhs.repn);
}

Expression Parameter::operator*(int arg) const { return times(repn, arg); }
Expression IndexParameter::operator*(int arg) const { return times(repn, arg); }
Expression Variable::operator*(int arg) const
{
    if (arg == 0) return std::dynamic_pointer_cast<BaseExpressionTerm>(ZEROCONST);
    if (arg == 1) return std::dynamic_pointer_cast<BaseExpressionTerm>(repn);
    return std::dynamic_pointer_cast<BaseExpressionTerm>(repn->const_mult(arg, repn));
}
Expression Expression::operator*(int arg) const
{
    if (arg == 0) return std::dynamic_pointer_cast<BaseExpressionTerm>(ZEROCONST);
    if (arg == 1) return std::dynamic_pointer_cast<BaseExpressionTerm>(repn);
    return times(repn, arg);
}
Expression Parameter::operator*(double arg) const { return times(repn, arg); }
Expression IndexParameter::operator*(double arg) const { return times(repn, arg); }
Expression Variable::operator*(double arg) const
{
    if (arg == 0.0) return std::dynamic_pointer_cast<BaseExpressionTerm>(ZEROCONST);
    if (arg == 1.0) return std::dynamic_pointer_cast<BaseExpressionTerm>(repn);
    return std::dynamic_pointer_cast<BaseExpressionTerm>(repn->const_mult(arg, repn));
}
Expression Expression::operator*(double arg) const
{
    if (arg == 0.0) return std::dynamic_pointer_cast<BaseExpressionTerm>(ZEROCONST);
    if (arg == 1.0) return repn;
    return times(repn, arg);
}

Expression Parameter::operator*(const Parameter& arg) const { return times(repn, arg.repn); }
Expression Parameter::operator*(const IndexParameter& arg) const { return times(repn, arg.repn); }
Expression Parameter::operator*(const Variable& arg) const { return times(repn, arg.repn); }
Expression Parameter::operator*(const Expression& arg) const { return times(repn, arg.repn); }

Expression IndexParameter::operator*(const Parameter& arg) const { return times(repn, arg.repn); }
Expression IndexParameter::operator*(const IndexParameter& arg) const
{
    return times(repn, arg.repn);
}
Expression IndexParameter::operator*(const Variable& arg) const { return times(repn, arg.repn); }
Expression IndexParameter::operator*(const Expression& arg) const { return times(repn, arg.repn); }

Expression Variable::operator*(const Parameter& arg) const { return times(repn, arg.repn); }
Expression Variable::operator*(const IndexParameter& arg) const { return times(repn, arg.repn); }
Expression Variable::operator*(const Variable& arg) const { return times(repn, arg.repn); }
Expression Variable::operator*(const Expression& arg) const { return times(repn, arg.repn); }

Expression Expression::operator*(const Parameter& arg) const { return times(repn, arg.repn); }
Expression Expression::operator*(const IndexParameter& arg) const { return times(repn, arg.repn); }
Expression Expression::operator*(const Variable& arg) const { return times(repn, arg.repn); }
Expression Expression::operator*(const Expression& arg) const { return times(repn, arg.repn); }

//
// operator/
//
Expression operator/(int lhs, const Parameter& rhs) { return divide(lhs, rhs.repn); }
Expression operator/(int lhs, const IndexParameter& rhs) { return divide(lhs, rhs.repn); }
Expression operator/(int lhs, const Variable& rhs) { return divide(lhs, rhs.repn); }
Expression operator/(int lhs, const Expression& rhs) { return divide(lhs, rhs.repn); }
Expression operator/(double lhs, const Parameter& rhs) { return divide(lhs, rhs.repn); }
Expression operator/(double lhs, const IndexParameter& rhs) { return divide(lhs, rhs.repn); }
Expression operator/(double lhs, const Variable& rhs) { return divide(lhs, rhs.repn); }
Expression operator/(double lhs, const Expression& rhs) { return divide(lhs, rhs.repn); }

Expression Parameter::operator/(int arg) const { return divide(repn, arg); }
Expression IndexParameter::operator/(int arg) const { return divide(repn, arg); }
Expression Variable::operator/(int arg) const
{
    if (arg == 1) return std::dynamic_pointer_cast<BaseExpressionTerm>(repn);
    if (arg == 0) throw std::domain_error("Division by zero.");
    return std::dynamic_pointer_cast<BaseExpressionTerm>(repn->const_mult(1.0 / arg, repn));
}
Expression Expression::operator/(int arg) const { return divide(repn, arg); }
Expression Parameter::operator/(double arg) const { return divide(repn, arg); }
Expression IndexParameter::operator/(double arg) const { return divide(repn, arg); }
Expression Variable::operator/(double arg) const
{
    if (arg == 1.0) return std::dynamic_pointer_cast<BaseExpressionTerm>(repn);
    if (arg == 0.0) throw std::domain_error("Division by zero.");
    return std::dynamic_pointer_cast<BaseExpressionTerm>(repn->const_mult(1.0 / arg, repn));
}
Expression Expression::operator/(double arg) const { return divide(repn, arg); }

Expression Parameter::operator/(const Parameter& arg) const { return divide(repn, arg.repn); }
Expression Parameter::operator/(const IndexParameter& arg) const { return divide(repn, arg.repn); }
Expression Parameter::operator/(const Variable& arg) const { return divide(repn, arg.repn); }
Expression Parameter::operator/(const Expression& arg) const { return divide(repn, arg.repn); }

Expression IndexParameter::operator/(const Parameter& arg) const { return divide(repn, arg.repn); }
Expression IndexParameter::operator/(const IndexParameter& arg) const
{
    return divide(repn, arg.repn);
}
Expression IndexParameter::operator/(const Variable& arg) const { return divide(repn, arg.repn); }
Expression IndexParameter::operator/(const Expression& arg) const { return divide(repn, arg.repn); }

Expression Variable::operator/(const Parameter& arg) const { return divide(repn, arg.repn); }
Expression Variable::operator/(const IndexParameter& arg) const { return divide(repn, arg.repn); }
Expression Variable::operator/(const Variable& arg) const { return divide(repn, arg.repn); }
Expression Variable::operator/(const Expression& arg) const { return divide(repn, arg.repn); }

Expression Expression::operator/(const Parameter& arg) const { return divide(repn, arg.repn); }
Expression Expression::operator/(const IndexParameter& arg) const { return divide(repn, arg.repn); }
Expression Expression::operator/(const Variable& arg) const { return divide(repn, arg.repn); }
Expression Expression::operator/(const Expression& arg) const { return divide(repn, arg.repn); }

//
// TODO - Maybe we should have an expression term for affine expressions.  But for now, we'll
// just create a sum of monomials.
//
Expression affine_expression(const std::vector<double>& coef, const std::vector<Variable>& var,
                             double offset)
{
    Expression e(offset);
    auto cit = coef.begin();
    auto vit = var.begin();
    for (; vit != var.end(); ++vit, ++cit) e += (*cit) * (*vit);
    return e;
}

Expression affine_expression(const std::vector<Variable>& var, double offset)
{
    Expression e(offset);
    for (auto it = var.begin(); it != var.end(); ++it) e += *it;
    return e;
}

}  // namespace coek
