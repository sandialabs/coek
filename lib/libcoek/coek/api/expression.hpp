#pragma once

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <set>
#include <memory>
#include <initializer_list>

#include <coek/api/constants.hpp>

#define COEK_API_OPERATORS\
    /** \returns a new expression: (*this) + arg */\
    Expression operator+(int arg) const;\
    /** \returns a new expression: (*this) + arg */\
    Expression operator+(double arg) const;\
    /** \returns a new expression: (*this) + arg */\
    Expression operator+(const Parameter& arg) const;\
    /** \returns a new expression: (*this) + arg */\
    Expression operator+(const IndexParameter& arg) const;\
    /** \returns a new expression: (*this) + arg */\
    Expression operator+(const Variable& arg) const;\
    /** \returns a new expression: (*this) + arg */\
    Expression operator+(const Expression& arg) const;\
\
    /** \returns a new expression: (*this) - arg */\
    Expression operator-(int arg) const;\
    /** \returns a new expression: (*this) - arg */\
    Expression operator-(double arg) const;\
    /** \returns a new expression: (*this) - arg */\
    Expression operator-(const Parameter& arg) const;\
    /** \returns a new expression: (*this) - arg */\
    Expression operator-(const IndexParameter& arg) const;\
    /** \returns a new expression: (*this) - arg */\
    Expression operator-(const Variable& arg) const;\
    /** \returns a new expression: (*this) - arg */\
    Expression operator-(const Expression& arg) const;\
\
    /** \returns a new expression: (*this) * arg */\
    Expression operator*(int arg) const;\
    /** \returns a new expression: (*this) * arg */\
    Expression operator*(double arg) const;\
    /** \returns a new expression: (*this) * arg */\
    Expression operator*(const Parameter& arg) const;\
    /** \returns a new expression: (*this) * arg */\
    Expression operator*(const IndexParameter& arg) const;\
    /** \returns a new expression: (*this) * arg */\
    Expression operator*(const Variable& arg) const;\
    /** \returns a new expression: (*this) * arg */\
    Expression operator*(const Expression& arg) const;\
\
    /** \returns a new expression: (*this) / arg */\
    Expression operator/(int arg) const;\
    /** \returns a new expression: (*this) / arg */\
    Expression operator/(double arg) const;\
    /** \returns a new expression: (*this) / arg */\
    Expression operator/(const Parameter& arg) const;\
    /** \returns a new expression: (*this) / arg */\
    Expression operator/(const IndexParameter& arg) const;\
    /** \returns a new expression: (*this) / arg */\
    Expression operator/(const Variable& arg) const;\
    /** \returns a new expression: (*this) / arg */\
    Expression operator/(const Expression& arg) const;


namespace coek {


class ParameterTerm;
class IndexParameterTerm;
class VariableTerm;
class BaseExpressionTerm;

typedef ParameterTerm* ParameterRepn;
typedef IndexParameterTerm* IndexParameterRepn;
typedef VariableTerm* VariableRepn;
typedef BaseExpressionTerm* ExpressionRepn;

class Parameter;
class IndexParameter;
class Variable;
class Expression;

//
// numerical operators
//

Expression operator+(const Parameter&);
Expression operator+(const IndexParameter&);
Expression operator+(const Variable&);
Expression operator+(const Expression&);

Expression operator-(const Parameter&);
Expression operator-(const IndexParameter&);
Expression operator-(const Variable&);
Expression operator-(const Expression&);

Expression operator+(int, const Parameter&);
Expression operator+(int, const IndexParameter&);
Expression operator+(int, const Variable&);
Expression operator+(int, const Expression&);
Expression operator+(double, const Parameter&);
Expression operator+(double, const IndexParameter&);
Expression operator+(double, const Variable&);
Expression operator+(double, const Expression&);

Expression operator-(int, const Parameter&);
Expression operator-(int, const IndexParameter&);
Expression operator-(int, const Variable&);
Expression operator-(int, const Expression&);
Expression operator-(double, const Parameter&);
Expression operator-(double, const IndexParameter&);
Expression operator-(double, const Variable&);
Expression operator-(double, const Expression&);

Expression operator*(int, const Parameter&);
Expression operator*(int, const IndexParameter&);
Expression operator*(int, const Variable&);
Expression operator*(int, const Expression&);
Expression operator*(double, const Parameter&);
Expression operator*(double, const IndexParameter&);
Expression operator*(double, const Variable&);
Expression operator*(double, const Expression&);

Expression operator/(int, const Parameter&);
Expression operator/(int, const IndexParameter&);
Expression operator/(int, const Variable&);
Expression operator/(int, const Expression&);
Expression operator/(double, const Parameter&);
Expression operator/(double, const IndexParameter&);
Expression operator/(double, const Variable&);
Expression operator/(double, const Expression&);

Expression affine_expression(const std::vector<double>& coef, const std::vector<Variable>& var, double offset);
Expression affine_expression(const std::vector<Variable>& var, double offset);


/**
  * \class Parameter
  * \brief Values that appear as mutable constants in expressions.
  *
  * Mutable parameters are used to define coefficients and
  * constant terms can be changed without reconstructing the expression.
  */
class Parameter
{
public:

    ParameterRepn repn;

public:

    /** Constructs a Parameter with the default value zero. */
    Parameter();
    /**
      * Constructs a Parameter initialized with a given value and a name.
      *  
      * \param value   the initial value of the parameter
      * \param name   the name of the parameter
      */
    explicit Parameter(const std::string& name);
    /**
      * Copy constructor
      *
      * \param arg   a parameter whose value is shared
      */
    Parameter(const Parameter& arg);
    Parameter(const ParameterRepn& _repn);
    ~Parameter();
    Parameter& operator=(const Parameter& arg);

    /** \returns the value of the parameter */
    double value() const;
    /** Set the parameter value. \returns the parameter object. */
    Parameter& value(const Expression& value);
    /** Set the value of the parameter */
    Parameter& value(double value);
    /** \returns the expression defining the parameter value */
    Expression value_expression() const;

    /** \returns the name of the parameter */
    std::string name() const;
    /** Set the name of the parameter */
    Parameter& name(const std::string& name);

    /** \returns \c false because this is not a contant */
    bool is_constant() const
        {return false;}

    COEK_API_OPERATORS

    /**
      * \name Stream operator
      *
      * Stream the parameter to an output stream.
      *
      * \param ostr   output stream
      * \param arg   parameter
      */
    friend std::ostream& operator<<(std::ostream& ostr, const Parameter& arg);
};


Parameter parameter();
Parameter parameter(const std::string& name);


// Index Parameter 
class IndexParameter
{
public:

    IndexParameterRepn repn;

public:

    IndexParameter();
    explicit IndexParameter(const std::string& name);
    IndexParameter(const IndexParameter& arg);
    ~IndexParameter();

    IndexParameter& operator=(const IndexParameter& arg);

    bool get_value(double& value) const;
    bool get_value(int& value) const;
    bool get_value(std::string& value) const;

    IndexParameter& value(double value);
    IndexParameter& value(int value);
    IndexParameter& value(const std::string& value);

    std::string name() const;
    IndexParameter& name(const std::string& name);

    COEK_API_OPERATORS

    friend std::ostream& operator<<(std::ostream& ostr, const IndexParameter& arg);
};


IndexParameter set_element();
IndexParameter set_element(const std::string& name);


enum VariableTypes : unsigned int { Reals=0, Integers=1, Boolean=2, Binary=3 };

/**
  * \class Variable
  * \brief Decision variables that appear in expressions.
  *
  * Variables are used to define decision variables that are optimized.
  */
class Variable
{
public:

    VariableRepn repn;

public:

    /** Construct a continuous decision variable.  */
    Variable();
    explicit Variable(const std::string& name);
    Variable(const VariableRepn& _repn);
    Variable(const Variable& arg);
    virtual ~Variable();

    Variable& operator=(const Variable& arg);

    /** Set the initial variable value. \returns the variable object. */
    Variable& value(double value);
    /** Set the initial variable value. \returns the variable object. */
    Variable& value(const Expression& value);
    /** \returns the initial value of the variable */
    double value() const;
    /** \returns the expression defining the initial variable value */
    Expression value_expression() const;

    /** Set the lower bound. \returns the variable object. */
    Variable& lower(double value);
    /** Set the lower bound. \returns the variable object. */
    Variable& lower(const Expression& value);
    /** \returns the lower bound */
    double lower() const;
    /** \returns the expression defining the variable lower bound */
    Expression lower_expression() const;

    /** Set the upper bound. \returns the variable object. */
    Variable& upper(double value);
    /** Set the upper bound. \returns the variable object. */
    Variable& upper(const Expression& value);
    /** \returns the upper bound */
    double upper() const;
    /** \returns the expression defining the variable upper bound */
    Expression upper_expression() const;

    /** Set the upper and lower bounds. \returns the variable object. */
    Variable& bounds(double lb, double ub);
    /** Set the upper and lower bounds. \returns the variable object. */
    Variable& bounds(const Expression& lb, double ub);
    /** Set the upper and lower bounds. \returns the variable object. */
    Variable& bounds(double lb, const Expression& ub);
    /** Set the upper and lower bounds. \returns the variable object. */
    Variable& bounds(const Expression& lb, const Expression& ub);

    /** Set a flag indicating if the variable is fixed. \returns the variable object. */
    Variable& fixed(bool flag);
    /** Set the variable value and declare the variable fixed. \returns the variable object. */
    Variable& fix(double value);
    /** \returns \c true if the variable is fixed */
    bool fixed() const;

    /** Set the name of the variable. \returns the variable object */
    Variable& name(const std::string& name);
    /** \returns the value of the variable.  */
    std::string name() const;

    /** Set the variable type. \returns the variable object */
    Variable& within(VariableTypes vtype);
    /** \returns the variable type. */
    VariableTypes within() const;
    /** \returns \c true if the variable is continuous */
    bool is_continuous() const;
    /** \returns \c true if the variable is binary */
    bool is_binary() const;
    /** \returns \c true if the variable is integer */
    bool is_integer() const;

    /** \returns the unique integer variable ID */
    unsigned int id() const;

    /** \returns \c false because this is not a constant expression */
    bool is_constant() const
        {return false;}

    COEK_API_OPERATORS

    /**
      * \name Stream operator
      *
      * Stream the variable to an output stream.
      *
      * \param ostr   output stream
      * \param arg   variable
      */
    friend std::ostream& operator<<(std::ostream& ostr, const Variable& arg);
};


Variable variable();
Variable variable(const std::string& name);


/**
  * \class Expression
  * \brief Container for expressions in equations.
  *
  * This class is the core container for expressions that appear in objectives and constraints.
  */
class Expression
{
public:

    ExpressionRepn repn;

public:

    /** Constructs an Expression without defining its value */
    Expression();
    /** Explict construction of an Expression from a double */
    explicit Expression(double value);
    /** Explict construction of an Expression from an integer */
    explicit Expression(int value);
    /** Implicit construction of an Expression from a Parameter */
    Expression(const Parameter& arg);
    /** Implicit construction of an Expression from an IndexParameter */
    Expression(const IndexParameter& arg);
    /** Implicit construction of an Expression from a Variable */
    Expression(const Variable& arg);

    Expression(const ExpressionRepn& _repn);
    Expression(const Expression& arg);
    ~Expression();

    Expression& operator=(const Expression& arg);

    /** \returns \c true if this is a constant expression */
    bool is_constant() const;

    /** \returns the value of the expression
      *
      * \note The expression value is computed from the
      * expression tree using values for the associated
      * Parameter and Variable objects.
      */
    double value() const;

    /** \returns a list representation of the expression */
    std::list<std::string> to_list() const;
    /**
      * Create an expression that computes the partial derivative relative to a specified variable.
      *
      * \param var - The variable that will be used to cmopute the partial derivative
      * \returns an expression that computes the partial derivative
      */
    Expression diff(const Variable& var) const;

    /** Add an integer to the expression */
    Expression& operator+=(int arg);
    /** Add a double to the expression */
    Expression& operator+=(double arg);
    /** Add a Parameter to the expression */
    Expression& operator+=(const Parameter& arg);
    /** Add an IndexParameter to the expression */
    Expression& operator+=(const IndexParameter& arg);
    /** Add a Variable to the expression */
    Expression& operator+=(const Variable& arg);
    /** Add an Expression to the expression */
    Expression& operator+=(const Expression& arg);

    /** Subtract an integer from the expression */
    Expression& operator-=(int arg);
    /** Subtract a double from the expression */
    Expression& operator-=(double arg);
    /** Subtract a Parameter from the expression */
    Expression& operator-=(const Parameter& arg);
    /** Subtract an IndexParameter from the expression */
    Expression& operator-=(const IndexParameter& arg);
    /** Subtract a Variable from the expression */
    Expression& operator-=(const Variable& arg);
    /** Subtract an Expression from the expression */
    Expression& operator-=(const Expression& arg);

    /** Multiply the expression by an integer */
    Expression& operator*=(int arg);
    /** Multiply the expression by a double */
    Expression& operator*=(double arg);
    /** Multiply the expression by a Parameter */
    Expression& operator*=(const Parameter& arg);
    /** Multiply the expression by an IndexParameter */
    Expression& operator*=(const IndexParameter& arg);
    /** Multiply the expression by a Variable */
    Expression& operator*=(const Variable& arg);
    /** Multiply the expression by an Expression */
    Expression& operator*=(const Expression& arg);

    /** Divide the expression by an integer */
    Expression& operator/=(int arg);
    /** Divide the expression by a double */
    Expression& operator/=(double arg);
    /** Divide the expression by a Parameter */
    Expression& operator/=(const Parameter& arg);
    /** Divide the expression by an IndexParameter */
    Expression& operator/=(const IndexParameter& arg);
    /** Divide the expression by a Variable */
    Expression& operator/=(const Variable& arg);
    /** Divide the expression by an Expression */
    Expression& operator/=(const Expression& arg);

    COEK_API_OPERATORS

    friend std::ostream& operator<<(std::ostream& ostr, const Expression& arg);

    /** \returns an expanded Expression */
    Expression expand();
};


Expression expression();
Expression expression(double arg);
Expression expression(int arg);
Expression expression(const Parameter& arg);
Expression expression(const Variable& arg);


//
// operator<<
//
std::ostream& operator<<(std::ostream& ostr, const Parameter& arg);
std::ostream& operator<<(std::ostream& ostr, const Variable& arg);
std::ostream& operator<<(std::ostream& ostr, const Expression& arg);

}
