#ifndef __coek_model_hpp
#define __coek_model_hpp

#pragma once

#include <string>
#include <map>
#include <set>
#include <memory>
#include <vector>
#include <unordered_map>
#include <coek/api/constants.hpp>

#ifdef COEK_WITH_COMPACT_MODEL
#include <variant>
#include <coek/compact/coek_sets.hpp>
#include <coek/compact/coek_indexed.hpp>
#endif

//#include "coek/api/constraint.hpp"
//#include "coek/api/objective.hpp"


namespace coek {

class VariableArray;
class Objective;
class Variable;
class Parameter;
class Expression;
class Constraint;
//class ExpressionSeqIteratorRepn;
//class ConstraintSeqIteratorRepn;
class SequenceContext;
class ObjectiveSequence;
class ConstraintSequence;

class ModelRepn;
class NLPModelRepn;
class SolverRepn;
class NLPSolverRepn;


// TODO - Move to *.cpp file
class ModelRepn
{
public:

    std::vector<Objective> objectives;
    std::vector<Constraint> constraints;
    std::vector<Variable> variables;

    std::map<std::string, Objective> objectives_by_name;
    std::map<std::string, Constraint> constraints_by_name;
    std::map<std::string, Variable> variables_by_name;

    std::map<std::string, std::unordered_map<unsigned int,double> > vsuffix;
    std::map<std::string, std::unordered_map<unsigned int,double> > csuffix;
    std::map<std::string, std::unordered_map<unsigned int,double> > osuffix;
    std::map<std::string, double > msuffix;
};

//
// Coek Model
//
// TODO - Define SimpeModel and AbstractModel.  Operations like get_constraint(i) may not make sense for
// an abstract model, and even when they do, these are probably more expensive operations.
//
class Model
{
public:

    static bool minimize;
    static bool maximize;
    static double inf;

    std::shared_ptr<ModelRepn> repn;

public:

    Model();
    Model(const Model& other);
    ~Model();
    Model& operator=(const Model&);

    //
    // Variables
    //
    size_t num_variables() const;
    std::set<std::string> variable_names() const;

    Variable add_variable(const std::string& name, double lb=-COEK_INFINITY, double ub=COEK_INFINITY, double value=COEK_NAN, bool binary=false, bool integer=false);
    Variable add_variable(double lb=-COEK_INFINITY, double ub=COEK_INFINITY, double value=COEK_NAN, bool binary=false, bool integer=false);
    Variable add_variable(Variable& var);
    void add_variable(VariableArray& var);
#ifdef COEK_WITH_COMPACT_MODEL
    void add_variable(ConcreteIndexedVariable& var);
#endif

    Variable get_variable(unsigned int i);
    Variable get_variable(const std::string& name);

    //
    // Objectives
    //
    size_t num_objectives() const;
    std::set<std::string> objective_names() const;

    Objective add_objective(const Expression& expr, bool _sense=Model::minimize);
    Objective add_objective(const std::string& name, const Expression& expr, bool _sense=Model::minimize);

    Objective get_objective(unsigned int i=0);
    Objective get_objective(const std::string& name);

    //
    // Constraint
    //
    size_t num_constraints() const;
    std::set<std::string> constraint_names() const;

    Constraint add_constraint(const Constraint& expr);
    Constraint add_constraint(const std::string& name, const Constraint& expr);

    Constraint get_constraint(unsigned int i);
    Constraint get_constraint(const std::string& name);

    //
    // Suffixes
    //
    std::set<std::string> variable_suffix_names() const;
    std::set<std::string> objective_suffix_names() const;
    std::set<std::string> constraint_suffix_names() const;
    std::set<std::string> model_suffix_names() const;

    void set_suffix(const std::string& name, Variable& var, double value);
    void set_suffix(const std::string& name, Constraint& con, double value);
    void set_suffix(const std::string& name, Objective& obj, double value);
    void set_suffix(const std::string& name, double value);

    double get_suffix(const std::string& name, Variable& var);
    double get_suffix(const std::string& name, Constraint& con);
    double get_suffix(const std::string& name, Objective& obj);
    double get_suffix(const std::string& name);

    //
    // I/O
    //

    void write(std::string filename);
    void write(std::string filename, std::map<int,int>& varmap, std::map<int,int>& conmap);
    void print_equations() const;
    void print_values() const;
    void print_equations(std::ostream& ostr) const;
    void print_values(std::ostream& ostr) const;

    friend std::ostream& operator<<(std::ostream& ostr, const Model& arg);
};


#ifdef COEK_WITH_COMPACT_MODEL
class CompactModel
{
public:

    // TODO - define ObjectiveSequence
    std::vector<std::variant<Objective, ObjectiveSequence>> objectives;
    std::vector<std::variant<Constraint, ConstraintSequence>> constraints;
    std::vector<Variable> variables;

public:

    Objective add_objective(const Expression& expr, bool _sense=Model::minimize);
    void add_objective(const Expression& expr, const SequenceContext& context, bool _sense=Model::minimize);

    Constraint add_constraint(const Constraint& expr);
    void add_constraint(const Constraint& expr, const SequenceContext& context);

    Variable add_variable(double lb=-COEK_INFINITY, double ub=COEK_INFINITY, double value=COEK_NAN, bool binary=false, bool integer=false);
    Variable add_variable(const std::string& name, double lb=-COEK_INFINITY, double ub=COEK_INFINITY, double value=COEK_NAN, bool binary=false, bool integer=false);
    Variable add_variable(Variable& var);
    void add_variable(VariableArray& var);
    void add_variable(ConcreteIndexedVariable& var);

    Model expand();

    void write(std::string filename);
    void write(std::string filename, std::map<int,int>& varmap, std::map<int,int>& conmap);
};
#endif

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

    bool available() const;

    int solve(Model& model);
    void load(Model& model);

    #ifdef COEK_WITH_COMPACT_MODEL
    int solve(CompactModel& model);
    void load(CompactModel& model);
    #endif

    int resolve();
    void reset();

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

public:

    NLPModel();
    NLPModel(Model& model, std::string type, bool sparse_JH=true);

    void initialize(Model& model, std::string type, bool sparse_JH=true);
    void reset();   // TODO - maybe this should be called 'update'?

    size_t num_variables() const;
    size_t num_objectives() const;
    size_t num_constraints() const;
    size_t num_nonzeros_Jacobian() const;
    size_t num_nonzeros_Hessian_Lagrangian() const;

    Variable get_variable(size_t i);
    void set_variable(size_t i, const Variable& v);

    void set_variables(std::vector<double>& x);
    void set_variables(const double* x, size_t n);

    Constraint get_constraint(size_t i);

    void get_J_nonzeros(std::vector<size_t>& jrow, std::vector<size_t>& jcol);
    void get_H_nonzeros(std::vector<size_t>& hrow, std::vector<size_t>& hcol);

    void write(std::string filename);
    void write(std::string filename, std::map<int,int>& varmap, std::map<int,int>& conmap);
    void print_equations() const;
    void print_values() const;
    void print_equations(std::ostream& ostr) const;
    void print_values(std::ostream& ostr) const;

    friend std::ostream& operator<<(std::ostream& ostr, const NLPModel& arg);

public:

    /// COMPUTE Value of Specific Objective Function

    double compute_f(unsigned int i=0);

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
    void reset();
    
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

Model read_problem_from_jpof_file(const std::string& filename, std::map<std::string,Parameter>& params);
Model read_problem_from_jpof_string(const std::string& jpof, std::map<std::string,Parameter>& params);

}

#endif
