#pragma once

#include <cassert>
#include <initializer_list>
#include <vector>

#include "coek/util/template_utils.hpp"
#include "variable_assoc_array.hpp"

namespace coek {

class VariableArrayRepn;

class VariableArray : public VariableAssocArray {
   public:
    std::shared_ptr<VariableArrayRepn> repn;
    std::shared_ptr<VariableAssocArrayRepn> get_repn();
    const std::shared_ptr<VariableAssocArrayRepn> get_repn() const;

    Variable index(const IndexVector& args);
    void index_error(size_t i);

    using iterator = std::vector<Variable>::iterator;
    using const_iterator = std::vector<Variable>::const_iterator;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;
    iterator begin() noexcept;
    iterator end() noexcept;

    VariableArray& generate_names();

   public:
    /// Collect arguments with references

    void collect_refargs(size_t i, size_t arg) { reftmp[i] = static_cast<int>(arg); }

    void collect_refargs(size_t i, int arg)
    {
        assert(arg >= 0);
        reftmp[i] = arg;
    }

    void collect_refargs(size_t i, const Expression& arg) { reftmp[i] = arg.repn; }

    void collect_refargs(size_t i, const IndexParameter& arg)
    {
        Expression e = arg;
        reftmp[i] = e.repn;
    }

    template <typename... ARGTYPES>
    void collect_refargs(size_t i, size_t arg, const ARGTYPES&... args)
    {
        reftmp[i] = static_cast<int>(arg);
        collect_refargs(i + 1, args...);
    }

    template <typename... ARGTYPES>
    void collect_refargs(size_t i, int arg, const ARGTYPES&... args)
    {
        assert(arg >= 0);
        reftmp[i] = arg;
        collect_refargs(i + 1, args...);
    }

    template <typename... ARGTYPES>
    void collect_refargs(size_t i, const Expression& arg, const ARGTYPES&... args)
    {
        reftmp[i] = arg.repn;
        collect_refargs(i + 1, args...);
    }

    template <typename... ARGTYPES>
    void collect_refargs(size_t i, const IndexParameter& arg, const ARGTYPES&... args)
    {
        Expression e = arg;
        reftmp[i] = e.repn;
        collect_refargs(i + 1, args...);
    }

    /// Collect arguments with int or size_t indices

    void collect_args(size_t i, size_t arg) { tmp[i] = static_cast<int>(arg); }

    void collect_args(size_t i, int arg)
    {
        assert(arg >= 0);  // TODO - Resolve coverage for this assertion
        tmp[i] = arg;
    }

    template <typename... ARGTYPES>
    void collect_args(size_t i, size_t arg, const ARGTYPES&... args)
    {
        tmp[i] = static_cast<int>(arg);
        collect_args(i + 1, args...);
    }

    template <typename... ARGTYPES>
    void collect_args(size_t i, int arg, const ARGTYPES&... args)
    {
        assert(arg >= 0);
        tmp[i] = arg;
        collect_args(i + 1, args...);
    }

#ifdef COEK_WITH_COMPACT_MODEL
    template <typename... ARGTYPES>
    typename std::enable_if<has_nonintegral_args<ARGTYPES...>::value, Expression>::type operator()(
        const ARGTYPES&... args)
    {
        const size_t nargs = count_args(args...);
        if (dim() != nargs)
            index_error(nargs);
        collect_refargs(static_cast<size_t>(0), args...);
        return create_ref(reftmp);
    }
#endif

    template <typename... ARGTYPES>
    typename std::enable_if<!has_nonintegral_args<ARGTYPES...>::value, Variable>::type operator()(
        const ARGTYPES&... args)
    {
        const size_t nargs = count_args(args...);
        if (dim() != nargs)
            index_error(nargs);
        collect_args(static_cast<size_t>(0), args...);
        return index(tmp);
    }

    Variable operator()(size_t i)
    {
        if (dim() != 1)
            index_error(1);
        tmp[0] = static_cast<int>(i);
        return index(tmp);
    }

    Variable operator()(size_t i, size_t j)
    {
        if (dim() != 2)
            index_error(2);
        tmp[0] = static_cast<int>(i);
        tmp[1] = static_cast<int>(j);
        return index(tmp);
    }
#ifdef __cpp_multidimensional_subscript
    Variable operator[](size_t i) { return operator()(i); }

    Variable operator[](size_t i, size_t j) { return operator()(i, j); }
#endif

   public:
    VariableArray(size_t n);
    VariableArray(const std::vector<size_t>& shape);
    // VariableArray(const std::vector<int>& shape);
    VariableArray(const std::initializer_list<size_t>& shape);
    virtual ~VariableArray() {}

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

    /** Set a flag indicating if the variables are fixed. \returns the variable array object. */
    VariableArray& fixed(bool flag);

    /** Set the name of the variable. \returns the variable object */
    VariableArray& name(const std::string& name);
    /** Get the name of the variable. \returns the variable string name */
    std::string name() const;

    /** Set the variable type. \returns the variable object */
    VariableArray& within(VariableTypes vtype);
};

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
