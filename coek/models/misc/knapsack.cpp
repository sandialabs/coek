#include <map>
#include <vector>
#include <random>
#include <functional>
#include <coek/coek.hpp>


void knapsack_scalar(coek::Model& model, const std::vector<int>& data)
{
if (data.size() != 1)
    throw std::runtime_error("knapsack_scalar - expecting one arguments (N)");
int N = data[0];   // Number of items (divided by 1000)

N *= 1000;
double W = N/10.0;

std::mt19937 rng(10000) ;
std::uniform_real_distribution<double> distribution(0,1);
auto uniform = std::bind( distribution, rng );

std::vector<double> v(N);
std::vector<double> w(N);
for (int n=0; n<N; n++) {
    v[n] = uniform();
    w[n] = uniform();
    }

std::vector<coek::Variable> x(N);
for (int n=0; n<N; n++)
    model.add( x[n].bounds(0,1).value(0) );

// obj
coek::Expression obj;
for (int n=0; n<N; n++)
    obj += v[n]*x[n];
model.add_objective( obj );

// con
coek::Expression con;
for (int n=0; n<N; n++)
    con += w[n]*x[n];
model.add( con <= W );
}

