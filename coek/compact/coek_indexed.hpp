#pragma once

#include <cassert>
#include <variant>
#include "coek_sets.hpp"

#define USING_INDEXVECTOR


namespace coek {

typedef BaseExpressionTerm* expr_pointer_t;
//typedef std::variant<int,double,std::string,expr_pointer_t> refarg_types;
typedef std::variant<int,expr_pointer_t> refarg_types;
class ConcreteIndexedVariableRepn;


#ifdef USING_INDEXVECTOR

class IndexVector
{
public:

    size_t len;
    int* data;

public:

    // The vector length is stored in the first element.  This limits the size, but this is OK for small vectors
    IndexVector()
        : len(0), data(0)
        {}

    explicit IndexVector(int* _data)
        : len(static_cast<size_t>(_data[0])), data(_data+1)
        {}

    explicit IndexVector(int* _data, size_t _len)
        : len(_len), data(_data)
        {}

    IndexVector(const IndexVector& other)
        : len(other.len), data(other.data)
        {}

    IndexVector& operator=(const IndexVector& other)
        {len = other.len; data=other.data; return *this;}

    size_t size() const
        { return static_cast<size_t>(len); }

    int& operator[](size_t i)
        { return data[i]; }

    const int& operator[](size_t i) const
        { return data[i]; }

    bool operator<(const IndexVector& other) const
        {
        for (size_t i=0; i<len; i++) {
            if (data[i] < other.data[i])
                return true;
            else if (data[i] > other.data[i])
                return false;
            }
        return true;
        }

    bool operator==(const IndexVector& other) const
        {
        if (len == 1)
            return data[0] == other.data[0];
        else if (len == 2) {
            return (data[0] == other.data[0]) and (data[1] == other.data[1]);
            }
        int* curr = data;
        int* end  = data+len;
        int* _other = other.data;

        for (; curr != end; ++curr, ++_other) {
            if (*curr != *_other)
                return false;
            }
        return true;
        }
};

#else
typedef typename std::vector<set_types> IndexVector;
#endif


class ConcreteIndexedVariable
{
protected:

    void collect_args(size_t i, bool& refflag, const int& arg)
        {
        if (refflag)
            reftmp[i++] = arg;
        else
            tmp[i++] = arg;
        }

    void collect_args(size_t i, bool& refflag, const Expression& arg)
        {
        if (!refflag) {
            for (size_t j=0; j<i; j++)
                reftmp[j] = tmp[j];
            refflag=true;
            }
        reftmp[i++] = arg.repn;
        }

    void collect_args(size_t i, bool& refflag, const IndexParameter& arg)
        {
        if (!refflag) {
            for (size_t j=0; j<i; j++)
                reftmp[j] = tmp[j];
            refflag=true;
            }
        Expression e = arg;
        reftmp[i++] = e.repn;
        }

    template <typename... ARGTYPES>
    void collect_args(size_t i, bool& refflag, const int& arg, const ARGTYPES&... args)
        {
        if (refflag)
            reftmp[i++] = arg;
        else
            tmp[i++] = arg;
        collect_args(i, refflag, args...);
        }

    template <typename... ARGTYPES>
    void collect_args(size_t i, bool& refflag, const Expression& arg, const ARGTYPES&... args)
        {
        if (!refflag) {
            for (size_t j=0; j<i; j++)
                reftmp[j] = tmp[j];
            refflag=true;
            }
        reftmp[i++] = arg.repn;
        collect_args(i, refflag, args...);
        }

    template <typename... ARGTYPES>
    void collect_args(size_t i, bool& refflag, const IndexParameter& arg, const ARGTYPES&... args)
        {
        if (!refflag) {
            for (size_t j=0; j<i; j++)
                reftmp[j] = tmp[j];
            refflag=true;
            }
        Expression e = arg;
        reftmp[i++] = e.repn;
        collect_args(i, refflag, args...);
        }

public:

    std::shared_ptr<ConcreteIndexedVariableRepn> repn;
    IndexVector tmp;
    std::vector<refarg_types> reftmp;

public:

    ConcreteIndexedVariable(const ConcreteSet& arg);
    ConcreteIndexedVariable(const ConcreteSet& arg, const std::string& name);
    ~ConcreteIndexedVariable() {}

    /** Set the initial variable value. \returns the variable object. */
    ConcreteIndexedVariable& value(double value);
    /** Set the initial variable value. \returns the variable object. */
    ConcreteIndexedVariable& value(const Expression& value);

    /** Set the lower bound. \returns the variable object. */
    ConcreteIndexedVariable& lower(double value);
    /** Set the lower bound. \returns the variable object. */
    ConcreteIndexedVariable& lower(const Expression& value);

    /** Set the upper bound. \returns the variable object. */
    ConcreteIndexedVariable& upper(double value);
    /** Set the upper bound. \returns the variable object. */
    ConcreteIndexedVariable& upper(const Expression& value);

    /** Set the name of the variable. \returns the variable object */
    ConcreteIndexedVariable& name(const std::string& name);

    /** Set the variable type. \returns the variable object */
    ConcreteIndexedVariable& within(VariableTypes vtype);

    std::vector<Variable>::iterator begin();
    std::vector<Variable>::iterator end();

    size_t size() const;

    template <typename... ARGTYPES>
    Expression operator()(const ARGTYPES&... args)
        {
        bool refflag=false;
        collect_args(static_cast<size_t>(0), refflag, args...);

        if (refflag)
            return create_varref(reftmp);
        else 
            return index(tmp);
        }

    Expression index(const IndexVector& args);
    Expression create_varref(const std::vector<refarg_types>& indices);
};


class AbstractIndexedVariable
{
public:

    std::string name;
    AbstractSet abstract_set;

public:

    AbstractIndexedVariable(const AbstractSet& _arg, const std::string& _name);

    ConcreteIndexedVariable initialize();

    template <typename ARGTYPE>
    void collect_args(std::vector<refarg_types>& _arg, const ARGTYPE& arg)
        {
        _arg.emplace_back(arg);
        }

    void collect_args(std::vector<refarg_types>& _arg, const Expression& arg)
        {
        _arg.emplace_back(arg.repn);
        }

    template <typename ARGTYPE, typename... ARGTYPES>
    void collect_args(std::vector<refarg_types>& _arg, const ARGTYPE& arg, const ARGTYPES&... args)
        {
        _arg.emplace_back(arg);
        collect_args(_arg, args...);
        }

    template <typename... ARGTYPES>
    void collect_args(std::vector<refarg_types>& _arg, const Expression& arg, const ARGTYPES&... args)
        {
        _arg.emplace_back(arg.repn);
        collect_args(_arg, args...);
        }

    template <typename... ARGTYPES>
    Expression operator()(const ARGTYPES&... args)
        {
        std::vector<refarg_types> arg;
        collect_args(arg, args...);
        return index(arg);
        }

    Expression index(const std::vector<refarg_types>& args);
};


//
// NOTE: The other variable() function declarations are in expression.hpp
//
// These need to be consolidated.
//
/*
AbstractIndexedVariable variable(const std::string& name, const AbstractSet& arg);

ConcreteIndexedVariable variable(const ConcreteSet& arg);
ConcreteIndexedVariable variable(const std::string& name, const ConcreteSet& arg);

ConcreteIndexedVariable variable(size_t n);
ConcreteIndexedVariable variable(const std::string& name, size_t n);

ConcreteIndexedVariable variable(const std::vector<size_t>& dim);
ConcreteIndexedVariable variable(const std::string& name, const std::vector<size_t>& n);
*/

}
