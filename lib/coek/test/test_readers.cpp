#include <cmath>

#include "catch2/catch_test_macros.hpp"
#include "catch2/catch_approx.hpp"
#include "catch2/matchers/catch_matchers.hpp"
#include "coek/ast/base_terms.hpp"
#include "coek/coek.hpp"

const std::string currdir = COEK_TEST_DIR;

#ifdef WITH_RAPIDJSON
TEST_CASE("jpof_reader_string", "[smoke]")
{
    SECTION("error1")
    {
        std::map<std::string, coek::Parameter> params;
        REQUIRE_THROWS_WITH(coek::read_problem_from_jpof_string("", params),
                            "Error parsing JPOF string (offset 0): The document is empty.");
        REQUIRE_THROWS_WITH(coek::read_problem_from_jpof_string("1", params),
                            "JPOF data is not a valid JSON object");
        REQUIRE_THROWS_WITH(coek::read_problem_from_jpof_string("{}", params),
                            "Missing '__metadata__' in JPOF data");
        REQUIRE_THROWS_WITH(coek::read_problem_from_jpof_string("{ \"__metadata__\":1 }", params),
                            "The '__metadata__' is not a valid JSON object");
        REQUIRE_THROWS_WITH(
            coek::read_problem_from_jpof_string("{ \"__metadata__\":{\"version\":1.5} }", params),
            "The 'version' is not an integer");
        REQUIRE_THROWS_WITH(
            coek::read_problem_from_jpof_string("{ \"__metadata__\":{\"version\":1} }", params),
            "Missing 'format' in JPOF data");
        REQUIRE_THROWS_WITH(coek::read_problem_from_jpof_string(
                                "{ \"__metadata__\":{\"version\":1, \"format\":1} }", params),
                            "The 'format' is not a string");
        REQUIRE_THROWS_WITH(
            coek::read_problem_from_jpof_string(
                "{ \"__metadata__\":{\"version\":1, \"format\":\"FORMAT\"} }", params),
            "Unexpected JSON file format: FORMAT");
        REQUIRE_THROWS_WITH(coek::read_problem_from_jpof_string(
                                "{ \"__metadata__\":{\"version\":1, \"format\":\"JSON "
                                "Parameterized Optimization Format (JPOF)\"} }",
                                params),
                            "Missing 'model' in JPOF data");
        REQUIRE_THROWS_WITH(coek::read_problem_from_jpof_string(
                                "{ \"__metadata__\":{\"version\":1, \"format\":\"JSON "
                                "Parameterized Optimization Format (JPOF)\"},"
                                "  \"model\":1 }",
                                params),
                            "The 'model' is not a valid JSON object");
        REQUIRE_THROWS_WITH(coek::read_problem_from_jpof_string(
                                "{ \"__metadata__\":{\"version\":1, \"format\":\"JSON "
                                "Parameterized Optimization Format (JPOF)\"},"
                                "  \"model\":{} }",
                                params),
                            "JPOF model without 'var' data: A JPOF model must contain variables");
        REQUIRE_THROWS_WITH(coek::read_problem_from_jpof_string(
                                "{ \"__metadata__\":{\"version\":1, \"format\":\"JSON "
                                "Parameterized Optimization Format (JPOF)\"},"
                                "  \"model\":{"
                                "       \"var\":[]"
                                "   }"
                                "}",
                                params),
                            "Unexpected JPOF format version: 1");
        REQUIRE_THROWS_WITH(coek::read_problem_from_jpof_string(
                                "{ \"__metadata__\":{\"version\":20210301, \"format\":\"JSON "
                                "Parameterized Optimization Format (JPOF)\"},"
                                "  \"model\":{"
                                "       \"var\":[]"
                                "   }"
                                "}",
                                params),
                            "A JPOF problem must have one or more variables");
    }

    SECTION("small1")
    {
        std::map<std::string, coek::Parameter> params;
        auto model = coek::read_problem_from_jpof_string(
            "{ \"__metadata__\":{\"version\":20210301, \"format\":\"JSON Parameterized "
            "Optimization Format (JPOF)\"},"
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
            params);
        REQUIRE(model.num_variables() == 2);
        REQUIRE(model.variable_names() == std::set<std::string>({"x", "y"}));
        REQUIRE(model.num_constraints() == 1);
        REQUIRE(model.num_objectives() == 1);

        auto obj = model.get_objective();
        static std::list<std::string> obj_expr = {"[", "pow", "x", std::to_string(2.000), "]"};
        REQUIRE(obj.expr().to_list() == obj_expr);

        auto con = model.get_constraint(0);
        static std::list<std::string> con_expr
            = {"[", "==", "[", "pow", "y", std::to_string(2.000), "]", std::to_string(4.000), "]"};
        REQUIRE(con.to_list() == con_expr);
    }
}

void test_var(coek::Model& model, unsigned int i, const std::string& name, double value, double lb,
              double ub, bool fixed, const std::string& vtype)
{
    auto v = model.get_variable(i);
    REQUIRE(v.name() == name);
    REQUIRE(v.value() == Catch::Approx(value));
    REQUIRE(v.lower() == Catch::Approx(lb));
    REQUIRE(v.upper() == Catch::Approx(ub));
    REQUIRE(v.fixed() == fixed);
    if (vtype == "R")
        REQUIRE(v.is_continuous());
    else if (vtype == "Z")
        REQUIRE(v.is_integer());
    else
        REQUIRE(v.is_binary());
}

void test_obj(coek::Model& model, unsigned int i, const std::list<std::string>& baseline,
              const std::string& name)
{
    auto obj = model.get_objective(i);
    REQUIRE(obj.to_list() == baseline);
    REQUIRE(obj.name() == name);
}

void test_con(coek::Model& model, unsigned int i, const std::list<std::string>& baseline,
              const std::string& name)
{
    auto con = model.get_constraint(i);
    REQUIRE(con.to_list() == baseline);
    REQUIRE(con.name() == name);
}

TEST_CASE("jpof_reader_file", "[smoke]")
{
    SECTION("bad")
    {
        std::map<std::string, coek::Parameter> params;
        REQUIRE_THROWS_WITH(coek::read_problem_from_jpof_file("bad.json", params),
                            "Unknown file: bad.json");
    }

    SECTION("error1")
    {
        std::map<std::string, coek::Parameter> params;
        // std::cout << currdir+"jpof/error1.json" << std::endl;
        REQUIRE_THROWS_WITH(
            coek::read_problem_from_jpof_file(currdir + "jpof/error1.json", params),
            "Error parsing JPOF file (offset 114): Missing a name for object member.");
    }

    SECTION("small1")
    {
        std::map<std::string, coek::Parameter> params;
        auto model = coek::read_problem_from_jpof_file(currdir + "jpof/small1.json", params);

        REQUIRE(model.num_variables() == 2);
        REQUIRE(model.num_objectives() == 1);
        REQUIRE(model.num_constraints() == 1);
        REQUIRE(params.size() == 0);

        REQUIRE(model.variable_names() == std::set<std::string>({"x", "y"}));
        REQUIRE(model.objective_names() == std::set<std::string>({"OBJ"}));
        REQUIRE(model.constraint_names() == std::set<std::string>({"CON1"}));

        test_var(model, 0, "x", 1.0, -COEK_INFINITY, COEK_INFINITY, false, "R");
        test_var(model, 1, "y", 1.0, -COEK_INFINITY, COEK_INFINITY, false, "R");

        test_obj(model, 0, {"[", "min", "[", "pow", "x", std::to_string(2.0), "]", "]"}, "OBJ");
        test_con(model, 0,
                 {"[", "==", "[", "pow", "y", std::to_string(2.0), "]", std::to_string(4.0), "]"},
                 "CON1");
    }

    SECTION("small2")
    {
        std::map<std::string, coek::Parameter> params;
        auto model = coek::read_problem_from_jpof_file(currdir + "jpof/small2.json", params);

        REQUIRE(model.num_variables() == 2);
        REQUIRE(model.num_constraints() == 1);
        REQUIRE(model.num_objectives() == 1);
        REQUIRE(params.size() == 0);

        REQUIRE(model.variable_names() == std::set<std::string>({"x", "y"}));
        REQUIRE(model.objective_names() == std::set<std::string>({"OBJ"}));
        REQUIRE(model.constraint_names() == std::set<std::string>({"CON1"}));

        test_var(model, 0, "x", 1.0, -COEK_INFINITY, COEK_INFINITY, false, "R");
        test_var(model, 1, "y", 1.0, -COEK_INFINITY, COEK_INFINITY, false, "R");

        test_obj(model, 0, {"[", "min", "x", "]"}, "OBJ");
        test_con(model, 0,
                 {"[", "==", "[", "pow", "y", std::to_string(2.0), "]", std::to_string(4.0), "]"},
                 "CON1");
    }

    SECTION("small3")
    {
        std::map<std::string, coek::Parameter> params;
        auto model = coek::read_problem_from_jpof_file(currdir + "jpof/small3.json", params);

        REQUIRE(model.num_variables() == 2);
        REQUIRE(model.num_constraints() == 1);
        REQUIRE(model.num_objectives() == 1);
        REQUIRE(params.size() == 0);

        REQUIRE(model.variable_names() == std::set<std::string>({"x", "y"}));
        REQUIRE(model.objective_names() == std::set<std::string>({"OBJ"}));
        REQUIRE(model.constraint_names() == std::set<std::string>({"CON1"}));

        test_var(model, 0, "x", 1.0, -COEK_INFINITY, COEK_INFINITY, false, "R");
        test_var(model, 1, "y", 1.0, -COEK_INFINITY, COEK_INFINITY, false, "R");

        test_obj(model, 0, {"[", "min", "[", "*", "x", "y", "]", "]"}, "OBJ");
        test_con(model, 0,
                 {"[", "==", "[", "pow", "y", std::to_string(2.0), "]", std::to_string(4.0), "]"},
                 "CON1");
    }

    SECTION("small4")
    {
        std::map<std::string, coek::Parameter> params;
        auto model = coek::read_problem_from_jpof_file(currdir + "jpof/small4.json", params);

        REQUIRE(model.num_variables() == 2);
        REQUIRE(model.num_constraints() == 1);
        REQUIRE(model.num_objectives() == 1);
        REQUIRE(params.size() == 0);

        REQUIRE(model.variable_names() == std::set<std::string>({"x", "y"}));
        REQUIRE(model.objective_names() == std::set<std::string>({"OBJ"}));
        REQUIRE(model.constraint_names() == std::set<std::string>({"CON1"}));

        test_var(model, 0, "x", 1.0, -COEK_INFINITY, COEK_INFINITY, false, "R");
        test_var(model, 1, "y", 1.0, -COEK_INFINITY, COEK_INFINITY, false, "R");

        test_obj(model, 0, {"[", "min", "[", "pow", "y", std::to_string(2.0), "]", "]"}, "OBJ");
        test_con(model, 0, {"[", "==", "[", "*", "y", "x", "]", std::to_string(4.0), "]"}, "CON1");
    }

    SECTION("small5")
    {
        std::map<std::string, coek::Parameter> params;
        auto model = coek::read_problem_from_jpof_file(currdir + "jpof/small5.json", params);

        REQUIRE(model.num_variables() == 3);
        REQUIRE(model.num_constraints() == 12);
        REQUIRE(model.num_objectives() == 1);
        REQUIRE(params.size() == 1);

        REQUIRE(model.variable_names() == std::set<std::string>({"v", "x", "y"}));
        REQUIRE(model.objective_names() == std::set<std::string>({"OBJ"}));
        REQUIRE(model.constraint_names()
                == std::set<std::string>({"CON1", "CON2", "CON3", "CON4", "CON5", "CON6", "CON7",
                                          "CON8", "CON9", "CON10", "CON11", "CON12"}));

        test_var(model, 0, "v", 3.0, -1.0, 1.0, false, "R");
        test_var(model, 1, "x", 1.0, -1.0, 1.0, false, "R");
        test_var(model, 2, "y", 2.0, -1.0, 1.0, false, "R");

        test_obj(model, 0,
                 {"[",
                  "min",
                  "[",
                  "+",
                  "[",
                  "/",
                  "[",
                  "pow",
                  "x",
                  std::to_string(2.0),
                  "]",
                  std::to_string(2.0),
                  "]",
                  "[",
                  "/",
                  "[",
                  "pow",
                  "x",
                  std::to_string(2.0),
                  "]",
                  "q",
                  "]",
                  "]",
                  "]"},
                 "OBJ");

        test_con(model, 0,
                 {"[",
                  "==",
                  "[",
                  "*",
                  "[",
                  "*",
                  std::to_string(0.5),
                  "v",
                  "]",
                  "[",
                  "+",
                  "x",
                  "[",
                  "*",
                  std::to_string(-1.0),
                  "y",
                  "]",
                  "]",
                  "]",
                  std::to_string(2.0),
                  "]"},
                 "CON1");
        test_con(model, 1,
                 {"[",
                  "==",
                  "[",
                  "*",
                  "v",
                  "[",
                  "+",
                  "[",
                  "*",
                  "[",
                  "/",
                  std::to_string(1.0),
                  "q",
                  "]",
                  "x",
                  "]",
                  "[",
                  "*",
                  "[",
                  "-",
                  "[",
                  "/",
                  std::to_string(1.0),
                  "q",
                  "]",
                  "]",
                  "y",
                  "]",
                  "]",
                  "]",
                  std::to_string(2.0),
                  "]"},
                 "CON10");
        test_con(model, 2,
                 {"[",
                  "==",
                  "[",
                  "*",
                  "[",
                  "*",
                  "v",
                  "[",
                  "+",
                  "x",
                  "[",
                  "*",
                  std::to_string(-1.0),
                  "y",
                  "]",
                  "]",
                  "]",
                  "[",
                  "/",
                  std::to_string(1.0),
                  "q",
                  "]",
                  "]",
                  std::to_string(2.0),
                  "]"},
                 "CON11");
        test_con(model, 3,
                 {"[",
                  "==",
                  "[",
                  "*",
                  "v",
                  "[",
                  "+",
                  "x",
                  "[",
                  "*",
                  std::to_string(-1.0),
                  "y",
                  "]",
                  "]",
                  "]",
                  "[",
                  "*",
                  std::to_string(2.0),
                  "q",
                  "]",
                  "]"},
                 "CON12");
        test_con(model, 4,
                 {"[",
                  "==",
                  "[",
                  "*",
                  "[",
                  "*",
                  std::to_string(0.5),
                  "v",
                  "]",
                  "[",
                  "+",
                  "x",
                  "[",
                  "*",
                  std::to_string(-1.0),
                  "y",
                  "]",
                  "]",
                  "]",
                  std::to_string(2.0),
                  "]"},
                 "CON2");
        test_con(model, 5,
                 {"[",
                  "==",
                  "[",
                  "/",
                  "[",
                  "*",
                  "v",
                  "[",
                  "+",
                  "x",
                  "[",
                  "*",
                  std::to_string(-1.0),
                  "y",
                  "]",
                  "]",
                  "]",
                  std::to_string(2.0),
                  "]",
                  std::to_string(2.0),
                  "]"},
                 "CON3");
        test_con(model, 6, {"[", "==", "[", "*", "v",
                            "[", "+",  "[", "*", std::to_string(0.5),
                            "x", "]",  "[", "*", std::to_string(-0.500),
                            "y", "]",  "]", "]", std::to_string(2.0),
                            "]"},
                 "CON4");
        test_con(model, 7,
                 {"[",
                  "==",
                  "[",
                  "*",
                  "[",
                  "*",
                  "v",
                  "[",
                  "+",
                  "x",
                  "[",
                  "*",
                  std::to_string(-1.0),
                  "y",
                  "]",
                  "]",
                  "]",
                  std::to_string(0.5),
                  "]",
                  std::to_string(2.0),
                  "]"},
                 "CON5");
        test_con(model, 8,
                 {"[", "==", "[", "*", "v", "[", "+", "x", "[", "*", std::to_string(-1.0), "y", "]",
                  "]", "]", std::to_string(4.0), "]"},
                 "CON6");
        test_con(model, 9,
                 {"[",
                  "==",
                  "[",
                  "*",
                  "[",
                  "*",
                  "[",
                  "/",
                  std::to_string(1.0),
                  "q",
                  "]",
                  "v",
                  "]",
                  "[",
                  "+",
                  "x",
                  "[",
                  "*",
                  std::to_string(-1.0),
                  "y",
                  "]",
                  "]",
                  "]",
                  std::to_string(2.0),
                  "]"},
                 "CON7");
        test_con(model, 10,
                 {"[",
                  "==",
                  "[",
                  "*",
                  "[",
                  "*",
                  "[",
                  "/",
                  std::to_string(1.0),
                  "q",
                  "]",
                  "v",
                  "]",
                  "[",
                  "+",
                  "x",
                  "[",
                  "*",
                  std::to_string(-1.0),
                  "y",
                  "]",
                  "]",
                  "]",
                  std::to_string(2.0),
                  "]"},
                 "CON8");
        test_con(model, 11,
                 {"[",
                  "==",
                  "[",
                  "/",
                  "[",
                  "*",
                  "v",
                  "[",
                  "+",
                  "x",
                  "[",
                  "*",
                  std::to_string(-1.0),
                  "y",
                  "]",
                  "]",
                  "]",
                  "q",
                  "]",
                  std::to_string(2.0),
                  "]"},
                 "CON9");
    }

    SECTION("small5a")
    {
        std::map<std::string, coek::Parameter> params;
        auto model = coek::read_problem_from_jpof_file(currdir + "jpof/small5a.json", params);

        REQUIRE(model.num_variables() == 2);
        REQUIRE(model.num_constraints() == 3);
        REQUIRE(model.num_objectives() == 1);
        REQUIRE(params.size() == 1);

        REQUIRE(model.variable_names() == std::set<std::string>({"x", "y"}));
        REQUIRE(model.objective_names() == std::set<std::string>({"OBJ"}));
        REQUIRE(model.constraint_names() == std::set<std::string>({"CON1", "CON2", "CON3"}));

        test_var(model, 0, "x", 0.0, 0, 1, false, "B");
        test_var(model, 1, "y", 0.0, -COEK_INFINITY, COEK_INFINITY, false, "Z");

        test_obj(model, 0, {"[", "min", "[", "*", "x", "y", "]", "]"}, "OBJ");

        test_con(model, 0,
                 {"[", "<=", "-Inf", "[", "+", "x", "y", "]", "[", "*", std::to_string(2.0), "q",
                  "]", "]"},
                 "CON1");
        test_con(model, 1, {"[", "<=", "q", "[", "+", "x", "y", "]", "Inf", "]"}, "CON2");
        test_con(
            model, 2,
            {"[", "<=", "q", "[", "+", "x", "y", "]", "[", "*", std::to_string(2.0), "q", "]", "]"},
            "CON3");
    }

    SECTION("small6")
    {
        std::map<std::string, coek::Parameter> params;
        auto model = coek::read_problem_from_jpof_file(currdir + "jpof/small6.json", params);

        REQUIRE(model.num_variables() == 4);
        REQUIRE(model.num_constraints() == 6);
        REQUIRE(model.num_objectives() == 1);
        REQUIRE(params.size() == 0);

        REQUIRE(model.variable_names() == std::set<std::string>({"p", "v", "x", "y"}));
        REQUIRE(model.objective_names() == std::set<std::string>({"OBJ"}));
        REQUIRE(model.constraint_names()
                == std::set<std::string>({"CON1", "CON2", "CON3", "CON4", "CON5", "CON6"}));

        test_var(model, 0, "p", 2.0, -COEK_INFINITY, COEK_INFINITY, true, "R");
        test_var(model, 1, "v", 3.0, -1.0, 1.0, false, "R");
        test_var(model, 2, "x", 1.0, -1.0, 1.0, false, "R");
        test_var(model, 3, "y", 2.0, -1.0, 1.0, false, "R");

        test_obj(model, 0,
                 {
                     "[",
                     "min",
                     "x",
                     "]",
                 },
                 "OBJ");

        test_con(model, 0,
                 {"[",
                  "==",
                  "[",
                  "*",
                  "[",
                  "*",
                  "[",
                  "/",
                  std::to_string(1.0),
                  "p",
                  "]",
                  "v",
                  "]",
                  "[",
                  "+",
                  "x",
                  "[",
                  "*",
                  std::to_string(-1.0),
                  "y",
                  "]",
                  "]",
                  "]",
                  std::to_string(2.0),
                  "]"},
                 "CON1");
        test_con(model, 1, {"[", "==", "[", "*", "[",
                            "/", "v",  "p", "]", "[",
                            "+", "x",  "[", "*", std::to_string(-1.0),
                            "y", "]",  "]", "]", std::to_string(2.0),
                            "]"},
                 "CON2");
        test_con(model, 2,
                 {"[",
                  "==",
                  "[",
                  "/",
                  "[",
                  "*",
                  "v",
                  "[",
                  "+",
                  "x",
                  "[",
                  "*",
                  std::to_string(-1.0),
                  "y",
                  "]",
                  "]",
                  "]",
                  "p",
                  "]",
                  std::to_string(2.0),
                  "]"},
                 "CON3");
        test_con(model, 3,
                 {"[",
                  "==",
                  "[",
                  "*",
                  "v",
                  "[",
                  "+",
                  "[",
                  "/",
                  "x",
                  "p",
                  "]",
                  "[",
                  "-",
                  "[",
                  "/",
                  "y",
                  "p",
                  "]",
                  "]",
                  "]",
                  "]",
                  std::to_string(2.0),
                  "]"},
                 "CON4");
        test_con(model, 4,
                 {"[",
                  "==",
                  "[",
                  "*",
                  "[",
                  "*",
                  "v",
                  "[",
                  "+",
                  "x",
                  "[",
                  "*",
                  std::to_string(-1.0),
                  "y",
                  "]",
                  "]",
                  "]",
                  "[",
                  "/",
                  std::to_string(1.0),
                  "p",
                  "]",
                  "]",
                  std::to_string(2.0),
                  "]"},
                 "CON5");
        test_con(model, 5,
                 {"[",
                  "==",
                  "[",
                  "+",
                  "[",
                  "*",
                  "v",
                  "[",
                  "+",
                  "x",
                  "[",
                  "*",
                  std::to_string(-1.0),
                  "y",
                  "]",
                  "]",
                  "]",
                  "[",
                  "*",
                  std::to_string(-2.0),
                  "p",
                  "]",
                  "]",
                  std::to_string(0.0),
                  "]"},
                 "CON6");
    }

    SECTION("small7")
    {
        std::map<std::string, coek::Parameter> params;
        auto model = coek::read_problem_from_jpof_file(currdir + "jpof/small7.json", params);

        REQUIRE(model.num_variables() == 4);
        REQUIRE(model.num_constraints() == 24);
        REQUIRE(model.num_objectives() == 1);
        REQUIRE(params.size() == 0);

        REQUIRE(model.variable_names() == std::set<std::string>({"p", "v", "x", "y"}));
        REQUIRE(model.objective_names() == std::set<std::string>({"OBJ"}));
        REQUIRE(model.constraint_names()
                == std::set<std::string>({"CON1a", "CON1b", "CON1c", "CON1d", "CON2a", "CON2b",
                                          "CON2c", "CON2d", "CON3a", "CON3b", "CON3c", "CON3d",
                                          "CON4a", "CON4b", "CON4c", "CON4d", "CON5a", "CON5b",
                                          "CON5c", "CON5d", "CON6a", "CON6b", "CON6c", "CON6d"}));

        test_var(model, 0, "p", 2.0, -COEK_INFINITY, COEK_INFINITY, true, "R");
        test_var(model, 1, "v", 3.0, -1.0, 1.0, false, "R");
        test_var(model, 2, "x", 1.0, -1.0, 1.0, false, "R");
        test_var(model, 3, "y", 2.0, -1.0, 1.0, false, "R");

        test_obj(model, 0,
                 {
                     "[",
                     "min",
                     "x",
                     "]",
                 },
                 "OBJ");

        test_con(model, 0,
                 {"[",
                  "==",
                  "[",
                  "*",
                  "[",
                  "*",
                  "[",
                  "/",
                  "[",
                  "/",
                  std::to_string(1.0),
                  "p",
                  "]",
                  std::to_string(2.0),
                  "]",
                  "v",
                  "]",
                  "[",
                  "+",
                  "x",
                  "[",
                  "*",
                  std::to_string(-1.0),
                  "y",
                  "]",
                  "]",
                  "]",
                  std::to_string(2.0),
                  "]"},
                 "CON1a");
        test_con(model, 1,
                 {"[",
                  "==",
                  "[",
                  "*",
                  "[",
                  "*",
                  "[",
                  "/",
                  std::to_string(1.0),
                  "[",
                  "*",
                  std::to_string(2.0),
                  "p",
                  "]",
                  "]",
                  "v",
                  "]",
                  "[",
                  "+",
                  "x",
                  "[",
                  "*",
                  std::to_string(-1.0),
                  "y",
                  "]",
                  "]",
                  "]",
                  std::to_string(2.0),
                  "]"},
                 "CON1b");
        test_con(model, 2,
                 {"[",
                  "==",
                  "[",
                  "*",
                  "[",
                  "*",
                  "[",
                  "/",
                  std::to_string(1.0),
                  "[",
                  "+",
                  "p",
                  std::to_string(2.0),
                  "]",
                  "]",
                  "v",
                  "]",
                  "[",
                  "+",
                  "x",
                  "[",
                  "*",
                  std::to_string(-1.0),
                  "y",
                  "]",
                  "]",
                  "]",
                  std::to_string(2.0),
                  "]"},
                 "CON1c");
        test_con(model, 3,
                 {"[",
                  "==",
                  "[",
                  "*",
                  "[",
                  "*",
                  "[",
                  "/",
                  std::to_string(1.0),
                  "[",
                  "pow",
                  "[",
                  "+",
                  "p",
                  std::to_string(2.0),
                  "]",
                  std::to_string(2.0),
                  "]",
                  "]",
                  "v",
                  "]",
                  "[",
                  "+",
                  "x",
                  "[",
                  "*",
                  std::to_string(-1.0),
                  "y",
                  "]",
                  "]",
                  "]",
                  std::to_string(2.0),
                  "]"},
                 "CON1d");

        test_con(model, 4,
                 {"[",
                  "==",
                  "[",
                  "*",
                  "[",
                  "/",
                  "[",
                  "/",
                  "v",
                  "p",
                  "]",
                  "p",
                  "]",
                  "[",
                  "+",
                  "x",
                  "[",
                  "*",
                  std::to_string(-1.0),
                  "y",
                  "]",
                  "]",
                  "]",
                  std::to_string(2.0),
                  "]"},
                 "CON2a");
        test_con(model, 5,
                 {"[",
                  "==",
                  "[",
                  "*",
                  "[",
                  "/",
                  "v",
                  "[",
                  "*",
                  "p",
                  "p",
                  "]",
                  "]",
                  "[",
                  "+",
                  "x",
                  "[",
                  "*",
                  std::to_string(-1.0),
                  "y",
                  "]",
                  "]",
                  "]",
                  std::to_string(2.0),
                  "]"},
                 "CON2b");
        test_con(model, 6,
                 {"[",
                  "==",
                  "[",
                  "*",
                  "[",
                  "/",
                  "v",
                  "[",
                  "+",
                  "p",
                  "p",
                  "]",
                  "]",
                  "[",
                  "+",
                  "x",
                  "[",
                  "*",
                  std::to_string(-1.0),
                  "y",
                  "]",
                  "]",
                  "]",
                  std::to_string(2.0),
                  "]"},
                 "CON2c");
        test_con(model, 7,
                 {"[",
                  "==",
                  "[",
                  "*",
                  "[",
                  "/",
                  "v",
                  "[",
                  "pow",
                  "[",
                  "+",
                  "p",
                  "p",
                  "]",
                  std::to_string(2.0),
                  "]",
                  "]",
                  "[",
                  "+",
                  "x",
                  "[",
                  "*",
                  std::to_string(-1.0),
                  "y",
                  "]",
                  "]",
                  "]",
                  std::to_string(2.0),
                  "]"},
                 "CON2d");

        test_con(model, 8,
                 {"[",
                  "==",
                  "[",
                  "/",
                  "[",
                  "/",
                  "[",
                  "*",
                  "v",
                  "[",
                  "+",
                  "x",
                  "[",
                  "*",
                  std::to_string(-1.0),
                  "y",
                  "]",
                  "]",
                  "]",
                  "p",
                  "]",
                  std::to_string(2.0),
                  "]",
                  std::to_string(2.0),
                  "]"},
                 "CON3a");
        test_con(model, 9,
                 {"[",
                  "==",
                  "[",
                  "/",
                  "[",
                  "*",
                  "v",
                  "[",
                  "+",
                  "x",
                  "[",
                  "*",
                  std::to_string(-1.0),
                  "y",
                  "]",
                  "]",
                  "]",
                  "[",
                  "*",
                  std::to_string(2.0),
                  "p",
                  "]",
                  "]",
                  std::to_string(2.0),
                  "]"},
                 "CON3b");
        test_con(model, 10,
                 {"[",
                  "==",
                  "[",
                  "/",
                  "[",
                  "*",
                  "v",
                  "[",
                  "+",
                  "x",
                  "[",
                  "*",
                  std::to_string(-1.0),
                  "y",
                  "]",
                  "]",
                  "]",
                  "[",
                  "+",
                  "p",
                  std::to_string(2.0),
                  "]",
                  "]",
                  std::to_string(2.0),
                  "]"},
                 "CON3c");
        test_con(model, 11,
                 {"[",
                  "==",
                  "[",
                  "/",
                  "[",
                  "*",
                  "v",
                  "[",
                  "+",
                  "x",
                  "[",
                  "*",
                  std::to_string(-1.0),
                  "y",
                  "]",
                  "]",
                  "]",
                  "[",
                  "pow",
                  "[",
                  "+",
                  "p",
                  std::to_string(2.0),
                  "]",
                  std::to_string(2.0),
                  "]",
                  "]",
                  std::to_string(2.0),
                  "]"},
                 "CON3d");

        test_con(model, 12,
                 {"[",
                  "==",
                  "[",
                  "*",
                  "v",
                  "[",
                  "+",
                  "[",
                  "/",
                  "[",
                  "/",
                  "x",
                  "p",
                  "]",
                  std::to_string(2.0),
                  "]",
                  "[",
                  "-",
                  "[",
                  "/",
                  "[",
                  "/",
                  "y",
                  "p",
                  "]",
                  std::to_string(2.0),
                  "]",
                  "]",
                  "]",
                  "]",
                  std::to_string(2.0),
                  "]"},
                 "CON4a");
        test_con(model, 13,
                 {"[",
                  "==",
                  "[",
                  "*",
                  "v",
                  "[",
                  "+",
                  "[",
                  "/",
                  "x",
                  "[",
                  "*",
                  std::to_string(2.0),
                  "p",
                  "]",
                  "]",
                  "[",
                  "-",
                  "[",
                  "/",
                  "y",
                  "[",
                  "*",
                  std::to_string(2.0),
                  "p",
                  "]",
                  "]",
                  "]",
                  "]",
                  "]",
                  std::to_string(2.0),
                  "]"},
                 "CON4b");
        test_con(model, 14,
                 {"[",
                  "==",
                  "[",
                  "*",
                  "v",
                  "[",
                  "+",
                  "[",
                  "/",
                  "x",
                  "[",
                  "+",
                  "p",
                  std::to_string(2.0),
                  "]",
                  "]",
                  "[",
                  "-",
                  "[",
                  "/",
                  "y",
                  "[",
                  "+",
                  "p",
                  std::to_string(2.0),
                  "]",
                  "]",
                  "]",
                  "]",
                  "]",
                  std::to_string(2.0),
                  "]"},
                 "CON4c");
        test_con(model, 15,
                 {"[",
                  "==",
                  "[",
                  "*",
                  "v",
                  "[",
                  "+",
                  "[",
                  "/",
                  "x",
                  "[",
                  "pow",
                  "[",
                  "+",
                  "p",
                  std::to_string(2.0),
                  "]",
                  std::to_string(2.0),
                  "]",
                  "]",
                  "[",
                  "-",
                  "[",
                  "/",
                  "y",
                  "[",
                  "pow",
                  "[",
                  "+",
                  "p",
                  std::to_string(2.0),
                  "]",
                  std::to_string(2.0),
                  "]",
                  "]",
                  "]",
                  "]",
                  "]",
                  std::to_string(2.0),
                  "]"},
                 "CON4d");

        test_con(model, 16,
                 {"[",
                  "==",
                  "[",
                  "*",
                  "[",
                  "*",
                  "v",
                  "[",
                  "+",
                  "x",
                  "[",
                  "*",
                  std::to_string(-1.0),
                  "y",
                  "]",
                  "]",
                  "]",
                  "[",
                  "/",
                  "[",
                  "/",
                  std::to_string(1.0),
                  "p",
                  "]",
                  std::to_string(2.0),
                  "]",
                  "]",
                  std::to_string(2.0),
                  "]"},
                 "CON5a");
        test_con(model, 17,
                 {"[",
                  "==",
                  "[",
                  "*",
                  "[",
                  "*",
                  "v",
                  "[",
                  "+",
                  "x",
                  "[",
                  "*",
                  std::to_string(-1.0),
                  "y",
                  "]",
                  "]",
                  "]",
                  "[",
                  "/",
                  std::to_string(1.0),
                  "[",
                  "*",
                  std::to_string(2.0),
                  "p",
                  "]",
                  "]",
                  "]",
                  std::to_string(2.0),
                  "]"},
                 "CON5b");
        test_con(model, 18,
                 {"[",
                  "==",
                  "[",
                  "*",
                  "[",
                  "*",
                  "v",
                  "[",
                  "+",
                  "x",
                  "[",
                  "*",
                  std::to_string(-1.0),
                  "y",
                  "]",
                  "]",
                  "]",
                  "[",
                  "/",
                  std::to_string(1.0),
                  "[",
                  "+",
                  "p",
                  std::to_string(2.0),
                  "]",
                  "]",
                  "]",
                  std::to_string(2.0),
                  "]"},
                 "CON5c");
        test_con(model, 19,
                 {"[",
                  "==",
                  "[",
                  "*",
                  "[",
                  "*",
                  "v",
                  "[",
                  "+",
                  "x",
                  "[",
                  "*",
                  std::to_string(-1.0),
                  "y",
                  "]",
                  "]",
                  "]",
                  "[",
                  "/",
                  std::to_string(1.0),
                  "[",
                  "pow",
                  "[",
                  "+",
                  "p",
                  std::to_string(2.0),
                  "]",
                  std::to_string(2.0),
                  "]",
                  "]",
                  "]",
                  std::to_string(2.0),
                  "]"},
                 "CON5d");

        test_con(model, 20,
                 {"[",
                  "==",
                  "[",
                  "+",
                  "[",
                  "*",
                  "v",
                  "[",
                  "+",
                  "x",
                  "[",
                  "*",
                  std::to_string(-1.0),
                  "y",
                  "]",
                  "]",
                  "]",
                  "[",
                  "*",
                  std::to_string(-4.0),
                  "p",
                  "]",
                  "]",
                  std::to_string(0.0),
                  "]"},
                 "CON6a");
        test_con(model, 21,
                 {"[",
                  "==",
                  "[",
                  "+",
                  "[",
                  "*",
                  "v",
                  "[",
                  "+",
                  "x",
                  "[",
                  "*",
                  std::to_string(-1.0),
                  "y",
                  "]",
                  "]",
                  "]",
                  "[",
                  "*",
                  std::to_string(-4.0),
                  "p",
                  "]",
                  "]",
                  std::to_string(0.0),
                  "]"},
                 "CON6b");
        test_con(model, 22,
                 {"[",
                  "==",
                  "[",
                  "+",
                  "[",
                  "*",
                  "v",
                  "[",
                  "+",
                  "x",
                  "[",
                  "*",
                  std::to_string(-1.0),
                  "y",
                  "]",
                  "]",
                  "]",
                  "[",
                  "-",
                  "[",
                  "*",
                  std::to_string(2.0),
                  "[",
                  "+",
                  "p",
                  std::to_string(2.0),
                  "]",
                  "]",
                  "]",
                  "]",
                  std::to_string(0.0),
                  "]"},
                 "CON6c");
        test_con(model, 23,
                 {"[",
                  "==",
                  "[",
                  "+",
                  "[",
                  "*",
                  "v",
                  "[",
                  "+",
                  "x",
                  "[",
                  "*",
                  std::to_string(-1.0),
                  "y",
                  "]",
                  "]",
                  "]",
                  "[",
                  "-",
                  "[",
                  "*",
                  std::to_string(2.0),
                  "[",
                  "pow",
                  "[",
                  "+",
                  "p",
                  std::to_string(2.0),
                  "]",
                  std::to_string(2.0),
                  "]",
                  "]",
                  "]",
                  "]",
                  std::to_string(0.0),
                  "]"},
                 "CON6d");
    }

    SECTION("small9")
    {
        std::map<std::string, coek::Parameter> params;
        auto model = coek::read_problem_from_jpof_file(currdir + "jpof/small9.json", params);

        REQUIRE(model.num_variables() == 3);
        REQUIRE(model.num_constraints() == 5);
        REQUIRE(model.num_objectives() == 1);
        REQUIRE(params.size() == 1);

        REQUIRE(model.variable_names() == std::set<std::string>({"x", "y", "z"}));
        REQUIRE(model.objective_names() == std::set<std::string>({"obj"}));
        REQUIRE(model.constraint_names()
                == std::set<std::string>({"con1", "con2", "con3", "con4", "con5"}));

        test_var(model, 0, "x", 0.0, -COEK_INFINITY, COEK_INFINITY, false, "R");
        test_var(model, 1, "y", 0.0, -COEK_INFINITY, COEK_INFINITY, true, "R");
        test_var(model, 2, "z", 0.0, -COEK_INFINITY, COEK_INFINITY, false, "R");

        test_obj(model, 0,
                 {
                     "[",
                     "min",
                     "x",
                     "]",
                 },
                 "obj");

        test_con(model, 0,
                 {"[", "==", "[", "+", "[", "*", "[", "*", "x", "y", "]", "z", "]", "x", "]",
                  std::to_string(1.0), "]"},
                 "con1");
        test_con(model, 1,
                 {"[", "==", "[", "+", "[", "*", "[", "*", "p", "x", "]", "z", "]", "x", "]",
                  std::to_string(1.0), "]"},
                 "con2");
        test_con(model, 2, {"[", "==", "x", std::to_string(1.0), "]"}, "con3");
        test_con(model, 3,
                 {"[", "==", "[", "*", "[", "*", "x", "y", "]", "z", "]", std::to_string(1.0), "]"},
                 "con4");
        test_con(model, 4,
                 {"[", "==", "[", "*", "[", "*", "p", "x", "]", "z", "]", std::to_string(1.0), "]"},
                 "con5");
    }

    SECTION("small10")
    {
        std::map<std::string, coek::Parameter> params;
        auto model = coek::read_problem_from_jpof_file(currdir + "jpof/small10.json", params);

        REQUIRE(model.num_variables() == 3);
        REQUIRE(model.num_constraints() == 14);
        REQUIRE(model.num_objectives() == 1);
        REQUIRE(params.size() == 1);

        REQUIRE(model.variable_names() == std::set<std::string>({"x", "y", "z"}));
        REQUIRE(model.objective_names() == std::set<std::string>({"obj"}));
        REQUIRE(model.constraint_names()
                == std::set<std::string>({"con1", "con2", "con3", "con4", "con5", "con6", "con7",
                                          "con8", "con9", "con12", "con13", "con14", "con15",
                                          "con17"}));

        test_var(model, 0, "x", 1.0, -COEK_INFINITY, COEK_INFINITY, true, "R");
        test_var(model, 1, "y", 0.0, -COEK_INFINITY, COEK_INFINITY, false, "R");
        test_var(model, 2, "z", 0.0, -COEK_INFINITY, COEK_INFINITY, true, "R");

        test_obj(model, 0,
                 {"[", "min", "[", "+", "[", "*", "x", "y", "]",   "[", "*",
                  "z", "y",   "]", "[", "*", "p", "y", "]", "[",   "*", "[",
                  "*", "y",   "y", "]", "p", "]", "[", "*", "[",   "*", "y",
                  "y", "]",   "z", "]", "[", "*", "z", "[", "pow", "y", std::to_string(2.0),
                  "]", "]",   "]", "]"},
                 "obj");

        test_con(model, 0, {"[", "==", "[", "*", "x", "y", "]", std::to_string(0.0), "]"}, "con1");
        test_con(model, 1,
                 {"[", "==", "[", "*", "p", "[", "pow", "y", std::to_string(2.0), "]", "]",
                  std::to_string(0.0), "]"},
                 "con12");
        test_con(model, 2,
                 {"[", "==", "[", "*", "[", "*", "p", "y", "]", "x", "]", std::to_string(0.0), "]"},
                 "con13");
        test_con(model, 3,
                 {"[", "==", "[", "*", "z", "[", "pow", "y", std::to_string(2.0), "]", "]",
                  std::to_string(0.0), "]"},
                 "con14");
        test_con(model, 4,
                 {"[", "==", "[", "*", "[", "*", "z", "y", "]", "x", "]", std::to_string(0.0), "]"},
                 "con15");
        test_con(model, 5, {"[", "==", "[", "*", "p", "y", "]", std::to_string(0.0), "]"}, "con17");
        test_con(model, 6,
                 {"[", "==", "[", "+", "[", "*", "z", "y", "]", "y", "]", std::to_string(0.0), "]"},
                 "con2");
        test_con(model, 7, {"[", "==", "y", std::to_string(0.0), "]"}, "con3");
        test_con(model, 8, {"[", "==", "y", std::to_string(0.0), "]"}, "con4");
        test_con(model, 9,
                 {"[", "==", "[", "+", "[", "*", "p", "[", "pow", "y", std::to_string(2.0), "]",
                  "]", "y", "]", std::to_string(0.0), "]"},
                 "con5");
        test_con(model, 10,
                 {"[", "==", "[", "+", "[", "*", "[", "*", "p", "y", "]", "x", "]", "y", "]",
                  std::to_string(0.0), "]"},
                 "con6");
        test_con(model, 11,
                 {"[", "==", "[", "+", "[", "*", "z", "[", "pow", "y", std::to_string(2.0), "]",
                  "]", "y", "]", std::to_string(0.0), "]"},
                 "con7");
        test_con(model, 12,
                 {"[", "==", "[", "+", "[", "*", "[", "*", "z", "y", "]", "x", "]", "y", "]",
                  std::to_string(0.0), "]"},
                 "con8");
        test_con(model, 13, {"[", "==", "[", "*", "z", "y", "]", std::to_string(0.0), "]"}, "con9");
    }

    SECTION("small11")
    {
        std::map<std::string, coek::Parameter> params;
        auto model = coek::read_problem_from_jpof_file(currdir + "jpof/small11.json", params);

        REQUIRE(model.num_variables() == 4);
        REQUIRE(model.variable_names()
                == std::set<std::string>({"x[1,1]", "x[1,2]", "x[1,3]", "x[3,3]"}));
        REQUIRE(model.num_objectives() == 1);
        REQUIRE(model.objective_names() == std::set<std::string>({"obj"}));
        REQUIRE(model.num_constraints() == 3);
        REQUIRE(model.constraint_names()
                == std::set<std::string>({"var_bnd[1]", "var_bnd[2]", "var_bnd[3]"}));
        REQUIRE(params.size() == 0);

        test_var(model, 0, "x[1,1]", 1.0, -COEK_INFINITY, COEK_INFINITY, true, "R");
        test_var(model, 1, "x[1,2]", 0.0, -COEK_INFINITY, COEK_INFINITY, false, "R");
        test_var(model, 2, "x[1,3]", 0.0, -COEK_INFINITY, COEK_INFINITY, false, "R");
        test_var(model, 3, "x[3,3]", 0.0, -COEK_INFINITY, COEK_INFINITY, false, "R");

        test_obj(model, 0, {"[", "min", "x[3,3]", "]"}, "obj");

        test_con(model, 0, {"[", "<=", std::to_string(-1.0), "x[1,1]", std::to_string(1.0), "]"},
                 "var_bnd[1]");
        test_con(model, 1, {"[", "<=", std::to_string(-1.0), "x[1,2]", std::to_string(1.0), "]"},
                 "var_bnd[2]");
        test_con(model, 2, {"[", "<=", std::to_string(-1.0), "x[1,3]", std::to_string(1.0), "]"},
                 "var_bnd[3]");
    }

    SECTION("small13")
    {
        std::map<std::string, coek::Parameter> params;
        auto model = coek::read_problem_from_jpof_file(currdir + "jpof/small13.json", params);

        REQUIRE(model.num_variables() == 1);
        REQUIRE(model.num_constraints() == 3);
        REQUIRE(model.num_objectives() == 1);
        REQUIRE(params.size() == 0);

        REQUIRE(model.variable_names() == std::set<std::string>({"x"}));
        REQUIRE(model.objective_names() == std::set<std::string>({"obj"}));
        REQUIRE(model.constraint_names() == std::set<std::string>({"c1", "c2", "c3"}));

        test_var(model, 0, "x", 0.5, -COEK_INFINITY, COEK_INFINITY, false, "R");

        test_obj(model, 0, {"[", "max", "x", "]"}, "obj");

        test_con(model, 0,
                 {"[", "==", "[", "+", "[", "pow", "x", std::to_string(3.000), "]", "[", "*",
                  std::to_string(-1.0), "x", "]", "]", std::to_string(0.0), "]"},
                 "c1");
        test_con(model, 1,
                 {"[",
                  "==",
                  "[",
                  "*",
                  std::to_string(10.000),
                  "[",
                  "+",
                  "[",
                  "pow",
                  "x",
                  std::to_string(3.0),
                  "]",
                  "[",
                  "*",
                  std::to_string(-1.0),
                  "x",
                  "]",
                  "]",
                  "]",
                  std::to_string(0.0),
                  "]"},
                 "c2");
        test_con(model, 2, {"[", "==",
                            "[", "/",
                            "[", "+",
                            "[", "pow",
                            "x", std::to_string(3.0),
                            "]", "[",
                            "*", std::to_string(-1.0),
                            "x", "]",
                            "]", std::to_string(10.0),
                            "]", std::to_string(0.0),
                            "]"},
                 "c3");
    }

    SECTION("small14")
    {
        std::map<std::string, coek::Parameter> params;
        auto model = coek::read_problem_from_jpof_file(currdir + "jpof/small14.json", params);

        REQUIRE(model.num_variables() == 2);
        REQUIRE(model.num_constraints() == 19);
        REQUIRE(model.num_objectives() == 1);
        REQUIRE(params.size() == 0);

        REQUIRE(model.variable_names() == std::set<std::string>({"ONE", "ZERO"}));
        REQUIRE(model.objective_names() == std::set<std::string>({"obj"}));
        REQUIRE(model.constraint_names()
                == std::set<std::string>({"c_abs",  "c_acos",  "c_acosh", "c_asin", "c_asinh",
                                          "c_atan", "c_atanh", "c_ceil",  "c_cos",  "c_cos",
                                          "c_cosh", "c_exp",   "c_floor", "c_log",  "c_log10",
                                          "c_sin",  "c_sinh",  "c_sqrt",  "c_tan",  "c_tanh"}));

        test_var(model, 0, "ONE", 1, -COEK_INFINITY, COEK_INFINITY, false, "R");
        test_var(model, 1, "ZERO", 0, -COEK_INFINITY, COEK_INFINITY, false, "R");

        test_obj(model, 0, {"[", "min", "[", "+", "ONE", "ZERO", "]", "]"}, "obj");

        test_con(model, 0, {"[", "==", "[", "abs", "ONE", "]", std::to_string(1.0), "]"}, "c_abs");
        test_con(model, 1, {"[", "==", "[", "acos", "ZERO", "]", std::to_string(1.570796), "]"},
                 "c_acos");
        test_con(model, 2,
                 {"[", "==", "[", "acosh", "[", "/", "[", "+", "ONE", std::to_string(7.389056), "]",
                  std::to_string(5.436564), "]", "]", std::to_string(0.0), "]"},
                 "c_acosh");
        test_con(model, 3, {"[", "==", "[", "asin", "ZERO", "]", std::to_string(0.0), "]"},
                 "c_asin");
        test_con(model, 4, {"[", "==", "[", "asinh", "ZERO", "]", std::to_string(0.0), "]"},
                 "c_asinh");
        test_con(model, 5, {"[", "==", "[", "atan", "ZERO", "]", std::to_string(0.0), "]"},
                 "c_atan");
        test_con(model, 6, {"[", "==", "[", "atanh", "ZERO", "]", std::to_string(0.0), "]"},
                 "c_atanh");
        test_con(model, 7, {"[", "==", "[", "ceil", "ONE", "]", std::to_string(1.0), "]"},
                 "c_ceil");
        test_con(model, 8, {"[", "==", "[", "cos", "ZERO", "]", std::to_string(1.0), "]"}, "c_cos");
        test_con(model, 9, {"[", "==", "[", "cosh", "ZERO", "]", std::to_string(1.0), "]"},
                 "c_cosh");
        test_con(model, 10, {"[", "==", "[", "exp", "ZERO", "]", std::to_string(1.0), "]"},
                 "c_exp");
        test_con(model, 11, {"[", "==", "[", "floor", "ONE", "]", std::to_string(1.0), "]"},
                 "c_floor");
        test_con(model, 12, {"[", "==", "[", "log", "ONE", "]", std::to_string(0.0), "]"}, "c_log");
        test_con(model, 13, {"[", "==", "[", "log10", "ONE", "]", std::to_string(0.0), "]"},
                 "c_log10");
        test_con(model, 14, {"[", "==", "[", "sin", "ZERO", "]", std::to_string(0.0), "]"},
                 "c_sin");
        test_con(model, 15, {"[", "==", "[", "sinh", "ZERO", "]", std::to_string(0.0), "]"},
                 "c_sinh");
        test_con(model, 16, {"[", "==", "[", "sqrt", "ONE", "]", std::to_string(1.0), "]"},
                 "c_sqrt");
        test_con(model, 17, {"[", "==", "[", "tan", "ZERO", "]", std::to_string(0.0), "]"},
                 "c_tan");
        test_con(model, 18, {"[", "==", "[", "tanh", "ZERO", "]", std::to_string(0.0), "]"},
                 "c_tanh");
    }

    SECTION("small15")
    {
        std::map<std::string, coek::Parameter> params;
        auto model = coek::read_problem_from_jpof_file(currdir + "jpof/small15.json", params);

        REQUIRE(model.num_variables() == 2);
        REQUIRE(model.num_constraints() == 1);
        REQUIRE(model.num_objectives() == 1);
        REQUIRE(params.size() == 0);

        REQUIRE(model.variable_names() == std::set<std::string>({"x", "b.y"}));
        REQUIRE(model.objective_names() == std::set<std::string>({"OBJ"}));
        REQUIRE(model.constraint_names() == std::set<std::string>({"CON1"}));

        test_var(model, 0, "x", 1, -COEK_INFINITY, COEK_INFINITY, false, "R");
        test_var(model, 1, "b.y", 1, -COEK_INFINITY, COEK_INFINITY, false, "R");

        test_obj(model, 0, {"[", "min", "[", "pow", "x", std::to_string(2.0), "]", "]"}, "OBJ");

        test_con(model, 0,
                 {"[", "==", "[", "pow", "b.y", std::to_string(2.0), "]", std::to_string(4.0), "]"},
                 "CON1");
    }
}
#endif
