#pragma once

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <set>
#include <memory>

#define COEK_INFINITY   1e100
#define COEK_UNDEFINED  1e101
#define COEK_NAN        coek::nan_value

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
    Expression operator/(const Expression& arg) const;\
\
    Constraint operator<(int arg) const;\
    Constraint operator<(double arg) const;\
    Constraint operator<(const Parameter& arg) const;\
    Constraint operator<(const IndexParameter& arg) const;\
    Constraint operator<(const Variable& arg) const;\
    Constraint operator<(const Expression& arg) const;\
\
    Constraint operator<=(int arg) const;\
    Constraint operator<=(double arg) const;\
    Constraint operator<=(const Parameter& arg) const;\
    Constraint operator<=(const IndexParameter& arg) const;\
    Constraint operator<=(const Variable& arg) const;\
    Constraint operator<=(const Expression& arg) const;\
\
    Constraint operator>(int arg) const;\
    Constraint operator>(double arg) const;\
    Constraint operator>(const Parameter& arg) const;\
    Constraint operator>(const IndexParameter& arg) const;\
    Constraint operator>(const Variable& arg) const;\
    Constraint operator>(const Expression& arg) const;\
\
    Constraint operator>=(int arg) const;\
    Constraint operator>=(double arg) const;\
    Constraint operator>=(const Parameter& arg) const;\
    Constraint operator>=(const IndexParameter& arg) const;\
    Constraint operator>=(const Variable& arg) const;\
    Constraint operator>=(const Expression& arg) const;\
\
    Constraint operator==(int arg) const;\
    Constraint operator==(double arg) const;\
    Constraint operator==(const Parameter& arg) const;\
    Constraint operator==(const IndexParameter& arg) const;\
    Constraint operator==(const Variable& arg) const;\
    Constraint operator==(const Expression& arg) const;


// A general streaming utility that is not put in the coek namespace for now
std::ostream& operator<<(std::ostream& ostr, const std::list<std::string>& arg);


namespace coek {

extern double nan_value;


std::ostream& operator<<(std::ostream& ostr, const std::list<std::string>& arg);

class ParameterTerm;
class IndexParameterTerm;
class VariableTerm;
class BaseExpressionTerm;
class ConstraintTerm;
class ObjectiveTerm;

typedef ParameterTerm* ParameterRepn;
typedef IndexParameterTerm* IndexParameterRepn;
typedef VariableTerm* VariableRepn;
typedef BaseExpressionTerm* ExpressionRepn;
typedef ConstraintTerm* ConstraintRepn;
typedef ObjectiveTerm* ObjectiveRepn;

class Parameter;
class IndexParameter;
class Variable;
class Expression;
class Constraint;
class Objective;
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

Expression affine_expression(std::vector<double>& coef, std::vector<Variable>& var, double offset);
Expression affine_expression(std::vector<Variable>& var, double offset);


// Mutable Parameter 
/**
 * @class Parameter
 * @brief Values that appear as mutable constants in expressions.
 *
 * Mutable parameters are used to declare expressions for which coefficients and
 * constant terms can be changed without reconstructing the expression.
 */
class Parameter
{
public:

    ParameterRepn repn;

public:

    /**
     * @name Constructors
     */
     //@{
    /**
     * Constructs a undefined Parameter.
     */
    Parameter();
    /**
     * Constructs a Parameter initialized with a given value.
     *
     * @param value the initial value of the parameter
     */
    Parameter(double value);
    /**
     * Constructs a named Parameter initialized with a given value.
     *
     * @param value the initial value of the parameter
     * @param name the name of the parameter
     */
    Parameter(double value, const std::string& name);
    /**
     * Copy constructor
     *
     * @param arg a parameter whose value is shared
     */
    Parameter(const Parameter& arg);
    ~Parameter();

    Parameter& operator=(const Parameter& arg);

    /**
     * Get the value of the parameter
     *
     * @return return the parameter value
     */
    double get_value() const;
    /**
     * Set the value of the parameter
     *
     * @param the parameter value
     */
    void set_value(double value);

    bool is_constant() const
        {return false;}

    /**
     * Get the name of the parameter
     *
     * @return return the parameter name
     */
    std::string get_name() const;

    COEK_API_OPERATORS

    /**
     * @name Stream operator
     *
     * Stream the parameter to an output stream.
     *
     * @param ostr output stream
     * @param arg parameter
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
    virtual ~Variable();

    Variable& operator=(const Variable& arg);

    void initialize(double lb, double ub, double value, bool binary, bool integer, bool fixed, const std::string& name);
    void initialize(double lb, double ub, double value, bool binary, bool integer, bool fixed);

    bool is_constant() const
        {return false;}

    double get_value() const;
    void set_value(double value);

    double get_lb() const;
    void set_lb(double value);

    double get_ub() const;
    void set_ub(double value);

    unsigned int id() const;
    std::string get_name() const;

    void set_fixed(bool flag);
    bool get_fixed() const;

    bool is_continuous() const;
    bool is_binary() const;
    bool is_integer() const;

    COEK_API_OPERATORS

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


// Coek AST Expression
class Expression
{
public:

    ExpressionRepn repn;

public:

    Expression();
    Expression(const ExpressionRepn& _repn);
    explicit Expression(double value);
    explicit Expression(int value);
    Expression(const Parameter& arg);
    Expression(const IndexParameter& arg);
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
    Expression& operator+=(const IndexParameter& arg);
    Expression& operator+=(const Variable& arg);
    Expression& operator+=(const Expression& arg);

    Expression& operator-=(int arg);
    Expression& operator-=(double arg);
    Expression& operator-=(const Parameter& arg);
    Expression& operator-=(const IndexParameter& arg);
    Expression& operator-=(const Variable& arg);
    Expression& operator-=(const Expression& arg);

    Expression& operator*=(int arg);
    Expression& operator*=(double arg);
    Expression& operator*=(const Parameter& arg);
    Expression& operator*=(const IndexParameter& arg);
    Expression& operator*=(const Variable& arg);
    Expression& operator*=(const Expression& arg);

    Expression& operator/=(int arg);
    Expression& operator/=(double arg);
    Expression& operator/=(const Parameter& arg);
    Expression& operator/=(const IndexParameter& arg);
    Expression& operator/=(const Variable& arg);
    Expression& operator/=(const Expression& arg);

    COEK_API_OPERATORS

    friend std::ostream& operator<<(std::ostream& ostr, const Expression& arg);

    template <typename... TYPES>
    ExpressionSequenceAux forall(const TYPES&... args)
        {
        std::vector<IndexParameter> indices;
        collect_args(indices, args...);
        return forall(indices);
        }

    ExpressionSequenceAux forall(const std::vector<IndexParameter>& indices);
    Expression expand();

protected:

    void collect_args(std::vector<IndexParameter>& indices, const IndexParameter& arg)
        {
        indices.emplace_back(arg);
        }

    template <typename... TYPES>
    void collect_args(std::vector<IndexParameter>& indices, const IndexParameter& arg, const TYPES&... args)
        {
        indices.emplace_back(arg);
        collect_args(indices, args...);
        }
};


// Coek Objective
class Objective
{
public:

    ObjectiveRepn repn;

public:

    Objective();
    Objective(const ObjectiveRepn& _repn);
    Objective(const Expression& _repn, bool sense);
    Objective(const Objective& arg);
    ~Objective();

    Objective& operator=(const Objective& arg);

    unsigned int id() const;

    Expression body() const;
    bool sense() const;

    std::list<std::string> to_list() const;

    friend std::ostream& operator<<(std::ostream& ostr, const Objective& arg);
};


class ConstraintSequenceAux
{
public:

    std::shared_ptr<ConstraintSequenceRepn> repn;

public:

    ConstraintSequenceAux(const std::shared_ptr<ConstraintSequenceRepn>& _repn);
    ConstraintSequence in(const ConcreteSet& _index_set);
};


// Coek Constraint
class Constraint
{
public:

    ConstraintRepn repn;

public:

    Constraint();
    Constraint(const ConstraintRepn& _repn);
    Constraint(const Constraint& arg);
    ~Constraint();

    Constraint& operator=(const Constraint& arg);

    unsigned int id() const;

    bool is_inequality() const;
    bool is_equality() const;
    bool is_feasible() const;

    Expression lower() const;
    Expression body() const;
    Expression upper() const;

    double get_lb() const;
    double get_ub() const;

    template <typename... TYPES>
    ConstraintSequenceAux forall(const TYPES&... args)
        {
        std::vector<IndexParameter> indices;
        collect_args(indices, args...);
        return forall(indices);
        }

    ConstraintSequenceAux forall(const std::vector<IndexParameter>& indices);
    Constraint expand();

    std::list<std::string> to_list() const;

    friend std::ostream& operator<<(std::ostream& ostr, const Constraint& arg);

protected:

    void collect_args(std::vector<IndexParameter>& indices, const IndexParameter& arg)
        {
        indices.emplace_back(arg);
        }

    template <typename... TYPES>
    void collect_args(std::vector<IndexParameter>& indices, const IndexParameter& arg, const TYPES&... args)
        {
        indices.emplace_back(arg);
        collect_args(indices, args...);
        }
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

    void collect_terms(const Expression& expr);
    void collect_terms(const Objective& expr);
    void collect_terms(const Constraint& expr);

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
    void collect_terms(Objective& expr);
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

Constraint inequality(int lower, const Expression& body, int upper, bool strict=false);
Constraint inequality(double lower, const Expression& body, double upper, bool strict=false);
Constraint inequality(const Expression& lower, const Expression& body, const Expression& upper, bool strict=false);

Constraint operator<(int, const Parameter&);
Constraint operator<(int, const IndexParameter&);
Constraint operator<(int, const Variable&);
Constraint operator<(int, const Expression&);
Constraint operator<(double, const Parameter&);
Constraint operator<(double, const IndexParameter&);
Constraint operator<(double, const Variable&);
Constraint operator<(double, const Expression&);

Constraint operator<=(int, const Parameter&);
Constraint operator<=(int, const IndexParameter&);
Constraint operator<=(int, const Variable&);
Constraint operator<=(int, const Expression&);
Constraint operator<=(double, const Parameter&);
Constraint operator<=(double, const IndexParameter&);
Constraint operator<=(double, const Variable&);
Constraint operator<=(double, const Expression&);

Constraint operator>(int, const Parameter&);
Constraint operator>(int, const IndexParameter&);
Constraint operator>(int, const Variable&);
Constraint operator>(int, const Expression&);
Constraint operator>(double, const Parameter&);
Constraint operator>(double, const IndexParameter&);
Constraint operator>(double, const Variable&);
Constraint operator>(double, const Expression&);

Constraint operator>=(int, const Parameter&);
Constraint operator>=(int, const IndexParameter&);
Constraint operator>=(int, const Variable&);
Constraint operator>=(int, const Expression&);
Constraint operator>=(double, const Parameter&);
Constraint operator>=(double, const IndexParameter&);
Constraint operator>=(double, const Variable&);
Constraint operator>=(double, const Expression&);

Constraint operator==(int, const Parameter&);
Constraint operator==(int, const IndexParameter&);
Constraint operator==(int, const Variable&);
Constraint operator==(int, const Expression&);
Constraint operator==(double, const Parameter&);
Constraint operator==(double, const IndexParameter&);
Constraint operator==(double, const Variable&);
Constraint operator==(double, const Expression&);

//
// intrinsic functions
//
Expression abs(const Expression& body);
Expression ceil(const Expression& body);
Expression floor(const Expression& body);
Expression exp(const Expression& body);
Expression log(const Expression& body);
Expression log10(const Expression& body);
Expression sqrt(const Expression& body);
Expression sin(const Expression& body);
Expression cos(const Expression& body);
Expression tan(const Expression& body);
Expression sinh(const Expression& body);
Expression cosh(const Expression& body);
Expression tanh(const Expression& body);
Expression asin(const Expression& body);
Expression acos(const Expression& body);
Expression atan(const Expression& body);
Expression asinh(const Expression& body);
Expression acosh(const Expression& body);
Expression atanh(const Expression& body);

Expression pow(const Expression&, const Expression&);
Expression pow(const Expression&, double);
Expression pow(double, const Expression&);

}
