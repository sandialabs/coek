
#include <cmath>
#include <iostream>
#include <memory>
#include <sstream>

#include "catch2/catch.hpp"
#include "coek/ast/base_terms.hpp"
#include "coek/ast/value_terms.hpp"
#include "coek/ast/expr_terms.hpp"
#include "coek/coek.hpp"

const double PI = 3.141592653589793238463;
const double E = exp(1.0);

TEST_CASE("elementary_expression", "[smoke]")
{
    SECTION("empty")
    {
        auto e = coek::expression();
        auto x = coek::variable("z", 3).generate_names();
        for (size_t i = 0; i < 3; i++) e += x(i);
        static std::list<std::string> baseline
            = {"[", "+", std::to_string(0.0), "z[0]", "z[1]", "z[2]", "]"};
        REQUIRE(e.to_list() == baseline);
    }

    SECTION("constant integer")
    {
        auto e = coek::expression(1);
        auto x = coek::variable("x", 3).generate_names();
        for (size_t i = 0; i < 3; i++) e += x(i);
        static std::list<std::string> baseline
            = {"[", "+", std::to_string(1.0), "x[0]", "x[1]", "x[2]", "]"};
        REQUIRE(e.to_list() == baseline);
    }

    SECTION("constant double")
    {
        auto e = coek::expression(1.3);
        auto x = coek::variable("x", 3).generate_names();
        for (size_t i = 0; i < 3; i++) e += x(i);
        static std::list<std::string> baseline
            = {"[", "+", std::to_string(1.300), "x[0]", "x[1]", "x[2]", "]"};
        REQUIRE(e.to_list() == baseline);
    }

    SECTION("param")
    {
        auto p = coek::parameter("p");
        auto e = coek::expression(p);
        auto x = coek::variable("x", 3).generate_names();
        for (size_t i = 0; i < 3; i++) e += x(i);
        static std::list<std::string> baseline = {"[", "+", "p", "x[0]", "x[1]", "x[2]", "]"};
        REQUIRE(e.to_list() == baseline);
    }

    SECTION("var")
    {
        auto y = coek::variable("y");
        auto e = coek::expression(y);
        auto x = coek::variable("x", 3).generate_names();
        for (size_t i = 0; i < 3; i++) e += x(i);
        static std::list<std::string> baseline = {"[", "+", "y", "x[0]", "x[1]", "x[2]", "]"};
        REQUIRE(e.to_list() == baseline);
    }
}

TEST_CASE("elementary_subexpression", "[smoke]")
{
    SECTION("empty")
    {
        auto e = coek::subexpression("e");
        auto x = coek::variable("z", 3).generate_names();
        for (size_t i = 0; i < 3; i++) e += x(i);
        static std::list<std::string> baseline
            = {"[", "_", "[", "+", std::to_string(0.0), "z[0]", "z[1]", "z[2]", "]", "]"};
        REQUIRE(e.to_list() == baseline);
    }

    SECTION("constant integer")
    {
        auto e = coek::subexpression("e").value(1);
        auto x = coek::variable("x", 3).generate_names();
        for (size_t i = 0; i < 3; i++) e += x(i);
        static std::list<std::string> baseline
            = {"[", "_", "[", "+", std::to_string(1.0), "x[0]", "x[1]", "x[2]", "]", "]"};
        REQUIRE(e.to_list() == baseline);
    }

    SECTION("constant double")
    {
        auto e = coek::subexpression("e").value(1.3);
        auto x = coek::variable("x", 3).generate_names();
        for (size_t i = 0; i < 3; i++) e += x(i);
        static std::list<std::string> baseline
            = {"[", "_", "[", "+", std::to_string(1.300), "x[0]", "x[1]", "x[2]", "]", "]"};
        REQUIRE(e.to_list() == baseline);
    }

    SECTION("param")
    {
        auto p = coek::parameter("p");
        auto e = coek::subexpression("e").value(p);
        auto x = coek::variable("x", 3).generate_names();
        for (size_t i = 0; i < 3; i++) e += x(i);
        static std::list<std::string> baseline
            = {"[", "_", "[", "+", "p", "x[0]", "x[1]", "x[2]", "]", "]"};
        REQUIRE(e.to_list() == baseline);
    }

    SECTION("var")
    {
        auto y = coek::variable("y");
        auto e = coek::subexpression("e").value(y);
        auto x = coek::variable("x", 3).generate_names();
        for (size_t i = 0; i < 3; i++) e += x(i);
        static std::list<std::string> baseline
            = {"[", "_", "[", "+", "y", "x[0]", "x[1]", "x[2]", "]", "]"};
        REQUIRE(e.to_list() == baseline);
    }
}

TEST_CASE("model_monomial", "[smoke]")
{
    SECTION("trivial - int*variable")
    {
        auto v = coek::variable("v").lower(0).upper(0).value(0);
        coek::Expression e = 1 * v;
        REQUIRE(e.repn == v.repn);
        static std::list<std::string> baseline = {"v"};
        REQUIRE(e.to_list() == baseline);
    }

    SECTION("simple monomial - int*variable")
    {
        auto v = coek::variable("v").lower(0).upper(0).value(0);
        coek::Expression e = 2 * v;
        static std::list<std::string> baseline = {"[", "*", "2", "v", "]"};
        REQUIRE(e.to_list() == baseline);
    }

    SECTION("trivial - int*parameter")
    {
        auto v = coek::parameter("v").value(1);
        coek::Expression e = 1 * v;
        // REQUIRE( e.repn == v.repn );
        static std::list<std::string> baseline = {"v"};
        REQUIRE(e.to_list() == baseline);
    }

    SECTION("simple monomial - int*parameter")
    {
        auto v = coek::parameter("v").value(1);
        coek::Expression e = 2 * v;
        static std::list<std::string> baseline = {"[", "*", std::to_string(2.0), "v", "]"};
        REQUIRE(e.to_list() == baseline);
    }

    SECTION("trivial - double*variable")
    {
        auto v = coek::variable("v").lower(0).upper(0).value(0);
        coek::Expression e = 1.0 * v;
        REQUIRE(e.repn == v.repn);
        static std::list<std::string> baseline = {"v"};
        REQUIRE(e.to_list() == baseline);
    }

    SECTION("simple monomial - double*variable")
    {
        auto v = coek::variable("v").lower(0).upper(0).value(0);
        coek::Expression e = 2.0 * v;
        static std::list<std::string> baseline = {"[", "*", "2", "v", "]"};
        REQUIRE(e.to_list() == baseline);
    }

    SECTION("trivial - double*parameter")
    {
        auto v = coek::parameter("v").value(1);
        coek::Expression e = 1.0 * v;
        // REQUIRE( e.repn == v.repn );
        static std::list<std::string> baseline = {"v"};
        REQUIRE(e.to_list() == baseline);
    }

    SECTION("simple monomial - int*parameter")
    {
        auto v = coek::parameter("v").value(1);
        coek::Expression e = 2.0 * v;
        static std::list<std::string> baseline = {"[", "*", std::to_string(2.0), "v", "]"};
        REQUIRE(e.to_list() == baseline);
    }

    /*
      SECTION( "unnamed monomial" ) {
        auto v = coek::variable("v").lower(0).upper(0).value(0);
        coek::Expression e = 2*v;
        static std::list<std::string> baseline = {"[", "*", "2", "x0", "]"};
        REQUIRE( e.to_list() == baseline );
      }
    */
}

TEST_CASE("model_expression", "[smoke]")
{
    SECTION("constructors")
    {
        WHEN("simple")
        {
            coek::Expression a;
            static std::list<std::string> baseline = {std::to_string(0.0)};
            REQUIRE(a.to_list() == baseline);
        }

        WHEN("double")
        {
            coek::Expression a(1.0);
            static std::list<std::string> baseline = {std::to_string(1.0)};
            REQUIRE(a.to_list() == baseline);
        }

        WHEN("equal")
        {
            coek::Expression a(1.0);
            coek::Expression b;
            b = a;
            static std::list<std::string> baseline = {std::to_string(1.0)};
            REQUIRE(a.to_list() == baseline);
        }
    }

    SECTION("is_constant")
    {
        WHEN("constant")
        {
            coek::Expression a(1.0);
            REQUIRE(a.is_constant() == true);
        }

        WHEN("parameter")
        {
            auto p = coek::parameter().value(1);
            coek::Expression a;
            a = p;
            REQUIRE(a.is_constant() == false);
        }

        WHEN("index_parameter")
        {
            auto p = coek::set_element("p");
            coek::Expression a;
            a = p;
            REQUIRE(a.is_constant() == false);
        }

        WHEN("variable")
        {
            auto p = coek::variable().lower(0).upper(0).value(0);
            coek::Expression a;
            a = p;
            REQUIRE(a.is_constant() == false);
        }
    }

    SECTION("plus-equal")
    {
        WHEN("variable")
        {
            auto v = coek::variable("v");
            auto p = coek::variable("p").lower(0).upper(0).value(0);
            coek::Expression a = v;
            a += p;
            static std::list<std::string> baseline = {"[", "+", "v", "p", "]"};
            REQUIRE(a.to_list() == baseline);
        }
        WHEN("double")
        {
            auto v = coek::variable("v");
            coek::Expression a = v;
            double p = 1;
            a += p;
            static std::list<std::string> baseline = {"[", "+", "v", std::to_string(1.0), "]"};
            REQUIRE(a.to_list() == baseline);
        }
        WHEN("int")
        {
            auto v = coek::variable("v");
            coek::Expression a = v;
            int p = 1;
            a += p;
            static std::list<std::string> baseline = {"[", "+", "v", std::to_string(1.0), "]"};
            REQUIRE(a.to_list() == baseline);
        }
        WHEN("parameter")
        {
            auto v = coek::variable("v");
            coek::Expression a = v;
            auto p = coek::parameter("p");
            a += p;
            static std::list<std::string> baseline = {"[", "+", "v", "p", "]"};
            REQUIRE(a.to_list() == baseline);
        }
        WHEN("index parameter")
        {
            auto v = coek::variable("v");
            coek::Expression a = v;
            auto p = coek::set_element("p");
            a += p;
            static std::list<std::string> baseline = {"[", "+", "v", "p", "]"};
            REQUIRE(a.to_list() == baseline);
        }
    }

    SECTION("minus-equal")
    {
        WHEN("variable")
        {
            auto v = coek::variable("v");
            auto p = coek::variable("p").lower(0).upper(1).lower(0);
            coek::Expression a = v;
            a -= p;
            static std::list<std::string> baseline = {"[", "+", "v", "[", "-", "p", "]", "]"};
            REQUIRE(a.to_list() == baseline);
        }
        WHEN("double")
        {
            auto v = coek::variable("v");
            coek::Expression a = v;
            double p = 1;
            a -= p;
            static std::list<std::string> baseline
                = {"[", "+", "v", "[", "-", std::to_string(1.0), "]", "]"};
            REQUIRE(a.to_list() == baseline);
        }
        WHEN("int")
        {
            auto v = coek::variable("v");
            coek::Expression a = v;
            int p = 1;
            a -= p;
            static std::list<std::string> baseline
                = {"[", "+", "v", "[", "-", std::to_string(1.0), "]", "]"};
            REQUIRE(a.to_list() == baseline);
        }
        WHEN("parameter")
        {
            auto v = coek::variable("v");
            coek::Expression a = v;
            auto p = coek::parameter("p");
            a -= p;
            static std::list<std::string> baseline = {"[", "+", "v", "[", "-", "p", "]", "]"};
            REQUIRE(a.to_list() == baseline);
        }
        WHEN("index parameter")
        {
            auto v = coek::variable("v");
            coek::Expression a = v;
            auto p = coek::set_element("p");
            a -= p;
            static std::list<std::string> baseline = {"[", "+", "v", "[", "-", "p", "]", "]"};
            REQUIRE(a.to_list() == baseline);
        }
    }

    SECTION("times-equal")
    {
        WHEN("variable")
        {
            auto v = coek::variable("v");
            auto p = coek::variable("p").lower(0).upper(1).value(0);
            coek::Expression a = v;
            a *= p;
            static std::list<std::string> baseline = {"[", "*", "v", "p", "]"};
            REQUIRE(a.to_list() == baseline);
        }
        WHEN("double")
        {
            auto v = coek::variable("v");
            coek::Expression a = v;
            double p = 1;
            a *= p;
            static std::list<std::string> baseline = {"[", "*", "v", std::to_string(1.0), "]"};
            REQUIRE(a.to_list() == baseline);
        }
        WHEN("int")
        {
            auto v = coek::variable("v");
            coek::Expression a = v;
            int p = 1;
            a *= p;
            static std::list<std::string> baseline = {"[", "*", "v", std::to_string(1.0), "]"};
            REQUIRE(a.to_list() == baseline);
        }
        WHEN("parameter")
        {
            auto v = coek::variable("v");
            coek::Expression a = v;
            auto p = coek::parameter("p");
            a *= p;
            static std::list<std::string> baseline = {"[", "*", "v", "p", "]"};
            REQUIRE(a.to_list() == baseline);
        }
        WHEN("index parameter")
        {
            auto v = coek::variable("v");
            coek::Expression a = v;
            auto p = coek::set_element("p");
            a *= p;
            static std::list<std::string> baseline = {"[", "*", "v", "p", "]"};
            REQUIRE(a.to_list() == baseline);
        }
    }

    SECTION("divide-equal")
    {
        WHEN("variable")
        {
            auto v = coek::variable("v");
            auto p = coek::variable("p").lower(0).upper(1).value(0);
            coek::Expression a = v;
            a /= p;
            static std::list<std::string> baseline = {"[", "/", "v", "p", "]"};
            REQUIRE(a.to_list() == baseline);
        }
        WHEN("double")
        {
            auto v = coek::variable("v");
            coek::Expression a = v;
            double p = 1;
            a /= p;
            static std::list<std::string> baseline = {"[", "/", "v", std::to_string(1.0), "]"};
            REQUIRE(a.to_list() == baseline);
        }
        WHEN("int")
        {
            auto v = coek::variable("v");
            coek::Expression a = v;
            int p = 1;
            a /= p;
            static std::list<std::string> baseline = {"[", "/", "v", std::to_string(1.0), "]"};
            REQUIRE(a.to_list() == baseline);
        }
        WHEN("parameter")
        {
            auto v = coek::variable("v");
            coek::Expression a = v;
            auto p = coek::parameter("p");
            a /= p;
            static std::list<std::string> baseline = {"[", "/", "v", "p", "]"};
            REQUIRE(a.to_list() == baseline);
        }
        WHEN("index parameter")
        {
            auto v = coek::variable("v");
            coek::Expression a = v;
            auto p = coek::set_element("p");
            a /= p;
            static std::list<std::string> baseline = {"[", "/", "v", "p", "]"};
            REQUIRE(a.to_list() == baseline);
        }
    }

    SECTION("collect_terms")
    {
        coek::QuadraticExpr repn;
        coek::Model m;
        auto v = coek::variable("v").lower(0).upper(0).value(0);
        coek::Expression a = 1 + v + v * v;

        repn.collect_terms(a);
        std::stringstream sstr;
        sstr << repn;
        REQUIRE(sstr.str() == "Constant: 1\nLinear: \n1 v\nQuadratic: \n1 v v\n");
    }
}

TEST_CASE("model_subexpression", "[smoke]")
{
    SECTION("constructors")
    {
        WHEN("simple")
        {
            coek::SubExpression a;
            static std::list<std::string> baseline = {"[", "_", std::to_string(0.0), "]"};
            REQUIRE(a.to_list() == baseline);
        }

        WHEN("double")
        {
            coek::SubExpression a(1.0);
            static std::list<std::string> baseline = {"[", "_", std::to_string(1.0), "]"};
            REQUIRE(a.to_list() == baseline);
        }

        WHEN("equal")
        {
            coek::SubExpression a(1.0);
            coek::SubExpression b;
            b = a;
            static std::list<std::string> baseline = {"[", "_", std::to_string(1.0), "]"};
            REQUIRE(a.to_list() == baseline);
        }
    }

    SECTION("is_constant")
    {
        WHEN("constant")
        {
            coek::SubExpression a(1.0);
            REQUIRE(a.is_constant() == false);
            REQUIRE(a.repn->body->is_constant() == true);
        }

        WHEN("parameter")
        {
            auto p = coek::parameter().value(1);
            coek::SubExpression a;
            a = p;
            REQUIRE(a.is_constant() == false);
        }

        WHEN("index_parameter")
        {
            auto p = coek::set_element("p");
            coek::SubExpression a;
            a = p;
            REQUIRE(a.is_constant() == false);
        }

        WHEN("variable")
        {
            auto p = coek::variable().lower(0).upper(0).value(0);
            coek::SubExpression a;
            a = p;
            REQUIRE(a.is_constant() == false);
        }
    }

    SECTION("plus-equal")
    {
        WHEN("variable")
        {
            auto v = coek::variable("v");
            auto p = coek::variable("p").lower(0).upper(0).value(0);
            coek::SubExpression a = v;
            a += p;
            static std::list<std::string> baseline = {"[", "_", "[", "+", "v", "p", "]", "]"};
            REQUIRE(a.to_list() == baseline);
        }
        WHEN("double")
        {
            auto v = coek::variable("v");
            coek::SubExpression a = v;
            double p = 1;
            a += p;
            static std::list<std::string> baseline
                = {"[", "_", "[", "+", "v", std::to_string(1.0), "]", "]"};
            REQUIRE(a.to_list() == baseline);
        }
        WHEN("int")
        {
            auto v = coek::variable("v");
            coek::SubExpression a = v;
            int p = 1;
            a += p;
            static std::list<std::string> baseline
                = {"[", "_", "[", "+", "v", std::to_string(1.0), "]", "]"};
            REQUIRE(a.to_list() == baseline);
        }
        WHEN("parameter")
        {
            auto v = coek::variable("v");
            coek::SubExpression a = v;
            auto p = coek::parameter("p");
            a += p;
            static std::list<std::string> baseline = {"[", "_", "[", "+", "v", "p", "]", "]"};
            REQUIRE(a.to_list() == baseline);
        }
        WHEN("index parameter")
        {
            auto v = coek::variable("v");
            coek::SubExpression a = v;
            auto p = coek::set_element("p");
            a += p;
            static std::list<std::string> baseline = {"[", "_", "[", "+", "v", "p", "]", "]"};
            REQUIRE(a.to_list() == baseline);
        }
    }

    SECTION("minus-equal")
    {
        WHEN("variable")
        {
            auto v = coek::variable("v");
            auto p = coek::variable("p").lower(0).upper(1).lower(0);
            coek::SubExpression a = v;
            a -= p;
            static std::list<std::string> baseline
                = {"[", "_", "[", "+", "v", "[", "-", "p", "]", "]", "]"};
            REQUIRE(a.to_list() == baseline);
        }
        WHEN("double")
        {
            auto v = coek::variable("v");
            coek::SubExpression a = v;
            double p = 1;
            a -= p;
            static std::list<std::string> baseline
                = {"[", "_", "[", "+", "v", "[", "-", std::to_string(1.0), "]", "]", "]"};
            REQUIRE(a.to_list() == baseline);
        }
        WHEN("int")
        {
            auto v = coek::variable("v");
            coek::SubExpression a = v;
            int p = 1;
            a -= p;
            static std::list<std::string> baseline
                = {"[", "_", "[", "+", "v", "[", "-", std::to_string(1.0), "]", "]", "]"};
            REQUIRE(a.to_list() == baseline);
        }
        WHEN("parameter")
        {
            auto v = coek::variable("v");
            coek::SubExpression a = v;
            auto p = coek::parameter("p");
            a -= p;
            static std::list<std::string> baseline
                = {"[", "_", "[", "+", "v", "[", "-", "p", "]", "]", "]"};
            REQUIRE(a.to_list() == baseline);
        }
        WHEN("index parameter")
        {
            auto v = coek::variable("v");
            coek::SubExpression a = v;
            auto p = coek::set_element("p");
            a -= p;
            static std::list<std::string> baseline
                = {"[", "_", "[", "+", "v", "[", "-", "p", "]", "]", "]"};
            REQUIRE(a.to_list() == baseline);
        }
    }

    SECTION("times-equal")
    {
        WHEN("variable")
        {
            auto v = coek::variable("v");
            auto p = coek::variable("p").lower(0).upper(1).value(0);
            coek::SubExpression a = v;
            a *= p;
            static std::list<std::string> baseline = {"[", "_", "[", "*", "v", "p", "]", "]"};
            REQUIRE(a.to_list() == baseline);
        }
        WHEN("double")
        {
            auto v = coek::variable("v");
            coek::SubExpression a = v;
            double p = 1;
            a *= p;
            static std::list<std::string> baseline
                = {"[", "_", "[", "*", "v", std::to_string(1.0), "]", "]"};
            REQUIRE(a.to_list() == baseline);
        }
        WHEN("int")
        {
            auto v = coek::variable("v");
            coek::SubExpression a = v;
            int p = 1;
            a *= p;
            static std::list<std::string> baseline
                = {"[", "_", "[", "*", "v", std::to_string(1.0), "]", "]"};
            REQUIRE(a.to_list() == baseline);
        }
        WHEN("parameter")
        {
            auto v = coek::variable("v");
            coek::SubExpression a = v;
            auto p = coek::parameter("p");
            a *= p;
            static std::list<std::string> baseline = {"[", "_", "[", "*", "v", "p", "]", "]"};
            REQUIRE(a.to_list() == baseline);
        }
        WHEN("index parameter")
        {
            auto v = coek::variable("v");
            coek::SubExpression a = v;
            auto p = coek::set_element("p");
            a *= p;
            static std::list<std::string> baseline = {"[", "_", "[", "*", "v", "p", "]", "]"};
            REQUIRE(a.to_list() == baseline);
        }
    }

    SECTION("divide-equal")
    {
        WHEN("variable")
        {
            auto v = coek::variable("v");
            auto p = coek::variable("p").lower(0).upper(1).value(0);
            coek::SubExpression a = v;
            a /= p;
            static std::list<std::string> baseline = {"[", "_", "[", "/", "v", "p", "]", "]"};
            REQUIRE(a.to_list() == baseline);
        }
        WHEN("double")
        {
            auto v = coek::variable("v");
            coek::SubExpression a = v;
            double p = 1;
            a /= p;
            static std::list<std::string> baseline
                = {"[", "_", "[", "/", "v", std::to_string(1.0), "]", "]"};
            REQUIRE(a.to_list() == baseline);
        }
        WHEN("int")
        {
            auto v = coek::variable("v");
            coek::SubExpression a = v;
            int p = 1;
            a /= p;
            static std::list<std::string> baseline
                = {"[", "_", "[", "/", "v", std::to_string(1.0), "]", "]"};
            REQUIRE(a.to_list() == baseline);
        }
        WHEN("parameter")
        {
            auto v = coek::variable("v");
            coek::SubExpression a = v;
            auto p = coek::parameter("p");
            a /= p;
            static std::list<std::string> baseline = {"[", "_", "[", "/", "v", "p", "]", "]"};
            REQUIRE(a.to_list() == baseline);
        }
        WHEN("index parameter")
        {
            auto v = coek::variable("v");
            coek::SubExpression a = v;
            auto p = coek::set_element("p");
            a /= p;
            static std::list<std::string> baseline = {"[", "_", "[", "/", "v", "p", "]", "]"};
            REQUIRE(a.to_list() == baseline);
        }
    }

    SECTION("collect_terms")
    {
        coek::QuadraticExpr repn;
        coek::Model m;
        auto v = coek::variable("v").lower(0).upper(0).value(0);
        coek::SubExpression a = 1 + v + v * v;

        repn.collect_terms(a);
        std::stringstream sstr;
        sstr << repn;
        REQUIRE(sstr.str() == "Constant: 1\nLinear: \n1 v\nQuadratic: \n1 v v\n");
    }
}

TEST_CASE("model_unary_expression", "[smoke]")
{
    SECTION("positive")
    {
        WHEN("param")
        {
            auto q = coek::parameter("q").value(1);
            coek::Expression e;
            e = +q;
            static std::list<std::string> baseline = {"q"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("iparam")
        {
            auto q = coek::set_element("q");
            coek::Expression e;
            e = +q;
            static std::list<std::string> baseline = {"q"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("var")
        {
            auto v = coek::variable("v").lower(0).upper(1).value(0);
            coek::Expression e;
            e = +v;
            static std::list<std::string> baseline = {"v"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("expr")
        {
            coek::Expression E(-1);
            coek::Expression e;
            e = +E;
            static std::list<std::string> baseline = {std::to_string(-1.0)};
            REQUIRE(e.to_list() == baseline);
        }
    }

    SECTION("negate")
    {
        WHEN("param")
        {
            auto q = coek::parameter("q").value(1);
            coek::Expression e;
            e = -q;
            static std::list<std::string> baseline = {"[", "-", "q", "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.value() == -1);
        }

        WHEN("iparam")
        {
            auto q = coek::set_element("q");
            coek::Expression e;
            e = -q;
            static std::list<std::string> baseline = {"[", "-", "q", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("var")
        {
            auto v = coek::variable("v").lower(0).upper(1).value(2);
            coek::Expression e;
            e = -v;
            static std::list<std::string> baseline = {"[", "*", "-1", "v", "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.value() == -2);
        }

        WHEN("expr")
        {
            coek::Expression E(-1);
            coek::Expression e;
            e = -E;
            static std::list<std::string> baseline = {std::to_string(1.0)};
            REQUIRE(e.to_list() == baseline);
        }
    }
}

TEST_CASE("model_add_expression", "[smoke]")
{
    {
        auto a = coek::variable("a").lower(0).upper(1).value(3);
        auto b = coek::variable("b").lower(0).upper(1).value(5);
        auto c = coek::variable("c").lower(0).upper(1).value(0);
        auto d = coek::variable("d").lower(0).upper(1).value(0);
        auto z = coek::parameter("z");
        auto Z = coek::set_element("Z");
        coek::Expression E = b;

        SECTION("Test simple")
        {
            WHEN("e = a + b")
            {
                coek::Expression e = a + b;

                static std::list<std::string> baseline = {"[", "+", "a", "b", "]"};
                REQUIRE(e.to_list() == baseline);
                REQUIRE(e.value() == 8);
            }

            WHEN("e = a+b + 2*a")
            {
                coek::Expression e1 = a + b;
                coek::Expression e2 = 2 * a;
                coek::Expression e = e1 + e2;

                static std::list<std::string> baseline
                    = {"[", "+", "a", "b", "[", "*", "2", "a", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }
        }

        SECTION("Test const + value")
        {
            WHEN("e = 5 + a")
            {
                coek::Expression e = 5 + a;

                static std::list<std::string> baseline = {"[", "+", std::to_string(5.0), "a", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 5.0 + a")
            {
                coek::Expression e = 5.0 + a;

                static std::list<std::string> baseline = {"[", "+", std::to_string(5.0), "a", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 5 + z")
            {
                coek::Expression e = 5 + z;

                static std::list<std::string> baseline = {"[", "+", std::to_string(5.0), "z", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 5.0 + z")
            {
                coek::Expression e = 5.0 + z;

                static std::list<std::string> baseline = {"[", "+", std::to_string(5.0), "z", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 5 + Z")
            {
                coek::Expression e = 5 + Z;

                static std::list<std::string> baseline = {"[", "+", std::to_string(5.0), "Z", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 5.0 + Z")
            {
                coek::Expression e = 5.0 + Z;

                static std::list<std::string> baseline = {"[", "+", std::to_string(5.0), "Z", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 5 + E")
            {
                coek::Expression e = 5 + E;

                static std::list<std::string> baseline = {"[", "+", std::to_string(5.0), "b", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 5.0 + E")
            {
                coek::Expression e = 5.0 + E;

                static std::list<std::string> baseline = {"[", "+", std::to_string(5.0), "b", "]"};
                REQUIRE(e.to_list() == baseline);
            }
        }

        SECTION("Test var + value")
        {
            WHEN("e = a + 5")
            {
                coek::Expression e = a + 5;

                static std::list<std::string> baseline = {"[", "+", "a", std::to_string(5.0), "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = a + 5.0")
            {
                coek::Expression e = a + 5.0;

                static std::list<std::string> baseline = {"[", "+", "a", std::to_string(5.0), "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = a + a")
            {
                coek::Expression e = a + a;

                static std::list<std::string> baseline = {"[", "+", "a", "a", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = a + z")
            {
                coek::Expression e = a + z;

                static std::list<std::string> baseline = {"[", "+", "a", "z", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = a + Z")
            {
                coek::Expression e = a + Z;

                static std::list<std::string> baseline = {"[", "+", "a", "Z", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = a + E")
            {
                coek::Expression e = a + E;

                static std::list<std::string> baseline = {"[", "+", "a", "b", "]"};
                REQUIRE(e.to_list() == baseline);
            }
        }

        SECTION("Test param + value")
        {
            WHEN("e = z + 5")
            {
                coek::Expression e = z + 5;

                static std::list<std::string> baseline = {"[", "+", "z", std::to_string(5.0), "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = z + 5.0")
            {
                coek::Expression e = z + 5.0;

                static std::list<std::string> baseline = {"[", "+", "z", std::to_string(5.0), "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = z + a")
            {
                coek::Expression e = z + a;

                static std::list<std::string> baseline = {"[", "+", "z", "a", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = z + z")
            {
                coek::Expression e = z + z;

                static std::list<std::string> baseline = {"[", "+", "z", "z", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = z + Z")
            {
                coek::Expression e = z + Z;

                static std::list<std::string> baseline = {"[", "+", "z", "Z", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = z + E")
            {
                coek::Expression e = z + E;

                static std::list<std::string> baseline = {"[", "+", "z", "b", "]"};
                REQUIRE(e.to_list() == baseline);
            }
        }

        SECTION("Test indexparam + value")
        {
            WHEN("e = Z + 5")
            {
                coek::Expression e = Z + 5;

                static std::list<std::string> baseline = {"[", "+", "Z", std::to_string(5.0), "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = Z + 5.0")
            {
                coek::Expression e = Z + 5.0;

                static std::list<std::string> baseline = {"[", "+", "Z", std::to_string(5.0), "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = Z + a")
            {
                coek::Expression e = Z + a;

                static std::list<std::string> baseline = {"[", "+", "Z", "a", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = Z + z")
            {
                coek::Expression e = Z + z;

                static std::list<std::string> baseline = {"[", "+", "Z", "z", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = Z + Z")
            {
                coek::Expression e = Z + Z;

                static std::list<std::string> baseline = {"[", "+", "Z", "Z", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = Z + E")
            {
                coek::Expression e = Z + E;

                static std::list<std::string> baseline = {"[", "+", "Z", "b", "]"};
                REQUIRE(e.to_list() == baseline);
            }
        }

        SECTION("Test expr + value")
        {
            WHEN("e = E + 5")
            {
                coek::Expression e = E + 5;

                static std::list<std::string> baseline = {"[", "+", "b", std::to_string(5.0), "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = E + 5.0")
            {
                coek::Expression e = E + 5.0;

                static std::list<std::string> baseline = {"[", "+", "b", std::to_string(5.0), "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = E + a")
            {
                coek::Expression e = E + a;

                static std::list<std::string> baseline = {"[", "+", "b", "a", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = E + z")
            {
                coek::Expression e = E + z;

                static std::list<std::string> baseline = {"[", "+", "b", "z", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = E + Z")
            {
                coek::Expression e = E + Z;

                static std::list<std::string> baseline = {"[", "+", "b", "Z", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = E + E")
            {
                coek::Expression e = E + E;

                static std::list<std::string> baseline = {"[", "+", "b", "b", "]"};
                REQUIRE(e.to_list() == baseline);
            }
        }

        SECTION("Test nestedSum")
        {
            WHEN("e = (a+b)+5")
            {
                coek::Expression e = (a + b) + 5;

                static std::list<std::string> baseline
                    = {"[", "+", "a", "b", std::to_string(5.0), "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 5+(a+b)")
            {
                coek::Expression e = 5 + (a + b);

                static std::list<std::string> baseline
                    = {"[", "+", std::to_string(5.0), "[", "+", "a", "b", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = (a+b)+5.0")
            {
                coek::Expression e = (a + b) + 5.0;

                static std::list<std::string> baseline
                    = {"[", "+", "a", "b", std::to_string(5.0), "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 5.0+(a+b)")
            {
                coek::Expression e = 5.0 + (a + b);

                static std::list<std::string> baseline
                    = {"[", "+", std::to_string(5.0), "[", "+", "a", "b", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = (a+b)+c")
            {
                coek::Expression e = (a + b) + c;

                static std::list<std::string> baseline = {"[", "+", "a", "b", "c", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = c+(a+b)")
            {
                coek::Expression e = c + (a + b);

                static std::list<std::string> baseline
                    = {"[", "+", "c", "[", "+", "a", "b", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = (a+b)+(c+d)")
            {
                coek::Expression e = (a + b) + (c + d);

                static std::list<std::string> baseline
                    = {"[", "+", "a", "b", "[", "+", "c", "d", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }
        }

        SECTION("Test nestedSum")
        {
            WHEN("e = 2*(a+b)+c")
            {
                coek::Expression e = 2 * (a + b) + c;

                static std::list<std::string> baseline
                    = {"[", "+", "[", "*", std::to_string(2.0), "[", "+", "a", "b",
                       "]", "]", "c", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 3*(2*(a+b)+c)")
            {
                coek::Expression e = 3 * (2 * (a + b) + c);

                static std::list<std::string> baseline = {"[",
                                                          "*",
                                                          std::to_string(3.0),
                                                          "[",
                                                          "+",
                                                          "[",
                                                          "*",
                                                          std::to_string(2.0),
                                                          "[",
                                                          "+",
                                                          "a",
                                                          "b",
                                                          "]",
                                                          "]",
                                                          "c",
                                                          "]",
                                                          "]"};
                REQUIRE(e.to_list() == baseline);
            }
        }

        SECTION("Test trivialSum")
        {
            auto q = coek::parameter("q").value(1);
            auto Q = coek::parameter("Q");
            auto Z = coek::set_element("Z");
            coek::Expression E = a;

            WHEN("e = 0 + a")
            {
                coek::Expression e = 0 + a;

                static std::list<std::string> baseline = {"a"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 0.0 + a")
            {
                coek::Expression e = 0.0 + a;

                static std::list<std::string> baseline = {"a"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 0 + q")
            {
                coek::Expression e = 0 + q;

                static std::list<std::string> baseline = {"q"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 0.0 + q")
            {
                coek::Expression e = 0.0 + q;

                static std::list<std::string> baseline = {"q"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 0 + Z")
            {
                coek::Expression e = 0 + Z;

                static std::list<std::string> baseline = {"Z"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 0.0 + Z")
            {
                coek::Expression e = 0.0 + Z;

                static std::list<std::string> baseline = {"Z"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 0 + E")
            {
                coek::Expression e = 0 + E;

                static std::list<std::string> baseline = {"a"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 0.0 + E")
            {
                coek::Expression e = 0.0 + E;

                static std::list<std::string> baseline = {"a"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = a + 0")
            {
                coek::Expression e = a + 0;

                static std::list<std::string> baseline = {"a"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = a + 0.0")
            {
                coek::Expression e = a + 0.0;

                static std::list<std::string> baseline = {"a"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = q + 0")
            {
                coek::Expression e = q + 0;

                static std::list<std::string> baseline = {"q"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = q + 0.0")
            {
                coek::Expression e = q + 0.0;

                static std::list<std::string> baseline = {"q"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = Z + 0")
            {
                coek::Expression e = Z + 0;

                static std::list<std::string> baseline = {"Z"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = Z + 0.0")
            {
                coek::Expression e = Z + 0.0;

                static std::list<std::string> baseline = {"Z"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = E + 0")
            {
                coek::Expression e = E + 0;

                static std::list<std::string> baseline = {"a"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = E + 0.0")
            {
                coek::Expression e = E + 0.0;

                static std::list<std::string> baseline = {"a"};
                REQUIRE(e.to_list() == baseline);
            }
        }

        SECTION("Test sum with zero-valued parameter")
        {
            auto q = coek::parameter("q").value(1);
            auto Q = coek::parameter("Q");

            WHEN("e = a + q{0}")
            {
                coek::Expression e = a + q;

                static std::list<std::string> baseline = {"[", "+", "a", "q", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = q{0} + a")
            {
                coek::Expression e = q + a;

                static std::list<std::string> baseline = {"[", "+", "q", "a", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = a + Q{0.0}")
            {
                coek::Expression e = a + Q;

                static std::list<std::string> baseline = {"[", "+", "a", "Q", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = Q{0.0} + a")
            {
                coek::Expression e = Q + a;

                static std::list<std::string> baseline = {"[", "+", "Q", "a", "]"};
                REQUIRE(e.to_list() == baseline);
            }
        }

        SECTION("Test nestedTrivialProduct")
        {
            WHEN("e = a*5 + b")
            {
                coek::Expression e = a * 5 + b;

                static std::list<std::string> baseline
                    = {"[", "+", "[", "*", "5", "a", "]", "b", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = b + a*5")
            {
                coek::Expression e = b + a * 5;

                static std::list<std::string> baseline
                    = {"[", "+", "b", "[", "*", "5", "a", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = a*5 + (b+c)")
            {
                coek::Expression e = a * 5 + (b + c);

                static std::list<std::string> baseline
                    = {"[", "+", "[", "*", "5", "a", "]", "[", "+", "b", "c", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = (b+c) + a*5")
            {
                coek::Expression e = (b + c) + a * 5;

                static std::list<std::string> baseline
                    = {"[", "+", "b", "c", "[", "*", "5", "a", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }
        }
    }
}

TEST_CASE("model_minus_expression", "[smoke]")
{
    {
        auto a = coek::variable("a").lower(0).upper(1).value(3);
        auto b = coek::variable("b").lower(0).upper(1).value(5);
        auto c = coek::variable("c").lower(0).upper(1).value(0);
        auto d = coek::variable("d").lower(0).upper(1).value(0);

        auto z = coek::parameter("z").value(1.0);
        auto Z = coek::set_element("Z");
        coek::Expression f;
        coek::Expression E = b;

        SECTION("Test simpleDiff")
        {
            coek::Expression e = a - b;

            static std::list<std::string> baseline = {"[", "+", "a", "[", "*", "-1", "b", "]", "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.value() == -2);
        }

        SECTION("Test termDiff")
        {
            coek::Expression e = 5 - 2 * a;

            static std::list<std::string> baseline
                = {"[", "+", std::to_string(5.0), "[", "*", "-2", "a", "]", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        SECTION("Test const - value")
        {
            WHEN("e = 5 - a")
            {
                coek::Expression e = 5 - a;

                static std::list<std::string> baseline
                    = {"[", "+", std::to_string(5.0), "[", "*", "-1", "a", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 5.0 - a")
            {
                coek::Expression e = 5.0 - a;

                static std::list<std::string> baseline
                    = {"[", "+", std::to_string(5.0), "[", "*", "-1", "a", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 5 - z")
            {
                coek::Expression e = 5 - z;

                static std::list<std::string> baseline
                    = {"[", "+", std::to_string(5.0), "[", "-", "z", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 5.0 - z")
            {
                coek::Expression e = 5.0 - z;

                static std::list<std::string> baseline
                    = {"[", "+", std::to_string(5.0), "[", "-", "z", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 5 - Z")
            {
                coek::Expression e = 5 - Z;

                static std::list<std::string> baseline
                    = {"[", "+", std::to_string(5.0), "[", "-", "Z", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 5.0 - Z")
            {
                coek::Expression e = 5.0 - Z;

                static std::list<std::string> baseline
                    = {"[", "+", std::to_string(5.0), "[", "-", "Z", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 5 - E")
            {
                coek::Expression e = 5 - E;

                static std::list<std::string> baseline
                    = {"[", "+", std::to_string(5.0), "[", "*", "-1", "b", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 5.0 - E")
            {
                coek::Expression e = 5.0 - E;

                static std::list<std::string> baseline
                    = {"[", "+", std::to_string(5.0), "[", "*", "-1", "b", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }
        }

        SECTION("Test var - value")
        {
            WHEN("e = a - 5")
            {
                coek::Expression e = a - 5;

                static std::list<std::string> baseline = {"[", "+", "a", std::to_string(-5.0), "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = a - 5.0")
            {
                coek::Expression e = a - 5.0;

                static std::list<std::string> baseline = {"[", "+", "a", std::to_string(-5.0), "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = a - a")
            {
                coek::Expression e = a - a;

                static std::list<std::string> baseline
                    = {"[", "+", "a", "[", "*", "-1", "a", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = a - z")
            {
                coek::Expression e = a - z;

                static std::list<std::string> baseline = {"[", "+", "a", "[", "-", "z", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = a - Z")
            {
                coek::Expression e = a - Z;

                static std::list<std::string> baseline = {"[", "+", "a", "[", "-", "Z", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = a - E")
            {
                coek::Expression e = a - E;

                static std::list<std::string> baseline
                    = {"[", "+", "a", "[", "*", "-1", "b", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }
        }

        SECTION("Test param - value")
        {
            WHEN("e = z - 5")
            {
                coek::Expression e = z - 5;

                static std::list<std::string> baseline = {"[", "+", "z", std::to_string(-5.0), "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = z - 5.0")
            {
                coek::Expression e = z - 5.0;

                static std::list<std::string> baseline = {"[", "+", "z", std::to_string(-5.0), "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = z - a")
            {
                coek::Expression e = z - a;

                static std::list<std::string> baseline
                    = {"[", "+", "z", "[", "*", "-1", "a", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = z - z")
            {
                coek::Expression e = z - z;

                static std::list<std::string> baseline = {"[", "+", "z", "[", "-", "z", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = z - Z")
            {
                coek::Expression e = z - Z;

                static std::list<std::string> baseline = {"[", "+", "z", "[", "-", "Z", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = z - E")
            {
                coek::Expression e = z - E;

                static std::list<std::string> baseline
                    = {"[", "+", "z", "[", "*", "-1", "b", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }
        }

        SECTION("Test indexparam - value")
        {
            WHEN("e = Z - 5")
            {
                coek::Expression e = Z - 5;

                static std::list<std::string> baseline = {"[", "+", "Z", std::to_string(-5.0), "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = Z - 5.0")
            {
                coek::Expression e = Z - 5.0;

                static std::list<std::string> baseline = {"[", "+", "Z", std::to_string(-5.0), "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = Z - a")
            {
                coek::Expression e = Z - a;

                static std::list<std::string> baseline
                    = {"[", "+", "Z", "[", "*", "-1", "a", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = Z - z")
            {
                coek::Expression e = Z - z;

                static std::list<std::string> baseline = {"[", "+", "Z", "[", "-", "z", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = Z - Z")
            {
                coek::Expression e = Z - Z;

                static std::list<std::string> baseline = {"[", "+", "Z", "[", "-", "Z", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = Z - E")
            {
                coek::Expression e = Z - E;

                static std::list<std::string> baseline
                    = {"[", "+", "Z", "[", "*", "-1", "b", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }
        }

        SECTION("Test expr - value")
        {
            WHEN("e = E - 5")
            {
                coek::Expression e = E - 5;

                static std::list<std::string> baseline = {"[", "+", "b", std::to_string(-5.0), "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = E - 5.0")
            {
                coek::Expression e = E - 5.0;

                static std::list<std::string> baseline = {"[", "+", "b", std::to_string(-5.0), "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = E - a")
            {
                coek::Expression e = E - a;

                static std::list<std::string> baseline
                    = {"[", "+", "b", "[", "*", "-1", "a", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = E - z")
            {
                coek::Expression e = E - z;

                static std::list<std::string> baseline = {"[", "+", "b", "[", "-", "z", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = E - Z")
            {
                coek::Expression e = E - Z;

                static std::list<std::string> baseline = {"[", "+", "b", "[", "-", "Z", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = E - E")
            {
                coek::Expression e = E - E;

                static std::list<std::string> baseline
                    = {"[", "+", "b", "[", "*", "-1", "b", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }
        }

        SECTION("Test nestedDiff")
        {
            WHEN("e = (a-b)-5")
            {
                coek::Expression e = (a - b) - 5;

                static std::list<std::string> baseline
                    = {"[", "+", "a", "[", "*", "-1", "b", "]", std::to_string(-5.0), "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 5-(a-b)")
            {
                coek::Expression e = 5 - (a - b);

                static std::list<std::string> baseline = {"[", "+",  std::to_string(5.0),
                                                          "[", "-",  "[",
                                                          "+", "a",  "[",
                                                          "*", "-1", "b",
                                                          "]", "]",  "]",
                                                          "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = (a-b)-c")
            {
                coek::Expression e = (a - b) - c;

                static std::list<std::string> baseline
                    = {"[", "+", "a", "[", "*", "-1", "b", "]", "[", "*", "-1", "c", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = (a-b)-(c-d)")
            {
                coek::Expression e = (a - b) - (c - d);

                static std::list<std::string> baseline
                    = {"[", "+", "a", "[", "*",  "-1", "b", "]", "[", "-", "[",
                       "+", "c", "[", "*", "-1", "d",  "]", "]", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }
        }

        SECTION("Test sumOf_nestedTrivialProduct2")
        {
            WHEN("a*5 - b")
            {
                coek::Expression e = a * 5 - b;

                static std::list<std::string> baseline
                    = {"[", "+", "[", "*", "5", "a", "]", "[", "*", "-1", "b", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("b - a*5")
            {
                coek::Expression e = b - a * 5;

                static std::list<std::string> baseline
                    = {"[", "+", "b", "[", "*", "-5", "a", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("a*5 - (b-c)")
            {
                coek::Expression e = a * 5 - (b - c);

                static std::list<std::string> baseline
                    = {"[", "+", "[", "*", "5",  "a", "]", "[", "-", "[",
                       "+", "b", "[", "*", "-1", "c", "]", "]", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("(b-c) - a*5")
            {
                coek::Expression e = (b - c) - a * 5;

                static std::list<std::string> baseline
                    = {"[", "+", "b", "[", "*", "-1", "c", "]", "[", "*", "-5", "a", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }
        }

        SECTION("Test trivialDiff")
        {
            auto q = coek::parameter("q").value(1);
            auto Q = coek::parameter("Q");
            auto Z = coek::set_element("Z");
            coek::Expression E = a;

            WHEN("e = 0 - a")
            {
                coek::Expression e = 0 - a;

                static std::list<std::string> baseline = {"[", "*", "-1", "a", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 0.0 - a")
            {
                coek::Expression e = 0.0 - a;

                static std::list<std::string> baseline = {"[", "*", "-1", "a", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 0 - q")
            {
                coek::Expression e = 0 - q;

                static std::list<std::string> baseline = {"[", "-", "q", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 0.0 - q")
            {
                coek::Expression e = 0.0 - q;

                static std::list<std::string> baseline = {"[", "-", "q", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 0 - Z")
            {
                coek::Expression e = 0 - Z;

                static std::list<std::string> baseline = {"[", "-", "Z", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 0.0 - Z")
            {
                coek::Expression e = 0.0 - Z;

                static std::list<std::string> baseline = {"[", "-", "Z", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 0 - E")
            {
                coek::Expression e = 0 - E;

                static std::list<std::string> baseline = {"[", "*", "-1", "a", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 0.0 - E")
            {
                coek::Expression e = 0.0 - E;

                static std::list<std::string> baseline = {"[", "*", "-1", "a", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = a - 0")
            {
                coek::Expression e = a - 0;

                static std::list<std::string> baseline = {"a"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = a - 0.0")
            {
                coek::Expression e = a - 0.0;

                static std::list<std::string> baseline = {"a"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = q - 0")
            {
                coek::Expression e = q - 0;

                static std::list<std::string> baseline = {"q"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = q - 0.0")
            {
                coek::Expression e = q - 0.0;

                static std::list<std::string> baseline = {"q"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = Z - 0")
            {
                coek::Expression e = Z - 0;

                static std::list<std::string> baseline = {"Z"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = Z - 0.0")
            {
                coek::Expression e = Z - 0.0;

                static std::list<std::string> baseline = {"Z"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = E - 0")
            {
                coek::Expression e = E - 0;

                static std::list<std::string> baseline = {"a"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = E - 0.0")
            {
                coek::Expression e = E - 0.0;

                static std::list<std::string> baseline = {"a"};
                REQUIRE(e.to_list() == baseline);
            }
        }

        SECTION("Test diff with zero-valued parameter")
        {
            auto q = coek::parameter("q");
            auto Q = coek::parameter("Q");

            WHEN("e = a - q{0}")
            {
                coek::Expression e = a - q;

                static std::list<std::string> baseline = {"[", "+", "a", "[", "-", "q", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = q{0} - a")
            {
                coek::Expression e = q - a;

                static std::list<std::string> baseline
                    = {"[", "+", "q", "[", "*", "-1", "a", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = q_{0} - a")
            {
                coek::Expression e = 0 * q - a;

                static std::list<std::string> baseline
                    = {"[", "+", std::to_string(0.0), "[", "*", "-1", "a", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = a - Q{0.0}")
            {
                coek::Expression e = a - Q;

                static std::list<std::string> baseline = {"[", "+", "a", "[", "-", "Q", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = Q{0.0} - a")
            {
                coek::Expression e = Q - a;

                static std::list<std::string> baseline
                    = {"[", "+", "Q", "[", "*", "-1", "a", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }
        }
    }
}

TEST_CASE("model_neg_expression", "[smoke]")
{
    SECTION("Test negation_param")
    {
        auto p = coek::parameter("p").value(2);

        WHEN("e = -p")
        {
            coek::Expression e = -p;

            static std::list<std::string> baseline = {"[", "-", "p", "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.value() == -2);
        }

        WHEN("e = -(-p)")
        {
            coek::Expression e = -(-p);

            static std::list<std::string> baseline = {"[", "-", "[", "-", "p", "]", "]"};
            REQUIRE(e.to_list() == baseline);
        }
    }

    SECTION("Test negation_terms")
    {
        auto p = coek::parameter("p").value(2);
        auto v = coek::variable("v").lower(0).upper(1).value(0);

        WHEN("e = - p*v")
        {
            coek::Expression e = -p * v;

            static std::list<std::string> baseline = {"[", "*", "[", "-", "p", "]", "v", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("e = -(- p*v)")
        {
            coek::Expression e = -(-p * v);

            static std::list<std::string> baseline
                = {"[", "-", "[", "*", "[", "-", "p", "]", "v", "]", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        WHEN("e = - 5*v")
        {
            coek::Expression e = -5 * v;

            static std::list<std::string> baseline = {"[", "*", "-5", "v", "]"};
            REQUIRE(e.to_list() == baseline);
        }
    }
}

TEST_CASE("model_mul_expression", "[smoke]")
{
    {
        auto a = coek::variable("a").lower(0).upper(1).value(3);
        auto b = coek::variable("b").lower(0).upper(1).value(5);
        auto c = coek::variable("c").lower(0).upper(1).value(0);
        auto d = coek::variable("d").lower(0).upper(1).value(0);

        auto z = coek::parameter("z");
        auto Z = coek::set_element("Z");
        coek::Expression f;
        coek::Expression E = b;

        SECTION("Test simpleProduct")
        {
            coek::Expression e = a * b;

            static std::list<std::string> baseline = {"[", "*", "a", "b", "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.value() == 15);
        }

        SECTION("Test const * value")
        {
            WHEN("e = 5 * a")
            {
                coek::Expression e = 5 * a;

                static std::list<std::string> baseline = {"[", "*", "5", "a", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 5.0 * a")
            {
                coek::Expression e = 5.0 * a;

                static std::list<std::string> baseline = {"[", "*", "5", "a", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 5 * z")
            {
                coek::Expression e = 5 * z;

                static std::list<std::string> baseline = {"[", "*", std::to_string(5.0), "z", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 5.0 * z")
            {
                coek::Expression e = 5.0 * z;

                static std::list<std::string> baseline = {"[", "*", std::to_string(5.0), "z", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 5 * Z")
            {
                coek::Expression e = 5 * Z;

                static std::list<std::string> baseline = {"[", "*", std::to_string(5.0), "Z", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 5.0 * Z")
            {
                coek::Expression e = 5.0 * Z;

                static std::list<std::string> baseline = {"[", "*", std::to_string(5.0), "Z", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 5 * E")
            {
                coek::Expression e = 5 * E;

                static std::list<std::string> baseline = {"[", "*", std::to_string(5.0), "b", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 5.0 * E")
            {
                coek::Expression e = 5.0 * E;

                static std::list<std::string> baseline = {"[", "*", std::to_string(5.0), "b", "]"};
                REQUIRE(e.to_list() == baseline);
            }
        }

        SECTION("Test var * value")
        {
            WHEN("e = a * 5")
            {
                coek::Expression e = a * 5;

                static std::list<std::string> baseline = {"[", "*", "5", "a", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = a * 5.0")
            {
                coek::Expression e = a * 5.0;

                static std::list<std::string> baseline = {"[", "*", "5", "a", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = a * a")
            {
                coek::Expression e = a * a;

                static std::list<std::string> baseline = {"[", "*", "a", "a", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = a * z")
            {
                coek::Expression e = a * z;

                static std::list<std::string> baseline = {"[", "*", "a", "z", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = a * Z")
            {
                coek::Expression e = a * Z;

                static std::list<std::string> baseline = {"[", "*", "a", "Z", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = a * E")
            {
                coek::Expression e = a * E;

                static std::list<std::string> baseline = {"[", "*", "a", "b", "]"};
                REQUIRE(e.to_list() == baseline);
            }
        }

        SECTION("Test param * value")
        {
            WHEN("e = z * 5")
            {
                coek::Expression e = z * 5;

                static std::list<std::string> baseline = {"[", "*", "z", std::to_string(5.0), "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = z * 5.0")
            {
                coek::Expression e = z * 5.0;

                static std::list<std::string> baseline = {"[", "*", "z", std::to_string(5.0), "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = z * a")
            {
                coek::Expression e = z * a;

                static std::list<std::string> baseline = {"[", "*", "z", "a", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = z * z")
            {
                coek::Expression e = z * z;

                static std::list<std::string> baseline = {"[", "*", "z", "z", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = z * Z")
            {
                coek::Expression e = z * Z;

                static std::list<std::string> baseline = {"[", "*", "z", "Z", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = z * E")
            {
                coek::Expression e = z * E;

                static std::list<std::string> baseline = {"[", "*", "z", "b", "]"};
                REQUIRE(e.to_list() == baseline);
            }
        }

        SECTION("Test indexparam * value")
        {
            WHEN("e = Z * 5")
            {
                coek::Expression e = Z * 5;

                static std::list<std::string> baseline = {"[", "*", "Z", std::to_string(5.0), "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = Z * 5.0")
            {
                coek::Expression e = Z * 5.0;

                static std::list<std::string> baseline = {"[", "*", "Z", std::to_string(5.0), "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = Z * a")
            {
                coek::Expression e = Z * a;

                static std::list<std::string> baseline = {"[", "*", "Z", "a", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = Z * z")
            {
                coek::Expression e = Z * z;

                static std::list<std::string> baseline = {"[", "*", "Z", "z", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = Z * Z")
            {
                coek::Expression e = Z * Z;

                static std::list<std::string> baseline = {"[", "*", "Z", "Z", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = Z * E")
            {
                coek::Expression e = Z * E;

                static std::list<std::string> baseline = {"[", "*", "Z", "b", "]"};
                REQUIRE(e.to_list() == baseline);
            }
        }

        SECTION("Test expr * value")
        {
            WHEN("e = E * 5")
            {
                coek::Expression e = E * 5;

                static std::list<std::string> baseline = {"[", "*", "b", std::to_string(5.0), "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = E * 5.0")
            {
                coek::Expression e = E * 5.0;

                static std::list<std::string> baseline = {"[", "*", "b", std::to_string(5.0), "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = E * a")
            {
                coek::Expression e = E * a;

                static std::list<std::string> baseline = {"[", "*", "b", "a", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = E * z")
            {
                coek::Expression e = E * z;

                static std::list<std::string> baseline = {"[", "*", "b", "z", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = E * Z")
            {
                coek::Expression e = E * Z;

                static std::list<std::string> baseline = {"[", "*", "b", "Z", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = E * E")
            {
                coek::Expression e = E * E;

                static std::list<std::string> baseline = {"[", "*", "b", "b", "]"};
                REQUIRE(e.to_list() == baseline);
            }
        }

        SECTION("Test nestedProduct")
        {
            WHEN("e = (a*b)*5")
            {
                coek::Expression e = (a * b) * 5;

                static std::list<std::string> baseline
                    = {"[", "*", "[", "*", "a", "b", "]", std::to_string(5.0), "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 5*(a*b)")
            {
                coek::Expression e = 5 * (a * b);

                static std::list<std::string> baseline
                    = {"[", "*", std::to_string(5.0), "[", "*", "a", "b", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = (a*b)*c")
            {
                coek::Expression e = (a * b) * c;

                static std::list<std::string> baseline
                    = {"[", "*", "[", "*", "a", "b", "]", "c", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = c*(a*b)")
            {
                coek::Expression e = c * (a * b);

                static std::list<std::string> baseline
                    = {"[", "*", "c", "[", "*", "a", "b", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = (a*b)*(c*d)")
            {
                coek::Expression e = (a * b) * (c * d);

                static std::list<std::string> baseline
                    = {"[", "*", "[", "*", "a", "b", "]", "[", "*", "c", "d", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }
        }

        SECTION("Test nestedProduct2")
        {
            WHEN("e = (c+(a+b)) * ((a+b)+d)")
            {
                coek::Expression e = (c + (a + b)) * ((a + b) + d);

                static std::list<std::string> baseline
                    = {"[", "*", "[", "+", "c", "[", "+", "a", "b",
                       "]", "]", "[", "+", "a", "b", "d", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = (c*(a+b)) * ((a+b)*d)")
            {
                coek::Expression e = (c * (a + b)) * ((a + b) * d);

                static std::list<std::string> baseline
                    = {"[", "*", "[", "*", "c", "[", "+", "a", "b", "]", "]",
                       "[", "*", "[", "+", "a", "b", "]", "d", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }
        }

        SECTION("Test nestedProduct3")
        {
            WHEN("e = (3*b)*5")
            {
                coek::Expression e = (3 * b) * 5;

                static std::list<std::string> baseline
                    = {"[", "*", "[", "*", "3", "b", "]", std::to_string(5.0), "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = (a*b)*5")
            {
                coek::Expression e = (a * b) * 5;

                static std::list<std::string> baseline
                    = {"[", "*", "[", "*", "a", "b", "]", std::to_string(5.0), "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 5*(3*b)")
            {
                coek::Expression e = 5 * (3 * b);

                static std::list<std::string> baseline
                    = {"[", "*", std::to_string(5.0), "[", "*", "3", "b", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 5*(a*b)")
            {
                coek::Expression e = 5 * (a * b);

                static std::list<std::string> baseline
                    = {"[", "*", std::to_string(5.0), "[", "*", "a", "b", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = (3*b)*c")
            {
                coek::Expression e = (3 * b) * c;

                static std::list<std::string> baseline
                    = {"[", "*", "[", "*", "3", "b", "]", "c", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = c*(a*b)")
            {
                coek::Expression e = c * (a * b);

                static std::list<std::string> baseline
                    = {"[", "*", "c", "[", "*", "a", "b", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = (a*b)*(c*d)")
            {
                coek::Expression e = (a * b) * (c * d);

                static std::list<std::string> baseline
                    = {"[", "*", "[", "*", "a", "b", "]", "[", "*", "c", "d", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }
        }

        SECTION("Test trivialMul - 0")
        {
            auto q = coek::parameter("q").value(1);
            auto Q = coek::parameter("Q");
            auto Z = coek::set_element("Z");
            coek::Expression E = a;

            WHEN("e = 0 * a")
            {
                coek::Expression e = 0 * a;

                static std::list<std::string> baseline = {std::to_string(0.0)};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 0.0 * a")
            {
                coek::Expression e = 0.0 * a;

                static std::list<std::string> baseline = {std::to_string(0.0)};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 0 * q")
            {
                coek::Expression e = 0 * q;

                static std::list<std::string> baseline = {std::to_string(0.0)};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 0.0 * q")
            {
                coek::Expression e = 0.0 * q;

                static std::list<std::string> baseline = {std::to_string(0.0)};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 0 * Z")
            {
                coek::Expression e = 0 * Z;

                static std::list<std::string> baseline = {std::to_string(0.0)};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 0.0 * Z")
            {
                coek::Expression e = 0.0 * Z;

                static std::list<std::string> baseline = {std::to_string(0.0)};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 0 * E")
            {
                coek::Expression e = 0 * E;

                static std::list<std::string> baseline = {std::to_string(0.0)};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 0.0 * E")
            {
                coek::Expression e = 0.0 * E;

                static std::list<std::string> baseline = {std::to_string(0.0)};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = a * 0")
            {
                coek::Expression e = a * 0;

                static std::list<std::string> baseline = {std::to_string(0.0)};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = a * 0.0")
            {
                coek::Expression e = a * 0.0;

                static std::list<std::string> baseline = {std::to_string(0.0)};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = q * 0")
            {
                coek::Expression e = q * 0;

                static std::list<std::string> baseline = {std::to_string(0.0)};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = q * 0.0")
            {
                coek::Expression e = q * 0.0;

                static std::list<std::string> baseline = {std::to_string(0.0)};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = Z * 0")
            {
                coek::Expression e = Z * 0;

                static std::list<std::string> baseline = {std::to_string(0.0)};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = Z * 0.0")
            {
                coek::Expression e = Z * 0.0;

                static std::list<std::string> baseline = {std::to_string(0.0)};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = E * 0")
            {
                coek::Expression e = E * 0;

                static std::list<std::string> baseline = {std::to_string(0.0)};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = E * 0.0")
            {
                coek::Expression e = E * 0.0;

                static std::list<std::string> baseline = {std::to_string(0.0)};
                REQUIRE(e.to_list() == baseline);
            }
        }

        SECTION("Test trivialMul - 1")
        {
            auto q = coek::parameter("q").value(1);
            auto Q = coek::parameter("Q");
            auto Z = coek::set_element("Z");
            coek::Expression E = a;

            WHEN("e = 1 * a")
            {
                coek::Expression e = 1 * a;

                static std::list<std::string> baseline = {"a"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 1.0 * a")
            {
                coek::Expression e = 1.0 * a;

                static std::list<std::string> baseline = {"a"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 1 * q")
            {
                coek::Expression e = 1 * q;

                static std::list<std::string> baseline = {"q"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 1.0 * q")
            {
                coek::Expression e = 1.0 * q;

                static std::list<std::string> baseline = {"q"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 1 * Z")
            {
                coek::Expression e = 1 * Z;

                static std::list<std::string> baseline = {"Z"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 1.0 * Z")
            {
                coek::Expression e = 1.0 * Z;

                static std::list<std::string> baseline = {"Z"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 1 * E")
            {
                coek::Expression e = 1 * E;

                static std::list<std::string> baseline = {"a"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 1.0 * E")
            {
                coek::Expression e = 1.0 * E;

                static std::list<std::string> baseline = {"a"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = a * 1")
            {
                coek::Expression e = a * 1;

                static std::list<std::string> baseline = {"a"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = a * 1.0")
            {
                coek::Expression e = a * 1.0;

                static std::list<std::string> baseline = {"a"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = q * 1")
            {
                coek::Expression e = q * 1;

                static std::list<std::string> baseline = {"q"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = q * 1.0")
            {
                coek::Expression e = q * 1.0;

                static std::list<std::string> baseline = {"q"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = Z * 1")
            {
                coek::Expression e = Z * 1;

                static std::list<std::string> baseline = {"Z"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = Z * 1.0")
            {
                coek::Expression e = Z * 1.0;

                static std::list<std::string> baseline = {"Z"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = E * 1")
            {
                coek::Expression e = E * 1;

                static std::list<std::string> baseline = {"a"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = E * 1.0")
            {
                coek::Expression e = E * 1.0;

                static std::list<std::string> baseline = {"a"};
                REQUIRE(e.to_list() == baseline);
            }
        }

        SECTION("Test mul with trivial parameter")
        {
            auto q = coek::parameter("q");
            auto Q = coek::parameter("Q");
            auto r = coek::parameter("r").value(1);
            auto R = coek::parameter("R").value(1.0);

            WHEN("e = a * q{0}")
            {
                coek::Expression e = a * q;

                static std::list<std::string> baseline = {"[", "*", "a", "q", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = q{0} * a")
            {
                coek::Expression e = q * a;

                static std::list<std::string> baseline = {"[", "*", "q", "a", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = a * Q{0.0}")
            {
                coek::Expression e = a * Q;

                static std::list<std::string> baseline = {"[", "*", "a", "Q", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = Q{0.0} * a")
            {
                coek::Expression e = Q * a;

                static std::list<std::string> baseline = {"[", "*", "Q", "a", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = a * r{1}")
            {
                coek::Expression e = a * r;

                static std::list<std::string> baseline = {"[", "*", "a", "r", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = r{1} * a")
            {
                coek::Expression e = r * a;

                static std::list<std::string> baseline = {"[", "*", "r", "a", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = a * R{1.0}")
            {
                coek::Expression e = a * R;

                static std::list<std::string> baseline = {"[", "*", "a", "R", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = R{1.0} * a")
            {
                coek::Expression e = R * a;

                static std::list<std::string> baseline = {"[", "*", "R", "a", "]"};
                REQUIRE(e.to_list() == baseline);
            }
        }
    }
}

TEST_CASE("model_div_expression", "[smoke]")
{
    {
        auto a = coek::variable("a").lower(0).upper(1).value(3);
        auto b = coek::variable("b").lower(0).upper(1).value(5);
        auto c = coek::variable("c").lower(0).upper(1).value(0);
        auto d = coek::variable("d").lower(0).upper(1).value(0);

        auto z = coek::parameter("z");
        auto Z = coek::set_element("Z");
        coek::Expression f;
        coek::Expression E = b;

        SECTION("Test simpleDivision")
        {
            coek::Expression e = a / b;

            static std::list<std::string> baseline = {"[", "/", "a", "b", "]"};
            REQUIRE(e.to_list() == baseline);
            REQUIRE(e.value() == 0.6);
        }

        SECTION("Test const / value")
        {
            WHEN("e = 5 / a")
            {
                coek::Expression e = 5 / a;

                static std::list<std::string> baseline = {"[", "/", std::to_string(5.0), "a", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 5.0 / a")
            {
                coek::Expression e = 5.0 / a;

                static std::list<std::string> baseline = {"[", "/", std::to_string(5.0), "a", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 5 / z")
            {
                coek::Expression e = 5 / z;

                static std::list<std::string> baseline = {"[", "/", std::to_string(5.0), "z", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 5.0 / z")
            {
                coek::Expression e = 5.0 / z;

                static std::list<std::string> baseline = {"[", "/", std::to_string(5.0), "z", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 5 / Z")
            {
                coek::Expression e = 5 / Z;

                static std::list<std::string> baseline = {"[", "/", std::to_string(5.0), "Z", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 5.0 / Z")
            {
                coek::Expression e = 5.0 / Z;

                static std::list<std::string> baseline = {"[", "/", std::to_string(5.0), "Z", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 5 / E")
            {
                coek::Expression e = 5 / E;

                static std::list<std::string> baseline = {"[", "/", std::to_string(5.0), "b", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 5.0 / E")
            {
                coek::Expression e = 5.0 / E;

                static std::list<std::string> baseline = {"[", "/", std::to_string(5.0), "b", "]"};
                REQUIRE(e.to_list() == baseline);
            }
        }

        SECTION("Test var / value")
        {
            WHEN("e = a / 5")
            {
                coek::Expression e = a / 5;

                static std::list<std::string> baseline = {"[", "*", "0.2", "a", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = a / 5.0")
            {
                coek::Expression e = a / 5.0;

                static std::list<std::string> baseline = {"[", "*", "0.2", "a", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = a / a")
            {
                coek::Expression e = a / a;

                static std::list<std::string> baseline = {"[", "/", "a", "a", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = a / z")
            {
                coek::Expression e = a / z;

                static std::list<std::string> baseline = {"[", "/", "a", "z", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = a / Z")
            {
                coek::Expression e = a / Z;

                static std::list<std::string> baseline = {"[", "/", "a", "Z", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = a / E")
            {
                coek::Expression e = a / E;

                static std::list<std::string> baseline = {"[", "/", "a", "b", "]"};
                REQUIRE(e.to_list() == baseline);
            }
        }

        SECTION("Test param / value")
        {
            WHEN("e = z / 5")
            {
                coek::Expression e = z / 5;

                static std::list<std::string> baseline = {"[", "/", "z", std::to_string(5.0), "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = z / 5.0")
            {
                coek::Expression e = z / 5.0;

                static std::list<std::string> baseline = {"[", "/", "z", std::to_string(5.0), "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = z / a")
            {
                coek::Expression e = z / a;

                static std::list<std::string> baseline = {"[", "/", "z", "a", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = z / z")
            {
                coek::Expression e = z / z;

                static std::list<std::string> baseline = {"[", "/", "z", "z", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = z / Z")
            {
                coek::Expression e = z / Z;

                static std::list<std::string> baseline = {"[", "/", "z", "Z", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = z / E")
            {
                coek::Expression e = z / E;

                static std::list<std::string> baseline = {"[", "/", "z", "b", "]"};
                REQUIRE(e.to_list() == baseline);
            }
        }

        SECTION("Test indexparam / value")
        {
            WHEN("e = Z / 5")
            {
                coek::Expression e = Z / 5;

                static std::list<std::string> baseline = {"[", "/", "Z", std::to_string(5.0), "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = Z / 5.0")
            {
                coek::Expression e = Z / 5.0;

                static std::list<std::string> baseline = {"[", "/", "Z", std::to_string(5.0), "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = Z / a")
            {
                coek::Expression e = Z / a;

                static std::list<std::string> baseline = {"[", "/", "Z", "a", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = Z / z")
            {
                coek::Expression e = Z / z;

                static std::list<std::string> baseline = {"[", "/", "Z", "z", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = Z / Z")
            {
                coek::Expression e = Z / Z;

                static std::list<std::string> baseline = {"[", "/", "Z", "Z", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = Z / E")
            {
                coek::Expression e = Z / E;

                static std::list<std::string> baseline = {"[", "/", "Z", "b", "]"};
                REQUIRE(e.to_list() == baseline);
            }
        }

        SECTION("Test expr / value")
        {
            WHEN("e = E / 5")
            {
                coek::Expression e = E / 5;

                static std::list<std::string> baseline = {"[", "/", "b", std::to_string(5.0), "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = E / 5.0")
            {
                coek::Expression e = E / 5.0;

                static std::list<std::string> baseline = {"[", "/", "b", std::to_string(5.0), "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = E / a")
            {
                coek::Expression e = E / a;

                static std::list<std::string> baseline = {"[", "/", "b", "a", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = E / z")
            {
                coek::Expression e = E / z;

                static std::list<std::string> baseline = {"[", "/", "b", "z", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = E / Z")
            {
                coek::Expression e = E / Z;

                static std::list<std::string> baseline = {"[", "/", "b", "Z", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = E / E")
            {
                coek::Expression e = E / E;

                static std::list<std::string> baseline = {"[", "/", "b", "b", "]"};
                REQUIRE(e.to_list() == baseline);
            }
        }

        SECTION("Test nested Division")
        {
            WHEN("e = (3*b)/5")
            {
                coek::Expression e = (3 * b) / 5;

                static std::list<std::string> baseline
                    = {"[", "/", "[", "*", "3", "b", "]", std::to_string(5.0), "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = (a/b)/5")
            {
                coek::Expression e = (a / b) / 5;

                static std::list<std::string> baseline
                    = {"[", "/", "[", "/", "a", "b", "]", std::to_string(5.0), "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 5/(a/b)")
            {
                coek::Expression e = 5 / (a / b);

                static std::list<std::string> baseline
                    = {"[", "/", std::to_string(5.0), "[", "/", "a", "b", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = (a/b)/c")
            {
                coek::Expression e = (a / b) / c;

                static std::list<std::string> baseline
                    = {"[", "/", "[", "/", "a", "b", "]", "c", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = c/(a/b)")
            {
                coek::Expression e = c / (a / b);

                static std::list<std::string> baseline
                    = {"[", "/", "c", "[", "/", "a", "b", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = (a/b)/(c/d)")
            {
                coek::Expression e = (a / b) / (c / d);

                static std::list<std::string> baseline
                    = {"[", "/", "[", "/", "a", "b", "]", "[", "/", "c", "d", "]", "]"};
                REQUIRE(e.to_list() == baseline);
            }
        }

        SECTION("Test trivialDiv - 0")
        {
            auto q = coek::parameter("q").value(1);
            auto Z = coek::set_element("Z");
            coek::Expression E = a;

            WHEN("e = 0 / a")
            {
                coek::Expression e = 0 / a;

                static std::list<std::string> baseline = {std::to_string(0.0)};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 0.0 / a")
            {
                coek::Expression e = 0.0 / a;

                static std::list<std::string> baseline = {std::to_string(0.0)};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 0 / q")
            {
                coek::Expression e = 0 / q;

                static std::list<std::string> baseline = {std::to_string(0.0)};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 0.0 / q")
            {
                coek::Expression e = 0.0 / q;

                static std::list<std::string> baseline = {std::to_string(0.0)};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 0 / Z")
            {
                coek::Expression e = 0 / Z;

                static std::list<std::string> baseline = {std::to_string(0.0)};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 0.0 / Z")
            {
                coek::Expression e = 0.0 / Z;

                static std::list<std::string> baseline = {std::to_string(0.0)};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 0 / E")
            {
                coek::Expression e = 0 / E;

                static std::list<std::string> baseline = {std::to_string(0.0)};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = 0.0 / E")
            {
                coek::Expression e = 0.0 / E;

                static std::list<std::string> baseline = {std::to_string(0.0)};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = a / 0") { REQUIRE_THROWS_WITH(a / 0, "Division by zero."); }

            WHEN("e = a / 0.0") { REQUIRE_THROWS_WITH(a / 0.0, "Division by zero."); }

            WHEN("e = q / 0") { REQUIRE_THROWS_WITH(q / 0, "Division by zero."); }

            WHEN("e = q / 0.0") { REQUIRE_THROWS_WITH(q / 0.0, "Division by zero."); }

            WHEN("e = Z / 0") { REQUIRE_THROWS_WITH(Z / 0, "Division by zero."); }

            WHEN("e = Z / 0.0") { REQUIRE_THROWS_WITH(Z / 0.0, "Division by zero."); }

            WHEN("e = E / 0") { REQUIRE_THROWS_WITH(E / 0, "Division by zero."); }

            WHEN("e = E / 0.0") { REQUIRE_THROWS_WITH(E / 0.0, "Division by zero."); }
        }

        SECTION("Test trivialDiv - 1")
        {
            auto q = coek::parameter("q").value(1);
            auto Z = coek::set_element("Z");

            WHEN("e = a/1")
            {
                coek::Expression e = a / 1;

                static std::list<std::string> baseline = {"a"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = a/1.0")
            {
                coek::Expression e = a / 1.0;

                static std::list<std::string> baseline = {"a"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = q/1")
            {
                coek::Expression e = q / 1;

                static std::list<std::string> baseline = {"q"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = q/1.0")
            {
                coek::Expression e = q / 1.0;

                static std::list<std::string> baseline = {"q"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = Z/1")
            {
                coek::Expression e = Z / 1;

                static std::list<std::string> baseline = {"Z"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = Z/1.0")
            {
                coek::Expression e = Z / 1.0;

                static std::list<std::string> baseline = {"Z"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = E/1")
            {
                coek::Expression e = E / 1;

                static std::list<std::string> baseline = {"b"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = E/1.0")
            {
                coek::Expression e = E / 1.0;

                static std::list<std::string> baseline = {"b"};
                REQUIRE(e.to_list() == baseline);
            }
        }

        SECTION("Test div with trivial parameter")
        {
            auto q = coek::parameter("q");
            auto Q = coek::parameter("Q");
            auto r = coek::parameter("r").value(1);
            auto R = coek::parameter("R").value(1.0);

            WHEN("e = q{0} / a")
            {
                coek::Expression e = q / a;

                static std::list<std::string> baseline = {"[", "/", "q", "a", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = Q{0.0} / a")
            {
                coek::Expression e = Q / a;

                static std::list<std::string> baseline = {"[", "/", "Q", "a", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = a/ r{1}")
            {
                coek::Expression e = a / r;

                static std::list<std::string> baseline = {"[", "/", "a", "r", "]"};
                REQUIRE(e.to_list() == baseline);
            }

            WHEN("e = a / R{1.0}")
            {
                coek::Expression e = a / R;

                static std::list<std::string> baseline = {"[", "/", "a", "R", "]"};
                REQUIRE(e.to_list() == baseline);
            }
        }
    }
}

TEST_CASE("intrinsics", "[smoke]")
{
    {
        auto v = coek::variable("v").lower(0).upper(1).value(0);
        auto p = coek::parameter("p");

        SECTION("Test abs")
        {
            {
                coek::Expression constant(-1);
                REQUIRE(constant.is_constant());
                coek::Expression e = abs(constant);
                REQUIRE(e.value() == 1.0);
            }

            coek::Expression e = abs(v);
            v.value(1);
            REQUIRE(e.value() == 1.0);
            v.value(-1);
            REQUIRE(e.value() == 1.0);

            static std::list<std::string> baseline = {"[", "abs", "v", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        SECTION("Test abs - constant")
        {
            coek::Expression constant(0);
            coek::Expression e = abs(constant);
            REQUIRE(e.value() == 0.0);

            static std::list<std::string> baseline = {std::to_string(0.0)};
            REQUIRE(e.to_list() == baseline);
        }

        SECTION("Test ceil")
        {
            coek::Expression e = ceil(v);
            v.value(1.5);
            REQUIRE(e.value() == 2.0);
            v.value(-1.5);
            REQUIRE(e.value() == -1.0);

            static std::list<std::string> baseline = {"[", "ceil", "v", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        SECTION("Test ceil - constant")
        {
            coek::Expression constant(1.5);
            coek::Expression e = ceil(constant);
            REQUIRE(e.value() == 2.0);

            static std::list<std::string> baseline = {std::to_string(2.0)};
            REQUIRE(e.to_list() == baseline);
        }

        SECTION("Test floor")
        {
            coek::Expression e = floor(v);
            v.value(1.5);
            REQUIRE(e.value() == 1.0);
            v.value(-1.5);
            REQUIRE(e.value() == -2.0);

            static std::list<std::string> baseline = {"[", "floor", "v", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        SECTION("Test floor - constant")
        {
            coek::Expression constant(1.5);
            coek::Expression e = floor(constant);
            REQUIRE(e.value() == 1.0);

            static std::list<std::string> baseline = {std::to_string(1.0)};
            REQUIRE(e.to_list() == baseline);
        }

        SECTION("Test exp")
        {
            coek::Expression e = exp(v);
            v.value(1);
            REQUIRE(e.value() == Approx(E));
            v.value(0);
            REQUIRE(e.value() == 1.0);

            static std::list<std::string> baseline = {"[", "exp", "v", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        SECTION("Test exp - constant")
        {
            coek::Expression constant(1);
            coek::Expression e = exp(constant);
            REQUIRE(e.value() == Approx(E));

            static std::list<std::string> baseline = {std::to_string(e.value())};
            REQUIRE(e.to_list() == baseline);
        }

        SECTION("Test log")
        {
            coek::Expression e = log(v);
            v.value(1);
            REQUIRE(e.value() == Approx(0.0));
            v.value(exp(1.0));
            REQUIRE(e.value() == 1.0);

            static std::list<std::string> baseline = {"[", "log", "v", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        SECTION("Test log - constant")
        {
            coek::Expression constant(1);
            coek::Expression e = log(constant);
            REQUIRE(e.value() == Approx(0.0));

            static std::list<std::string> baseline = {std::to_string(0.0)};
            REQUIRE(e.to_list() == baseline);
        }

        SECTION("Test log10")
        {
            coek::Expression e = log10(v);
            v.value(1);
            REQUIRE(e.value() == Approx(0.0));
            v.value(10);
            REQUIRE(e.value() == 1.0);

            static std::list<std::string> baseline = {"[", "log10", "v", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        SECTION("Test log10 - constant")
        {
            coek::Expression constant(1);
            coek::Expression e = log(constant);
            REQUIRE(e.value() == Approx(0.0));

            static std::list<std::string> baseline = {std::to_string(0.0)};
            REQUIRE(e.to_list() == baseline);
        }

        SECTION("Test pow")
        {
            {
                coek::Expression constant0(0);
                auto p = coek::parameter("p");
                coek::Expression e = pow(constant0, p);
                p.value(1);
                REQUIRE(p.value() == Approx(1.0));
                REQUIRE(e.value() == Approx(0.0));
            }
            {
                coek::Expression constant1(1);
                coek::Expression e = pow(constant1, p);
                p.value(1);
                REQUIRE(e.value() == Approx(1.0));
            }
            {
                coek::Expression constant2(2);
                coek::Expression constant1(1);
                coek::Expression e = pow(constant2, constant1);
                REQUIRE(e.value() == Approx(2.0));
            }
            {
                v.value(2);
                coek::Expression constant1(1);
                coek::Expression e = pow(v, constant1);
                REQUIRE(e.value() == Approx(2.0));
            }

            coek::Expression e = pow(v, p);
            v.value(2);
            p.value(0);
            REQUIRE(e.value() == Approx(1.0));
            p.value(1);
            REQUIRE(e.value() == Approx(2.0));

            static std::list<std::string> baseline = {"[", "pow", "v", "p", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        SECTION("Test pow - constant")
        {
            coek::Expression constant(0);
            coek::Expression e = pow(v, constant);
            REQUIRE(e.value() == Approx(1.0));

            static std::list<std::string> baseline = {std::to_string(1.0)};
            REQUIRE(e.to_list() == baseline);
        }

        SECTION("Test sqrt")
        {
            coek::Expression e = sqrt(v);
            v.value(1);
            REQUIRE(e.value() == Approx(1.0));
            v.value(4);
            REQUIRE(e.value() == 2.0);

            static std::list<std::string> baseline = {"[", "sqrt", "v", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        SECTION("Test sqrt - constant")
        {
            coek::Expression constant(1);
            coek::Expression e = sqrt(constant);
            REQUIRE(e.value() == Approx(1.0));

            static std::list<std::string> baseline = {std::to_string(1.0)};
            REQUIRE(e.to_list() == baseline);
        }

        SECTION("Test sin")
        {
            coek::Expression e = sin(v);
            v.value(0);
            REQUIRE(e.value() == Approx(0.0));
            v.value(PI / 2.0);
            REQUIRE(e.value() == Approx(1.0));

            static std::list<std::string> baseline = {"[", "sin", "v", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        SECTION("Test sin - constant")
        {
            coek::Expression constant(0);
            coek::Expression e = sqrt(constant);
            REQUIRE(e.value() == Approx(0.0));

            static std::list<std::string> baseline = {std::to_string(0.0)};
            REQUIRE(e.to_list() == baseline);
        }

        SECTION("Test cos")
        {
            coek::Expression e = cos(v);
            v.value(0);
            REQUIRE(e.value() == Approx(1.0));
            v.value(PI / 2.0);
            REQUIRE(e.value() == Approx(0.0).margin(1e-7));

            static std::list<std::string> baseline = {"[", "cos", "v", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        SECTION("Test cos - constant")
        {
            coek::Expression constant(0);
            coek::Expression e = cos(constant);
            REQUIRE(e.value() == Approx(1.0));

            static std::list<std::string> baseline = {std::to_string(1.0)};
            REQUIRE(e.to_list() == baseline);
        }

        SECTION("Test tan")
        {
            coek::Expression e = tan(v);
            v.value(0);
            REQUIRE(e.value() == Approx(0.0));
            v.value(PI / 4.0);
            REQUIRE(e.value() == Approx(1.0));

            static std::list<std::string> baseline = {"[", "tan", "v", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        SECTION("Test tan - constant")
        {
            coek::Expression constant(0);
            coek::Expression e = tan(constant);
            REQUIRE(e.value() == Approx(0.0));

            static std::list<std::string> baseline = {std::to_string(0.0)};
            REQUIRE(e.to_list() == baseline);
        }

        SECTION("Test asin")
        {
            coek::Expression e = asin(v);
            v.value(0);
            REQUIRE(e.value() == Approx(0.0));
            v.value(1);
            REQUIRE(e.value() == Approx(PI / 2.0));

            static std::list<std::string> baseline = {"[", "asin", "v", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        SECTION("Test asin - constant")
        {
            coek::Expression constant(0);
            coek::Expression e = asin(constant);
            REQUIRE(e.value() == Approx(0.0));

            static std::list<std::string> baseline = {std::to_string(0.0)};
            REQUIRE(e.to_list() == baseline);
        }

        SECTION("Test acos")
        {
            coek::Expression e = acos(v);
            v.value(1);
            REQUIRE(e.value() == Approx(0.0));
            v.value(0);
            REQUIRE(e.value() == Approx(PI / 2.0));

            static std::list<std::string> baseline = {"[", "acos", "v", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        SECTION("Test acos - constant")
        {
            coek::Expression constant(1);
            coek::Expression e = acos(constant);
            REQUIRE(e.value() == Approx(0.0));

            static std::list<std::string> baseline = {std::to_string(0.0)};
            REQUIRE(e.to_list() == baseline);
        }

        SECTION("Test atan")
        {
            coek::Expression e = atan(v);
            v.value(0);
            REQUIRE(e.value() == Approx(0.0));
            v.value(1);
            REQUIRE(e.value() == Approx(PI / 4.0));

            static std::list<std::string> baseline = {"[", "atan", "v", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        SECTION("Test atan - constant")
        {
            coek::Expression constant(0);
            coek::Expression e = atan(constant);
            REQUIRE(e.value() == Approx(0.0));

            static std::list<std::string> baseline = {std::to_string(0.0)};
            REQUIRE(e.to_list() == baseline);
        }

        SECTION("Test sinh")
        {
            coek::Expression e = sinh(v);
            v.value(0);
            REQUIRE(e.value() == Approx(0.0));
            v.value(1);
            REQUIRE(e.value() == Approx((E - 1 / E) / 2.0));

            static std::list<std::string> baseline = {"[", "sinh", "v", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        SECTION("Test sinh - constant")
        {
            coek::Expression constant(0);
            coek::Expression e = sinh(constant);
            REQUIRE(e.value() == Approx(0.0));

            static std::list<std::string> baseline = {std::to_string(0.0)};
            REQUIRE(e.to_list() == baseline);
        }

        SECTION("Test cosh")
        {
            coek::Expression e = cosh(v);
            v.value(0);
            REQUIRE(e.value() == Approx(1.0));
            v.value(1);
            REQUIRE(e.value() == Approx((E + 1 / E) / 2.0));

            static std::list<std::string> baseline = {"[", "cosh", "v", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        SECTION("Test cosh - constant")
        {
            coek::Expression constant(0);
            coek::Expression e = cosh(constant);
            REQUIRE(e.value() == Approx(1.0));

            static std::list<std::string> baseline = {std::to_string(1.0)};
            REQUIRE(e.to_list() == baseline);
        }

        SECTION("Test tanh")
        {
            coek::Expression e = tanh(v);
            v.value(0);
            REQUIRE(e.value() == Approx(0.0));
            v.value(1);
            REQUIRE(e.value() == Approx((E - 1 / E) / (E + 1 / E)));

            static std::list<std::string> baseline = {"[", "tanh", "v", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        SECTION("Test tanh - constant")
        {
            coek::Expression constant(0);
            coek::Expression e = tanh(constant);
            REQUIRE(e.value() == Approx(0.0));

            static std::list<std::string> baseline = {std::to_string(0.0)};
            REQUIRE(e.to_list() == baseline);
        }

        SECTION("Test asinh")
        {
            coek::Expression e = asinh(v);
            v.value(0);
            REQUIRE(e.value() == Approx(0.0));
            v.value((E - 1 / E) / 2.0);
            REQUIRE(e.value() == Approx(1.0));

            static std::list<std::string> baseline = {"[", "asinh", "v", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        SECTION("Test asinh - constant")
        {
            coek::Expression constant(0);
            coek::Expression e = asinh(constant);
            REQUIRE(e.value() == Approx(0.0));

            static std::list<std::string> baseline = {std::to_string(0.0)};
            REQUIRE(e.to_list() == baseline);
        }

        SECTION("Test acosh")
        {
            coek::Expression e = acosh(v);
            v.value(1);
            REQUIRE(e.value() == Approx(0.0));
            v.value((E + 1 / E) / 2.0);
            REQUIRE(e.value() == Approx(1.0));

            static std::list<std::string> baseline = {"[", "acosh", "v", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        SECTION("Test acosh - constant")
        {
            coek::Expression constant(1);
            coek::Expression e = acosh(constant);
            REQUIRE(e.value() == Approx(0.0));

            static std::list<std::string> baseline = {std::to_string(0.0)};
            REQUIRE(e.to_list() == baseline);
        }

        SECTION("Test atanh")
        {
            coek::Expression e = atanh(v);
            v.value(0);
            REQUIRE(e.value() == Approx(0.0));
            v.value((E - 1 / E) / (E + 1 / E));
            REQUIRE(e.value() == Approx(1.0));

            static std::list<std::string> baseline = {"[", "atanh", "v", "]"};
            REQUIRE(e.to_list() == baseline);
        }

        SECTION("Test atanh - constant")
        {
            coek::Expression constant(0);
            coek::Expression e = atanh(constant);
            REQUIRE(e.value() == Approx(0.0));

            static std::list<std::string> baseline = {std::to_string(0.0)};
            REQUIRE(e.to_list() == baseline);
        }
    }
}

TEST_CASE("expression_value", "[smoke]")
{
    auto a = coek::variable("a").lower(0.0).upper(1.0).value(0.0);
    auto b = coek::variable("b").lower(0.0).upper(1.0).value(1.0);
    auto q = coek::parameter("q").value(2);

    SECTION("expression")
    {
        WHEN("e = q")
        {
            coek::Expression e = q;
            REQUIRE(e.value() == 2);
        }

        WHEN("e = a")
        {
            coek::Expression e = a;
            REQUIRE(e.value() == 0);
        }

        WHEN("e = 3*b + q")
        {
            coek::Expression e = 3 * b + q;
            REQUIRE(e.value() == 5.0);
        }
    }

    SECTION("subexpression")
    {
        WHEN("e = q")
        {
            coek::SubExpression e = q;
            REQUIRE(e.value() == 2);
        }

        WHEN("e = a")
        {
            coek::SubExpression e = a;
            REQUIRE(e.value() == 0);
        }

        WHEN("e = 3*b + q")
        {
            coek::SubExpression e = 3 * b + q;
            REQUIRE(e.value() == 5.0);
        }
    }
}
