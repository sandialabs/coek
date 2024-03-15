
#include <cmath>
#include <iostream>
#include <memory>
#include <sstream>

#include "catch2/catch.hpp"
#include "coek/ast/base_terms.hpp"
#include "coek/ast/value_terms.hpp"
#include "coek/coek.hpp"

const double PI = 3.141592653589793238463;
const double E = exp(1.0);

TEST_CASE("elementary_objective", "[smoke]")
{
    SECTION("constructors")
    {
        WHEN("repn")
        {
            auto v = coek::variable("v");
            auto o = coek::objective("test", 2 * v);
            coek::Objective O(o.repn);
            REQUIRE(O.name() == "test");
            REQUIRE(o.expr().to_list() == O.expr().to_list());
            REQUIRE(o.id() == O.id());
        }

        WHEN("copy")
        {
            auto v = coek::variable("v");
            auto o = coek::objective("test", 2 * v);
            coek::Objective O(o);
            REQUIRE(O.name() == "test");
            REQUIRE(o.expr().to_list() == O.expr().to_list());
            REQUIRE(o.id() == O.id());
        }

        WHEN("equal")
        {
            auto v = coek::variable("v");
            auto o = coek::objective("test", 2 * v);
            auto O = coek::objective("old");
            O = o;
            REQUIRE(O.name() == "test");
            REQUIRE(o.expr().to_list() == O.expr().to_list());
            REQUIRE(o.id() == O.id());
        }
    }

    SECTION("id")
    {
        auto v = coek::variable("v");
        auto o = coek::objective(2 * v);
        REQUIRE(o.id() > 0);
    }

    SECTION("name")
    {
        auto v = coek::variable("v");
        auto o = coek::objective(2 * v);
        o.name("foo");

        REQUIRE(o.name() == "foo");
    }

    SECTION("body")
    {
        auto v = coek::variable("v");
        auto o = coek::objective().expr(2 * v);

        static std::list<std::string> baseline1 = {"[", "*", std::to_string(2.0), "v", "]"};
        REQUIRE(o.expr().to_list() == baseline1);

        o.expr(3 * v);
        static std::list<std::string> baseline2 = {"[", "*", std::to_string(3.0), "v", "]"};
        REQUIRE(o.expr().to_list() == baseline2);
    }

    SECTION("sense")
    {
        auto v = coek::variable("v");
        auto o = coek::objective(2 * v);

        REQUIRE(o.sense() == coek::Model::minimize);

        o.sense(coek::Model::maximize);
        REQUIRE(o.sense() == coek::Model::maximize);
    }

    SECTION("value")
    {
        auto v = coek::variable("v").value(1);
        auto o = coek::objective("test", 2 * v + 1);
        REQUIRE(o.value() == 3);
    }
}
