#pragma once

#include <unordered_map>
#include <iostream>

#include "visitor.hpp"
#if defined(DEBUG)
#define WITH_AST_ENV
#endif

namespace coek {

class Visitor;
class BaseExpressionTerm;

// SHARED_PTR
//typedef std::shared_ptr<BaseExpressionTerm> expr_pointer_t;
//typedef std::shared_ptr<VariableTerm> var_pointer_t;
//#define CREATE_POINTER(PTR, ...) std::make_shared<PTR>(__VA_ARGS__)
//#define OWN_POINTER(PTR) 
//#define DISOWN_POINTER(PTR) 
//#define STATIC_CAST(TYPE, ARG)   std::static_pointer_cast<TYPE>(ARG)

typedef BaseExpressionTerm* expr_pointer_t;
//typedef std::variant<int,double,std::string,expr_pointer_t> refarg_types;
//typedef std::variant<int,expr_pointer_t> refarg_types;

#ifdef WITH_AST_ENV
#define CREATE_POINTER(PTR, ...) env.cache(new PTR(__VA_ARGS__))
#define CACHE_POINTER(PTR) env.cache(PTR)
#define DISCARD_POINTER(PTR) env.uncache(PTR)
#define OWN_POINTER(PTR) env.own(PTR)
#define DISOWN_POINTER(PTR) env.disown(PTR)
#define FREE_POINTER(PTR) env.free(PTR)
#else
#define CREATE_POINTER(PTR, ...) new PTR(__VA_ARGS__)
#define CACHE_POINTER(PTR)
#define DISCARD_POINTER(PTR) {if ((PTR)->refcount == 0) delete PTR;}
#define OWN_POINTER(PTR) (PTR)->refcount++
#define DISOWN_POINTER(PTR) {(PTR)->refcount--; if ((PTR)->refcount == 0) delete PTR;}
#define FREE_POINTER(PTR) (PTR)->refcount--
#endif
#define STATIC_CAST(TYPE, ARG)   ARG


class BaseExpressionTerm
{
public:

    int refcount;
    bool non_variable;

    BaseExpressionTerm(int _refcount=0)
        : refcount(_refcount), non_variable(false) {}

    virtual ~BaseExpressionTerm() {}

    virtual double eval() const = 0;

    virtual bool is_constant() const
        {return false;}
    virtual bool is_parameter() const
        {return false;}
    virtual bool is_abstract_parameter() const
        {return false;}
    virtual bool is_variable() const
        {return false;}
    virtual bool is_monomial() const
        {return false;}
    virtual bool is_expression() const
        {return false;}

    virtual expr_pointer_t const_mult(double coef, const expr_pointer_t& repn);
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
        {non_variable=true;}

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


class DummyConstraintTerm;
class DummyObjectiveTerm;
#ifdef WITH_AST_ENV
}
#include "constraint_terms.hpp"

namespace coek {

//
// GCOVR_EXCL_START
class ASTEnvironment
{
public:

    typedef std::unordered_map<BaseExpressionTerm*,int> cache_t;
    std::unordered_map<BaseExpressionTerm*,int> data;

    static bool debug;
    unsigned int num_global_constants;

    ConstantTerm OneConstant;
    ConstantTerm ZeroConstant;
    ConstantTerm NegativeOneConstant; 
    DummyConstraintTerm DummyConstraint; 
    DummyObjectiveTerm DummyObjective; 

public:

    ASTEnvironment()
        : num_global_constants(5), OneConstant(1), ZeroConstant(0), NegativeOneConstant(-1), DummyConstraint(), DummyObjective()
        { reset(); }

    ~ASTEnvironment();

    bool undeleted_memory();

    bool check_memory();

    void reset();

    template <typename TYPE>
    TYPE* cache(TYPE* ptr, int ctr=0)
        {
        BaseExpressionTerm* tmp = ptr;
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
        if (curr == data.end()) {
            //throw std::runtime_error("ASTEnvironment::own called with a pointer that is not cached.");
            data.emplace( std::unordered_map<BaseExpressionTerm*,int>::value_type(tmp, 1) );
            }
         else
            curr->second += 1;
        }

    template <typename TYPE>
    void disown(TYPE* ptr)
        {
        BaseExpressionTerm* tmp = ptr;
        cache_t::iterator curr = data.find(tmp);
        if (curr == data.end())
            throw std::runtime_error("ASTEnvironment::disown called with a pointer that is not cached.");
        if (curr->second > 0)
            curr->second -= 1;
        if (curr->second == 0) {
            data.erase( curr );
            delete tmp;
            }
        }

    template <typename TYPE>
    void free(TYPE* ptr)
        {
        BaseExpressionTerm* tmp = ptr;
        cache_t::iterator curr = data.find(tmp);
        if (curr == data.end())
            throw std::runtime_error("ASTEnvironment::free called with a pointer that is not cached.");
        if (curr->second > 0)
            curr->second -= 1;
        }

    void write(std::ostream& ostr);
};

extern ASTEnvironment env;
#define ZEROCONST &(env.ZeroConstant)
#define ONECONST &(env.OneConstant)
#define NEGATIVEONECONST &(env.NegativeOneConstant)
#define DUMMYCONSTRAINT &(env.DummyConstraint)
#define DUMMYOBJECTIVE &(env.DummyObjective)

#else
extern ConstantTerm ZeroConstant;
extern ConstantTerm OneConstant;
extern ConstantTerm NegativeOneConstant;
extern DummyConstraintTerm DummyConstraint;
extern DummyObjectiveTerm DummyObjective;
#define ZEROCONST &(ZeroConstant)
#define ONECONST &(OneConstant)
#define NEGATIVEONECONST &(NegativeOneConstant)
#define DUMMYCONSTRAINT &(DummyConstraint)
#define DUMMYOBJECTIVE &(DummyObjective)
// GCOVR_EXCL_STOP
#endif

}
