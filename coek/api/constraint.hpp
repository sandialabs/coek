#ifndef COEK_API_CONSTRAINT_H
#define COEK_API_CONSTRAINT_H
#pragma once

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <memory>

namespace coek {

class ConstraintTerm;
typedef ConstraintTerm* ConstraintRepn;

class ConstraintSequenceRepn;
class ConstraintSequence;

class Parameter;
class IndexParameter;
class Variable;
class Expression;
class Constraint;
class ConcreteSet;


class ConstraintSequenceAux
{
public:

    std::shared_ptr<ConstraintSequenceRepn> repn;

public:

    ConstraintSequenceAux(const std::shared_ptr<ConstraintSequenceRepn>& _repn);
    ConstraintSequence In(const ConcreteSet& _index_set);
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
    ConstraintSequenceAux Forall(const TYPES&... args)
        {
        std::vector<IndexParameter> indices;
        collect_args(indices, args...);
        return Forall(indices);
        }

    ConstraintSequenceAux Forall(const std::vector<IndexParameter>& indices);
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
#endif
