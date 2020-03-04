#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <string>
#include "coek_model.hpp"

using namespace std;

double yt(int j, double dx) {
  return 0.5*(1 - (j*dx)*(j*dx));
}


int main(int argc, char** argv) {
  
  cout << "START" << endl << flush;
  string action = argv[2];
  bool lp_file = (action == "lp");

  int n = atoi(argv[1]);
  int m = n;
  int n1 = n-1;
  double dx = 1.0/n;
  double T = 1.58;
  double dt = T/m;
  double h2 = dx*dx;
  double a = 0.001;


  coek::Model model;

  vector<vector<coek::Variable> > y(m+1, vector<coek::Variable>(n+1));
  for (int i = 0; i <= m; i++)
    for (int j = 0; j <= n; j++)
      y[i][j] = model.getVariable(0, 1, 0);
  
  vector<coek::Variable> u(m+1);
  for (int i = 1; i <= m; i++) 
    u[i] = model.getVariable(-1, 1, 0);


  // OBJECTIVE  
  // First term
  coek::Expression term1;
    term1 +=   ( y[m][0] - yt(0,dx) ) * ( y[m][0] - yt(0,dx) );
  for (int j = 1; j <= n-1; j++) {
    term1 += 2*( y[m][j] - yt(j,dx) ) * ( y[m][j] - yt(j,dx) );
  }
    term1 +=   ( y[m][n] - yt(n,dx) ) * ( y[m][n] - yt(n,dx) );

  // Second term
  coek::Expression term2;
  for (int i = 1; i <= m-1; i++)
    term2 += 2*u[i]*u[i];
  term2 += u[m]*u[m];

  model.add(0.25*dx*term1 + 0.25*a*dt*term2);


  // PDE
  for (int i = 0; i < m; i++) {
    for (int j = 1; j < n; j++) {
      model.add( y[i+1][j] - y[i][j] == dt*0.5/h2*(y[i][j-1] - 2*y[i][j] + y[i][j+1] + y[i+1][j-1] - 2*y[i+1][j] + y[i+1][j+1]) );
    }
  }


  // IC
  for (int j = 0; j <= n; j++) {
    model.add( y[0][j] == 0 );
  }


  // BC
  for (int i = 1; i <= m; i++)
    model.add( y[i][2] - 4*y[i][1] + 3*y[i][0] == 0 );
  for (int i = 1; i <= m; i++)
    model.add( (y[i][n-2] - 4*y[i][n1] + 3*y[i][n])/(2*dx) == u[i]-y[i][n]);
  
  
if (lp_file) {
    cout << "WRITING LP" << endl << flush;
    model.write("foo.lp");
    }
else {
    cout << "SOLVER: " << action << endl << flush;
    coek::Solver opt;
    opt.initialize(action);
    opt.set_option("TimeLimit", 0);

    opt.solve(model);
    }

  cout << "DONE" << endl << flush;
  return 0;
}
