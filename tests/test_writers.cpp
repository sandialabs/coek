
#include <cmath>
#include "catch.hpp"
#include "expr/ast_term.hpp"
#include "coek_model.hpp"


void error1(coek::Model& model) 
{
auto x = model.getVariable();
auto y = coek::Variable();

model.add( x );
model.add( x*y == 4 );
}

void small1(coek::Model& model) 
{
auto x = model.getVariable();
x.set_value(1.0);
auto y = model.getVariable();
y.set_value(1.0);

model.add( pow(x,2) );
model.add( pow(y,2) == 4 );
}

void small2(coek::Model& model) 
{
auto x = model.getVariable();
x.set_value(1.0);
auto y = model.getVariable();
y.set_value(1.0);

model.add( x );
model.add( pow(y,2) == 4 );
}

void small3(coek::Model& model) 
{
auto x = model.getVariable();
x.set_value(1.0);
auto y = model.getVariable();
y.set_value(1.0);

model.add( x*y );
model.add( pow(y,2) == 4 );
}

void small4(coek::Model& model) 
{
auto x = model.getVariable();
x.set_value(1.0);
auto y = model.getVariable();
y.set_value(1.0);

model.add( pow(y,2) );
model.add( y*x == 4 );
}

void small5(coek::Model& model) 
{
auto x = model.getVariable(-1,1,1);
auto y = model.getVariable(-1,1,2);
auto v = model.getVariable(-1,1,3);
double p = 2;
auto q = coek::Parameter(2);

model.add( pow(x,2)/p + pow(x,2)/q );

model.add( 1/p*v*(x-y) == 2 );
model.add( v*1/p*(x-y) == 2 );
model.add( v*(x-y)/p == 2 );
model.add( v*(x/p-y/p) == 2 );
model.add( v*(x-y)*(1/p) == 2 );
model.add( v*(x-y) == 2*p );

model.add( 1/q*v*(x-y) == 2 );
model.add( v*1/q*(x-y) == 2 );
model.add( v*(x-y)/q == 2 );
model.add( v*(x/p-y/q) == 2 );
model.add( v*(x-y)*(1/q) == 2 );
model.add( v*(x-y) == 2*q );
}

void small6(coek::Model& model) 
{
auto x = model.getVariable(-1,1,1);
auto y = model.getVariable(-1,1,2);
auto v = model.getVariable(-1,1,3);
auto p = model.getVariable();
p.set_value(2);
p.set_fixed(true);

model.add( x );

model.add( 1/p*v*(x-y) == 2 );
model.add( v*1/p*(x-y) == 2 );
model.add( v*(x-y)/p == 2 );
model.add( v*(x/p-y/p) == 2 );
model.add( v*(x-y)*(1/p) == 2 );
model.add( v*(x-y) == 2*p );
}

void small7(coek::Model& model) 
{
auto x = model.getVariable(-1,1,1);
auto y = model.getVariable(-1,1,2);
auto v = model.getVariable(-1,1,3);
auto p = model.getVariable();
p.set_value(2);
p.set_fixed(true);
auto q = coek::Parameter(2);

model.add( x );

model.add( 1/p/q*v*(x-y) == 2 );
model.add( v*1/p/p*(x-y) == 2 );
model.add( v*(x-y)/p/q == 2 );
model.add( v*(x/p/q-y/p/q) == 2 );
model.add( v*(x-y)*(1/p/q) == 2 );
model.add( v*(x-y) == 2*p*q );

model.add( 1/(p*q)*v*(x-y) == 2 );
model.add( v*1/(p*p)*(x-y) == 2 );
model.add( v*(x-y)/(p*q) == 2 );
model.add( v*(x/(p*q)-y/(p*q)) == 2 );
model.add( v*(x-y)*(1/(p*q)) == 2 );
model.add( v*(x-y) == 2*(p*q) );

model.add( 1/(p+q)*v*(x-y) == 2 );
model.add( v*1/(p+p)*(x-y) == 2 );
model.add( v*(x-y)/(p+q) == 2 );
model.add( v*(x/(p+q)-y/(p+q)) == 2 );
model.add( v*(x-y)*(1/(p+q)) == 2 );
model.add( v*(x-y) == 2*(p+q) );

model.add( 1/pow(p+q,2)*v*(x-y) == 2 );
model.add( v*1/pow(p+p,2)*(x-y) == 2 );
model.add( v*(x-y)/pow(p+q,2) == 2 );
model.add( v*(x/(p+q)-y/pow(p+q,2)) == 2 );
model.add( v*(x-y)*(1/pow(p+q,2)) == 2 );
model.add( v*(x-y) == 2*pow(p+q,2) );
}

void small8(coek::Model& model) 
{
double a = 2.0;
auto x = model.getVariable(0, COEK_INFINITY);
auto y = model.getVariable(0, COEK_INFINITY);
auto z = model.getVariable(7, COEK_INFINITY);

model.add( z + x*x + y );
model.add( a <= y*y );
model.add( x/a >= y );
model.add( z <= y + a );
}

void small9(coek::Model& model) 
{
auto x = model.getVariable();
auto y = model.getVariable();
auto z = model.getVariable();
y.set_value(1.0);
y.set_fixed(true);

coek::Parameter p = coek::Parameter(0.0);
double q = 0.0;

model.add( x );
model.add( x*y*z + x == 1 );
model.add( x*p*z + x == 1 );
model.add( x*q*z + x == 1 );
model.add( x*y*z == 1 );
model.add( x*p*z == 1 );
model.add( x*q*z == 1 );
}

void small13(coek::Model& model)
{
auto x = model.getVariable();
x.set_value(0.5);

model.add( x, coek::Model::maximize );
model.add( pow(x,3) - x == 0 );
model.add( 10*(pow(x,3) - x) == 0 );
model.add( (pow(x,3) - x)/10 == 0 );
}

void small14(coek::Model& model)
{
double e = M_E;
double pi = M_PI;

auto ONE = model.getVariable();
ONE.set_value(1);
auto ZERO = model.getVariable();
ZERO.set_value(1);

model.add( ONE+ZERO );
model.add( log(ONE) == 0 );
model.add( log10(ONE) == 0 );

model.add( sin(ZERO) == 0 );
model.add( cos(ZERO) == 1 );
model.add( tan(ZERO) == 0 );

model.add( sinh(ZERO) == 0 );
model.add( cosh(ZERO) == 1 );
model.add( tanh(ZERO) == 0 );

model.add( asin(ZERO) == 0 );
model.add( acos(ZERO) == pi/2 );
model.add( atan(ZERO) == 0 );

model.add( asinh(ZERO) == 0 );
model.add( acosh((e*e + ONE)/(2*e)) == 0 );
model.add( atanh(ZERO) == 0 );

model.add( exp(ZERO) == 1 );
model.add( sqrt(ONE) == 1 );
model.add( ceil(ONE) == 1 );
model.add( floor(ONE) == 1 );
model.add( abs(ONE) == 1 );
}

void testing1(coek::Model& model)
{
coek::Variable a = model.getVariable(0.0, 1.0, 0.0, false, true, "a");
coek::Variable b(0.0, 1.0, 0.0, true, false, "b");
model.addVariable(b);
coek::Parameter q(2, "q");
    
model.add( 3*a + q );
model.add( 3*b + q - a <= 0 );
model.add( 3*b + b == 0 );
model.add( 3*b*a + q + b*b + b*b == 0 );
model.add( 3*b*b + q - a*b - a*a <= 0 );
model.add( inequality(-7, 3*b*b + q - a*b - a*a, 7) );
coek::Variable c = model.getVariable(0, COEK_INFINITY);
coek::Variable d = model.getVariable(-COEK_INFINITY, 0);
model.add (c + d == 0);
}


TEST_CASE( "lp_writer", "[smoke]" ) {

  SECTION( "error1" ) {
    coek::Model model;
    //error1(model);
    //REQUIRE_THROWS_WITH(model.write("error1.lp"),
    //    "Error writing LP file: Unexpected variable encountered in a model expression");
    }

  SECTION( "small1" ) {
    coek::Model model;
    small1(model);
    model.write("small1.lp");
    }

  SECTION( "small2" ) {
    coek::Model model;
    small2(model);
    model.write("small2.lp");
    }

  SECTION( "small3" ) {
    coek::Model model;
    small3(model);
    model.write("small3.lp");
    }

  SECTION( "small4" ) {
    coek::Model model;
    small4(model);
    model.write("small4.lp");
    }

  SECTION( "small8" ) {
    coek::Model model;
    small8(model);
    model.write("small8.lp");
    }

  SECTION( "testing1" ) {
    coek::Model model;
    testing1(model);
    model.write("testing1.lp");
    }

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}

TEST_CASE( "nl_writer", "[smoke]" ) {

  SECTION( "error1" ) {
    coek::Model model;
    error1(model);
    REQUIRE_THROWS_WITH(model.write("error1.nl"),
        "Model expressions contain variables that are not declared in the model.");
    }

  SECTION( "small1" ) {
    coek::Model model;
    small1(model);
    model.write("small1.nl");
    }

  SECTION( "small2" ) {
    coek::Model model;
    small2(model);
    model.write("small2.nl");
    }

  SECTION( "small3" ) {
    coek::Model model;
    small3(model);
    model.write("small3.nl");
    }

  SECTION( "small4" ) {
    coek::Model model;
    small4(model);
    model.write("small4.nl");
    }

  SECTION( "small5" ) {
    coek::Model model;
    small5(model);
    model.write("small5.nl");
    }

  SECTION( "small6" ) {
    coek::Model model;
    small6(model);
    model.write("small6.nl");
    }

  SECTION( "small7" ) {
    coek::Model model;
    small7(model);
    model.write("small7.nl");
    }

  SECTION( "small8" ) {
    coek::Model model;
    small8(model);
    model.write("small8.nl");
    }

  SECTION( "small9" ) {
    coek::Model model;
    small9(model);
    model.write("small9.nl");
    }

  SECTION( "small13" ) {
    coek::Model model;
    small13(model);
    model.write("small13.nl");
    }

  SECTION( "small14" ) {
    coek::Model model;
    small14(model);
    model.write("small14.nl");
    }

  SECTION( "testing1" ) {
    coek::Model model;
    testing1(model);
    model.write("testing1.nl");
    }

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}
