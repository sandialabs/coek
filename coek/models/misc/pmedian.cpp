#include <map>
#include <vector>
#include <coek/coek.hpp>


void pmedian_scalar(coek::Model& model, const std::vector<int>& data)
{
if (data.size() != 2)
    throw std::runtime_error("pmedian_scalar - expecting two arguments (N,P)");
int N = data[0];   // Locations
int P = data[1];   // Facilities

int M = N;          // Customers

std::vector<std::vector<double>> d(N, std::vector<double>(M));
for (int n=0; n<N; n++)
    for (int m=0; m<M; m++)
        d[n][m] = 1.0+1.0/(n+m+1);

std::vector<std::vector<coek::Variable>> x(N, std::vector<coek::Variable>(M));
for (int n=0; n<N; n++)
    for (int m=0; m<M; m++)
        model.add( x[n][m].bounds(0,1).value(0) );

std::vector<coek::Variable> y(N);
for (int n=0; n<N; n++)
    model.add( y[n].bounds(0,1).value(0) );

// obj
coek::Expression obj;
for (int n=0; n<N; n++)
    for (int m=0; m<M; m++)
        obj += d[n][m]*x[n][m];
model.add_objective( obj );

// single_x
for (int m=0; m<M; m++) {
    coek::Expression c;
    for (int n=0; n<N; n++)
        c += x[n][m];
    model.add( c == 1 );
    }

// bound_y
for (int n=0; n<N; n++)
    for (int m=0; m<M; m++)
        model.add( x[n][m] - y[n] <= 0 );

// num_facilities
coek::Expression num_facilities;
for (int n=0; n<N; n++)
    num_facilities += y[n];
model.add( num_facilities == P );
}

