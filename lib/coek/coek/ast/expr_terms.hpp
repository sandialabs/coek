#pragma once

#include <cmath>
#include <memory>
#include <vector>

#include "base_terms.hpp"

namespace coek {

//
// ExpressionTerm
//

class ExpressionTerm : public BaseExpressionTerm {
   public:
    bool is_expression() const { return true; }

    virtual size_t num_expressions() const = 0;

    virtual expr_pointer_t expression(size_t) = 0;
};

//
// UnaryTerm
//

class UnaryTerm : public ExpressionTerm {
   public:
    expr_pointer_t body;

   public:
    explicit UnaryTerm(const expr_pointer_t& repn) : body(repn)
    {
        non_variable = repn->non_variable;
    }

    size_t num_expressions() const { return 1; }
    expr_pointer_t expression(size_t) { return body; }
};

//
// BinaryTerm
//

class BinaryTerm : public ExpressionTerm {
   public:
    expr_pointer_t lhs;
    expr_pointer_t rhs;

   public:
    BinaryTerm(const expr_pointer_t& _lhs, const expr_pointer_t& _rhs);

    size_t num_expressions() const { return 2; }
    expr_pointer_t expression(size_t i)
    {
        if (i == 0) return lhs;
        return rhs;
    }
};

class NAryPrefixTerm : public ExpressionTerm {
   public:
    typedef std::vector<expr_pointer_t> shared_t;
    std::shared_ptr<shared_t> data;
    size_t n;

    ~NAryPrefixTerm();

    void initialize(const expr_pointer_t& lhs, const expr_pointer_t& rhs);

    void initialize(NAryPrefixTerm* lhs, const expr_pointer_t& rhs);

    void push_back(const expr_pointer_t& rhs);

    size_t num_expressions() const { return n; }

    expr_pointer_t expression(size_t i) { return (*data)[i]; }
};

//
// SubExpressionTerm
//

class SubExpressionTerm : public UnaryTerm {
   public:
    static unsigned int count;

   public:
    unsigned int index;
    std::string name;

   public:
    explicit SubExpressionTerm(const expr_pointer_t& body);

    double _eval() const { return body->_eval(); }

    virtual std::string get_simple_name() { return "S[" + std::to_string(index) + "]"; }
    virtual std::string get_name()
    {
        if (name == "")
            return get_simple_name();
        else
            return name;
    }

    void accept(Visitor& v) { v.visit(*this); }
    term_id id() { return SubExpressionTerm_id; }
};

//
// NegateTerm
//

class NegateTerm : public UnaryTerm {
   public:
    explicit NegateTerm(const expr_pointer_t& body) : UnaryTerm(body) {}

    double _eval() const { return -body->_eval(); }

    void accept(Visitor& v) { v.visit(*this); }
    term_id id() { return NegateTerm_id; }
};

//
// PlusTerm
//

class PlusTerm : public NAryPrefixTerm {
   public:
    PlusTerm(const expr_pointer_t& lhs, const expr_pointer_t& rhs);
    PlusTerm(const expr_pointer_t& lhs, const expr_pointer_t& rhs, bool dummy);

    double _eval() const
    {
        // NOTE: Must limit this loop to 0..n-1.  The value 'n' defines the
        //      number of terms in the shared prefix term that are used here.
        double ans = 0;
        for (size_t i = 0; i < n; i++) ans += (*data)[i]->_eval();
        return ans;
    }

    void accept(Visitor& v) { v.visit(*this); }
    term_id id() { return PlusTerm_id; }
};

//
// TimesTerm
//

class TimesTerm : public BinaryTerm {
   public:
    TimesTerm(const expr_pointer_t& lhs, const expr_pointer_t& rhs) : BinaryTerm(lhs, rhs) {}

    double _eval() const { return lhs->_eval() * rhs->_eval(); }

    void accept(Visitor& v) { v.visit(*this); }
    term_id id() { return TimesTerm_id; }
};

//
// DivideTerm
//

class DivideTerm : public BinaryTerm {
   public:
    DivideTerm(const expr_pointer_t& lhs, const expr_pointer_t& rhs) : BinaryTerm(lhs, rhs) {}

    double _eval() const { return lhs->_eval() / rhs->_eval(); }

    void accept(Visitor& v) { v.visit(*this); }
    term_id id() { return DivideTerm_id; }
};

//
// Unary Terms
//

#define UNARY_CLASS(FN, TERM)                                       \
    class TERM : public UnaryTerm {                                 \
       public:                                                      \
        explicit TERM(const expr_pointer_t& body) : UnaryTerm(body) \
        {                                                           \
        }                                                           \
                                                                    \
        double _eval() const                                        \
        {                                                           \
            return ::FN(body->_eval());                             \
        }                                                           \
                                                                    \
        void accept(Visitor& v)                                     \
        {                                                           \
            v.visit(*this);                                         \
        }                                                           \
        term_id id()                                                \
        {                                                           \
            return TERM##_id;                                       \
        }                                                           \
    };

UNARY_CLASS(fabs, AbsTerm)
UNARY_CLASS(ceil, CeilTerm)
UNARY_CLASS(floor, FloorTerm)
UNARY_CLASS(exp, ExpTerm)
UNARY_CLASS(log, LogTerm)
UNARY_CLASS(log10, Log10Term)
UNARY_CLASS(sqrt, SqrtTerm)
UNARY_CLASS(sin, SinTerm)
UNARY_CLASS(cos, CosTerm)
UNARY_CLASS(tan, TanTerm)
UNARY_CLASS(sinh, SinhTerm)
UNARY_CLASS(cosh, CoshTerm)
UNARY_CLASS(tanh, TanhTerm)
UNARY_CLASS(asin, ASinTerm)
UNARY_CLASS(acos, ACosTerm)
UNARY_CLASS(atan, ATanTerm)
UNARY_CLASS(asinh, ASinhTerm)
UNARY_CLASS(acosh, ACoshTerm)
UNARY_CLASS(atanh, ATanhTerm)

// UNARY_CLASS(cot, CotTerm)
// UNARY_CLASS(csc, CscTerm)
// UNARY_CLASS(sec, SecTerm)
// UNARY_CLASS(coth, CothTerm)
// UNARY_CLASS(csch, CschTerm)
// UNARY_CLASS(sech, SechTerm)
// UNARY_CLASS(acot, ACotTerm)
// UNARY_CLASS(acsc, ACscTerm)
// UNARY_CLASS(asec, ASecTerm)
// UNARY_CLASS(acoth, ACothTerm)
// UNARY_CLASS(acsch, ACschTerm)
// UNARY_CLASS(asech, ASechTerm)

//
// Binary Terms
//

#define BINARY_CLASS(FN, TERM)                                                            \
    class TERM : public BinaryTerm {                                                      \
       public:                                                                            \
        TERM(const expr_pointer_t& lhs, const expr_pointer_t& rhs) : BinaryTerm(lhs, rhs) \
        {                                                                                 \
        }                                                                                 \
                                                                                          \
        double _eval() const                                                              \
        {                                                                                 \
            return ::FN(lhs->_eval(), rhs->_eval());                                      \
        }                                                                                 \
                                                                                          \
        void accept(Visitor& v)                                                           \
        {                                                                                 \
            v.visit(*this);                                                               \
        }                                                                                 \
        term_id id()                                                                      \
        {                                                                                 \
            return TERM##_id;                                                             \
        }                                                                                 \
    };

BINARY_CLASS(pow, PowTerm)
// BINARY_CLASS(atan2, ATan2Term)

}  // namespace coek
