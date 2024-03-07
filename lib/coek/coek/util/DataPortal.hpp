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

class DataPortalRepn
{
public:

    using StringType = std::string;
    using StringSetType = std::set<std::string>;

    using IntegerType = int;
    using IntegerSetType = std::set<int>;

    using TupleType = std::vector<std::variant<StringType, IntegerType>>;
    using TupleSetType = std::set<TupleType>;

    using SetTypes = std::variant<StringSetType,IntegerSetType,TupleSetType>;

    using KeyTypes = std::variant<StringType, IntegerType, TupleType>;
    using ParameterTypes = std::variant<StringType, IntegerType, TupleType>;

    std::map<std::string, SetTypes> set_data;
    std::map<std::string, ParameterTypes> parameter_data;

    std::map<std::string, std::map<KeyTypes,SetTypes>> indexed_set_data;
    std::map<std::string, std::map<KeyTypes,ParameterTypes>> indexed_parameter_data;

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

    // Set of simple data
    // Set<TYPE> data
    template <typename TYPE>
    bool
    get(const std::string& name, std::set<TYPE>& data) const;

    // Set of tuple data
    // Set<tuple<ARGTYPES ...>> data
    template <typename... ARGTYPES>
    bool
    get(const std::string& name, std::set<std::tuple<ARGTYPES ...>>& data) const;

    // Indexed set of simple data, with simple indices
    // Map<KeyType, Set<SetType>>
    template <typename KeyType, typename SetType>
    typename std::enable_if<!is_std_tuple<KeyType>::value && !is_std_tuple<SetType>::value, bool>::type
    get(const std::string& name, std::map<KeyType,std::set<SetType>>& data);

    // Indexed set of simple data, with tuple indices
    // Map<tuple<KEYTYPES...>, Set<SetType>>
    template <typename SetType, typename ...KEYTYPES>
    typename std::enable_if<!is_std_tuple<SetType>::value, bool>::type
    get(const std::string& name, std::map<std::tuple<KEYTYPES...>,std::set<SetType>>& data);

    // Indexed set of tuple data, with simple indices
    // Map<KeyType, Set<std::tuple<SETTYPES...>>>
    template <typename KeyType, typename ...SETTYPES>
    typename std::enable_if<!is_std_tuple<KeyType>::value, bool>::type
    get(const std::string& name, std::map<KeyType,std::set<std::tuple<SETTYPES...>>>& data);

    // Indexed set of tuple data, with tuple indices
    // Map<std::tuple<KEYTYPES...>, Set<std::tuple<SETTYPES...>>>
    template <typename ...KEYTYPES, typename ...SETTYPES>
    bool
    get(const std::string& name, std::map<std::tuple<KEYTYPES...>,std::set<std::tuple<SETTYPES...>>>& data);

    // Parameter data
    // TYPE
    template <typename TYPE>
    typename std::enable_if<!is_std_map<TYPE>::value && !is_std_set<TYPE>::value, bool>::type
    get(const std::string& name, TYPE& data) const;

    // Indexed parameter data with simple indices
    // Map<Key,Value> - Indexed parameter data 
    template <typename KeyType, typename ValueType>
    typename std::enable_if<!is_std_tuple<KeyType>::value && !is_std_set<ValueType>::value && !is_std_tuple<ValueType>::value, bool>::type
    get(const std::string& name, std::map<KeyType,ValueType>& data) const;

    // Indexed parameter data with tuple indices
    // Map<tuple<KEYTYPES...>,Value> - Indexed parameter data 
    template <typename ValueType, typename ...KEYTYPES>
    typename std::enable_if<!is_std_set<ValueType>::value && !is_std_tuple<ValueType>::value, bool>::type
    get(const std::string& name, std::map<std::tuple<KEYTYPES...>,ValueType>& data) const;

  protected:

    template <typename ...ARGTYPES>
    std::tuple<ARGTYPES...> convert_to_tuple(const TupleType& v) const;
    
};


template<size_t N, typename... Ts>
using NthTypeOf = typename std::tuple_element<N, std::tuple<Ts...>>::type;

template <typename... ARGTYPES, std::size_t... Indices>
auto vector_to_tuple_helper(const DataPortalRepn::TupleType& v, std::index_sequence<Indices...>) {
    return std::make_tuple(
        //std::any_cast<NthTypeOf<Indices, ARGTYPES...>>(v[Indices])...
        std::get<NthTypeOf<Indices, ARGTYPES...>>(v[Indices])...
    );
}

template <typename... ARGTYPES>
std::tuple<ARGTYPES...> DataPortalRepn::convert_to_tuple(const TupleType& v) const
{ return vector_to_tuple_helper<ARGTYPES...>(v, std::make_index_sequence<sizeof...(ARGTYPES)>()); }
    

#if 0
template <typename TYPE>
std::string type_name()
{ return "unknown"; }

template <>
std::string type_name<std::string>()
{ return "string"; }

template <>
std::string type_name<int>()
{ return "integer"; }
#endif


// Set data
template <typename TYPE>
bool
DataPortalRepn::get(const std::string& name, std::set<TYPE>& data) const
{
auto it = set_data.find(name);
if (it == set_data.end()) {
    // TODO - raise an exception
    //std::cerr << "ERROR - missing data '" << name << "'" << std::endl;
    return false;
    }

if (not std::holds_alternative<std::set<TYPE>>(it->second)) {
    // TODO - raise an exception
    //std::cerr << "ERROR - data '" << name << "' is not a set of " << type_name<TYPE>() << " values." << std::endl;;
    return false;
    }

data = std::get<std::set<TYPE>>(it->second);

return true;
}

// Set<tuple<ARGTYPES ...>> data
template <typename... ARGTYPES>
bool
DataPortalRepn::get(const std::string& name, std::set<std::tuple<ARGTYPES ...>>& data) const
{
auto it = set_data.find(name);
if (it == set_data.end()) {
    // TODO - raise an exception
    //std::cerr << "ERROR - missing data '" << name << "'" << std::endl;
    return false;
    }

if (not std::holds_alternative<std::set<TupleType>>(it->second)) {
    // TODO - raise an exception
    //std::cerr << "ERROR - data '" << name << "' is not a set of " << type_name<TYPE>() << " values." << std::endl;;
    return false;
    }

data.clear();

for (auto& v : std::get<std::set<TupleType>>(it->second))
    data.insert( convert_to_tuple<ARGTYPES...>(v) );
    //auto tmp = convert_to_tuple<ARGTYPES...>(v);
    //data.insert(tmp);

return true;
}

// Indexed set of simple data, with simple indices
// Map<KeyType, Set<SetType>>
template <typename KeyType, typename SetType>
typename std::enable_if<!is_std_tuple<KeyType>::value && !is_std_tuple<SetType>::value, bool>::type
DataPortalRepn::get(const std::string& name, std::map<KeyType,std::set<SetType>>& data)
{
auto it = indexed_set_data.find(name);
if (it == indexed_set_data.end())
    return false;

data.clear();
for (auto& v : it->second)
    data.emplace( std::get<KeyType>(v.first), std::get<std::set<SetType>>(v.second) );

return true;
}

// Indexed set of simple data, with tuple indices
// Map<tuple<KEYTYPES...>, Set<SetType>>
template <typename SetType, typename ...KEYTYPES>
typename std::enable_if<!is_std_tuple<SetType>::value, bool>::type
DataPortalRepn::get(const std::string& name, std::map<std::tuple<KEYTYPES...>,std::set<SetType>>& data)
{
auto it = indexed_set_data.find(name);
if (it == indexed_set_data.end())
    return false;

data.clear();
for (auto& v : it->second)
    data.emplace( convert_to_tuple<KEYTYPES...>(std::get<TupleType>(v.first)), std::get<std::set<SetType>>(v.second) );

return true;
}

// Indexed set of tuple data, with simple indices
// Map<KeyType, Set<std::tuple<SETTYPES...>>>
template <typename KeyType, typename ...SETTYPES>
typename std::enable_if<!is_std_tuple<KeyType>::value, bool>::type
DataPortalRepn::get(const std::string& name, std::map<KeyType,std::set<std::tuple<SETTYPES...>>>& data)
{
auto it = indexed_set_data.find(name);
if (it == indexed_set_data.end())
    return false;

data.clear();
for (auto& v : it->second) {
    std::set<std::tuple<SETTYPES...>> tmp;
    for (auto& sv : std::get<TupleSetType>(v.second))
        tmp.insert( convert_to_tuple<SETTYPES...>(sv) );

    data.emplace( std::get<KeyType>(v.first), tmp );
    }

return true;
}

// Indexed set of tuple data, with tuple indices
// Map<std::tuple<KEYTYPES...>, Set<std::tuple<SETTYPES...>>>
template <typename ...KEYTYPES, typename ...SETTYPES>
bool
DataPortalRepn::get(const std::string& name, std::map<std::tuple<KEYTYPES...>,std::set<std::tuple<SETTYPES...>>>& data)
{
auto it = indexed_set_data.find(name);
if (it == indexed_set_data.end())
    return false;

data.clear();
for (auto& v : it->second) {
    std::set<std::tuple<SETTYPES...>> tmp;
    for (auto& sv : std::get<TupleSetType>(v.second))
        tmp.insert( convert_to_tuple<SETTYPES...>(sv) );

    data.emplace( convert_to_tuple<KEYTYPES...>(std::get<TupleType>(v.first)), tmp );
    }

return true;
}

// Parameter data
template <typename TYPE>
typename std::enable_if<!is_std_map<TYPE>::value && !is_std_set<TYPE>::value, bool>::type
DataPortalRepn::get(const std::string& name, TYPE& data) const
{
auto it = parameter_data.find(name);
if (it == parameter_data.end()) {
    // TODO - raise an exception
    //std::cerr << "ERROR - missing data '" << name << "'" << std::endl;
    return false;
    }

if (not std::holds_alternative<TYPE>(it->second)) {
    // TODO - raise an exception
    //std::cerr << "ERROR - data '" << name << "' is not a " << type_name<TYPE>() << " value." << std::endl;;
    return false;
    }

data = std::get<TYPE>(it->second);

return true;
}

// Indexed parameter data with simple indices
// Map<Key,Value> - Indexed parameter data 
template <typename KeyType, typename ValueType>
typename std::enable_if<!is_std_tuple<KeyType>::value && !is_std_set<ValueType>::value && !is_std_tuple<ValueType>::value, bool>::type
DataPortalRepn::get(const std::string& name, std::map<KeyType,ValueType>& data) const
{
auto it = indexed_parameter_data.find(name);
if (it == indexed_parameter_data.end()) {
    // TODO - raise an exception
    //std::cerr << "ERROR - missing data '" << name << "'" << std::endl;
    return false;
    }

data.clear();
for (auto& v : it->second)
    data.emplace( std::get<KeyType>(v.first), std::get<ValueType>(v.second) );

return true;
}

// Indexed parameter data with tuple indices
// Map<tuple<KEYTYPES...>,Value> - Indexed parameter data 
template <typename ValueType, typename ...KEYTYPES>
typename std::enable_if<!is_std_set<ValueType>::value && !is_std_tuple<ValueType>::value, bool>::type
DataPortalRepn::get(const std::string& name, std::map<std::tuple<KEYTYPES...>,ValueType>& data) const
{
auto it = indexed_parameter_data.find(name);
if (it == indexed_parameter_data.end()) {
    // TODO - raise an exception
    //std::cerr << "ERROR - missing data '" << name << "'" << std::endl;
    return false;
    }

data.clear();
for (auto& v : it->second)
    data.emplace( convert_to_tuple<KEYTYPES...>(std::get<TupleType>(v.first)), std::get<ValueType>(v.second) );

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
    DataPortal()
        { repn = std::make_shared<DataPortalRepn>(); }

    /** Initialize a data portal from a file. */
    void load_from_file(const std::string& filename)
        { repn->load_from_file(filename); }

    /** Initialize a data portal from a JSON string. */
    void load_from_json_string(const std::string& json_string)
        { repn->load_from_json_string(json_string); }

    // Set data

    // Set of simple data
    // Set<TYPE> data
    template <typename TYPE>
    bool
    get(const std::string& name, std::set<TYPE>& data) const
        { return repn->get(name, data); }

    // Set of tuple data
    // Set<tuple<ARGTYPES ...>> data
    template <typename... ARGTYPES>
    bool
    get(const std::string& name, std::set<std::tuple<ARGTYPES ...>>& data) const
        { return repn->get(name, data); }

    // Indexed set of simple data, with simple indices
    // Map<KeyType, Set<SetType>>
    template <typename KeyType, typename SetType>
    typename std::enable_if<!is_std_tuple<KeyType>::value && !is_std_tuple<SetType>::value, bool>::type
    get(const std::string& name, std::map<KeyType,std::set<SetType>>& data)
        { return repn->get(name, data); }

    // Indexed set of simple data, with tuple indices
    // Map<tuple<KEYTYPES...>, Set<SetType>>
    template <typename SetType, typename ...KEYTYPES>
    typename std::enable_if<!is_std_tuple<SetType>::value, bool>::type
    get(const std::string& name, std::map<std::tuple<KEYTYPES...>,std::set<SetType>>& data)
        { return repn->get(name, data); }

    // Indexed set of tuple data, with simple indices
    // Map<KeyType, Set<std::tuple<SETTYPES...>>>
    template <typename KeyType, typename ...SETTYPES>
    typename std::enable_if<!is_std_tuple<KeyType>::value, bool>::type
    get(const std::string& name, std::map<KeyType,std::set<std::tuple<SETTYPES...>>>& data)
        { return repn->get(name, data); }

    // Indexed set of tuple data, with tuple indices
    // Map<std::tuple<KEYTYPES...>, Set<std::tuple<SETTYPES...>>>
    template <typename ...KEYTYPES, typename ...SETTYPES>
    bool
    get(const std::string& name, std::map<std::tuple<KEYTYPES...>,std::set<std::tuple<SETTYPES...>>>& data)
        { return repn->get(name, data); }

    // Parameter data

    // Parameter data
    template <typename TYPE>
    typename std::enable_if<!is_std_map<TYPE>::value && !is_std_set<TYPE>::value, bool>::type
    get(const std::string& name, TYPE& data) const
        { return repn->get(name, data); }

    // Indexed parameter data
    template <typename KeyType, typename ValueType>
    typename std::enable_if<!is_std_tuple<KeyType>::value && !is_std_set<ValueType>::value && !is_std_tuple<ValueType>::value, bool>::type
    get(const std::string& name, std::map<KeyType,ValueType>& data)
        { return repn->get(name, data); }

    // Indexed parameter data
    template <typename ValueType, typename ...KEYTYPES>
    typename std::enable_if<!is_std_set<ValueType>::value && !is_std_tuple<ValueType>::value, bool>::type
    get(const std::string& name, std::map<std::tuple<KEYTYPES...>,ValueType>& data) const
        { return repn->get(name, data); }
};

}  // namespace coek
