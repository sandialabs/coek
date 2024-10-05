#pragma once

#include <cassert>
#include <initializer_list>
#include <vector>

#include "coek/util/template_utils.hpp"
#include "data_assoc_array.hpp"

namespace coek {

class DataArrayRepn;

class DataArray : public DataAssocArray {
   public:
    std::shared_ptr<DataArrayRepn> repn;
    std::shared_ptr<DataAssocArrayRepn> get_repn();
    const std::shared_ptr<DataAssocArrayRepn> get_repn() const;

    Expression index(const IndexVector& args);
    void index_error(size_t i);

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
        assert(arg >= 0);  // TODO - fix code coverage here
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
    typename std::enable_if<!has_nonintegral_args<ARGTYPES...>::value, Expression>::type operator()(
        const ARGTYPES&... args)
    {
        const size_t nargs = count_args(args...);
        if (dim() != nargs)
            index_error(nargs);
        collect_args(static_cast<size_t>(0), args...);
        return index(tmp);
    }

    Expression operator()(size_t i)
    {
        if (dim() != 1)
            index_error(1);
        tmp[0] = static_cast<int>(i);
        return index(tmp);
    }

    Expression operator()(size_t i, size_t j)
    {
        if (dim() != 2)
            index_error(2);
        tmp[0] = static_cast<int>(i);
        tmp[1] = static_cast<int>(j);
        return index(tmp);
    }

   public:
    DataArray(size_t n);
    DataArray(const std::vector<size_t>& shape);
    DataArray(const std::initializer_list<size_t>& shape);
    virtual ~DataArray() {}

    /** Set the initial data value. \returns the data object. */
    DataArray& value(double value);
    /** Set the initial data value. \returns the data object. */
    DataArray& value(const Expression& value);

    /** Set the name of the data. \returns the data object */
    DataArray& name(const std::string& name);
    /** Get the name of the data. \returns the data string name */
    std::string name() const;
};

DataArray data(size_t n);
DataArray data(const std::vector<size_t>& shape);
DataArray data(const std::initializer_list<size_t>& shape);

inline DataArray data_array(size_t n) { return data(n); }
inline DataArray data_array(const std::vector<size_t>& shape) { return data(shape); }
inline DataArray data_array(const std::initializer_list<size_t>& shape)
{
    return data(shape);
}

inline DataArray data(const std::string& name, size_t n)
{
    return data(n).name(name);
}
inline DataArray data(const std::string& name, const std::vector<size_t>& shape)
{
    return data(shape).name(name);
}
inline DataArray data(const std::string& name, const std::initializer_list<size_t>& shape)
{
    return data(shape).name(name);
}

inline DataArray data_array(const std::string& name, size_t n)
{
    return data(n).name(name);
}
inline DataArray data_array(const std::string& name, const std::vector<size_t>& shape)
{
    return data(shape).name(name);
}
inline DataArray data_array(const std::string& name,
                                      const std::initializer_list<size_t>& shape)
{
    return data(shape).name(name);
}

}  // namespace coek
