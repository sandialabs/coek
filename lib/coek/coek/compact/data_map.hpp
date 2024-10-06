#pragma once

#include "coek/api/data_assoc_array.hpp"
#include "coek/compact/sequence_context.hpp"
#include "coek/util/template_utils.hpp"
#include "coek_sets.hpp"

namespace coek {

class DataMapRepn;

class DataMap : public DataAssocArray {
   public:
    std::shared_ptr<DataMapRepn> repn;
    std::shared_ptr<DataAssocArrayRepn> get_repn();
    const std::shared_ptr<DataAssocArrayRepn> get_repn() const;

    Expression index(const IndexVector& args);
    void index_error(size_t i);

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
    typename std::enable_if<!has_nonintegral_args<ARGTYPES...>::value, Expression>::type operator()(
        const ARGTYPES&... args)
    {
        const size_t nargs = count_args(args...);
        if (dim() != nargs)
            index_error(nargs);
        collect_args(static_cast<size_t>(0), args...);
        return index(tmp);
    }

    Expression operator()(int i)
    {
        if (dim() != 1)
            index_error(1);
        tmp[0] = i;
        return index(tmp);
    }

    Expression operator()(int i, int j)
    {
        if (dim() != 2)
            index_error(2);
        tmp[0] = i;
        tmp[1] = j;
        return index(tmp);
    }

   public:
    explicit DataMap(const ConcreteSet& arg);
    explicit DataMap(const SequenceContext& arg);
    virtual ~DataMap() {}

    /** Set the initial data value. \returns the data object. */
    DataMap& value(double value);
    /** Set the initial data value. \returns the data object. */
    DataMap& value(const Expression& value);
    template <typename KeyType, typename ValueType>
    DataMap& value(const std::map<KeyType, ValueType>& values)
    {
        for (auto& [key, value] : values) {
            this->operator()(key).value(value);
        }
        return *this;
    }

    /** Set the name of the data. \returns the data object */
    DataMap& name(const std::string& name);
};

DataMap data(const ConcreteSet& arg);
inline DataMap data_map(const ConcreteSet& arg) { return data(arg); }

inline DataMap data(const std::string& name, const ConcreteSet& arg)
{
    return data(arg).name(name);
}
inline DataMap data_map(const std::string& name, const ConcreteSet& arg)
{
    return data(arg).name(name);
}

DataMap data(const SequenceContext& arg);
inline DataMap data_map(const SequenceContext& arg) { return data(arg); }

inline DataMap data(const std::string& name, const SequenceContext& arg)
{
    return data(arg).name(name);
}
inline DataMap data_map(const std::string& name, const SequenceContext& arg)
{
    return data(arg).name(name);
}

}  // namespace coek
