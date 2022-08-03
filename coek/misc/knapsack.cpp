#include <map>
#include <vector>
#include <random>
#include <functional>
#include <coek/coek.hpp>


void knapsack(coek::Model& model, int N)
{
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
    x[n] = model.add_variable(0,1,0);

// obj
coek::Expression obj;
for (int n=0; n<N; n++)
    obj += v[n]*x[n];
model.add_objective( obj );

// con
coek::Expression con;
for (int n=0; n<N; n++)
    con += w[n]*x[n];
model.add_constraint( con <= W );
}


void knapsack_50(coek::Model& model)
{ knapsack(model, 50); }

void knapsack_100(coek::Model& model)
{ knapsack(model, 100); }

void knapsack_500(coek::Model& model)
{ knapsack(model, 500); }

void knapsack_1000(coek::Model& model)
{ knapsack(model, 1000); }


std::map <std::string, void (*)(coek::Model&)> knapsack_tests {
    {"knapsack_50", knapsack_50},
    {"knapsack_100", knapsack_100},
    {"knapsack_500", knapsack_500},
    {"knapsack_1000", knapsack_1000} };

void knapsack_instance(coek::Model& model, const std::string& name)
{ return knapsack_tests[name](model); }

