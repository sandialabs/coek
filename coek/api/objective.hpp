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

    void set_body(const Expression& body);
    Expression body() const;
    void set_sense(bool sense);
    bool sense() const;

    std::list<std::string> to_list() const;

    friend std::ostream& operator<<(std::ostream& ostr, const Objective& arg);
};

}
#endif
