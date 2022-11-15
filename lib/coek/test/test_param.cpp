
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

void xyz() {}

TEST_CASE("elementary_param", "[smoke]")
{
    SECTION("values")
    {
        WHEN("parameter - 3")
        {
            auto q = coek::parameter("q").value(2);
            REQUIRE(q.value() == 2);
            q.value(3);
            REQUIRE(q.value() == 3);
        }
    }

    SECTION("constructors")
    {
        WHEN("copy")
        {
            // Simple constructor
            auto q = coek::parameter().value(3);

            // Test copy constructor
            coek::Parameter Q(q);
            REQUIRE(Q.value() == 3);
        }

        WHEN("equal")
        {
            // Simple constructor
            auto q = coek::parameter().value(4);

            // Test copy constructor
            auto Q = coek::parameter().value(5);
            Q = q;
            REQUIRE(Q.value() == 4);
        }
    }

    SECTION("constructors")
    {
        auto q = coek::parameter("q").value(3);
        REQUIRE(q.name() == "q");
    }

    SECTION("write")
    {
        std::stringstream sstr;
        auto q = coek::parameter("q").value(3);
        sstr << q;
        REQUIRE(sstr.str() == "q");
    }

#ifdef WITH_AST_ENV
    REQUIRE(coek::env.check_memory() == true);
#endif
}

TEST_CASE("model_indexparam", "[smoke]")
{
    SECTION("values")
    {
        WHEN("IndexParameter - 3")
        {
            auto q = coek::set_element("q");
            q.value(1);
            int tmp = -1;
            auto success = q.get_value(tmp);
            REQUIRE(success == true);
            REQUIRE(tmp == 1);
            q.value(3.5);
            success = q.get_value(tmp);
            REQUIRE(success == false);
        }
        WHEN("IndexParameter - 3.5")
        {
            auto q = coek::set_element();
            q.value(3.5);
            double tmp = -3.5;
            auto success = q.get_value(tmp);
            REQUIRE(success == true);
            REQUIRE(tmp == 3.5);
            q.value(3);
            success = q.get_value(tmp);
            REQUIRE(success == false);
        }
        WHEN("IndexParameter - 'here'")
        {
            auto q = coek::set_element();
            q.value("here");
            std::string tmp = "there";
            auto success = q.get_value(tmp);
            REQUIRE(success == true);
            REQUIRE(tmp == "here");
            q.value(3.5);
            success = q.get_value(tmp);
            REQUIRE(success == false);
        }
    }

    SECTION("constructors")
    {
        WHEN("copy")
        {
            // Simple constructor
            auto q = coek::set_element();
            q.value(3);

            // Test copy constructor
            coek::IndexParameter Q(q);
            int tmp = -1;
            auto success = Q.get_value(tmp);
            REQUIRE(success == true);
            REQUIRE(tmp == 3);
        }

        WHEN("equal")
        {
            // Simple constructor
            auto q = coek::set_element();
            q.value(4);

            // Test copy constructor
            auto Q = coek::set_element();
            Q.value(5);
            Q = q;
            int tmp = -1;
            auto success = Q.get_value(tmp);
            REQUIRE(success == true);
            REQUIRE(tmp == 4);
        }
    }

    SECTION("constructors")
    {
        auto q = coek::set_element("q");
        REQUIRE(q.name() == "q");
    }

    SECTION("write")
    {
        std::stringstream sstr;
        auto q = coek::set_element("q");
        sstr << q;
        REQUIRE(sstr.str() == "q");
    }

    SECTION("name")
    {
        auto p = coek::set_element();
        auto q = coek::set_element("q");
        REQUIRE(p.name() == "unknown");
        p.name("P");
        REQUIRE(p.name() == "P");
        REQUIRE(q.name() == "q");
    }

#ifdef WITH_AST_ENV
    REQUIRE(coek::env.check_memory() == true);
#endif
}

#ifdef COEK_WITH_COMPACT_MODEL
TEST_CASE("1D_param_map", "[smoke]")
{
    SECTION("int")
    {
        std::vector<int> v = {1, 5, 3, 7};

        WHEN("constructors")
        {
            auto s = coek::SetOf(v);
            auto v1 = coek::parameter(s);
            REQUIRE(v1.size() == 4);
            auto v2 = coek::parameter("v2", s);
            REQUIRE(v2.size() == 4);
            auto v3 = coek::parameter_map(s);
            REQUIRE(v3.size() == 4);
            auto v4 = coek::parameter_map("v4", s);
            REQUIRE(v4.size() == 4);
        }

        WHEN("size")
        {
            auto s = coek::SetOf(v);
            auto params = coek::parameter(s);
            REQUIRE(params.size() == 4);
        }

        WHEN("typeof")
        {
            auto s = coek::SetOf(v);
            auto params = coek::parameter(s);
            REQUIRE(typeid(params(1)).name() == typeid(coek::Parameter).name());
        }

        WHEN("value")
        {
            auto s = coek::SetOf(v);
            auto params = coek::parameter(s).value(1);
            REQUIRE(params(5).value() == 1);
        }
    }

    SECTION("int_ranged")
    {
        std::vector<int> v = {1, 3, 5, 7};
        auto s = coek::RangeSet(1, 7, 2);

        WHEN("size")
        {
            auto params = coek::parameter(s);
            REQUIRE(params.size() == 4);
        }

        WHEN("typeof")
        {
            auto params = coek::parameter(s);
            REQUIRE(typeid(params(1)).name() == typeid(coek::Parameter).name());
        }
    }

    SECTION("abstract")
    {
        std::vector<int> v = {1, 5, 3, 7};
        auto s = coek::SetOf(v);
        auto r = coek::SetOf({0});
        auto S = s - r;
        auto params = coek::parameter("params", S).value(0).generate_names();

        WHEN("typeof") { REQUIRE(typeid(params(1)).name() == typeid(coek::Parameter).name()); }

        WHEN("index1")
        {
            auto i = coek::set_element("i");
            REQUIRE(typeid(params(1)).name() == typeid(coek::Parameter).name());
            REQUIRE(typeid(params(i)).name() == typeid(coek::Expression).name());
        }

        WHEN("index2")
        {
            auto v = coek::parameter();
            coek::Expression f = v;
            auto e = params(1);
            REQUIRE(e.name() == "params[1]");
            // auto it = e.to_list().begin();
            // REQUIRE_THAT( *it, Catch::Matchers::StartsWith("params") );
        }

        WHEN("index3")
        {
            auto i = coek::set_element("i");
            auto e = params(i);
            static std::list<std::string> baseline = {"params[i]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("index4")
        {
            auto i = coek::set_element("i");
            auto e = params(i + 1);
            static std::list<std::string> baseline = {"params[i + 1]"};
            REQUIRE(e.to_list() == baseline);
        }
    }

#    ifdef WITH_AST_ENV
    REQUIRE(coek::env.check_memory() == true);
#    endif
}
#endif

TEST_CASE("1D_param_array", "[smoke]")
{
    SECTION("int_vector")
    {
        WHEN("constructors")
        {
            auto v1 = coek::parameter(4);
            REQUIRE(v1.size() == 4);
            auto v2 = coek::parameter("v2", 4);
            REQUIRE(v2.size() == 4);
            auto v3 = coek::parameter_array(4);
            REQUIRE(v3.size() == 4);
            auto v4 = coek::parameter_array("v4", 4);
            REQUIRE(v4.size() == 4);
        }

        WHEN("size")
        {
            auto params = coek::parameter(4);
            REQUIRE(params.size() == 4);
        }

        WHEN("typeof")
        {
            auto params = coek::parameter(4);
            REQUIRE(typeid(params(1)).name() == typeid(coek::Parameter).name());
        }

        WHEN("index")
        {
            auto params = coek::parameter(4).value(1);
            for (size_t i = 0; i < 4; i++) REQUIRE(params(i).value() == 1);
            for (int i = 0; i < 4; i++) REQUIRE(params(i).value() == 1);
        }

        WHEN("name")
        {
            auto params = coek::parameter(4).name("v").generate_names();
            for (int i = 0; i < 4; i++) REQUIRE(params(i).name() == "v[" + std::to_string(i) + "]");
        }
    }

    SECTION("int_vector_dim")
    {
        std::vector<size_t> dim{4};

        WHEN("size")
        {
            auto params = coek::parameter("v", dim);
            REQUIRE(params.size() == 4);
        }

        WHEN("typeof")
        {
            auto params = coek::parameter(dim);
            REQUIRE(typeid(params(1)).name() == typeid(coek::Parameter).name());
        }

        WHEN("index")
        {
            auto params = coek::parameter(dim).value(1);
            for (size_t i = 0; i < 4; i++) REQUIRE(params(i).value() == 1);
        }

        WHEN("name")
        {
            auto params = coek::parameter(dim).name("v").generate_names();
            for (int i = 0; i < 4; i++) REQUIRE(params(i).name() == "v[" + std::to_string(i) + "]");
        }
    }

    SECTION("int_initializer_list")
    {
        WHEN("size")
        {
            auto params = coek::parameter("v", {4});
            REQUIRE(params.size() == 4);
        }

        WHEN("typeof")
        {
            auto params = coek::parameter({4});
            REQUIRE(typeid(params(1)).name() == typeid(coek::Parameter).name());
        }

        WHEN("index")
        {
            auto params = coek::parameter({4}).value(1);
            for (size_t i = 0; i < 4; i++) REQUIRE(params(i).value() == 1);
        }

        WHEN("name")
        {
            auto params = coek::parameter({4}).name("v").generate_names();
            for (int i = 0; i < 4; i++) REQUIRE(params(i).name() == "v[" + std::to_string(i) + "]");
        }
    }

#ifdef WITH_AST_ENV
    REQUIRE(coek::env.check_memory() == true);
#endif
}

#ifdef COEK_WITH_COMPACT_MODEL
TEST_CASE("2D_param_map", "[smoke]")
{
    SECTION("int")
    {
        std::vector<int> v = {1, 5, 3, 7};
        std::vector<int> w = {2, 6, 4, 8};

        WHEN("constructors")
        {
            auto V = coek::SetOf(v);
            auto W = coek::SetOf(w);
            auto S = V * W;
            auto v1 = coek::parameter(S);
            REQUIRE(v1.size() == 16);
            auto v2 = coek::parameter("v2", S);
            REQUIRE(v2.size() == 16);
            auto v3 = coek::parameter_map(S);
            REQUIRE(v3.size() == 16);
            auto v4 = coek::parameter_map("v4", S);
            REQUIRE(v4.size() == 16);
        }

        WHEN("size")
        {
            auto V = coek::SetOf(v);
            auto W = coek::SetOf(w);
            auto S = V * W;
            auto params = coek::parameter(S);
            REQUIRE(params.size() == 16);
        }

        WHEN("typeof")
        {
            auto V = coek::SetOf(v);
            auto W = coek::SetOf(w);
            auto S = V * W;
            auto params = coek::parameter(S);
            REQUIRE(typeid(params(1, 2)).name() == typeid(coek::Parameter).name());
        }

        WHEN("param_index_error")
        {
            auto V = coek::SetOf(v);
            auto W = coek::SetOf(w);
            auto params = coek::parameter(V * W).name("x");
            REQUIRE(params.dim() == 2);
            REQUIRE_THROWS_WITH(params(0),
                                "Unexpected index value: x is an 2-D parameter map but is being "
                                "indexed with 1 indices.");
        }
    }

    SECTION("abstract")
    {
        std::vector<int> v = {1, 5, 3, 7};
        std::vector<int> w = {2, 6, 4, 8};
        auto V = coek::SetOf(v);
        auto W = coek::SetOf(w);
        auto r = coek::SetOf({0});
        auto S = V * W;
        auto params = coek::parameter("params", S).value(0).generate_names();

        WHEN("typeof") { REQUIRE(typeid(params(1, 2)).name() == typeid(coek::Parameter).name()); }

        WHEN("index1")
        {
            auto i = coek::set_element("i");
            auto j = coek::set_element("j");
            REQUIRE(typeid(params(1, 2)).name() == typeid(coek::Parameter).name());
            REQUIRE(typeid(params(10, 11)).name() == typeid(coek::Parameter).name());
            REQUIRE(typeid(params(i, j)).name() == typeid(coek::Expression).name());
        }

        WHEN("index2")
        {
            auto e = params(1, 2);
            REQUIRE(e.name() == "params[1,2]");
            // auto it = e.to_list().begin();
            // REQUIRE_THAT( *it, Catch::Matchers::StartsWith("params") );
        }

        WHEN("index3")
        {
            auto i = coek::set_element("i");
            auto j = coek::set_element("j");
            auto e = params(i, j);
            static std::list<std::string> baseline = {"params[i,j]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("index4")
        {
            auto i = coek::set_element("i");
            auto j = coek::set_element("j");
            auto e = params(i + 1, j - 1);
            static std::list<std::string> baseline = {"params[i + 1,j + -1]"};
            REQUIRE(e.to_list() == baseline);
        }
    }

#    ifdef WITH_AST_ENV
    REQUIRE(coek::env.check_memory() == true);
#    endif
}
#endif

TEST_CASE("2D_param_array", "[smoke]")
{
    SECTION("int_vector_dim")
    {
        WHEN("constructors")
        {
            auto v1 = coek::parameter({4, 4});
            REQUIRE(v1.size() == 16);
            auto v2 = coek::parameter("v2", {4, 4});
            REQUIRE(v2.size() == 16);
            auto v3 = coek::parameter_array({4, 4});
            REQUIRE(v3.size() == 16);
            auto v4 = coek::parameter_array("v4", {4, 4});
            REQUIRE(v4.size() == 16);
        }

        WHEN("size")
        {
            std::vector<size_t> dim{4, 3};
            auto params = coek::parameter("v", dim);
            REQUIRE(params.size() == 12);
        }

        WHEN("typeof")
        {
            std::vector<size_t> dim{4, 3};
            auto params = coek::parameter(dim);
            REQUIRE(typeid(params(1, 1)).name() == typeid(coek::Parameter).name());
        }

        WHEN("index")
        {
            std::vector<size_t> dim{4, 5};
            auto params = coek::parameter(dim).value(1);
            for (size_t i = 0; i < 4; i++) REQUIRE(params(i, i).value() == 1);
        }

        WHEN("index error")
        {
            std::vector<size_t> dim{4, 3};
            auto params = coek::parameter(dim).name("x");
            REQUIRE(params.dim() == 2);
            REQUIRE_THROWS_WITH(params(0),
                                "Unexpected index value: x is an 2-D parameter array but is being "
                                "indexed with 1 indices.");
        }

        WHEN("name")
        {
            std::vector<size_t> dim{4, 3};
            auto params = coek::parameter(dim).name("v").generate_names();
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 3; j++)
                    REQUIRE(params(i, j).name()
                            == "v[" + std::to_string(i) + "," + std::to_string(j) + "]");
        }
    }

    SECTION("int_initializer_list")
    {
        WHEN("size")
        {
            auto params = coek::parameter("v", {4, 3});
            REQUIRE(params.size() == 12);
        }

        WHEN("typeof")
        {
            auto params = coek::parameter({4, 3});
            REQUIRE(typeid(params(1, 1)).name() == typeid(coek::Parameter).name());
        }

        WHEN("index")
        {
            auto params = coek::parameter({4, 3}).value(1);
            for (size_t i = 0; i < 3; i++) REQUIRE(params(i, i).value() == 1);
        }

        WHEN("index error")
        {
            auto params = coek::parameter({4, 3}).name("x");
            REQUIRE(params.dim() == 2);
            REQUIRE_THROWS_WITH(params(0),
                                "Unexpected index value: x is an 2-D parameter array but is being "
                                "indexed with 1 indices.");
        }

        WHEN("name")
        {
            auto params = coek::parameter({4, 3}).name("v").generate_names();
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 3; j++)
                    REQUIRE(params(i, j).name()
                            == "v[" + std::to_string(i) + "," + std::to_string(j) + "]");
        }
    }

#ifdef WITH_AST_ENV
    REQUIRE(coek::env.check_memory() == true);
#endif
}

TEST_CASE("3D_param_array", "[smoke]")
{
    SECTION("int_vector_dim")
    {
        WHEN("constructors")
        {
            auto v1 = coek::parameter({5, 4, 3});
            REQUIRE(v1.size() == 60);
            auto v2 = coek::parameter("v2", {5, 4, 3});
            REQUIRE(v2.size() == 60);
            auto v3 = coek::parameter({5, 4, 3});
            REQUIRE(v3.size() == 60);
            auto v4 = coek::parameter("v4", {5, 4, 3});
            REQUIRE(v4.size() == 60);
        }

        WHEN("size")
        {
            std::vector<size_t> dim{5, 4, 3};
            auto params = coek::parameter("v", dim);
            REQUIRE(params.size() == 60);
        }

        WHEN("typeof")
        {
            std::vector<size_t> dim{5, 4, 3};
            auto params = coek::parameter(dim);
            REQUIRE(typeid(params(1, 1)).name() == typeid(coek::Parameter).name());
        }

        WHEN("index")
        {
            std::vector<size_t> dim{5, 4, 3};
            auto params = coek::parameter(dim).value(1);
            for (size_t i = 0; i < 3; i++) REQUIRE(params(i, i, i).value() == 1);
        }

        WHEN("index error")
        {
            std::vector<size_t> dim{5, 4, 3};
            auto params = coek::parameter(dim).name("x");
            REQUIRE(params.dim() == 3);
            REQUIRE_THROWS_WITH(params(0),
                                "Unexpected index value: x is an 3-D parameter array but is being "
                                "indexed with 1 indices.");
        }

        WHEN("name")
        {
            std::vector<size_t> dim{5, 4, 3};
            auto params = coek::parameter(dim).name("v").generate_names();
            for (int i = 0; i < 5; i++)
                for (int j = 0; j < 4; j++)
                    for (int k = 0; k < 3; k++)
                        REQUIRE(params(i, j, k).name()
                                == "v[" + std::to_string(i) + "," + std::to_string(j) + ","
                                       + std::to_string(k) + "]");
        }
    }

    SECTION("int_initializer_list")
    {
        WHEN("size")
        {
            auto params = coek::parameter("v", {5, 4, 3});
            REQUIRE(params.size() == 60);
        }

        WHEN("typeof")
        {
            auto params = coek::parameter({5, 4, 3});
            REQUIRE(typeid(params(1, 1, 1)).name() == typeid(coek::Parameter).name());
        }

        WHEN("index")
        {
            auto params = coek::parameter({5, 4, 3}).value(1);
            for (size_t i = 0; i < 3; i++) REQUIRE(params(i, i, i).value() == 1);
        }

        WHEN("index error")
        {
            auto params = coek::parameter({5, 4, 3}).name("x");
            REQUIRE(params.dim() == 3);
            REQUIRE_THROWS_WITH(params(0),
                                "Unexpected index value: x is an 3-D parameter array but is being "
                                "indexed with 1 indices.");
        }

        WHEN("name")
        {
            auto params = coek::parameter({5, 4, 3}).name("v").generate_names();
            for (int i = 0; i < 5; i++)
                for (int j = 0; j < 4; j++)
                    for (int k = 0; k < 3; k++)
                        REQUIRE(params(i, j, k).name()
                                == "v[" + std::to_string(i) + "," + std::to_string(j) + ","
                                       + std::to_string(k) + "]");
        }
    }

#ifdef WITH_AST_ENV
    REQUIRE(coek::env.check_memory() == true);
#endif
}

#ifdef COEK_WITH_COMPACT_MODEL
TEST_CASE("3D_param_api", "[smoke]")
{
    SECTION("map")
    {
        std::vector<int> vec = {1, 2, 3, 4};
        auto p = coek::parameter("p").value(1);

        WHEN("index api")
        {
            auto V = coek::SetOf(vec);
            auto v = coek::parameter_map(V * V * V).name("x").value(1);
            REQUIRE(v.size() == 64);

            int ival = 1;
            size_t uval = 1;
            auto e = p + 1;
            // Misc tests to improve code coverage for the collect_args() methods
            REQUIRE(v(e, uval, ival).expand().value() == 1);

            REQUIRE(v(uval, uval, e).expand().value() == 1);

            REQUIRE(v(ival, ival, e).expand().value() == 1);

            REQUIRE(v(uval, uval, uval).value() == 1);

            REQUIRE(v(ival, ival, ival).value() == 1);

            REQUIRE_THROWS_WITH(v(uval, uval),
                                "Unexpected index value: x is an 3-D parameter map but is being "
                                "indexed with 2 indices.");

            REQUIRE_THROWS_WITH(v(ival, ival),
                                "Unexpected index value: x is an 3-D parameter map but is being "
                                "indexed with 2 indices.");

            REQUIRE_THROWS_WITH(v(e, ival),
                                "Unexpected index value: x is an 3-D parameter map but is being "
                                "indexed with 2 indices.");

            REQUIRE_THROWS_WITH(v(ival, ival, -1), "Unknown index value: x[1,1,-1]");
        }

        WHEN("param value")
        {
            auto q = coek::parameter("q").value(1);
            auto V = coek::SetOf(vec);
            auto v = coek::parameter_map(V).name("x").value(1);

            REQUIRE(v(1).value() == 1);
            v.value(q + 1);
            REQUIRE(v(1).value() == 2);
            q.value(2);
            REQUIRE(v(1).value() == 3);
        }
    }

    SECTION("array")
    {
        auto p = coek::parameter("p").value(1);

        WHEN("index api")
        {
            auto v = coek::parameter_array({4, 4, 4}).name("x").value(1);
            REQUIRE(v.size() == 64);

            int ival = 1;
            size_t uval = 1;
            auto e = p + 1;
            // Misc tests to improve code coverage for the collect_args() methods
            REQUIRE(v(e, uval, ival).expand().value() == 1);

            REQUIRE(v(uval, uval, e).expand().value() == 1);

            REQUIRE(v(ival, ival, e).expand().value() == 1);

            REQUIRE(v(uval, uval, uval).value() == 1);

            REQUIRE(v(ival, ival, ival).value() == 1);

            REQUIRE_THROWS_WITH(v(uval),
                                "Unexpected index value: x is an 3-D parameter array but is being "
                                "indexed with 1 indices.");

            REQUIRE_THROWS_WITH(v(uval, uval),
                                "Unexpected index value: x is an 3-D parameter array but is being "
                                "indexed with 2 indices.");

            REQUIRE_THROWS_WITH(v(e, ival),
                                "Unexpected index value: x is an 3-D parameter array but is being "
                                "indexed with 2 indices.");

            REQUIRE_THROWS_WITH(v(ival, ival),
                                "Unexpected index value: x is an 3-D parameter array but is being "
                                "indexed with 2 indices.");

            REQUIRE_THROWS_WITH(v(100, 100, 100), "Unknown index value: x[100,100,100]");
        }

        WHEN("param value")
        {
            auto q = coek::parameter("q").value(1);
            auto v = coek::parameter_array(4).name("x").value(1);

            REQUIRE(v(1).value() == 1);
            v.value(q + 1);
            REQUIRE(v(1).value() == 2);
            q.value(2);
            REQUIRE(v(1).value() == 3);
        }
    }

#    ifdef WITH_AST_ENV
    REQUIRE(coek::env.check_memory() == true);
#    endif
}

#endif
