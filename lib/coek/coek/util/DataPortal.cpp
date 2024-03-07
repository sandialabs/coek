//#include <cstdio>
//#include <deque>
//#include <exception>
//#include <iostream>
//#include <list>
//#include <map>
//#include <set>
//#include <sstream>
//#include <stack>
//#include <string>
#include <coek/util/DataPortal.hpp>
#ifdef WITH_RAPIDJSON
#    include <rapidjson/document.h>
#    include <rapidjson/error/en.h>
#    include <rapidjson/filereadstream.h>
#endif

#define RUNTIME_ASSERT(flag, msg) \
    if (not(flag)) throw std::runtime_error(msg)

namespace coek {

#ifdef WITH_RAPIDJSON
namespace {


template <typename T>
std::vector<std::variant<std::string,int>> get_tuple_repn(T& obj, int ctr)
{
std::vector<std::variant<std::string,int>> tmp;
for (auto& val : obj.GetArray()) {
    if (val.IsString())
        tmp.push_back(val.GetString());
    else if (val.IsInt())
        tmp.push_back(val.GetInt());
    else
        RUNTIME_ASSERT(false, "Unexpected tuple value (value " + std::to_string(ctr) + ")");
    }

return tmp;
}

//
// Load set data
//
template <typename T>
void load_set_data(T& obj, DataPortalRepn::SetTypes& set_variant)
{
int type=0;
for (auto& item : obj.GetArray()) {
    if (item.IsString()) type=1;
    if (item.IsInt()) type=2;
    if (item.IsArray()) type=3;
    break;
    }
if (type == 0) {
    std::string tmp = obj.GetString();
    throw std::runtime_error("Error processing data for " + tmp + ": unexpected data type.");
    }
else if (type == 1) {
    //
    // Set of strings
    //
    std::set<std::string> data;
    int ctr=0;
    for (auto& item : obj.GetArray()) {
        RUNTIME_ASSERT(item.IsString(), "Unexpected non-string value (value " + std::to_string(ctr) + ")");
        ctr++;
        data.insert(item.GetString());
        }
    set_variant = data;
    }
else if (type == 2) {
    //
    // Set of integers
    //
    std::set<int> data;
    int ctr=0;
    for (auto& item : obj.GetArray()) {
        RUNTIME_ASSERT(item.IsInt(), "Unexpected non-integer value (value " + std::to_string(ctr) + ")");
        ctr++;
        data.insert(item.GetInt());
        }
    set_variant = data;
    }
else if (type == 3) {
    //
    // Set of tuples
    //
    std::set<std::vector<std::variant<std::string,int>>> data;
    int ctr=0;
    for (auto& item : obj.GetArray()) {
        RUNTIME_ASSERT(item.IsArray(), "Unexpected non-tuple value (value " + std::to_string(ctr) + ")");
        ctr++;

        data.insert( get_tuple_repn(item, ctr) );
        }
    set_variant = data;
    }
}

//
// Process JPOF file version 20210301
//
void initialize_from_dom(coek::DataPortalRepn& repn, rapidjson::Document& doc)
{
//int i=0;
for (auto& v : doc.GetObject()) {
    //std::cout << i++ << " " << v.name.GetString() << std::endl;
    std::string name = v.name.GetString();

    if (v.value.IsArray()) {
        coek::DataPortalRepn::SetTypes data;
        load_set_data(v.value, data);
        repn.set_data[name] = data;
        }

    else if (v.value.IsString()) {
        std::string tmp = v.value.GetString();
        repn.parameter_data[name] = tmp;
        }

    else if (v.value.IsInt()) {
        int tmp = v.value.GetInt();
        repn.parameter_data[name] = tmp;
        }

    else if (v.value.IsObject()) {
        RUNTIME_ASSERT(v.value.HasMember("key_types"), "Missing 'key_types' in indexed data representation");
        RUNTIME_ASSERT(v.value.HasMember("value_types"), "Missing 'value_types' in indexed data representation");
        RUNTIME_ASSERT(v.value.HasMember("data"), "Missing 'data' in indexed data representation");
        RUNTIME_ASSERT(v.value["data"].IsArray(), "Expected 'data' in indexed data representation to be an array");

        bool is_param = true;
        std::map<DataPortalRepn::KeyTypes,DataPortalRepn::ParameterTypes> data;

        std::map<DataPortalRepn::KeyTypes,DataPortalRepn::SetTypes> set_data;
#if 0
        coek::DataPortalRepn::KeyTypes key;
        coek::DataPortalRepn::SetTypes value;
        //set_data.insert(key, value);

        std::map<DataPortalRepn::KeyTypes,int> set_data2;
        coek::DataPortalRepn::KeyTypes key2;
        int value2;
        set_data2.insert({key2, value2});
#endif
        int ctr=0;
        for (auto& kv : v.value["data"].GetArray()) {
            RUNTIME_ASSERT(kv.HasMember("k"), "Missing 'key'");
            RUNTIME_ASSERT(kv.HasMember("v"), "Missing 'value'");
            if (kv["k"].IsArray()) {
                if (kv["v"].IsString())
                    data.emplace( get_tuple_repn(kv["k"], ctr), kv["v"].GetString() );
                else if (kv["v"].IsInt())
                    data.emplace( get_tuple_repn(kv["k"], ctr), kv["v"].GetInt() );
                else if (kv["v"].IsArray()) {
                    is_param = false;
                    coek::DataPortalRepn::SetTypes value;
                    load_set_data(kv["v"], value);
                    set_data.emplace(get_tuple_repn(kv["k"], ctr), value);
                    }
                else
                    RUNTIME_ASSERT(false, "Unexpected value type");
                }
            else if (kv["k"].IsString()) {
                if (kv["v"].IsString())
                    data.emplace( kv["k"].GetString(), kv["v"].GetString() );
                else if (kv["v"].IsInt())
                    data.emplace( kv["k"].GetString(), kv["v"].GetInt() );
                else if (kv["v"].IsArray()) {
                    is_param = false;
                    coek::DataPortalRepn::SetTypes value;
                    load_set_data(kv["v"], value);
                    set_data.emplace(kv["k"].GetString(), value);
                    }
                else
                    RUNTIME_ASSERT(false, "Unexpected value type");
                }
            else if (kv["k"].IsInt()) {
                if (kv["v"].IsString())
                    data.emplace( kv["k"].GetInt(), kv["v"].GetString() );
                else if (kv["v"].IsInt())
                    data.emplace( kv["k"].GetInt(), kv["v"].GetInt() );
                else if (kv["v"].IsArray()) {
                    is_param = false;
                    //coek::DataPortalRepn::KeyTypes key = kv["k"].GetInt();
                    coek::DataPortalRepn::SetTypes value;
                    load_set_data(kv["v"], value);
                    set_data.emplace(kv["k"].GetInt(), value);
                    }
                else
                    RUNTIME_ASSERT(false, "Unexpected value type");
                }
            else 
                RUNTIME_ASSERT(false, "Unexpected key type");

            ctr++;
            }

        if (is_param)
            repn.indexed_parameter_data[name] = data;
        else
            repn.indexed_set_data[name] = set_data;
        }
    }
}

}
#endif

void DataPortalRepn::load_from_file(const std::string& fname)
{
#ifdef WITH_RAPIDJSON
    FILE* fp = fopen(fname.c_str(), "r");
    if (!fp) throw std::runtime_error("Unknown file: " + fname);

    char readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

    rapidjson::Document d;
    d.ParseStream(is);
    if (d.HasParseError()) {
        std::string msg(rapidjson::GetParseError_En(d.GetParseError()));
        throw std::runtime_error("Error parsing JPOF file (offset "
                                 + std::to_string((unsigned)d.GetErrorOffset()) + "): " + msg);
    }

    fclose(fp);

    initialize_from_dom(*this, d);
#else
    throw std::runtime_error("Must install RapidJSON to initialize a DataPortal object.");
#endif
}

void DataPortalRepn::load_from_json_string(const std::string& json_string)
{
#ifdef WITH_RAPIDJSON
    rapidjson::Document d;
    d.Parse(json_string.c_str());
    if (d.HasParseError()) {
        std::string msg(rapidjson::GetParseError_En(d.GetParseError()));
        throw std::runtime_error("Error parsing JSON string (offset "
                                 + std::to_string((unsigned)d.GetErrorOffset()) + "): " + msg);
    }

    initialize_from_dom(*this, d);
#else
    // GCOVR_EXCL_START
    throw std::runtime_error("Must install RapidJSON to parse a JSON string.");
// GCOVR_EXCL_STOP
#endif
}

}  // namespace coek
