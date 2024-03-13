#include <iostream>
#include <tuple>

#include "catch2/catch.hpp"
#include "coek/util/DataPortal.hpp"

TEST_CASE("get_DP_set", "[smoke]")
{
    SECTION("error1")
    {
        auto dp = coek::DataPortal();
        std::set<int> data;
        REQUIRE(dp.get("unknown", data) == false);
    }

    SECTION("error2")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string(R"(
{
"A": {
    "set_type": "i",
    "data": [1, 2, 3]
    }
}
)");
        std::set<std::string> data;
        REQUIRE(dp.get("unknown", data) == false);
        REQUIRE(dp.get("A", data) == false);
    }

    SECTION("int-set")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string(R"(
{
"A": {
    "set_type": "i",
    "data": [1, 2, 3]
    }
}
)");
        std::set<int> data;
        REQUIRE(dp.get("unknown", data) == false);
        REQUIRE(dp.get("A", data) == true);
        std::set<int> baseline = {1, 2, 3};
        REQUIRE(data == baseline);
    }

    SECTION("double-set")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string(R"(
{
"A": {
    "set_type": "d",
    "data": [1.1, 2, 3]
    }
}
)");
        std::set<double> data;
        REQUIRE(dp.get("unknown", data) == false);
        REQUIRE(dp.get("A", data) == true);
        std::set<double> baseline = {1.1, 2, 3};
        REQUIRE(data == baseline);
    }

    SECTION("string-set")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string(R"(
{
"A": {
    "set_type": "s",
    "data": ["a", "b", "c"]
    }
}
)");
        std::set<std::string> data;
        REQUIRE(dp.get("unknown", data) == false);
        REQUIRE(dp.get("A", data) == true);
    }

    SECTION("int-tuple-set")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string(R"(
{
"A": {
    "set_type": ["i","i"],
    "data": [   [0, 0],
                [1, 1],
                [2, 2]]
    }
}
)");
        std::set<std::tuple<int, int>> data;
        REQUIRE(dp.get("unknown", data) == false);
        REQUIRE(dp.get("A", data) == true);
    }

    SECTION("double-tuple-set")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string(R"(
{
"A": {
    "set_type": ["d","d"],
    "data": [   [0, 0],
                [1.2, 1],
                [2.3, 2]]
    }
}
)");
        std::set<std::tuple<double, double>> data;
        REQUIRE(dp.get("unknown", data) == false);
        REQUIRE(dp.get("A", data) == true);
    }

    SECTION("string-tuple-set")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string(R"(
{
"A": {
    "set_type": ["s","s"],
    "data": [   ["a", "a"],
                ["b", "b"],
                ["c", "c"]]
    }
}
)");
        std::set<std::tuple<std::string, std::string>> data;
        REQUIRE(dp.get("unknown", data) == false);
        REQUIRE(dp.get("A", data) == true);
    }

    SECTION("mixed-tuple-set")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string(R"(
{
"A": {
    "set_type": ["s","i","d"],
    "data": [   ["a", 0, 1],
                ["b", 1, 2.2],
                ["c", 2, 3.3]]
    }
}
)");
        std::set<std::tuple<std::string, int, double>> data;
        REQUIRE(dp.get("unknown", data) == false);
        REQUIRE(dp.get("A", data) == true);
    }
}

TEST_CASE("get_DP_param", "[smoke]")
{
    SECTION("error1")
    {
        auto dp = coek::DataPortal();
        int data;
        REQUIRE(dp.get("unknown", data) == false);
    }

    SECTION("error2")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string("{ \"A\": 1 }");
        std::string data;
        REQUIRE(dp.get("unknown", data) == false);
        REQUIRE(dp.get("A", data) == false);
    }

    SECTION("int-param")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string("{ \"A\": 1 }");
        int data = 0;
        REQUIRE(dp.get("unknown", data) == false);
        REQUIRE(dp.get("A", data) == true);
        REQUIRE(data == 1);
    }

    SECTION("double-param")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string("{ \"A\": 1.1 }");
        double data = 0;
        REQUIRE(dp.get("unknown", data) == false);
        REQUIRE(dp.get("A", data) == true);
        REQUIRE(data == 1.1);
    }

    SECTION("double-param-from-int")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string("{ \"A\": 1 }");
        double data = 0;
        REQUIRE(dp.get("unknown", data) == false);
        REQUIRE(dp.get("A", data) == true);
        REQUIRE(data == 1);
    }

    SECTION("string-param")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string("{ \"A\": \"a\" }");
        std::string data = "";
        REQUIRE(dp.get("unknown", data) == false);
        REQUIRE(dp.get("A", data) == true);
        REQUIRE(data == "a");
    }

#if 0
// TODO
    SECTION("tuple-param")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string("{ \"A\": [\"a\",1] }");
        std::tuple<std::string,int> data;
        REQUIRE(dp.get("unknown", data) == false);
        REQUIRE(dp.get("A", data) == true);
        auto baseline = std::make_tuple("a",1);
        REQUIRE(data == baseline);
    }
#endif
}

TEST_CASE("get_DP_indexed_set", "[smoke]")
{
    SECTION("error1")
    {
        auto dp = coek::DataPortal();
        std::map<int, std::set<int>> data;
        REQUIRE(dp.get("unknown", data) == false);
    }

    SECTION("error2")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string("{ \"A\": 1 }");
        std::map<int, std::set<int>> data;
        REQUIRE(dp.get("unknown", data) == false);
        REQUIRE(dp.get("A", data) == false);
    }

    SECTION("int-set-int")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string(R"(
{
"A": {
    "key_type": "i",
    "set_type": "i",
    "data":  [
                [0, [1]],
                [1, [1, 2]],
                [2, [1, 2, 3]]
             ]
    }
}
)");

        std::map<int, std::set<int>> data;
        REQUIRE(dp.get("unknown", data) == false);
        REQUIRE(dp.get("A", data) == true);
        REQUIRE(data.size() == 3);
        std::set<int> baseline = {1, 2, 3};
        REQUIRE(data[2] == baseline);
    }

    SECTION("int-set-string")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string(R"(
{
"A": {
    "key_type": "i",
    "set_type": "s",
    "data":  [
                [0, ["a"]],
                [1, ["a", "b"]],
                [2, ["a", "b", "c"]]
             ]
    }
}
)");

        std::map<int, std::set<std::string>> data;
        REQUIRE(dp.get("unknown", data) == false);
        REQUIRE(dp.get("A", data) == true);
        REQUIRE(data.size() == 3);
        std::set<std::string> baseline = {"a", "b", "c"};
        REQUIRE(data[2] == baseline);
    }

    SECTION("string-set-string")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string(R"(
{
"A": {
    "key_type": "s",
    "set_type": "s",
    "data":  [
                ["0", ["a"]],
                ["1", ["a", "b"]],
                ["2", ["a", "b", "c"]]
             ]
    }
}
)");

        std::map<std::string, std::set<std::string>> data;
        REQUIRE(dp.get("unknown", data) == false);
        REQUIRE(dp.get("A", data) == true);
        REQUIRE(data.size() == 3);
        std::set<std::string> baseline = {"a", "b", "c"};
        REQUIRE(data["2"] == baseline);
    }

    SECTION("string-set-int")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string(R"(
{
"A": {
    "key_type": "s",
    "set_type": "i",
    "data":  [
                ["0", [0]],
                ["1", [0, 1]],
                ["2", [0, 1, 2]]
             ]
    }
}
)");

        std::map<std::string, std::set<int>> data;
        REQUIRE(dp.get("unknown", data) == false);
        REQUIRE(dp.get("A", data) == true);
        REQUIRE(data.size() == 3);
        std::set<int> baseline = {0, 1, 2};
        REQUIRE(data["2"] == baseline);
    }

    SECTION("string-tuple-set-int")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string(R"(
{
"A": {
    "key_type": ["s", "s"],
    "set_type": "i",
    "data":  [
                [["0", "1"], [0]],
                [["1", "1"], [0, 1]],
                [["2", "1"], [0, 1, 2]]
             ]
    }
}
)");

        std::map<std::tuple<std::string, std::string>, std::set<int>> data;
        REQUIRE(dp.get("unknown", data) == false);
        REQUIRE(dp.get("A", data) == true);
        REQUIRE(data.size() == 3);
        std::set<int> baseline{0, 1, 2};
        REQUIRE(data[{"2", "1"}] == baseline);
    }

    SECTION("int-tuple-set-int")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string(R"(
{
"A": {
    "key_type": ["i", "i"],
    "set_type": "i",
    "data":  [
                [[0, 1], [0]],
                [[1, 1], [0, 1]],
                [[2, 1], [0, 1, 2]]
             ]
    }
}
)");

        std::map<std::tuple<int, int>, std::set<int>> data;
        REQUIRE(dp.get("unknown", data) == false);
        REQUIRE(dp.get("A", data) == true);
        REQUIRE(data.size() == 3);
        std::set<int> baseline{0, 1, 2};
        REQUIRE(data[{2, 1}] == baseline);
    }

    SECTION("mixed-tuple-set-int")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string(R"(
{
"A": {
    "key_type": ["s", "i"],
    "set_type": "i",
    "data":  [
                [["0", 1], [0]],
                [["1", 1], [0, 1]],
                [["2", 1], [0, 1, 2]]
             ]
    }
}
)");

        std::map<std::tuple<std::string, int>, std::set<int>> data;
        REQUIRE(dp.get("unknown", data) == false);
        REQUIRE(dp.get("A", data) == true);
        REQUIRE(data.size() == 3);
        std::set<int> baseline{0, 1, 2};
        REQUIRE(data[{"2", 1}] == baseline);
    }

#if 0
// TODO

    SECTION("mixed-tuple-set-tuple-int")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string(R"(
{
"A": {
    "key_type": ["s", "i"],
    "set_type": ["i", "i"],
    "data":  [
                [["0", 1], [[0,0]]],
                [["1", 1], [[0,0], [1,1]]],
                [["2", 1], [[0,0], [1,1], [2,2]]]
             ]
    }
}
)");

        std::map<std::tuple<std::string, int>, std::set<int>> data;
        REQUIRE(dp.get("unknown", data) == false);
        REQUIRE(dp.get("A", data) == true);
        REQUIRE(data.size() == 3);
        std::set<std::tuple<int,int>> baseline{{0,0}, {1,1}, {2,2}};
        REQUIRE(data[{"2", 1}] == baseline);
    }
#endif
}

TEST_CASE("get_DP_indexed_param", "[smoke]")
{
    SECTION("error1")
    {
        auto dp = coek::DataPortal();
        std::map<int, int> data;
        REQUIRE(dp.get("unknown", data) == false);
    }

    SECTION("error2")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string(R"(
{
"A": 1
}
)");
        std::map<int, int> data;
        REQUIRE(dp.get("unknown", data) == false);
        REQUIRE(dp.get("A", data) == false);
    }

    SECTION("int-int-param")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string(R"(
{
"A": {
    "key_type": "i",
    "param_type": "i",
    "data":  [
                [0, 0],
                [1, 1],
                [2, 2]
             ]
    }
}
)");

        std::map<int, int> data;
        REQUIRE(dp.get("unknown", data) == false);
        REQUIRE(dp.get("A", data) == true);
        REQUIRE(data.size() == 3);
        REQUIRE(data[2] == 2);
    }

    SECTION("int-string-param")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string(R"(
{
"A": {
    "key_type": "i",
    "param_type": "s",
    "data":  [
                [0, "a"],
                [1, "b"],
                [2, "c"]
             ]
    }
}
)");

        std::map<int, std::string> data;
        REQUIRE(dp.get("unknown", data) == false);
        REQUIRE(dp.get("A", data) == true);
        REQUIRE(data.size() == 3);
        REQUIRE(data[2] == "c");
    }

    SECTION("string-string-param")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string(R"(
{
"A": {
    "key_type": "s",
    "param_type": "s",
    "data":  [
                ["0", "a"],
                ["1", "b"],
                ["2", "c"]
             ]
    }
}
)");

        std::map<std::string, std::string> data;
        REQUIRE(dp.get("unknown", data) == false);
        REQUIRE(dp.get("A", data) == true);
        REQUIRE(data.size() == 3);
        REQUIRE(data["2"] == "c");
    }

    SECTION("string-int-param")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string(R"(
{
"A": {
    "key_type": "s",
    "param_type": "i",
    "data":  [
                ["0", 0],
                ["1", 1],
                ["2", 2]
             ]
    }
}
)");

        std::map<std::string, int> data;
        REQUIRE(dp.get("unknown", data) == false);
        REQUIRE(dp.get("A", data) == true);
        REQUIRE(data.size() == 3);
        REQUIRE(data["2"] == 2);
    }

    SECTION("string-tuple-int-param")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string(R"(
{
"A": {
    "key_type": ["s", "s"],
    "param_type": "i",
    "data":  [
                [["0", "1"], 0],
                [["1", "2"], 1],
                [["2", "3"], 2]
             ]
    }
}
)");

        std::map<std::tuple<std::string, std::string>, int> data;
        REQUIRE(dp.get("unknown", data) == false);
        REQUIRE(dp.get("A", data) == true);
        REQUIRE(data.size() == 3);
        REQUIRE(data[{"2", "3"}] == 2);
    }

    SECTION("int-tuple-param")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string(R"(
{
"A": {
    "key_type": "i",
    "param_type": ["d", "s", "i"],
    "data":  [
                [1, [0.1, "a", 0]],
                [2, [1.1, "b", 1]],
                [3, [2.1, "c", 2]]
             ]
    }
}
)");

        std::map<int, std::tuple<double, std::string, int>> data;
        REQUIRE(dp.get("unknown", data) == false);
        REQUIRE(dp.get("A", data) == true);
        REQUIRE(data.size() == 3);
        auto baseline = std::make_tuple<double, std::string, int>(2.1, "c", 2);
        REQUIRE(data[3] == baseline);

        std::map<std::tuple<std::string, int>, int> idata;
        REQUIRE(dp.get("A", idata) == false);
    }

    SECTION("int-tuple-int-param")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string(R"(
{
"A": {
    "key_type": ["i", "i"],
    "param_type": "i",
    "data":  [
                [[0, 1], 0],
                [[1, 2], 1],
                [[2, 3], 2]
             ]
    }
}
)");

        std::map<std::tuple<int, int>, int> data;
        REQUIRE(dp.get("unknown", data) == false);
        REQUIRE(dp.get("A", data) == true);
        REQUIRE(data.size() == 3);
        REQUIRE(data[{2, 3}] == 2);
    }

    SECTION("mixed-tuple-int-param")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string(R"(
{
"A": {
    "key_type": ["s", "i"],
    "param_type": "i",
    "data":  [
                [["0", 1], 0],
                [["1", 2], 1],
                [["2", 3], 2]
             ]
    }
}
)");

        std::map<std::tuple<std::string, int>, int> data;
        REQUIRE(dp.get("unknown", data) == false);
        REQUIRE(dp.get("A", data) == true);
        REQUIRE(data.size() == 3);
        REQUIRE(data[{"2", 3}] == 2);
    }

    SECTION("mixed-tuple-double-param")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string(R"(
{
"A": {
    "key_type": ["s", "i"],
    "param_type": "d",
    "data":  [
                [["0", 1], 0.1],
                [["1", 2], 1.1],
                [["2", 3], 2.1]
             ]
    }
}
)");

        std::map<std::tuple<std::string, int>, double> data;
        REQUIRE(dp.get("unknown", data) == false);
        REQUIRE(dp.get("A", data) == true);
        REQUIRE(data.size() == 3);
        REQUIRE(data[{"2", 3}] == 2.1);

        std::map<std::tuple<std::string, int>, int> idata;
        REQUIRE(dp.get("A", idata) == false);
    }

    SECTION("mixed-tuple-tuple-param")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string(R"(
{
"A": {
    "key_type": ["s", "i"],
    "param_type": ["d", "s", "i"],
    "data":  [
                [["0", 1], [0.1, "a", 0]],
                [["1", 2], [1.1, "b", 1]],
                [["2", 3], [2.1, "c", 2]]
             ]
    }
}
)");

        std::map<std::tuple<std::string, int>, std::tuple<double, std::string, int>> data;
        REQUIRE(dp.get("unknown", data) == false);
        REQUIRE(dp.get("A", data) == true);
        REQUIRE(data.size() == 3);
        auto baseline = std::make_tuple<double, std::string, int>(2.1, "c", 2);
        REQUIRE(data[{"2", 3}] == baseline);

        std::map<std::tuple<std::string, int>, int> idata;
        REQUIRE(dp.get("A", idata) == false);
    }
}

TEST_CASE("put_DP_set", "[smoke]")
{
    SECTION("int-set")
    {
        auto dp = coek::DataPortal();
        std::set<int> data = {1, 2, 3};
        REQUIRE(dp.put("A", data) == true);
        REQUIRE( dp.to_string(4) == R"({
    "A": {
        "set_type": [
            "i"
        ],
        "data": [
            1,
            2,
            3
        ]
    }
})");
    }

    SECTION("double-set")
    {
        auto dp = coek::DataPortal();
        std::set<double> data = {1.1, 2, 3};
        REQUIRE(dp.put("A", data) == true);
        REQUIRE( dp.to_string(4) == R"({
    "A": {
        "set_type": [
            "d"
        ],
        "data": [
            1.1,
            2.0,
            3.0
        ]
    }
})");
    }

    SECTION("string-set")
    {
        auto dp = coek::DataPortal();
        std::set<std::string> data = {"a", "b", "c"};
        REQUIRE(dp.put("A", data) == true);
        REQUIRE( dp.to_string(4) == R"({
    "A": {
        "set_type": [
            "s"
        ],
        "data": [
            "a",
            "b",
            "c"
        ]
    }
})");
    }

    SECTION("int-tuple-set")
    {
        auto dp = coek::DataPortal();
        std::set<std::tuple<int,int>> data = {{0,0}, {1,1}, {2,2}};
        REQUIRE(dp.put("A", data) == true);
        REQUIRE( dp.to_string(4) == R"({
    "A": {
        "set_type": [
            "i",
            "i"
        ],
        "data": [
            [
                0,
                0
            ],
            [
                1,
                1
            ],
            [
                2,
                2
            ]
        ]
    }
})");
    }

    SECTION("double-tuple-set")
    {
        auto dp = coek::DataPortal();
        std::set<std::tuple<double,double>> data = {{0,0}, {1.2,1}, {2.3,2}};
        REQUIRE(dp.put("A", data) == true);
        REQUIRE( dp.to_string(4) == R"({
    "A": {
        "set_type": [
            "d",
            "d"
        ],
        "data": [
            [
                0.0,
                0.0
            ],
            [
                1.2,
                1.0
            ],
            [
                2.3,
                2.0
            ]
        ]
    }
})");
    }

    SECTION("string-tuple-set")
    {
        auto dp = coek::DataPortal();
        std::set<std::tuple<std::string,std::string>> data = {{"a","a"}, {"b","b"}, {"c","c"}};
        REQUIRE(dp.put("A", data) == true);
        REQUIRE( dp.to_string(4) == R"({
    "A": {
        "set_type": [
            "s",
            "s"
        ],
        "data": [
            [
                "a",
                "a"
            ],
            [
                "b",
                "b"
            ],
            [
                "c",
                "c"
            ]
        ]
    }
})");
    }

    SECTION("mixed-tuple-set")
    {
        auto dp = coek::DataPortal();
        std::set<std::tuple<std::string,int,double>> data = {{"a",0,1}, {"b",1,2.2}, {"c",2,3.3}};
        REQUIRE(dp.put("A", data) == true);
        REQUIRE( dp.to_string(4) == R"({
    "A": {
        "set_type": [
            "s",
            "i",
            "d"
        ],
        "data": [
            [
                "a",
                0,
                1.0
            ],
            [
                "b",
                1,
                2.2
            ],
            [
                "c",
                2,
                3.3
            ]
        ]
    }
})");
    }
}

TEST_CASE("put_DP_param", "[smoke]")
{
    SECTION("error1")
    {
        auto dp = coek::DataPortal();
        int data;
        dp.put("unknown", data);
        REQUIRE(dp.put("A", data) == true);
    }

    SECTION("int-param")
    {
        auto dp = coek::DataPortal();
        int data = 1;
        REQUIRE(dp.put("A", data) == true);
        REQUIRE( dp.to_string(4) == R"({
    "A": 1
})");
    }

    SECTION("double-param")
    {
        auto dp = coek::DataPortal();
        double data = 0.1;
        REQUIRE(dp.put("A", data) == true);
        REQUIRE( dp.to_string(4) == R"({
    "A": 0.1
})");
    }

    SECTION("double-param-from-int")
    {
        auto dp = coek::DataPortal();
        double data = 1;
        REQUIRE(dp.put("A", data) == true);
        REQUIRE( dp.to_string(4) == R"({
    "A": 1.0
})");
    }

    SECTION("string-param")
    {
        auto dp = coek::DataPortal();
        std::string data = "abc";
        REQUIRE(dp.put("A", data) == true);
        REQUIRE( dp.to_string(4) == R"({
    "A": "abc"
})");
    }

#if 0
// TODO - tuple param
#endif
}

TEST_CASE("put_DP_indexed_set", "[smoke]")
{
    SECTION("int-set-int")
    {
        auto dp = coek::DataPortal();
        std::map<int,std::set<int>> data = {{0,{1}}, {1,{1,2}}, {2,{1,2,3}}};
        REQUIRE(dp.put("A", data) == true);
        REQUIRE( dp.to_string(4) == R"({
    "A": {
        "key_type": [
            "i"
        ],
        "set_type": [
            "i"
        ],
        "data": [
            [
                0,
                [
                    1
                ]
            ],
            [
                1,
                [
                    1,
                    2
                ]
            ],
            [
                2,
                [
                    1,
                    2,
                    3
                ]
            ]
        ]
    }
})");
    }

    SECTION("int-set-string")
    {
        auto dp = coek::DataPortal();
        std::map<int,std::set<std::string>> data = {{0,{"1"}}, {1,{"1","2"}}, {2,{"1","2","3"}}};
        REQUIRE(dp.put("A", data) == true);
        REQUIRE( dp.to_string(4) == R"({
    "A": {
        "key_type": [
            "i"
        ],
        "set_type": [
            "s"
        ],
        "data": [
            [
                0,
                [
                    "1"
                ]
            ],
            [
                1,
                [
                    "1",
                    "2"
                ]
            ],
            [
                2,
                [
                    "1",
                    "2",
                    "3"
                ]
            ]
        ]
    }
})");
    }

    SECTION("string-set-string")
    {
        auto dp = coek::DataPortal();
        std::map<std::string,std::set<std::string>> data = {{"0",{"1"}}, {"1",{"1","2"}}, {"2",{"1","2","3"}}};
        REQUIRE(dp.put("A", data) == true);
        REQUIRE( dp.to_string(4) == R"({
    "A": {
        "key_type": [
            "s"
        ],
        "set_type": [
            "s"
        ],
        "data": [
            [
                "0",
                [
                    "1"
                ]
            ],
            [
                "1",
                [
                    "1",
                    "2"
                ]
            ],
            [
                "2",
                [
                    "1",
                    "2",
                    "3"
                ]
            ]
        ]
    }
})");
    }

    SECTION("string-set-int")
    {
        auto dp = coek::DataPortal();
        std::map<std::string,std::set<int>> data = {{"0",{1}}, {"1",{1,2}}, {"2",{1,2,3}}};
        REQUIRE(dp.put("A", data) == true);
        REQUIRE( dp.to_string(4) == R"({
    "A": {
        "key_type": [
            "s"
        ],
        "set_type": [
            "i"
        ],
        "data": [
            [
                "0",
                [
                    1
                ]
            ],
            [
                "1",
                [
                    1,
                    2
                ]
            ],
            [
                "2",
                [
                    1,
                    2,
                    3
                ]
            ]
        ]
    }
})");
    }

    SECTION("string-tuple-set-int")
    {
        auto dp = coek::DataPortal();
        std::map<std::tuple<std::string,std::string>,std::set<int>> data = {{{"0","1"},{1}}, {{"1","2"},{1,2}}, {{"2","3"},{1,2,3}}};
        REQUIRE(dp.put("A", data) == true);
        REQUIRE( dp.to_string(4) == R"({
    "A": {
        "key_type": [
            "s",
            "s"
        ],
        "set_type": [
            "i"
        ],
        "data": [
            [
                [
                    "0",
                    "1"
                ],
                [
                    1
                ]
            ],
            [
                [
                    "1",
                    "2"
                ],
                [
                    1,
                    2
                ]
            ],
            [
                [
                    "2",
                    "3"
                ],
                [
                    1,
                    2,
                    3
                ]
            ]
        ]
    }
})");
    }

    SECTION("int-tuple-set-int")
    {
        auto dp = coek::DataPortal();
        std::map<std::tuple<int,int>,std::set<int>> data = {{{0,1},{1}}, {{1,2},{1,2}}, {{2,3},{1,2,3}}};
        REQUIRE(dp.put("A", data) == true);
        REQUIRE( dp.to_string(4) == R"({
    "A": {
        "key_type": [
            "i",
            "i"
        ],
        "set_type": [
            "i"
        ],
        "data": [
            [
                [
                    0,
                    1
                ],
                [
                    1
                ]
            ],
            [
                [
                    1,
                    2
                ],
                [
                    1,
                    2
                ]
            ],
            [
                [
                    2,
                    3
                ],
                [
                    1,
                    2,
                    3
                ]
            ]
        ]
    }
})");
    }

    SECTION("mixed-tuple-set-int")
    {
        auto dp = coek::DataPortal();
        std::map<std::tuple<std::string,int>,std::set<int>> data = {{{"0",1},{1}}, {{"1",2},{1,2}}, {{"2",3},{1,2,3}}};
        REQUIRE(dp.put("A", data) == true);
        REQUIRE( dp.to_string(4) == R"({
    "A": {
        "key_type": [
            "s",
            "i"
        ],
        "set_type": [
            "i"
        ],
        "data": [
            [
                [
                    "0",
                    1
                ],
                [
                    1
                ]
            ],
            [
                [
                    "1",
                    2
                ],
                [
                    1,
                    2
                ]
            ],
            [
                [
                    "2",
                    3
                ],
                [
                    1,
                    2,
                    3
                ]
            ]
        ]
    }
})");
    }

#if 0
// TODO - indexed set of tuples
#endif
}

TEST_CASE("put_DP_indexed_param", "[smoke]")
{
    SECTION("int-int-param")
    {
        auto dp = coek::DataPortal();
        std::map<int,int> data = {{0,0}, {1,1}, {2,2}};
        REQUIRE(dp.put("A", data) == true);
        REQUIRE( dp.to_string(4) == R"({
    "A": {
        "key_type": [
            "i"
        ],
        "param_type": [
            "i"
        ],
        "data": [
            [
                0,
                0
            ],
            [
                1,
                1
            ],
            [
                2,
                2
            ]
        ]
    }
})");
    }

    SECTION("int-string-param")
    {
        auto dp = coek::DataPortal();
        std::map<int,std::string> data = {{0,"a"}, {1,"b"}, {2,"c"}};
        REQUIRE(dp.put("A", data) == true);
        REQUIRE( dp.to_string(4) == R"({
    "A": {
        "key_type": [
            "i"
        ],
        "param_type": [
            "s"
        ],
        "data": [
            [
                0,
                "a"
            ],
            [
                1,
                "b"
            ],
            [
                2,
                "c"
            ]
        ]
    }
})");
    }

    SECTION("string-string-param")
    {
        auto dp = coek::DataPortal();
        std::map<std::string,std::string> data = {{"0","a"}, {"1","b"}, {"2","c"}};
        REQUIRE(dp.put("A", data) == true);
        REQUIRE( dp.to_string(4) == R"({
    "A": {
        "key_type": [
            "s"
        ],
        "param_type": [
            "s"
        ],
        "data": [
            [
                "0",
                "a"
            ],
            [
                "1",
                "b"
            ],
            [
                "2",
                "c"
            ]
        ]
    }
})");
    }

    SECTION("string-int-param")
    {
        auto dp = coek::DataPortal();
        std::map<std::string,int> data = {{"0",1}, {"1",2}, {"2",3}};
        REQUIRE(dp.put("A", data) == true);
        REQUIRE( dp.to_string(4) == R"({
    "A": {
        "key_type": [
            "s"
        ],
        "param_type": [
            "i"
        ],
        "data": [
            [
                "0",
                1
            ],
            [
                "1",
                2
            ],
            [
                "2",
                3
            ]
        ]
    }
})");
    }

    SECTION("int-tuple-param")
    {
        auto dp = coek::DataPortal();
        std::map<int,std::tuple<double,std::string,int>> data = {{1,{0.1,"a",0}}, {2,{1.1,"b",1}}, {3,{2.1,"c",2}}};
        REQUIRE(dp.put("A", data) == true);
        REQUIRE( dp.to_string(4) == R"({
    "A": {
        "key_type": [
            "i"
        ],
        "param_type": [
            "d",
            "s",
            "i"
        ],
        "data": [
            [
                1,
                [
                    0.1,
                    "a",
                    0
                ]
            ],
            [
                2,
                [
                    1.1,
                    "b",
                    1
                ]
            ],
            [
                3,
                [
                    2.1,
                    "c",
                    2
                ]
            ]
        ]
    }
})");
    }

    SECTION("string-tuple-int-param")
    {
        auto dp = coek::DataPortal();
        std::map<std::tuple<std::string,std::string>,int> data = {{{"0","1"},1}, {{"1","2"},2}, {{"2","3"},3}};
        REQUIRE(dp.put("A", data) == true);
        REQUIRE( dp.to_string(4) == R"({
    "A": {
        "key_type": [
            "s",
            "s"
        ],
        "param_type": [
            "i"
        ],
        "data": [
            [
                [
                    "0",
                    "1"
                ],
                1
            ],
            [
                [
                    "1",
                    "2"
                ],
                2
            ],
            [
                [
                    "2",
                    "3"
                ],
                3
            ]
        ]
    }
})");
    }

    SECTION("int-tuple-int-param")
    {
        auto dp = coek::DataPortal();
        std::map<std::tuple<int,int>,int> data = {{{0,1},1}, {{1,2},2}, {{2,3},3}};
        REQUIRE(dp.put("A", data) == true);
        REQUIRE( dp.to_string(4) == R"({
    "A": {
        "key_type": [
            "i",
            "i"
        ],
        "param_type": [
            "i"
        ],
        "data": [
            [
                [
                    0,
                    1
                ],
                1
            ],
            [
                [
                    1,
                    2
                ],
                2
            ],
            [
                [
                    2,
                    3
                ],
                3
            ]
        ]
    }
})");
    }

    SECTION("mixed-tuple-int-param")
    {
        auto dp = coek::DataPortal();
        std::map<std::tuple<std::string,int>,int> data = {{{"0",1},1}, {{"1",2},2}, {{"2",3},3}};
        REQUIRE(dp.put("A", data) == true);
        REQUIRE( dp.to_string(4) == R"({
    "A": {
        "key_type": [
            "s",
            "i"
        ],
        "param_type": [
            "i"
        ],
        "data": [
            [
                [
                    "0",
                    1
                ],
                1
            ],
            [
                [
                    "1",
                    2
                ],
                2
            ],
            [
                [
                    "2",
                    3
                ],
                3
            ]
        ]
    }
})");
    }

    SECTION("mixed-tuple-double-param")
    {
        auto dp = coek::DataPortal();
        std::map<std::tuple<std::string,int>,double> data = {{{"0",1},1.1}, {{"1",2},2.1}, {{"2",3},3.1}};
        REQUIRE(dp.put("A", data) == true);
        REQUIRE( dp.to_string(4) == R"({
    "A": {
        "key_type": [
            "s",
            "i"
        ],
        "param_type": [
            "d"
        ],
        "data": [
            [
                [
                    "0",
                    1
                ],
                1.1
            ],
            [
                [
                    "1",
                    2
                ],
                2.1
            ],
            [
                [
                    "2",
                    3
                ],
                3.1
            ]
        ]
    }
})");
    }

    SECTION("mixed-tuple-tuple-param")
    {
        auto dp = coek::DataPortal();
        std::map<std::tuple<std::string,int>,std::tuple<double,std::string,int>> data = {{{"0",1},{1.1,"a",0}}, {{"1",2},{2.1,"b",1}}, {{"2",3},{3.1,"c",2}}};
        REQUIRE(dp.put("A", data) == true);
        REQUIRE( dp.to_string(4) == R"({
    "A": {
        "key_type": [
            "s",
            "i"
        ],
        "param_type": [
            "d",
            "s",
            "i"
        ],
        "data": [
            [
                [
                    "0",
                    1
                ],
                [
                    1.1,
                    "a",
                    0
                ]
            ],
            [
                [
                    "1",
                    2
                ],
                [
                    2.1,
                    "b",
                    1
                ]
            ],
            [
                [
                    "2",
                    3
                ],
                [
                    3.1,
                    "c",
                    2
                ]
            ]
        ]
    }
})");
    }
}

