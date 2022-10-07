#pragma once

#include <iostream>
#include <list>
#include <string>

namespace coek {

class ObjectiveTerm;
typedef ObjectiveTerm* ObjectiveRepn;
class Objective;
class Expression;

/**
 * An object that represents a model objective.
 */
class Objective {
   public:
    ObjectiveRepn repn;

   public:
    Objective();
    Objective(const ObjectiveRepn& _repn);
    Objective(const Objective& arg);
    ~Objective();

    Objective& operator=(const Objective& arg);

    /** \returns the value of the objective expression */
    double value() const;

    /** Set the objective expression body */
    Objective& expr(const Expression& body);
    /** \returns the objective expression body */
    Expression expr() const;

    /** Set the objective sense */
    Objective& sense(bool sense);
    /** \returns the objective sense */
    bool sense() const;

    /** Set the objective name */
    Objective& name(const std::string& name);
    /** \returns the objective name */
    std::string name() const;

    /** \returns the unique integer objective ID */
    unsigned int id() const;

    /** \returns a list representation of the objective */
    std::list<std::string> to_list() const;

    friend std::ostream& operator<<(std::ostream& ostr, const Objective& arg);
};

Objective objective();
Objective objective(const std::string& name);
Objective objective(const Expression& expr);
Objective objective(const std::string& name, const Expression& expr);

}  // namespace coek
