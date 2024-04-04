#include <coek/util/DataPortal.hpp>
#ifdef WITH_RAPIDJSON
#    include <rapidjson/document.h>
#    include <rapidjson/error/en.h>
#    include <rapidjson/filereadstream.h>
#    include <rapidjson/filewritestream.h>
#    include <rapidjson/writer.h>
#    include <rapidjson/prettywriter.h>
#endif

#define RUNTIME_ASSERT(flag, msg) \
    if (not(flag))                \
    throw std::runtime_error(msg)

namespace coek {

#ifdef WITH_RAPIDJSON
namespace {

template <typename T>
DataPortalRepn::TupleKeyType get_tuple_key(T& obj, std::vector<std::string>& key_type, int ctr)
{
    DataPortalRepn::TupleKeyType tmp;
    size_t i = 0;
    for (auto& val : obj.GetArray()) {
        if (key_type[i] == "s") {
            RUNTIME_ASSERT(val.IsString(), "Unexpected tuple type (key " + std::to_string(i)
                                               + " in tuple " + std::to_string(ctr) + ")");
            tmp.push_back(val.GetString());
        }
        else if (key_type[i] == "i") {
            RUNTIME_ASSERT(val.IsInt(), "Unexpected tuple type (key " + std::to_string(i)
                                            + " in tuple " + std::to_string(ctr) + ")");
            tmp.push_back(val.GetInt());
        }
        else
            RUNTIME_ASSERT(false, "Unexpected tuple value (value " + std::to_string(ctr) + ")");
        i++;
    }

    return tmp;
}

template <typename T>
DataPortalRepn::TupleValueType get_tuple_value(T& obj, std::vector<std::string>& set_type, int ctr)
{
    DataPortalRepn::TupleValueType tmp;
    size_t i = 0;
    for (auto& val : obj.GetArray()) {
        if (set_type[i] == "s") {
            RUNTIME_ASSERT(val.IsString(), "Unexpected tuple type (key " + std::to_string(i)
                                               + " in tuple " + std::to_string(ctr) + ")");
            tmp.push_back(val.GetString());
        }
        else if (set_type[i] == "i") {
            RUNTIME_ASSERT(val.IsInt(), "Unexpected tuple type (key " + std::to_string(i)
                                            + " in tuple " + std::to_string(ctr) + ")");
            tmp.push_back(val.GetInt());
        }
        else if (set_type[i] == "d") {
            if (val.IsDouble())
                tmp.push_back(val.GetDouble());
            else if (val.IsInt()) {
                double v = val.GetInt();
                tmp.push_back(v);
            }
            else
                RUNTIME_ASSERT(false, "Unexpected tuple type (key " + std::to_string(i)
                                          + " in tuple " + std::to_string(ctr) + ")");
        }
        else
            RUNTIME_ASSERT(false, "Unexpected tuple type (value " + std::to_string(ctr) + ")");
        i++;
    }

    return tmp;
}

template <typename T>
std::vector<std::string> load_type(T& obj)
{
    std::vector<std::string> ans;
    if (obj.IsArray()) {
        for (auto& item : obj.GetArray()) {
            RUNTIME_ASSERT(item.IsString(), "Unexpected JSON data while loading type information.");
            ans.push_back(item.GetString());
        }
    }
    else if (obj.IsString()) {
        ans.push_back(obj.GetString());
    }
    else
        throw std::runtime_error("Unexpected JSON data while loading type information.");
    return ans;
}

//
// Load set data - Single set
//
template <typename T>
void load_set_data(T& obj, DataPortalRepn::SetTypes& set_variant,
                   std::vector<std::string>& set_type)
{
    RUNTIME_ASSERT(obj.IsArray(), "Expecting an array of set data.");
    RUNTIME_ASSERT(set_type.size() > 0, "Error processing set data empty set type specification.");

    if (set_type.size() == 1) {
        if (set_type[0] == "s") {
            //
            // Set of strings
            //
            std::set<std::string> data;
            int ctr = 0;
            for (auto& item : obj.GetArray()) {
                RUNTIME_ASSERT(item.IsString(),
                               "Unexpected non-string value (value " + std::to_string(ctr) + ")");
                ctr++;
                data.insert(item.GetString());
            }
            set_variant = data;
        }
        else if (set_type[0] == "i") {
            //
            // Set of integers
            //
            std::set<int> data;
            int ctr = 0;
            for (auto& item : obj.GetArray()) {
                RUNTIME_ASSERT(item.IsInt(),
                               "Unexpected non-integer value (value " + std::to_string(ctr) + ")");
                ctr++;
                data.insert(item.GetInt());
            }
            set_variant = data;
        }
        else if (set_type[0] == "d") {
            //
            // Set of doubles
            //
            std::set<double> data;
            int ctr = 0;
            for (auto& item : obj.GetArray()) {
                if (item.IsInt())
                    data.insert(item.GetInt());
                else if (item.IsDouble())
                    data.insert(item.GetDouble());
                else
                    RUNTIME_ASSERT(
                        false, "Unexpected non-double value (value " + std::to_string(ctr) + ")");
                ctr++;
            }
            set_variant = data;
        }
    }
    else {
        //
        // Set of tuples
        //
        std::set<DataPortalRepn::TupleValueType> data;
        int ctr = 0;
        for (auto& item : obj.GetArray()) {
            RUNTIME_ASSERT(item.IsArray(),
                           "Unexpected non-tuple value (value " + std::to_string(ctr) + ")");
            ctr++;

            data.insert(get_tuple_value(item, set_type, ctr));
        }
        set_variant = data;
    }
}

//
// Load set data - Indexed sets
//
template <typename T>
void load_set_data(T& obj, std::map<DataPortalRepn::KeyTypes, DataPortalRepn::SetTypes>& data,
                   std::vector<std::string>& key_type, std::vector<std::string>& set_type)
{
    RUNTIME_ASSERT(obj.IsArray(), "Expecting an array of set data.");
    RUNTIME_ASSERT(set_type.size() > 0, "Error processing set data empty set type specification.");
    RUNTIME_ASSERT(key_type.size() > 0, "Error processing set data empty key type specification.");

    int ctr = 0;
    for (auto& item : obj.GetArray()) {
        RUNTIME_ASSERT(item.IsArray(), "Index value must be an array.");
        auto tmp = item.GetArray();
        RUNTIME_ASSERT(tmp.Size() == 2, "Unexpected data for key-value pair.");

        DataPortalRepn::SetTypes set_variant;
        load_set_data(tmp[1], set_variant, set_type);

        if (key_type.size() == 1) {
            if (key_type[0] == "s") {
                RUNTIME_ASSERT(tmp[0].IsString(),
                               "Unexpected key type (value " + std::to_string(ctr) + ")");
                data.emplace(tmp[0].GetString(), set_variant);
            }
            else if (key_type[0] == "i") {
                RUNTIME_ASSERT(tmp[0].IsInt(),
                               "Unexpected key type (value " + std::to_string(ctr) + ")");
                data.emplace(tmp[0].GetInt(), set_variant);
            }
        }
        else {
            data.emplace(get_tuple_key(tmp[0], key_type, ctr), set_variant);
        }
        ctr++;
    }
}

//
// Load param data - Single parameter
//
template <typename T>
void load_param_data(T& obj, DataPortalRepn::ParameterTypes& param_variant,
                     std::vector<std::string>& param_type)
{
    // RUNTIME_ASSERT(obj.IsArray(), "Expecting an array of parameter data.");
    RUNTIME_ASSERT(param_type.size() > 0,
                   "Error processing parameter data empty type specification.");

    if (param_type.size() == 1) {
        if (param_type[0] == "s") {
            //
            // String parameter
            //
            RUNTIME_ASSERT(obj.IsString(), "Unexpected non-string value");
            param_variant = obj.GetString();
        }
        else if (param_type[0] == "i") {
            //
            // Integer parameter
            //
            RUNTIME_ASSERT(obj.IsInt(), "Unexpected non-integer value");
            param_variant = obj.GetInt();
        }
        else if (param_type[0] == "d") {
            //
            // Double parameter
            //
            if (obj.IsInt())
                param_variant = obj.GetInt();
            else if (obj.IsDouble())
                param_variant = obj.GetDouble();
            else
                RUNTIME_ASSERT(false, "Unexpected non-double value");
        }
    }
    else {
        //
        // Tuple parameter
        //
        DataPortalRepn::TupleValueType data;
        param_variant = get_tuple_value(obj, param_type, 0);
    }
}

//
// Load param data - Indexed parameters
//
template <typename T>
void load_param_data(T& obj,
                     std::map<DataPortalRepn::KeyTypes, DataPortalRepn::ParameterTypes>& data,
                     std::vector<std::string>& key_type, std::vector<std::string>& param_type)
{
    RUNTIME_ASSERT(obj.IsArray(), "Expecting an array of parameter data.");
    RUNTIME_ASSERT(param_type.size() > 0,
                   "Error processing parameter data empty param type specification.");
    RUNTIME_ASSERT(key_type.size() > 0,
                   "Error processing parameter data empty key type specification.");

    int ctr = 0;
    for (auto& item : obj.GetArray()) {
        RUNTIME_ASSERT(item.IsArray(), "Index value must be an array.");
        auto tmp = item.GetArray();
        RUNTIME_ASSERT(tmp.Size() == 2, "Unexpected data for key-value pair.");

        DataPortalRepn::ParameterTypes param_variant;
        load_param_data(tmp[1], param_variant, param_type);

        if (key_type.size() == 1) {
            if (key_type[0] == "s") {
                RUNTIME_ASSERT(tmp[0].IsString(),
                               "Unexpected key type (value " + std::to_string(ctr) + ")");
                data.emplace(tmp[0].GetString(), param_variant);
            }
            else if (key_type[0] == "i") {
                RUNTIME_ASSERT(tmp[0].IsInt(),
                               "Unexpected key type (value " + std::to_string(ctr) + ")");
                data.emplace(tmp[0].GetInt(), param_variant);
            }
        }
        else {
            data.emplace(get_tuple_key(tmp[0], key_type, ctr), param_variant);
        }
        ctr++;
    }
}

//
// Process JSON file
//
void initialize_from_dom(coek::DataPortalRepn& repn, rapidjson::Document& doc)
{
    // int i=0;
    for (auto& v : doc.GetObject()) {
        // std::cout << i++ << " " << v.name.GetString() << std::endl;
        std::string name = v.name.GetString();

        if (v.value.IsArray()) {
            coek::DataPortalRepn::TupleValueType tmp;
            for (auto& val : v.value.GetArray()) {
                if (val.IsString())
                    tmp.push_back(val.GetString());
                else if (val.IsInt())
                    tmp.push_back(val.GetInt());
                else if (val.IsDouble())
                    tmp.push_back(val.GetDouble());
            }
            repn.parameter_data[name] = tmp;
        }

        else if (v.value.IsString()) {
            std::string tmp = v.value.GetString();
            repn.parameter_data[name] = tmp;
        }

        else if (v.value.IsDouble()) {
            double tmp = v.value.GetDouble();
            repn.parameter_data[name] = tmp;
        }

        else if (v.value.IsInt()) {
            int tmp = v.value.GetInt();
            repn.parameter_data[name] = tmp;
        }

        else if (v.value.IsObject()) {
            // RUNTIME_ASSERT(v.value.HasMember("key_type"), "Missing 'key_type' in indexed data
            // representation");
            RUNTIME_ASSERT(v.value.HasMember("data"),
                           "Missing 'data' in indexed data representation");
            RUNTIME_ASSERT(v.value["data"].IsArray(),
                           "Expected 'data' in indexed data representation to be an array");

            if (v.value.HasMember("set_type")) {
                std::vector<std::string> set_type = load_type(v.value["set_type"]);
                if (v.value.HasMember("key_type")) {
                    // Indexed set
                    std::vector<std::string> key_type = load_type(v.value["key_type"]);
                    std::map<DataPortalRepn::KeyTypes, DataPortalRepn::SetTypes> data;
                    load_set_data(v.value["data"], data, key_type, set_type);
                    repn.indexed_set_data[name] = data;
                }
                else {
                    // Set
                    DataPortalRepn::SetTypes data;
                    load_set_data(v.value["data"], data, set_type);
                    repn.set_data[name] = data;
                }
            }
            else {
                // Indexed parameter
                RUNTIME_ASSERT(
                    v.value.HasMember("param_type"),
                    "Missing 'set_type' and 'param_type' in indexed data representation");
                RUNTIME_ASSERT(v.value.HasMember("key_type"),
                               "Missing 'key_type' in indexed data representation");

                std::vector<std::string> key_type = load_type(v.value["key_type"]);
                std::vector<std::string> param_type = load_type(v.value["param_type"]);
                std::map<DataPortalRepn::KeyTypes, DataPortalRepn::ParameterTypes> data;
                load_param_data(v.value["data"], data, key_type, param_type);
                repn.indexed_parameter_data[name] = data;
            }
        }
    }
}

template <typename AllocT>
void value_tuple_to_array(const DataPortalRepn::TupleValueType& tvec, rapidjson::Value& array,
                          rapidjson::Value& tuple_type, AllocT& allocator)
{
    bool first = tuple_type.Size() == 0;
    for (const auto& v : tvec) {
        if (std::holds_alternative<DataPortalRepn::StringType>(v)) {
            if (first)
                tuple_type.PushBack("s", allocator);
            const auto& val = std::get<DataPortalRepn::StringType>(v);
            rapidjson::Value v_(val.c_str(), static_cast<rapidjson::SizeType>(val.size()),
                                allocator);
            array.PushBack(v_, allocator);
        }
        else if (std::holds_alternative<DataPortalRepn::IntegerType>(v)) {
            if (first)
                tuple_type.PushBack("i", allocator);
            int val = std::get<DataPortalRepn::IntegerType>(v);
            rapidjson::Value v_(val);
            array.PushBack(v_, allocator);
        }
        else if (std::holds_alternative<DataPortalRepn::DoubleType>(v)) {
            if (first)
                tuple_type.PushBack("d", allocator);
            double val = std::get<DataPortalRepn::DoubleType>(v);
            rapidjson::Value v_(val);
            array.PushBack(v_, allocator);
        }
    }
}

template <typename AllocT>
void key_tuple_to_array(const DataPortalRepn::TupleKeyType& tvec, rapidjson::Value& array,
                        rapidjson::Value& key_type, AllocT& allocator)
{
    bool first = key_type.Size() == 0;
    for (const auto& v : tvec) {
        if (std::holds_alternative<DataPortalRepn::StringType>(v)) {
            if (first)
                key_type.PushBack("s", allocator);
            const auto& val = std::get<DataPortalRepn::StringType>(v);
            rapidjson::Value v_(val.c_str(), static_cast<rapidjson::SizeType>(val.size()),
                                allocator);
            array.PushBack(v_, allocator);
        }
        else if (std::holds_alternative<DataPortalRepn::IntegerType>(v)) {
            if (first)
                key_type.PushBack("i", allocator);
            int val = std::get<DataPortalRepn::IntegerType>(v);
            rapidjson::Value v_(val);
            array.PushBack(v_, allocator);
        }
    }
}

template <typename AllocT>
void set_to_array(const DataPortalRepn::SetTypes& s, rapidjson::Value& array,
                  rapidjson::Value& set_type, AllocT& allocator)
{
    if (std::holds_alternative<DataPortalRepn::StringSetType>(s)) {
        if (set_type.Size() == 0)
            set_type.PushBack("s", allocator);
        const auto& set_ = std::get<DataPortalRepn::StringSetType>(s);
        for (const std::string& sval : set_) {
            rapidjson::Value v(sval.c_str(), static_cast<rapidjson::SizeType>(sval.size()),
                               allocator);
            array.PushBack(v, allocator);
        }
    }

    else if (std::holds_alternative<DataPortalRepn::IntegerSetType>(s)) {
        if (set_type.Size() == 0)
            set_type.PushBack("i", allocator);
        const auto& set_ = std::get<DataPortalRepn::IntegerSetType>(s);
        for (const auto& sval : set_) {
            rapidjson::Value v(sval);
            array.PushBack(v, allocator);
        }
    }

    else if (std::holds_alternative<DataPortalRepn::DoubleSetType>(s)) {
        if (set_type.Size() == 0)
            set_type.PushBack("d", allocator);
        const auto& set_ = std::get<DataPortalRepn::DoubleSetType>(s);
        for (const auto& sval : set_) {
            rapidjson::Value v(sval);
            array.PushBack(v, allocator);
        }
    }

    else if (std::holds_alternative<DataPortalRepn::TupleSetType>(s)) {
        const auto& set_ = std::get<DataPortalRepn::TupleSetType>(s);
        for (const auto& sval : set_) {
            rapidjson::Value v(rapidjson::kArrayType);
            value_tuple_to_array(sval, v, set_type, allocator);
            array.PushBack(v, allocator);
        }
    }
}

template <typename AllocT>
void param_to_value(const DataPortalRepn::ParameterTypes& p, rapidjson::Value& value,
                    rapidjson::Value& param_type, AllocT& allocator)
{
    if (std::holds_alternative<DataPortalRepn::StringType>(p)) {
        if (param_type.Size() == 0)
            param_type.PushBack("s", allocator);
        const auto& val = std::get<DataPortalRepn::StringType>(p);
        value.SetString(val.c_str(), static_cast<rapidjson::SizeType>(val.size()), allocator);
    }
    else if (std::holds_alternative<DataPortalRepn::IntegerType>(p)) {
        if (param_type.Size() == 0)
            param_type.PushBack("i", allocator);
        int val = std::get<DataPortalRepn::IntegerType>(p);
        value.SetInt(val);
    }
    else if (std::holds_alternative<DataPortalRepn::DoubleType>(p)) {
        if (param_type.Size() == 0)
            param_type.PushBack("d", allocator);
        double val = std::get<DataPortalRepn::DoubleType>(p);
        value.SetDouble(val);
    }
    else if (std::holds_alternative<DataPortalRepn::TupleValueType>(p)) {
        value.SetArray();
        value_tuple_to_array(std::get<DataPortalRepn::TupleValueType>(p), value, param_type,
                             allocator);
    }
}

//
// Create JSON DOM
//
void initialize_to(const coek::DataPortalRepn& repn, rapidjson::Document& doc)
{
    // Top-level object
    doc.SetObject();

    // must pass an allocator when the object may need to allocate memory
    auto& allocator = doc.GetAllocator();

    for (const auto& s : repn.set_data) {
        rapidjson::Value k(s.first.c_str(), static_cast<rapidjson::SizeType>(s.first.size()),
                           allocator);
        rapidjson::Value obj(rapidjson::kObjectType);
        //
        rapidjson::Value set_type(rapidjson::kArrayType);
        rapidjson::Value array(rapidjson::kArrayType);
        set_to_array(s.second, array, set_type, allocator);
        obj.AddMember("set_type", set_type, allocator);
        obj.AddMember("data", array, allocator);
        //
        doc.AddMember(k, obj, allocator);
    }

    for (const auto& is : repn.indexed_set_data) {
        rapidjson::Value k(is.first.c_str(), static_cast<rapidjson::SizeType>(is.first.size()),
                           allocator);
        rapidjson::Value obj(rapidjson::kObjectType);
        //
        rapidjson::Value key_type(rapidjson::kArrayType);
        rapidjson::Value set_type(rapidjson::kArrayType);
        rapidjson::Value data(rapidjson::kArrayType);

        for (const auto& kv : is.second) {
            rapidjson::Value kv_array(rapidjson::kArrayType);
            if (std::holds_alternative<DataPortalRepn::StringType>(kv.first)) {
                if (key_type.Size() == 0)
                    key_type.PushBack("s", allocator);
                const auto& val = std::get<DataPortalRepn::StringType>(kv.first);
                rapidjson::Value key(val.c_str(), static_cast<rapidjson::SizeType>(val.size()),
                                     allocator);
                kv_array.PushBack(key, allocator);
            }
            else if (std::holds_alternative<DataPortalRepn::IntegerType>(kv.first)) {
                if (key_type.Size() == 0)
                    key_type.PushBack("i", allocator);
                int val = std::get<DataPortalRepn::IntegerType>(kv.first);
                rapidjson::Value key(val);
                kv_array.PushBack(key, allocator);
            }
            else if (std::holds_alternative<DataPortalRepn::TupleKeyType>(kv.first)) {
                rapidjson::Value key(rapidjson::kArrayType);
                key_tuple_to_array(std::get<DataPortalRepn::TupleKeyType>(kv.first), key, key_type,
                                   allocator);
                kv_array.PushBack(key, allocator);
            }

            rapidjson::Value value(rapidjson::kArrayType);
            set_to_array(kv.second, value, set_type, allocator);
            kv_array.PushBack(value, allocator);
            data.PushBack(kv_array, allocator);
        }
        obj.AddMember("key_type", key_type, allocator);
        obj.AddMember("set_type", set_type, allocator);
        obj.AddMember("data", data, allocator);
        //
        doc.AddMember(k, obj, allocator);
    }

    for (const auto& p : repn.parameter_data) {
        // std::cout << "PARAM " << p.first << std::endl;
        rapidjson::Value k(p.first.c_str(), static_cast<rapidjson::SizeType>(p.first.size()),
                           allocator);

        rapidjson::Value param_type(rapidjson::kArrayType);
        rapidjson::Value v;
        param_to_value(p.second, v, param_type, allocator);
        doc.AddMember(k, v, allocator);
    }

    for (const auto& ip : repn.indexed_parameter_data) {
        rapidjson::Value k(ip.first.c_str(), static_cast<rapidjson::SizeType>(ip.first.size()),
                           allocator);
        rapidjson::Value obj(rapidjson::kObjectType);
        //
        rapidjson::Value key_type(rapidjson::kArrayType);
        rapidjson::Value param_type(rapidjson::kArrayType);
        rapidjson::Value data(rapidjson::kArrayType);

        for (const auto& kv : ip.second) {
            rapidjson::Value kv_array(rapidjson::kArrayType);
            if (std::holds_alternative<DataPortalRepn::StringType>(kv.first)) {
                if (key_type.Size() == 0)
                    key_type.PushBack("s", allocator);
                const auto& val = std::get<DataPortalRepn::StringType>(kv.first);
                rapidjson::Value key(val.c_str(), static_cast<rapidjson::SizeType>(val.size()),
                                     allocator);
                kv_array.PushBack(key, allocator);
            }
            else if (std::holds_alternative<DataPortalRepn::IntegerType>(kv.first)) {
                if (key_type.Size() == 0)
                    key_type.PushBack("i", allocator);
                int val = std::get<DataPortalRepn::IntegerType>(kv.first);
                rapidjson::Value key(val);
                kv_array.PushBack(key, allocator);
            }
            else if (std::holds_alternative<DataPortalRepn::TupleKeyType>(kv.first)) {
                rapidjson::Value key(rapidjson::kArrayType);
                key_tuple_to_array(std::get<DataPortalRepn::TupleKeyType>(kv.first), key, key_type,
                                   allocator);
                kv_array.PushBack(key, allocator);
            }

            rapidjson::Value value;
            param_to_value(kv.second, value, param_type, allocator);
            kv_array.PushBack(value, allocator);
            data.PushBack(kv_array, allocator);
        }
        obj.AddMember("key_type", key_type, allocator);
        obj.AddMember("param_type", param_type, allocator);
        obj.AddMember("data", data, allocator);
        //
        doc.AddMember(k, obj, allocator);
    }
}

}  // namespace
#endif

void DataPortalRepn::load_from_file(const std::string& fname)
{
#ifdef WITH_RAPIDJSON
    FILE* fp = fopen(fname.c_str(), "r");
    if (!fp)
        throw std::runtime_error("Unknown file: " + fname);

    char buffer[65536];
    rapidjson::FileReadStream is(fp, buffer, sizeof(buffer));

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

void DataPortalRepn::save_to_file(const std::string& fname, unsigned int indent)
{
#ifdef WITH_RAPIDJSON
    rapidjson::Document d;
    initialize_to(*this, d);

    FILE* fp = fopen(fname.c_str(), "w");

    char buffer[65536];
    rapidjson::FileWriteStream os(fp, buffer, sizeof(buffer));
    if (indent == 0) {
        rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
        d.Accept(writer);
    }
    else {
        rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(os);
        writer.SetIndent(' ', indent);
        d.Accept(writer);
    }

    fclose(fp);
#else
    throw std::runtime_error("Must install RapidJSON to save a DataPortal object.");
#endif
}

std::string DataPortalRepn::to_string(unsigned int indent)
{
#ifdef WITH_RAPIDJSON
    rapidjson::Document d;
    initialize_to(*this, d);

    rapidjson::StringBuffer buffer;
    if (indent == 0) {
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        d.Accept(writer);
    }
    else {
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
        writer.SetIndent(' ', indent);
        d.Accept(writer);
    }
    return buffer.GetString();
#else
    // GCOVR_EXCL_START
    throw std::runtime_error(
        "Must install RapidJSON to generate a JSON string for a DataPortal object.");
// GCOVR_EXCL_STOP
#endif
}

}  // namespace coek
