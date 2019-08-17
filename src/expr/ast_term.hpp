#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <cmath>

#include "ast_visitor.hpp"

#if defined(DEBUG)
#define WITH_AST_ENV
#endif


namespace coek {

// SHARED_PTR
//typedef std::shared_ptr<BaseExpressionTerm> expr_pointer_t;
//typedef std::shared_ptr<VariableTerm> var_pointer_t;
//#define CREATE_POINTER(PTR, ...) std::make_shared<PTR>(__VA_ARGS__)
//#define OWN_POINTER(PTR) 
//#define DISOWN_POINTER(PTR) 
//#define STATIC_CAST(TYPE, ARG)   std::static_pointer_cast<TYPE>(ARG)

typedef BaseExpressionTerm* expr_pointer_t;

#ifdef WITH_AST_ENV
#define CREATE_POINTER(PTR, ...) env.cache(new PTR(__VA_ARGS__))
#define DISCARD_POINTER(PTR) env.uncache(PTR)
#define OWN_POINTER(PTR) env.own(PTR)
#define DISOWN_POINTER(PTR) env.disown(PTR)
#else
#define CREATE_POINTER(PTR, ...) new PTR(__VA_ARGS__)
#define DISCARD_POINTER(PTR) {if ((PTR)->refcount == 0) delete PTR;}
#define OWN_POINTER(PTR) (PTR)->refcount++
#define DISOWN_POINTER(PTR) {(PTR)->refcount--; if ((PTR)->refcount == 0) delete PTR;}
#endif
#define STATIC_CAST(TYPE, ARG)   ARG


class BaseExpressionTerm
{
public:

    int refcount;

    BaseExpressionTerm(int _refcount=0)
        : refcount(_refcount) {}

    virtual ~BaseExpressionTerm() {}

    virtual double eval() const = 0;

    virtual bool is_constant() const
        {return false;}
    virtual bool is_parameter() const
        {return false;}
    virtual bool is_variable() const
        {return false;}
    virtual bool is_monomial() const
        {return false;}
    virtual bool is_expression() const
        {return false;}

    virtual expr_pointer_t negate(const expr_pointer_t& repn);
    virtual void accept(Visitor& v) = 0;
    virtual term_id id() = 0;
};

//
// ConstantTerm
//

class ConstantTerm : public BaseExpressionTerm
{
public:

    double value;

    ConstantTerm(double _value, int refcount=0)
        : BaseExpressionTerm(refcount), value(_value)
        {}

    double eval() const
        { return value; }

    bool is_constant() const
        {return true;}
    bool is_parameter() const
        {return true;}

    expr_pointer_t negate(const expr_pointer_t& repn);

    void accept(Visitor& v)
        { v.visit(*this); }
    term_id id()
        {return ConstantTerm_id;}
};

//
// ParameterTerm
//

class ParameterTerm : public ConstantTerm
{
public:

    std::string name;

    ParameterTerm(double _value)
        : ConstantTerm(_value)
        {}

    bool is_constant() const
        {return false;}

    expr_pointer_t negate(const expr_pointer_t& repn);

    void accept(Visitor& v)
        { v.visit(*this); }
    term_id id()
        {return ParameterTerm_id;}
};

//
// VariableTerm
//

class VariableTerm : public BaseExpressionTerm
{
public:

    static int count;

public:

    int index;
    double initialize;
    double value;
    double lb;
    double ub;
    bool binary;
    bool integer;
    bool fixed;
    std::string name;

    VariableTerm(double _value, double _lb, double _ub, bool _binary, bool _integer);

    double eval() const
        { return value; }

    bool is_variable() const
        {return true;}

    expr_pointer_t negate(const expr_pointer_t& repn);

    void accept(Visitor& v)
        { v.visit(*this); }
    term_id id()
        {return VariableTerm_id;}

    virtual std::string get_name()
        {
        if (name == "")
            return "x[" + std::to_string(index) + "]";
        else
            return name;
        }
};

//
// MonomialTerm
//

class MonomialTerm : public BaseExpressionTerm
{
public:

    double coef;
    VariableTerm* var;

    MonomialTerm(double lhs, VariableTerm* rhs);
    ~MonomialTerm();

    double eval() const
        { return coef * var->value; }

    bool is_monomial() const
        {return true;}

    expr_pointer_t negate(const expr_pointer_t& repn);

    void accept(Visitor& v)
        { v.visit(*this); }
    term_id id()
        {return MonomialTerm_id;}
};

//
// ConstraintTerm
//

class ConstraintTerm : public BaseExpressionTerm
{
public:

    expr_pointer_t body;

public:

    ConstraintTerm(const expr_pointer_t& repn);
    ~ConstraintTerm();

    double eval() const
        {return body->eval();}
    virtual bool feasible() const = 0;
    bool is_constraint() const
        {return true;}
    virtual bool is_inequality() const
        {return false;}
    virtual bool is_equality() const
        {return false;}
};

class InequalityTerm : public ConstraintTerm
{
public:

    bool strict;

    InequalityTerm(const expr_pointer_t& repn, bool _strict=false)
        : ConstraintTerm(repn), strict(_strict) {}

    bool feasible() const
        {
        if (strict)
            return body->eval() < 0.0;
        return body->eval() <= 0.0;
        }

    bool is_inequality() const
        {return true;}

    void accept(Visitor& v)
        { v.visit(*this); }
    term_id id()
        {return InequalityTerm_id;}
};

class EqualityTerm : public ConstraintTerm
{
public:

    EqualityTerm(const expr_pointer_t& repn)
        : ConstraintTerm(repn) {}

    bool feasible() const
        {return body->eval() == 0.0;}

    bool is_equality() const
        {return true;}

    void accept(Visitor& v)
        { v.visit(*this); }
    term_id id()
        {return EqualityTerm_id;}
};

//
// ExpressionTerm
//

class ExpressionTerm : public BaseExpressionTerm
{
public:

    bool is_expression() const
        {return true;}

    virtual unsigned int num_expressions() const = 0;

    virtual expr_pointer_t expression(unsigned int) = 0;
};

//
// UnaryTerm
//

class UnaryTerm : public ExpressionTerm
{
public:

    expr_pointer_t body;

public:

    UnaryTerm(const expr_pointer_t& repn);
    ~UnaryTerm();

    unsigned int num_expressions() const
        {return 1;}
    expr_pointer_t expression(unsigned int)
        {return body;}
};

//
// BinaryTerm
//

class BinaryTerm : public ExpressionTerm
{
public:

    expr_pointer_t lhs;
    expr_pointer_t rhs;

public:

    BinaryTerm(const expr_pointer_t& _lhs, const expr_pointer_t& _rhs);
    ~BinaryTerm();

    unsigned int num_expressions() const
        {return 2;}
    expr_pointer_t expression(unsigned int i)
        {
        if (i == 0)
            return lhs;
        return rhs;
        }
};

class NAryPrefixTerm : public ExpressionTerm
{
public:

    typedef std::vector<expr_pointer_t> shared_t;
    std::shared_ptr<shared_t> data;
    unsigned int n;

    ~NAryPrefixTerm();

    void initialize(const expr_pointer_t& lhs, const expr_pointer_t& rhs);

    void initialize(NAryPrefixTerm* lhs, const expr_pointer_t& rhs);

    unsigned int num_expressions() const
        {return n;}

    expr_pointer_t expression(unsigned int i)
        { return (*data)[i]; }
};

//
// NegateTerm
//

class NegateTerm : public UnaryTerm
{
public:

    NegateTerm(const expr_pointer_t& body)
        : UnaryTerm(body) {}

    double eval() const
        {return - body->eval(); }

    void accept(Visitor& v)
        { v.visit(*this); }
    term_id id()
        {return NegateTerm_id;}
};

//
// PlusTerm
//

class PlusTerm : public NAryPrefixTerm
{
public:

    PlusTerm(const expr_pointer_t& lhs, const expr_pointer_t& rhs);

    double eval() const
        {
        double ans = 0;
        for (unsigned int i=0; i<n; i++)
            ans += (*data)[i]->eval();
        return ans;
        }

    void accept(Visitor& v)
        { v.visit(*this); }
    term_id id()
        {return PlusTerm_id;}
};

//
// TimesTerm
//

class TimesTerm : public BinaryTerm
{
public:

    TimesTerm(const expr_pointer_t& lhs, const expr_pointer_t& rhs)
        : BinaryTerm(lhs, rhs) {}

    double eval() const
        {return lhs->eval() * rhs->eval(); }

    void accept(Visitor& v)
        { v.visit(*this); }
    term_id id()
        {return TimesTerm_id;}
};

//
// DivideTerm
//

class DivideTerm : public BinaryTerm
{
public:

    DivideTerm(const expr_pointer_t& lhs, const expr_pointer_t& rhs)
        : BinaryTerm(lhs, rhs) {}

    double eval() const
        {return lhs->eval() / rhs->eval(); }

    void accept(Visitor& v)
        { v.visit(*this); }
    term_id id()
        {return DivideTerm_id;}
};


// TODO: Idea - Intrinsic functions

//
// Unary Terms
//

#define UNARY_CLASS(FN, TERM)\
class TERM : public UnaryTerm\
{\
public:\
\
    TERM(const expr_pointer_t& body)\
        : UnaryTerm(body) {}\
\
    double eval() const\
        {return ::FN(body->eval()); }\
\
    void accept(Visitor& v)\
        { v.visit(*this); }\
    term_id id()\
        {return TERM ## _id;}\
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

//UNARY_CLASS(cot, CotTerm)
//UNARY_CLASS(csc, CscTerm)
//UNARY_CLASS(sec, SecTerm)
//UNARY_CLASS(coth, CothTerm)
//UNARY_CLASS(csch, CschTerm)
//UNARY_CLASS(sech, SechTerm)
//UNARY_CLASS(acot, ACotTerm)
//UNARY_CLASS(acsc, ACscTerm)
//UNARY_CLASS(asec, ASecTerm)
//UNARY_CLASS(acoth, ACothTerm)
//UNARY_CLASS(acsch, ACschTerm)
//UNARY_CLASS(asech, ASechTerm)


//
// Binary Terms
//

#define BINARY_CLASS(FN, TERM)\
class TERM : public BinaryTerm\
{\
public:\
\
    TERM(const expr_pointer_t& lhs, const expr_pointer_t& rhs)\
        : BinaryTerm(lhs,rhs) {}\
\
    double eval() const\
        {return ::FN(lhs->eval(), rhs->eval()); }\
\
    void accept(Visitor& v)\
        { v.visit(*this); }\
    term_id id()\
        {return TERM ## _id;}\
};

BINARY_CLASS(pow, PowTerm)
//BINARY_CLASS(atan2, ATan2Term)


#ifdef WITH_AST_ENV
// GCOVR_EXCL_START
class ASTEnvironment
{
public:

    ConstantTerm OneConstant;
    ConstantTerm ZeroConstant;
    ConstantTerm NegativeOneConstant; 

    typedef std::unordered_map<BaseExpressionTerm*,int> cache_t;
    std::unordered_map<BaseExpressionTerm*,int> data;

    static bool debug;
    unsigned int num_global_constants;

public:

    ASTEnvironment()
        : OneConstant(1), ZeroConstant(0), NegativeOneConstant(-1)
        { reset(); }

    ~ASTEnvironment();

    bool undeleted_memory()
        { return (data.size() > num_global_constants); }

    bool check_memory();

    void reset();

    template <typename TYPE>
    TYPE* cache(TYPE* ptr, int ctr=0)
        {
        BaseExpressionTerm* tmp = ptr;
#if 0
        if (debug) {
            std::cout << "Caching (" << ctr << ") : ";
            tmp->write(std::cout);
            std::cout << std::endl;
            write(std::cout);
            std::cout << std::endl << std::flush;
            }
#endif
        cache_t::iterator curr = data.find(tmp);
        if (curr == data.end()) {
            data.emplace( std::unordered_map<BaseExpressionTerm*,int>::value_type(tmp, ctr) );
            return ptr;
            }
        throw std::runtime_error("ASTEnvironment::cache called with a pointer that is already cached.");
        return 0;
        }

    /// Remove an item if its reference counter is zero
    template <typename TYPE>
    void uncache(TYPE* ptr)
        {
        BaseExpressionTerm* tmp = ptr;
        cache_t::iterator curr = data.find(tmp);
        if (curr == data.end())
            throw std::runtime_error("ASTEnvironment::uncache called with a pointer that is not cached.");
        if (curr->second == 0) {
            data.erase( curr );
            delete tmp;
            }
        }

    template <typename TYPE>
    void own(TYPE* ptr)
        {
        BaseExpressionTerm* tmp = ptr;
        cache_t::iterator curr = data.find(tmp);
        if (curr == data.end())
            throw std::runtime_error("ASTEnvironment::own called with a pointer that is not cached.");
         curr->second += 1;
#if 0
        if (debug) {
            std::cout << "Owning (" << curr->second << ") : ";
            curr->first->write(std::cout);
            std::cout << std::endl;
            write(std::cout);
            std::cout << std::endl << std::flush;
            }
#endif
        }

    template <typename TYPE>
    void disown(TYPE* ptr)
        {
#if 0
        if (debug) {
            std::cout << "Disowning : ";
            ptr->write(std::cout);
            std::cout << std::endl;
            write(std::cout);
            std::cout << std::endl << std::flush;
            }
#endif
        BaseExpressionTerm* tmp = ptr;
        cache_t::iterator curr = data.find(tmp);
        if (curr == data.end())
            throw std::runtime_error("ASTEnvironment::disown called with a pointer that is not cached.");
        curr->second -= 1;
        if (curr->second == 0) {
            data.erase( curr );
            delete tmp;
            }
        }

    void write(std::ostream& ostr);
};

extern ASTEnvironment env;
#define ZEROCONST &(env.ZeroConstant)
#define ONECONST &(env.OneConstant)
#define NEGATIVEONECONST &(env.NegativeOneConstant)

#else
extern ConstantTerm ZeroConstant;
extern ConstantTerm OneConstant;
extern ConstantTerm NegativeOneConstant;
#define ZEROCONST &(ZeroConstant)
#define ONECONST &(OneConstant)
#define NEGATIVEONECONST &(NegativeOneConstant)
// GCOVR_EXCL_STOP
#endif

}
