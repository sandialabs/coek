
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
TEMPLATE_TEST_CASE("model_add", "[smoke]", coek::Model, coek::CompactModel)
{
#else
TEMPLATE_TEST_CASE("model_add", "[smoke]", coek::Model)
{
#endif
    {
        TestType model;

        SECTION("variables"){WHEN("elementary"){auto w = model.add(coek::variable());
        auto x = model.add(coek::variable().value(1));
        auto y = model.add(coek::variable()).value(1);
    }

    WHEN("array")
    {
        auto w = model.add(coek::variable(3));
        auto x = model.add(coek::variable(3).value(1));
        auto y = model.add(coek::variable(3)).value(1);
        REQUIRE(w.size() == 3);
        REQUIRE(x.size() == 3);
        REQUIRE(y.size() == 3);
    }

    WHEN("multi-dimensional array")
    {
        auto w = model.add(coek::variable({3, 3}));
        auto x = model.add(coek::variable({3, 3}).value(1));
        auto y = model.add(coek::variable({3, 3})).value(1);
        REQUIRE(w.size() == 9);
        REQUIRE(x.size() == 9);
        REQUIRE(y.size() == 9);
    }

#ifdef COEK_WITH_COMPACT_MODEL
    WHEN("map")
    {
        auto A = coek::RangeSet(0, 2) * coek::RangeSet(0, 2);
        auto w = model.add(coek::variable(A));
        auto x = model.add(coek::variable(A).value(1));
        auto y = model.add(coek::variable(A)).value(1);
        REQUIRE(w.size() == 9);
        REQUIRE(x.size() == 9);
        REQUIRE(y.size() == 9);
    }
#endif
}

SECTION("constraints")
{
    WHEN("elementary")
    {
        auto v = coek::variable();
        auto c1 = coek::constraint(v == 0);
        model.add(c1);
    }

    WHEN("array")
    {
        auto v = coek::variable();
        auto c1 = coek::constraint(10);
        c1(0) = v == 0;
        model.add(c1);
        REQUIRE(c1.size() == 1);
    }

    WHEN("multi-dimensional array")
    {
        auto v = coek::variable();
        auto c1 = coek::constraint({10, 10});
        c1(0, 0) = v == 0;
        model.add(c1);
        REQUIRE(c1.size() == 1);
    }

#ifdef COEK_WITH_COMPACT_MODEL
    WHEN("map")
    {
        auto v = coek::variable();
        auto A = coek::RangeSet(0, 2) * coek::RangeSet(0, 2);
        auto c1 = coek::constraint(A);
        c1(0, 0) = v == 0;
        model.add(c1);
        REQUIRE(c1.size() == 1);
    }
#endif
}
}
#ifdef DEBUG
REQUIRE(coek::env.check_memory() == true);
#endif
}

#ifdef COEK_WITH_COMPACT_MODEL
TEST_CASE("compact_model_add", "[smoke]")
{
    {
        coek::CompactModel model;

        SECTION("simple")
        {
            auto I = coek::RangeSet(0, 3);
            auto i = coek::set_element("i");
            auto x = model.add(coek::variable());
            model.add(coek::constraint(i * x == 0, Forall(i).In(I)));
            auto M = model.expand();
            REQUIRE(M.num_constraints() == 4);
        }
    }
#    ifdef DEBUG
    REQUIRE(coek::env.check_memory() == true);
#    endif
}
#endif

TEST_CASE("model_setup", "[smoke]")
{
    {
        auto q = coek::parameter("q").value(2);
        coek::Model model;
        auto a = model.add_variable("a").lower(0).upper(1).value(0).within(coek::Integers);
        auto b = coek::variable("b").lower(0).upper(1).value(0).within(coek::Boolean);
        model.add_variable(b);
        auto c = model.add_variable().lower(0).upper(1).value(3 * q).within(coek::Boolean);
        auto d = model.add_variable("d").lower(0).upper(1).value(4 * q).within(coek::Boolean);

        SECTION("variables")
        {
            WHEN("mutable initial values")
            {
                REQUIRE(c.value() == 6);
                REQUIRE(d.value() == 8);
                q.value(3);
                REQUIRE(c.value() == 9);
                REQUIRE(d.value() == 12);
            }

            WHEN("error1")
            {
                REQUIRE(model.get_variable(0).name() == "a");
                REQUIRE_THROWS_WITH(model.get_variable(4),
                                    "Variable index 4 is too large: 4 variables available.");
            }

            WHEN("error2")
            {
                REQUIRE(model.get_variable("d").name() == "d");
                REQUIRE_THROWS_WITH(model.get_variable("e"), "Unknown variable name e");
            }

            WHEN("suffix")
            {
                model.set_suffix("varval", c, 2.0);
                REQUIRE(model.get_suffix("varval", c) == 2.0);
                static std::set<std::string> names = {"varval"};
                REQUIRE(model.variable_suffix_names() == names);
            }
        }

        SECTION("objectives")
        {
            WHEN("add")
            {
                coek::Expression e = 3 * b + q;
                REQUIRE(model.num_objectives() == 0);
                model.add_objective(e);
                REQUIRE(model.num_objectives() == 1);
            }

            WHEN("error1")
            {
                model.add_objective(3 * b + q);
                REQUIRE_THROWS_WITH(model.get_objective(1),
                                    "Objective index 1 is too large: 1 objectives available.");
            }

            WHEN("error2")
            {
                model.add_objective("obj", 3 * b + q);
                REQUIRE(model.get_objective("obj").name() == "obj");
                REQUIRE_THROWS_WITH(model.get_objective("OBJ"), "Unknown objective name OBJ");
            }

            WHEN("suffix")
            {
                auto o = model.add_objective("obj", 3 * b + q);
                model.set_suffix("objval", o, 2.0);
                REQUIRE(model.get_suffix("objval", o) == 2.0);
                static std::set<std::string> names = {"objval"};
                REQUIRE(model.objective_suffix_names() == names);
            }
        }

        SECTION("constraints")
        {
            WHEN("inequality")
            {
                auto c = 3 * b + q <= 0;
                REQUIRE(model.num_constraints() == 0);
                model.add_constraint(c);
                REQUIRE(model.num_constraints() == 1);
            }

            WHEN("equality")
            {
                auto c = 3 * b + q == 0;
                REQUIRE(model.num_constraints() == 0);
                model.add_constraint(c);
                REQUIRE(model.num_constraints() == 1);
            }

            WHEN("error1")
            {
                model.add_constraint(3 * b + q == 0);
                REQUIRE_THROWS_WITH(model.get_constraint(1),
                                    "Constraint index 1 is too large: 1 constraints available.");
            }

            WHEN("error2")
            {
                model.add_constraint("c", 3 * b + q == 0);
                REQUIRE(model.get_constraint("c").name() == "c");
                REQUIRE_THROWS_WITH(model.get_constraint("C"), "Unknown constraint name C");
            }

            WHEN("suffix")
            {
                auto c = model.add_constraint("c", 3 * b + q == 0);
                model.set_suffix("conval", c, 2.0);
                REQUIRE(model.get_suffix("conval", c) == 2.0);
                static std::set<std::string> names = {"conval"};
                REQUIRE(model.constraint_suffix_names() == names);
            }
        }

        SECTION("model")
        {
            coek::Expression e0 = 3 * a + q;
            model.add_objective(e0);

            auto e2 = 3 * b + q <= 0;
            model.add_constraint(e2);

            auto e3 = 3 * b + q == 0;
            model.add_constraint(e3);

            WHEN("print (df == 0)")
            {
                std::stringstream os;
                os << model;
                std::string tmp = os.str();
                REQUIRE( tmp == "MODEL\n\
  Objectives\n\
    min( 3*a + q )\n\
  Constraints\n\
    3*b + q <= 0\n\
    3*b + q == 0\n\
");
            }

            WHEN("suffix")
            {
                model.set_suffix("mval", 2.0);
                REQUIRE(model.get_suffix("mval") == 2.0);
                static std::set<std::string> names = {"mval"};
                REQUIRE(model.model_suffix_names() == names);
            }
        }
    }
#ifdef DEBUG
    REQUIRE(coek::env.check_memory() == true);
#endif
}

#ifdef COEK_WITH_COMPACT_MODEL
TEST_CASE("compact_model", "[smoke]")
{
#    if 0
    SECTION("add_variable") {
        auto I = coek::RangeSet(0,3);
        auto i = coek::set_element("i");
        coek::CompactModel Model;
        //Model.add_variable( Forall(i).In(I) ).lower(i+1).upper(2*i).value(3*i+2);
        auto x = coek::variable( Forall(i).In(I) ).lower(i+1).upper(2*i).value(3*i+2);
        auto model = Model.expand();

        REQUIRE( model.num_variables() == 4 );
        {
        static std::list<std::string> baseline = {"[", "+", std::to_string(0.0), "1.000", "]"};
        REQUIRE( model.get_variable(0).lower_expression().to_list() == baseline );
        }
        {
        static std::list<std::string> baseline = {"[", "+", "1.000", "1.000", "]"};
        REQUIRE( model.get_variable(1).lower_expression().to_list() == baseline );
        }
        {
        static std::list<std::string> baseline = {"[", "+", "2.000", "1.000", "]"};
        REQUIRE( model.get_variable(2).lower_expression().to_list() == baseline );
        }
        {
        static std::list<std::string> baseline = {"[", "+", "3.000", "1.000", "]"};
        REQUIRE( model.get_variable(3).lower_expression().to_list() == baseline );
        }
    }
#    endif

    SECTION("add_objective")
    {
        auto I = coek::RangeSet(0, 3);
        auto i = coek::set_element("i");
        coek::CompactModel Model;
        auto x = Model.add_variable("x");
#    if 0
        Model.add_objective(i*x).index( Forall(i).In(I) ).sense(coek::Model::maximize);
        TODO - Fix this.  

        auto model = Model.expand();

        REQUIRE( model.num_objectives() == 4 );
        {
        static std::list<std::string> baseline = {"[", "*", std::to_string(0.0), "x", "]"};
        REQUIRE( model.get_objective(0).expr().to_list() == baseline );
        }
        {
        static std::list<std::string> baseline = {"[", "*", "1.000", "x", "]"};
        REQUIRE( model.get_objective(1).expr().to_list() == baseline );
        }
        {
        static std::list<std::string> baseline = {"[", "*", "2.000", "x", "]"};
        REQUIRE( model.get_objective(2).expr().to_list() == baseline );
        }
        {
        static std::list<std::string> baseline = {"[", "*", "3.000", "x", "]"};
        REQUIRE( model.get_objective(3).expr().to_list() == baseline );
        }
#    endif
    }

    SECTION("add_constraint")
    {
        auto I = coek::RangeSet(0, 3);
        auto i = coek::set_element("i");
        coek::CompactModel M;
        auto x = M.add_variable("x");
        M.add_constraint(i * x == 0, Forall(i).In(I));
        auto model = M.expand();

        REQUIRE(model.num_constraints() == 4);
        {
            static std::list<std::string> baseline
                = {"[", "==", "[", "*", std::to_string(0.0), "x", "]", std::to_string(0.0), "]"};
            REQUIRE(model.get_constraint(0).to_list() == baseline);
        }
        {
            static std::list<std::string> baseline
                = {"[", "==", "[", "*", std::to_string(1.0), "x", "]", std::to_string(0.0), "]"};
            REQUIRE(model.get_constraint(1).to_list() == baseline);
        }
        {
            static std::list<std::string> baseline
                = {"[", "==", "[", "*", std::to_string(2.0), "x", "]", std::to_string(0.0), "]"};
            REQUIRE(model.get_constraint(2).to_list() == baseline);
        }
        {
            static std::list<std::string> baseline
                = {"[", "==", "[", "*", std::to_string(3.0), "x", "]", std::to_string(0.0), "]"};
            REQUIRE(model.get_constraint(3).to_list() == baseline);
        }
    }

#    ifdef DEBUG
    REQUIRE(coek::env.check_memory() == true);
#    endif
}
#endif
