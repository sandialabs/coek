#include <iostream>

#include "catch2/catch.hpp"
#include "coek/ast/base_terms.hpp"
#include "coek/coek.hpp"

const double PI = 3.141592653589793238463;
const double E = exp(1.0);

#define ADNAME "asl"

TEST_CASE("autograd_asl", "[smoke]")
{
    SECTION("options")
    {
        coek::Model model;
        auto x = model.add_variable("x").lower(0).upper(1).value(0);
        auto y = model.add_variable("y").lower(0).upper(1).value(0);
        model.add_objective("o", x + y);
        coek::NLPModel m;

        std::string dir;
        REQUIRE(m.get_option("bad_option", dir) == false);
        REQUIRE(m.get_option("temp_directory", dir) == false);

        m.set_option("temp_directory", "foo");
        REQUIRE(m.get_option("temp_directory", dir) == true);
        REQUIRE(dir == "foo");

        m.set_option("temp_directory", "./");
        m.initialize(model, ADNAME);
    }
}
