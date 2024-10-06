
#include <cmath>
#include <iostream>
#include <memory>
#include <sstream>

#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers.hpp"
#include "coek/ast/base_terms.hpp"
#include "coek/ast/value_terms.hpp"
#include "coek/coek.hpp"

const double E = exp(1.0);

TEST_CASE("elementary_data", "[smoke]")
{
    SECTION("values")
    {
        WHEN("data - 3")
        {
            auto q = coek::data("q").value(2);
            REQUIRE(q.value() == 2);
            q.value(3);
            REQUIRE(q.value() == 3);
        }
        WHEN("data - p+1")
        {
            auto p = coek::data("p").value(3);
            auto q = coek::data("q").value(2);
            REQUIRE(q.value() == 2);
            q.value(p + 1);
            REQUIRE(q.value() == 4);
        }
    }

    SECTION("constructors")
    {
        WHEN("copy")
        {
            // Simple constructor
            auto q = coek::data().value(3);

            // Test copy constructor
            coek::Parameter Q(q);
            REQUIRE(Q.value() == 3);
        }

        WHEN("equal")
        {
            // Simple constructor
            auto q = coek::data().value(4);

            // Test copy constructor
            auto Q = coek::data().value(5);
            Q = q;
            REQUIRE(Q.value() == 4);
        }
    }

    SECTION("constructors")
    {
        auto q = coek::data("q").value(3);
        REQUIRE(q.name() == "q");
    }

    SECTION("write")
    {
        std::stringstream sstr;
        auto q = coek::data("q").value(3);
        sstr << q;
        REQUIRE(sstr.str() == "q");
    }
}

TEST_CASE("model_indexdata", "[smoke]")
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
}

#ifdef COEK_WITH_COMPACT_MODEL
TEST_CASE("1D_data_map", "[smoke]")
{
    SECTION("int")
    {
        std::vector<int> v = {1, 5, 3, 7};

        WHEN("constructors")
        {
            auto s = coek::SetOf(v);
            auto v1 = coek::data(s);
            REQUIRE(v1.size() == 4);
            auto v2 = coek::data("v2", s);
            REQUIRE(v2.size() == 4);
            auto v3 = coek::data_map(s);
            REQUIRE(v3.size() == 4);
            auto v4 = coek::data_map("v4", s);
            REQUIRE(v4.size() == 4);
        }

        WHEN("size")
        {
            auto s = coek::SetOf(v);
            auto dats = coek::data(s);
            REQUIRE(dats.size() == 4);
        }

        WHEN("typeof")
        {
            auto s = coek::SetOf(v);
            auto dats = coek::data(s);
            REQUIRE(typeid(dats(1)).name() == typeid(coek::Expression).name());
        }

        WHEN("value - 1")
        {
            auto s = coek::SetOf(v);
            auto dats = coek::data(s).value(1);
            REQUIRE(dats(5).value() == 1);
        }
    }

    SECTION("int_ranged")
    {
        std::vector<int> v = {1, 3, 5, 7};
        auto s = coek::RangeSet(1, 7, 2);

        WHEN("size")
        {
            auto dats = coek::data(s);
            REQUIRE(dats.size() == 4);
        }

        WHEN("typeof")
        {
            auto dats = coek::data(s);
            REQUIRE(typeid(dats(1)).name() == typeid(coek::Expression).name());
        }
    }

    SECTION("abstract")
    {
        std::vector<int> v = {1, 5, 3, 7};
        auto s = coek::SetOf(v);
        auto r = coek::SetOf({0});
        auto S = s - r;
        auto dats = coek::data("dats", S).value(0);

        WHEN("typeof") { REQUIRE(typeid(dats(1)).name() == typeid(coek::Expression).name()); }

        WHEN("index1")
        {
            auto i = coek::set_element("i");
            REQUIRE(typeid(dats(1)).name() == typeid(coek::Expression).name());
            REQUIRE(typeid(dats(i)).name() == typeid(coek::Expression).name());
        }

        WHEN("index2")
        {
            auto v = coek::data();
            coek::Expression f = v;
            auto e = dats(1);
            // REQUIRE(e.name() == "dats[1]");
            //  auto it = e.to_list().begin();
            //  REQUIRE_THAT( *it, Catch::Matchers::StartsWith("dats") );
        }

        WHEN("index3")
        {
            auto i = coek::set_element("i");
            auto e = dats(i);
            static std::list<std::string> baseline = {"dats[i]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("index4")
        {
            auto i = coek::set_element("i");
            auto e = dats(i + 1);
            static std::list<std::string> baseline = {"dats[i + 1]"};
            REQUIRE(e.to_list() == baseline);
        }
    }
}
#endif

#if __cpp_lib_variant
TEST_CASE("1D_data_array", "[smoke]")
{
    SECTION("int_vector")
    {
        WHEN("constructors")
        {
            auto v1 = coek::data(4);
            REQUIRE(v1.size() == 4);
            auto v2 = coek::data("v2", 4);
            REQUIRE(v2.size() == 4);
            auto v3 = coek::data_array(4);
            REQUIRE(v3.size() == 4);
            auto v4 = coek::data_array("v4", 4);
            REQUIRE(v4.size() == 4);
        }

        WHEN("size")
        {
            auto dats = coek::data(4);
            REQUIRE(dats.size() == 4);
        }

        WHEN("typeof")
        {
            auto dats = coek::data(4);
            REQUIRE(typeid(dats(1)).name() == typeid(coek::Expression).name());
        }

        WHEN("value - 1")
        {
            auto dats = coek::data(4);

            // Set value using template
            dats.value(1);
            for (size_t i = 0; i < 4; i++)
                REQUIRE(dats(i).value() == 1);

            // Set value for all indices
            dats.value(2);
            for (size_t i = 0; i < 4; i++)
                REQUIRE(dats(i).value() == 2);
        }
#    if 0
        WHEN("value - q")
        {
            auto dats = coek::data(4);
            auto q = coek::data().value(2);
            for (size_t i = 0; i < 4; i++)
                dats(i).value(q + (int)i);  // TODO - generalize API to include unsigned ints
            for (size_t i = 0; i < 4; i++)
                REQUIRE(dats(i).value() == 2 + (int)i);
        }
#    endif
        WHEN("value all - q")
        {
            auto dats = coek::data(4);
            auto q = coek::data().value(2);

            // Set value using template
            dats.value(q + (int)2);  // TODO - generalize API to include unsigned ints
            for (size_t i = 0; i < 4; i++)
                REQUIRE(dats(i).value() == 4);

            // Set value for all indices
            dats.value(q + (int)3);
            for (size_t i = 0; i < 4; i++)
                REQUIRE(dats(i).value() == 5);
        }
#    if 0
        WHEN("name")
        {
            auto dats = coek::data(4);

            dats.name("v");
            for (int i = 0; i < 4; i++)
                REQUIRE(dats(i).name() == "v[" + std::to_string(i) + "]");

            // We don't need to call again.  Names are automatically generated
            // after the first time.
            dats.name("w");
            for (int i = 0; i < 4; i++)
                REQUIRE(dats(i).name() == "w[" + std::to_string(i) + "]");

            dats.name("");
            for (int i = 0; i < 4; i++)
                REQUIRE(dats(i).name()[0] == 'P');
        }
#    endif
    }

    SECTION("int_vector_dim")
    {
        std::vector<size_t> dim{4};

        WHEN("size")
        {
            auto dats = coek::data("v", dim);
            REQUIRE(dats.size() == 4);
        }

        WHEN("typeof")
        {
            auto dats = coek::data(dim);
            REQUIRE(typeid(dats(1)).name() == typeid(coek::Expression).name());
        }

        WHEN("index")
        {
            auto dats = coek::data(dim).value(1);
            for (size_t i = 0; i < 4; i++)
                REQUIRE(dats(i).value() == 1);
        }

#    if 0
        WHEN("name")
        {
            auto dats = coek::data(dim).name("v");
            for (int i = 0; i < 4; i++)
                REQUIRE(dats(i).name() == "v[" + std::to_string(i) + "]");
        }
#    endif
    }

    SECTION("int_initializer_list")
    {
        WHEN("size")
        {
            auto dats = coek::data("v", {4});
            REQUIRE(dats.size() == 4);
        }

        WHEN("typeof")
        {
            auto dats = coek::data({4});
            REQUIRE(typeid(dats(1)).name() == typeid(coek::Expression).name());
        }

        WHEN("index")
        {
            auto dats = coek::data({4}).value(1);
            for (size_t i = 0; i < 4; i++)
                REQUIRE(dats(i).value() == 1);
        }

#    if 0
        WHEN("name")
        {
            auto dats = coek::data({4}).name("v");
            for (int i = 0; i < 4; i++)
                REQUIRE(dats(i).name() == "v[" + std::to_string(i) + "]");
        }
#    endif
    }
}
#endif

#ifdef COEK_WITH_COMPACT_MODEL
TEST_CASE("2D_data_map", "[smoke]")
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
            auto v1 = coek::data(S);
            REQUIRE(v1.size() == 16);
            auto v2 = coek::data("v2", S);
            REQUIRE(v2.size() == 16);
            auto v3 = coek::data_map(S);
            REQUIRE(v3.size() == 16);
            auto v4 = coek::data_map("v4", S);
            REQUIRE(v4.size() == 16);
        }

        WHEN("size")
        {
            auto V = coek::SetOf(v);
            auto W = coek::SetOf(w);
            auto S = V * W;
            auto dats = coek::data(S);
            REQUIRE(dats.size() == 16);
        }

        WHEN("typeof")
        {
            auto V = coek::SetOf(v);
            auto W = coek::SetOf(w);
            auto S = V * W;
            auto dats = coek::data(S);
            REQUIRE(typeid(dats(1, 2)).name() == typeid(coek::Expression).name());
        }

        WHEN("data_index_error")
        {
            auto V = coek::SetOf(v);
            auto W = coek::SetOf(w);
            auto dats = coek::data(V * W).name("x");
            REQUIRE(dats.dim() == 2);
            REQUIRE_THROWS_WITH(dats(0),
                                "Unexpected index value: x is an 2-D data map but is being "
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
        auto dats = coek::data("dats", S).value(0);

        WHEN("typeof") { REQUIRE(typeid(dats(1, 2)).name() == typeid(coek::Expression).name()); }

        WHEN("index1")
        {
            auto i = coek::set_element("i");
            auto j = coek::set_element("j");
            REQUIRE(typeid(dats(1, 2)).name() == typeid(coek::Expression).name());
            REQUIRE(typeid(dats(10, 11)).name() == typeid(coek::Expression).name());
            REQUIRE(typeid(dats(i, j)).name() == typeid(coek::Expression).name());
        }

#    if 0
        WHEN("index2")
        {
            auto e = dats(1, 2);
            REQUIRE(e.name() == "dats[1,2]");
            // auto it = e.to_list().begin();
            // REQUIRE_THAT( *it, Catch::Matchers::StartsWith("dats") );
        }
#    endif

        WHEN("index3")
        {
            auto i = coek::set_element("i");
            auto j = coek::set_element("j");
            auto e = dats(i, j);
            static std::list<std::string> baseline = {"dats[i,j]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("index4")
        {
            auto i = coek::set_element("i");
            auto j = coek::set_element("j");
            auto e = dats(i + 1, j - 1);
            static std::list<std::string> baseline = {"dats[i + 1,j + -1]"};
            REQUIRE(e.to_list() == baseline);
        }
    }
}
#endif

#if __cpp_lib_variant
TEST_CASE("2D_data_array", "[smoke]")
{
    SECTION("int_vector_dim")
    {
        WHEN("constructors")
        {
            auto v1 = coek::data({4, 4});
            REQUIRE(v1.size() == 16);
            auto v2 = coek::data("v2", {4, 4});
            REQUIRE(v2.size() == 16);
            auto v3 = coek::data_array({4, 4});
            REQUIRE(v3.size() == 16);
            auto v4 = coek::data_array("v4", {4, 4});
            REQUIRE(v4.size() == 16);
        }

        WHEN("size")
        {
            std::vector<size_t> dim{4, 3};
            auto dats = coek::data("v", dim);
            REQUIRE(dats.size() == 12);
        }

        WHEN("typeof")
        {
            std::vector<size_t> dim{4, 3};
            auto dats = coek::data(dim);
            REQUIRE(typeid(dats(1, 1)).name() == typeid(coek::Expression).name());
        }

        WHEN("index")
        {
            std::vector<size_t> dim{4, 5};
            auto dats = coek::data(dim).value(1);
            for (size_t i = 0; i < 4; i++)
                REQUIRE(dats(i, i).value() == 1);
        }

        WHEN("index error")
        {
            std::vector<size_t> dim{4, 3};
            auto dats = coek::data(dim).name("x");
            REQUIRE(dats.dim() == 2);
            REQUIRE_THROWS_WITH(dats(0),
                                "Unexpected index value: x is an 2-D data array but is being "
                                "indexed with 1 indices.");
        }

#    if 0
        WHEN("name")
        {
            std::vector<size_t> dim{4, 3};
            auto dats = coek::data(dim).name("v");
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 3; j++)
                    REQUIRE(dats(i, j).name()
                            == "v[" + std::to_string(i) + "," + std::to_string(j) + "]");
        }
#    endif
    }

    SECTION("int_initializer_list")
    {
        WHEN("size")
        {
            auto dats = coek::data("v", {4, 3});
            REQUIRE(dats.size() == 12);
        }

        WHEN("typeof")
        {
            auto dats = coek::data({4, 3});
            REQUIRE(typeid(dats(1, 1)).name() == typeid(coek::Expression).name());
        }

        WHEN("index")
        {
            auto dats = coek::data({4, 3}).value(1);
            for (size_t i = 0; i < 3; i++)
                REQUIRE(dats(i, i).value() == 1);
        }

        WHEN("index error")
        {
            auto dats = coek::data({4, 3}).name("x");
            REQUIRE(dats.dim() == 2);
            REQUIRE_THROWS_WITH(dats(0),
                                "Unexpected index value: x is an 2-D data array but is being "
                                "indexed with 1 indices.");
        }

#    if 0
        WHEN("name")
        {
            auto dats = coek::data({4, 3}).name("v");
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 3; j++)
                    REQUIRE(dats(i, j).name()
                            == "v[" + std::to_string(i) + "," + std::to_string(j) + "]");
        }
#    endif
    }
}

TEST_CASE("3D_data_array", "[smoke]")
{
    SECTION("int_vector_dim")
    {
        WHEN("constructors")
        {
            auto v1 = coek::data({5, 4, 3});
            REQUIRE(v1.size() == 60);
            auto v2 = coek::data("v2", {5, 4, 3});
            REQUIRE(v2.size() == 60);
            auto v3 = coek::data({5, 4, 3});
            REQUIRE(v3.size() == 60);
            auto v4 = coek::data("v4", {5, 4, 3});
            REQUIRE(v4.size() == 60);
        }

        WHEN("size")
        {
            std::vector<size_t> dim{5, 4, 3};
            auto dats = coek::data("v", dim);
            REQUIRE(dats.size() == 60);
        }

        WHEN("typeof")
        {
            std::vector<size_t> dim{5, 4, 3};
            auto dats = coek::data(dim);
            REQUIRE(typeid(dats(1, 1)).name() == typeid(coek::Expression).name());
        }

        WHEN("index")
        {
            std::vector<size_t> dim{5, 4, 3};
            auto dats = coek::data(dim).value(1);
            for (size_t i = 0; i < 3; i++)
                REQUIRE(dats(i, i, i).value() == 1);
        }

        WHEN("index error")
        {
            std::vector<size_t> dim{5, 4, 3};
            auto dats = coek::data(dim).name("x");
            REQUIRE(dats.dim() == 3);
            REQUIRE_THROWS_WITH(dats(0),
                                "Unexpected index value: x is an 3-D data array but is being "
                                "indexed with 1 indices.");
        }

#    if 0
        WHEN("name")
        {
            std::vector<size_t> dim{5, 4, 3};
            auto dats = coek::data(dim).name("v");
            for (int i = 0; i < 5; i++)
                for (int j = 0; j < 4; j++)
                    for (int k = 0; k < 3; k++)
                        REQUIRE(dats(i, j, k).name()
                                == "v[" + std::to_string(i) + "," + std::to_string(j) + ","
                                       + std::to_string(k) + "]");
        }
#    endif
    }

    SECTION("int_initializer_list")
    {
        WHEN("size")
        {
            auto dats = coek::data("v", {5, 4, 3});
            REQUIRE(dats.size() == 60);
        }

        WHEN("typeof")
        {
            auto dats = coek::data({5, 4, 3});
            REQUIRE(typeid(dats(1, 1, 1)).name() == typeid(coek::Expression).name());
        }

        WHEN("index")
        {
            auto dats = coek::data({5, 4, 3}).value(1);
            for (size_t i = 0; i < 3; i++)
                REQUIRE(dats(i, i, i).value() == 1);
        }

        WHEN("index error")
        {
            auto dats = coek::data({5, 4, 3}).name("x");
            REQUIRE(dats.dim() == 3);
            REQUIRE_THROWS_WITH(dats(0),
                                "Unexpected index value: x is an 3-D data array but is being "
                                "indexed with 1 indices.");
        }

#    if 0
        WHEN("name")
        {
            auto dats = coek::data({5, 4, 3}).name("v");
            for (int i = 0; i < 5; i++)
                for (int j = 0; j < 4; j++)
                    for (int k = 0; k < 3; k++)
                        REQUIRE(dats(i, j, k).name()
                                == "v[" + std::to_string(i) + "," + std::to_string(j) + ","
                                       + std::to_string(k) + "]");
        }
#    endif
    }
}
#endif

#ifdef COEK_WITH_COMPACT_MODEL
TEST_CASE("3D_data_api", "[smoke]")
{
    SECTION("map")
    {
        std::vector<int> vec = {1, 2, 3, 4};
        auto p = coek::data("p").value(1);

        WHEN("index api")
        {
            auto V = coek::SetOf(vec);
            auto v = coek::data_map(V * V * V).name("x").value(1);
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
                                "Unexpected index value: x is an 3-D data map but is being "
                                "indexed with 2 indices.");

            REQUIRE_THROWS_WITH(v(ival, ival),
                                "Unexpected index value: x is an 3-D data map but is being "
                                "indexed with 2 indices.");

            REQUIRE_THROWS_WITH(v(e, ival),
                                "Unexpected index value: x is an 3-D data map but is being "
                                "indexed with 2 indices.");

            REQUIRE_THROWS_WITH(v(ival, ival, -1), "Unknown index value: x[1,1,-1]");
        }

        WHEN("data value")
        {
            auto q = coek::data("q").value(1);
            auto V = coek::SetOf(vec);
            auto v = coek::data_map(V).name("x").value(1);

            REQUIRE(v(1).value() == 1);
            v.value(q + 1);
            REQUIRE(v(1).value() == 2);
            q.value(2);
            // Changing the value of q does *not* impact the value of v(1), since
            // this is data and not a parameter.
            REQUIRE(v(1).value() == 2);
        }
    }

    SECTION("array")
    {
        auto p = coek::data("p").value(1);

        WHEN("index api")
        {
            auto v = coek::data_array({4, 4, 4}).name("x").value(1);
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
                                "Unexpected index value: x is an 3-D data array but is being "
                                "indexed with 1 indices.");

            REQUIRE_THROWS_WITH(v(uval, uval),
                                "Unexpected index value: x is an 3-D data array but is being "
                                "indexed with 2 indices.");

            REQUIRE_THROWS_WITH(v(e, ival),
                                "Unexpected index value: x is an 3-D data array but is being "
                                "indexed with 2 indices.");

            REQUIRE_THROWS_WITH(v(ival, ival),
                                "Unexpected index value: x is an 3-D data array but is being "
                                "indexed with 2 indices.");

            REQUIRE_THROWS_WITH(v(100, 100, 100), "Unknown index value: x[100,100,100]");
        }

        WHEN("data value")
        {
            auto q = coek::data("q").value(1);
            auto v = coek::data_array(4).name("x").value(1);

            REQUIRE(v(1).value() == 1);
            v.value(q + 1);
            REQUIRE(v(1).value() == 2);
            q.value(2);
            // Changing the value of q does *not* impact the value of v(1), since
            // this is data and not a parameter.
            REQUIRE(v(1).value() == 2);
        }
    }
}
#endif

#if __cpp_lib_variant
TEST_CASE("ND_data_array_errors", "[smoke]")
{
    SECTION("wrong index values")
    {
        auto p = coek::data("p", {10, 10}).value(1);
        REQUIRE_THROWS_WITH(p(11, 11).value(), "Unknown index value: p[11,11]");
    }
}
#endif
