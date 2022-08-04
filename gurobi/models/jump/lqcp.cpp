//#define _GLIBCXX_USE_CXX11_ABI 0
#include <cmath>
#include <cstdlib>
#include <vector>
#include "gurobi_c++.h"

inline double yt(int j, double dx) {
  return 0.5*(1 - (j*dx)*(j*dx));
}


void lqcp(GRBModel& model, const std::vector<int>& data)
{
  if (data.size() != 1)
     throw std::runtime_error("lqcp - expecting one arguments (N)");
  
  int n = data[0]; // n
  int m = n;
  int n1 = n-1;
  int m1 = m-1;
  double dx = 1.0/n;
  double T = 1.58;
  double dt = T/m;
  double h2 = dx*dx;
  double a = 0.001;

  std::vector<std::vector<GRBVar> > y(m+1, std::vector<GRBVar>(n+1));
  for (int i = 0; i <= m; i++)
    for (int j = 0; j <= n; j++)
      y[i][j] = model.addVar(0, 1, 0, GRB_CONTINUOUS);
  
  std::vector<GRBVar> u(m+1);
  for (int i = 1; i <= m; i++) 
    u[i] = model.addVar(-1, 1, 0, GRB_CONTINUOUS);

  model.update();


  // OBJECTIVE  
  // First term
  GRBQuadExpr term1;
    term1 +=   ( y[m][0] - yt(0,dx) ) * ( y[m][0] - yt(0,dx) );
  for (int j = 1; j <= n-1; j++) {
    term1 += 2*( y[m][j] - yt(j,dx) ) * ( y[m][j] - yt(j,dx) );
  }
    term1 +=   ( y[m][n] - yt(n,dx) ) * ( y[m][n] - yt(n,dx) );

  // Second term
  GRBQuadExpr term2;
  for (int i = 1; i <= m-1; i++)
    term2 += 2*u[i]*u[i];
  term2 += u[m]*u[m];

  model.setObjective(0.25*dx*term1 + 0.25*a*dt*term2);


  // PDE
  for (int i = 0; i <= m1; i++) {
    for (int j = 1; j <= n1; j++) {
      model.addConstr( y[i+1][j] - y[i][j] == dt*0.5/h2*(y[i][j-1] - 
2*y[i][j] + y[i][j+1] + y[i+1][j-1] - 2*y[i+1][j] + y[i+1][j+1]) );
    }
  }


  // IC
  for (int j = 0; j <= n; j++) {
    model.addConstr( y[0][j] == 0 );
  }


  // BC
  for (int i = 1; i <= m; i++) {
    model.addConstr( y[i][2] - 4*y[i][1] + 3*y[i][0] == 0 );
    model.addConstr( (y[i][n-2] - 4*y[i][n1] + 3*y[i][n])/(2*dx) == 
u[i]-y[i][n]);
  }
}
