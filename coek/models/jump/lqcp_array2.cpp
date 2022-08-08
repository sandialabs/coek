#include <cmath>
#include <cstdlib>
#include <vector>
#include <string>
#include <coek/coek.hpp>


namespace {

inline double yt(size_t j, double dx) {
  return 0.5*(1 - (j*dx)*(j*dx));
}

}


void lqcp_array2(coek::Model& model, size_t n)
{
size_t m = n;
size_t n1 = n-1;
double dx = 1.0/n;
double T = 1.58;
double dt = T/m;
double h2 = dx*dx;
double a = 0.001;

auto y = model.add( coek::variable("y", {m+1,n+1}) ).
                                        bounds(0,1).value(0);
auto u = model.add( coek::variable("u", m+1) ).
                                        bounds(-1,1).value(0);
  
// OBJECTIVE  
// First term
auto term1 = coek::expression();
term1 +=       ( y(m,0) - yt(0,dx) ) * ( y(m,0) - yt(0,dx) );
for (size_t j : coek::sequence<size_t>(1, n-1))
    term1 += 2*( y(m,j) - yt(j,dx) ) * ( y(m,j) - yt(j,dx) );
term1 +=       ( y(m,n) - yt(n,dx) ) * ( y(m,n) - yt(n,dx) );

// Second term
auto term2 = coek::expression();
for (size_t i : coek::sequence<size_t>(1, m-1) )
    term2 += 2*u(i)*u(i);
term2 += u(m)*u(m);

model.add_objective(0.25*dx*term1 + 0.25*a*dt*term2);


// PDE
auto pde = coek::constraint("pde", {m, n});
for (size_t i : coek::sequence<size_t>(m-1))
    for (size_t j : coek::sequence<size_t>(1,n-1))
        pde(i,j) = y(i+1,j) - y(i,j) == dt*0.5/h2*(y(i,j-1) - 2*y(i,j) + y(i,j+1) + y(i+1,j-1) - 2*y(i+1,j) + y(i+1,j+1));
model.add( pde );

// IC
auto ic = coek::constraint("ic", n+1);
for (size_t j : coek::sequence<size_t>(n))
    ic(j) = y(0,j) == 0;
model.add( ic );

// BC
auto bc1 = coek::constraint("bc1", m+1);
for (size_t i : coek::sequence<size_t>(1,m))
    bc1(i) = y(i,2) - 4*y(i,1) + 3*y(i,0) == 0;
model.add( bc1 );

auto bc2 = coek::constraint("bc2", m+1);
for (size_t i : coek::sequence<size_t>(1,m))
    bc2(i) = (y(i,n-2) - 4*y(i,n1) + 3*y(i,n))/(2*dx) == u(i)-y(i,n);
model.add( bc2 );
}

