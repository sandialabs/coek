#ifndef COEK_API_EXPRESSION_H
#define COEK_API_EXPRESSION_H
#pragma once

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <set>
#include <memory>

#include "constants.hpp"

#define COEK_API_OPERATORS\
    Expression operator+(int arg) const;\
    Expression operator+(double arg) const;\
    Expression operator+(const Parameter& arg) const;\
    Expression operator+(const IndexParameter& arg) const;\
    Expression operator+(const Variable& arg) const;\
    Expression operator+(const Expression& arg) const;\
\
    Expression operator-(int arg) const;\
    Expression operator-(double arg) const;\
    Expression operator-(const Parameter& arg) const;\
    Expression operator-(const IndexParameter& arg) const;\
    Expression operator-(const Variable& arg) const;\
    Expression operator-(const Expression& arg) const;\
\
    Expression operator*(int arg) const;\
    Expression operator*(double arg) const;\
    Expression operator*(const Parameter& arg) const;\
    Expression operator*(const IndexParameter& arg) const;\
    Expression operator*(const Variable& arg) const;\
    Expression operator*(const Expression& arg) const;\
\
    Expression operator/(int arg) const;\
    Expression operator/(double arg) const;\
    Expression operator/(const Parameter& arg) const;\
    Expression operator/(const IndexParameter& arg) const;\
    Expression operator/(const Variable& arg) const;\
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
class ExpressionSequence;
class ExpressionSequenceRepn;
class ConstraintSequence;
class ConstraintSequenceRepn;
class ConcreteSet;

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

    /** Constructs a Parameter without defining its value. */
    Parameter();
    /**
      * Constructs a Parameter initialized with a given value.
      *
      * \param value   the initial value of the parameter
      */
    Parameter(double value);
    /**
      * Constructs a Parameter initialized with a given value and a name.
      *  
      * \param value   the initial value of the parameter
      * \param name   the name of the parameter
      */
    Parameter(double value, const std::string& name);
    /**
      * Copy constructor
      *
      * \param arg   a parameter whose value is shared
      */
    Parameter(const Parameter& arg);
    ~Parameter();

    Parameter& operator=(const Parameter& arg);

    /** \returns the value of the parameter.  */
    double get_value() const;
    /**
      * Set the value of the parameter
      *
      * \param value   the parameter value
      */
    void set_value(double value);

    bool is_constant() const
        {return false;}

    /** \returns the name of the parameter. */
    std::string get_name() const;

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


// Index Parameter 
class IndexParameter
{
public:

    IndexParameterRepn repn;

public:

    IndexParameter();
    IndexParameter(const std::string& name);
    IndexParameter(const IndexParameter& arg);
    ~IndexParameter();

    IndexParameter& operator=(const IndexParameter& arg);

    void get_value(double& value) const;
    void get_value(int& value) const;
    void get_value(std::string& value) const;

    void set_value(double value);
    void set_value(int value);
    void set_value(const std::string& value);

    std::string get_name() const;

    COEK_API_OPERATORS

    friend std::ostream& operator<<(std::ostream& ostr, const IndexParameter& arg);
};


/**
  * \class Variable
  * \brief Decision variables that appear in expressions.
  *
  * Variables are used to define decision variables that are used in expressions.
  */
class Variable
{
public:

    VariableRepn repn;

public:

    /**
      * Constructs a Variable initialized with standard arguments.
      *
      * This constructor accepts one or more standard options.  When default values are used, this
      * specifies an unbounded, continuous decision variable.
      *
      * \param lb   the lower bound (default is negative infinity)
      * \param ub   the upper bound (default is infinity)
      * \param value   the initial value (default is 0.0)
      * \param binary   a boolean flag that indicates whether the variable is boolean (default is \c false)
      * \param integer   a boolean flag that indicates whether the variable is a general integer (default is \c false)
      */
    Variable(double lb=-COEK_INFINITY, double ub=COEK_INFINITY, double value=0.0, bool binary=false, bool integer=false);
    /**
      * Constructs a Variable initialized with standard arguments.
      *
      * This constructor accepts one or more standard options.  The first argument is the 
      * name of the variable. When default values are used, this specifies an unbounded,
      * continuous decision variable.
      *
      * \param name   the name of variable
      * \param lb   the lower bound (default is negative infinity)
      * \param ub   the upper bound (default is infinity)
      * \param value   the initial value (default is 0.0)
      * \param binary   a boolean flag that indicates whether the variable is boolean (default is \c false)
      * \param integer   a boolean flag that indicates whether the variable is a general integer (default is \c false)
      */
    Variable(const std::string& name, double lb=-COEK_INFINITY, double ub=COEK_INFINITY, double value=0.0, bool binary=false, bool integer=false);
    Variable(const VariableRepn& _repn);
    Variable(const Variable& arg);
    virtual ~Variable();

    Variable& operator=(const Variable& arg);

    void initialize(double lb, double ub, double value, bool binary, bool integer, bool fixed);
    void initialize(double lb, double ub, double value, bool binary, bool integer, bool fixed, const std::string& name);

    /** \returns the value of the variable */
    double get_value() const;
    /** Set the variable value */
    void set_value(double value);

    /** \returns the lower bound */
    double get_lb() const;
    /** Set the lower bound */
    void set_lb(double value);

    /** \returns the upper bound */
    double get_ub() const;
    /** Set the upper bound */
    void set_ub(double value);

    /** \returns \c true if the variable is fixed */
    bool get_fixed() const;
    /** Set a flag indicating if the variable is fixed */
    void set_fixed(bool flag);

    /** \returns the value of the parameter.  */
    std::string get_name() const;
    /** Set the name */
    void set_name(const std::string& name);

    /** \returns \c true if the variable is continuous */
    bool is_continuous() const;
    /** \returns \c true if the variable is binary */
    bool is_binary() const;
    /** \returns \c true if the variable is integer */
    bool is_integer() const;

    /** \returns the unique integer variable ID */
    unsigned int id() const;

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


class ExpressionSequenceAux
{
public:

    std::shared_ptr<ExpressionSequenceRepn> repn;

public:

    ExpressionSequenceAux(const std::shared_ptr<ExpressionSequenceRepn>& _repn);
    ExpressionSequence in(const ConcreteSet& _index_set);
};


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

    /** Constructs an Expression without defining its value. */
    Expression();
    /** Explict construction of an Expression from a double. */
    explicit Expression(double value);
    /** Explict construction of an Expression from an integer. */
    explicit Expression(int value);
    /** Implicit construction of an Expression from a Parameter. */
    Expression(const Parameter& arg);
    /** Implicit construction of an Expression from an IndexParameter. */
    Expression(const IndexParameter& arg);
    /** Implicit construction of an Expression from a Variable. */
    Expression(const Variable& arg);

    Expression(const ExpressionRepn& _repn);
    Expression(const Expression& arg);
    ~Expression();

    Expression& operator=(const Expression& arg);

    bool is_constant() const;

    /** \returns the value of the expression
      *
      * \note The expression value, is computed from the
      * expression tree, using values for the associated
      * Parameter and Variable objects.
      */
    double get_value() const;

    /** \returns a string representation of the expression. */
    std::list<std::string> to_list() const;
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
    /** Subtract an IndexVariable from the expression */
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

    /** \returns a modifier to specify the indices used in this expression */
    template <typename... TYPES>
    ExpressionSequenceAux forall(const TYPES&... args)
        {
        std::vector<IndexParameter> indices;
        collect_args(indices, args...);
        return forall(indices);
        }

    /** \returns a modifier to specify the indices used in this expression */
    ExpressionSequenceAux forall(const std::vector<IndexParameter>& indices);
    /** \returns an expanded Expression */
    Expression expand();

protected:

    /** Collect template arguments. */
    void collect_args(std::vector<IndexParameter>& indices, const IndexParameter& arg)
        {
        indices.emplace_back(arg);
        }

    /** Collect template arguments. */
    template <typename... TYPES>
    void collect_args(std::vector<IndexParameter>& indices, const IndexParameter& arg, const TYPES&... args)
        {
        indices.emplace_back(arg);
        collect_args(indices, args...);
        }
};


//
// operator<<
//
std::ostream& operator<<(std::ostream& ostr, const Parameter& arg);
std::ostream& operator<<(std::ostream& ostr, const Variable& arg);
std::ostream& operator<<(std::ostream& ostr, const Expression& arg);

}
#endif
