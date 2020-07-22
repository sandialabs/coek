#pragma once

#include "base_terms.hpp"


namespace coek {

//
// ObjectiveTerm
//

class ObjectiveTerm : public BaseExpressionTerm
{
public:

    static int count;

public:

    expr_pointer_t body;
    bool sense;
    int index;

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

}
