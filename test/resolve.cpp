//
// Demonstrating resolves on a canonical linear program:
//
//   max c'x
//       Ax <= b
//       x >= 0
//
#include <chrono>
#include <map>
#include <vector>
#include <random>
#include <functional>
#include <coek/coek.hpp>

void baseline_test(int N, const std::string& solver);
void objective_test(int N, int nsolves, const std::string& solver);
void row_test(int N, int nsolves, const std::string& solver);
void col_test(int N, int nsolves, const std::string& solver);
void rhs_test(int N, int nsolves, const std::string& solver);

int main(int argc, char** argv)
{
if (argc == 1) {
    std::cout << "resolve <N> <n-solves> <test> <solver>" << std::endl;
    std::cout << "   Tests: baseline, obj, row, col, rhs" << std::endl;
    return 0;
    }

int N = atoi(argv[1]);          // A is an NxN matrix
int nsolves = atoi(argv[2]);    // Number of solvers
std::string test = argv[3];     // Specify the type of test that will be performed
std::string solver = argv[4];   // Specify the solver used during the tests

if (test == "baseline")
    baseline_test(N, solver);

else if (test == "obj")
    objective_test(N, nsolves, solver);

else if (test == "row")
    row_test(N, nsolves, solver);

else if (test == "col")
    col_test(N, nsolves, solver);

else if (test == "rhs")
    rhs_test(N, nsolves, solver);

return 0;
}


void baseline_test(int N, const std::string& solver)
{
std::mt19937 rng(10000) ;
std::uniform_real_distribution<double> distribution(0,1);
auto uniform = std::bind( distribution, rng );

coek::Model model;

std::vector<std::vector<double>> A(N, std::vector<double>(N));
for (int n=0; n<N; n++)
    for (int m=0; m<N; m++)
        A[n][m] = uniform();

std::vector<double> c(N);
for (int n=0; n<N; n++)
    c[n] = uniform();

std::vector<double> b(N);
for (int n=0; n<N; n++)
    b[n] = uniform();

std::vector<coek::Variable> x(N);
for (int n=0; n<N; n++)
    x[n] = model.getVariable(0, COEK_INFINITY, 1);

// obj
coek::Expression obj;
for (int n=0; n<N; n++)
    obj += c[n] * x[n];
model.add( obj, coek::Model::maximize );

// Ax <= b
for (int n=0; n<N; n++) {
    coek::Expression expr;
    for (int m=0; m<N; m++)
        expr += A[n][m] * x[m];
    model.add( expr <= b[n] );
    break;
    }


if (solver == "gurobi") {
    coek::Solver opt(solver);
    //opt.set_option("TimeLimit", 0);
    opt.solve(model);
    std::cout << "Solve: " << 0 << "  Objective: " << model.repn->objectives[0].body().get_value() << std::endl;
    }
else {
    coek::NLPModel nlp(model, "cppad");
    coek::NLPSolver opt("ipopt");
    opt.set_option("print_level", 0);
    opt.solve(nlp);
    std::cout << "Solve: " << 0 << "  Objective: " << nlp.compute_f() << std::endl;
    }
}


void objective_test(int N, int nsolves, const std::string& solver)
{
auto start = std::chrono::high_resolution_clock::now();

std::mt19937 rng(10000) ;
std::uniform_real_distribution<double> distribution(0,1);
auto uniform = std::bind( distribution, rng );

coek::Model model;

std::vector<std::vector<double>> A(N, std::vector<double>(N));
for (int n=0; n<N; n++)
    for (int m=0; m<N; m++)
        A[n][m] = uniform();

std::vector<coek::Parameter> c(N);
for (int n=0; n<N; n++)
    c[n] = coek::Parameter(uniform());

std::vector<double> b(N);
for (int n=0; n<N; n++)
    b[n] = uniform();

std::vector<coek::Variable> x(N);
for (int n=0; n<N; n++)
    x[n] = model.getVariable(0, COEK_INFINITY, 0);

// obj
coek::Expression obj;
for (int n=0; n<N; n++)
    obj += c[n] * x[n];
model.add( obj, coek::Model::maximize );

// Ax <= b
for (int n=0; n<N; n++) {
    coek::Expression expr;
    for (int m=0; m<N; m++)
        expr += A[n][m] * x[m];
    model.add( expr <= b[n] );
    }

if (solver == "gurobi") {
    coek::Solver opt(solver);
    opt.load(model);
    for (int i=0; i<nsolves; i++) {
        opt.resolve();
        std::cout << "Solve: " << i << "  Objective: " << model.repn->objectives[0].body().get_value() << std::endl;

        for (int n=0; n<N; n++)
            c[n].set_value( uniform() );
        std::cout << std::endl;
        }
    }
else {
    coek::NLPModel nlp(model, "cppad");
    coek::NLPSolver opt("ipopt");
    opt.set_option("print_level", 0);
    opt.load(nlp);

    auto curr = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = curr-start;
    std::cout << "Time to build model: " << diff.count() << " s\n";
    start = curr;

    for (int i=0; i<nsolves; i++) {
        opt.resolve();

        auto curr = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = curr-start;
        std::cout << "Time to solve model: " << diff.count() << " s\n";
        start = curr;

        std::cout << "Solve: " << 0 << "  Objective: " << nlp.compute_f() << std::endl;

        for (int n=0; n<N; n++)
            c[n].set_value( uniform() );
        std::cout << std::endl;
        }
    }
}


void row_test(int N, int nsolves, const std::string& solver)
{
// Make this row have random values
int row = std::min(10, N-1);

std::mt19937 rng(10000) ;
std::uniform_real_distribution<double> distribution(0,1);
auto uniform = std::bind( distribution, rng );

coek::Model model;

std::vector<coek::Parameter> A_(N);
for (int m=0; m<N; m++)
    A_[m] = coek::Parameter(uniform());

std::vector<std::vector<double>> A(N, std::vector<double>(N));
for (int n=0; n<N; n++)
    for (int m=0; m<N; m++)
        A[n][m] = uniform();

std::vector<double> c(N);
for (int n=0; n<N; n++)
    c[n] = uniform();

std::vector<double> b(N);
for (int n=0; n<N; n++)
    b[n] = uniform();

std::vector<coek::Variable> x(N);
for (int n=0; n<N; n++)
    x[n] = model.getVariable(0, COEK_INFINITY, 0);

// obj
coek::Expression obj;
for (int n=0; n<N; n++)
    obj += c[n] * x[n];
model.add( obj, coek::Model::maximize );

// Ax <= b
for (int n=0; n<N; n++) {
    coek::Expression expr;
    if (n != row) {
        for (int m=0; m<N; m++)
            expr += A[n][m] * x[m];
        }
    else {
        for (int m=0; m<N; m++)
            expr += A_[m] * x[m];
        }
    model.add( expr <= b[n] );
    }


if (solver == "gurobi") {
    coek::Solver opt(solver);
    opt.load(model);
    for (int i=0; i<nsolves; i++) {
        opt.resolve();
        std::cout << "Solve: " << i << "  Objective: " << model.repn->objectives[0].body().get_value() << std::endl;

        for (int m=0; m<N; m++)
            A_[m].set_value( uniform() );
        std::cout << std::endl;
        }
    }
else {
    coek::NLPModel nlp(model, "cppad");
    coek::NLPSolver opt("ipopt");
    opt.set_option("print_level", 0);
    opt.load(nlp);
    for (int i=0; i<nsolves; i++) {
        opt.resolve();
        std::cout << "Solve: " << 0 << "  Objective: " << nlp.compute_f() << std::endl;

        for (int m=0; m<N; m++)
            A_[m].set_value( uniform() );
        std::cout << std::endl;
        }
    }
}


void col_test(int N, int nsolves, const std::string& solver)
{
// Make this col have random values
int col = std::min(10, N-1);

std::mt19937 rng(10000) ;
std::uniform_real_distribution<double> distribution(0,1);
auto uniform = std::bind( distribution, rng );

coek::Model model;

std::vector<coek::Parameter> A_(N);
for (int n=0; n<N; n++)
    A_[n] = coek::Parameter(uniform());

std::vector<std::vector<double>> A(N, std::vector<double>(N));
for (int n=0; n<N; n++)
    for (int m=0; m<N; m++)
        A[n][m] = uniform();

std::vector<double> c(N);
for (int n=0; n<N; n++)
    c[n] = uniform();

std::vector<double> b(N);
for (int n=0; n<N; n++)
    b[n] = uniform();

std::vector<coek::Variable> x(N);
for (int n=0; n<N; n++)
    x[n] = model.getVariable(0, COEK_INFINITY, 0);

// obj
coek::Expression obj;
for (int n=0; n<N; n++)
    obj += c[n] * x[n];
model.add( obj, coek::Model::maximize );

// Ax <= b
for (int n=0; n<N; n++) {
    coek::Expression expr;
    for (int m=0; m<N; m++)
        if (m != col) {
            expr += A[n][m] * x[m];
        }
        else {
            expr += A_[n] * x[m];
        }
    model.add( expr <= b[n] );
    }


if (solver == "gurobi") {
    coek::Solver opt(solver);
    opt.load(model);
    for (int i=0; i<nsolves; i++) {
        opt.resolve();
        std::cout << "Solve: " << i << "  Objective: " << model.repn->objectives[0].body().get_value() << std::endl;

        for (int n=0; n<N; n++)
            A_[n].set_value( uniform() );
        std::cout << std::endl;
        }
    }
else {
    coek::NLPModel nlp(model, "cppad");
    coek::NLPSolver opt("ipopt");
    opt.set_option("print_level", 0);
    opt.load(nlp);
    for (int i=0; i<nsolves; i++) {
        opt.resolve();
        std::cout << "Solve: " << 0 << "  Objective: " << nlp.compute_f() << std::endl;

        for (int n=0; n<N; n++)
            A_[n].set_value( uniform() );
        std::cout << std::endl;
        }
    }
}


void rhs_test(int N, int nsolves, const std::string& solver)
{
std::mt19937 rng(10000) ;
std::uniform_real_distribution<double> distribution(0,1);
auto uniform = std::bind( distribution, rng );

coek::Model model;

std::vector<std::vector<double>> A(N, std::vector<double>(N));
for (int n=0; n<N; n++)
    for (int m=0; m<N; m++)
        A[n][m] = uniform();

std::vector<double> c(N);
for (int n=0; n<N; n++)
    c[n] = uniform();

std::vector<coek::Parameter> b(N);
for (int n=0; n<N; n++)
    b[n] = coek::Parameter(uniform());

std::vector<coek::Variable> x(N);
for (int n=0; n<N; n++)
    x[n] = model.getVariable(0, COEK_INFINITY, 0);

// obj
coek::Expression obj;
for (int n=0; n<N; n++)
    obj += c[n] * x[n];
model.add( obj, coek::Model::maximize );

// Ax <= b
for (int n=0; n<N; n++) {
    coek::Expression expr;
    for (int m=0; m<N; m++)
        expr += A[n][m] * x[m];
    model.add( expr <= b[n] );
    }


if (solver == "gurobi") {
    coek::Solver opt(solver);
    opt.load(model);
    for (int i=0; i<nsolves; i++) {
        opt.resolve();
        std::cout << "Solve: " << i << "  Objective: " << model.repn->objectives[0].body().get_value() << std::endl;

        for (int n=0; n<N; n++)
            b[n].set_value( uniform() );
        std::cout << std::endl;
        }
    }
else {
    coek::NLPModel nlp(model, "cppad");
    coek::NLPSolver opt("ipopt");
    opt.set_option("print_level", 0);
    opt.load(nlp);
    for (int i=0; i<nsolves; i++) {
        opt.resolve();
        std::cout << "Solve: " << 0 << "  Objective: " << nlp.compute_f() << std::endl;

        for (int n=0; n<N; n++)
            b[n].set_value( uniform() );
        std::cout << std::endl;
        }
    }
}
