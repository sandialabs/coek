#pragma once

#include "coek/api/variable_assoc_array.hpp"
#include "coek/compact/sequence_context.hpp"
#include "coek/util/template_utils.hpp"
#include "coek_sets.hpp"

namespace coek {

class VariableMapRepn;

class VariableMap : public VariableAssocArray {
   public:
    std::shared_ptr<VariableMapRepn> repn;
    std::shared_ptr<VariableAssocArrayRepn> get_repn();
    const std::shared_ptr<VariableAssocArrayRepn> get_repn() const;

    Variable index(const IndexVector& args);
    void index_error(size_t i);

    VariableMap& generate_names();

   public:
    /// Collect arguments with references

    void collect_refargs(size_t i, size_t arg) { reftmp[i] = static_cast<int>(arg); }

    void collect_refargs(size_t i, int arg) { reftmp[i] = arg; }

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

    void collect_args(size_t i, int arg) { tmp[i] = arg; }

    template <typename... ARGTYPES>
    void collect_args(size_t i, size_t arg, const ARGTYPES&... args)
    {
        tmp[i] = static_cast<int>(arg);
        collect_args(i + 1, args...);
    }

    template <typename... ARGTYPES>
    void collect_args(size_t i, int arg, const ARGTYPES&... args)
    {
        tmp[i] = arg;
        collect_args(i + 1, args...);
    }

    template <typename... ARGTYPES>
    typename std::enable_if<has_nonintegral_args<ARGTYPES...>::value, Expression>::type operator()(
        const ARGTYPES&... args)
    {
        const size_t nargs = count_args(args...);
        if (dim() != nargs)
            index_error(nargs);
        collect_refargs(static_cast<size_t>(0), args...);
        return create_varref(reftmp);
    }

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

    Variable operator()(int i)
    {
        if (dim() != 1)
            index_error(1);
        tmp[0] = i;
        return index(tmp);
    }

    Variable operator()(int i, int j)
    {
        if (dim() != 2)
            index_error(2);
        tmp[0] = i;
        tmp[1] = j;
        return index(tmp);
    }

   public:
    explicit VariableMap(const ConcreteSet& arg);
    explicit VariableMap(const SequenceContext& arg);
    virtual ~VariableMap() {}

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

VariableMap variable(const SequenceContext& arg);
inline VariableMap variable_map(const SequenceContext& arg) { return variable(arg); }

inline VariableMap variable(const std::string& name, const SequenceContext& arg)
{
    return variable(arg).name(name);
}
inline VariableMap variable_map(const std::string& name, const SequenceContext& arg)
{
    return variable(arg).name(name);
}

}  // namespace coek
