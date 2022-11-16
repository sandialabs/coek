#pragma once

#include <cassert>
#include <initializer_list>
#include <vector>

#include "coek/util/template_utils.hpp"
#include "parameter_assoc_array.hpp"

namespace coek {

class ParameterArrayRepn;

class ParameterArray : public ParameterAssocArray {
   public:
    std::shared_ptr<ParameterArrayRepn> repn;
    ParameterAssocArrayRepn* get_repn();

    Parameter index(const IndexVector& args);
    void index_error(size_t i);

    ParameterArray& generate_names();

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
        assert(arg >= 0);
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

    template <typename... ARGTYPES>
    typename std::enable_if<has_nonintegral_args<ARGTYPES...>::value, Expression>::type operator()(
        const ARGTYPES&... args)
    {
        const size_t nargs = count_args(args...);
        if (dim() != nargs) index_error(nargs);
        collect_refargs(static_cast<size_t>(0), args...);
        return create_paramref(reftmp);
    }

    template <typename... ARGTYPES>
    typename std::enable_if<!has_nonintegral_args<ARGTYPES...>::value, Parameter>::type operator()(
        const ARGTYPES&... args)
    {
        const size_t nargs = count_args(args...);
        if (dim() != nargs) index_error(nargs);
        collect_args(static_cast<size_t>(0), args...);
        return index(tmp);
    }

    Parameter operator()(size_t i)
    {
        if (dim() != 1) index_error(1);
        tmp[0] = static_cast<int>(i);
        return index(tmp);
    }

    Parameter operator()(size_t i, size_t j)
    {
        if (dim() != 2) index_error(2);
        tmp[0] = static_cast<int>(i);
        tmp[1] = static_cast<int>(j);
        return index(tmp);
    }

   public:
    ParameterArray(size_t n);
    ParameterArray(const std::vector<size_t>& shape);
    ParameterArray(const std::initializer_list<size_t>& shape);
    ~ParameterArray() {}

    /** Set the initial parameter value. \returns the parameter object. */
    ParameterArray& value(double value);
    /** Set the initial parameter value. \returns the parameter object. */
    ParameterArray& value(const Expression& value);

    /** Set the name of the parameter. \returns the parameter object */
    ParameterArray& name(const std::string& name);
};

ParameterArray parameter(size_t n);
ParameterArray parameter(const std::vector<size_t>& shape);
ParameterArray parameter(const std::initializer_list<size_t>& shape);

inline ParameterArray parameter_array(size_t n) { return parameter(n); }
inline ParameterArray parameter_array(const std::vector<size_t>& shape) { return parameter(shape); }
inline ParameterArray parameter_array(const std::initializer_list<size_t>& shape)
{
    return parameter(shape);
}

inline ParameterArray parameter(const std::string& name, size_t n)
{
    return parameter(n).name(name);
}
inline ParameterArray parameter(const std::string& name, const std::vector<size_t>& shape)
{
    return parameter(shape).name(name);
}
inline ParameterArray parameter(const std::string& name, const std::initializer_list<size_t>& shape)
{
    return parameter(shape).name(name);
}

inline ParameterArray parameter_array(const std::string& name, size_t n)
{
    return parameter(n).name(name);
}
inline ParameterArray parameter_array(const std::string& name, const std::vector<size_t>& shape)
{
    return parameter(shape).name(name);
}
inline ParameterArray parameter_array(const std::string& name,
                                      const std::initializer_list<size_t>& shape)
{
    return parameter(shape).name(name);
}

}  // namespace coek
