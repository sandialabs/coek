#pragma once

#include <string>
#include "base_terms.hpp"


namespace coek {

//
// ObjectiveTerm
//

class ObjectiveTerm : public BaseExpressionTerm
{
public:

    static unsigned int count;

public:

    expr_pointer_t body;
    bool sense;
    unsigned int index;
    std::string name;

public:

    ObjectiveTerm();
    ObjectiveTerm(const expr_pointer_t& body, bool sense);
    ~ObjectiveTerm();

    double eval() const
        {return body->eval();}

    void accept(Visitor& v)
        { v.visit(*this); }
    term_id id()
        {return ObjectiveTerm_id;}
};

class DummyObjectiveTerm : public ObjectiveTerm
{
public:

    DummyObjectiveTerm()
        : body(0), sense(false), index(0), name("")
        {}

    double eval() const
        {return 0.0;}
};


}
