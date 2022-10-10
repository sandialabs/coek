#pragma once

#include <string>
#include "base_terms.hpp"

namespace coek {

//
// ObjectiveTerm
//

class ObjectiveTerm : public BaseExpressionTerm {
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

    double eval() const { return body->eval(); }

    void accept(Visitor& v) { v.visit(*this); }
    term_id id() { return ObjectiveTerm_id; }
};

class DummyObjectiveTerm : public ObjectiveTerm {
   public:
    DummyObjectiveTerm() : ObjectiveTerm() {}
    term_id id() { return DummyObjectiveTerm_id; }
#if 0
    double eval() const
        {return 0.0;}
#endif
};

//
// ConstraintTerm
//

class ConstraintTerm : public BaseExpressionTerm {
   public:
    static unsigned int count;

   public:
    unsigned int index;
    expr_pointer_t lower;
    expr_pointer_t body;
    expr_pointer_t upper;
    std::string name;

   public:
    ConstraintTerm();
    ConstraintTerm(const expr_pointer_t& lower, const expr_pointer_t& body,
                   const expr_pointer_t& upper);
    ~ConstraintTerm();

    double eval() const { return body->eval(); }
    bool is_constraint() const { return true; }
    virtual bool is_inequality() const { return false; }
    virtual bool is_equality() const { return false; }
    virtual bool is_feasible() const = 0;
    /* WEH - Not used (yet?)
    virtual bool is_trivial() const
        {return false;}
    */
};

class InequalityTerm : public ConstraintTerm {
   public:
    bool strict;

    InequalityTerm(const expr_pointer_t& lower, const expr_pointer_t& body,
                   const expr_pointer_t& upper, bool _strict = false)
        : ConstraintTerm(lower, body, upper), strict(_strict)
    {
    }

    bool is_inequality() const { return true; }
    bool is_feasible() const
    {
        double bodyval = body->eval();
        if (strict) {
            bool lhs = (not lower or (lower->eval() < bodyval));
            bool rhs = (not upper or (upper->eval() > bodyval));
            return lhs and rhs;
        }
        else {
            bool lhs = (not lower or (lower->eval() <= bodyval));
            bool rhs = (not upper or (upper->eval() >= bodyval));
            return lhs and rhs;
        }
    }

    void accept(Visitor& v) { v.visit(*this); }
    term_id id() { return InequalityTerm_id; }
};

class EqualityTerm : public ConstraintTerm {
   public:
    EqualityTerm(const expr_pointer_t& body, const expr_pointer_t& rhs)
        : ConstraintTerm(rhs, body, 0)
    {
    }

    bool is_equality() const { return true; }
    bool is_feasible() const { return body->eval() == lower->eval(); }

    void accept(Visitor& v) { v.visit(*this); }
    term_id id() { return EqualityTerm_id; }
};

class DummyConstraintTerm : public ConstraintTerm {
   public:
    DummyConstraintTerm();
    ~DummyConstraintTerm()
    {
        body = 0;
        lower = 0;
        upper = 0;
    }

    bool is_feasible() const { return true; }

    void accept(Visitor& v) { v.visit(*this); }
    term_id id() { return DummyConstraintTerm_id; }
};

}  // namespace coek
