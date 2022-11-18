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

Expression::Expression(const ParameterRepn& _repn) : repn(_repn) {}
Expression::Expression(const IndexParameterRepn& _repn) : repn(_repn) {}
Expression::Expression(const VariableRepn& _repn) : repn(_repn) {}
Expression::Expression(const ExpressionRepn& _repn) : repn(_repn) {}

Expression::Expression(ExpressionRepn&& _repn) : repn(_repn) {}

Expression::Expression(ParameterRepn&& _repn) : repn(_repn) {}

Expression::Expression(IndexParameterRepn&& _repn) : repn(_repn) {}

Expression::Expression(VariableRepn&& _repn) : repn(_repn) {}

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
    std::map<std::shared_ptr<VariableTerm>, expr_pointer_t> ans;
    symbolic_diff_all(repn, ans);
    Expression e;
    if (ans.find(var.repn) != ans.end()) e = ans[var.repn];
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
    repn = CREATE_POINTER(PlusTerm, repn, CREATE_POINTER(NegateTerm, arg.repn));
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
Expression operator+(const Variable& var) { return var.repn; }
Expression operator+(const Expression& expr) { return expr; }

Expression operator+(int lhs, const Variable& rhs) { return plus(lhs, rhs.repn); }
Expression operator+(int lhs, const Expression& rhs) { return plus(lhs, rhs.repn); }
Expression operator+(double lhs, const Variable& rhs) { return plus(lhs, rhs.repn); }
Expression operator+(double lhs, const Expression& rhs) { return plus(lhs, rhs.repn); }

Expression operator+(const Variable& lhs, int arg) { return plus(lhs.repn, arg); }
Expression operator+(const Expression& lhs, int arg) { return plus(lhs.repn, arg); }
Expression operator+(const Variable& lhs, double arg) { return plus(lhs.repn, arg); }
Expression operator+(const Expression& lhs, double arg) { return plus(lhs.repn, arg); }

Expression operator+(const Variable& lhs, const Variable& rhs)
{
    return Expression(CREATE_POINTER(PlusTerm, lhs.repn, rhs.repn));
}
Expression operator+(const Variable& lhs, const Expression& rhs)
{
    return Expression(CREATE_POINTER(PlusTerm, lhs.repn, rhs.repn));
}
Expression operator+(const Expression& lhs, const Variable& rhs)
{
    return Expression(CREATE_POINTER(PlusTerm, lhs.repn, rhs.repn));
}
Expression operator+(const Expression& lhs, const Expression& rhs)
{
    return Expression(CREATE_POINTER(PlusTerm, lhs.repn, rhs.repn));
}

//
// operator-
//
Expression operator-(const Variable& var) { return var.repn->negate(var.repn); }
Expression operator-(const Expression& expr) { return expr.repn->negate(expr.repn); }

Expression operator-(int lhs, const Variable& rhs) { return minus(lhs, rhs.repn); }
Expression operator-(int lhs, const Expression& rhs) { return minus(lhs, rhs.repn); }
Expression operator-(double lhs, const Variable& rhs) { return minus(lhs, rhs.repn); }
Expression operator-(double lhs, const Expression& rhs) { return minus(lhs, rhs.repn); }

Expression operator-(const Variable& lhs, int arg) { return plus(lhs.repn, -arg); }
Expression operator-(const Expression& lhs, int arg) { return plus(lhs.repn, -arg); }
Expression operator-(const Variable& lhs, double arg) { return plus(lhs.repn, -arg); }
Expression operator-(const Expression& lhs, double arg) { return plus(lhs.repn, -arg); }

Expression operator-(const Variable& lhs, const Variable& rhs)
{
    return Expression(CREATE_POINTER(PlusTerm, lhs.repn, rhs.repn->negate(rhs.repn)));
}
Expression operator-(const Variable& lhs, const Expression& rhs)
{
    return Expression(CREATE_POINTER(PlusTerm, lhs.repn, rhs.repn->negate(rhs.repn)));
}
Expression operator-(const Expression& lhs, const Variable& rhs)
{
    return Expression(CREATE_POINTER(PlusTerm, lhs.repn, rhs.repn->negate(rhs.repn)));
}
Expression operator-(const Expression& lhs, const Expression& rhs)
{
    return Expression(CREATE_POINTER(PlusTerm, lhs.repn, rhs.repn->negate(rhs.repn)));
}

//
// operator*
//
Expression operator*(int lhs, const Variable& rhs)
{
    expr_pointer_t tmp;
    if (lhs == 0)
        tmp = ZEROCONST;
    else if (lhs == 1)
        tmp = rhs.repn;
    else
        tmp = rhs.repn->const_mult(lhs, rhs.repn);
    return tmp;
}
Expression operator*(int lhs, const Expression& rhs)
{
    expr_pointer_t tmp;
    if (lhs == 0)
        tmp = ZEROCONST;
    else if (lhs == 1)
        tmp = rhs.repn;
    else
        tmp = times(lhs, rhs.repn);
    return tmp;
}
Expression operator*(double lhs, const Variable& rhs)
{
    expr_pointer_t tmp;
    if (lhs == 0.0)
        tmp = ZEROCONST;
    else if (lhs == 1.0)
        tmp = rhs.repn;
    else
        tmp = rhs.repn->const_mult(lhs, rhs.repn);
    return tmp;
}
Expression operator*(double lhs, const Expression& rhs)
{
    expr_pointer_t tmp;
    if (lhs == 0.0)
        tmp = ZEROCONST;
    else if (lhs == 1.0)
        tmp = rhs.repn;
    else
        tmp = times(lhs, rhs.repn);
    return tmp;
}

Expression operator*(const Variable& lhs, int arg)
{
    expr_pointer_t tmp;
    if (arg == 0)
        tmp = ZEROCONST;
    else if (arg == 1)
        tmp = lhs.repn;
    else
        tmp = lhs.repn->const_mult(arg, lhs.repn);
    return tmp;
}
Expression operator*(const Expression& lhs, int arg)
{
    expr_pointer_t tmp;
    if (arg == 0)
        tmp = ZEROCONST;
    else if (arg == 1)
        tmp = lhs.repn;
    else
        tmp = times(lhs.repn, arg);
    return tmp;
}
Expression operator*(const Variable& lhs, double arg)
{
    expr_pointer_t tmp;
    if (arg == 0.0)
        tmp = ZEROCONST;
    else if (arg == 1.0)
        tmp = lhs.repn;
    else
        tmp = lhs.repn->const_mult(arg, lhs.repn);
    return tmp;
}
Expression operator*(const Expression& lhs, double arg)
{
    expr_pointer_t tmp;
    if (arg == 0.0)
        tmp = ZEROCONST;
    else if (arg == 1.0)
        tmp = lhs.repn;
    else
        tmp = times(lhs.repn, arg);
    return tmp;
}

Expression operator*(const Variable& lhs, const Variable& rhs) { return times(lhs.repn, rhs.repn); }
Expression operator*(const Variable& lhs, const Expression& rhs)
{
    return times(lhs.repn, rhs.repn);
}
Expression operator*(const Expression& lhs, const Variable& rhs)
{
    return times(lhs.repn, rhs.repn);
}
Expression operator*(const Expression& lhs, const Expression& rhs)
{
    return times(lhs.repn, rhs.repn);
}

//
// operator/
//
Expression operator/(int lhs, const Variable& rhs) { return divide(lhs, rhs.repn); }
Expression operator/(int lhs, const Expression& rhs) { return divide(lhs, rhs.repn); }
Expression operator/(double lhs, const Variable& rhs) { return divide(lhs, rhs.repn); }
Expression operator/(double lhs, const Expression& rhs) { return divide(lhs, rhs.repn); }

Expression operator/(const Variable& lhs, int arg)
{
    expr_pointer_t tmp;
    if (arg == 1)
        tmp = lhs.repn;
    else if (arg == 0)
        throw std::domain_error("Division by zero.");
    else
        tmp = lhs.repn->const_mult(1.0 / arg, lhs.repn);
    return tmp;
}
Expression operator/(const Expression& lhs, int arg) { return divide(lhs.repn, arg); }
Expression operator/(const Variable& lhs, double arg)
{
    expr_pointer_t tmp;
    if (arg == 1.0)
        tmp = lhs.repn;
    else if (arg == 0.0)
        throw std::domain_error("Division by zero.");
    else
        tmp = lhs.repn->const_mult(1.0 / arg, lhs.repn);
    return tmp;
}
Expression operator/(const Expression& lhs, double arg) { return divide(lhs.repn, arg); }

Expression operator/(const Variable& lhs, const Variable& rhs)
{
    return divide(lhs.repn, rhs.repn);
}
Expression operator/(const Variable& lhs, const Expression& rhs)
{
    return divide(lhs.repn, rhs.repn);
}
Expression operator/(const Expression& lhs, const Variable& rhs)
{
    return divide(lhs.repn, rhs.repn);
}
Expression operator/(const Expression& lhs, const Expression& rhs)
{
    return divide(lhs.repn, rhs.repn);
}

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
    for (auto& v : var) e += v;
    return e;
}

}  // namespace coek
