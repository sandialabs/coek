#include <map>
#include <vector>
#include <coek/coek.hpp>


void pmedian(coek::Model& model, int N, int P)
{
// N - Locations
int M = N;              // Customers
// P - Facilities

std::vector<std::vector<double>> d(N, std::vector<double>(M));
for (int n=0; n<N; n++)
    for (int m=0; m<M; m++)
        d[n][m] = 1.0+1.0/(n+m+1);

std::vector<std::vector<coek::Variable>> x(N, std::vector<coek::Variable>(M));
for (int n=0; n<N; n++)
    for (int m=0; m<M; m++)
        x[n][m] = model.add_variable(0,1,0);

std::vector<coek::Variable> y(N);
for (int n=0; n<N; n++)
    y[n] = model.add_variable(0,1,0);

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
    model.add_constraint( c == 1 );
    }

// bound_y
for (int n=0; n<N; n++)
    for (int m=0; m<M; m++)
        model.add_constraint( x[n][m] - y[n] <= 0 );

// num_facilities
coek::Expression num_facilities;
for (int n=0; n<N; n++)
    num_facilities += y[n];
model.add_constraint( num_facilities == P );
}


void pmedian_160_1(coek::Model& model)
{ pmedian(model, 160, 1); }

void pmedian_320_1(coek::Model& model)
{ pmedian(model, 320, 1); }

void pmedian_640_1(coek::Model& model)
{ pmedian(model, 640, 1); }

void pmedian_1280_1(coek::Model& model)
{ pmedian(model, 1280, 1); }


std::map <std::string, void (*)(coek::Model&)> pmedian_tests {
        {"pmedian_160_1", pmedian_160_1},
        {"pmedian_320_1", pmedian_320_1},
        {"pmedian_640_1", pmedian_640_1},
        {"pmedian_1280_1", pmedian_1280_1} };

void pmedian_instance(coek::Model& model, const std::string& name)
{ return pmedian_tests[name](model); }
