//#define _GLIBCXX_USE_CXX11_ABI 0
#include <map>
#include <vector>
#include "gurobi_c++.h"


void nqueens_scalar(GRBModel& model, size_t N)
{
std::vector<std::vector<GRBVar>> x(N, std::vector<GRBVar>(N));
for (size_t i=0; i<N; i++)
    for (size_t j=0; j<N; j++)
        x[i][j] = model.addVar(0,1,0,GRB_BINARY);

// obj
GRBLinExpr obj;
for (size_t i=0; i<N; i++)
    for (size_t j=0; j<N; j++)
        obj += x[i][j];
model.setObjective( obj );

// one per row
for (size_t i=0; i<N; i++) {
    GRBLinExpr c;
    for (size_t j=0; j<N; j++)
        c += x[i][j];
    model.addConstr( c == 1 );
    }

// one per column
for (size_t j=0; j<N; j++) {
    GRBLinExpr c;
    for (size_t i=0; i<N; i++)
        c += x[i][j];
    model.addConstr( c == 1 );
    }

// \diagonals_col
for (size_t i=0; i<N-1; i++) {
    GRBLinExpr c;
    c += x[0][i];
    for (size_t j=1; j<N-i; j++)
        c += x[j][i+j];
    model.addConstr( c <= 1 );
    }
// \diagonals_row
for (size_t i=1; i<N-1; i++) {
    GRBLinExpr c;
    c += x[i][0];
    for (size_t j=1; j<N-i; j++)
        c += x[i+j][j];
    model.addConstr( c <= 1 );
    }

// /diagonals_col
for (size_t i=1; i<N; i++) {
    GRBLinExpr c;
    c += x[0][i];
    for (size_t j=1; j<=i; j++)
        c += x[j][i-j];
    model.addConstr( c <= 1 );
    }
// /diagonals_row
for (size_t i=1; i<N-1; i++) {
    GRBLinExpr c;
    c += x[i][N-1];
    for (size_t j=1; j<N-i; j++)
        c += x[i+j][N-1-j];
    model.addConstr( c <= 1 );
    }
}
