#include <iostream>
#include <tuple>

#include "catch2/catch.hpp"
#include "coek/util/DataPortal.hpp"


TEST_CASE("DP_set", "[smoke]")
{
    SECTION("error1")
    {
        auto dp = coek::DataPortal();
        std::set<int> data;
        REQUIRE( dp.get("unknown", data) == false );
    }

    SECTION("error2")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string("{ \"A\": [1,2,3] }");
        std::set<std::string> data;
        REQUIRE( dp.get("unknown", data) == false );
        REQUIRE( dp.get("A", data) == false );
    }

    SECTION("int-set")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string("{ \"A\": [1,2,3] }");
        std::set<int> data;
        REQUIRE( dp.get("unknown", data) == false );
        REQUIRE( dp.get("A", data) == true );
        std::set<int> baseline = {1,2,3};
        REQUIRE( data == baseline );
    }

    SECTION("string-set")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string("{ \"A\": [\"a\",\"b\",\"c\"] }");
        std::set<std::string> data;
        REQUIRE( dp.get("unknown", data) == false );
        REQUIRE( dp.get("A", data) == true );
    }

    SECTION("int-tuple-set")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string("{ \"A\": [ [0, 0], [1,1], [2,2] ] }");
        std::set<std::tuple<int,int>> data;
        REQUIRE( dp.get("unknown", data) == false );
        REQUIRE( dp.get("A", data) == true );
    }

    SECTION("string-tuple-set")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string("{ \"A\": [ [\"a\", \"a\"], [\"b\",\"b\"], [\"c\",\"c\"] ] }");
        std::set<std::tuple<std::string,std::string>> data;
        REQUIRE( dp.get("unknown", data) == false );
        REQUIRE( dp.get("A", data) == true );
    }

    SECTION("mixed-tuple-set")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string("{ \"A\": [ [\"a\", 0], [\"b\",1], [\"c\",2] ] }");
        std::set<std::tuple<std::string,int>> data;
        REQUIRE( dp.get("unknown", data) == false );
        REQUIRE( dp.get("A", data) == true );
    }
}


TEST_CASE("DP_param", "[smoke]")
{
    SECTION("error1")
    {
        auto dp = coek::DataPortal();
        int data;
        REQUIRE( dp.get("unknown", data) == false );
    }

    SECTION("error2")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string("{ \"A\": 1 }");
        std::string data;
        REQUIRE( dp.get("unknown", data) == false );
        REQUIRE( dp.get("A", data) == false );
    }

    SECTION("int-param")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string("{ \"A\": 1 }");
        int data = 0;
        REQUIRE( dp.get("unknown", data) == false );
        REQUIRE( dp.get("A", data) == true );
        REQUIRE( data == 1 );
    }

    SECTION("string-param")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string("{ \"A\": \"a\" }");
        std::string data = "";
        REQUIRE( dp.get("unknown", data) == false );
        REQUIRE( dp.get("A", data) == true );
        REQUIRE( data == "a" );
    }
}


TEST_CASE("DP_indexed_set", "[smoke]")
{
    SECTION("error1")
    {
        auto dp = coek::DataPortal();
        std::map<int,std::set<int>> data;
        REQUIRE( dp.get("unknown", data) == false );
    }

    SECTION("error2")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string("{ \"A\": 1 }");
        std::map<int,std::set<int>> data;
        REQUIRE( dp.get("unknown", data) == false );
        REQUIRE( dp.get("A", data) == false );
    }

    SECTION("int-set-int")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string(R"(
{
"A": {
    "key_types": "i",
    "value_types": "set_i",
    "data": [
            {
                "k": 0,
                "v": [1,1]
            },
            {
                "k": 1,
                "v": [2,2]
            },
            {
                "k": 2,
                "v": [3,3]
            }
            ]
    }
}
)");

        std::map<int,std::set<int>> data;
        REQUIRE( dp.get("unknown", data) == false );
        REQUIRE( dp.get("A", data) == true );
        REQUIRE( data.size() == 3 );
        std::set<int> baseline = {3,3};
        REQUIRE( data[2] == baseline );
    }

    SECTION("int-set-string")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string(R"(
{
"A": {
    "key_types": "i",
    "value_types": "set_s",
    "data": [
            {
                "k": 0,
                "v": ["a","a"]
            },
            {
                "k": 1,
                "v": ["b","b"]
            },
            {
                "k": 2,
                "v": ["c","c"]
            }
            ]
    }
}
)");

        std::map<int,std::set<std::string>> data;
        REQUIRE( dp.get("unknown", data) == false );
        REQUIRE( dp.get("A", data) == true );
        REQUIRE( data.size() == 3 );
        std::set<std::string> baseline = {"c","c"};
        REQUIRE( data[2] == baseline );
    }

    SECTION("string-set-string")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string(R"(
{
"A": {
    "key_types": "s",
    "value_types": "set_s",
    "data": [
            {
                "k": "0",
                "v": ["a","a"]
            },
            {
                "k": "1",
                "v": ["b","b"]
            },
            {
                "k": "2",
                "v": ["c","c"]
            }
            ]
    }
}
)");

        std::map<std::string,std::set<std::string>> data;
        REQUIRE( dp.get("unknown", data) == false );
        REQUIRE( dp.get("A", data) == true );
        REQUIRE( data.size() == 3 );
        std::set<std::string> baseline = {"c","c"};
        REQUIRE( data["2"] == baseline );
    }

    SECTION("string-set-int")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string(R"(
{
"A": {
    "key_types": "i",
    "value_types": "set_i",
    "data": [
            {
                "k": "0",
                "v": [0,0]
            },
            {
                "k": "1",
                "v": [1,1]
            },
            {
                "k": "2",
                "v": [2,2]
            }
            ]
    }
}
)");

        std::map<std::string,std::set<int>> data;
        REQUIRE( dp.get("unknown", data) == false );
        REQUIRE( dp.get("A", data) == true );
        REQUIRE( data.size() == 3 );
        std::set<int> baseline = {2,2};
        REQUIRE( data["2"] == baseline );
    }

    SECTION("string-tuple-set-int")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string(R"(
{
"A": {
    "key_types": "i",
    "value_types": "s",
    "data": [
            {
                "k": ["0","1"],
                "v": [0,1]
            },
            {
                "k": ["1","2"],
                "v": [1,2]
            },
            {
                "k": ["2","3"],
                "v": [2,3]
            }
            ]
    }
}
)");

        std::map<std::tuple<std::string,std::string>,std::set<int>> data;
        REQUIRE( dp.get("unknown", data) == false );
        REQUIRE( dp.get("A", data) == true );
        REQUIRE( data.size() == 3 );
        std::set<int> baseline{2,3};
        REQUIRE( data[std::make_tuple("2","3")] == baseline );
    }

    SECTION("int-tuple-set-int")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string(R"(
{
"A": {
    "key_types": "i",
    "value_types": "s",
    "data": [
            {
                "k": [0,1],
                "v": [0,1]
            },
            {
                "k": [1,2],
                "v": [1,2]
            },
            {
                "k": [2,3],
                "v": [2,3]
            }
            ]
    }
}
)");

        std::map<std::tuple<int,int>,std::set<int>> data;
        REQUIRE( dp.get("unknown", data) == false );
        REQUIRE( dp.get("A", data) == true );
        REQUIRE( data.size() == 3 );
        std::set<int> baseline{2,3};
        REQUIRE( data[std::make_tuple(2,3)] == baseline );
    }

    SECTION("mixed-tuple-set-int")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string(R"(
{
"A": {
    "key_types": "i",
    "value_types": "s",
    "data": [
            {
                "k": ["0",1],
                "v": [0,1]
            },
            {
                "k": ["1",2],
                "v": [1,2]
            },
            {
                "k": ["2",3],
                "v": [2,3]
            }
            ]
    }
}
)");

        std::map<std::tuple<std::string,int>,std::set<int>> data;
        REQUIRE( dp.get("unknown", data) == false );
        REQUIRE( dp.get("A", data) == true );
        REQUIRE( data.size() == 3 );
        std::set<int> baseline{2,3};
        REQUIRE( data[std::make_tuple("2",3)] == baseline );
    }
}



TEST_CASE("DP_indexed_param", "[smoke]")
{
    SECTION("error1")
    {
        auto dp = coek::DataPortal();
        std::map<int,int> data;
        REQUIRE( dp.get("unknown", data) == false );
    }

    SECTION("error2")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string("{ \"A\": 1 }");
        std::map<int,int> data;
        REQUIRE( dp.get("unknown", data) == false );
        REQUIRE( dp.get("A", data) == false );
    }

    SECTION("int-int-param")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string(R"(
{
"A": {
    "key_types": "i",
    "value_types": "i",
    "data": [
            {
                "k": 0,
                "v": 1
            },
            {
                "k": 1,
                "v": 2
            },
            {
                "k": 2,
                "v": 3
            }
            ]
    }
}
)");

        std::map<int,int> data;
        REQUIRE( dp.get("unknown", data) == false );
        REQUIRE( dp.get("A", data) == true );
        REQUIRE( data.size() == 3 );
        REQUIRE( data[2] == 3 );
    }

    SECTION("int-string-param")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string(R"(
{
"A": {
    "key_types": "i",
    "value_types": "s",
    "data": [
            {
                "k": 0,
                "v": "a"
            },
            {
                "k": 1,
                "v": "b"
            },
            {
                "k": 2,
                "v": "c"
            }
            ]
    }
}
)");

        std::map<int,std::string> data;
        REQUIRE( dp.get("unknown", data) == false );
        REQUIRE( dp.get("A", data) == true );
        REQUIRE( data.size() == 3 );
        REQUIRE( data[2] == "c" );
    }

    SECTION("string-string-param")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string(R"(
{
"A": {
    "key_types": "i",
    "value_types": "s",
    "data": [
            {
                "k": "0",
                "v": "a"
            },
            {
                "k": "1",
                "v": "b"
            },
            {
                "k": "2",
                "v": "c"
            }
            ]
    }
}
)");

        std::map<std::string,std::string> data;
        REQUIRE( dp.get("unknown", data) == false );
        REQUIRE( dp.get("A", data) == true );
        REQUIRE( data.size() == 3 );
        REQUIRE( data["2"] == "c" );
    }

    SECTION("string-int-param")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string(R"(
{
"A": {
    "key_types": "i",
    "value_types": "s",
    "data": [
            {
                "k": "0",
                "v": 0 
            },
            {
                "k": "1",
                "v": 1 
            },
            {
                "k": "2",
                "v": 2
            }
            ]
    }
}
)");

        std::map<std::string,int> data;
        REQUIRE( dp.get("unknown", data) == false );
        REQUIRE( dp.get("A", data) == true );
        REQUIRE( data.size() == 3 );
        REQUIRE( data["2"] == 2 );
    }

    SECTION("string-tuple-int-param")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string(R"(
{
"A": {
    "key_types": "i",
    "value_types": "s",
    "data": [
            {
                "k": ["0","1"],
                "v": 0 
            },
            {
                "k": ["1","2"],
                "v": 1 
            },
            {
                "k": ["2","3"],
                "v": 2
            }
            ]
    }
}
)");

        std::map<std::tuple<std::string,std::string>,int> data;
        REQUIRE( dp.get("unknown", data) == false );
        REQUIRE( dp.get("A", data) == true );
        REQUIRE( data.size() == 3 );
        REQUIRE( data[std::make_tuple("2","3")] == 2 );
    }

    SECTION("int-tuple-int-param")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string(R"(
{
"A": {
    "key_types": "i",
    "value_types": "s",
    "data": [
            {
                "k": [0,1],
                "v": 0 
            },
            {
                "k": [1,2],
                "v": 1 
            },
            {
                "k": [2,3],
                "v": 2
            }
            ]
    }
}
)");

        std::map<std::tuple<int,int>,int> data;
        REQUIRE( dp.get("unknown", data) == false );
        REQUIRE( dp.get("A", data) == true );
        REQUIRE( data.size() == 3 );
        REQUIRE( data[std::make_tuple(2,3)] == 2 );
    }

    SECTION("mixed-tuple-int-param")
    {
        auto dp = coek::DataPortal();
        dp.load_from_json_string(R"(
{
"A": {
    "key_types": "i",
    "value_types": "s",
    "data": [
            {
                "k": ["0",1],
                "v": 0 
            },
            {
                "k": ["1",2],
                "v": 1 
            },
            {
                "k": ["2",3],
                "v": 2
            }
            ]
    }
}
)");

        std::map<std::tuple<std::string,int>,int> data;
        REQUIRE( dp.get("unknown", data) == false );
        REQUIRE( dp.get("A", data) == true );
        REQUIRE( data.size() == 3 );
        REQUIRE( data[std::make_tuple("2",3)] == 2 );
    }
}

