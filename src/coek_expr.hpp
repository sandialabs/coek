#pragma once

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <set>


#define COEK_INFINITY   1e100
#define COEK_UNDEFINED  1e101
#define COEK_NAN        coek::nan_value

#define COEK_API_OPERATORS\
    Expression operator+(int arg) const;\
    Expression operator+(double arg) const;\
    Expression operator+(const Parameter& arg) const;\
    Expression operator+(const Variable& arg) const;\
    Expression operator+(const Expression& arg) const;\
\
    Expression operator-(int arg) const;\
    Expression operator-(double arg) const;\
    Expression operator-(const Parameter& arg) const;\
    Expression operator-(const Variable& arg) const;\
    Expression operator-(const Expression& arg) const;\
\
    Expression operator*(int arg) const;\
    Expression operator*(double arg) const;\
    Expression operator*(const Parameter& arg) const;\
    Expression operator*(const Variable& arg) const;\
    Expression operator*(const Expression& arg) const;\
\
    Expression operator/(int arg) const;\
    Expression operator/(double arg) const;\
    Expression operator/(const Parameter& arg) const;\
    Expression operator/(const Variable& arg) const;\
    Expression operator/(const Expression& arg) const;\
\
    Constraint operator<(int arg) const;\
    Constraint operator<(double arg) const;\
    Constraint operator<(const Parameter& arg) const;\
    Constraint operator<(const Variable& arg) const;\
    Constraint operator<(const Expression& arg) const;\
\
    Constraint operator<=(int arg) const;\
    Constraint operator<=(double arg) const;\
    Constraint operator<=(const Parameter& arg) const;\
    Constraint operator<=(const Variable& arg) const;\
    Constraint operator<=(const Expression& arg) const;\
\
    Constraint operator>(int arg) const;\
    Constraint operator>(double arg) const;\
    Constraint operator>(const Parameter& arg) const;\
    Constraint operator>(const Variable& arg) const;\
    Constraint operator>(const Expression& arg) const;\
\
    Constraint operator>=(int arg) const;\
    Constraint operator>=(double arg) const;\
    Constraint operator>=(const Parameter& arg) const;\
    Constraint operator>=(const Variable& arg) const;\
    Constraint operator>=(const Expression& arg) const;\
\
    Constraint operator==(int arg) const;\
    Constraint operator==(double arg) const;\
    Constraint operator==(const Parameter& arg) const;\
    Constraint operator==(const Variable& arg) const;\
    Constraint operator==(const Expression& arg) const;


// A general streaming utility that is not put in the coek namespace for now
std::ostream& operator<<(std::ostream& ostr, const std::list<std::string>& arg);


namespace coek {

extern double nan_value;


std::ostream& operator<<(std::ostream& ostr, const std::list<std::string>& arg);

class ParameterTerm;
class VariableTerm;
class BaseExpressionTerm;
class ConstraintTerm;

// SHARED_PTR
//typedef std::shared_ptr<ParameterTerm> ParameterRepn;
//typedef std::shared_ptr<VariableTerm> VariableRepn;
//typedef std::shared_ptr<BaseExpressionTerm> ExpressionRepn;
//typedef std::shared_ptr<ConstraintTerm> ConstraintRepn;
typedef ParameterTerm* ParameterRepn;
typedef VariableTerm* VariableRepn;
typedef BaseExpressionTerm* ExpressionRepn;
typedef ConstraintTerm* ConstraintRepn;

class Parameter;
class Variable;
class Expression;
class Constraint;

//
// numerical operators
//

Expression operator+(const Parameter&);
Expression operator+(const Variable&);
Expression operator+(const Expression&);

Expression operator-(const Parameter&);
Expression operator-(const Variable&);
Expression operator-(const Expression&);

Expression operator+(int, const Parameter&);
Expression operator+(int, const Variable&);
Expression operator+(int, const Expression&);
Expression operator+(double, const Parameter&);
Expression operator+(double, const Variable&);
Expression operator+(double, const Expression&);

Expression operator-(int, const Parameter&);
Expression operator-(int, const Variable&);
Expression operator-(int, const Expression&);
Expression operator-(double, const Parameter&);
Expression operator-(double, const Variable&);
Expression operator-(double, const Expression&);

Expression operator*(int, const Parameter&);
Expression operator*(int, const Variable&);
Expression operator*(int, const Expression&);
Expression operator*(double, const Parameter&);
Expression operator*(double, const Variable&);
Expression operator*(double, const Expression&);

Expression operator/(int, const Parameter&);
Expression operator/(int, const Variable&);
Expression operator/(int, const Expression&);
Expression operator/(double, const Parameter&);
Expression operator/(double, const Variable&);
Expression operator/(double, const Expression&);

Expression affine_expression(std::vector<double>& coef, std::vector<Variable>& var, double offset);
Expression affine_expression(std::vector<Variable>& var, double offset);


// Mutable Parameter 
class Parameter
{
public:

    ParameterRepn repn;

public:

    Parameter();
    Parameter(double value);
    Parameter(double value, const std::string& name);
    Parameter(const Parameter& arg);
    ~Parameter();

    Parameter& operator=(const Parameter& arg);

    double get_value() const;
    void set_value(double value);

    bool is_constant() const
        {return false;}

    std::string get_name() const;

    COEK_API_OPERATORS

    friend std::ostream& operator<<(std::ostream& ostr, const Parameter& arg);
};


// Decision Variables
class Variable
{
public:

    VariableRepn repn;

public:

    Variable(const VariableRepn& _repn);
    Variable(double lb=-COEK_INFINITY, double ub=COEK_INFINITY, double value=0.0);
    Variable(double lb, double ub, double value, const std::string& name);
    Variable(double lb, double ub, double value, bool binary, bool integer, const std::string& name);
    Variable(double lb, double ub, double value, bool binary, bool integer);
    Variable(const Variable& arg);
    ~Variable();

    Variable& operator=(const Variable& arg);

    void initialize(double lb, double ub, double value, bool binary, bool integer, bool fixed, const std::string& name);
    void initialize(double lb, double ub, double value, bool binary, bool integer, bool fixed);

    bool is_constant() const
        {return false;}

    double get_initial() const;
    void set_initial(double value);

    double get_value() const;
    void set_value(double value);

    double get_lb() const;
    void set_lb(double value);

    double get_ub() const;
    void set_ub(double value);

    unsigned int get_index() const;
    std::string get_name() const;

    void set_fixed(bool flag);
    bool get_fixed() const;

    bool is_continuous() const;
    bool is_binary() const;
    bool is_integer() const;

    COEK_API_OPERATORS

    friend std::ostream& operator<<(std::ostream& ostr, const Variable& arg);
};


// Coek AST Expression
class Expression
{
public:

    ExpressionRepn repn;

public:

    Expression();
    Expression(const ExpressionRepn& _repn);
    Expression(double value);
    Expression(int value);
    Expression(const Parameter& arg);
    Expression(const Variable& arg);
    Expression(const Expression& arg);
    ~Expression();

    Expression& operator=(const Expression& arg);

    bool is_constant() const;

    double get_value() const;

    std::list<std::string> to_list() const;
    Expression diff(const Variable& var) const;

    Expression& operator+=(int arg);
    Expression& operator+=(double arg);
    Expression& operator+=(const Parameter& arg);
    Expression& operator+=(const Variable& arg);
    Expression& operator+=(const Expression& arg);

    Expression& operator-=(int arg);
    Expression& operator-=(double arg);
    Expression& operator-=(const Parameter& arg);
    Expression& operator-=(const Variable& arg);
    Expression& operator-=(const Expression& arg);

    Expression& operator*=(int arg);
    Expression& operator*=(double arg);
    Expression& operator*=(const Parameter& arg);
    Expression& operator*=(const Variable& arg);
    Expression& operator*=(const Expression& arg);

    Expression& operator/=(int arg);
    Expression& operator/=(double arg);
    Expression& operator/=(const Parameter& arg);
    Expression& operator/=(const Variable& arg);
    Expression& operator/=(const Expression& arg);

    COEK_API_OPERATORS

    friend std::ostream& operator<<(std::ostream& ostr, const Expression& arg);
};


// Coek Constraint
class Constraint
{
public:

    ConstraintRepn repn;

public:

    Constraint(const ConstraintRepn& _repn);
    Constraint(const Constraint& arg);
    ~Constraint();

    Constraint& operator=(const Constraint& arg);

    bool is_inequality() const;
    bool is_equality() const;
    bool is_feasible() const;

    double get_value() const;

    std::list<std::string> to_list() const;

    friend std::ostream& operator<<(std::ostream& ostr, const Constraint& arg);
};


class QuadraticExpr
{
public:

    double constval;

    std::vector<VariableTerm*> linear_vars;
    std::vector<double> linear_coefs;

    std::vector<VariableTerm*> quadratic_lvars;
    std::vector<VariableTerm*> quadratic_rvars;
    std::vector<double> quadratic_coefs;

    QuadraticExpr() : constval(0.0) {}

    void collect_terms(Expression& expr);
    void collect_terms(Constraint& expr);

    bool is_constant() const
        {return (linear_coefs.size() == 0) and (quadratic_coefs.size() == 0);}
    bool is_linear() const
        {return (linear_coefs.size() > 0) and (quadratic_coefs.size() == 0);}
    bool is_quadratic() const
        {return quadratic_coefs.size() > 0;}
};


class MutableNLPExpr
{
public:

    struct varterm_compare {
        bool operator() (const VariableTerm*, const VariableTerm*) const;
    };

public:

    Expression constval;

    std::vector<VariableTerm*> linear_vars;
    std::vector<Expression> linear_coefs;

    std::vector<VariableTerm*> quadratic_lvars;
    std::vector<VariableTerm*> quadratic_rvars;
    std::vector<Expression> quadratic_coefs;

    std::set<VariableTerm*,varterm_compare> nonlinear_vars;
    Expression nonlinear;

    //std::unordered_set<coek::VariableTerm*> fixed_vars;
    //std::unordered_set<coek::ParameterTerm*> params;

    bool mutable_values;

    MutableNLPExpr();
    ~MutableNLPExpr();

    void collect_terms(Expression& expr);
    void collect_terms(Constraint& expr);

    bool is_mutable()
        {return mutable_values;}
};


//
// operator<<
//
std::ostream& operator<<(std::ostream& ostr, const Parameter& arg);
std::ostream& operator<<(std::ostream& ostr, const Variable& arg);
std::ostream& operator<<(std::ostream& ostr, const Expression& arg);
std::ostream& operator<<(std::ostream& ostr, const Constraint& arg);
std::ostream& operator<<(std::ostream& ostr, const QuadraticExpr& arg);

//
// logical operators
//

Constraint operator<(int, const Parameter&);
Constraint operator<(int, const Variable&);
Constraint operator<(int, const Expression&);
Constraint operator<(double, const Parameter&);
Constraint operator<(double, const Variable&);
Constraint operator<(double, const Expression&);

Constraint operator<=(int, const Parameter&);
Constraint operator<=(int, const Variable&);
Constraint operator<=(int, const Expression&);
Constraint operator<=(double, const Parameter&);
Constraint operator<=(double, const Variable&);
Constraint operator<=(double, const Expression&);

Constraint operator>(int, const Parameter&);
Constraint operator>(int, const Variable&);
Constraint operator>(int, const Expression&);
Constraint operator>(double, const Parameter&);
Constraint operator>(double, const Variable&);
Constraint operator>(double, const Expression&);

Constraint operator>=(int, const Parameter&);
Constraint operator>=(int, const Variable&);
Constraint operator>=(int, const Expression&);
Constraint operator>=(double, const Parameter&);
Constraint operator>=(double, const Variable&);
Constraint operator>=(double, const Expression&);

Constraint operator==(int, const Parameter&);
Constraint operator==(int, const Variable&);
Constraint operator==(int, const Expression&);
Constraint operator==(double, const Parameter&);
Constraint operator==(double, const Variable&);
Constraint operator==(double, const Expression&);

//
// intrinsic functions
//
#define INTRINSIC_DECL1(FN)\
Expression FN(const Expression& body);

INTRINSIC_DECL1(abs);
INTRINSIC_DECL1(ceil);
INTRINSIC_DECL1(floor);
INTRINSIC_DECL1(exp);
INTRINSIC_DECL1(log);
INTRINSIC_DECL1(log10);
INTRINSIC_DECL1(sqrt);
INTRINSIC_DECL1(sin);
INTRINSIC_DECL1(cos);
INTRINSIC_DECL1(tan);
INTRINSIC_DECL1(sinh);
INTRINSIC_DECL1(cosh);
INTRINSIC_DECL1(tanh);
INTRINSIC_DECL1(asin);
INTRINSIC_DECL1(acos);
INTRINSIC_DECL1(atan);
INTRINSIC_DECL1(asinh);
INTRINSIC_DECL1(acosh);
INTRINSIC_DECL1(atanh);

#define INTRINSIC_DECL2(FN)\
Expression FN(const Expression&, const Expression&);

INTRINSIC_DECL2(pow);

}
