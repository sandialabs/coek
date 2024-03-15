
#include <iostream>

#include "catch2/catch.hpp"
#include "coek/coek.hpp"

TEST_CASE("range_int", "[smoke]")
{
    SECTION("range_stop")
    {
        std::vector<int> test;
        for (int i : coek::range(4)) test.push_back(i);
        std::vector<int> ans{0, 1, 2, 3};
        REQUIRE(test == ans);
    }

    SECTION("range_start_stop")
    {
        std::vector<int> test;
        for (int i : coek::range(1, 4)) test.push_back(i);
        std::vector<int> ans{1, 2, 3};
        REQUIRE(test == ans);
    }

    SECTION("range_start_stop_step")
    {
        std::vector<int> test;
        for (int i : coek::range(1, 9, 2)) test.push_back(i);
        std::vector<int> ans{1, 3, 5, 7};
        REQUIRE(test == ans);
    }
}

TEST_CASE("sequence_int", "[smoke]")
{
    SECTION("sequence_stop")
    {
        std::vector<int> test;
        for (int i : coek::sequence(4)) test.push_back(i);
        std::vector<int> ans{1, 2, 3, 4};
        REQUIRE(test == ans);
    }

    SECTION("sequence_start_stop")
    {
        std::vector<int> test;
        for (int i : coek::sequence(2, 4)) test.push_back(i);
        std::vector<int> ans{2, 3, 4};
        REQUIRE(test == ans);
    }

    SECTION("sequence_start_stop_step")
    {
        std::vector<int> test;
        for (int i : coek::sequence(1, 9, 2)) test.push_back(i);
        std::vector<int> ans{1, 3, 5, 7, 9};
        REQUIRE(test == ans);
    }
}

TEST_CASE("range_size_t", "[smoke]")
{
    SECTION("range_stop")
    {
        std::vector<size_t> test;
        for (size_t i : coek::range<size_t>(4)) test.push_back(i);
        std::vector<size_t> ans{0, 1, 2, 3};
        REQUIRE(test == ans);
    }

    SECTION("range_start_stop")
    {
        std::vector<size_t> test;
        for (size_t i : coek::range<size_t>(1, 4)) test.push_back(i);
        std::vector<size_t> ans{1, 2, 3};
        REQUIRE(test == ans);
    }

    SECTION("range_start_stop_step")
    {
        std::vector<size_t> test;
        for (size_t i : coek::range<size_t>(1, 9, 2)) test.push_back(i);
        std::vector<size_t> ans{1, 3, 5, 7};
        REQUIRE(test == ans);
    }
}

TEST_CASE("sequence_size_t", "[smoke]")
{
    SECTION("sequence_stop")
    {
        std::vector<size_t> test;
        for (size_t i : coek::sequence<size_t>(4)) test.push_back(i);
        std::vector<size_t> ans{1, 2, 3, 4};
        REQUIRE(test == ans);
    }

    SECTION("sequence_start_stop")
    {
        std::vector<size_t> test;
        for (size_t i : coek::sequence<size_t>(2, 4)) test.push_back(i);
        std::vector<size_t> ans{2, 3, 4};
        REQUIRE(test == ans);
    }

    SECTION("sequence_start_stop_step")
    {
        std::vector<size_t> test;
        for (size_t i : coek::sequence<size_t>(1, 9, 2)) test.push_back(i);
        std::vector<size_t> ans{1, 3, 5, 7, 9};
        REQUIRE(test == ans);
    }
}

TEST_CASE("containers", "[smoke]")
{
    SECTION("vector")
    {
        std::vector<size_t> test;
        std::vector<size_t> obj{1, 2, 3, 4};
        for (auto i : coek::indices(obj)) test.push_back(i);
        std::vector<size_t> ans{0, 1, 2, 3};
        REQUIRE(test == ans);
    }

    SECTION("list")
    {
        std::vector<size_t> test;
        std::list<size_t> obj{1, 2, 3, 4};
        for (auto i : coek::indices(obj)) test.push_back(i);
        std::vector<size_t> ans{0, 1, 2, 3};
        REQUIRE(test == ans);
    }

    SECTION("initializer_list")
    {
        std::vector<size_t> test;
        for (auto i : coek::indices({0, 3})) test.push_back(i);
        std::vector<size_t> ans{0, 1};
        REQUIRE(test == ans);
    }
}
