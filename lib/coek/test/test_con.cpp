
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

TEST_CASE("elementary_constraint", "[smoke]")
{
    auto v = coek::variable("v").lower(0).upper(1).value(1);
    auto p = coek::parameter("p");
    auto P = coek::set_element("P");
    P.value(0);
    coek::Expression f;

    SECTION("constraint function")
    {
        WHEN("empty")
        {
            auto e = coek::constraint();
            static std::list<std::string> baseline = {};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("named")
        {
            auto e = coek::constraint("foo");
            static std::list<std::string> baseline = {};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.name() == "foo");
        }

        WHEN("simple")
        {
            auto v = coek::variable("v");
            auto e = coek::constraint(1 < v);
            static std::list<std::string> baseline
                = {"[", "<", std::to_string(1.0), "v", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("simple named")
        {
            auto v = coek::variable("v");
            auto e = coek::constraint("c", 1 < v);
            static std::list<std::string> baseline
                = {"[", "<", std::to_string(1.0), "v", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.name() == "c");
        }
    }

    SECTION("methods")
    {
        WHEN("name")
        {
            auto v = coek::variable("v");
            auto e = 1 < v;
            e.name("c");
            static std::list<std::string> baseline
                = {"[", "<", std::to_string(1.0), "v", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.name() == "c");
        }

        WHEN("lower")
        {
            auto v = coek::variable("v");
            auto e = 1 < v;
            REQUIRE(e.lower().value() == 1.0);
        }

        WHEN("has_lower")
        {
            auto v = coek::variable("v");
            auto e = 1 < v;
            REQUIRE(e.has_lower());
            REQUIRE(not e.has_upper());
        }

        WHEN("upper")
        {
            auto v = coek::variable("v");
            auto e = 1 > v;
            REQUIRE(e.upper().value() == 1.0);
        }

        WHEN("has_upper")
        {
            auto v = coek::variable("v");
            auto e = 1 > v;
            REQUIRE(e.upper().value() == 1.0);
            REQUIRE(not e.has_lower());
            REQUIRE(e.has_upper());
        }

        WHEN("body")
        {
            auto v = coek::variable("v").value(2.0);
            auto e = 1 > v;
            REQUIRE(e.body().value() == 2.0);
        }
    }

    SECTION("lt-operator")
    {
        WHEN("1 < p")
        {
            auto e = 1 < p;
            static std::list<std::string> baseline
                = {"[", "<", std::to_string(1.0), "p", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.is_feasible() == false);
        }

        WHEN("p < 1")
        {
            auto e = p < 1;
            static std::list<std::string> baseline
                = {"[", "<", "-Inf", "p", std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.is_feasible() == true);
        }

        WHEN("1.0 < p")
        {
            auto e = 1.0 < p;
            static std::list<std::string> baseline
                = {"[", "<", std::to_string(1.0), "p", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.is_feasible() == false);
        }

        WHEN("p < 1.0")
        {
            auto e = p < 1.0;
            static std::list<std::string> baseline
                = {"[", "<", "-Inf", "p", std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.is_feasible() == true);
        }

        WHEN("1 < P")
        {
            auto e = 1 < P;
            static std::list<std::string> baseline
                = {"[", "<", std::to_string(1.0), "P", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.is_feasible() == false);
        }

        WHEN("P < 1")
        {
            auto e = P < 1;
            static std::list<std::string> baseline
                = {"[", "<", "-Inf", "P", std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.is_feasible() == true);
        }

        WHEN("1.0 < P")
        {
            auto e = 1.0 < P;
            static std::list<std::string> baseline
                = {"[", "<", std::to_string(1.0), "P", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.is_feasible() == false);
        }

        WHEN("P < 1.0")
        {
            auto e = P < 1.0;
            static std::list<std::string> baseline
                = {"[", "<", "-Inf", "P", std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.is_feasible() == true);
        }

        WHEN("1 < v")
        {
            auto e = 1 < v;
            static std::list<std::string> baseline
                = {"[", "<", std::to_string(1.0), "v", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.is_feasible() == false);
        }

        WHEN("v < 1")
        {
            auto e = v < 1;
            static std::list<std::string> baseline
                = {"[", "<", "-Inf", "v", std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.is_feasible() == false);
        }

        WHEN("1.0 < v")
        {
            auto e = 1.0 < v;
            static std::list<std::string> baseline
                = {"[", "<", std::to_string(1.0), "v", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.is_feasible() == false);
        }

        WHEN("v < 1.0")
        {
            auto e = v < 1.0;
            static std::list<std::string> baseline
                = {"[", "<", "-Inf", "v", std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.is_feasible() == false);
        }

        WHEN("1 < f")
        {
            auto e = 1 < f;
            static std::list<std::string> baseline
                = {"[", "<", std::to_string(1.0), std::to_string(0.0), "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("f < 1")
        {
            auto e = f < 1;
            static std::list<std::string> baseline
                = {"[", "<", "-Inf", std::to_string(0.0), std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("1.0 < f")
        {
            auto e = 1.0 < f;
            static std::list<std::string> baseline
                = {"[", "<", std::to_string(1.0), std::to_string(0.0), "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("f < 1.0")
        {
            auto e = f < 1.0;
            static std::list<std::string> baseline
                = {"[", "<", "-Inf", std::to_string(0.0), std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        // TODO - Error because no variables?
        WHEN("p < p")
        {
            auto e = p < p;
            static std::list<std::string> baseline = {"[", "<", "p", "p", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("p < P")
        {
            auto e = p < P;
            static std::list<std::string> baseline = {"[", "<", "p", "P", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("P < p")
        {
            auto e = P < p;
            static std::list<std::string> baseline = {"[", "<", "P", "p", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("p < v")
        {
            auto e = p < v;
            static std::list<std::string> baseline = {"[", "<", "p", "v", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("v < p")
        {
            auto e = v < p;
            static std::list<std::string> baseline = {"[", "<", "-Inf", "v", "p", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("p < f")
        {
            auto e = p < f;
            static std::list<std::string> baseline
                = {"[", "<", "p", std::to_string(0.0), "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("f < p")
        {
            auto e = f < p;
            static std::list<std::string> baseline
                = {"[", "<", std::to_string(0.0), "p", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        // TODO - Error because no variables?
        WHEN("P < P")
        {
            auto e = P < P;
            static std::list<std::string> baseline = {"[", "<", "P", "P", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("P < v")
        {
            auto e = P < v;
            static std::list<std::string> baseline = {"[", "<", "P", "v", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("v < P")
        {
            auto e = v < P;
            static std::list<std::string> baseline = {"[", "<", "-Inf", "v", "P", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("P < f")
        {
            auto e = P < f;
            static std::list<std::string> baseline
                = {"[", "<", "P", std::to_string(0.0), "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("f < P")
        {
            auto e = f < P;
            static std::list<std::string> baseline
                = {"[", "<", std::to_string(0.0), "P", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("f < v")
        {
            auto e = f < v;
            static std::list<std::string> baseline
                = {"[", "<", std::to_string(0.0), "v", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("v < f")
        {
            auto e = v < f;
            static std::list<std::string> baseline
                = {"[", "<", "-Inf", "v", std::to_string(0.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("v < v")
        {
            auto e = v < v;
            static std::list<std::string> baseline = {
                "[", "<", "-Inf", "[", "+", "v", "[", "*", "-1", "v", "]", "]", std::to_string(0.0),
                "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("f < f")
        {
            auto e = f < f;
            static std::list<std::string> baseline
                = {"[", "<", std::to_string(0.0), std::to_string(0.0), "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }
    }

    SECTION("le-operator")
    {
        WHEN("0 <= p")
        {
            auto e = 0 <= p;
            static std::list<std::string> baseline
                = {"[", "<=", std::to_string(0.0), "p", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.is_feasible() == true);
        }

        WHEN("p <= 0")
        {
            auto e = p <= 0;
            static std::list<std::string> baseline
                = {"[", "<=", "-Inf", "p", std::to_string(0.0), "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.is_feasible() == true);
        }

        WHEN("0.0 <= p")
        {
            auto e = 0.0 <= p;
            static std::list<std::string> baseline
                = {"[", "<=", std::to_string(0.0), "p", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.is_feasible() == true);
        }

        WHEN("p <= 0.0")
        {
            auto e = p <= 0.0;
            static std::list<std::string> baseline
                = {"[", "<=", "-Inf", "p", std::to_string(0.0), "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.is_feasible() == true);
        }

        WHEN("1 <= p")
        {
            auto e = 1 <= p;
            static std::list<std::string> baseline
                = {"[", "<=", std::to_string(1.0), "p", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.is_feasible() == false);
        }

        WHEN("p <= 1")
        {
            auto e = p <= 1;
            static std::list<std::string> baseline
                = {"[", "<=", "-Inf", "p", std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.is_feasible() == true);
        }

        WHEN("1.0 <= p")
        {
            auto e = 1.0 <= p;
            static std::list<std::string> baseline
                = {"[", "<=", std::to_string(1.0), "p", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.is_feasible() == false);
        }

        WHEN("p <= 1.0")
        {
            auto e = p <= 1.0;
            static std::list<std::string> baseline
                = {"[", "<=", "-Inf", "p", std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.is_feasible() == true);
        }

        WHEN("1 <= P")
        {
            auto e = 1 <= P;
            static std::list<std::string> baseline
                = {"[", "<=", std::to_string(1.0), "P", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.is_feasible() == false);
        }

        WHEN("P <= 1")
        {
            auto e = P <= 1;
            static std::list<std::string> baseline
                = {"[", "<=", "-Inf", "P", std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.is_feasible() == true);
        }

        WHEN("1.0 <= P")
        {
            auto e = 1.0 <= P;
            static std::list<std::string> baseline
                = {"[", "<=", std::to_string(1.0), "P", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.is_feasible() == false);
        }

        WHEN("P <= 1.0")
        {
            auto e = P <= 1.0;
            static std::list<std::string> baseline
                = {"[", "<=", "-Inf", "P", std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.is_feasible() == true);
        }

        WHEN("1 <= v")
        {
            auto e = 1 <= v;
            static std::list<std::string> baseline
                = {"[", "<=", std::to_string(1.0), "v", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.is_feasible() == true);
        }

        WHEN("v <= 1")
        {
            auto e = v <= 1;
            static std::list<std::string> baseline
                = {"[", "<=", "-Inf", "v", std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.is_feasible() == true);
        }

        WHEN("1.0 <= v")
        {
            auto e = 1.0 <= v;
            static std::list<std::string> baseline
                = {"[", "<=", std::to_string(1.0), "v", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.is_feasible() == true);
        }

        WHEN("v <= 1.0")
        {
            auto e = v <= 1.0;
            static std::list<std::string> baseline
                = {"[", "<=", "-Inf", "v", std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.is_feasible() == true);
        }

        WHEN("1 <= f")
        {
            auto e = 1 <= f;
            static std::list<std::string> baseline
                = {"[", "<=", std::to_string(1.0), std::to_string(0.0), "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("f <= 1")
        {
            auto e = f <= 1;
            static std::list<std::string> baseline
                = {"[", "<=", "-Inf", std::to_string(0.0), std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("1.0 <= f")
        {
            auto e = 1.0 <= f;
            static std::list<std::string> baseline
                = {"[", "<=", std::to_string(1.0), std::to_string(0.0), "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("f <= 1.0")
        {
            auto e = f <= 1.0;
            static std::list<std::string> baseline
                = {"[", "<=", "-Inf", std::to_string(0.0), std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("p <= p")
        {
            auto e = p <= p;
            static std::list<std::string> baseline = {"[", "<=", "p", "p", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("p <= P")
        {
            auto e = p <= P;
            static std::list<std::string> baseline = {"[", "<=", "p", "P", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("P <= p")
        {
            auto e = P <= p;
            static std::list<std::string> baseline = {"[", "<=", "P", "p", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("p <= v")
        {
            auto e = p <= v;
            static std::list<std::string> baseline = {"[", "<=", "p", "v", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("v <= p")
        {
            auto e = v <= p;
            static std::list<std::string> baseline = {"[", "<=", "-Inf", "v", "p", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("p <= f")
        {
            auto e = p <= f;
            static std::list<std::string> baseline
                = {"[", "<=", "p", std::to_string(0.0), "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("f <= p")
        {
            auto e = f <= p;
            static std::list<std::string> baseline
                = {"[", "<=", std::to_string(0.0), "p", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("P <= P")
        {
            auto e = P <= P;
            static std::list<std::string> baseline = {"[", "<=", "P", "P", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("P <= v")
        {
            auto e = P <= v;
            static std::list<std::string> baseline = {"[", "<=", "P", "v", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("v <= P")
        {
            auto e = v <= P;
            static std::list<std::string> baseline = {"[", "<=", "-Inf", "v", "P", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("P <= f")
        {
            auto e = P <= f;
            static std::list<std::string> baseline
                = {"[", "<=", "P", std::to_string(0.0), "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("f <= P")
        {
            auto e = f <= P;
            static std::list<std::string> baseline
                = {"[", "<=", std::to_string(0.0), "P", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("f <= v")
        {
            auto e = f <= v;
            static std::list<std::string> baseline
                = {"[", "<=", std::to_string(0.0), "v", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("v <= f")
        {
            auto e = v <= f;
            static std::list<std::string> baseline
                = {"[", "<=", "-Inf", "v", std::to_string(0.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("v <= v")
        {
            auto e = v <= v;
            static std::list<std::string> baseline = {"[",
                                                      "<=",
                                                      "-Inf",
                                                      "[",
                                                      "+",
                                                      "v",
                                                      "[",
                                                      "*",
                                                      "-1",
                                                      "v",
                                                      "]",
                                                      "]",
                                                      std::to_string(0.0),
                                                      "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("f <= f")
        {
            auto e = f <= f;
            static std::list<std::string> baseline
                = {"[", "<=", std::to_string(0.0), std::to_string(0.0), "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }
    }

    SECTION("eq-operator")
    {
        WHEN("0 == p")
        {
            auto e = 0 == p;
            static std::list<std::string> baseline = {"[", "==", "p", std::to_string(0.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("p == 0")
        {
            auto e = p == 0;
            static std::list<std::string> baseline = {"[", "==", "p", std::to_string(0.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("0.0 == p")
        {
            auto e = 0.0 == p;
            static std::list<std::string> baseline = {"[", "==", "p", std::to_string(0.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("p == 0.0")
        {
            auto e = p == 0.0;
            static std::list<std::string> baseline = {"[", "==", "p", std::to_string(0.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("1 == p")
        {
            auto e = 1 == p;
            static std::list<std::string> baseline = {"[", "==", "p", std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("p == 1")
        {
            auto e = p == 1;
            static std::list<std::string> baseline = {"[", "==", "p", std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("1.0 == p")
        {
            auto e = 1.0 == p;
            static std::list<std::string> baseline = {"[", "==", "p", std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("p == 1.0")
        {
            auto e = p == 1.0;
            static std::list<std::string> baseline = {"[", "==", "p", std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("1 == P")
        {
            auto e = 1 == P;
            static std::list<std::string> baseline = {"[", "==", "P", std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("P == 1")
        {
            auto e = P == 1;
            static std::list<std::string> baseline = {"[", "==", "P", std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("1.0 == P")
        {
            auto e = 1.0 == P;
            static std::list<std::string> baseline = {"[", "==", "P", std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("P == 1.0")
        {
            auto e = P == 1.0;
            static std::list<std::string> baseline = {"[", "==", "P", std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("1 == v")
        {
            auto e = 1 == v;
            static std::list<std::string> baseline = {"[", "==", "v", std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("v == 1")
        {
            auto e = v == 1;
            static std::list<std::string> baseline = {"[", "==", "v", std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("1.0 == v")
        {
            auto e = 1.0 == v;
            static std::list<std::string> baseline = {"[", "==", "v", std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("v == 1.0")
        {
            auto e = v == 1.0;
            static std::list<std::string> baseline = {"[", "==", "v", std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("1 == f")
        {
            auto e = 1 == f;
            static std::list<std::string> baseline
                = {"[", "==", std::to_string(0.0), std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("f == 1")
        {
            auto e = f == 1;
            static std::list<std::string> baseline
                = {"[", "==", std::to_string(0.0), std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("1.0 == f")
        {
            auto e = 1.0 == f;
            static std::list<std::string> baseline
                = {"[", "==", std::to_string(0.0), std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("f == 1.0")
        {
            auto e = f == 1.0;
            static std::list<std::string> baseline
                = {"[", "==", std::to_string(0.0), std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("p == p")
        {
            auto e = p == p;
            static std::list<std::string> baseline = {"[", "==", "p", "p", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("p == P")
        {
            auto e = p == P;
            static std::list<std::string> baseline = {"[", "==", "P", "p", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("P == p")
        {
            auto e = P == p;
            static std::list<std::string> baseline = {"[", "==", "p", "P", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("p == v")
        {
            auto e = p == v;
            static std::list<std::string> baseline = {"[", "==", "v", "p", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("v == p")
        {
            auto e = v == p;
            static std::list<std::string> baseline = {"[", "==", "v", "p", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("p == f")
        {
            auto e = p == f;
            static std::list<std::string> baseline = {"[", "==", std::to_string(0.0), "p", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("f == p")
        {
            auto e = f == p;
            static std::list<std::string> baseline = {"[", "==", "p", std::to_string(0.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("P == P")
        {
            auto e = P == P;
            static std::list<std::string> baseline = {"[", "==", "P", "P", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("P == v")
        {
            auto e = P == v;
            static std::list<std::string> baseline = {"[", "==", "v", "P", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("v == P")
        {
            auto e = v == P;
            static std::list<std::string> baseline = {"[", "==", "v", "P", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("P == f")
        {
            auto e = P == f;
            static std::list<std::string> baseline = {"[", "==", std::to_string(0.0), "P", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("f == P")
        {
            auto e = f == P;
            static std::list<std::string> baseline = {"[", "==", "P", std::to_string(0.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("f == v")
        {
            auto e = f == v;
            static std::list<std::string> baseline = {"[", "==", "v", std::to_string(0.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("v == f")
        {
            auto e = v == f;
            static std::list<std::string> baseline = {"[", "==", "v", std::to_string(0.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("v == v")
        {
            auto e = v == v;
            static std::list<std::string> baseline = {
                "[", "==", "[", "+", "v", "[", "*", "-1", "v", "]", "]", std::to_string(0.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("f == f")
        {
            auto e = f == f;
            static std::list<std::string> baseline
                = {"[", "==", std::to_string(0.0), std::to_string(0.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }
    }

    SECTION("gt-operator")
    {
        WHEN("1 > p")
        {
            auto e = 1 > p;
            static std::list<std::string> baseline
                = {"[", "<", "-Inf", "p", std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("p > 1")
        {
            auto e = p > 1;
            static std::list<std::string> baseline
                = {"[", "<", std::to_string(1.0), "p", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("1.0 > p")
        {
            auto e = 1.0 > p;
            static std::list<std::string> baseline
                = {"[", "<", "-Inf", "p", std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("p > 1.0")
        {
            auto e = p > 1.0;
            static std::list<std::string> baseline
                = {"[", "<", std::to_string(1.0), "p", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("1 > P")
        {
            auto e = 1 > P;
            static std::list<std::string> baseline
                = {"[", "<", "-Inf", "P", std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("P > 1")
        {
            auto e = P > 1;
            static std::list<std::string> baseline
                = {"[", "<", std::to_string(1.0), "P", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("1.0 > P")
        {
            auto e = 1.0 > P;
            static std::list<std::string> baseline
                = {"[", "<", "-Inf", "P", std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("P > 1.0")
        {
            auto e = P > 1.0;
            static std::list<std::string> baseline
                = {"[", "<", std::to_string(1.0), "P", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("1 > v")
        {
            auto e = 1 > v;
            static std::list<std::string> baseline
                = {"[", "<", "-Inf", "v", std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("v > 1")
        {
            auto e = v > 1;
            static std::list<std::string> baseline
                = {"[", "<", std::to_string(1.0), "v", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("1.0 > v")
        {
            auto e = 1.0 > v;
            static std::list<std::string> baseline
                = {"[", "<", "-Inf", "v", std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("v > 1.0")
        {
            auto e = v > 1.0;
            static std::list<std::string> baseline
                = {"[", "<", std::to_string(1.0), "v", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("1 > f")
        {
            auto e = 1 > f;
            static std::list<std::string> baseline
                = {"[", "<", "-Inf", std::to_string(0.0), std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("f > 1")
        {
            auto e = f > 1;
            static std::list<std::string> baseline
                = {"[", "<", std::to_string(1.0), std::to_string(0.0), "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("1.0 > f")
        {
            auto e = 1.0 > f;
            static std::list<std::string> baseline
                = {"[", "<", "-Inf", std::to_string(0.0), std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("f > 1.0")
        {
            auto e = f > 1.0;
            static std::list<std::string> baseline
                = {"[", "<", std::to_string(1.0), std::to_string(0.0), "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("p > p")
        {
            auto e = p > p;
            static std::list<std::string> baseline = {"[", "<", "p", "p", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("p > P")
        {
            auto e = p > P;
            static std::list<std::string> baseline = {"[", "<", "P", "p", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("P > p")
        {
            auto e = P > p;
            static std::list<std::string> baseline = {"[", "<", "p", "P", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("p > v")
        {
            auto e = p > v;
            static std::list<std::string> baseline = {"[", "<", "-Inf", "v", "p", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("v > p")
        {
            auto e = v > p;
            static std::list<std::string> baseline = {"[", "<", "p", "v", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("p > f")
        {
            auto e = p > f;
            static std::list<std::string> baseline
                = {"[", "<", std::to_string(0.0), "p", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("f > p")
        {
            auto e = f > p;
            static std::list<std::string> baseline
                = {"[", "<", "p", std::to_string(0.0), "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("P > P")
        {
            auto e = P > P;
            static std::list<std::string> baseline = {"[", "<", "P", "P", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("P > v")
        {
            auto e = P > v;
            static std::list<std::string> baseline = {"[", "<", "-Inf", "v", "P", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("v > P")
        {
            auto e = v > P;
            static std::list<std::string> baseline = {"[", "<", "P", "v", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("P > f")
        {
            auto e = P > f;
            static std::list<std::string> baseline
                = {"[", "<", std::to_string(0.0), "P", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("f > P")
        {
            auto e = f > P;
            static std::list<std::string> baseline
                = {"[", "<", "P", std::to_string(0.0), "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("f > v")
        {
            auto e = f > v;
            static std::list<std::string> baseline
                = {"[", "<", "-Inf", "v", std::to_string(0.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("v > f")
        {
            auto e = v > f;
            static std::list<std::string> baseline
                = {"[", "<", std::to_string(0.0), "v", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("v > v")
        {
            auto e = v > v;
            static std::list<std::string> baseline = {
                "[", "<", "-Inf", "[", "+", "v", "[", "*", "-1", "v", "]", "]", std::to_string(0.0),
                "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("f > f")
        {
            auto e = f > f;
            static std::list<std::string> baseline
                = {"[", "<", std::to_string(0.0), std::to_string(0.0), "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }
    }

    SECTION("ge-operator")
    {
        WHEN("1 >= p")
        {
            auto e = 1 >= p;
            static std::list<std::string> baseline
                = {"[", "<=", "-Inf", "p", std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("p >= 1")
        {
            auto e = p >= 1;
            static std::list<std::string> baseline
                = {"[", "<=", std::to_string(1.0), "p", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("1.0 >= p")
        {
            auto e = 1.0 >= p;
            static std::list<std::string> baseline
                = {"[", "<=", "-Inf", "p", std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("p >= 1.0")
        {
            auto e = p >= 1.0;
            static std::list<std::string> baseline
                = {"[", "<=", std::to_string(1.0), "p", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("1 >= P")
        {
            auto e = 1 >= P;
            static std::list<std::string> baseline
                = {"[", "<=", "-Inf", "P", std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("P >= 1")
        {
            auto e = P >= 1;
            static std::list<std::string> baseline
                = {"[", "<=", std::to_string(1.0), "P", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("1.0 >= P")
        {
            auto e = 1.0 >= P;
            static std::list<std::string> baseline
                = {"[", "<=", "-Inf", "P", std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("P >= 1.0")
        {
            auto e = P >= 1.0;
            static std::list<std::string> baseline
                = {"[", "<=", std::to_string(1.0), "P", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("1 >= v")
        {
            auto e = 1 >= v;
            static std::list<std::string> baseline
                = {"[", "<=", "-Inf", "v", std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("v >= 1")
        {
            auto e = v >= 1;
            static std::list<std::string> baseline
                = {"[", "<=", std::to_string(1.0), "v", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("1.0 >= v")
        {
            auto e = 1.0 >= v;
            static std::list<std::string> baseline
                = {"[", "<=", "-Inf", "v", std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("v >= 1.0")
        {
            auto e = v >= 1.0;
            static std::list<std::string> baseline
                = {"[", "<=", std::to_string(1.0), "v", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("1 >= f")
        {
            auto e = 1 >= f;
            static std::list<std::string> baseline
                = {"[", "<=", "-Inf", std::to_string(0.0), std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("f >= 1")
        {
            auto e = f >= 1;
            static std::list<std::string> baseline
                = {"[", "<=", std::to_string(1.0), std::to_string(0.0), "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("1.0 >= f")
        {
            auto e = 1.0 >= f;
            static std::list<std::string> baseline
                = {"[", "<=", "-Inf", std::to_string(0.0), std::to_string(1.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("f >= 1.0")
        {
            auto e = f >= 1.0;
            static std::list<std::string> baseline
                = {"[", "<=", std::to_string(1.0), std::to_string(0.0), "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("p >= p")
        {
            auto e = p >= p;
            static std::list<std::string> baseline = {"[", "<=", "p", "p", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("p >= P")
        {
            auto e = p >= P;
            static std::list<std::string> baseline = {"[", "<=", "P", "p", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("P >= p")
        {
            auto e = P >= p;
            static std::list<std::string> baseline = {"[", "<=", "p", "P", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("p >= v")
        {
            auto e = p >= v;
            static std::list<std::string> baseline = {"[", "<=", "-Inf", "v", "p", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("v >= p")
        {
            auto e = v >= p;
            static std::list<std::string> baseline = {"[", "<=", "p", "v", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("p >= f")
        {
            auto e = p >= f;
            static std::list<std::string> baseline
                = {"[", "<=", std::to_string(0.0), "p", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("f >= p")
        {
            auto e = f >= p;
            static std::list<std::string> baseline
                = {"[", "<=", "p", std::to_string(0.0), "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("P >= P")
        {
            auto e = P >= P;
            static std::list<std::string> baseline = {"[", "<=", "P", "P", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("P >= v")
        {
            auto e = P >= v;
            static std::list<std::string> baseline = {"[", "<=", "-Inf", "v", "P", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("v >= P")
        {
            auto e = v >= P;
            static std::list<std::string> baseline = {"[", "<=", "P", "v", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("P >= f")
        {
            auto e = P >= f;
            static std::list<std::string> baseline
                = {"[", "<=", std::to_string(0.0), "P", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("f >= P")
        {
            auto e = f >= P;
            static std::list<std::string> baseline
                = {"[", "<=", "P", std::to_string(0.0), "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("f >= v")
        {
            auto e = f >= v;
            static std::list<std::string> baseline
                = {"[", "<=", "-Inf", "v", std::to_string(0.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("v >= f")
        {
            auto e = v >= f;
            static std::list<std::string> baseline
                = {"[", "<=", std::to_string(0.0), "v", "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("v >= v")
        {
            auto e = v >= v;
            static std::list<std::string> baseline = {"[",
                                                      "<=",
                                                      "-Inf",
                                                      "[",
                                                      "+",
                                                      "v",
                                                      "[",
                                                      "*",
                                                      "-1",
                                                      "v",
                                                      "]",
                                                      "]",
                                                      std::to_string(0.0),
                                                      "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("f >= f")
        {
            auto e = f >= f;
            static std::list<std::string> baseline
                = {"[", "<=", std::to_string(0.0), std::to_string(0.0), "Inf", "]"};
            REQUIRE(e.to_list() == baseline);
        }
    }

    SECTION("constructor")
    {
        WHEN("equal")
        {
            auto e = v <= 0;
            auto f = v >= 0;
            f = e;
            static std::list<std::string> baseline
                = {"[", "<=", "-Inf", "v", std::to_string(0.0), "]"};
            REQUIRE(e.to_list() == baseline);
        }
    }

    SECTION("Test inequality")
    {
        WHEN("inline")
        {
            auto v = coek::variable("v").lower(0).upper(1).value(0);
            REQUIRE((v - 1 == 0).is_inequality() == false);
            REQUIRE((v - 1 == 0).is_equality() == true);
        }

        WHEN("v <= 0")
        {
            auto e = v <= 0;

            static std::list<std::string> baseline
                = {"[", "<=", "-Inf", "v", std::to_string(0.0), "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.is_inequality() == true);
            REQUIRE(e.is_equality() == false);
        }

        WHEN("v < 0")
        {
            auto e = v < 0;

            static std::list<std::string> baseline
                = {"[", "<", "-Inf", "v", std::to_string(0.0), "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.is_inequality() == true);
            REQUIRE(e.is_equality() == false);
        }

        WHEN("v < 0.0")
        {
            auto e = v < 0.0;

            static std::list<std::string> baseline
                = {"[", "<", "-Inf", "v", std::to_string(0.0), "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.is_inequality() == true);
            REQUIRE(e.is_equality() == false);
        }

        WHEN("-1 < v < 0")
        {
            auto e = inequality(-1, v, 0, true);

            static std::list<std::string> baseline
                = {"[", "<", std::to_string(-1.000), "v", std::to_string(0.0), "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.is_inequality() == true);
            REQUIRE(e.is_equality() == false);
        }

        WHEN("-1 <= v <= 0")
        {
            auto e = inequality(-1, v, 0);

            static std::list<std::string> baseline
                = {"[", "<=", std::to_string(-1.000), "v", std::to_string(0.0), "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.is_inequality() == true);
            REQUIRE(e.is_equality() == false);
        }

        WHEN("-1.0 < v < 0.0")
        {
            auto e = inequality(-1.0, v, 0.0, true);

            static std::list<std::string> baseline
                = {"[", "<", std::to_string(-1.0), "v", std::to_string(0.0), "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.is_inequality() == true);
            REQUIRE(e.is_equality() == false);
        }

        WHEN("-1.0 <= v <= 0.0")
        {
            auto e = inequality(-1.0, v, 0.0);

            static std::list<std::string> baseline
                = {"[", "<=", std::to_string(-1.0), "v", std::to_string(0.0), "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.is_inequality() == true);
            REQUIRE(e.is_equality() == false);
        }

        WHEN("v + p <= 0")
        {
            auto e = v + p <= 0;

            static std::list<std::string> baseline
                = {"[", "<=", "-Inf", "[", "+", "v", "p", "]", std::to_string(0.0), "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.is_inequality() == true);
            REQUIRE(e.is_equality() == false);
        }

        WHEN("v + p < 0")
        {
            auto e = v + p < 0;

            static std::list<std::string> baseline
                = {"[", "<", "-Inf", "[", "+", "v", "p", "]", std::to_string(0.0), "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.is_inequality() == true);
            REQUIRE(e.is_equality() == false);
        }

        WHEN("v + P <= 0")
        {
            auto e = v + P <= 0;

            static std::list<std::string> baseline
                = {"[", "<=", "-Inf", "[", "+", "v", "P", "]", std::to_string(0.0), "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.is_inequality() == true);
            REQUIRE(e.is_equality() == false);
        }

        WHEN("v + P < 0")
        {
            auto e = v + P < 0;

            static std::list<std::string> baseline
                = {"[", "<", "-Inf", "[", "+", "v", "P", "]", std::to_string(0.0), "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.is_inequality() == true);
            REQUIRE(e.is_equality() == false);
        }
    }

    SECTION("Test Equality")
    {
        WHEN("v == 0")
        {
            auto e = v == 0;

            static std::list<std::string> baseline = {"[", "==", "v", std::to_string(0.0), "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.is_inequality() == false);
            REQUIRE(e.is_equality() == true);
        }

        WHEN("v == 0.0")
        {
            auto e = v == 0.0;

            static std::list<std::string> baseline = {"[", "==", "v", std::to_string(0.0), "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.is_inequality() == false);
            REQUIRE(e.is_equality() == true);
        }

        WHEN("v + p == 0")
        {
            auto e = v + p == 0;

            static std::list<std::string> baseline
                = {"[", "==", "[", "+", "v", "p", "]", std::to_string(0.0), "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.is_inequality() == false);
            REQUIRE(e.is_equality() == true);
        }

        WHEN("v + P == 0")
        {
            auto e = v + P == 0;

            static std::list<std::string> baseline
                = {"[", "==", "[", "+", "v", "P", "]", std::to_string(0.0), "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.is_inequality() == false);
            REQUIRE(e.is_equality() == true);
        }
    }

    SECTION("collect_terms")
    {
        coek::QuadraticExpr repn;
        coek::Model m;
        auto v = m.add_variable("v").lower(0).upper(1).value(0);
        auto a = 0 >= 1 + v;

        repn.collect_terms(a);
        std::stringstream sstr;
        sstr << repn;
        REQUIRE(sstr.str() == "Constant: 1\nLinear: \n1 v\nQuadratic: \n");
    }

    SECTION("Constraint Unit Tests")
    {
        /*
            WHEN( "Empty" ) {
                coek::Constraint c;
                REQUIRE( c.id() == 0 );
            }
        */
    }

    SECTION("constraint value")
    {
        auto a = coek::variable("a").lower(0.0).upper(1.0).value(0.0);
        auto b = coek::variable("b").lower(0.0).upper(1.0).value(1.0);
        auto q = coek::parameter("q").value(2);

        WHEN("e = 3*b + q == 0")
        {
            auto e = 3 * b + q == 0;
            REQUIRE(e.body().value() == 5.0);
            REQUIRE(e.is_feasible() == false);
        }
    }
}

#ifdef COEK_WITH_COMPACT_MODEL
TEMPLATE_TEST_CASE("indexed_constraint", "[smoke]", coek::Model, coek::CompactModel)
{
    TestType m;

    SECTION("simple array")
    {
        WHEN("constructor")
        {
            auto c = coek::constraint(10);
            REQUIRE(c.dim() == 1);
            auto C = coek::constraint({10});
            REQUIRE(C.dim() == 1);
        }

        WHEN("name")
        {
            auto c = coek::constraint("c", 10);
            REQUIRE(c.name() == "c");
        }

        WHEN("size")
        {
            auto v = coek::variable();
            auto c = coek::constraint(10);
            REQUIRE(c.size() == 0);
            c(0) = v == 0;
            REQUIRE(c.size() == 1);
        }

        WHEN("operator () - size_t")
        {
            auto v = coek::variable("v", 10);
            v.generate_names();
            auto c = coek::constraint(10);
            for (size_t i = 0; i < 10; ++i) {
                c(i) = v(i) == 1.0 * i;
            }
            m.add(c);

            static std::list<std::string> baseline = {"[", "==", "v[1]", std::to_string(1.0), "]"};
            REQUIRE(c.size() == 10);
            auto& con = c(1);
            REQUIRE(con.to_list() == baseline);
        }

        WHEN("operator () - int")
        {
            auto v = coek::variable(10);
            auto c = coek::constraint(10);
            for (int i = 0; i < 10; ++i) {
                c(i) = v(i) == 1.0 * i;
            }
            m.add(c);
        }

        WHEN("operator () - param")
        {
            auto p = coek::parameter();
            auto v = coek::variable(10);
            auto c = coek::constraint(10);
            for (int i = 0; i < 10; ++i) {
                p.value(i);
                c(p) = v(p) == 1.0 * i;
            }
            m.add(c);
        }

        /*
        TODO - Should we allow this?

        WHEN("operator () - expression") {
            auto p = coek::parameter();
            auto v = coek::variable(10);
            auto c = coek::constraint(10);
            for (int i=0; i<10; ++i) {
                p.value(i+1);
                c(p-1) = v(p+1) == 1.0*i;
                }
            m.add(c);
            }
        */

        WHEN("operator() - error1")
        {
            auto c = coek::constraint(10);
            REQUIRE_THROWS_WITH(c(0, 0, 0),
                                "Unexpected index value:  is an 1-D variable map but is being "
                                "indexed with 3 indices.");
        }

        WHEN("operator() - error2")
        {
            auto c = coek::constraint(10);
            REQUIRE_THROWS_WITH(c(0, 0),
                                "Unexpected index value:  is an 1-D variable map but is being "
                                "indexed with 2 indices.");
        }

        WHEN("operator() - error3")
        {
            auto c = coek::constraint(10);
            REQUIRE_THROWS_WITH(c(-1), "Unexpected index value: (-1)");
        }
    }

    SECTION("multi-dimensional array")
    {
        WHEN("constructor")
        {
            auto c = coek::constraint({10, 10});
            REQUIRE(c.dim() == 2);
            std::vector<size_t> dim = {10, 10};
            auto C = coek::constraint(dim);
            REQUIRE(C.dim() == 2);
        }

        WHEN("name")
        {
            auto c = coek::constraint("c", {10, 10});
            REQUIRE(c.name() == "c");
        }

        WHEN("size")
        {
            auto v = coek::variable();
            auto c = coek::constraint({10, 10});
            REQUIRE(c.size() == 0);
            c(0, 0) = v == 0;
            REQUIRE(c.size() == 1);
        }

        WHEN("operator () - size_t")
        {
            auto v = coek::variable({10, 10});
            auto c = coek::constraint({10, 10});
            for (size_t i = 0; i < 10; ++i) {
                auto tmp = v(i, i) == 1.0 * i;
                c(i, i) = tmp;
            }
            REQUIRE(c.size() == 10);
            m.add(c);
        }

        WHEN("operator () - int")
        {
            auto v = coek::variable({10, 10, 10});
            auto c = coek::constraint({10, 10, 10});
            for (int i = 0; i < 10; ++i) {
                auto tmp = v(i, i, i) == 1.0 * i;
                c(i, i, i) = tmp;
            }
            REQUIRE(c.size() == 10);
            m.add(c);
        }

        WHEN("operator() - error1")
        {
            auto c = coek::constraint({10, 10});
            REQUIRE_THROWS_WITH(c(0),
                                "Unexpected index value:  is an 2-D variable map but is being "
                                "indexed with 1 indices.");
        }

        WHEN("operator() - error2")
        {
            auto c = coek::constraint({10, 10});
            REQUIRE_THROWS_WITH(c(-1, -1), "Unexpected index value: (-1,-1)");
        }
    }

    SECTION("map")
    {
        WHEN("constructor")
        {
            auto A = coek::RangeSet(0, 9) * coek::RangeSet(0, 9);
            auto c = coek::constraint(A);
            REQUIRE(c.dim() == 2);
        }

        WHEN("name")
        {
            auto A = coek::RangeSet(0, 9) * coek::RangeSet(0, 9);
            auto c = coek::constraint("c", A);
            REQUIRE(c.name() == "c");
        }

        WHEN("size")
        {
            auto A = coek::RangeSet(0, 9) * coek::RangeSet(0, 9);
            auto v = coek::variable();
            auto c = coek::constraint(A);
            REQUIRE(c.size() == 0);
            c(0, 0) = v == 0;
            REQUIRE(c.size() == 1);
        }

        WHEN("operator () - size_t")
        {
            auto A = coek::RangeSet(0, 9) * coek::RangeSet(0, 9);
            auto v = coek::variable(A);
            auto c = coek::constraint(A);
            for (size_t i = 0; i < 10; ++i) {
                auto tmp = v(i, i) == 1.0 * i;
                c(i, i) = tmp;
            }
            REQUIRE(c.size() == 10);
            m.add(c);
        }

        WHEN("operator () - int")
        {
            auto A = coek::RangeSet(0, 9) * coek::RangeSet(0, 9) * coek::RangeSet(0, 9);
            auto v = coek::variable(A);
            auto c = coek::constraint(A);
            for (int i = 0; i < 10; ++i) {
                auto tmp = v(i, i, i) == 1.0 * i;
                c(i, i, i) = tmp;
            }
            REQUIRE(c.size() == 10);
            m.add(c);
        }

        WHEN("operator() - error1")
        {
            auto A = coek::RangeSet(0, 9) * coek::RangeSet(0, 9);
            auto c = coek::constraint(A);
            REQUIRE_THROWS_WITH(c(0),
                                "Unexpected index value:  is an 2-D variable map but is being "
                                "indexed with 1 indices.");
        }

        WHEN("operator() - error2")
        {
            auto A = coek::RangeSet(0, 9) * coek::RangeSet(0, 9);
            auto c = coek::constraint(A);
            REQUIRE_THROWS_WITH(c(-1, -1), "Unexpected index value: (-1,-1)");
        }
    }
}
#endif
