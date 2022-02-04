#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <string>
#include <coek/coek.hpp>
#ifdef WITH_CALIPER
#include <caliper/cali.h>
#else
#define CALI_MARK_BEGIN(x)
#define CALI_MARK_END(x)
#endif

using namespace std;

double yt(size_t j, double dx) {
  return 0.5*(1 - (j*dx)*(j*dx));
}


int main(int argc, char** argv) {

  CALI_MARK_BEGIN("main");
  
  if (argc == 1) {
    cout << "lqcp <size> <action=lp,nl,fmtlp,ostrlp,fmtnl,ostrnl,gurobi,ipopt>" << endl;
    return 0;
    }

  cout << "START" << endl << flush;
  string action = argv[2];
  bool ostrlp_file = (action == "ostrlp");
  bool fmtlp_file = (action == "fmtlp");
  bool lp_file = (action == "lp");
  bool nl_file = (action == "nl");
  bool ostrnl_file = (action == "ostrnl");
  bool fmtnl_file = (action == "fmtnl");

  size_t n = stoul(argv[1]);
  size_t m = n;
  size_t n1 = n-1;
  double dx = 1.0/n;
  double T = 1.58;
  double dt = T/m;
  double h2 = dx*dx;
  double a = 0.001;


  CALI_MARK_BEGIN("main:create model");
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

  model.add_objective(0.25*dx*term1 + 0.25*a*dt*term2);


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
  for (size_t i = 1; i <= m; i++)
    model.add_constraint( y[i][2] - 4*y[i][1] + 3*y[i][0] == 0 );
  for (size_t i = 1; i <= m; i++)
    model.add_constraint( (y[i][n-2] - 4*y[i][n1] + 3*y[i][n])/(2*dx) == u[i]-y[i][n]);

  CALI_MARK_END("main:create model");
  
  
if (lp_file) {
    cout << "WRITING LP" << endl << flush;
    model.write("foo.lp");
    }
else if (fmtlp_file) {
    cout << "WRITING FMTLP" << endl << flush;
    model.write("foo.fmtlp");
    }
else if (ostrlp_file) {
    cout << "WRITING OSTRLP" << endl << flush;
    model.write("foo.ostrlp");
    }
else if (nl_file) {
    cout << "WRITING NL" << endl << flush;
    model.write("foo.nl");
    }
else if (fmtnl_file) {
    cout << "WRITING FMTNL" << endl << flush;
    model.write("foo.fmtnl");
    }
else if (ostrnl_file) {
    cout << "WRITING OSTRNL" << endl << flush;
    model.write("foo.ostrnl");
    }
else {
    cout << "SOLVER: " << action << endl << flush;
    coek::Solver opt;
    opt.initialize(action);
    opt.set_option("TimeLimit", 0);

    opt.solve(model);
    }

  cout << "DONE" << endl << flush;

  CALI_MARK_END("main");
  return 0;
}
