#pragma once

#include <memory>
#include <vector>
#include "coek_expr.hpp"

namespace coek {

class NLPModelRepn;
class SolverRepn;
class NLPSolverRepn;


// Coek Model
class Model
{
public:

    static bool minimize;
    static bool maximize;
    static double inf;

    std::vector<bool> sense;
    std::vector<Expression> objectives;
    std::vector<Constraint> constraints;
    std::vector<Variable> variables;

public:

    void add(const Expression& expr, bool _sense=Model::minimize)
        {
        objectives.push_back( expr );
        sense.push_back(_sense);
        }

    void add(const Constraint& expr)
        {
        constraints.push_back( expr );
        }

    Variable& getVariable(double lb, double ub, const std::string& name);
    Variable& getVariable(double lb=-COEK_INFINITY, double ub=COEK_INFINITY, double value=0.0);
    Variable& getVariable(double lb, double ub, double value, const std::string& name);
    Variable& getVariable(double lb, double ub, double value, bool binary, bool integer, const std::string& name);
    void addVariable(Variable& var);

    void write(std::string ostr);

    friend std::ostream& operator<<(std::ostream& ostr, const Model& arg);
};


// Coek Solver
class Solver
{
public:

    std::shared_ptr<SolverRepn> repn;

public:

    Solver(void)
        {}
    Solver(std::string solver)
        {initialize(solver);}

    void initialize(std::string solver);

    int solve(Model& model);

    void load(Model& model);
    int resolve();

    // Solver Options

    bool get_option(const std::string& option, int& value) const;
    bool get_option(const std::string& option, double& value) const;
    bool get_option(const std::string& option, std::string& value) const;
    bool get_option(int option, int& value) const;
    bool get_option(int option, double& value) const;
    bool get_option(int option, std::string& value) const;

    void set_option(const std::string& option, int value);
    void set_option(const std::string& option, double value);
    void set_option(const std::string& option, const std::string value);
    void set_option(int option, int value);
    void set_option(int option, double value);
    void set_option(int option, const std::string value);

};


// Coek NLPModel
class NLPModel
{
public:

    std::shared_ptr<NLPModelRepn> repn;

    Model model;

public:

    NLPModel();
    NLPModel(Model& model, std::string type, bool sparse_JH=true);

    void initialize(Model& model, std::string type, bool sparse_JH=true);
    void reset();

    size_t num_variables() const;
    size_t num_objectives() const;
    size_t num_constraints() const;
    size_t num_nonzeros_Jacobian() const;
    size_t num_nonzeros_Hessian_Lagrangian() const;

    Variable get_variable(size_t i);
    void set_variable(size_t i, const Variable& v);

    void set_variables(std::vector<double>& x);
    void set_variables(const double* x, size_t n);

    void get_J_nonzeros(std::vector<size_t>& jrow, std::vector<size_t>& jcol);
    void get_H_nonzeros(std::vector<size_t>& hrow, std::vector<size_t>& hcol);

    void write(std::ostream& ostr) const;

    friend std::ostream& operator<<(std::ostream& ostr, const NLPModel& arg);

public:

    /// COMPUTE Value of Specific Objective Function

    double compute_f(unsigned int i);

    double compute_f(std::vector<double>& x)
        {
        set_variables(x);
        return compute_f(0);
        }

    double compute_f(std::vector<double>& x, unsigned int i)
        {
        set_variables(x);
        return compute_f(i);
        }

    /// COMPUTE Gradient of Specified Objective Function

    void compute_df(double& f, std::vector<double>& df, unsigned int i);

    void compute_df(std::vector<double>& x, std::vector<double>& df)
        {
        set_variables(x);
        double f;
        compute_df(f, df, 0);
        }

    void compute_df(std::vector<double>& x, std::vector<double>& df, unsigned int i)
        {
        set_variables(x);
        double f;
        compute_df(f, df, i);
        }

    void compute_df(double& f, std::vector<double>& df)
        { compute_df(f, df, 0); }

    void compute_df(std::vector<double>& df)
        { double f; compute_df(f, df, 0); }

    void compute_df(std::vector<double>& df, unsigned int i)
        { double f; compute_df(f, df, i); }

    /// COMPUTE Hessian of Lagrangian

    void compute_H(std::vector<double>& w, std::vector<double>& H);

    void compute_H(std::vector<double>& x, std::vector<double>& w, std::vector<double>& H)
        {
        set_variables(x);
        compute_H(w, H);
        }

    /// COMPUTE Constraint Values

    void compute_c(std::vector<double>& c);

    void compute_c(std::vector<double>& x, std::vector<double>& c)
        {
        set_variables(x);
        compute_c(c);
        }

    /// COMPUTE Gradient of Specific Constraint

    void compute_dc(std::vector<double>& dc, unsigned int i);

    void compute_dc(std::vector<double>& x, std::vector<double>& dc, unsigned int i)
        {
        set_variables(x);
        compute_dc(dc, i);
        }

    /// COMPUTE Jacobian

    void compute_J(std::vector<double>& J);

    void compute_J(std::vector<double>& x, std::vector<double>& J)
        {
        set_variables(x);
        compute_J(J);
        }
}; 


// Coek NLPSolver
class NLPSolver
{
public:

    std::shared_ptr<NLPSolverRepn> repn;

public:

    NLPSolver(void)
        {}
    NLPSolver(std::string solver)
        {initialize(solver);}

    void initialize(std::string solver);

    int solve(NLPModel& model);

    void load(NLPModel& model);
    int resolve();
    
    // Solver Options

    bool get_option(const std::string& option, int& value) const;
    bool get_option(const std::string& option, double& value) const;
    bool get_option(const std::string& option, std::string& value) const;
    bool get_option(int option, int& value) const;
    bool get_option(int option, double& value) const;
    bool get_option(int option, std::string& value) const;

    void set_option(const std::string& option, int value);
    void set_option(const std::string& option, double value);
    void set_option(const std::string& option, const std::string value);
    void set_option(int option, int value);
    void set_option(int option, double value);
    void set_option(int option, const std::string value);

};


//
// operator<<
//
std::ostream& operator<<(std::ostream& ostr, const Model& arg);
std::ostream& operator<<(std::ostream& ostr, const NLPModel& arg);

}
