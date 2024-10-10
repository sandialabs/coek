#pragma once

#include "coek/api/parameter_assoc_array.hpp"
#include "coek/compact/sequence_context.hpp"
#include "coek/util/template_utils.hpp"
#include "coek/util/tuple_utils.hpp"
#include "coek_sets.hpp"

namespace coek {

class ParameterMapRepn;

class ParameterMap : public ParameterAssocArray {
   public:
    std::shared_ptr<ParameterMapRepn> repn;
    std::shared_ptr<ParameterAssocArrayRepn> get_repn();
    const std::shared_ptr<ParameterAssocArrayRepn> get_repn() const;

    Parameter index(const IndexVector& args);
    void index_error(size_t i);

    ParameterMap& generate_names();

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
        return create_ref(reftmp);
    }

    template <typename... ARGTYPES>
    typename std::enable_if<!has_nonintegral_args<ARGTYPES...>::value, Parameter>::type operator()(
        const ARGTYPES&... args)
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
    explicit ParameterMap(const SequenceContext& arg);
    virtual ~ParameterMap() {}

    /** Set the initial parameter value. \returns the parameter object. */
    ParameterMap& value(double value);

    /** Set the initial parameter value. \returns the parameter object. */
    ParameterMap& value(const Expression& value);

    template <typename KeyType, typename ValueType>
    ParameterMap& value(const std::map<KeyType, ValueType>& values_);

    /** Set the name of the parameter. \returns the parameter object. */
    ParameterMap& name(const std::string& name);

    /** Get the name of the parameter. \returns a string. */
    std::string name();
};

template <typename KeyType, typename ValueType>
inline ParameterMap& ParameterMap::value(const std::map<KeyType, ValueType>& values_)
{
    for (auto& [key, value] : values_) {
        copy_tuple_to_vector(key, tmp);
        index(tmp).value(value);
    }
    return *this;
}

template <>
inline ParameterMap& ParameterMap::value(const std::map<int, double>& values_)
{
    if (dim() != 1)
        index_error(1);
    for (auto& [key, value] : values_) {
        tmp[0] = key;
        index(tmp).value(value);
    }
    return *this;
}

ParameterMap parameter(const ConcreteSet& arg);
inline ParameterMap parameter_map(const ConcreteSet& arg) { return parameter(arg); }

inline ParameterMap parameter(const std::string& name, const ConcreteSet& arg)
{
    return parameter(arg).name(name);
}
inline ParameterMap parameter_map(const std::string& name, const ConcreteSet& arg)
{
    return parameter(arg).name(name);
}

ParameterMap parameter(const SequenceContext& arg);
inline ParameterMap parameter_map(const SequenceContext& arg) { return parameter(arg); }

inline ParameterMap parameter(const std::string& name, const SequenceContext& arg)
{
    return parameter(arg).name(name);
}
inline ParameterMap parameter_map(const std::string& name, const SequenceContext& arg)
{
    return parameter(arg).name(name);
}

}  // namespace coek
