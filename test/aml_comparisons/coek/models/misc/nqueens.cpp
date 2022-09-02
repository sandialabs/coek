#include <map>
#include <vector>
#include <coek/coek.hpp>


void nqueens_scalar(coek::Model& model, size_t N)
{
std::vector<std::vector<coek::Variable>> x;
for (size_t i=0; i<N; i++) {
    x.push_back(std::vector<coek::Variable>(N));
    for (size_t j=0; j<N; j++)
        model.add( x[i][j].bounds(0,1).value(0).within(coek::VariableTypes::Binary) );
    }

// obj
coek::Expression obj;
for (size_t i=0; i<N; i++)
    for (size_t j=0; j<N; j++)
        obj += x[i][j];
model.add_objective( obj );

// one per row
for (size_t i=0; i<N; i++) {
    coek::Expression c;
    for (size_t j=0; j<N; j++)
        c += x[i][j];
    model.add( c == 1 );
    }

// one per column
for (size_t j=0; j<N; j++) {
    coek::Expression c;
    for (size_t i=0; i<N; i++)
        c += x[i][j];
    model.add( c == 1 );
    }

// \diagonals_col
for (size_t i=0; i<N-1; i++) {
    coek::Expression c;
    c += x[0][i];
    for (size_t j=1; j<N-i; j++)
        c += x[j][i+j];
    model.add( c <= 1 );
    }
// \diagonals_row
for (size_t i=1; i<N-1; i++) {
    coek::Expression c;
    c += x[i][0];
    for (size_t j=1; j<N-i; j++)
        c += x[i+j][j];
    model.add( c <= 1 );
    }

// /diagonals_col
for (size_t i=1; i<N; i++) {
    coek::Expression c;
    c += x[0][i];
    for (size_t j=1; j<=i; j++)
        c += x[j][i-j];
    model.add( c <= 1 );
    }
// /diagonals_row
for (size_t i=1; i<N-1; i++) {
    coek::Expression c;
    c += x[i][N-1];
    for (size_t j=1; j<N-i; j++)
        c += x[i+j][N-1-j];
    model.add( c <= 1 );
    }
}

