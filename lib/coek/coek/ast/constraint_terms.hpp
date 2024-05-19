#pragma once

#include <string>
#include "base_terms.hpp"

namespace coek {

//
// ObjectiveTerm
//

class ObjectiveTerm : public BaseExpressionTerm {
   public:
    static size_t count;

   public:
    bool sense;
    bool active;
    expr_pointer_t body;
    std::string name;
    size_t index;

   public:
    ObjectiveTerm();
    ObjectiveTerm(const expr_pointer_t& body, bool sense);

    double _eval() const { return body->_eval(); }

    term_id id() { return ObjectiveTerm_id; }

    virtual std::string get_simple_name() { return "O[" + std::to_string(index) + "]"; }
    virtual std::string get_name()
    {
        if (name == "")
            return get_simple_name();
        else
            return name;
    }
};

//
// ConstraintTerm
//

class ConstraintTerm : public BaseExpressionTerm {
   public:
    static size_t count;

   public:
    bool active;
    expr_pointer_t lower;
    expr_pointer_t body;
    expr_pointer_t upper;
    std::string name;
    size_t index;

   public:
    ConstraintTerm();
    ConstraintTerm(const expr_pointer_t& lower, const expr_pointer_t& body,
                   const expr_pointer_t& upper);
    ConstraintTerm(const expr_pointer_t& lower, const expr_pointer_t& body, int upper);
    ConstraintTerm(int lower, const expr_pointer_t& body, const expr_pointer_t& upper);

    double _eval() const { return body->_eval(); }

    bool is_constraint() const { return true; }
    virtual bool is_inequality() const { return false; }
    virtual bool is_equality() const { return false; }
    virtual bool is_feasible() const = 0;

    virtual std::string get_simple_name() { return "C[" + std::to_string(index) + "]"; }
    virtual std::string get_name()
    {
        if (name == "")
            return get_simple_name();
        else
            return name;
    }
};

class InequalityTerm : public ConstraintTerm {
   public:
    InequalityTerm(const expr_pointer_t& lower, const expr_pointer_t& body,
                   const expr_pointer_t& upper)
        : ConstraintTerm(lower, body, upper)
    {
    }

    InequalityTerm(const expr_pointer_t& lower, const expr_pointer_t& body, int upper)
        : ConstraintTerm(lower, body, upper)
    {
    }

    InequalityTerm(int lower, const expr_pointer_t& body, const expr_pointer_t& upper)
        : ConstraintTerm(lower, body, upper)
    {
    }

    bool is_inequality() const { return true; }
    bool is_feasible() const
    {
        double bodyval = body->eval();
        bool lhs = (not lower or (lower->eval() <= bodyval));
        bool rhs = (not upper or (upper->eval() >= bodyval));
        return lhs and rhs;
    }

    term_id id() { return InequalityTerm_id; }
};

class StrictInequalityTerm : public ConstraintTerm
{
public:
    StrictInequalityTerm(const expr_pointer_t& lower, const expr_pointer_t& body,
                   const expr_pointer_t& upper)
        : ConstraintTerm(lower, body, upper)
    {
    }

    StrictInequalityTerm(const expr_pointer_t& lower, const expr_pointer_t& body, int upper)
        : ConstraintTerm(lower, body, upper)
    {
    }

    StrictInequalityTerm(int lower, const expr_pointer_t& body, const expr_pointer_t& upper)
        : ConstraintTerm(lower, body, upper)
    {
    }

    bool is_inequality() const { return true; }
    bool is_feasible() const
    {
        double bodyval = body->eval();
        bool lhs = (not lower or (lower->eval() < bodyval));
        bool rhs = (not upper or (upper->eval() > bodyval));
        return lhs and rhs;
    }

    term_id id() { return StrictInequalityTerm_id; }
};

class EqualityTerm : public ConstraintTerm {
   public:
    EqualityTerm(const expr_pointer_t& body, const expr_pointer_t& rhs)
        : ConstraintTerm(rhs, body, 0)
    {
    }

    bool is_equality() const { return true; }
    bool is_feasible() const { return body->eval() == lower->eval(); }

    term_id id() { return EqualityTerm_id; }
};

class EmptyConstraintTerm : public ConstraintTerm {
   public:
    EmptyConstraintTerm();

    bool is_feasible() const { return true; }

    term_id id() { return EmptyConstraintTerm_id; }
};

extern std::shared_ptr<EmptyConstraintTerm> EmptyConstraintRepn;

}  // namespace coek
