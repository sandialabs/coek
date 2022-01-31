#ifndef COEK_API_OBJECTIVE_H
#define COEK_API_OBJECTIVE_H
#pragma once

#include <iostream>
#include <string>
#include <list>


namespace coek {

class ObjectiveTerm;
typedef ObjectiveTerm* ObjectiveRepn;
class Objective;
class Expression;


/**
  * An object that represents a model objective.
  */
class Objective
{
public:

    ObjectiveRepn repn;

public:

    /**
      * Creates an empty objective object.
      */
    Objective();
    /**
      * Creates an objective.
      *
      * \param expr   The objective expression
      * \param sense   A boolean that is \c true if the sense is minimize
      */
    Objective(const Expression& expr, bool sense);
    /**
      * Creates an objective.
      *
      * \param name   The name of the objective
      * \param expr   The objective expression
      * \param sense   A boolean that is \c true if the sense is minimize
      */
    Objective(const std::string& name, const Expression& _repn, bool sense);
    Objective(const ObjectiveRepn& _repn);
    Objective(const Objective& arg);
    ~Objective();

    Objective& operator=(const Objective& arg);

    /** \returns the unique integer objective ID */
    unsigned int id() const;
    /** \returns the value of the objective expression
      */
    double get_value() const;

    /** Set the objective expression body */
    void set_body(const Expression& body);
    /** \returns the objective expression body */
    Expression body() const;
    /** Set the objective sense */
    void set_sense(bool sense);
    /** \returns the objective sense */
    bool sense() const;
    /** Set the objective name */
    void set_name(const std::string& name);
    /** \returns the objective name */
    std::string get_name() const;

    /** \returns a list representation of the objective */
    std::list<std::string> to_list() const;

    friend std::ostream& operator<<(std::ostream& ostr, const Objective& arg);
};

}
#endif
