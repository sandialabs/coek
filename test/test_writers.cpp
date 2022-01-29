
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iterator>
#include <string>
#include <algorithm>

#include "catch2/catch.hpp"

#include "coek/ast/base_terms.hpp"
#include "coek/coek.hpp"

const std::string currdir = COEK_TEST_DIR;


namespace {

void error1(coek::Model& model) 
{
auto x = model.add_variable();
x.set_name("x");
auto y = coek::Variable();
y.set_name("y");

model.add_objective( x );
model.add_constraint( x*y == 4 );
}

void small1(coek::Model& model) 
{
auto x = model.add_variable();
x.set_value(1.0);
auto y = model.add_variable();
y.set_value(1.0);

model.add_objective( pow(x,2) );
model.add_constraint( pow(y,2) == 4 );
}

void small2(coek::Model& model) 
{
auto x = model.add_variable();
x.set_value(1.0);
auto y = model.add_variable();
y.set_value(1.0);

model.add_objective( x );
model.add_constraint( pow(y,2) == 4 );
}

void small3(coek::Model& model) 
{
auto x = model.add_variable();
x.set_value(1.0);
auto y = model.add_variable();
y.set_value(1.0);

model.add_objective( x*y );
model.add_constraint( pow(y,2) == 4 );
}

void small4(coek::Model& model) 
{
auto x = model.add_variable();
x.set_value(1.0);
auto y = model.add_variable();
y.set_value(1.0);

model.add_objective( pow(y,2) );
model.add_constraint( y*x == 4 );
}

void small5(coek::Model& model) 
{
auto x = model.add_variable(-1,1,1);
auto y = model.add_variable(-1,1,2);
auto v = model.add_variable(-1,1,3);
double p = 2;
auto q = coek::Parameter(2);

model.add_objective( pow(x,2)/p + pow(x,2)/q );

model.add_constraint( 1/p*v*(x-y) == 2 );
model.add_constraint( v*1/p*(x-y) == 2 );
model.add_constraint( v*(x-y)/p == 2 );
model.add_constraint( v*(x/p-y/p) == 2 );
model.add_constraint( v*(x-y)*(1/p) == 2 );
model.add_constraint( v*(x-y) == 2*p );

model.add_constraint( 1/q*v*(x-y) == 2 );
model.add_constraint( v*1/q*(x-y) == 2 );
model.add_constraint( v*(x-y)/q == 2 );
model.add_constraint( v*(x/p-y/q) == 2 );
model.add_constraint( v*(x-y)*(1/q) == 2 );
model.add_constraint( v*(x-y) == 2*q );
}

void small6(coek::Model& model) 
{
auto x = model.add_variable(-1,1,1);
auto y = model.add_variable(-1,1,2);
auto v = model.add_variable(-1,1,3);
auto p = model.add_variable();
p.set_value(2);
p.set_fixed(true);

model.add_objective( x );

model.add_constraint( 1/p*v*(x-y) == 2 );
model.add_constraint( v*1/p*(x-y) == 2 );
model.add_constraint( v*(x-y)/p == 2 );
model.add_constraint( v*(x/p-y/p) == 2 );
model.add_constraint( v*(x-y)*(1/p) == 2 );
model.add_constraint( v*(x-y) == 2*p );
}

void small7(coek::Model& model) 
{
auto x = model.add_variable(-1,1,1);
auto y = model.add_variable(-1,1,2);
auto v = model.add_variable(-1,1,3);
auto p = model.add_variable();
p.set_value(2);
p.set_fixed(true);
auto q = coek::Parameter(2);

model.add_objective( x );

model.add_constraint( 1/p/q*v*(x-y) == 2 );
model.add_constraint( v*1/p/p*(x-y) == 2 );
model.add_constraint( v*(x-y)/p/q == 2 );
model.add_constraint( v*(x/p/q-y/p/q) == 2 );
model.add_constraint( v*(x-y)*(1/p/q) == 2 );
model.add_constraint( v*(x-y) == 2*p*q );

model.add_constraint( 1/(p*q)*v*(x-y) == 2 );
model.add_constraint( v*1/(p*p)*(x-y) == 2 );
model.add_constraint( v*(x-y)/(p*q) == 2 );
model.add_constraint( v*(x/(p*q)-y/(p*q)) == 2 );
model.add_constraint( v*(x-y)*(1/(p*q)) == 2 );
model.add_constraint( v*(x-y) == 2*(p*q) );

model.add_constraint( 1/(p+q)*v*(x-y) == 2 );
model.add_constraint( v*1/(p+p)*(x-y) == 2 );
model.add_constraint( v*(x-y)/(p+q) == 2 );
model.add_constraint( v*(x/(p+q)-y/(p+q)) == 2 );
model.add_constraint( v*(x-y)*(1/(p+q)) == 2 );
model.add_constraint( v*(x-y) == 2*(p+q) );

model.add_constraint( 1/pow(p+q,2)*v*(x-y) == 2 );
model.add_constraint( v*1/pow(p+p,2)*(x-y) == 2 );
model.add_constraint( v*(x-y)/pow(p+q,2) == 2 );
model.add_constraint( v*(x/(p+q)-y/pow(p+q,2)) == 2 );
model.add_constraint( v*(x-y)*(1/pow(p+q,2)) == 2 );
model.add_constraint( v*(x-y) == 2*pow(p+q,2) );
}

void small8(coek::Model& model) 
{
double a = 2.0;
auto x = model.add_variable(0, COEK_INFINITY);
auto y = model.add_variable(0, COEK_INFINITY);
auto z = model.add_variable(7, COEK_INFINITY);

model.add_objective( z + x*x + y );
model.add_constraint( a <= y*y );
model.add_constraint( x/a >= y );
model.add_constraint( z <= y + a );
}

void small9(coek::Model& model) 
{
auto x = model.add_variable();
auto y = model.add_variable();
auto z = model.add_variable();
y.set_value(1.0);
y.set_fixed(true);

coek::Parameter p = coek::Parameter(0.0);
double q = 0.0;

model.add_objective( x );
model.add_constraint( x*y*z + x == 1 );
model.add_constraint( x*p*z + x == 1 );
model.add_constraint( x*q*z + x == 1 );
model.add_constraint( x*y*z == 1 );
model.add_constraint( x*p*z == 1 );
model.add_constraint( x*q*z == 1 );
}

void small13(coek::Model& model)
{
auto x = model.add_variable();
x.set_value(0.5);

model.add_objective( x, coek::Model::maximize );
model.add_constraint( pow(x,3) - x == 0 );
model.add_constraint( 10*(pow(x,3) - x) == 0 );
model.add_constraint( (pow(x,3) - x)/10 == 0 );
}

void small14(coek::Model& model)
{
double e = M_E;
double pi = M_PI;

auto ONE = model.add_variable();
ONE.set_value(1);
auto ZERO = model.add_variable();
ZERO.set_value(0);

model.add_objective( ONE+ZERO );
model.add_constraint( log(ONE) == 0 );
model.add_constraint( log10(ONE) == 0 );

model.add_constraint( sin(ZERO) == 0 );
model.add_constraint( cos(ZERO) == 1 );
model.add_constraint( tan(ZERO) == 0 );

model.add_constraint( sinh(ZERO) == 0 );
model.add_constraint( cosh(ZERO) == 1 );
model.add_constraint( tanh(ZERO) == 0 );

model.add_constraint( asin(ZERO) == 0 );
model.add_constraint( acos(ZERO) == pi/2 );
model.add_constraint( atan(ZERO) == 0 );

model.add_constraint( asinh(ZERO) == 0 );
model.add_constraint( acosh((e*e + ONE)/(2*e)) == 0 );
model.add_constraint( atanh(ZERO) == 0 );

model.add_constraint( exp(ZERO) == 1 );
model.add_constraint( sqrt(ONE) == 1 );
model.add_constraint( ceil(ONE) == 1 );
model.add_constraint( floor(ONE) == 1 );
model.add_constraint( abs(ONE) == 1 );
}

void testing1(coek::Model& model)
{
coek::Variable a = model.add_variable("a", 0.0, 1.0, 0.0, false, true);
coek::Variable b("b", 0.0, 1.0, 0.0, true, false);
model.add_variable(b);
coek::Parameter q(2, "q");
    
model.add_objective( 3*a + q );
model.add_constraint( 3*b + q - a <= 0 );
model.add_constraint( 3*b + b == 0 );
model.add_constraint( 3*b*a + q + b*b + b*b == 0 );
model.add_constraint( 3*b*b + q - a*b - a*a <= 0 );
model.add_constraint( inequality(-7, 3*b*b + q - a*b - a*a, 7) );
coek::Variable c = model.add_variable(0, COEK_INFINITY);
coek::Variable d = model.add_variable(-COEK_INFINITY, 0);
model.add_constraint(c + d == 0);
}

bool compare_files(const std::string& fname1, const std::string& fname2)
{
std::ifstream f1(fname1, std::ifstream::binary|std::ifstream::ate);
std::ifstream f2(fname2, std::ifstream::binary|std::ifstream::ate);

// Problem opening files
if (f1.fail() || f2.fail())
    return false;

// Size mismatch
if (f1.tellg() != f2.tellg())
    return false;

// seek back to beginning and use std::equal to compare contents
f1.seekg(0, std::ifstream::beg);
f2.seekg(0, std::ifstream::beg);
return std::equal(
            std::istreambuf_iterator<char>(f1.rdbuf()),
            std::istreambuf_iterator<char>(),
            std::istreambuf_iterator<char>(f2.rdbuf()));
}

bool run_test(coek::Model& model, const std::string& name, const std::string& suffix)
{
std::string fname = name + "." + suffix;
std::string baseline = currdir + "/baselines/" + fname;
model.write(fname);
auto same = compare_files(fname, baseline);
if (same) {
    if( std::remove( fname.c_str() ) != 0 )
        return false;
    }
return same;
}

}

TEST_CASE( "writer", "[smoke]" ) {
{
    std::vector<std::string> nonlinear = {"nl"
#ifdef WITH_FMTLIB
                ,"fmtnl"
#endif
                };
    std::vector<std::string> linear = {"lp","nl"
#ifdef WITH_FMTLIB
                ,"fmtlp","fmtnl"
#endif
                };
    coek::Model model;

SECTION( "error1" ) {
    error1(model);
    REQUIRE_THROWS_WITH(model.write("error1.nl"),
        "Model expressions contain variable 'y' that is not declared in the model.");
    REQUIRE_THROWS_WITH(model.write("error1.fmtnl"),
        "Model expressions contain variable 'y' that is not declared in the model.");
    }

SECTION( "small1" ) {
    small1(model);
    for (const std::string& suffix : linear)
        REQUIRE( run_test(model, "small1", suffix) );
    }

SECTION( "small2" ) {
    small2(model);
    for (const std::string& suffix : linear)
        run_test(model, "small2", suffix);
    }

SECTION( "small3" ) {
    small3(model);
    for (const std::string& suffix : linear)
        run_test(model, "small3", suffix);
    }

SECTION( "small4" ) {
    small4(model);
    for (const std::string& suffix : linear)
        run_test(model, "small4", suffix);
    }

SECTION( "small5" ) {
    small5(model);
    for (const std::string& suffix : nonlinear)
        run_test(model, "small5", suffix);
    }

SECTION( "small6" ) {
    small6(model);
    for (const std::string& suffix : nonlinear)
        run_test(model, "small6", suffix);
    }

SECTION( "small7" ) {
    small7(model);
    for (const std::string& suffix : nonlinear)
        run_test(model, "small7", suffix);
    }

SECTION( "small8" ) {
    small8(model);
    for (const std::string& suffix : linear)
        run_test(model, "small8", suffix);
    }

SECTION( "small9" ) {
    small9(model);
    for (const std::string& suffix : nonlinear)
        run_test(model, "small9", suffix);
    }

SECTION( "small13" ) {
    small13(model);
    for (const std::string& suffix : nonlinear)
        run_test(model, "small13", suffix);
    }

SECTION( "small14" ) {
    small14(model);
    for (const std::string& suffix : nonlinear)
        run_test(model, "small14", suffix);
    }

SECTION( "testing1" ) {
    testing1(model);
    for (const std::string& suffix : linear)
        run_test(model, "testing1", suffix);
    }
}

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}
