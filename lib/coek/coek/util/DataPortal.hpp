#pragma once

#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>
#include <variant>
#include <iostream>
#include <coek/util/template_utils.hpp>

namespace coek {

class DataPortalRepn {
   public:
    using StringType = std::string;
    using IntegerType = int;
    using TupleKeyType = std::vector<std::variant<StringType, IntegerType>>;
    using KeyTypes = std::variant<StringType, IntegerType, TupleKeyType>;

    using DoubleType = double;
    using TupleValueType = std::vector<std::variant<StringType, IntegerType, DoubleType>>;

    using ParameterTypes = std::variant<StringType, IntegerType, DoubleType, TupleValueType>;

    using StringSetType = std::set<std::string>;
    using IntegerSetType = std::set<int>;
    using DoubleSetType = std::set<double>;
    using TupleSetType = std::set<TupleValueType>;
    using SetTypes = std::variant<StringSetType, IntegerSetType, DoubleSetType, TupleSetType>;

   public:
    std::map<std::string, ParameterTypes> parameter_data;
    std::map<std::string, std::map<KeyTypes, ParameterTypes>> indexed_parameter_data;

    std::map<std::string, SetTypes> set_data;
    std::map<std::string, std::map<KeyTypes, SetTypes>> indexed_set_data;

   public:
    void clear()
    {
        set_data.clear();
        parameter_data.clear();
        indexed_set_data.clear();
        indexed_parameter_data.clear();
    }

    void load_from_file(const std::string& filename);
    void load_from_json_string(const std::string& json_string);

    void save_to_file(const std::string& filename, unsigned int indent = 0);
    std::string to_string(unsigned int indent = 0);

    bool contains(const std::string& name) const;

    //
    // GET methods
    //

    // Set of simple data
    // Set<TYPE> data
    template <typename TYPE>
    bool get(const std::string& name, std::set<TYPE>& data) const;

    // Set of tuple data
    // Set<tuple<ARGTYPES ...>> data
    template <typename... ARGTYPES>
    bool get(const std::string& name, std::set<std::tuple<ARGTYPES...>>& data) const;

    // Indexed set of simple data, with simple indices
    // Map<KeyType, Set<SetType>>
    template <typename KeyType, typename SetType>
    typename std::enable_if<!is_std_tuple<KeyType>::value && !is_std_tuple<SetType>::value,
                            bool>::type
    get(const std::string& name, std::map<KeyType, std::set<SetType>>& data) const;

    // Indexed set of simple data, with tuple indices
    // Map<tuple<KEYTYPES...>, Set<SetType>>
    template <typename SetType, typename... KEYTYPES>
    typename std::enable_if<!is_std_tuple<SetType>::value, bool>::type get(
        const std::string& name, std::map<std::tuple<KEYTYPES...>, std::set<SetType>>& data) const;

    // Indexed set of tuple data, with simple indices
    // Map<KeyType, Set<std::tuple<SETTYPES...>>>
    template <typename KeyType, typename... SETTYPES>
    typename std::enable_if<!is_std_tuple<KeyType>::value, bool>::type get(
        const std::string& name, std::map<KeyType, std::set<std::tuple<SETTYPES...>>>& data) const;

    // Indexed set of tuple data, with tuple indices
    // Map<std::tuple<KEYTYPES...>, Set<std::tuple<SETTYPES...>>>
    template <typename... KEYTYPES, typename... SETTYPES>
    bool get(const std::string& name,
             std::map<std::tuple<KEYTYPES...>, std::set<std::tuple<SETTYPES...>>>& data) const;

    // Parameter data
    // TYPE
    template <typename TYPE>
    typename std::enable_if<!is_std_map<TYPE>::value && !is_std_set<TYPE>::value
                                && !is_std_tuple<TYPE>::value,
                            bool>::type
    get(const std::string& name, TYPE& data) const;

    // Tuple<VALUETYPES...>
    template <typename... VALUETYPES>
    bool get(const std::string& name, std::tuple<VALUETYPES...>& data) const;

    // Indexed parameter data with simple indices
    // Map<Key,Value> - Indexed parameter data
    template <typename KeyType, typename ValueType>
    typename std::enable_if<!is_std_tuple<KeyType>::value && !is_std_set<ValueType>::value
                                && !is_std_tuple<ValueType>::value,
                            bool>::type
    get(const std::string& name, std::map<KeyType, ValueType>& data) const;

    // Indexed parameter data with tuple indices
    // Map<tuple<KEYTYPES...>,Value> - Indexed parameter data
    template <typename ValueType, typename... KEYTYPES>
    typename std::enable_if<!is_std_set<ValueType>::value && !is_std_tuple<ValueType>::value,
                            bool>::type
    get(const std::string& name, std::map<std::tuple<KEYTYPES...>, ValueType>& data) const;

    // Indexed parameter data
    // Map<KeyType,tuple<VALUETYPES...>> - Indexed parameter data
    template <typename KeyType, typename... VALUETYPES>
    typename std::enable_if<!is_std_tuple<KeyType>::value, bool>::type get(
        const std::string& name, std::map<KeyType, std::tuple<VALUETYPES...>>& data) const;

    // Indexed parameter data with tuple indices
    // Map<tuple<KEYTYPES...>,tuple<VALUETYPES...>> - Indexed parameter data
    template <typename... KEYTYPES, typename... VALUETYPES>
    bool get(const std::string& name,
             std::map<std::tuple<KEYTYPES...>, std::tuple<VALUETYPES...>>& data) const;

    //
    // PUT methods
    //

    // Set of simple data
    // Set<TYPE> data
    template <typename TYPE>
    bool put(const std::string& name, const std::set<TYPE>& data);

    // Set of tuple data
    // Set<tuple<ARGTYPES ...>> data
    template <typename... ARGTYPES>
    bool put(const std::string& name, const std::set<std::tuple<ARGTYPES...>>& data);

    // Indexed set of simple data, with simple indices
    // Map<KeyType, Set<SetType>>
    template <typename KeyType, typename SetType>
    typename std::enable_if<!is_std_tuple<KeyType>::value && !is_std_tuple<SetType>::value,
                            bool>::type
    put(const std::string& name, const std::map<KeyType, std::set<SetType>>& data);

    // Indexed set of simple data, with tuple indices
    // Map<tuple<KEYTYPES...>, Set<SetType>>
    template <typename SetType, typename... KEYTYPES>
    typename std::enable_if<!is_std_tuple<SetType>::value, bool>::type put(
        const std::string& name, const std::map<std::tuple<KEYTYPES...>, std::set<SetType>>& data);

    // Indexed set of tuple data, with simple indices
    // Map<KeyType, Set<std::tuple<SETTYPES...>>>
    template <typename KeyType, typename... SETTYPES>
    typename std::enable_if<!is_std_tuple<KeyType>::value, bool>::type put(
        const std::string& name, const std::map<KeyType, std::set<std::tuple<SETTYPES...>>>& data);

    // Indexed set of tuple data, with tuple indices
    // Map<std::tuple<KEYTYPES...>, Set<std::tuple<SETTYPES...>>>
    template <typename... KEYTYPES, typename... SETTYPES>
    bool put(const std::string& name,
             const std::map<std::tuple<KEYTYPES...>, std::set<std::tuple<SETTYPES...>>>& data);

    // Parameter data
    // TYPE
    template <typename TYPE>
    typename std::enable_if<!is_std_map<TYPE>::value && !is_std_set<TYPE>::value
                                && !is_std_tuple<TYPE>::value,
                            bool>::type
    put(const std::string& name, const TYPE& data);

    // Tuple<VALUETYPES...>
    template <typename... VALUETYPES>
    bool put(const std::string& name, const std::tuple<VALUETYPES...>& data);

    // Indexed parameter data with simple indices
    // Map<Key,Value> - Indexed parameter data
    template <typename KeyType, typename ValueType>
    typename std::enable_if<!is_std_tuple<KeyType>::value && !is_std_set<ValueType>::value
                                && !is_std_tuple<ValueType>::value,
                            bool>::type
    put(const std::string& name, const std::map<KeyType, ValueType>& data);

    // Indexed parameter data with tuple indices
    // Map<tuple<KEYTYPES...>,Value> - Indexed parameter data
    template <typename ValueType, typename... KEYTYPES>
    typename std::enable_if<!is_std_set<ValueType>::value && !is_std_tuple<ValueType>::value,
                            bool>::type
    put(const std::string& name, const std::map<std::tuple<KEYTYPES...>, ValueType>& data);

    // Indexed parameter data
    // Map<KeyType,tuple<VALUETYPES...>> - Indexed parameter data
    template <typename KeyType, typename... VALUETYPES>
    typename std::enable_if<!is_std_tuple<KeyType>::value, bool>::type put(
        const std::string& name, const std::map<KeyType, std::tuple<VALUETYPES...>>& data);

    // Indexed parameter data with tuple indices
    // Map<tuple<KEYTYPES...>,tuple<VALUETYPES...>> - Indexed parameter data
    template <typename... KEYTYPES, typename... VALUETYPES>
    bool put(const std::string& name,
             const std::map<std::tuple<KEYTYPES...>, std::tuple<VALUETYPES...>>& data);

   protected:
    template <typename TupleT, typename... ARGTYPES>
    std::tuple<ARGTYPES...> convert_to_tuple(const TupleT& v) const;

    template <typename TupleT, typename... ARGTYPES>
    TupleT convert_from_tuple(const std::tuple<ARGTYPES...>& v) const;
};

template <size_t N, typename... Ts>
using NthTypeOf = typename std::tuple_element<N, std::tuple<Ts...>>::type;

template <typename TupleT, typename... ARGTYPES, std::size_t... Indices>
inline auto vector_to_tuple_helper(const TupleT& v, std::index_sequence<Indices...>)
{
    return std::make_tuple(std::get<NthTypeOf<Indices, ARGTYPES...>>(v[Indices])...);
}

template <typename TupleT, typename... ARGTYPES>
inline std::tuple<ARGTYPES...> DataPortalRepn::convert_to_tuple(const TupleT& v) const
{
    return vector_to_tuple_helper<TupleT, ARGTYPES...>(
        v, std::make_index_sequence<sizeof...(ARGTYPES)>());
}

template <typename TupleT, typename... ARGTYPES, std::size_t... Indices>
inline TupleT tuple_to_vector_helper(const std::tuple<ARGTYPES...>& v,
                                     std::index_sequence<Indices...>)
{
    return TupleT{std::get<Indices>(v)...};
}

template <typename TupleT, typename... ARGTYPES>
inline TupleT DataPortalRepn::convert_from_tuple(const std::tuple<ARGTYPES...>& v) const
{
    return tuple_to_vector_helper<TupleT, ARGTYPES...>(
        v, std::make_index_sequence<sizeof...(ARGTYPES)>());
}

#if 0
template <typename TYPE>
inline std::string type_name()
{ return "unknown"; }

template <>
inline std::string type_name<std::string>()
{ return "string"; }

template <>
inline std::string type_name<int>()
{ return "integer"; }

template <>
inline std::string type_name<double>()
{ return "double"; }
#endif

// Set data
template <typename ValueType>
inline bool DataPortalRepn::get(const std::string& name, std::set<ValueType>& data) const
{
    auto it = set_data.find(name);
    if (it == set_data.end())
        return false;
    if (not std::holds_alternative<std::set<ValueType>>(it->second))
        return false;

    data = std::get<std::set<ValueType>>(it->second);

    return true;
}

// Set<tuple<ARGTYPES ...>> data
template <typename... ARGTYPES>
inline bool DataPortalRepn::get(const std::string& name,
                                std::set<std::tuple<ARGTYPES...>>& data) const
{
    auto it = set_data.find(name);
    if (it == set_data.end())
        return false;
    if (not std::holds_alternative<std::set<TupleValueType>>(it->second))
        return false;

    data.clear();

    for (auto& v : std::get<std::set<TupleValueType>>(it->second))
        data.insert(convert_to_tuple<TupleValueType, ARGTYPES...>(v));

    return true;
}

// Indexed set of simple data, with simple indices
// Map<KeyType, Set<SetType>>
template <typename KeyType, typename SetType>
inline typename std::enable_if<!is_std_tuple<KeyType>::value && !is_std_tuple<SetType>::value,
                               bool>::type
DataPortalRepn::get(const std::string& name, std::map<KeyType, std::set<SetType>>& data) const
{
    auto it = indexed_set_data.find(name);
    if (it == indexed_set_data.end())
        return false;

    data.clear();
    for (auto& v : it->second) {
        if (not std::holds_alternative<std::set<SetType>>(v.second))
            return false;

        data.emplace(std::get<KeyType>(v.first), std::get<std::set<SetType>>(v.second));
    }

    return true;
}

// Indexed set of simple data, with tuple indices
// Map<tuple<KEYTYPES...>, Set<SetType>>
template <typename SetType, typename... KEYTYPES>
inline typename std::enable_if<!is_std_tuple<SetType>::value, bool>::type DataPortalRepn::get(
    const std::string& name, std::map<std::tuple<KEYTYPES...>, std::set<SetType>>& data) const
{
    auto it = indexed_set_data.find(name);
    if (it == indexed_set_data.end())
        return false;

    data.clear();
    for (auto& v : it->second) {
        if (not std::holds_alternative<std::set<SetType>>(v.second))
            return false;

        data.emplace(convert_to_tuple<TupleKeyType, KEYTYPES...>(std::get<TupleKeyType>(v.first)),
                     std::get<std::set<SetType>>(v.second));
    }

    return true;
}

// Indexed set of tuple data, with simple indices
// Map<KeyType, Set<std::tuple<SETTYPES...>>>
template <typename KeyType, typename... SETTYPES>
inline typename std::enable_if<!is_std_tuple<KeyType>::value, bool>::type DataPortalRepn::get(
    const std::string& name, std::map<KeyType, std::set<std::tuple<SETTYPES...>>>& data) const
{
    auto it = indexed_set_data.find(name);
    if (it == indexed_set_data.end())
        return false;

    data.clear();
    for (auto& v : it->second) {
        if (not std::holds_alternative<KeyType>(v.first))
            return false;
        if (not std::holds_alternative<TupleSetType>(v.second))
            return false;

        std::set<std::tuple<SETTYPES...>> tmp;
        auto& tmp_ = data[std::get<KeyType>(v.first)] = tmp;

        for (auto& sv : std::get<TupleSetType>(v.second))
            tmp_.insert(convert_to_tuple<TupleValueType, SETTYPES...>(sv));
    }

    return true;
}

// Indexed set of tuple data, with tuple indices
// Map<std::tuple<KEYTYPES...>, Set<std::tuple<SETTYPES...>>>
template <typename... KEYTYPES, typename... SETTYPES>
inline bool DataPortalRepn::get(
    const std::string& name,
    std::map<std::tuple<KEYTYPES...>, std::set<std::tuple<SETTYPES...>>>& data) const
{
    auto it = indexed_set_data.find(name);
    if (it == indexed_set_data.end())
        return false;

    data.clear();
    for (auto& v : it->second) {
        if (not std::holds_alternative<TupleSetType>(v.second))
            return false;

        std::set<std::tuple<SETTYPES...>> tmp;
        auto& tmp_
            = data[convert_to_tuple<TupleKeyType, KEYTYPES...>(std::get<TupleKeyType>(v.first))]
            = tmp;

        for (auto& sv : std::get<TupleSetType>(v.second))
            tmp_.insert(convert_to_tuple<TupleValueType, SETTYPES...>(sv));
    }

    return true;
}

// Parameter data
// TYPE
template <typename TYPE>
inline typename std::enable_if<
    !is_std_map<TYPE>::value && !is_std_set<TYPE>::value && !is_std_tuple<TYPE>::value, bool>::type
DataPortalRepn::get(const std::string& name, TYPE& data) const
{
    auto it = parameter_data.find(name);
    if (it == parameter_data.end())
        return false;
    if (not std::holds_alternative<TYPE>(it->second))
        return false;

    data = std::get<TYPE>(it->second);

    return true;
}

template <>
inline bool DataPortalRepn::get(const std::string& name, double& data) const
{
    auto it = parameter_data.find(name);
    if (it == parameter_data.end())
        return false;

    if (std::holds_alternative<double>(it->second))
        data = std::get<double>(it->second);
    else if (std::holds_alternative<int>(it->second))
        data = std::get<int>(it->second);
    else
        return false;

    return true;
}

// Tuple<VALUETYPES...>
template <typename... VALUETYPES>
inline bool DataPortalRepn::get(const std::string& name, std::tuple<VALUETYPES...>& data) const
{
    auto it = parameter_data.find(name);
    if (it == parameter_data.end())
        return false;
    if (not std::holds_alternative<TupleValueType>(it->second))
        return false;

    data = convert_to_tuple<TupleValueType, VALUETYPES...>(std::get<TupleValueType>(it->second));

    return true;
}

// Indexed parameter data with simple indices
// Map<Key,Value> - Indexed parameter data
template <typename KeyType, typename ValueType>
inline typename std::enable_if<!is_std_tuple<KeyType>::value && !is_std_set<ValueType>::value
                                   && !is_std_tuple<ValueType>::value,
                               bool>::type
DataPortalRepn::get(const std::string& name, std::map<KeyType, ValueType>& data) const
{
    auto it = indexed_parameter_data.find(name);
    if (it == indexed_parameter_data.end())
        return false;

    data.clear();
    for (auto& v : it->second) {
        if (not std::holds_alternative<ValueType>(v.second))
            return false;
        data.emplace(std::get<KeyType>(v.first), std::get<ValueType>(v.second));
    }

    return true;
}

// Indexed parameter data with tuple indices
// Map<tuple<KEYTYPES...>,Value> - Indexed parameter data
template <typename ValueType, typename... KEYTYPES>
inline typename std::enable_if<!is_std_set<ValueType>::value && !is_std_tuple<ValueType>::value,
                               bool>::type
DataPortalRepn::get(const std::string& name,
                    std::map<std::tuple<KEYTYPES...>, ValueType>& data) const
{
    auto it = indexed_parameter_data.find(name);
    if (it == indexed_parameter_data.end())
        return false;

    data.clear();
    for (auto& v : it->second) {
        if (not std::holds_alternative<ValueType>(v.second))
            return false;
        data.emplace(convert_to_tuple<TupleKeyType, KEYTYPES...>(std::get<TupleKeyType>(v.first)),
                     std::get<ValueType>(v.second));
    }

    return true;
}

// Indexed parameter data
// Map<KeyType,tuple<VALUETYPES...>> - Indexed parameter data
template <typename KeyType, typename... VALUETYPES>
inline typename std::enable_if<!is_std_tuple<KeyType>::value, bool>::type DataPortalRepn::get(
    const std::string& name, std::map<KeyType, std::tuple<VALUETYPES...>>& data) const
{
    auto it = indexed_parameter_data.find(name);
    if (it == indexed_parameter_data.end())
        return false;

    data.clear();
    for (auto& v : it->second) {
        if (not std::holds_alternative<TupleValueType>(v.second))
            return false;
        data.emplace(std::get<KeyType>(v.first), convert_to_tuple<TupleValueType, VALUETYPES...>(
                                                     std::get<TupleValueType>(v.second)));
    }

    return true;
}

// Indexed parameter data with tuple indices
// Indexed parameter data with tuple indices
// Map<tuple<KEYTYPES...>,tuple<VALUETYPES...>> - Indexed parameter data
template <typename... KEYTYPES, typename... VALUETYPES>
inline bool DataPortalRepn::get(
    const std::string& name,
    std::map<std::tuple<KEYTYPES...>, std::tuple<VALUETYPES...>>& data) const
{
    auto it = indexed_parameter_data.find(name);
    if (it == indexed_parameter_data.end())
        return false;

    data.clear();
    for (auto& v : it->second) {
        if (not std::holds_alternative<TupleValueType>(v.second))
            return false;
        data.emplace(
            convert_to_tuple<TupleKeyType, KEYTYPES...>(std::get<TupleKeyType>(v.first)),
            convert_to_tuple<TupleValueType, VALUETYPES...>(std::get<TupleValueType>(v.second)));
    }

    return true;
}

//
// PUT methods
//

// Set data
template <typename ValueType>
inline bool DataPortalRepn::put(const std::string& name, const std::set<ValueType>& data)
{
    set_data[name] = data;
    return true;
}

// Set<tuple<ARGTYPES ...>> data
template <typename... ARGTYPES>
inline bool DataPortalRepn::put(const std::string& name,
                                const std::set<std::tuple<ARGTYPES...>>& data)
{
    std::set<TupleValueType> tmp;

    for (const auto& v : data)
        tmp.insert(convert_from_tuple<TupleValueType, ARGTYPES...>(v));

    set_data[name] = tmp;

    return true;
}

// Indexed set of simple data, with simple indices
// Map<KeyType, Set<SetType>>
template <typename KeyType, typename SetType>
inline typename std::enable_if<!is_std_tuple<KeyType>::value && !is_std_tuple<SetType>::value,
                               bool>::type
DataPortalRepn::put(const std::string& name, const std::map<KeyType, std::set<SetType>>& data)
{
    std::map<KeyTypes, SetTypes> tmp;
    auto& tmp_ = indexed_set_data[name] = tmp;

    for (const auto& kv : data)
        tmp_.insert(kv);

    return true;
}

// Indexed set of simple data, with tuple indices
// Map<tuple<KEYTYPES...>, Set<SetType>>
template <typename SetType, typename... KEYTYPES>
inline typename std::enable_if<!is_std_tuple<SetType>::value, bool>::type DataPortalRepn::put(
    const std::string& name, const std::map<std::tuple<KEYTYPES...>, std::set<SetType>>& data)
{
    std::map<KeyTypes, SetTypes> tmp;
    auto& tmp_ = indexed_set_data[name] = tmp;

    for (const auto& kv : data)
        tmp_[convert_from_tuple<TupleKeyType, KEYTYPES...>(kv.first)] = kv.second;

    return true;
}

// Indexed set of tuple data, with simple indices
// Map<KeyType, Set<std::tuple<SETTYPES...>>>
template <typename KeyType, typename... SETTYPES>
inline typename std::enable_if<!is_std_tuple<KeyType>::value, bool>::type DataPortalRepn::put(
    const std::string& name, const std::map<KeyType, std::set<std::tuple<SETTYPES...>>>& data)
{
    std::map<KeyTypes, SetTypes> tmp;
    auto& tmp_ = indexed_set_data[name] = tmp;

    for (const auto& kv : data) {
        TupleSetType tset;
        auto& tset_ = std::get<TupleSetType>(tmp_[kv.first] = tset);
        for (const auto& sv : kv.second)
            tset_.insert(convert_from_tuple<TupleValueType, SETTYPES...>(sv));
    }

    return true;
}

// Indexed set of tuple data, with tuple indices
// Map<std::tuple<KEYTYPES...>, Set<std::tuple<SETTYPES...>>>
template <typename... KEYTYPES, typename... SETTYPES>
inline bool DataPortalRepn::put(
    const std::string& name,
    const std::map<std::tuple<KEYTYPES...>, std::set<std::tuple<SETTYPES...>>>& data)
{
    std::map<KeyTypes, SetTypes> tmp;
    auto& tmp_ = indexed_set_data[name] = tmp;

    for (const auto& kv : data) {
        TupleSetType tset;
        auto& tset_ = std::get<TupleSetType>(
            tmp_[convert_from_tuple<TupleKeyType, KEYTYPES...>(kv.first)] = tset);
        for (const auto& sv : kv.second)
            tset_.insert(convert_from_tuple<TupleValueType, SETTYPES...>(sv));
    }

    return true;
}

// Parameter data
template <typename TYPE>
inline typename std::enable_if<
    !is_std_map<TYPE>::value && !is_std_set<TYPE>::value && !is_std_tuple<TYPE>::value, bool>::type
DataPortalRepn::put(const std::string& name, const TYPE& data)
{
    parameter_data[name] = data;

    return true;
}

template <>
inline bool DataPortalRepn::put(const std::string& name, const double& data)
{
    parameter_data[name] = data;

    return true;
}

template <typename... VALUETYPES>
inline bool DataPortalRepn::put(const std::string& name, const std::tuple<VALUETYPES...>& data)
{
    parameter_data[name] = convert_from_tuple<TupleValueType, VALUETYPES...>(data);

    return true;
}

// Indexed parameter data with simple indices
// Map<Key,Value> - Indexed parameter data
template <typename KeyType, typename ValueType>
inline typename std::enable_if<!is_std_tuple<KeyType>::value && !is_std_set<ValueType>::value
                                   && !is_std_tuple<ValueType>::value,
                               bool>::type
DataPortalRepn::put(const std::string& name, const std::map<KeyType, ValueType>& data)
{
    std::map<KeyTypes, ParameterTypes> tmp;
    auto& tmp_ = indexed_parameter_data[name] = tmp;

    for (const auto& kv : data)
        tmp_.insert(kv);

    return true;
}

// Indexed parameter data with tuple indices
// Map<tuple<KEYTYPES...>,Value> - Indexed parameter data
template <typename ValueType, typename... KEYTYPES>
inline typename std::enable_if<!is_std_set<ValueType>::value && !is_std_tuple<ValueType>::value,
                               bool>::type
DataPortalRepn::put(const std::string& name,
                    const std::map<std::tuple<KEYTYPES...>, ValueType>& data)
{
    std::map<KeyTypes, ParameterTypes> tmp;
    auto& tmp_ = indexed_parameter_data[name] = tmp;

    for (const auto& kv : data)
        tmp_[convert_from_tuple<TupleKeyType, KEYTYPES...>(kv.first)] = kv.second;

    return true;
}

// Indexed parameter data
// Map<KeyType,tuple<VALUETYPES...>> - Indexed parameter data
template <typename KeyType, typename... VALUETYPES>
inline typename std::enable_if<!is_std_tuple<KeyType>::value, bool>::type DataPortalRepn::put(
    const std::string& name, const std::map<KeyType, std::tuple<VALUETYPES...>>& data)
{
    std::map<KeyTypes, ParameterTypes> tmp;
    auto& tmp_ = indexed_parameter_data[name] = tmp;

    for (const auto& kv : data)
        tmp_[kv.first] = convert_from_tuple<TupleValueType, VALUETYPES...>(kv.second);

    return true;
}

// Indexed parameter data with tuple indices
// Map<tuple<KEYTYPES...>,tuple<VALUETYPES...>> - Indexed parameter data
template <typename... KEYTYPES, typename... VALUETYPES>
inline bool DataPortalRepn::put(
    const std::string& name,
    const std::map<std::tuple<KEYTYPES...>, std::tuple<VALUETYPES...>>& data)
{
    std::map<KeyTypes, ParameterTypes> tmp;
    auto& tmp_ = indexed_parameter_data[name] = tmp;

    for (const auto& kv : data)
        tmp_[convert_from_tuple<TupleKeyType, KEYTYPES...>(kv.first)]
            = convert_from_tuple<TupleValueType, VALUETYPES...>(kv.second);

    return true;
}

/**
 * A class for loading data from external resources and standardizing the initialization of
 * C++ data types.
 *
 * NOTE: This API needs to be generalized to account for different types of data stores.
 */
class DataPortal {
   public:
    std::shared_ptr<DataPortalRepn> repn;

   public:
    /** Construct an empty data portal. */
    DataPortal() { repn = std::make_shared<DataPortalRepn>(); }

    /** Empty the object data. */
    void clear() { repn->clear(); }

    /** Initialize a data portal from a file. */
    void load_from_file(const std::string& filename) { repn->load_from_file(filename); }

    /** Initialize a data portal from a JSON string. */
    void load_from_json_string(const std::string& json_string)
    {
        repn->load_from_json_string(json_string);
    }

    /** Save data portal to a file.

        When the indent option is specified, the output is a human
        readable JSON format. */
    void save_to_file(const std::string& filename, unsigned int indent = 0)
    {
        repn->save_to_file(filename, indent);
    }

    /** Create a JSON string representation.

        When the indent option is specified, the output is a human
        readable JSON format. */
    std::string to_string(unsigned int indent = 0) { return repn->to_string(indent); }

    /** Returns true if the dataportal contains the specified data. */
    bool contains(const std::string& name) const { return repn->contains(name); }

    //
    // GET methods
    //

    // Set data

    // Set of simple data
    // Set<TYPE> data
    template <typename TYPE>
    bool get(const std::string& name, std::set<TYPE>& data) const
    {
        return repn->get(name, data);
    }

    // Set of tuple data
    // Set<tuple<ARGTYPES ...>> data
    template <typename... ARGTYPES>
    bool get(const std::string& name, std::set<std::tuple<ARGTYPES...>>& data) const
    {
        return repn->get(name, data);
    }

    // Indexed set of simple data, with simple indices
    // Map<KeyType, Set<SetType>>
    template <typename KeyType, typename SetType>
    typename std::enable_if<!is_std_tuple<KeyType>::value && !is_std_tuple<SetType>::value,
                            bool>::type
    get(const std::string& name, std::map<KeyType, std::set<SetType>>& data) const
    {
        return repn->get(name, data);
    }

    // Indexed set of simple data, with tuple indices
    // Map<tuple<KEYTYPES...>, Set<SetType>>
    template <typename SetType, typename... KEYTYPES>
    typename std::enable_if<!is_std_tuple<SetType>::value, bool>::type get(
        const std::string& name, std::map<std::tuple<KEYTYPES...>, std::set<SetType>>& data) const
    {
        return repn->get(name, data);
    }

    // Indexed set of tuple data, with simple indices
    // Map<KeyType, Set<std::tuple<SETTYPES...>>>
    template <typename KeyType, typename... SETTYPES>
    typename std::enable_if<!is_std_tuple<KeyType>::value, bool>::type get(
        const std::string& name, std::map<KeyType, std::set<std::tuple<SETTYPES...>>>& data) const
    {
        return repn->get(name, data);
    }

    // Indexed set of tuple data, with tuple indices
    // Map<std::tuple<KEYTYPES...>, Set<std::tuple<SETTYPES...>>>
    template <typename... KEYTYPES, typename... SETTYPES>
    bool get(const std::string& name,
             std::map<std::tuple<KEYTYPES...>, std::set<std::tuple<SETTYPES...>>>& data) const
    {
        return repn->get(name, data);
    }

    // Parameter data

    // TYPE
    template <typename TYPE>
    typename std::enable_if<!is_std_map<TYPE>::value && !is_std_set<TYPE>::value
                                && !is_std_tuple<TYPE>::value,
                            bool>::type
    get(const std::string& name, TYPE& data) const
    {
        return repn->get(name, data);
    }

    // Tuple<VALUETYPES...>
    template <typename... VALUETYPES>
    bool get(const std::string& name, std::tuple<VALUETYPES...>& data) const
    {
        return repn->get(name, data);
    }

    // Indexed parameter data with simple indices

    // Indexed parameter data
    template <typename KeyType, typename ValueType>
    typename std::enable_if<!is_std_tuple<KeyType>::value && !is_std_set<ValueType>::value
                                && !is_std_tuple<ValueType>::value,
                            bool>::type
    get(const std::string& name, std::map<KeyType, ValueType>& data) const
    {
        return repn->get(name, data);
    }

    // Indexed parameter data
    template <typename ValueType, typename... KEYTYPES>
    typename std::enable_if<!is_std_set<ValueType>::value && !is_std_tuple<ValueType>::value,
                            bool>::type
    get(const std::string& name, std::map<std::tuple<KEYTYPES...>, ValueType>& data) const
    {
        return repn->get(name, data);
    }

    // Indexed parameter data
    // Map<KeyType,tuple<VALUETYPES...>> - Indexed parameter data
    template <typename KeyType, typename... VALUETYPES>
    typename std::enable_if<!is_std_tuple<KeyType>::value, bool>::type get(
        const std::string& name, std::map<KeyType, std::tuple<VALUETYPES...>>& data) const
    {
        return repn->get(name, data);
    }

    // Indexed parameter data with tuple indices
    // Map<tuple<KEYTYPES...>,tuple<VALUETYPES...>> - Indexed parameter data
    template <typename... KEYTYPES, typename... VALUETYPES>
    bool get(const std::string& name,
             std::map<std::tuple<KEYTYPES...>, std::tuple<VALUETYPES...>>& data) const
    {
        return repn->get(name, data);
    }

    //
    // PUT methods
    //

    // Set data

    // Set of simple data
    // Set<TYPE> data
    template <typename TYPE>
    bool put(const std::string& name, const std::set<TYPE>& data)
    {
        return repn->put(name, data);
    }

    // Set of tuple data
    // Set<tuple<ARGTYPES ...>> data
    template <typename... ARGTYPES>
    bool put(const std::string& name, const std::set<std::tuple<ARGTYPES...>>& data)
    {
        return repn->put(name, data);
    }

    // Indexed set of simple data, with simple indices
    // Map<KeyType, Set<SetType>>
    template <typename KeyType, typename SetType>
    typename std::enable_if<!is_std_tuple<KeyType>::value && !is_std_tuple<SetType>::value,
                            bool>::type
    put(const std::string& name, const std::map<KeyType, std::set<SetType>>& data)
    {
        return repn->put(name, data);
    }

    // Indexed set of simple data, with tuple indices
    // Map<tuple<KEYTYPES...>, Set<SetType>>
    template <typename SetType, typename... KEYTYPES>
    typename std::enable_if<!is_std_tuple<SetType>::value, bool>::type put(
        const std::string& name, const std::map<std::tuple<KEYTYPES...>, std::set<SetType>>& data)
    {
        return repn->put(name, data);
    }

    // Indexed set of tuple data, with simple indices
    // Map<KeyType, Set<std::tuple<SETTYPES...>>>
    template <typename KeyType, typename... SETTYPES>
    typename std::enable_if<!is_std_tuple<KeyType>::value, bool>::type put(
        const std::string& name, const std::map<KeyType, std::set<std::tuple<SETTYPES...>>>& data)
    {
        return repn->put(name, data);
    }

    // Indexed set of tuple data, with tuple indices
    // Map<std::tuple<KEYTYPES...>, Set<std::tuple<SETTYPES...>>>
    template <typename... KEYTYPES, typename... SETTYPES>
    bool put(const std::string& name,
             const std::map<std::tuple<KEYTYPES...>, std::set<std::tuple<SETTYPES...>>>& data)
    {
        return repn->put(name, data);
    }

    // Parameter data

    // TYPE
    template <typename TYPE>
    typename std::enable_if<!is_std_map<TYPE>::value && !is_std_set<TYPE>::value
                                && !is_std_tuple<TYPE>::value,
                            bool>::type
    put(const std::string& name, const TYPE& data)
    {
        return repn->put(name, data);
    }

    // Tuple<VALUETYPES...>
    template <typename... VALUETYPES>
    bool put(const std::string& name, const std::tuple<VALUETYPES...>& data)
    {
        return repn->put(name, data);
    }

    // Indexed parameter data
    template <typename KeyType, typename ValueType>
    typename std::enable_if<!is_std_tuple<KeyType>::value && !is_std_set<ValueType>::value
                                && !is_std_tuple<ValueType>::value,
                            bool>::type
    put(const std::string& name, const std::map<KeyType, ValueType>& data)
    {
        return repn->put(name, data);
    }

    // Indexed parameter data
    template <typename ValueType, typename... KEYTYPES>
    typename std::enable_if<!is_std_set<ValueType>::value && !is_std_tuple<ValueType>::value,
                            bool>::type
    put(const std::string& name, const std::map<std::tuple<KEYTYPES...>, ValueType>& data)
    {
        return repn->put(name, data);
    }

    // Indexed parameter data
    // Map<KeyType,tuple<VALUETYPES...>> - Indexed parameter data
    template <typename KeyType, typename... VALUETYPES>
    typename std::enable_if<!is_std_tuple<KeyType>::value, bool>::type put(
        const std::string& name, const std::map<KeyType, std::tuple<VALUETYPES...>>& data)
    {
        return repn->put(name, data);
    }

    // Indexed parameter data with tuple indices
    // Map<tuple<KEYTYPES...>,tuple<VALUETYPES...>> - Indexed parameter data
    template <typename... KEYTYPES, typename... VALUETYPES>
    bool put(const std::string& name,
             const std::map<std::tuple<KEYTYPES...>, std::tuple<VALUETYPES...>>& data)
    {
        return repn->put(name, data);
    }
};

}  // namespace coek
