#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <string>
#include <coek/coek.hpp>

using namespace std;

double yt(size_t j, double dx) {
  return 0.5*(1 - (j*dx)*(j*dx));
}


int main(int argc, char** argv) {
  
  if (argc == 1) {
    cout << "rlqcp <size> <action=lp,gurobi,ipopt>" << endl;
    return 0;
    }

  cout << "START" << endl << flush;
  string action = argv[2];
  bool lp_file = (action == "lp");

  size_t n = stoul(argv[1]);
  size_t m = n;
  size_t n1 = n-1;
  double dx = 1.0/n;
  double T = 1.58;
  double dt = T/m;
  double h2 = dx*dx;
  double a = 0.001;
  coek::Parameter W("W", 0.25);

  coek::Model model;

  vector<vector<coek::Variable> > y(m+1, vector<coek::Variable>(n+1));
  for (size_t i = 0; i <= m; i++)
    for (size_t j = 0; j <= n; j++)
      y[i][j] = model.add_variable(0, 1, 0);
  
  vector<coek::Variable> u(m+1);
  for (size_t i = 1; i <= m; i++) 
    u[i] = model.add_variable(-1, 1, 0);


  // OBJECTIVE  
  // First term
  coek::Expression term1;
    term1 +=   ( y[m][0] - yt(0,dx) ) * ( y[m][0] - yt(0,dx) );
  for (size_t j = 1; j <= n-1; j++) {
    term1 += 2*( y[m][j] - yt(j,dx) ) * ( y[m][j] - yt(j,dx) );
  }
    term1 +=   ( y[m][n] - yt(n,dx) ) * ( y[m][n] - yt(n,dx) );

  // Second term
  coek::Expression term2;
  for (size_t i = 1; i <= m-1; i++)
    term2 += 2*u[i]*u[i];
  term2 += u[m]*u[m];

  model.add_objective(0.25*dx*term1 + W*a*dt*term2);


  // PDE
  for (size_t i = 0; i < m; i++) {
    for (size_t j = 1; j < n; j++) {
      model.add_constraint( y[i+1][j] - y[i][j] == dt*0.5/h2*(y[i][j-1] - 2*y[i][j] + y[i][j+1] + y[i+1][j-1] - 2*y[i+1][j] + y[i+1][j+1]) );
    }
  }


  // IC
  for (size_t j = 0; j <= n; j++) {
    model.add_constraint( y[0][j] == 0 );
  }


  // BC
  for (size_t i = 1; i <= m; i++) {
    model.add_constraint( y[i][2] - 4*y[i][1] + 3*y[i][0] == 0 );
  }
  for (size_t i = 1; i <= m; i++) {
    model.add_constraint( (y[i][n-2] - 4*y[i][n1] + 3*y[i][n])/(2*dx) == u[i]-y[i][n]);
  }
  
  
if (lp_file) {
    cout << "WRITING LP" << endl << flush;
    model.write("foo.lp");
    }
else {
    cout << "SOLVER:   " << action << endl << flush;
    size_t nsolves = stoul(argv[3]);
    cout << "# SOLVES: " << nsolves << endl << flush;
    coek::Solver opt(action);
    opt.load(model);
    for (size_t i=0; i<nsolves; i++) {
        //cout << "i: " << i << "\tT: " << T.get_value() << endl << flush;
        cout << "i: " << i << "\tW: " << W.get_value() << endl << flush;
        opt.resolve();
        //T.set_value(1.58 + (i+1.0)/10);
        //W.set_value(0.25 + (i+1.0)/10);
        }
    }

  cout << "DONE" << endl << flush;
  return 0;
}
