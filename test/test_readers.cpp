#include <cmath>
#include "catch.hpp"

#include "coek/ast/base_terms.hpp"
#include "coek/coek.hpp"

const std::string currdir = COEK_TEST_DIR;

#ifdef WITH_RAPIDJSON
TEST_CASE( "jpof_reader_string", "[smoke]" ) {

  SECTION( "error1" ) {
    std::map<int,int> vmap;
    REQUIRE_THROWS_WITH(coek::read_problem_from_jpof_string("", vmap),
        "Error parsing JPOF string (offset 0): The document is empty.");
    REQUIRE_THROWS_WITH(coek::read_problem_from_jpof_string("1", vmap),
        "JPOF data is not a valid JSON object");
    REQUIRE_THROWS_WITH(coek::read_problem_from_jpof_string("{}", vmap),
        "Missing '__metadata__' in JPOF data");
    REQUIRE_THROWS_WITH(coek::read_problem_from_jpof_string("{ \"__metadata__\":1 }", vmap),
        "The '__metadata__' is not a valid JSON object");
    REQUIRE_THROWS_WITH(coek::read_problem_from_jpof_string("{ \"__metadata__\":{\"version\":1.5} }", vmap),
        "The 'version' is not an integer");
    REQUIRE_THROWS_WITH(coek::read_problem_from_jpof_string("{ \"__metadata__\":{\"version\":1} }", vmap),
        "Missing 'format' in JPOF data");
    REQUIRE_THROWS_WITH(coek::read_problem_from_jpof_string("{ \"__metadata__\":{\"version\":1, \"format\":1} }", vmap),
        "The 'format' is not a string");
    REQUIRE_THROWS_WITH(coek::read_problem_from_jpof_string("{ \"__metadata__\":{\"version\":1, \"format\":\"FORMAT\"} }", vmap),
        "Unexpected JSON file format: FORMAT");
    REQUIRE_THROWS_WITH(coek::read_problem_from_jpof_string("{ \"__metadata__\":{\"version\":1, \"format\":\"JSON Parameterized Optimization Format (JPOF)\"} }", vmap),
        "Missing 'model' in JPOF data");
    REQUIRE_THROWS_WITH(coek::read_problem_from_jpof_string(
        "{ \"__metadata__\":{\"version\":1, \"format\":\"JSON Parameterized Optimization Format (JPOF)\"},"
        "  \"model\":1 }", vmap),
        "The 'model' is not a valid JSON object");
    REQUIRE_THROWS_WITH(coek::read_problem_from_jpof_string(
        "{ \"__metadata__\":{\"version\":1, \"format\":\"JSON Parameterized Optimization Format (JPOF)\"},"
        "  \"model\":{} }", vmap),
        "JPOF model without 'var' data: A JPOF model must contain variables");
    REQUIRE_THROWS_WITH(coek::read_problem_from_jpof_string(
        "{ \"__metadata__\":{\"version\":1, \"format\":\"JSON Parameterized Optimization Format (JPOF)\"},"
        "  \"model\":{"
        "       \"var\":[]"
        "   }"
        "}", vmap),
        "Unexpected JPOF format version: 1");
    REQUIRE_THROWS_WITH(coek::read_problem_from_jpof_string(
        "{ \"__metadata__\":{\"version\":20210301, \"format\":\"JSON Parameterized Optimization Format (JPOF)\"},"
        "  \"model\":{"
        "       \"var\":[]"
        "   }"
        "}", vmap),
        "A JPOF problem must have one or more variables");
    }

  SECTION( "small1" ) {
    std::map<int,int> vmap;
    auto model = coek::read_problem_from_jpof_string(
        "{ \"__metadata__\":{\"version\":20210301, \"format\":\"JSON Parameterized Optimization Format (JPOF)\"},"
        "  \"model\":{"
        "       \"var\":["
        "               {\"type\":\"R\", \"id\":0, \"value\":1, \"label\":\"x\"},"
        "               {\"type\":\"R\", \"id\":1, \"value\":1, \"label\":\"y\"}"
        "               ],"
        "       \"obj\":["
        "               {\"expr\":\"pow,V,0,N,2\"}"
        "               ],"
        "       \"con\":["
        "               {\"expr\":\"pow,V,1,N,2\", \"eq\":4}"
        "               ]"
        "   }"
        "}",
        vmap);
    REQUIRE( model.num_constraints() == 1 );
    REQUIRE( model.num_objectives() == 1 );

    auto obj = model.get_objective();
    static std::list<std::string> obj_expr = {"[", "pow", "x", "2.000", "]"};
    REQUIRE( obj.body().to_list() == obj_expr );

    auto con = model.get_constraint(0);
    static std::list<std::string> con_expr = {"[", "==", "[", "pow", "y", "2.000", "]", "4.000", "]"};
    REQUIRE( con.to_list() == con_expr );
    }

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}

void test_var(coek::Model& model, int i, const std::string& name, double value, double lb, double ub, bool fixed, const std::string& vtype)
{
auto v = model.get_variable(i);
REQUIRE( v.get_name() == name );
REQUIRE( v.get_value() == Approx(value) );
REQUIRE( v.get_lb() == Approx(lb) );
REQUIRE( v.get_ub() == Approx(ub) );
REQUIRE( v.get_fixed() == fixed );
if (vtype == "R")
    REQUIRE( v.is_continuous() );
else if (vtype == "Z")
    REQUIRE( v.is_integer() );
else 
    REQUIRE( v.is_binary() );
}

void test_obj(coek::Model& model, int i, const std::list<std::string>& baseline, const std::string& name)
{
auto obj = model.get_objective(i);
REQUIRE( obj.to_list() == baseline );
}

void test_con(coek::Model& model, int i, const std::list<std::string>& baseline, const std::string& name)
{
auto con = model.get_constraint(i);
REQUIRE( con.to_list() == baseline );
}


TEST_CASE( "jpof_reader_file", "[smoke]" ) {

  SECTION( "small1" ) {
    std::map<int,int> vmap;
    auto model = coek::read_problem_from_jpof_file(currdir+"jpof/small1.json", vmap);

    REQUIRE( model.num_variables() == 2 );
    REQUIRE( model.num_constraints() == 1 );
    REQUIRE( model.num_objectives() == 1 );

    test_var(model, 0, "x", 1.0, -COEK_INFINITY, COEK_INFINITY, false, "R");
    test_var(model, 1, "y", 1.0, -COEK_INFINITY, COEK_INFINITY, false, "R");

    test_obj(model, 0, {"[", "min", "[", "pow", "x", "2.000", "]", "]"}, "OBJ");
    test_con(model, 0, {"[", "==", "[", "pow", "y", "2.000", "]", "4.000", "]"}, "CON");
    }

  SECTION( "small2" ) {
    std::map<int,int> vmap;
    auto model = coek::read_problem_from_jpof_file(currdir+"jpof/small2.json", vmap);

    REQUIRE( model.num_variables() == 2 );
    REQUIRE( model.num_constraints() == 1 );
    REQUIRE( model.num_objectives() == 1 );

    test_var(model, 0, "x", 1.0, -COEK_INFINITY, COEK_INFINITY, false, "R");
    test_var(model, 1, "y", 1.0, -COEK_INFINITY, COEK_INFINITY, false, "R");

    test_obj(model, 0, {"[", "min", "x", "]"}, "OBJ");
    test_con(model, 0, {"[", "==", "[", "pow", "y", "2.000", "]", "4.000", "]"}, "CON");
    }

  SECTION( "small3" ) {
    std::map<int,int> vmap;
    auto model = coek::read_problem_from_jpof_file(currdir+"jpof/small3.json", vmap);

    REQUIRE( model.num_variables() == 2 );
    REQUIRE( model.num_constraints() == 1 );
    REQUIRE( model.num_objectives() == 1 );

    test_var(model, 0, "x", 1.0, -COEK_INFINITY, COEK_INFINITY, false, "R");
    test_var(model, 1, "y", 1.0, -COEK_INFINITY, COEK_INFINITY, false, "R");

    test_obj(model, 0, {"[", "min", "[", "*", "x", "y", "]", "]"}, "OBJ");
    test_con(model, 0, {"[", "==", "[", "pow", "y", "2.000", "]", "4.000", "]"}, "CON");
    }

  SECTION( "small4" ) {
    std::map<int,int> vmap;
    auto model = coek::read_problem_from_jpof_file(currdir+"jpof/small4.json", vmap);

    REQUIRE( model.num_variables() == 2 );
    REQUIRE( model.num_constraints() == 1 );
    REQUIRE( model.num_objectives() == 1 );

    test_var(model, 0, "x", 1.0, -COEK_INFINITY, COEK_INFINITY, false, "R");
    test_var(model, 1, "y", 1.0, -COEK_INFINITY, COEK_INFINITY, false, "R");

    test_obj(model, 0, {"[", "min", "[", "pow", "y", "2.000", "]", "]"}, "OBJ");
    test_con(model, 0, {"[", "==", "[", "*", "y", "x", "]", "4.000", "]"}, "CON1");
    }

  SECTION( "small5" ) {
    std::map<int,int> vmap;
    auto model = coek::read_problem_from_jpof_file(currdir+"jpof/small5.json", vmap);

    REQUIRE( model.num_variables() == 3 );
    REQUIRE( model.num_constraints() == 12 );
    REQUIRE( model.num_objectives() == 1 );

    test_var(model, 0, "v", 3.0, -1.0, 1.0, false, "R");
    test_var(model, 1, "x", 1.0, -1.0, 1.0, false, "R");
    test_var(model, 2, "y", 2.0, -1.0, 1.0, false, "R");

    test_obj(model, 0, {"[", "min", "[", "+", "[", "/", "[", "pow", "x", "2.000", "]", "2.000", "]", "[", "/", "[", "pow",   "x", "2.000", "]", "q", "]", "]", "]"}, "OBJ");

    test_con(model, 0, {"[", "==", "[", "*", "[", "*", "0.500", "v", "]", "[", "+", "x", "[", "*",
  "-1.000", "y", "]", "]", "]", "2.000", "]"}, "CON1");
    test_con(model, 1, {"[", "==", "[", "*", "v", "[", "+", "[", "*", "[", "/", "1.000", "q", "]",
  "x", "]", "[", "*", "[", "-", "[", "/", "1.000", "q", "]", "]", "y", "]", "]", "]", "2.000", "]"}, "CON10");
    test_con(model, 2, {"[", "==", "[", "*", "[", "*", "v", "[", "+", "x", "[", "*", "-1.000", "y",
  "]", "]", "]", "[", "/", "1.000", "q", "]", "]", "2.000", "]"}, "CON11");
    test_con(model, 3, {"[", "==", "[", "*", "v", "[", "+", "x", "[", "*", "-1.000", "y", "]", "]",
  "]", "[", "*", "2.000", "q", "]", "]"}, "CON12");
    test_con(model, 4, {"[", "==", "[", "*", "[", "*", "0.500", "v", "]", "[", "+", "x", "[", "*",
  "-1.000", "y", "]", "]", "]", "2.000", "]"}, "CON2");
    test_con(model, 5, {"[", "==", "[", "/", "[", "*", "v", "[", "+", "x", "[", "*", "-1.000", "y",
  "]", "]", "]", "2.000", "]", "2.000", "]"}, "CON3");
    test_con(model, 6, {"[", "==", "[", "*", "v", "[", "+", "[", "*", "0.500", "x", "]", "[", "*",
  "-0.500", "y", "]", "]", "]", "2.000", "]"}, "CON4");
    test_con(model, 7, {"[", "==", "[", "*", "[", "*", "v", "[", "+", "x", "[", "*", "-1.000", "y",
  "]", "]", "]", "0.500", "]", "2.000", "]"}, "CON5");
    test_con(model, 8, {"[", "==", "[", "*", "v", "[", "+", "x", "[", "*", "-1.000", "y", "]", "]",
  "]", "4.000", "]"}, "CON6");
    test_con(model, 9, {"[", "==", "[", "*", "[", "*", "[", "/", "1.000", "q", "]", "v", "]", "[",
  "+", "x", "[", "*", "-1.000", "y", "]", "]", "]", "2.000", "]"}, "CON7");
    test_con(model, 10, {"[", "==", "[", "*", "[", "*", "[", "/", "1.000", "q", "]", "v", "]", "[",
  "+", "x", "[", "*", "-1.000", "y", "]", "]", "]", "2.000", "]"}, "CON8");
    test_con(model, 11, {"[", "==", "[", "/", "[", "*", "v", "[", "+", "x", "[", "*", "-1.000", "y",
  "]", "]", "]", "q", "]", "2.000", "]"}, "CON9");
    }

  SECTION( "small6" ) {
    std::map<int,int> vmap;
    auto model = coek::read_problem_from_jpof_file(currdir+"jpof/small6.json", vmap);

    REQUIRE( model.num_variables() == 4 );
    REQUIRE( model.num_constraints() == 6 );
    REQUIRE( model.num_objectives() == 1 );

    test_var(model, 0, "p", 2.0, -COEK_INFINITY, COEK_INFINITY, true, "R");
    test_var(model, 1, "v", 3.0, -1.0, 1.0, false, "R");
    test_var(model, 2, "x", 1.0, -1.0, 1.0, false, "R");
    test_var(model, 3, "y", 2.0, -1.0, 1.0, false, "R");

    test_obj(model, 0, {"[", "min", "x", "]",}, "OBJ");

    test_con(model, 0, {"[", "==", "[", "*", "[", "*", "[", "/", "1.000", "p", "]", "v", "]", "[",
  "+", "x", "[", "*", "-1.000", "y", "]", "]", "]", "2.000", "]"}, "CON1");
    test_con(model, 1, {"[", "==", "[", "*", "[", "/", "v", "p", "]", "[", "+", "x", "[", "*", "-1.000", "y", "]", "]", "]", "2.000", "]"}, "CON2");
    test_con(model, 2, {"[", "==", "[", "/", "[", "*", "v", "[", "+", "x", "[", "*", "-1.000", "y",
  "]", "]", "]", "p", "]", "2.000", "]"}, "CON3");
    test_con(model, 3, {"[", "==", "[", "*", "v", "[", "+", "[", "/", "x", "p", "]", "[", "-", "[",
  "/", "y", "p", "]", "]", "]", "]", "2.000", "]"}, "CON4");
    test_con(model, 4, {"[", "==", "[", "*", "[", "*", "v", "[", "+", "x", "[", "*", "-1.000", "y",
  "]", "]", "]", "[", "/", "1.000", "p", "]", "]", "2.000", "]"}, "CON5");
    test_con(model, 5, {"[", "==", "[", "+", "[", "*", "v", "[", "+", "x", "[", "*", "-1.000", "y",
  "]", "]", "]", "[", "*", "-2.000", "p", "]", "]", "0.000", "]"}, "CON6");
    }

  SECTION( "small7" ) {
    std::map<int,int> vmap;
    auto model = coek::read_problem_from_jpof_file(currdir+"jpof/small7.json", vmap);

    REQUIRE( model.num_variables() == 4 );
    REQUIRE( model.num_constraints() == 24 );
    REQUIRE( model.num_objectives() == 1 );

    test_var(model, 0, "p", 2.0, -COEK_INFINITY, COEK_INFINITY, true, "R");
    test_var(model, 1, "v", 3.0, -1.0, 1.0, false, "R");
    test_var(model, 2, "x", 1.0, -1.0, 1.0, false, "R");
    test_var(model, 3, "y", 2.0, -1.0, 1.0, false, "R");

    test_obj(model, 0, {"[", "min", "x", "]",}, "OBJ");

    test_con(model, 0, {"[", "==", "[", "*", "[", "*", "[", "/", "[", "/", "1.000", "p", "]", "2.000", "]", "v", "]", "[", "+", "x", "[", "*", "-1.000", "y", "]", "]", "]", "2.000", "]"}, "CON1a");
    test_con(model, 1, {"[", "==", "[", "*", "[", "*", "[", "/", "1.000", "[", "*", "2.000", "p", "]", "]", "v", "]", "[", "+", "x", "[", "*", "-1.000", "y", "]", "]", "]", "2.000", "]"}, "CON1b");
    test_con(model, 2, {"[", "==", "[", "*", "[", "*", "[", "/", "1.000", "[", "+", "p", "2.000", "]", "]", "v", "]", "[", "+", "x", "[", "*", "-1.000", "y", "]", "]", "]", "2.000", "]"}, "CON1c");
    test_con(model, 3, {"[", "==", "[", "*", "[", "*", "[", "/", "1.000", "[", "pow", "[", "+", "p", "2.000", "]", "2.000", "]", "]", "v", "]", "[", "+", "x", "[", "*", "-1.000", "y", "]", "]", "]", "2.000", "]"}, "CON1d");

    test_con(model, 4, {"[", "==", "[", "*", "[", "/", "[", "/", "v", "p", "]", "p", "]", "[", "+", "x", "[", "*", "-1.000", "y", "]", "]", "]", "2.000", "]"}, "CON2a");
    test_con(model, 5, {"[", "==", "[", "*", "[", "/", "v", "[", "*", "p", "p", "]", "]", "[", "+", "x", "[", "*", "-1.000", "y", "]", "]", "]", "2.000", "]"}, "CON2b");
    test_con(model, 6, {"[", "==", "[", "*", "[", "/", "v", "[", "+", "p", "p", "]", "]", "[", "+", "x", "[", "*", "-1.000", "y", "]", "]", "]", "2.000", "]"}, "CON2c");
    test_con(model, 7, {"[", "==", "[", "*", "[", "/", "v", "[", "pow", "[", "+", "p", "p", "]", "2.000", "]", "]", "[", "+", "x", "[", "*", "-1.000", "y", "]", "]", "]", "2.000", "]"}, "CON2d");

    test_con(model, 8, {"[", "==", "[", "/", "[", "/", "[", "*", "v", "[", "+", "x", "[", "*", "-1.000", "y", "]", "]", "]", "p", "]", "2.000", "]", "2.000", "]"}, "CON3a");
    test_con(model, 9, {"[", "==", "[", "/", "[", "*", "v", "[", "+", "x", "[", "*", "-1.000", "y", "]", "]", "]", "[", "*", "2.000", "p", "]", "]", "2.000", "]"}, "CON3b");
    test_con(model, 10, {"[", "==", "[", "/", "[", "*", "v", "[", "+", "x", "[", "*", "-1.000", "y", "]", "]", "]", "[", "+", "p", "2.000", "]", "]", "2.000", "]"}, "CON3c");
    test_con(model, 11, {"[", "==", "[", "/", "[", "*", "v", "[", "+", "x", "[", "*", "-1.000", "y", "]", "]", "]", "[", "pow", "[", "+", "p", "2.000", "]", "2.000", "]", "]", "2.000", "]"}, "CON3d");

    test_con(model, 12, {"[", "==", "[", "*", "v", "[", "+", "[", "/", "[", "/", "x", "p", "]", "2.000", "]", "[", "-", "[", "/", "[", "/", "y", "p", "]", "2.000", "]", "]", "]", "]", "2.000", "]"}, "CON4a");
    test_con(model, 13, {"[", "==", "[", "*", "v", "[", "+", "[", "/", "x", "[", "*", "2.000", "p", "]", "]", "[", "-", "[", "/", "y", "[", "*", "2.000", "p", "]", "]", "]", "]", "]", "2.000", "]"}, "CON4b");
    test_con(model, 14, {"[", "==", "[", "*", "v", "[", "+", "[", "/", "x", "[", "+", "p", "2.000", "]", "]", "[", "-", "[", "/", "y", "[", "+", "p", "2.000", "]", "]", "]", "]", "]", "2.000", "]"}, "CON4c");
    test_con(model, 15, {"[", "==", "[", "*", "v", "[", "+", "[", "/", "x", "[", "pow", "[", "+", "p", "2.000", "]", "2.000", "]", "]", "[", "-", "[", "/", "y", "[", "pow", "[", "+", "p", "2.000", "]", "2.000", "]", "]", "]", "]", "]", "2.000", "]"}, "CON4d");

    test_con(model, 16, {"[", "==", "[", "*", "[", "*", "v", "[", "+", "x", "[", "*", "-1.000", "y", "]", "]", "]", "[", "/", "[", "/", "1.000", "p", "]", "2.000", "]", "]", "2.000", "]"}, "CON5a");
    test_con(model, 17, {"[", "==", "[", "*", "[", "*", "v", "[", "+", "x", "[", "*", "-1.000", "y", "]", "]", "]", "[", "/", "1.000", "[", "*", "2.000", "p", "]", "]", "]", "2.000", "]"}, "CON5b");
    test_con(model, 18, {"[", "==", "[", "*", "[", "*", "v", "[", "+", "x", "[", "*", "-1.000", "y", "]", "]", "]", "[", "/", "1.000", "[", "+", "p", "2.000", "]", "]", "]", "2.000", "]"}, "CON5c");
    test_con(model, 19, {"[", "==", "[", "*", "[", "*", "v", "[", "+", "x", "[", "*", "-1.000", "y", "]", "]", "]", "[", "/", "1.000", "[", "pow", "[", "+", "p", "2.000", "]", "2.000", "]", "]", "]", "2.000", "]"}, "CON5d");

    test_con(model, 20, {"[", "==", "[", "+", "[", "*", "v", "[", "+", "x", "[", "*", "-1.000", "y", "]", "]", "]", "[", "*", "-4.000", "p", "]", "]", "0.000", "]"}, "CON6a");
    test_con(model, 21, {"[", "==", "[", "+", "[", "*", "v", "[", "+", "x", "[", "*", "-1.000", "y", "]", "]", "]", "[", "*", "-4.000", "p", "]", "]", "0.000", "]"}, "CON6b");
    test_con(model, 22, {"[", "==", "[", "+", "[", "*", "v", "[", "+", "x", "[", "*", "-1.000", "y", "]", "]", "]", "[", "-", "[", "*", "2.000", "[", "+", "p", "2.000", "]", "]", "]", "]", "0.000", "]"}, "CON6c");
    test_con(model, 23, {"[", "==", "[", "+", "[", "*", "v", "[", "+", "x", "[", "*", "-1.000", "y", "]", "]", "]", "[", "-", "[", "*", "2.000", "[", "pow", "[", "+", "p", "2.000", "]", "2.000", "]", "]", "]", "]", "0.000", "]"}, "CON6d");
    }

  SECTION( "small9" ) {
    std::map<int,int> vmap;
    auto model = coek::read_problem_from_jpof_file(currdir+"jpof/small9.json", vmap);

    REQUIRE( model.num_variables() == 3 );
    REQUIRE( model.num_constraints() == 5 );
    REQUIRE( model.num_objectives() == 1 );

    test_var(model, 0, "x", 0.0, -COEK_INFINITY, COEK_INFINITY, false, "R");
    test_var(model, 1, "y", 0.0, -COEK_INFINITY, COEK_INFINITY, true, "R");
    test_var(model, 2, "z", 0.0, -COEK_INFINITY, COEK_INFINITY, false, "R");

    test_obj(model, 0, {"[", "min", "x", "]",}, "obj");

    test_con(model, 0, {"[", "==", "[", "+", "[", "*", "[", "*", "x", "y", "]", "z", "]", "x", "]",
  "1.000", "]"}, "con1");
    test_con(model, 1, {"[", "==", "[", "+", "[", "*", "[", "*", "p", "x", "]", "z", "]", "x", "]",
  "1.000", "]"}, "con2");
    test_con(model, 2, {"[", "==", "x", "1.000", "]"}, "con3");
    test_con(model, 3, {"[", "==", "[", "*", "[", "*", "x", "y", "]", "z", "]", "1.000", "]"}, "con4");
    test_con(model, 4, {"[", "==", "[", "*", "[", "*", "p", "x", "]", "z", "]", "1.000", "]"}, "con5");
    }

  SECTION( "small10" ) {
    std::map<int,int> vmap;
    auto model = coek::read_problem_from_jpof_file(currdir+"jpof/small10.json", vmap);

    REQUIRE( model.num_variables() == 3 );
    REQUIRE( model.num_constraints() == 14 );
    REQUIRE( model.num_objectives() == 1 );

    test_var(model, 0, "x", 1.0, -COEK_INFINITY, COEK_INFINITY, true, "R");
    test_var(model, 1, "y", 0.0, -COEK_INFINITY, COEK_INFINITY, false, "R");
    test_var(model, 2, "z", 0.0, -COEK_INFINITY, COEK_INFINITY, true, "R");

    test_obj(model, 0, {"[", "min", "[", "+", "[", "*", "x", "y", "]", "[", "*", "z", "y", "]", "[", "*", "p", "y", "]", "[", "*", "[", "*", "y", "y", "]", "p", "]", "[", "*", "[", "*", "y", "y", "]", "z", "]", "[", "*", "z", "[", "pow", "y", "2.000", "]", "]", "]", "]"}, "obj");

    test_con(model, 0, {"[", "==", "[", "*", "x", "y", "]", "0.000", "]"}, "con1");
    test_con(model, 1, {"[", "==", "[", "*", "p", "[", "pow", "y", "2.000", "]", "]", "0.000", "]"}, "con12");
    test_con(model, 2, {"[", "==", "[", "*", "[", "*", "p", "y", "]", "x", "]", "0.000", "]"}, "con13");
    test_con(model, 3, {"[", "==", "[", "*", "z", "[", "pow", "y", "2.000", "]", "]", "0.000", "]"}, "con14");
    test_con(model, 4, {"[", "==", "[", "*", "[", "*", "z", "y", "]", "x", "]", "0.000", "]"}, "con15");
    test_con(model, 5, {"[", "==", "[", "*", "p", "y", "]", "0.000", "]"}, "con17");
    test_con(model, 6, {"[", "==", "[", "+", "[", "*", "z", "y", "]", "y", "]", "0.000", "]"}, "con2");
    test_con(model, 7, {"[", "==", "y", "0.000", "]"}, "con3");
    test_con(model, 8, {"[", "==", "y", "0.000", "]"}, "con4");
    test_con(model, 9, {"[", "==", "[", "+", "[", "*", "p", "[", "pow", "y", "2.000", "]", "]", "y", "]", "0.000", "]"}, "con5");
    test_con(model, 10, {"[", "==", "[", "+", "[", "*", "[", "*", "p", "y", "]", "x", "]", "y", "]", "0.000", "]"}, "con6");
    test_con(model, 11, {"[", "==", "[", "+", "[", "*", "z", "[", "pow", "y", "2.000", "]", "]", "y", "]", "0.000", "]"}, "con7");
    test_con(model, 12, {"[", "==", "[", "+", "[", "*", "[", "*", "z", "y", "]", "x", "]", "y", "]", "0.000", "]"}, "con8");
    test_con(model, 13, {"[", "==", "[", "*", "z", "y", "]", "0.000", "]"}, "con9");
    }

  SECTION( "small11" ) {
    std::map<int,int> vmap;
    auto model = coek::read_problem_from_jpof_file(currdir+"jpof/small11.json", vmap);

    REQUIRE( model.num_variables() == 4 );
    REQUIRE( model.num_constraints() == 3 );
    REQUIRE( model.num_objectives() == 1 );

    test_var(model, 0, "x[1,1]", 1.0, -COEK_INFINITY, COEK_INFINITY, true, "R");
    test_var(model, 1, "x[1,2]", 0.0, -COEK_INFINITY, COEK_INFINITY, false, "R");
    test_var(model, 2, "x[1,3]", 0.0, -COEK_INFINITY, COEK_INFINITY, false, "R");
    test_var(model, 3, "x[3,3]", 0.0, -COEK_INFINITY, COEK_INFINITY, false, "R");

    test_obj(model, 0, {"[", "min", "x[3,3]", "]"}, "obj");

    test_con(model, 0, {"[", "<=", "-1.000", "x[1,1]", "1.000", "]"}, "var_bnd[1]");
    test_con(model, 1, {"[", "<=", "-1.000", "x[1,2]", "1.000", "]"}, "var_bnd[2]");
    test_con(model, 2, {"[", "<=", "-1.000", "x[1,3]", "1.000", "]"}, "var_bnd[3]");
    }

  SECTION( "small13" ) {
    std::map<int,int> vmap;
    auto model = coek::read_problem_from_jpof_file(currdir+"jpof/small13.json", vmap);

    REQUIRE( model.num_variables() == 1 );
    REQUIRE( model.num_constraints() == 3 );
    REQUIRE( model.num_objectives() == 1 );

    test_var(model, 0, "x", 0.5, -COEK_INFINITY, COEK_INFINITY, false, "R");

    test_obj(model, 0, {"[", "max", "x", "]"}, "obj");

    test_con(model, 0, {"[", "==", "[", "+", "[", "pow", "x", "3.000", "]", "[", "*", "-1.000", "x", "]", "]", "0.000", "]"}, "c1");
    test_con(model, 1, {"[", "==", "[", "*", "10.000", "[", "+", "[", "pow", "x", "3.000", "]", "[", "*", "-1.000", "x", "]", "]", "]", "0.000", "]"}, "c2");
    test_con(model, 2, {"[", "==", "[", "/", "[", "+", "[", "pow", "x", "3.000", "]", "[", "*", "-1.000", "x", "]", "]", "10.000", "]", "0.000", "]"}, "c3");
    }

  SECTION( "small14" ) {
    std::map<int,int> vmap;
    auto model = coek::read_problem_from_jpof_file(currdir+"jpof/small14.json", vmap);

    REQUIRE( model.num_variables() == 2 );
    REQUIRE( model.num_constraints() == 19 );
    REQUIRE( model.num_objectives() == 1 );

    test_var(model, 0, "ONE", 1, -COEK_INFINITY, COEK_INFINITY, false, "R");
    test_var(model, 1, "ZERO", 0, -COEK_INFINITY, COEK_INFINITY, false, "R");

    test_obj(model, 0, {"[", "min", "[", "+", "ONE", "ZERO", "]", "]"}, "obj");

    test_con(model, 0, {"[", "==", "[", "abs", "ONE", "]", "1.000", "]"}, "c_abs");
    test_con(model, 1, {"[", "==", "[", "acos", "ZERO", "]", "1.571", "]"}, "c_acos");
    test_con(model, 2, {"[", "==", "[", "acosh", "[", "/", "[", "+", "ONE", "7.389", "]", "5.437", "]", "]", "0.000", "]"}, "c_acosh");
    test_con(model, 3, {"[", "==", "[", "asin", "ZERO", "]", "0.000", "]"}, "c_asin");
    test_con(model, 4, {"[", "==", "[", "asinh", "ZERO", "]", "0.000", "]"}, "c_asinh");
    test_con(model, 5, {"[", "==", "[", "atan", "ZERO", "]", "0.000", "]"}, "c_atan");
    test_con(model, 6, {"[", "==", "[", "atanh", "ZERO", "]", "0.000", "]"}, "c_atanh");
    test_con(model, 7, {"[", "==", "[", "ceil", "ONE", "]", "1.000", "]"}, "c_ceil");
    test_con(model, 8, {"[", "==", "[", "cos", "ZERO", "]", "1.000", "]"}, "c_cos");
    test_con(model, 9, {"[", "==", "[", "cosh", "ZERO", "]", "1.000", "]"}, "c_cosh");
    test_con(model, 10, {"[", "==", "[", "exp", "ZERO", "]", "1.000", "]"}, "c_exp");
    test_con(model, 11, {"[", "==", "[", "floor", "ONE", "]", "1.000", "]"}, "c_floor");
    test_con(model, 12, {"[", "==", "[", "log", "ONE", "]", "0.000", "]"}, "c_log");
    test_con(model, 13, {"[", "==", "[", "log10", "ONE", "]", "0.000", "]"}, "c_log10");
    test_con(model, 14, {"[", "==", "[", "sin", "ZERO", "]", "0.000", "]"}, "c_sin");
    test_con(model, 15, {"[", "==", "[", "sinh", "ZERO", "]", "0.000", "]"}, "c_sinh");
    test_con(model, 16, {"[", "==", "[", "sqrt", "ONE", "]", "1.000", "]"}, "c_sqrt");
    test_con(model, 17, {"[", "==", "[", "tan", "ZERO", "]", "0.000", "]"}, "c_tan");
    test_con(model, 18, {"[", "==", "[", "tanh", "ZERO", "]", "0.000", "]"}, "c_tanh");
    }

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}
#endif
