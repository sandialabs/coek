
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


#ifdef COEK_WITH_COMPACT_MODEL
TEST_CASE("indexed_subexpression", "[smoke]")
{
    SECTION("simple array")
    {
        WHEN("constructor")
        {
            auto e = coek::subexpression("e", 10);
            REQUIRE(e.dim() == 1);
            auto E = coek::subexpression("e", {10});
            REQUIRE(E.dim() == 1);
        }

        WHEN("name")
        {
            auto e = coek::subexpression("e", 10);
            REQUIRE(e.name() == "e");
        }

        WHEN("size")
        {
            auto v = coek::variable();
            auto e = coek::subexpression("e", 10);
            REQUIRE(e.size() == 0);
            e(0) = v+1;
            REQUIRE(e.size() == 1);
        }

        WHEN("operator () - size_t")
        {
            auto v = coek::variable("v", 10);
            v.generate_names();
            auto e = coek::subexpression("e", 10);
            for (size_t i = 0; i < 10; ++i) {
                e(i) = v(i) + 1.0 * i;
            }

            static std::list<std::string> baseline = {"[", "_", "[", "+", "v[1]", std::to_string(1.0), "]", "]"};
            REQUIRE(e.size() == 10);
            auto& expr = e(1);
            REQUIRE(expr.to_list() == baseline);
        }

        WHEN("operator () - int")
        {
            auto v = coek::variable(10);
            auto e = coek::subexpression("e", 10);
            for (int i = 0; i < 10; ++i) {
                e(i) = v(i) + 1.0 * i;
            }
        }

        WHEN("operator () - param")
        {
            auto p = coek::parameter();
            auto v = coek::variable(10);
            auto e = coek::subexpression("e", 10);
            for (int i = 0; i < 10; ++i) {
                p.value(i);
                e(p) = v(p) + 1.0 * i;
            }
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
            auto e = coek::subexpression("e", 10);
            REQUIRE_THROWS_WITH(e(0, 0, 0),
                                "Unexpected index value: e is an 1-D variable map but is being "
                                "indexed with 3 indices.");
        }

        WHEN("operator() - error2")
        {
            auto e = coek::subexpression("e", 10);
            REQUIRE_THROWS_WITH(e(0, 0),
                                "Unexpected index value: e is an 1-D variable map but is being "
                                "indexed with 2 indices.");
        }

        WHEN("operator() - error3")
        {
            auto e = coek::subexpression("e", 10);
            REQUIRE_THROWS_WITH(e(-1), "Unexpected index value: e(-1)");
        }
    }

    SECTION("multi-dimensional array")
    {
        WHEN("constructor")
        {
            auto e = coek::subexpression("e", {10, 10});
            REQUIRE(e.dim() == 2);
            std::vector<size_t> dim = {10, 10};
            auto E = coek::constraint("E", dim);
            REQUIRE(E.dim() == 2);
        }

        WHEN("name")
        {
            auto e = coek::subexpression("e", {10, 10});
            REQUIRE(e.name() == "e");
        }

        WHEN("size")
        {
            auto v = coek::variable();
            auto e = coek::subexpression("e", {10, 10});
            REQUIRE(e.size() == 0);
            e(0, 0) = v + 1;
            REQUIRE(e.size() == 1);
        }

        WHEN("operator () - size_t")
        {
            auto v = coek::variable({10, 10});
            auto e = coek::subexpression("e", {10, 10});
            for (size_t i = 0; i < 10; ++i) {
                e(i, i) = v(i, i) + 1.0 * i;
            }
            REQUIRE(e.size() == 10);
        }

        WHEN("operator () - int")
        {
            auto v = coek::variable({10, 10, 10});
            auto e = coek::subexpression("e", {10, 10, 10});
            for (int i = 0; i < 10; ++i) {
                e(i, i, i) = v(i, i, i) + 1.0 * i;
            }
            REQUIRE(e.size() == 10);
        }

        WHEN("operator() - error1")
        {
            auto e = coek::subexpression("e", {10, 10});
            REQUIRE_THROWS_WITH(e(0),
                                "Unexpected index value: e is an 2-D variable map but is being "
                                "indexed with 1 indices.");
        }

        WHEN("operator() - error2")
        {
            auto e = coek::subexpression("e", {10, 10});
            REQUIRE_THROWS_WITH(e(-1, -1), "Unexpected index value: e(-1,-1)");
        }
    }

    SECTION("map")
    {
        WHEN("constructor")
        {
            auto A = coek::RangeSet(0, 9) * coek::RangeSet(0, 9);
            auto e = coek::subexpression("e", A);
            REQUIRE(e.dim() == 2);
        }

        WHEN("name")
        {
            auto A = coek::RangeSet(0, 9) * coek::RangeSet(0, 9);
            auto e = coek::subexpression("e", A);
            REQUIRE(e.name() == "e");
        }

        WHEN("size")
        {
            auto A = coek::RangeSet(0, 9) * coek::RangeSet(0, 9);
            auto v = coek::variable();
            auto e = coek::subexpression("e", A);
            REQUIRE(e.size() == 0);
            e(0, 0) = v + 1;
            REQUIRE(e.size() == 1);
        }

        WHEN("operator () - size_t")
        {
            auto A = coek::RangeSet(0, 9) * coek::RangeSet(0, 9);
            auto v = coek::variable(A);
            auto e = coek::subexpression("e", A);
            for (size_t i = 0; i < 10; ++i) {
                e(i, i) = v(i, i) + 1.0 * i;
            }
            REQUIRE(e.size() == 10);
        }

        WHEN("operator () - int")
        {
            auto A = coek::RangeSet(0, 9) * coek::RangeSet(0, 9) * coek::RangeSet(0, 9);
            auto v = coek::variable(A);
            auto e = coek::subexpression("e", A);
            for (int i = 0; i < 10; ++i) {
                e(i, i, i) = v(i, i, i) + 1.0 * i;
            }
            REQUIRE(e.size() == 10);
        }

        WHEN("operator() - error1")
        {
            auto A = coek::RangeSet(0, 9) * coek::RangeSet(0, 9);
            auto e = coek::subexpression("e", A);
            REQUIRE_THROWS_WITH(e(0),
                                "Unexpected index value: e is an 2-D variable map but is being "
                                "indexed with 1 indices.");
        }

        WHEN("operator() - error2")
        {
            auto A = coek::RangeSet(0, 9) * coek::RangeSet(0, 9);
            auto e = coek::subexpression("e", A);
            REQUIRE_THROWS_WITH(e(-1, -1), "Unexpected index value: e(-1,-1)");
        }
    }
}
#endif
