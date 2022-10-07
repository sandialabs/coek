#pragma once

#include <cassert>
#include <initializer_list>
#include <variant>

#include "coek_sets.hpp"

#define USING_INDEXVECTOR

namespace coek {

typedef BaseExpressionTerm* expr_pointer_t;
// typedef std::variant<int,double,std::string,expr_pointer_t> refarg_types;
typedef std::variant<int, expr_pointer_t> refarg_types;
class VariableAssocArrayRepn;

#ifdef USING_INDEXVECTOR
class IndexVector {
   public:
    typedef int vecdata_t;
    size_t len;
    vecdata_t* data;

   public:
    // The vector length is stored in the first element.  This limits the size, but this is OK for
    // small vectors
    IndexVector() : len(0), data(0) {}

    explicit IndexVector(vecdata_t* _data) : len(static_cast<size_t>(_data[0])), data(_data + 1) {}

    explicit IndexVector(vecdata_t* _data, size_t _len) : len(_len), data(_data) {}

    IndexVector(const IndexVector& other) : len(other.len), data(other.data) {}

    IndexVector& operator=(const IndexVector& other)
    {
        len = other.len;
        data = other.data;
        return *this;
    }

    size_t size() const { return len; }

    vecdata_t& operator[](size_t i) { return data[i]; }

    const vecdata_t& operator[](size_t i) const { return data[i]; }

    bool operator<(const IndexVector& other) const
    {
        for (size_t i = 0; i < len; i++) {
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
        vecdata_t* curr = data;
        vecdata_t* end = data + len;
        vecdata_t* _other = other.data;

        for (; curr != end; ++curr, ++_other) {
            if (*curr != *_other) return false;
        }
        return true;
    }
};

#else
typedef typename std::vector<set_types> IndexVector;
#endif

class VariableAssocArray {
   public:
    std::shared_ptr<VariableAssocArrayRepn> repn;
    IndexVector tmp;
    std::vector<refarg_types> reftmp;

   public:
    std::vector<Variable>::iterator begin();
    std::vector<Variable>::iterator end();

    size_t size() const;

    Expression index(const IndexVector& args);
    Expression create_varref(const std::vector<refarg_types>& indices);
};

class VariableMap : public VariableAssocArray {
   public:
    /// Collect arguments with references

    bool collect_refargs(size_t i, size_t arg)
    {
        reftmp[i] = static_cast<int>(arg);
        return true;
    }

    bool collect_refargs(size_t i, int arg)
    {
        assert(arg >= 0);
        reftmp[i] = arg;
        return true;
    }

    bool collect_refargs(size_t i, const Expression& arg)
    {
        reftmp[i] = arg.repn;
        return true;
    }

    bool collect_refargs(size_t i, const IndexParameter& arg)
    {
        Expression e = arg;
        reftmp[i] = e.repn;
        return true;
    }

    template <typename... ARGTYPES>
    bool collect_refargs(size_t i, size_t arg, const ARGTYPES&... args)
    {
        collect_refargs(i + 1, args...);
        reftmp[i] = static_cast<int>(arg);
        return true;
    }

    template <typename... ARGTYPES>
    bool collect_refargs(size_t i, int arg, const ARGTYPES&... args)
    {
        assert(arg >= 0);
        collect_refargs(i + 1, args...);
        reftmp[i] = arg;
        return true;
    }

    template <typename... ARGTYPES>
    bool collect_refargs(size_t i, const Expression& arg, const ARGTYPES&... args)
    {
        collect_refargs(i + 1, args...);
        reftmp[i] = arg.repn;
        return true;
    }

    template <typename... ARGTYPES>
    bool collect_refargs(size_t i, const IndexParameter& arg, const ARGTYPES&... args)
    {
        collect_refargs(i + 1, args...);
        Expression e = arg;
        reftmp[i] = e.repn;
        return true;
    }

    /// Collect arguments with size_t indices

    bool collect_args(size_t i, size_t arg)
    {
        tmp[i] = static_cast<int>(arg);
        return false;
    }

    bool collect_args(size_t i, int arg)
    {
        assert(arg >= 0);
        tmp[i] = arg;
        return false;
    }

    bool collect_args(size_t i, const Expression& arg)
    {
        reftmp[i] = arg.repn;
        return true;
    }

    bool collect_args(size_t i, const IndexParameter& arg)
    {
        Expression e = arg;
        reftmp[i] = e.repn;
        return true;
    }

    template <typename... ARGTYPES>
    bool collect_args(size_t i, size_t arg, const ARGTYPES&... args)
    {
        if (collect_args(i + 1, args...)) {
            reftmp[i] = arg;
            return true;
        }
        else {
            tmp[i] = static_cast<int>(arg);
            return false;
        }
    }

    template <typename... ARGTYPES>
    bool collect_args(size_t i, int arg, const ARGTYPES&... args)
    {
        assert(arg >= 0);
        if (collect_args(i + 1, args...)) {
            reftmp[i] = arg;
            return true;
        }
        else {
            tmp[i] = arg;
            return false;
        }
    }

    template <typename... ARGTYPES>
    bool collect_args(size_t i, const Expression& arg, const ARGTYPES&... args)
    {
        collect_refargs(i + 1, args...);
        reftmp[i] = arg.repn;
        return true;
    }

    template <typename... ARGTYPES>
    bool collect_args(size_t i, const IndexParameter& arg, const ARGTYPES&... args)
    {
        collect_refargs(i + 1, args...);
        Expression e = arg;
        reftmp[i] = e.repn;
        return true;
    }

    template <typename... ARGTYPES>
    Expression operator()(const ARGTYPES&... args)
    {
        if (collect_args(static_cast<size_t>(0), args...))
            return create_varref(reftmp);
        else
            return index(tmp);
    }

    Expression operator()(int i)
    {
        tmp[0] = i;
        return index(tmp);
    }

    Expression operator()(int i, int j)
    {
        tmp[0] = i;
        tmp[1] = j;
        return index(tmp);
    }

   public:
    VariableMap(const ConcreteSet& arg);
    ~VariableMap() {}

    /** Set the initial variable value. \returns the variable object. */
    VariableMap& value(double value);
    /** Set the initial variable value. \returns the variable object. */
    VariableMap& value(const Expression& value);

    /** Set the lower bound. \returns the variable object. */
    VariableMap& lower(double value);
    /** Set the lower bound. \returns the variable object. */
    VariableMap& lower(const Expression& value);

    /** Set the upper bound. \returns the variable object. */
    VariableMap& upper(double value);
    /** Set the upper bound. \returns the variable object. */
    VariableMap& upper(const Expression& value);

    /** Set the upper and lower bounds. \returns the variable object. */
    VariableMap& bounds(double lb, double ub);
    /** Set the upper and lower bounds. \returns the variable object. */
    VariableMap& bounds(const Expression& lb, double ub);
    /** Set the upper and lower bounds. \returns the variable object. */
    VariableMap& bounds(double lb, const Expression& ub);
    /** Set the upper and lower bounds. \returns the variable object. */
    VariableMap& bounds(const Expression& lb, const Expression& ub);

    /** Set the name of the variable. \returns the variable object */
    VariableMap& name(const std::string& name);

    /** Set the variable type. \returns the variable object */
    VariableMap& within(VariableTypes vtype);
};

class VariableArray : public VariableAssocArray {
   public:
    /// Collect arguments with references

    bool collect_refargs(size_t i, size_t arg)
    {
        reftmp[i] = static_cast<int>(arg);
        return true;
    }

    bool collect_refargs(size_t i, int arg)
    {
        assert(arg >= 0);
        reftmp[i] = arg;
        return true;
    }

    bool collect_refargs(size_t i, const Expression& arg)
    {
        reftmp[i] = arg.repn;
        return true;
    }

    bool collect_refargs(size_t i, const IndexParameter& arg)
    {
        Expression e = arg;
        reftmp[i] = e.repn;
        return true;
    }

    template <typename... ARGTYPES>
    bool collect_refargs(size_t i, size_t arg, const ARGTYPES&... args)
    {
        collect_refargs(i + 1, args...);
        reftmp[i] = static_cast<int>(arg);
        return true;
    }

    template <typename... ARGTYPES>
    bool collect_refargs(size_t i, int arg, const ARGTYPES&... args)
    {
        assert(arg >= 0);
        collect_refargs(i + 1, args...);
        reftmp[i] = arg;
        return true;
    }

    template <typename... ARGTYPES>
    bool collect_refargs(size_t i, const Expression& arg, const ARGTYPES&... args)
    {
        collect_refargs(i + 1, args...);
        reftmp[i] = arg.repn;
        return true;
    }

    template <typename... ARGTYPES>
    bool collect_refargs(size_t i, const IndexParameter& arg, const ARGTYPES&... args)
    {
        collect_refargs(i + 1, args...);
        Expression e = arg;
        reftmp[i] = e.repn;
        return true;
    }

    /// Collect arguments with size_t indices

    bool collect_args(size_t i, size_t arg)
    {
        tmp[i] = static_cast<int>(arg);
        return false;
    }

    bool collect_args(size_t i, int arg)
    {
        assert(arg >= 0);
        tmp[i] = arg;
        return false;
    }

    bool collect_args(size_t i, const Expression& arg)
    {
        reftmp[i] = arg.repn;
        return true;
    }

    bool collect_args(size_t i, const IndexParameter& arg)
    {
        Expression e = arg;
        reftmp[i] = e.repn;
        return true;
    }

    template <typename... ARGTYPES>
    bool collect_args(size_t i, size_t arg, const ARGTYPES&... args)
    {
        if (collect_args(i + 1, args...)) {
            reftmp[i] = arg;
            return true;
        }
        else {
            tmp[i] = static_cast<int>(arg);
            return false;
        }
    }

    template <typename... ARGTYPES>
    bool collect_args(size_t i, int arg, const ARGTYPES&... args)
    {
        assert(arg >= 0);
        if (collect_args(i + 1, args...)) {
            reftmp[i] = arg;
            return true;
        }
        else {
            tmp[i] = arg;
            return false;
        }
    }

    template <typename... ARGTYPES>
    bool collect_args(size_t i, const Expression& arg, const ARGTYPES&... args)
    {
        collect_refargs(i + 1, args...);
        reftmp[i] = arg.repn;
        return true;
    }

    template <typename... ARGTYPES>
    bool collect_args(size_t i, const IndexParameter& arg, const ARGTYPES&... args)
    {
        collect_refargs(i + 1, args...);
        Expression e = arg;
        reftmp[i] = e.repn;
        return true;
    }

    template <typename... ARGTYPES>
    Expression operator()(const ARGTYPES&... args)
    {
        if (collect_args(static_cast<size_t>(0), args...))
            return create_varref(reftmp);
        else
            return index(tmp);
    }

    Expression operator()(size_t i)
    {
        tmp[0] = static_cast<int>(i);
        return index(tmp);
    }

    Expression operator()(size_t i, size_t j)
    {
        tmp[0] = static_cast<int>(i);
        tmp[1] = static_cast<int>(j);
        return index(tmp);
    }

   public:
    VariableArray(const ConcreteSet& arg);
    ~VariableArray() {}

    /** Set the initial variable value. \returns the variable object. */
    VariableArray& value(double value);
    /** Set the initial variable value. \returns the variable object. */
    VariableArray& value(const Expression& value);

    /** Set the lower bound. \returns the variable object. */
    VariableArray& lower(double value);
    /** Set the lower bound. \returns the variable object. */
    VariableArray& lower(const Expression& value);

    /** Set the upper bound. \returns the variable object. */
    VariableArray& upper(double value);
    /** Set the upper bound. \returns the variable object. */
    VariableArray& upper(const Expression& value);

    /** Set the upper and lower bounds. \returns the variable object. */
    VariableArray& bounds(double lb, double ub);
    /** Set the upper and lower bounds. \returns the variable object. */
    VariableArray& bounds(const Expression& lb, double ub);
    /** Set the upper and lower bounds. \returns the variable object. */
    VariableArray& bounds(double lb, const Expression& ub);
    /** Set the upper and lower bounds. \returns the variable object. */
    VariableArray& bounds(const Expression& lb, const Expression& ub);

    /** Set the name of the variable. \returns the variable object */
    VariableArray& name(const std::string& name);

    /** Set the variable type. \returns the variable object */
    VariableArray& within(VariableTypes vtype);
};

class AbstractIndexedVariable {
   public:
    std::string name;
    AbstractSet abstract_set;

   public:
    AbstractIndexedVariable(const AbstractSet& _arg, const std::string& _name);

    VariableMap initialize();

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
    void collect_args(std::vector<refarg_types>& _arg, const Expression& arg,
                      const ARGTYPES&... args)
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
*/

// variable map

VariableMap variable(const ConcreteSet& arg);
inline VariableMap variable_map(const ConcreteSet& arg) { return variable(arg); }

inline VariableMap variable(const std::string& name, const ConcreteSet& arg)
{
    return variable(arg).name(name);
}
inline VariableMap variable_map(const std::string& name, const ConcreteSet& arg)
{
    return variable(arg).name(name);
}

// variable array

VariableArray variable(size_t n);
VariableArray variable(const std::vector<size_t>& shape);
VariableArray variable(const std::initializer_list<size_t>& shape);

inline VariableArray variable_array(size_t n) { return variable(n); }
inline VariableArray variable_array(const std::vector<size_t>& shape) { return variable(shape); }
inline VariableArray variable_array(const std::initializer_list<size_t>& shape)
{
    return variable(shape);
}

inline VariableArray variable(const std::string& name, size_t n) { return variable(n).name(name); }
inline VariableArray variable(const std::string& name, const std::vector<size_t>& shape)
{
    return variable(shape).name(name);
}
inline VariableArray variable(const std::string& name, const std::initializer_list<size_t>& shape)
{
    return variable(shape).name(name);
}

inline VariableArray variable_array(const std::string& name, size_t n)
{
    return variable(n).name(name);
}
inline VariableArray variable_array(const std::string& name, const std::vector<size_t>& shape)
{
    return variable(shape).name(name);
}
inline VariableArray variable_array(const std::string& name,
                                    const std::initializer_list<size_t>& shape)
{
    return variable(shape).name(name);
}

}  // namespace coek
