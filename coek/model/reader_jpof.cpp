#include <map>
#include <string>
#include <exception>
#include <iostream>
#include <cstdio>
#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
 
#include "coek/api/expression.hpp"
#include "coek/api/objective.hpp"
#include "coek/api/constraint.hpp"
#include "coek/coek_model.hpp"

/*
#include "../ast/visitor_fns.hpp"
#include "../ast/value_terms.hpp"
#include "coek/api/expression.hpp"
#include "coek/api/objective.hpp"
#include "coek/api/constraint.hpp"
#include "coek/api/expression_visitor.hpp"
#ifdef COEK_WITH_COMPACT_MODEL
#include "coek/compact/objective_sequence.hpp"
#include "coek/compact/constraint_sequence.hpp"
#endif
*/

namespace coek {

namespace {

// Process JPOF file version 1
void process_jpof_v1(rapidjson::Document& doc, Model& model, std::map<int,int>& vmap)
{
std::map<int,Variable> jpof_vmap;
std::map<int,Parameter> jpof_pmap;

const rapidjson::Value& mdoc = doc["model"];

// Process variables
assert(mdoc["var"].IsArray());
int ctr=0;
for (auto& var : mdoc["var"].GetArray()) {

    double value=0.0;
    if (var.HasMember("value")) {
        if (var["value"].IsInt())
            value = var["value"].GetInt();
        else if (var["value"].IsDouble())
            value = var["value"].GetDouble();
        else
            throw std::runtime_error("Error processing variable "+std::to_string(ctr)+": Non-numeric value for variable value");
        }

    double lb=-COEK_INFINITY;
    if (var.HasMember("lb")) {
        if (var["lb"].IsInt())
            lb = var["lb"].GetInt();
        else if (var["lb"].IsDouble())
            lb = var["lb"].GetDouble();
        else
            throw std::runtime_error("Error processing variable "+std::to_string(ctr)+": Non-numeric value for variable lb");
        }

    double ub=COEK_INFINITY;
    if (var.HasMember("ub")) {
        if (var["ub"].IsInt())
            ub = var["ub"].GetInt();
        else if (var["ub"].IsDouble())
            ub = var["ub"].GetDouble();
        else
            throw std::runtime_error("Error processing variable "+std::to_string(ctr)+": Non-numeric value for variable ub");
        }

    std::string vtype = "R";
    bool binary=false;
    bool integer=false;
    if (var.HasMember("type")) {
        if (var["type"].IsString())
            vtype = var["vtype"].GetString();
        else
            throw std::runtime_error("Error processing variable "+std::to_string(ctr)+": Non-string value for variable type");
        }
    if (vtype == "B")
        binary = true;
    else if (vtype == "Z")
        integer = true;

    std::string label;
    if (var.HasMember("label")) {
        if (var["label"].IsString())
            label = var["label"].GetString();
        else
            throw std::runtime_error("Error processing variable "+std::to_string(ctr)+": Non-string value for variable label");
        }

    bool fixed=false;
    if (var.HasMember("fixed")) {
        if (var["fixed"].IsInt())
            fixed = var["value"].GetInt();
        else
            throw std::runtime_error("Error processing variable "+std::to_string(ctr)+": Non-integer value for variable fixed flag");
        }

    auto v = model.add_variable(lb, ub, value, binary, integer);
    if (not (label == ""))
        v.set_name(label);
    v.set_fixed(fixed);

    if (var.HasMember("id")) {
        if (var["id"].IsUint()) {
            int tmp = var["id"].GetUint();
            vmap[v.id()] = tmp;
            jpof_vmap[tmp] = v;
            }
        else
            throw std::runtime_error("Error processing variable "+std::to_string(ctr)+": Variable id must be an unsigned integer");
        }
    else
        throw std::runtime_error("Error processing variable "+std::to_string(ctr)+": Variable id is required");

    ctr++;
    }

// Process variables
if (mdoc.HasMember("param")) {
    assert(mdoc["param"].IsArray());
    int ctr=0;
    for (auto& param : mdoc["param"].GetArray()) {

        double value=0.0;
        if (param.HasMember("value")) {
            if (param["value"].IsInt())
                value = param["value"].GetInt();
            else if (param["value"].IsDouble())
                value = param["value"].GetDouble();
            else
                throw std::runtime_error("Error processing parameter "+std::to_string(ctr)+": Non-numeric value for parameter value");
            }

        Parameter p(value);

        if (param.HasMember("label")) {
            if (param["label"].IsString())
                p.set_name( param["label"].GetString() );
            else
                throw std::runtime_error("Error processing parameter "+std::to_string(ctr)+": Non-string value for parameter label");
            }

        if (param.HasMember("id")) {
            if (param["id"].IsInt())
                jpof_pmap[ param["id"].GetInt() ] = p;
            else
                throw std::runtime_error("Error processing parameter "+std::to_string(ctr)+": Non-integer value for parameter id");
            }
        else
            throw std::runtime_error("Error processing parameter "+std::to_string(ctr)+": Parameter id is required");

        ctr++;
        }
    }
}

}


Model read_problem(std::string& fname, std::map<int,int>& vmap)
{
FILE* fp = fopen(fname.c_str(), "r");
 
char readBuffer[65536];
rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
 
rapidjson::Document d;
d.ParseStream(is);
if (d.HasParseError()) {
    std::string msg(rapidjson::GetParseError_En(d.GetParseError()));
    throw std::runtime_error("Error parsing JPOF file (offset " + std::to_string((unsigned)d.GetErrorOffset()) +"): "+msg);
    }
 
fclose(fp);

// DOM sanity checks
assert(d.IsObject());
assert(d.HasMember("__metadata__"));
assert(d["__metadata__"].IsObject());
assert(d["__metadata__"].HasMember("version"));
assert(d["__metadata__"]["version"].IsInt());
assert(d["__metadata__"].HasMember("format"));
assert(d["__metadata__"]["format"].IsString());
std::string format = d["__metadata__"]["format"].GetString();
if (format != "JSON Parameterized Optimization Format")
    throw std::runtime_error("Unexpected JSON file format: "+format);
assert(d.HasMember("model"));
if (not d["model"].HasMember("var"))
    throw std::runtime_error("JPOF model without \"var\" data: A JPOF model must contain variables");

// Process DOM data to populate a model
Model model;
int version = d["__metadata__"]["version"].GetInt();
if (version == 1)
    process_jpof_v1(d, model, vmap);
else
    throw std::runtime_error("Unexpected JPOF format version: "+std::to_string(version));

return model;
}

}
