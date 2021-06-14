#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <string>
#include <coek/coek.hpp>

using namespace std;

template <typename TYPE>
coek::Expression yt(TYPE j, double dx) {
  return coek::Expression(0.5*(1 - (j*dx)*(j*dx)));
}


int main(int argc, char** argv) {
  
  cout << "START" << endl << flush;
  string action = argv[2];
  bool lp_file = (action == "lp");

  int n = atoi(argv[1]);
  int m = n;
  double dx = 1.0/n;
  double T = 1.58;
  double dt = T/m;
  double h2 = dx*dx;
  double a = 0.001;


#ifdef COEK_WITH_COMPACT_MODEL
  coek::CompactModel model;

  auto M = coek::RangeSet(0, m);
  auto N = coek::RangeSet(0, n);
  auto y = coek::IndexedVariable(M*N, 0, 1, 0, "y");
  auto u = coek::IndexedVariable(coek::RangeSet(1, m), -1, 1, 0, "u");

  model.add_variable(y);
  model.add_variable(u);

  coek::IndexParameter i("i");
  coek::IndexParameter j("j");

  // OBJECTIVE  
  // First term
  coek::Expression term1;
    term1 +=               ( y(m,0) - yt(0,dx) ) * ( y(m,0) - yt(0,dx) );
    term1 += coek::Sum( 2*( y(m,j) - yt(j,dx) ) * ( y(m,j) - yt(j,dx) ),
                        coek::Forall(j).In( coek::RangeSet(1, n-1) ) );
    term1 +=               ( y(m,n) - yt(n,dx) ) * ( y(m,n) - yt(n,dx) );

  // Second term
  coek::Expression term2;
  term2 += coek::Sum( 2*u(i)*u(i),
                        coek::Forall(i).In( coek::RangeSet(1, m-1) ) );
  term2 += u(m)*u(m);

  model.add_objective(0.25*dx*term1 + 0.25*a*dt*term2);

  // PDE
  model.add_constraint( y(i+1,j) - y(i,j) == dt*0.5/h2*(y(i,j-1) - 2*y(i,j) + y(i,j+1) + y(i+1,j-1) - 2*y(i+1,j) + y(i+1,j+1)),
             coek::Forall(i,j).In( coek::RangeSet(0, m-1)*coek::RangeSet(1, n-1) ) );

  // IC
  model.add_constraint( y(0,j) == 0,
             coek::Forall(j).In( coek::RangeSet(0, n) ) );

  // BC
  model.add_constraint( y(i,2) - 4*y(i,1) + 3*y(i,0) == 0,
             coek::Forall(i).In( coek::RangeSet(1,m) ) );
  model.add_constraint( (y(i,n-2) - 4*y(i,n-1) + 3*y(i,n))/(2*dx) == u(i) - y(i,n),
             coek::Forall(i).In( coek::RangeSet(1,m) ) );
  
  if (lp_file) {
    cout << "WRITING LP" << endl << flush;
    model.write("foo2.lp");
    }
  else {
    cout << "SOLVER: " << action << endl << flush;
    coek::Solver opt;
    opt.initialize(action);
    opt.set_option("TimeLimit", 0);

    opt.solve(model);
    }

  cout << "DONE" << endl << flush;
#endif
  return 0;
}
