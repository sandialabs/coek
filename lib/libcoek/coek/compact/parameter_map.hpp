#pragma once

#include "coek_sets.hpp"
#include "coek/util/template_utils.hpp"
#include "coek/api/parameter_assoc_array.hpp"


namespace coek {

class ParameterMapRepn;


class ParameterMap : public ParameterAssocArray
{
public:

    std::shared_ptr<ParameterMapRepn> repn;
    ParameterAssocArrayRepn* get_repn();

    Parameter index(const IndexVector& args);
    void index_error(size_t i);

public:

    /// Collect arguments with references

    void collect_refargs(size_t i, size_t arg)
        { reftmp[i] = static_cast<int>(arg); }

    void collect_refargs(size_t i, int arg)
        { reftmp[i] = arg; }

    void collect_refargs(size_t i, const Expression& arg)
        { reftmp[i] = arg.repn; }

    void collect_refargs(size_t i, const IndexParameter& arg)
        {
        Expression e = arg;
        reftmp[i] = e.repn;
        }

    template <typename... ARGTYPES>
    void collect_refargs(size_t i, size_t arg, const ARGTYPES&... args)
        {
        reftmp[i] = static_cast<int>(arg);
        collect_refargs(i+1, args...);
        }

    template <typename... ARGTYPES>
    void collect_refargs(size_t i, int arg, const ARGTYPES&... args)
        {
        reftmp[i] = arg;
        collect_refargs(i+1, args...);
        }

    template <typename... ARGTYPES>
    void collect_refargs(size_t i, const Expression& arg, const ARGTYPES&... args)
        {
        reftmp[i] = arg.repn;
        collect_refargs(i+1, args...);
        }

    template <typename... ARGTYPES>
    void collect_refargs(size_t i, const IndexParameter& arg, const ARGTYPES&... args)
        {
        Expression e = arg;
        reftmp[i] = e.repn;
        collect_refargs(i+1, args...);
        }

    /// Collect arguments with int or size_t indices

    void collect_args(size_t i, size_t arg)
        { tmp[i] = static_cast<int>(arg); }

    void collect_args(size_t i, int arg)
        { tmp[i] = arg; }

    template <typename... ARGTYPES>
    void collect_args(size_t i, size_t arg, const ARGTYPES&... args)
        {
        tmp[i] = static_cast<int>(arg);
        collect_args(i+1, args...);
        }

    template <typename... ARGTYPES>
    void collect_args(size_t i, int arg, const ARGTYPES&... args)
        {
        tmp[i] = arg;
        collect_args(i+1, args...);
        }

    template <typename... ARGTYPES>
    typename std::enable_if<has_nonintegral_args<ARGTYPES...>::value,Expression>::type
    operator()(const ARGTYPES&... args)
        {
        const size_t nargs = count_args(args...);
        if (dim() != nargs)
            index_error(nargs);
        collect_refargs(static_cast<size_t>(0), args...);
        return create_paramref(reftmp);
        }

    template <typename... ARGTYPES>
    typename std::enable_if<!has_nonintegral_args<ARGTYPES...>::value,Parameter>::type
    operator()(const ARGTYPES&... args)
        {
        const size_t nargs = count_args(args...);
        if (dim() != nargs)
            index_error(nargs);
        collect_args(static_cast<size_t>(0), args...);
        return index(tmp);
        }

    Parameter operator()(int i)
        {
        if (dim() != 1)
            index_error(1);
        tmp[0] = i;
        return index(tmp);
        }

    Parameter operator()(int i, int j)
        {
        if (dim() != 2)
            index_error(2);
        tmp[0] = i;
        tmp[1] = j;
        return index(tmp);
        }

public:

    explicit ParameterMap(const ConcreteSet& arg);
    ~ParameterMap() {}

    /** Set the initial parameter value. \returns the parameter object. */
    ParameterMap& value(double value);
    /** Set the initial parameter value. \returns the parameter object. */
    ParameterMap& value(const Expression& value);

    /** Set the name of the parameter. \returns the parameter object */
    ParameterMap& name(const std::string& name);

};


ParameterMap parameter(const ConcreteSet& arg);
inline ParameterMap parameter_map(const ConcreteSet& arg)
{ return parameter(arg); }

inline ParameterMap parameter(const std::string& name, const ConcreteSet& arg)
{ return parameter(arg).name(name); }
inline ParameterMap parameter_map(const std::string& name, const ConcreteSet& arg)
{ return parameter(arg).name(name); }

}
