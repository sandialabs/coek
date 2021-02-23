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

TEST_CASE( "jpof_reader_file", "[smoke]" ) {

  SECTION( "small1" ) {
    std::map<int,int> vmap;
    auto model = coek::read_problem_from_jpof_file(currdir+"jpof/small1.json", vmap);

    REQUIRE( model.num_variables() == 2 );
    REQUIRE( model.num_constraints() == 1 );
    REQUIRE( model.num_objectives() == 1 );

    auto v0 = model.get_variable(0);
    REQUIRE( v0.get_name() == "x" );
    REQUIRE( v0.get_value() == Approx(1.0) );
    REQUIRE( v0.get_lb() == Approx(-COEK_INFINITY) );
    REQUIRE( v0.get_ub() == Approx(COEK_INFINITY) );
    REQUIRE( v0.get_fixed() == false );
    REQUIRE( v0.is_continuous() );
    auto v1 = model.get_variable(1);
    REQUIRE( v1.get_name() == "y" );
    REQUIRE( v1.get_value() == Approx(1.0) );
    REQUIRE( v1.get_lb() == Approx(-COEK_INFINITY) );
    REQUIRE( v1.get_ub() == Approx(COEK_INFINITY) );
    REQUIRE( v1.get_fixed() == false );
    REQUIRE( v1.is_continuous() );

    auto obj = model.get_objective();
    static std::list<std::string> obj_expr = {"[", "pow", "x", "2.000", "]"};
    REQUIRE( obj.body().to_list() == obj_expr );

    auto con = model.get_constraint(0);
    static std::list<std::string> con_expr = {"[", "==", "[", "pow", "y", "2.000", "]", "4.000", "]"};
    REQUIRE( con.to_list() == con_expr );
    }

  SECTION( "small2" ) {
    std::map<int,int> vmap;
    auto model = coek::read_problem_from_jpof_file(currdir+"jpof/small2.json", vmap);

    REQUIRE( model.num_variables() == 2 );
    REQUIRE( model.num_constraints() == 1 );
    REQUIRE( model.num_objectives() == 1 );

    auto obj = model.get_objective();
    static std::list<std::string> obj_expr = {"x"};
    REQUIRE( obj.body().to_list() == obj_expr );
    //REQUIRE( obj.get_name() == "OBJ" );

    auto con = model.get_constraint(0);
    static std::list<std::string> con_expr = {"[", "==", "[", "pow", "y", "2.000", "]", "4.000", "]"};
    REQUIRE( con.to_list() == con_expr );
    //REQUIRE( con.get_name() == "CON1" );
    }

  SECTION( "small3" ) {
    std::map<int,int> vmap;
    auto model = coek::read_problem_from_jpof_file(currdir+"jpof/small3.json", vmap);

    REQUIRE( model.num_variables() == 2 );
    REQUIRE( model.num_constraints() == 1 );
    REQUIRE( model.num_objectives() == 1 );

    auto obj = model.get_objective();
    static std::list<std::string> obj_expr = {"[", "*", "x", "y", "]"};
    REQUIRE( obj.body().to_list() == obj_expr );
    //REQUIRE( obj.get_name() == "OBJ" );

    auto con = model.get_constraint(0);
    static std::list<std::string> con_expr = {"[", "==", "[", "pow", "y", "2.000", "]", "4.000", "]"};
    REQUIRE( con.to_list() == con_expr );
    //REQUIRE( con.get_name() == "CON1" );
    }

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}
#endif
