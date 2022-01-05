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

    int* data;

public:

    IndexVector()
        : data(0)
        {}

    IndexVector(int* _data)
        : data(_data)
        {}

    IndexVector(const IndexVector& other)
        : data(other.data)
        {}

    // The vector length is stored in the first element.  This limits the size, but this is OK for small vectors
    size_t size() const
        { return static_cast<size_t>(*data); }

    int& operator[](size_t i)
        { return data[i+1]; }

    const int& operator[](size_t i) const
        { return data[i+1]; }

    bool operator<(const IndexVector& other) const
        {
        for (int i=1; i<=*data; i++) {
            if (data[i] < other.data[i])
                return true;
            else if (data[i] > other.data[i])
                return false;
            }
        return true;
        }

    bool operator==(const IndexVector& other) const
        {
        if (*data == 1)
            return data[1] == other.data[1];
        else if (*data==2) {
            return (data[1] == other.data[1]) and (data[2] == other.data[2]);
            }
        int* curr = data+1;
        int* end  = data+1+*data;
        int* _other = other.data+1;

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
public:

    std::shared_ptr<ConcreteIndexedVariableRepn> repn;
    IndexVector tmp;
    std::vector<refarg_types> reftmp;

public:

    ConcreteIndexedVariable(const ConcreteSet& arg, double lb=-COEK_INFINITY, double ub=COEK_INFINITY, double value=0.0);
    ConcreteIndexedVariable(const ConcreteSet& arg, double lb, double ub, double value, const std::string& name);
    ConcreteIndexedVariable(const ConcreteSet& arg, double lb, double ub, double value, bool binary, bool integer, const std::string& name);
    ConcreteIndexedVariable(const ConcreteSet& arg, double lb, double ub, double value, bool binary, bool integer);
    ~ConcreteIndexedVariable() {}

    std::vector<Variable>::iterator begin();
    std::vector<Variable>::iterator end();

    size_t size() const;

    void collect_args(unsigned int i, bool& refflag, const int& arg)
        {
        if (refflag)
            reftmp[i++] = arg;
        else
            tmp[i++] = arg;
        }

    void collect_args(unsigned int i, bool& refflag, const Expression& arg)
        {
        if (!refflag) {
            for (unsigned int j=0; j<i; j++)
                reftmp[j] = tmp[j];
            refflag=true;
            }
        reftmp[i++] = arg.repn;
        }

    void collect_args(unsigned int i, bool& refflag, const IndexParameter& arg)
        {
        if (!refflag) {
            for (unsigned int j=0; j<i; j++)
                reftmp[j] = tmp[j];
            refflag=true;
            }
        Expression e = arg;
        reftmp[i++] = e.repn;
        }

    template <typename... ARGTYPES>
    void collect_args(unsigned int i, bool& refflag, const int& arg, const ARGTYPES&... args)
        {
        if (refflag)
            reftmp[i++] = arg;
        else
            tmp[i++] = arg;
        collect_args(i, refflag, args...);
        }

    template <typename... ARGTYPES>
    void collect_args(unsigned int i, bool& refflag, const Expression& arg, const ARGTYPES&... args)
        {
        if (!refflag) {
            for (unsigned int j=0; j<i; j++)
                reftmp[j] = tmp[j];
            refflag=true;
            }
        reftmp[i++] = arg.repn;
        collect_args(i, refflag, args...);
        }

    template <typename... ARGTYPES>
    void collect_args(unsigned int i, bool& refflag, const IndexParameter& arg, const ARGTYPES&... args)
        {
        if (!refflag) {
            for (unsigned int j=0; j<i; j++)
                reftmp[j] = tmp[j];
            refflag=true;
            }
        Expression e = arg;
        reftmp[i++] = e.repn;
        collect_args(i, refflag, args...);
        }

    template <typename... ARGTYPES>
    Expression operator()(const ARGTYPES&... args)
        {
        bool refflag=false;
        collect_args(0, refflag, args...);

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


AbstractIndexedVariable IndexedVariable(const AbstractSet& arg, const std::string& name);

ConcreteIndexedVariable IndexedVariable(const ConcreteSet& arg, double lb=-COEK_INFINITY, double ub=COEK_INFINITY, double value=0.0);
ConcreteIndexedVariable IndexedVariable(const ConcreteSet& arg, const std::string& name);
ConcreteIndexedVariable IndexedVariable(const ConcreteSet& arg, double lb, double ub, double value, const std::string& name);
ConcreteIndexedVariable IndexedVariable(const ConcreteSet& arg, double lb, double ub, double value, bool binary, bool integer, const std::string& name);
ConcreteIndexedVariable IndexedVariable(const ConcreteSet& arg, double lb, double ub, double value, bool binary, bool integer);

}
