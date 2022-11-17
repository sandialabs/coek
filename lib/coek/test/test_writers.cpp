
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iterator>
#include <sstream>
#include <string>

#include "catch2/catch.hpp"
#include "coek/ast/base_terms.hpp"
#include "coek/coek.hpp"

const std::string currdir = COEK_TEST_DIR;

namespace {

// ERROR: Unknown variable
template <class ModelType>
void error1(ModelType& model)
{
    auto x = model.add(coek::variable("x"));
    auto y = coek::variable("y");

    model.add(coek::objective(x));
    model.add(x + y == 4);
}

// ERROR: Missing objective
template <class ModelType>
void error2(ModelType& model)
{
    auto x = model.add(coek::variable("x"));

    model.add(x == 4);
}

// ERROR: Multiple objectives
template <class ModelType>
void error3(ModelType& model)
{
    auto x = model.add(coek::variable("x"));

    model.add(coek::objective(x));
    model.add(coek::objective(2 * x));
}

// ERROR: IndexParameter
template <class ModelType>
void error4(ModelType& model)
{
    auto x = model.add(coek::variable("x"));
    auto i = coek::set_element("i");

    model.add(coek::objective(i * x));
}

void small1(coek::Model& model)
{
    auto x = model.add(coek::variable("x").value(1.0));
    auto y = model.add(coek::variable("y").value(1.0));

    model.add(coek::objective(pow(x, 2)));
    model.add(pow(y, 2) == 4);
}

void small2(coek::Model& model)
{
    auto x = model.add(coek::variable().value(1.0));
    auto y = model.add(coek::variable().value(1.0));

    model.add(coek::objective(x));
    model.add(pow(y, 2) == 4);
}

void small3(coek::Model& model)
{
    auto x = model.add(coek::variable().value(1.0));
    auto y = model.add(coek::variable().value(1.0));

    model.add(coek::objective(x * y));
    model.add(pow(y, 2) == 4);
}

void small4(coek::Model& model)
{
    auto x = model.add(coek::variable().value(1.0));
    auto y = model.add(coek::variable().value(1.0));

    model.add(coek::objective(pow(y, 2)));
    model.add(y * x == 4);
}

void small5(coek::Model& model)
{
    auto x = model.add(coek::variable().lower(-1).upper(1).value(1));
    auto y = model.add(coek::variable().lower(-1).upper(1).value(2));
    auto v = model.add(coek::variable().lower(-1).upper(1).value(3));
    double p = 2;
    auto q = coek::parameter().value(2);

    model.add(coek::objective(pow(x, 2) / p + pow(x, 2) / q));

    model.add(1 / p * v * (x - y) == 2);
    model.add(v * 1 / p * (x - y) == 2);
    model.add(v * (x - y) / p == 2);
    model.add(v * (x / p - y / p) == 2);
    model.add(v * (x - y) * (1 / p) == 2);
    model.add(v * (x - y) == 2 * p);

    model.add(1 / q * v * (x - y) == 2);
    model.add(v * 1 / q * (x - y) == 2);
    model.add(v * (x - y) / q == 2);
    model.add(v * (x / p - y / q) == 2);
    model.add(v * (x - y) * (1 / q) == 2);
    model.add(v * (x - y) == 2 * q);
}

void small6(coek::Model& model)
{
    auto x = model.add_variable().lower(-1).upper(1).value(1);
    auto y = model.add_variable().lower(-1).upper(1).value(2);
    auto v = model.add_variable().lower(-1).upper(1).value(3);
    auto p = model.add_variable();
    p.value(2);
    p.fixed(true);

    model.add(coek::objective(x));

    model.add(1 / p * v * (x - y) == 2);
    model.add(v * 1 / p * (x - y) == 2);
    model.add(v * (x - y) / p == 2);
    model.add(v * (x / p - y / p) == 2);
    model.add(v * (x - y) * (1 / p) == 2);
    model.add(v * (x - y) == 2 * p);
}

void small7(coek::Model& model)
{
    auto x = model.add_variable().lower(-1).upper(1).value(1);
    auto y = model.add_variable().lower(-1).upper(1).value(2);
    auto v = model.add_variable().lower(-1).upper(1).value(3);
    auto p = model.add_variable();
    p.value(2);
    p.fixed(true);
    auto q = coek::parameter().value(2);

    model.add(coek::objective(x));

    model.add(1 / p / q * v * (x - y) == 2);
    model.add(v * 1 / p / p * (x - y) == 2);
    model.add(v * (x - y) / p / q == 2);
    model.add(v * (x / p / q - y / p / q) == 2);
    model.add(v * (x - y) * (1 / p / q) == 2);
    model.add(v * (x - y) == 2 * p * q);

    model.add(1 / (p * q) * v * (x - y) == 2);
    model.add(v * 1 / (p * p) * (x - y) == 2);
    model.add(v * (x - y) / (p * q) == 2);
    model.add(v * (x / (p * q) - y / (p * q)) == 2);
    model.add(v * (x - y) * (1 / (p * q)) == 2);
    model.add(v * (x - y) == 2 * (p * q));

    model.add(1 / (p + q) * v * (x - y) == 2);
    model.add(v * 1 / (p + p) * (x - y) == 2);
    model.add(v * (x - y) / (p + q) == 2);
    model.add(v * (x / (p + q) - y / (p + q)) == 2);
    model.add(v * (x - y) * (1 / (p + q)) == 2);
    model.add(v * (x - y) == 2 * (p + q));

    model.add(1 / pow(p + q, 2) * v * (x - y) == 2);
    model.add(v * 1 / pow(p + p, 2) * (x - y) == 2);
    model.add(v * (x - y) / pow(p + q, 2) == 2);
    model.add(v * (x / (p + q) - y / pow(p + q, 2)) == 2);
    model.add(v * (x - y) * (1 / pow(p + q, 2)) == 2);
    model.add(v * (x - y) == 2 * pow(p + q, 2));
}

void small8(coek::Model& model)
{
    double a = 2.0;
    auto x = model.add_variable().lower(0);
    auto y = model.add_variable().lower(0);
    auto z = model.add_variable().lower(7);

    model.add(coek::objective(z + x * x + y));
    model.add(a <= y * y);
    model.add(x / a >= y);
    model.add(z <= y + a);
}

void small9(coek::Model& model)
{
    auto x = model.add_variable();
    auto y = model.add_variable();
    auto z = model.add_variable();
    y.value(1.0);
    y.fixed(true);

    auto p = coek::parameter();
    double q = 0.0;

    model.add(coek::objective(x));
    model.add(x * y * z + x == 1);
    model.add(x * p * z + x == 1);
    model.add(x * q * z + x == 1);
    model.add(x * y * z == 1);
    model.add(x * p * z == 1);
    model.add(x * q * z == 1);
}

void small13(coek::Model& model)
{
    auto x = model.add_variable();
    x.value(0.5);

    model.add(coek::objective(x)).sense(coek::Model::maximize);
    model.add(pow(x, 3) - x == 0);
    model.add(10 * (pow(x, 3) - x) == 0);
    model.add((pow(x, 3) - x) / 10 == 0);
}

void small14(coek::Model& model)
{
    double e = M_E;
    double pi = M_PI;

    auto ONE = model.add_variable();
    ONE.value(1);
    auto ZERO = model.add_variable();
    ZERO.value(0);

    model.add(coek::objective(ONE + ZERO));
    model.add(log(ONE) == 0);
    model.add(log10(ONE) == 0);

    model.add(sin(ZERO) == 0);
    model.add(cos(ZERO) == 1);
    model.add(tan(ZERO) == 0);

    model.add(sinh(ZERO) == 0);
    model.add(cosh(ZERO) == 1);
    model.add(tanh(ZERO) == 0);

    model.add(asin(ZERO) == 0);
    model.add(acos(ZERO) == pi / 2);
    model.add(atan(ZERO) == 0);

    model.add(asinh(ZERO) == 0);
    model.add(acosh((e * e + ONE) / (2 * e)) == 0);
    model.add(atanh(ZERO) == 0);

    model.add(exp(ZERO) == 1);
    model.add(sqrt(ONE) == 1);
    model.add(ceil(ONE) == 1);
    model.add(floor(ONE) == 1);
    model.add(abs(ONE) == 1);
}

void testing1(coek::Model& model)
{
    auto a = model.add_variable("a").lower(0).upper(1).value(0).within(coek::Integers);
    auto b = coek::variable("b").lower(0).upper(1).value(0).within(coek::Binary);
    model.add_variable(b);
    auto c = model.add_variable().lower(0);
    auto d = model.add_variable().upper(0);
    auto e = model.add_variable("e");
    auto q = coek::parameter("q").value(2);

    model.add(coek::objective(3 * a + q)).sense(model.maximize);
    model.add(3 * b + q - a <= 0);
    model.add(3 * b + b == 0);
    model.add(3 * b * a + q + b * b + b * b == 0);
    model.add(3 * b * b + q - a * b - a * a <= 0);
    model.add(inequality(-7, 3 * b * b + q - a * b - a * a, 7));
    e.fix(1.0);
    model.add(c + d == 0);
    model.add(e + 3 * d == 1);
    model.add(inequality(7, 3 * b + q - a, 7));  // This is really an equality
}

void testing2(coek::Model& model)
{
    auto a = model.add_variable("a").lower(0).upper(1).value(0).within(coek::Integers);
    auto b = coek::variable("b").lower(0).upper(1).value(0).within(coek::Binary);
    model.add_variable(b);
    auto q = coek::parameter("q").value(2);

    b.fix(2.0);
    auto e = 3 * a + q + a * a * a * (-a + b + 3 * a + 3 * b)
             + sin(-cos(a));  // This forces the use of a Negate term
    model.add(coek::objective(e));
}

void testing3(coek::Model& model)
{
    auto a = model.add_variable("a").lower(0).upper(1).value(0).within(coek::Integers);
    auto b = coek::variable("b").lower(0).upper(1).value(0).within(coek::Binary);
    model.add_variable(b);

    coek::Expression e(0);
    model.add(coek::objective(e));
    model.add(a + b == 1);
}

// Confirming logic counting for integer nonlinear variables
void testing4(coek::Model& model)
{
    auto x = model.add_variable("x").lower(0).upper(1).value(0).within(coek::Binary);
    auto y = model.add_variable("y").lower(0).upper(1).value(0).within(coek::Binary);
    auto z = model.add_variable("z").lower(0).upper(1).value(0).within(coek::Binary);
    auto a = model.add_variable("a").lower(0).upper(1).value(0).within(coek::Integers);
    auto b = model.add_variable("b").lower(0).upper(1).value(0).within(coek::Binary);

    model.add(coek::objective(a + cos(x) + cos(y)));
    model.add(b + cos(y) + cos(z) == 1);
}

// Confirming logic for variables with same upper-and-lower bounds
void testing5(coek::Model& model)
{
    auto x = model.add_variable("x").lower(2).upper(2).value(0);
    model.add(coek::objective(x));
}

#ifdef COEK_WITH_COMPACT_MODEL
void compact1(coek::CompactModel& model)
{
    auto v = model.add_variable("v");
    model.add(v);
}
#endif

bool compare_files(const std::string& fname1, const std::string& fname2)
{
    std::ifstream f1(fname1, std::ifstream::binary | std::ifstream::ate);
    std::ifstream f2(fname2, std::ifstream::binary | std::ifstream::ate);

    // Problem opening files
    if (f1.fail() || f2.fail()) return false;

    // Size mismatch
    if (f1.tellg() != f2.tellg()) return false;

    // seek back to beginning and use std::equal to compare contents
    f1.seekg(0, std::ifstream::beg);
    f2.seekg(0, std::ifstream::beg);
    return std::equal(std::istreambuf_iterator<char>(f1.rdbuf()), std::istreambuf_iterator<char>(),
                      std::istreambuf_iterator<char>(f2.rdbuf()));
}

template <class ModelType>
bool run_test(ModelType& model, const std::string& name, const std::string& suffix)
{
    std::string fname = name + "." + suffix;
    std::string baseline = currdir + "/baselines/" + fname;
    model.write(fname);
    auto same = compare_files(fname, baseline);
    // std::cout << "name " << name << " " << same << std::endl;
    if (same) {
        if (std::remove(fname.c_str()) != 0) return false;
    }
    return same;
}

}  // namespace

TEST_CASE("model_writer", "[smoke]")
{
    {
        std::vector<std::string> nonlinear = {"nl", "ostrnl"
#ifdef WITH_FMTLIB
                                              ,
                                              "fmtnl"
#endif
        };
        std::vector<std::string> linear = {"lp",
                                           "nl",
                                           "ostrlp",
                                           "ostrnl"
#ifdef WITH_FMTLIB
                                           ,
                                           "fmtlp",
                                           "fmtnl"
#endif
        };
        coek::Model model;

        SECTION("error1")
        {
            error1(model);
            REQUIRE_THROWS_WITH(model.write("error1.nl"),
                                "Error writing NL file: Model expressions contain variable 'y' "
                                "that is not declared in the model.");
            std::remove("error1.nl");
            REQUIRE_THROWS_WITH(model.write("error1.fmtnl"),
                                "Error writing NL file: Model expressions contain variable 'y' "
                                "that is not declared in the model.");
            std::remove("error1.fmtnl");
            REQUIRE_THROWS_WITH(model.write("error1.ostrnl"),
                                "Error writing NL file: Model expressions contain variable 'y' "
                                "that is not declared in the model.");
            std::remove("error1.ostrnl");
            REQUIRE_THROWS_WITH(model.write("error1.lp"),
                                "Error writing LP file: Model expressions contain variable that is "
                                "not declared in the model.");
            std::remove("error1.lp");
            REQUIRE_THROWS_WITH(model.write("error1.fmtlp"),
                                "Error writing LP file: Model expressions contain variable that is "
                                "not declared in the model.");
            std::remove("error1.fmtlp");
            REQUIRE_THROWS_WITH(model.write("error1.ostrlp"),
                                "Error writing LP file: Model expressions contain variable that is "
                                "not declared in the model.");
            std::remove("error1.ostrlp");
        }

        SECTION("error2")
        {
            error2(model);
            REQUIRE_THROWS_WITH(model.write("error2.nl"),
                                "Error writing NL file: No objectives specified!");
            std::remove("error2.nl");
            REQUIRE_THROWS_WITH(model.write("error2.fmtnl"),
                                "Error writing NL file: No objectives specified!");
            std::remove("error2.fmtnl");
            REQUIRE_THROWS_WITH(model.write("error2.ostrnl"),
                                "Error writing NL file: No objectives specified!");
            std::remove("error2.ostrnl");
            REQUIRE_THROWS_WITH(model.write("error2.lp"),
                                "Error writing LP file: No objectives specified!");
            std::remove("error2.lp");
            REQUIRE_THROWS_WITH(model.write("error2.fmtlp"),
                                "Error writing LP file: No objectives specified!");
            std::remove("error2.fmtlp");
            REQUIRE_THROWS_WITH(model.write("error2.ostrlp"),
                                "Error writing LP file: No objectives specified!");
            std::remove("error2.ostrlp");
        }

        SECTION("error3")
        {
            error3(model);
            REQUIRE_THROWS_WITH(model.write("error3.nl"),
                                "Error writing NL file: More than one objective defined!");
            std::remove("error3.nl");
            REQUIRE_THROWS_WITH(model.write("error3.fmtnl"),
                                "Error writing NL file: More than one objective defined!");
            std::remove("error3.fmtnl");
            REQUIRE_THROWS_WITH(model.write("error3.ostrnl"),
                                "Error writing NL file: More than one objective defined!");
            std::remove("error3.ostrnl");
            REQUIRE_THROWS_WITH(model.write("error3.lp"),
                                "Error writing LP file: More than one objective defined!");
            std::remove("error3.lp");
            REQUIRE_THROWS_WITH(model.write("error3.fmtlp"),
                                "Error writing LP file: More than one objective defined!");
            std::remove("error3.fmtlp");
            REQUIRE_THROWS_WITH(model.write("error3.ostrlp"),
                                "Error writing LP file: More than one objective defined!");
            std::remove("error3.ostrlp");
        }

        SECTION("error4")
        {
            error4(model);
            REQUIRE_THROWS_WITH(model.write("error3.nl"),
                                "Error writing NL file: Unexpected index parameter.");
            std::remove("error3.nl");
            REQUIRE_THROWS_WITH(model.write("error3.fmtnl"),
                                "Error writing NL file: Unexpected index parameter.");
            std::remove("error3.fmtnl");
            REQUIRE_THROWS_WITH(model.write("error3.ostrnl"),
                                "Error writing NL file: Unexpected index parameter.");
            std::remove("error3.ostrnl");
            REQUIRE_THROWS_WITH(model.write("error3.lp"),
                                "Error writing LP file: Unexpected index parameter.");
            std::remove("error3.lp");
            REQUIRE_THROWS_WITH(model.write("error3.fmtlp"),
                                "Error writing LP file: Unexpected index parameter.");
            std::remove("error3.fmtlp");
            REQUIRE_THROWS_WITH(model.write("error3.ostrlp"),
                                "Error writing LP file: Unexpected index parameter.");
            std::remove("error3.ostrlp");
        }

        SECTION("error5")
        {
            error1(model);
            REQUIRE_THROWS_WITH(model.write("error1.bad"), "Unknown problem type: error1.bad");
        }

        SECTION("small1")
        {
            small1(model);
            for (const std::string& suffix : linear) REQUIRE(run_test(model, "small1", suffix));
        }

        SECTION("small2")
        {
            small2(model);
            for (const std::string& suffix : linear) REQUIRE(run_test(model, "small2", suffix));
        }

        SECTION("small3")
        {
            small3(model);
            for (const std::string& suffix : linear) REQUIRE(run_test(model, "small3", suffix));
        }

        SECTION("small4")
        {
            small4(model);
            for (const std::string& suffix : linear) REQUIRE(run_test(model, "small4", suffix));
        }

        SECTION("small5")
        {
            small5(model);
            for (const std::string& suffix : nonlinear) REQUIRE(run_test(model, "small5", suffix));
        }

        SECTION("small6")
        {
            small6(model);
            for (const std::string& suffix : nonlinear) REQUIRE(run_test(model, "small6", suffix));
        }

        SECTION("small7")
        {
            small7(model);
            for (const std::string& suffix : nonlinear) REQUIRE(run_test(model, "small7", suffix));
        }

        SECTION("small8")
        {
            small8(model);
            for (const std::string& suffix : linear) REQUIRE(run_test(model, "small8", suffix));
        }

        SECTION("small9")
        {
            small9(model);
            for (const std::string& suffix : nonlinear) REQUIRE(run_test(model, "small9", suffix));
        }

        SECTION("small13")
        {
            small13(model);
            for (const std::string& suffix : nonlinear) REQUIRE(run_test(model, "small13", suffix));
        }

        SECTION("small14")
        {
            small14(model);
            for (const std::string& suffix : nonlinear) REQUIRE(run_test(model, "small14", suffix));
        }

        SECTION("testing1")
        {
            testing1(model);
            for (const std::string& suffix : linear) REQUIRE(run_test(model, "testing1", suffix));
        }

        SECTION("testing2")
        {
            testing2(model);
            for (const std::string& suffix : nonlinear)
                REQUIRE(run_test(model, "testing2", suffix));
        }

        SECTION("testing3")
        {
            testing3(model);
            for (const std::string& suffix : nonlinear)
                REQUIRE(run_test(model, "testing3", suffix));
        }

        SECTION("testing4")
        {
            testing4(model);
            for (const std::string& suffix : nonlinear)
                REQUIRE(run_test(model, "testing4", suffix));
        }

        SECTION("testing5")
        {
            testing5(model);
            for (const std::string& suffix : linear) REQUIRE(run_test(model, "testing5", suffix));
        }

        // TODO - Add separate NLP writer tests to confirm the variable mappings
        SECTION("testing1-nlp")
        {
            testing1(model);
            coek::NLPModel nlp(model, "cppad");
            for (const std::string& suffix : linear) REQUIRE(run_test(nlp, "testing1", suffix));
        }
    }

#ifdef WITH_AST_ENV
    REQUIRE(coek::env.check_memory() == true);
#endif
}

#if 0
TEST_CASE( "compact_model_writer", "[smoke]" ) {
{
    std::vector<std::string> nonlinear = {"nl", "ostrnl"
#    ifdef WITH_FMTLIB
                ,"fmtnl"
#    endif
                };
    std::vector<std::string> linear = {"lp","nl","ostrlp","ostrnl"
#    ifdef WITH_FMTLIB
                ,"fmtlp","fmtnl"
#    endif
                };
    coek::CompactModel model;

#    if 0
SECTION( "error1" ) {
    error1(model);
    REQUIRE_THROWS_WITH(model.write("error1.lp"),
        "Error writing LP file: Model expressions contain variable that is not declared in the model.");
    std::remove("error1.lp");
    REQUIRE_THROWS_WITH(model.write("error1.fmtlp"),
        "Error writing LP file: Model expressions contain variable that is not declared in the model.");
    std::remove("error1.fmtlp");
    REQUIRE_THROWS_WITH(model.write("error1.ostrlp"),
        "Error writing LP file: Model expressions contain variable that is not declared in the model.");
    std::remove("error1.ostrlp");
    }

SECTION( "error2" ) {
    error2(model);
    REQUIRE_THROWS_WITH(model.write("error2.lp"),
        "Error writing LP file: No objectives specified!");
    std::remove("error2.lp");
    REQUIRE_THROWS_WITH(model.write("error2.fmtlp"),
        "Error writing LP file: No objectives specified!");
    std::remove("error2.fmtlp");
    REQUIRE_THROWS_WITH(model.write("error2.ostrlp"),
        "Error writing LP file: No objectives specified!");
    std::remove("error2.ostrlp");
    }
#    endif

SECTION( "error3" ) {
    error3(model);
#    if 0
    REQUIRE_THROWS_WITH(model.write("error3.lp"),
        "Error writing LP file: More than one objective defined!");
    std::remove("error3.lp");
#    endif
    REQUIRE_THROWS_WITH(model.write("error3.fmtlp"),
        "Error writing LP file: More than one objective defined!");
    std::remove("error3.fmtlp");
#    if 0
    REQUIRE_THROWS_WITH(model.write("error3.ostrlp"),
        "Error writing LP file: More than one objective defined!");
    std::remove("error3.ostrlp");
#    endif
    }

#    ifdef COEK_WITH_COMPACT_MODEL
#        if 0
SECTION( "compact1" ) {
    compact1(model);
    for (const std::string& suffix : linear)
        run_test(model, "compact1", suffix);
    }
#        endif
#    endif
}

#    ifdef WITH_AST_ENV
REQUIRE( coek::env.check_memory() == true );
#    endif
}
#endif

TEST_CASE("model_io", "[smoke]")
{
    {
        SECTION("Model")
        {
            coek::Model model;
            auto a = model.add_variable("a");
            auto b = model.add_variable("b");
            auto q = coek::parameter("q");

            model.add(coek::objective(3 * a + q)).sense(model.maximize);
            model.add(3 * b + q <= 0);
            model.add(3 * b + q == 0);
            model.add(coek::inequality(0, 3 * b + q, 1));
            model.add(coek::inequality(2, 3 * b + q, 3, true));

            WHEN("simple")
            {
                std::stringstream os;
                os << model;
                std::string tmp = os.str();
                REQUIRE( tmp == "MODEL\n\
  Objectives\n\
    max( 3*a + q )\n\
  Constraints\n\
    3*b + q <= 0\n\
    3*b + q == 0\n\
    0 <= 3*b + q <= 1\n\
    2 < 3*b + q < 3\n\
");
            }

            WHEN("nlp - cppad")
            {
                coek::NLPModel nlp(model, "cppad");
                std::stringstream os;
                os << nlp;
                std::string tmp = os.str();
                REQUIRE( tmp == "NLPModel:\n\
  variables:         2\n\
  all variables:     2\n\
  fixed variables:   0\n\
  parameters:        1\n\
  objectives:        1\n\
  constraints:       4\n\
  nonzeros Jacobian: 4\n\
  nonzeros Hessian:  0\n\
\n\
MODEL\n\
  Objectives\n\
    max( 3*a + q )\n\
  Constraints\n\
    3*b + q <= 0\n\
    3*b + q == 0\n\
    0 <= 3*b + q <= 1\n\
    2 < 3*b + q < 3\n\n");
            }
        }

        SECTION("Model values")
        {
            coek::Model model;
            auto a = model.add_variable("a").lower(0).upper(1).value(0);
            auto b = model.add_variable("b").lower(0).upper(1).value(0.5);
            auto q = coek::parameter("q");

            model.add(coek::objective(3 * a + q));
            model.add(3 * b + q <= 0);
            model.add(3 * b + q == 0);

            WHEN("simple")
            {
                std::stringstream os;
                model.print_values(os);
                std::string tmp = os.str();
                REQUIRE( tmp == "Model Variables: 2\n\
Nonzero Variables\n\
   b 0.5 0\n");
            }

            WHEN("nlp - cppad")
            {
                coek::NLPModel nlp(model, "cppad");
                std::stringstream os;
                nlp.print_values(os);
                std::string tmp = os.str();
                REQUIRE( tmp == "Model Variables: 2\n\
Nonzero Variables\n\
   1 0.5 0\n");
            }
        }
    }

#ifdef WITH_AST_ENV
    REQUIRE(coek::env.check_memory() == true);
#endif
}
