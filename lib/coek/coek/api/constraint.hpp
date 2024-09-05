#pragma once

#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <vector>

namespace coek {

class ConstraintTerm;
typedef std::shared_ptr<ConstraintTerm> ConstraintRepn;

class Parameter;
class IndexParameter;
class Variable;
class Expression;
class Constraint;

/**
 * An object for constraint expressions that represent
 * equality constraints or inequality constraints.
 */
class Constraint {
   public:
    ConstraintRepn repn;

   public:
    /**
     * Creates an empty constraint object.
     */
    Constraint();
    Constraint(const ConstraintRepn& _repn);
    Constraint(const Constraint& arg);

    Constraint& operator=(const Constraint& arg);

    /** Set the constraint */
    Constraint& expr(const Constraint& body);
    /** \returns the constraint */
    Constraint expr() const;

    /** \returns \c true if this is an inequality constraint */
    bool is_inequality() const;
    /** \returns \c true if this is an equality constraint */
    bool is_equality() const;
    /** \returns \c true if the constraint is feasible */
    bool is_feasible() const;


    /** \returns the expression for the constraint lower bound */
    Expression lower() const;
    /** \returns the expression for the constraint body */
    Expression body() const;
    /** \returns the expression for the constraint upper bound */
    Expression upper() const;

    bool has_lower();
    bool has_upper();

    /** Set the constraint name */
    Constraint& name(const std::string& name);
    /** \returns the constraint name */
    std::string name() const;

    Constraint expand();

    /** \returns the active state */
    bool active() const;
    /** Sets the active state to true */
    void activate();
    /** Sets the active state to false */
    void deactivate();

    /** \returns the unique integer constraint ID */
    size_t id() const;

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

inline Constraint constraint()
{
    Constraint tmp;
    return tmp;
}

inline Constraint constraint(const std::string& name)
{
    Constraint tmp;
    tmp.name(name);
    return tmp;
}

inline Constraint constraint(const Constraint& con) { return con; }

inline Constraint constraint(const std::string& name, Constraint& con) { return con.name(name); }

inline Constraint constraint(const std::string& name, Constraint&& con) { return con.name(name); }

//
// inequality(lower, body, upper)
//
Constraint inequality(const Expression& lower, const Expression& body, const Expression& upper,
                      bool strict = false);

template <typename LowerType, typename UpperType,
          typename = typename std::enable_if<std::is_arithmetic<LowerType>::value, LowerType>::type>
inline Constraint inequality(LowerType _lower, const Expression& body, const UpperType& upper,
                             bool strict = false)
{
    Expression lower(_lower);
    return inequality(lower, body, upper, strict);
}

template <typename UpperType,
          typename = typename std::enable_if<std::is_arithmetic<UpperType>::value, UpperType>::type>
inline Constraint inequality(const Expression& lower, const Expression& body, UpperType _upper,
                             bool strict = false)
{
    Expression upper(_upper);
    return inequality(lower, body, upper, strict);
}

}  // namespace coek
