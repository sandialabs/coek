#pragma once

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <memory>


namespace coek {

class ConstraintTerm;
typedef ConstraintTerm* ConstraintRepn;

class Parameter;
class IndexParameter;
class Variable;
class Expression;
class Constraint;


/**
  * An object for constraint expressions that represent
  * equality constraints or inequality constraints.
  */
class Constraint
{
public:

    ConstraintRepn repn;

public:

    /**
      * Creates an empty constraint object.
      */
    Constraint();
    Constraint(const ConstraintRepn& _repn);
    Constraint(const Constraint& arg);
    ~Constraint();

    Constraint& operator=(const Constraint& arg);

/** \returns the unique integer constraint ID */
    unsigned int id() const;

    /** \returns \c true if this is an inequality constraint */
    bool is_inequality() const;
    /** \returns \c true if this is an equality constraint */
    bool is_equality() const;
    /** \returns \c true if the constraint is feasible */
    bool is_feasible() const;

    /** \returns the expression for the constraint lower bound */
    Expression get_lower() const;
    /** \returns the expression for the constraint body */
    Expression get_body() const;
    /** \returns the expression for the constraint upper bound */
    Expression get_upper() const;

    /** Set the constraint name */
    void set_name(const std::string& name);
    /** \returns the constraint name */
    std::string get_name() const;

    Constraint expand();

    /** \returns a list representation of the constraint */
    std::list<std::string> to_list() const;

    friend std::ostream& operator<<(std::ostream& ostr, const Constraint& arg);
};


//
// logical operators
//

Constraint operator<(int, const Parameter&);
Constraint operator<(int, const IndexParameter&);
Constraint operator<(int, const Variable&);
Constraint operator<(int, const Expression&);
Constraint operator<(double, const Parameter&);
Constraint operator<(double, const IndexParameter&);
Constraint operator<(double, const Variable&);
Constraint operator<(double, const Expression&);
Constraint operator<(const Parameter&, int);
Constraint operator<(const Parameter&, double);
Constraint operator<(const Parameter&, const Parameter&);
Constraint operator<(const Parameter&, const IndexParameter&);
Constraint operator<(const Parameter&, const Variable&);
Constraint operator<(const Parameter&, const Expression&);
Constraint operator<(const Variable&, int);
Constraint operator<(const Variable&, double);
Constraint operator<(const Variable&, const Parameter&);
Constraint operator<(const Variable&, const IndexParameter&);
Constraint operator<(const Variable&, const Variable&);
Constraint operator<(const Variable&, const Expression&);
Constraint operator<(const IndexParameter&, int);
Constraint operator<(const IndexParameter&, double);
Constraint operator<(const IndexParameter&, const Parameter&);
Constraint operator<(const IndexParameter&, const IndexParameter&);
Constraint operator<(const IndexParameter&, const Variable&);
Constraint operator<(const IndexParameter&, const Expression&);
Constraint operator<(const Expression&, int);
Constraint operator<(const Expression&, double);
Constraint operator<(const Expression&, const Parameter&);
Constraint operator<(const Expression&, const IndexParameter&);
Constraint operator<(const Expression&, const Variable&);
Constraint operator<(const Expression&, const Expression&);

Constraint operator<=(int, const Parameter&);
Constraint operator<=(int, const IndexParameter&);
Constraint operator<=(int, const Variable&);
Constraint operator<=(int, const Expression&);
Constraint operator<=(double, const Parameter&);
Constraint operator<=(double, const IndexParameter&);
Constraint operator<=(double, const Variable&);
Constraint operator<=(double, const Expression&);
Constraint operator<=(const Parameter&, int);
Constraint operator<=(const Parameter&, double);
Constraint operator<=(const Parameter&, const Parameter&);
Constraint operator<=(const Parameter&, const IndexParameter&);
Constraint operator<=(const Parameter&, const Variable&);
Constraint operator<=(const Parameter&, const Expression&);
Constraint operator<=(const Variable&, int);
Constraint operator<=(const Variable&, double);
Constraint operator<=(const Variable&, const Parameter&);
Constraint operator<=(const Variable&, const IndexParameter&);
Constraint operator<=(const Variable&, const Variable&);
Constraint operator<=(const Variable&, const Expression&);
Constraint operator<=(const IndexParameter&, int);
Constraint operator<=(const IndexParameter&, double);
Constraint operator<=(const IndexParameter&, const Parameter&);
Constraint operator<=(const IndexParameter&, const IndexParameter&);
Constraint operator<=(const IndexParameter&, const Variable&);
Constraint operator<=(const IndexParameter&, const Expression&);
Constraint operator<=(const Expression&, int);
Constraint operator<=(const Expression&, double);
Constraint operator<=(const Expression&, const Parameter&);
Constraint operator<=(const Expression&, const IndexParameter&);
Constraint operator<=(const Expression&, const Variable&);
Constraint operator<=(const Expression&, const Expression&);

Constraint operator>(int, const Parameter&);
Constraint operator>(int, const IndexParameter&);
Constraint operator>(int, const Variable&);
Constraint operator>(int, const Expression&);
Constraint operator>(double, const Parameter&);
Constraint operator>(double, const IndexParameter&);
Constraint operator>(double, const Variable&);
Constraint operator>(double, const Expression&);
Constraint operator>(const Parameter&, int);
Constraint operator>(const Parameter&, double);
Constraint operator>(const Parameter&, const Parameter&);
Constraint operator>(const Parameter&, const IndexParameter&);
Constraint operator>(const Parameter&, const Variable&);
Constraint operator>(const Parameter&, const Expression&);
Constraint operator>(const Variable&, int);
Constraint operator>(const Variable&, double);
Constraint operator>(const Variable&, const Parameter&);
Constraint operator>(const Variable&, const IndexParameter&);
Constraint operator>(const Variable&, const Variable&);
Constraint operator>(const Variable&, const Expression&);
Constraint operator>(const IndexParameter&, int);
Constraint operator>(const IndexParameter&, double);
Constraint operator>(const IndexParameter&, const Parameter&);
Constraint operator>(const IndexParameter&, const IndexParameter&);
Constraint operator>(const IndexParameter&, const Variable&);
Constraint operator>(const IndexParameter&, const Expression&);
Constraint operator>(const Expression&, int);
Constraint operator>(const Expression&, double);
Constraint operator>(const Expression&, const Parameter&);
Constraint operator>(const Expression&, const IndexParameter&);
Constraint operator>(const Expression&, const Variable&);
Constraint operator>(const Expression&, const Expression&);

Constraint operator>=(int, const Parameter&);
Constraint operator>=(int, const IndexParameter&);
Constraint operator>=(int, const Variable&);
Constraint operator>=(int, const Expression&);
Constraint operator>=(double, const Parameter&);
Constraint operator>=(double, const IndexParameter&);
Constraint operator>=(double, const Variable&);
Constraint operator>=(double, const Expression&);
Constraint operator>=(const Parameter&, int);
Constraint operator>=(const Parameter&, double);
Constraint operator>=(const Parameter&, const Parameter&);
Constraint operator>=(const Parameter&, const IndexParameter&);
Constraint operator>=(const Parameter&, const Variable&);
Constraint operator>=(const Parameter&, const Expression&);
Constraint operator>=(const Variable&, int);
Constraint operator>=(const Variable&, double);
Constraint operator>=(const Variable&, const Parameter&);
Constraint operator>=(const Variable&, const IndexParameter&);
Constraint operator>=(const Variable&, const Variable&);
Constraint operator>=(const Variable&, const Expression&);
Constraint operator>=(const IndexParameter&, int);
Constraint operator>=(const IndexParameter&, double);
Constraint operator>=(const IndexParameter&, const Parameter&);
Constraint operator>=(const IndexParameter&, const IndexParameter&);
Constraint operator>=(const IndexParameter&, const Variable&);
Constraint operator>=(const IndexParameter&, const Expression&);
Constraint operator>=(const Expression&, int);
Constraint operator>=(const Expression&, double);
Constraint operator>=(const Expression&, const Parameter&);
Constraint operator>=(const Expression&, const IndexParameter&);
Constraint operator>=(const Expression&, const Variable&);
Constraint operator>=(const Expression&, const Expression&);

Constraint operator==(int, const Parameter&);
Constraint operator==(int, const IndexParameter&);
Constraint operator==(int, const Variable&);
Constraint operator==(int, const Expression&);
Constraint operator==(double, const Parameter&);
Constraint operator==(double, const IndexParameter&);
Constraint operator==(double, const Variable&);
Constraint operator==(double, const Expression&);
Constraint operator==(const Parameter&, int);
Constraint operator==(const Parameter&, double);
Constraint operator==(const Parameter&, const Parameter&);
Constraint operator==(const Parameter&, const IndexParameter&);
Constraint operator==(const Parameter&, const Variable&);
Constraint operator==(const Parameter&, const Expression&);
Constraint operator==(const Variable&, int);
Constraint operator==(const Variable&, double);
Constraint operator==(const Variable&, const Parameter&);
Constraint operator==(const Variable&, const IndexParameter&);
Constraint operator==(const Variable&, const Variable&);
Constraint operator==(const Variable&, const Expression&);
Constraint operator==(const IndexParameter&, int);
Constraint operator==(const IndexParameter&, double);
Constraint operator==(const IndexParameter&, const Parameter&);
Constraint operator==(const IndexParameter&, const IndexParameter&);
Constraint operator==(const IndexParameter&, const Variable&);
Constraint operator==(const IndexParameter&, const Expression&);
Constraint operator==(const Expression&, int);
Constraint operator==(const Expression&, double);
Constraint operator==(const Expression&, const Parameter&);
Constraint operator==(const Expression&, const IndexParameter&);
Constraint operator==(const Expression&, const Variable&);
Constraint operator==(const Expression&, const Expression&);

Constraint inequality(int lower, const Expression& body, int upper, bool strict=false);
Constraint inequality(double lower, const Expression& body, double upper, bool strict=false);
Constraint inequality(const Expression& lower, const Expression& body, const Expression& upper, bool strict=false);
}
