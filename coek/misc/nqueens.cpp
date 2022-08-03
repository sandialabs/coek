#include <map>
#include <vector>
#include <coek/coek.hpp>


void nqueens(coek::Model& model, int N)
{
std::vector<std::vector<coek::Variable>> x(N, std::vector<coek::Variable>(N));
for (int i=0; i<N; i++)
    for (int j=0; j<N; j++)
        x[i][j] = model.add_variable(0,1,0,true,false);

// obj
coek::Expression obj;
for (int i=0; i<N; i++)
    for (int j=0; j<N; j++)
        obj += x[i][j];
model.add_objective( obj );

// one per row
for (int i=0; i<N; i++) {
    coek::Expression c;
    for (int j=0; j<N; j++)
        c += x[i][j];
    model.add_constraint( c == 1 );
    }

// one per column
for (int j=0; j<N; j++) {
    coek::Expression c;
    for (int i=0; i<N; i++)
        c += x[i][j];
    model.add_constraint( c == 1 );
    }

// \diagonals_col
for (int i=0; i<N-1; i++) {
    coek::Expression c;
    c += x[0][i];
    for (int j=1; j<N-i; j++)
        c += x[j][i+j];
    model.add_constraint( c <= 1 );
    }
// \diagonals_row
for (int i=1; i<N-1; i++) {
    coek::Expression c;
    c += x[i][0];
    for (int j=1; j<N-i; j++)
        c += x[i+j][j];
    model.add_constraint( c <= 1 );
    }

// /diagonals_col
for (int i=1; i<N; i++) {
    coek::Expression c;
    c += x[0][i];
    for (int j=1; j<=i; j++)
        c += x[j][i-j];
    model.add_constraint( c <= 1 );
    }
// /diagonals_row
for (int i=1; i<N-1; i++) {
    coek::Expression c;
    c += x[i][N-1];
    for (int j=1; j<N-i; j++)
        c += x[i+j][N-1-j];
    model.add_constraint( c <= 1 );
    }
}


void nqueens_400(coek::Model& model)
{ nqueens(model, 400); }

void nqueens_600(coek::Model& model)
{ nqueens(model, 600); }

void nqueens_800(coek::Model& model)
{ nqueens(model, 800); }

void nqueens_1000(coek::Model& model)
{ nqueens(model, 1000); }


std::map <std::string, void (*)(coek::Model&)> nqueens_tests {
        {"nqueens_400", nqueens_400},
        {"nqueens_600", nqueens_600},
        {"nqueens_800", nqueens_800},
        {"nqueens_1000", nqueens_1000} };

void nqueens_instance(coek::Model& model, const std::string& name)
{ return nqueens_tests[name](model); }

