#include <set>
#include <map>
#include <deque>
#include <list>
#include <string>
#include <stack>
#include <exception>
#include <iostream>
#include <sstream>
#include <cstdio>
#ifdef WITH_RAPIDJSON
#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#endif
 
#include "coek/api/intrinsic_fn.hpp"
#include "coek/api/expression.hpp"
#include "coek/api/objective.hpp"
#include "coek/api/constraint.hpp"
#include "coek/coek_model.hpp"

#define RUNTIME_ASSERT(flag, msg)\
if (not (flag))\
    throw std::runtime_error(msg)


namespace coek {

namespace reader_jpof {

std::set<std::string> binary_operators = {"*", "+", "/", "pow"};
std::set<std::string> unary_operators = {"neg", "ceil", "floor", "abs", "exp", "log", "log10", "sqrt", 
                                        "sin", "cos", "tan", "sinh", "cosh", "tanh",
                                        "asin", "acos", "atan", "asinh", "acosh", "atanh"
                                        };
std::map<std::string, Expression (*)(const Expression&)> unary_functions = {
        {"ceil",ceil},
        {"floor",floor},
        {"abs",abs},
        {"exp",exp},
        {"log",log},
        {"log10",log10},
        {"sqrt",sqrt},
        {"sin",sin},
        {"cos",cos},
        {"tan",tan},
        {"sinh",sinh},
        {"cosh",cosh},
        {"tanh",tanh},
        {"asin",asin},
        {"acos",acos},
        {"atan",atan},
        {"asinh",asinh},
        {"acosh",acosh},
        {"atanh",atanh}
        };

class EType
{
public:

    int type;
    int ival;           // 0
    double dval;        // 1
    Variable vval;      // 2
    Parameter pval;     // 3
    Expression eval;    // 4

    EType(int i) : type(0), ival(i) {}
    EType(double d) : type(1), dval(d) {}
    EType(const Variable& v) : type(2), vval(v) {}
    EType(const Parameter& p) : type(3), pval(p) {}
    EType(const Expression& e) : type(4), eval(e) {}

    Expression as_expression()
        {
        Expression e;
        switch (type) {
            case 0: e=Expression(ival); break;
            case 1: e=Expression(dval); break;
            case 2: e=Expression(vval); break;
            case 3: e=Expression(pval); break;
            case 4: e=Expression(eval); break;
            };
        return e;
        }
};

void split(const std::string& s, char delimiter, std::vector<std::string>& tokens)
{
std::string token;
std::istringstream tokenStream(s);
while (std::getline(tokenStream, token, delimiter)) {
    tokens.push_back(token);
    }
}

Expression create_expression(const std::string& expr, std::map<int,Variable>& jpof_vmap, std::map<int,Parameter>& jpof_pmap)
{
// Split the expression string by commas
std::vector<std::string> tokens;
split(expr, ',', tokens);

size_t curr = 0;
std::stack<int> estack;     // stack of expression strings
std::stack<EType> args;     // stack of arguments that need to be processed
std::stack<int> nargs;      // # of arguments that still need to be generated
nargs.push( 1 );
nargs.push( 1 );

while (curr < tokens.size()) {
    //
    // Process the next token
    //
    std::string& op = tokens[curr];
    if (op == "V") {
        curr++;
        int id = stoi(tokens[curr]);                // TODO - error check
        args.push( EType(jpof_vmap[id]) );     // TODO - error check
        nargs.top()--;
        curr++;
        }
    else if (op == "P") {
        curr++;
        int id = stoi(tokens[curr]);                // TODO - error check
        args.push( EType(jpof_pmap[id]) );     // TODO - error check
        nargs.top()--;
        curr++;
        }
    else if (op == "N") {
        curr++;
        double dval = stod(tokens[curr]);           // TODO - error check
        args.push( EType(dval) );
        nargs.top()--;
        curr++;
        }
    else {
        estack.push(curr);
        curr++;
        if (binary_operators.find(op) != binary_operators.end()) {
            nargs.push( 2 );
            nargs.push( 2 );
            }
        else if (unary_operators.find(op) != unary_operators.end()) {
            nargs.push( 1 );
            nargs.push( 1 );
            }
        else if (op == "sum") {
            int ival = stoi(tokens[curr]);          // TODO - error check
            nargs.push( ival );
            nargs.push( ival );
            curr++;
            }
        else
            throw std::runtime_error("Error generating expression: token="+std::to_string(curr)+" op="+op);
        }
    //
    // If we have completed an operator, create the expression
    //
    // Keep iterating through completed operators, since we may
    // be at the end of the expression.
    //
    while (nargs.top() == 0) {
        nargs.pop();
        int n = nargs.top(); // # of arguments to collect from the end of args
        nargs.pop();

        if (estack.size() == 0)
            break;
        std::string& op = tokens[estack.top()];
        estack.pop();

        if (n == 2) {
            // Process a binary operator
            EType arg2 = args.top();
            args.pop();
            EType arg1 = args.top();
            args.pop();
            if (op == "+")
                args.push( arg1.as_expression() + arg2.as_expression() );
            else if (op == "/")
                args.push( arg1.as_expression() / arg2.as_expression() );
            else if (op == "*")
                args.push( arg1.as_expression() * arg2.as_expression() );
            else if (op == "pow")
                args.push( pow(arg1.as_expression(), arg2.as_expression()) );
            }
        else if (n == 1) {
            // Process a unary operator
            if (op == "neg") {
                EType arg = args.top();
                args.pop();
                Expression e = - arg.as_expression();
                args.push(e);
                }
            else {
                EType arg = args.top();
                args.pop();
                Expression e = unary_functions[op]( arg.as_expression() );      // ERROR CHECK
                args.push(e);
                }
            }
        else if (op == "sum") {
#if 0
            // Process a sum operator
            Expression e;
            for (int i=0; i<n; i++) {
                EType arg = args.top();
                args.pop();
                if (i == 0)             // Avoid having a zero in the expression
                    e = arg.as_expression();
                else                    // Keep things in order
                    e = arg.as_expression() + e;
                }
#else
            if (n == 0) {
                Expression e(0);
                args.push(e);
                }
            else {
                // Collect the arguments and then sum them in order
                std::vector<Expression> arg(n);
                for (int i=n-1; i>=0; i--) {
                    EType a = args.top();
                    args.pop();
                    arg[i] = a.as_expression();
                    }
                Expression e = arg[0];
                for (int i=1; i<n; i++)
                    e += arg[i];
                args.push(e);
                }
#endif
            }
        
        nargs.top()--;
        }
    }

EType arg = args.top();
args.pop();
return arg.as_expression();
}

#ifdef WITH_RAPIDJSON
//
// Process JPOF file version 20210301
//
void process_jpof_v20210301(rapidjson::Document& doc, Model& model, std::map<int,int>& vmap)
{
std::map<int,Variable> jpof_vmap;
std::map<int,Parameter> jpof_pmap;

const rapidjson::Value& mdoc = doc["model"];

//
// Process variables
//
RUNTIME_ASSERT(mdoc["var"].IsArray(), "Missing 'var' in JPOF data");
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
            vtype = var["type"].GetString();
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
            fixed = var["fixed"].GetInt();
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
            jpof_vmap[tmp] = v;         // TODO - error check
            }
        else
            throw std::runtime_error("Error processing variable "+std::to_string(ctr)+": Variable id must be an unsigned integer");
        }
    else
        throw std::runtime_error("Error processing variable "+std::to_string(ctr)+": Variable id is required");

    ctr++;
    }
RUNTIME_ASSERT(ctr > 0, "A JPOF problem must have one or more variables");

//
// Process mutable parameters
//
if (mdoc.HasMember("param")) {
    RUNTIME_ASSERT(mdoc["param"].IsArray(), "Missing 'param' in JPOF data");
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
                jpof_pmap[ param["id"].GetInt() ] = p;      // TODO - error check
            else
                throw std::runtime_error("Error processing parameter "+std::to_string(ctr)+": Non-integer value for parameter id");
            }
        else
            throw std::runtime_error("Error processing parameter "+std::to_string(ctr)+": Parameter id is required");

        ctr++;
        }
    }

//
// Objectives
//
if (mdoc.HasMember("obj")) {
    RUNTIME_ASSERT(mdoc["obj"].IsArray(), "Missing 'obj' in JPOF data");
    int ctr=0;
    for (auto& obj : mdoc["obj"].GetArray()) {

        std::string expr;
        if (obj.HasMember("expr")) {
            if (obj["expr"].IsString())
                expr = obj["expr"].GetString();
            else
                throw std::runtime_error("Error processing objective "+std::to_string(ctr)+": Non-string value for objective expression");
            }
        else
            throw std::runtime_error("Error processing objective "+std::to_string(ctr)+": Objective expression not defined");

        // NOTE: objective labels are currently ignored by COEK
        std::string label;
        if (obj.HasMember("label")) {
            if (obj["label"].IsString())
                label = obj["label"].GetString();
            else
                throw std::runtime_error("Error processing objective "+std::to_string(ctr)+": Non-string value for objective label");
            }

        bool min=true;
        if (obj.HasMember("sense")) {
            if (obj["sense"].IsString()) {
                std::string sense = obj["sense"].GetString();
                min = (sense == "min");
                }
            else
                throw std::runtime_error("Error processing objective "+std::to_string(ctr)+": Non-string value for objective sense");
            }

        if (min)
            model.add_objective( create_expression(expr, jpof_vmap, jpof_pmap) );
        else
            model.add_objective( create_expression(expr, jpof_vmap, jpof_pmap), Model::maximize );

        ctr++;
        }
    }

//
// Constraints
//
if (mdoc.HasMember("con")) {
    RUNTIME_ASSERT(mdoc["con"].IsArray(), "Missing 'con' in JPOF data");
    int ctr=0;
    for (auto& con : mdoc["con"].GetArray()) {

        std::string expr;
        if (con.HasMember("expr")) {
            if (con["expr"].IsString())
                expr = con["expr"].GetString();
            else
                throw std::runtime_error("Error processing constraint "+std::to_string(ctr)+": Non-string value for constraint expression");
            }
        else
            throw std::runtime_error("Error processing constraint "+std::to_string(ctr)+": Constraint expression not defined");

        // NOTE: constraint labels are currently ignored by COEK
        std::string label;
        if (con.HasMember("label")) {
            if (con["label"].IsString())
                label = con["label"].GetString();
            else
                throw std::runtime_error("Error processing constraint "+std::to_string(ctr)+": Non-string value for constraint label");
            }

        // TODO: generalize logic to allow expressions for lower- and upper-bounds, using parameter values
        Constraint c;
        if (con.HasMember("eq")) {
            if (con["eq"].IsInt())
                c = create_expression(expr, jpof_vmap, jpof_pmap) == con["eq"].GetInt();
            else if (con["eq"].IsDouble())
                c = create_expression(expr, jpof_vmap, jpof_pmap) == con["eq"].GetDouble();
            else if (con["eq"].IsString()) {
                std::string eq = con["eq"].GetString();
                auto rhs = create_expression(eq, jpof_vmap, jpof_pmap);
                c = create_expression(expr, jpof_vmap, jpof_pmap) == rhs;
                }
            else
                throw std::runtime_error("Error processing constraint "+std::to_string(ctr)+": Unexpected value for constraint eq");
            }
        else if (con.HasMember("geq") or con.HasMember("leq")) {
            bool has_geq=false;
            bool has_leq=false;
            Expression lb;
            Expression ub;

            if (con.HasMember("geq")) {
                if (con["geq"].IsDouble()) {
                    has_geq=true;
                    lb = Expression(con["geq"].GetDouble());
                    }
                else if (con["geq"].IsString()) {
                    has_geq=true;
                    lb = create_expression(con["geq"].GetString(), jpof_vmap, jpof_pmap);
                    }
                else
                    throw std::runtime_error("Error processing constraint "+std::to_string(ctr)+": Unexpected value for constraint geq");
                }

            if (con.HasMember("leq")) {
                if (con["leq"].IsDouble()) {
                    has_leq=true;
                    ub = Expression(con["leq"].GetDouble());
                    }
                else if (con["leq"].IsString()) {
                    has_leq=true;
                    ub = create_expression(con["leq"].GetString(), jpof_vmap, jpof_pmap);
                    }
                else
                    throw std::runtime_error("Error processing constraint "+std::to_string(ctr)+": Unexpected value for constraint leq");
                }

            if (has_geq and has_leq)
                c = inequality(lb, create_expression(expr, jpof_vmap, jpof_pmap), ub);
            else if (has_geq)
                c = lb <= create_expression(expr, jpof_vmap, jpof_pmap);
            else
                c = create_expression(expr, jpof_vmap, jpof_pmap) <= ub;
            }
        else
            throw std::runtime_error("Error processing constraint "+std::to_string(ctr)+": Must specify equality or inequality constraint values (eq, geq, leq)");
        model.add_constraint(c);

        ctr++;
        }
    }
}


Model create_model_from_dom(rapidjson::Document& d, std::map<int,int>& vmap)
{
// DOM sanity checks
RUNTIME_ASSERT(d.IsObject(), "JPOF data is not a valid JSON object");
RUNTIME_ASSERT(d.HasMember("__metadata__"), "Missing '__metadata__' in JPOF data");
RUNTIME_ASSERT(d["__metadata__"].IsObject(), "The '__metadata__' is not a valid JSON object");
RUNTIME_ASSERT(d["__metadata__"].HasMember("version"), "Missing 'version' in JPOF data");
RUNTIME_ASSERT(d["__metadata__"]["version"].IsInt(), "The 'version' is not an integer");
RUNTIME_ASSERT(d["__metadata__"].HasMember("format"), "Missing 'format' in JPOF data");
RUNTIME_ASSERT(d["__metadata__"]["format"].IsString(), "The 'format' is not a string");
std::string format = d["__metadata__"]["format"].GetString();
if (format != "JSON Parameterized Optimization Format (JPOF)")
    throw std::runtime_error("Unexpected JSON file format: "+format);
RUNTIME_ASSERT(d.HasMember("model"), "Missing 'model' in JPOF data");
RUNTIME_ASSERT(d["model"].IsObject(), "The 'model' is not a valid JSON object");
if (not d["model"].HasMember("var"))
    throw std::runtime_error("JPOF model without 'var' data: A JPOF model must contain variables");

// Process DOM data to populate a model
Model model;
int version = d["__metadata__"]["version"].GetInt();
if (version >= 20210301)
    process_jpof_v20210301(d, model, vmap);
else
    throw std::runtime_error("Unexpected JPOF format version: "+std::to_string(version));

return model;
}
#endif

}


Model read_problem_from_jpof_file(const std::string& fname, std::map<int,int>& vmap)
{
#ifdef WITH_RAPIDJSON
FILE* fp = fopen(fname.c_str(), "r");
if (!fp)
    throw std::runtime_error("Unknown file: "+fname);
 
char readBuffer[65536];
rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
 
rapidjson::Document d;
d.ParseStream(is);
if (d.HasParseError()) {
    std::string msg(rapidjson::GetParseError_En(d.GetParseError()));
    throw std::runtime_error("Error parsing JPOF file (offset " + std::to_string((unsigned)d.GetErrorOffset()) +"): "+msg);
    }
 
fclose(fp);

return reader_jpof::create_model_from_dom(d, vmap);
#else
throw std::runtime_error("Must install RapidJSON to read a JPOF file.");
#endif
}

Model read_problem_from_jpof_string(const std::string& jpof, std::map<int,int>& vmap)
{
#ifdef WITH_RAPIDJSON
rapidjson::Document d;
d.Parse(jpof.c_str());
if (d.HasParseError()) {
    std::string msg(rapidjson::GetParseError_En(d.GetParseError()));
    throw std::runtime_error("Error parsing JPOF string (offset " + std::to_string((unsigned)d.GetErrorOffset()) +"): "+msg);
    }

return reader_jpof::create_model_from_dom(d, vmap);
#else
throw std::runtime_error("Must install RapidJSON to parse a JPOF string.");
#endif
}

}
