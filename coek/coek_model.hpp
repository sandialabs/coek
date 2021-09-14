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


/**
  * An optimization model that contains objectives and constraints.
  *
  * Variables are added to this model to facilitate error checking and 
  * enable efficient processing of model expressions.
  */
class Model
{
public:

    /** A constant used to indicate a minimization objective */
    static bool minimize;
    /** A constant used to indicate a maximization objective */
    static bool maximize;
    /** A constant that indicates an infinite value */
    static double inf;

    std::shared_ptr<ModelRepn> repn;

public:

    /** Constructs an empty optimization model */
    Model();
    Model(const Model& other);
    ~Model();
    Model& operator=(const Model&);

    //
    // Variables
    //
    /** \returns the number of variables in the model */
    size_t num_variables() const;
    /** \returns a list of variable names in the model */
    std::set<std::string> variable_names() const;

    /** Create a variable and add it to the model with standard arguments.
      *
      * This method accepts one or more standard options.  The first argument is the 
      * name of the variable. When default values are used, this specifies an unbounded,
      * continuous decision variable.
      *
      * \param name   the name of variable
      * \param lb   the lower bound (default is negative infinity)
      * \param ub   the upper bound (default is infinity)
      * \param value   the initial value (default is 0.0)
      * \param binary   a boolean flag that indicates whether the variable is boolean (default is \c false)
      * \param integer   a boolean flag that indicates whether the variable is a general integer (default is \c false)
      *
      * \returns the variable object
      */
    Variable add_variable(const std::string& name, double lb=-COEK_INFINITY, double ub=COEK_INFINITY, double value=COEK_NAN, bool binary=false, bool integer=false);
    /** Create a variable and add it to the model with standard arguments.
      *
      * This method accepts one or more standard options.  
      * When default values are used, this specifies an unbounded,
      * continuous decision variable.
      *
      * \param lb   the lower bound (default is negative infinity)
      * \param ub   the upper bound (default is infinity)
      * \param value   the initial value (default is 0.0)
      * \param binary   a boolean flag that indicates whether the variable is boolean (default is \c false)
      * \param integer   a boolean flag that indicates whether the variable is a general integer (default is \c false)
      *
      * \returns the variable object
      */
    Variable add_variable(double lb=-COEK_INFINITY, double ub=COEK_INFINITY, double value=COEK_NAN, bool binary=false, bool integer=false);
    /** Add a variable to the model.
      *
      * \returns the variable
      */
    Variable add_variable(Variable& var);
    void add_variable(VariableArray& var);
#ifdef COEK_WITH_COMPACT_MODEL
    void add_variable(ConcreteIndexedVariable& var);
#endif

    /** \returns the variable with the specified id */
    Variable get_variable(unsigned int i);
    /** \returns the variable with the specified name */
    Variable get_variable(const std::string& name);

    //
    // Objectives
    //
    /** \returns the number of objecives in the model */
    size_t num_objectives() const;
    /** \returns a list of objective names in the model */
    std::set<std::string> objective_names() const;

    /**
      * Create an objective and add it to the model.
      *
      * \param expr   the objective expression
      * \param _sense   A boolean that is \c true if the sense is minimize
      *
      * \returns the objective
      */
    Objective add_objective(const Expression& expr, bool _sense=Model::minimize);
    /**
      * Create an objective and add it to the model.
      *
      * \param name   the objective name
      * \param expr   the objective expression
      * \param sense   A boolean that is \c true if the sense is minimize
      *
      * \returns the objective
      */
    Objective add_objective(const std::string& name, const Expression& expr, bool _sense=Model::minimize);

    /** \returns the objective with the specified id */
    Objective get_objective(unsigned int i=0);
    /** \returns the objective with the specified name */
    Objective get_objective(const std::string& name);

    //
    // Constraint
    //
    /** \returns the number of constraints in the model */
    size_t num_constraints() const;
    /** \returns a list of constraint names in the model */
    std::set<std::string> constraint_names() const;

    /**
      * Add a constraint to the model.
      *
      * \returns the constraint
      */
    Constraint add_constraint(const Constraint& expr);
    /**
      * Add a constraint to the model.
      *
      * \param name the constraint name
      * \param expr the constraint expression
      *
      * \returns the constraint
      */
    Constraint add_constraint(const std::string& name, const Constraint& expr);

    /** \returns the constraint with the specified id */
    Constraint get_constraint(unsigned int i);
    /** \returns the constraint with the specified name */
    Constraint get_constraint(const std::string& name);

    //
    // Suffixes
    //
    /** \returns a set of names for variable suffixes */
    std::set<std::string> variable_suffix_names() const;
    /** \returns a set of names for objective suffixes */
    std::set<std::string> objective_suffix_names() const;
    /** \returns a set of names for constraint suffixes */
    std::set<std::string> constraint_suffix_names() const;
    /** \returns a set of names for model suffixes */
    std::set<std::string> model_suffix_names() const;

    /** Set a variable suffix
      *
      * \param name  the suffix name
      * \param var  the variable
      * \param value  the suffix value
      */
    void set_suffix(const std::string& name, Variable& var, double value);
    /** Set a constraint suffix
      *
      * \param name  the suffix name
      * \param con  the constraint
      * \param value  the suffix value
      */
    void set_suffix(const std::string& name, Constraint& con, double value);
    /** Set an objective suffix
      *
      * \param name  the suffix name
      * \param obj  the objective
      * \param value  the suffix value
      */
    void set_suffix(const std::string& name, Objective& obj, double value);
    /** Set a model suffix
      *
      * \param name  the suffix name
      * \param value  the suffix value
      */
    void set_suffix(const std::string& name, double value);

    /** \returns a variable suffix */
    double get_suffix(const std::string& name, Variable& var);
    /** \returns a constraint suffix */
    double get_suffix(const std::string& name, Constraint& con);
    /** \returns an objective suffix */
    double get_suffix(const std::string& name, Objective& obj);
    /** \returns a model suffix */
    double get_suffix(const std::string& name);

    //
    // I/O
    //

    /** Write the model to the specified file */
    void write(std::string filename);
    /** Write the model to the specified file
      *
      * \param filename  the output file
      * \param varmap  varmap[i] contains the id of the i-th variable in this model
      * \param conmap  conmap[i] contains the id of the i-th constraint in this model
      */
    void write(std::string filename, std::map<int,int>& varmap, std::map<int,int>& conmap);
    /** Print the equations in the model to \c std::cout */
    void print_equations() const;
    /** Print the equations in the model to the specified output stream */
    void print_equations(std::ostream& ostr) const;
    /** Print the values in the model to \c std::cout */
    void print_values() const;
    /** Print the values in the model to the specified output stream */
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

/**
  * An optimization solver object that is initialized using the optimizer string name.
  *
  * These solvers can be used to optimize coek::Model objects.
  */
class Solver
{
public:

    std::shared_ptr<SolverRepn> repn;

public:

    /** Creates an empty solver */
    Solver(void)
        {}
    /** Creates a solver that is initialized using the solver name */
    Solver(std::string solver)
        {initialize(solver);}

    /** Initialize the solver with the specified name */
    void initialize(std::string solver);

    /** \returns \c true if the solver is available */
    bool available() const;

    /** Optimize the specified model
      *
      * \returns an error code that is nonzero if an error occurs */
    int solve(Model& model);
    /** Load a model */
    void load(Model& model);

    #ifdef COEK_WITH_COMPACT_MODEL
    int solve(CompactModel& model);
    void load(CompactModel& model);
    #endif

    /* Resolve a model that has been loaded
     *
     * \returns an error code that is nonzero if an error occurs */
    int resolve();
    /** Resets the state of the optimizer */
    void reset();

    // Solver Options

    /** Get the value of an integer option
      *
      * The option value is returned by reference if it has
      * a value.
      *
      * \param option  the option name
      * \param value   an integer value that is passed by reference
      *
      * \returns \c true if the option is found
      */
    bool get_option(const std::string& option, int& value) const;
    /** Get the value of a double option
      *
      * The option value is returned by reference if it has
      * a value.
      *
      * \param option  the option name
      * \param value   a double value that is passed by reference
      *
      * \returns \c true if the option is found
      */
    bool get_option(const std::string& option, double& value) const;
    /** Get the value of a string option
      *
      * The option value is returned by reference if it has
      * a value.
      *
      * \param option  the option name
      * \param value   a string value that is passed by reference
      *
      * \returns \c true if the option is found
      */
    bool get_option(const std::string& option, std::string& value) const;
    /** Get the value of an integer option
      *
      * The option value is returned by reference if it has
      * a value.
      *
      * \param option  the option integer id
      * \param value   an integer value that is passed by reference
      *
      * \returns \c true if the option is found
      */
    bool get_option(int option, int& value) const;
    /** Get the value of a double option
      *
      * The option value is returned by reference if it has
      * a value.
      *
      * \param option  the option integer id
      * \param value   a double value that is passed by reference
      *
      * \returns \c true if the option is found
      */
    bool get_option(int option, double& value) const;
    /** Get the value of a string option
      *
      * The option value is returned by reference if it has
      * a value.
      *
      * \param option  the option integer id
      * \param value   a string value that is passed by reference
      *
      * \returns \c true if the option is found
      */
    bool get_option(int option, std::string& value) const;

    /** Set an integer option
      *
      * \param option  the option name
      * \param value   the integer value
      */
    void set_option(const std::string& option, int value);
    /** Set a double option
      *
      * \param option  the option name
      * \param value   the double value
      */
    void set_option(const std::string& option, double value);
    /** Set a string option
      *
      * \param option  the option name
      * \param value   the string value
      */
    void set_option(const std::string& option, const std::string value);
    /** Set an integer option
      *
      * \param option  the option integer id
      * \param value   the integer value
      */
    void set_option(int option, int value);
    /** Set a double option
      *
      * \param option  the option integer id
      * \param value   the double value
      */
    void set_option(int option, double value);
    /** Set a string option
      *
      * \param option  the option integer id
      * \param value   the string value
      */
    void set_option(int option, const std::string value);

};


/**
  * An optimization model that provides a view of a coek::Model
  * object that support computations needed for NLP solvers.
  */
class NLPModel
{
public:

    std::shared_ptr<NLPModelRepn> repn;

public:

    /** Create an empty NLP model view */
    NLPModel();
    /** Create a NLP model view
      *
      * \param model  a coek::Model
      * \param type  the type of AD used for computations
      * \param sparse_JH  a boolean that indicates if sparse Jacobian and Hessian computations are used (default is \c true)
      */
    NLPModel(Model& model, std::string type, bool sparse_JH=true);

    /** Initialize the NLP model view
      *
      * \param model  a coek::Model
      * \param type  the type of AD used for computations
      * \param sparse_JH  a boolean that indicates if sparse Jacobian and Hessian computations are used (default is \c true)
      */
    void initialize(Model& model, std::string type, bool sparse_JH=true);

    /** TODO - maybe this should be called 'update' */
    void reset();

    /** \returns the number of variables in the model */
    size_t num_variables() const;
    /** \returns the number of objectives in the model */
    size_t num_objectives() const;
    /** \returns the number of constraints in the model */
    size_t num_constraints() const;
    /** \returns the number of nonzeros in the Jacobian */
    size_t num_nonzeros_Jacobian() const;
    /** \returns the number of nonzeros in the Hesian Lagrangian */
    size_t num_nonzeros_Hessian_Lagrangian() const;

    /** \returns the i-th variable in the model view */
    Variable get_variable(size_t i);
    /** Sets the i-th variable in the model view */
    void set_variable(size_t i, const Variable& v);

    /** Sets the value of variables in the model view */
    void set_variables(std::vector<double>& x);
    /** Sets the value of variables in the model view */
    void set_variables(const double* x, size_t n);

    /** \returns the i-th constraint in the model view */
    Constraint get_constraint(size_t i);

    /**
      * Return the row and column indices of the Jacobian nonzeros.
      *
      * \param jrow   vector that is filled with row indices
      * \param jcol   vector that is filled with column indices
      */
    void get_J_nonzeros(std::vector<size_t>& jrow, std::vector<size_t>& jcol);
    /**
      * Return the row and column indices of the Jacobian nonzeros.
      *
      * \param hrow   vector that is filled with row indices
      * \param hcol   vector that is filled with column indices
      */
    void get_H_nonzeros(std::vector<size_t>& hrow, std::vector<size_t>& hcol);

    /** Write the model to the specified file */
    void write(std::string filename);
    /** Write the model to the specified file
      *
      * \param filename  the output file
      * \param varmap  varmap[i] contains the id of the i-th variable in this model
      * \param conmap  conmap[i] contains the id of the i-th constraint in this model
      */
    void write(std::string filename, std::map<int,int>& varmap, std::map<int,int>& conmap);
    /** Print the equations in the model to \c std::cout */
    void print_equations() const;
    /** Print the equations in the model to the specified output stream */
    void print_equations(std::ostream& ostr) const;
    /** Print the values in the model to \c std::cout */
    void print_values() const;
    /** Print the values in the model to the specified output stream */
    void print_values(std::ostream& ostr) const;

    friend std::ostream& operator<<(std::ostream& ostr, const NLPModel& arg);

public:

    /**
      * Compute the value of the i-th objective function.
      *
      * This method uses the variable values stored in the model.
      *
      * \param i   objective index (default is 0)
      *
      * \returns value of the specified objective function.
      */
    double compute_f(unsigned int i=0);
    /**
      * Compute the value of the 0-th objective function.
      *
      * This method stores the variable values x in the model.
      *
      * \param x   variable values that are stored in the model
      *
      * \returns value of the specified objective function.
      */
    double compute_f(std::vector<double>& x)
        {
        set_variables(x);
        return compute_f(0);
        }
    /**
      * Compute the value of the i-th objective function.
      *
      * This method stores the variable values x in the model.
      *
      * \param x   variable values that are stored in the model
      * \param i   objective index (default is 0)
      *
      * \returns value of the specified objective function.
      */
    double compute_f(std::vector<double>& x, unsigned int i)
        {
        set_variables(x);
        return compute_f(i);
        }

    /**
      * Compute the value and gradient of the i-th objective function.
      *
      * This method uses the variable values stored in the model.
      *
      * \param f   reference that stores the objective value
      * \param df   reference that stores the gradient 
      * \param i   objective index
      */
    void compute_df(double& f, std::vector<double>& df, unsigned int i);
    /**
      * Compute the value and gradient of the 0-th objective function.
      *
      * This method uses the variable values stored in the model.
      *
      * \param f   reference that stores the objective value
      * \param df   reference that stores the gradient 
      */
    void compute_df(double& f, std::vector<double>& df)
        { compute_df(f, df, 0); }
    /**
      * Compute the gradient of the 0-th objective function.
      *
      * This method uses the variable values stored in the model.
      *
      * \param df   reference that stores the gradient 
      */
    void compute_df(std::vector<double>& df)
        { double f; compute_df(f, df, 0); }
    /**
      * Compute the gradient of the i-th objective function.
      *
      * This method uses the variable values stored in the model.
      *
      * \param df   reference that stores the gradient 
      * \param i   objective index
      */
    void compute_df(std::vector<double>& df, unsigned int i)
        { double f; compute_df(f, df, i); }
    /**
      * Compute the gradient of the i-th objective function.
      *
      * This method stores the variable values in the model.
      *
      * \param x   variable values that are stored in the model
      * \param df   reference that stores the gradient 
      * \param i   objective index
      */
    void compute_df(std::vector<double>& x, std::vector<double>& df, unsigned int i)
        {
        set_variables(x);
        double f;
        compute_df(f, df, i);
        }
    /**
      * Compute the gradient of the 0-th objective function.
      *
      * This method stores the variable values in the model.
      *
      * \param x   variable values that are stored in the model
      * \param df   reference that stores the gradient 
      */
    void compute_df(std::vector<double>& x, std::vector<double>& df)
        {
        set_variables(x);
        double f;
        compute_df(f, df, 0);
        }

    /**
      * Compute the Hessian of Lagrangian 
      *
      * This method uses the variable values stored in the model.
      *
      * \param w   weights in the Lagrangian
      * \param H   reference that stores the Hessian 
      */
    void compute_H(std::vector<double>& w, std::vector<double>& H);
    /**
      * Compute the Hessian of Lagrangian 
      *
      * This method stores the variable values in the model.
      *
      * \param x   variable values that are stored in the model
      * \param w   weights in the Lagrangian
      * \param H   reference that stores the Hessian 
      */
    void compute_H(std::vector<double>& x, std::vector<double>& w, std::vector<double>& H)
        {
        set_variables(x);
        compute_H(w, H);
        }

    /**
      * Compute constraint values
      *
      * This method uses the variable values stored in the model.
      *
      * \param c   reference that stores the constraint values
      */
    void compute_c(std::vector<double>& c);
    /**
      * Compute constraint values
      *
      * This method stores the variable values in the model.
      *
      * \param x   variable values that are stored in the model
      * \param c   reference that stores the constraint values
      */
    void compute_c(std::vector<double>& x, std::vector<double>& c)
        {
        set_variables(x);
        compute_c(c);
        }

    /**
      * Compute the gradient of the i-th constraint function.
      *
      * This method uses the variable values stored in the model.
      *
      * \param dc   reference that stores the gradient 
      * \param i   constraint index
      */
    void compute_dc(std::vector<double>& dc, unsigned int i);
    /**
      * Compute the gradient of the i-th constraint function.
      *
      * This method stores the variable values in the model.
      *
      * \param x   variable values that are stored in the model
      * \param dc   reference that stores the gradient 
      * \param i   constraint index
      */
    void compute_dc(std::vector<double>& x, std::vector<double>& dc, unsigned int i)
        {
        set_variables(x);
        compute_dc(dc, i);
        }

    /**
      * Compute the Jacobian
      *
      * This method uses the variable values stored in the model.
      *
      * \param J   reference that stores the Jacobian 
      */
    void compute_J(std::vector<double>& J);
    /**
      * Compute the Jacobian
      *
      * This method stores the variable values in the model.
      *
      * \param x   variable values that are stored in the model
      * \param J   reference that stores the Jacobian 
      */
    void compute_J(std::vector<double>& x, std::vector<double>& J)
        {
        set_variables(x);
        compute_J(J);
        }
}; 


/**
  * An optimization solver object that is initialized using the optimizer string name.
  *
  * These solvers can be used to optimize coek::NLPModel objects.
  */
class NLPSolver
{
public:

    std::shared_ptr<NLPSolverRepn> repn;

public:

    /** Creates an empty solver */
    NLPSolver(void)
        {}
    /** Creates a solver that is initialized using the solver name */
    NLPSolver(std::string solver)
        {initialize(solver);}

    /** Initialize the solver with the specified name */
    void initialize(std::string solver);

    /** \returns \c true if the solver is available */
    bool available() const;

    /** Optimize the specified model
      *
      * \returns an error code that is nonzero if an error occurs */
    int solve(NLPModel& model);

    /** Load a model */
    void load(NLPModel& model);
    /* Resolve a model that has been loaded
     *
     * \returns an error code that is nonzero if an error occurs */
    int resolve();
    /** Resets the state of the optimizer */
    void reset();
    
    /** Get the value of an integer option
      *
      * The option value is returned by reference if it has
      * a value.
      *
      * \param option  the option name
      * \param value   an integer value that is passed by reference
      *
      * \returns \c true if the option is found
      */
    bool get_option(const std::string& option, int& value) const;
    /** Get the value of a double option
      *
      * The option value is returned by reference if it has
      * a value.
      *
      * \param option  the option name
      * \param value   a double value that is passed by reference
      *
      * \returns \c true if the option is found
      */
    bool get_option(const std::string& option, double& value) const;
    /** Get the value of a string option
      *
      * The option value is returned by reference if it has
      * a value.
      *
      * \param option  the option name
      * \param value   a string value that is passed by reference
      *
      * \returns \c true if the option is found
      */
    bool get_option(const std::string& option, std::string& value) const;
    /** Get the value of an integer option
      *
      * The option value is returned by reference if it has
      * a value.
      *
      * \param option  the option integer id
      * \param value   an integer value that is passed by reference
      *
      * \returns \c true if the option is found
      */
    bool get_option(int option, int& value) const;
    /** Get the value of a double option
      *
      * The option value is returned by reference if it has
      * a value.
      *
      * \param option  the option integer id
      * \param value   a double value that is passed by reference
      *
      * \returns \c true if the option is found
      */
    bool get_option(int option, double& value) const;
    /** Get the value of a string option
      *
      * The option value is returned by reference if it has
      * a value.
      *
      * \param option  the option integer id
      * \param value   a string value that is passed by reference
      *
      * \returns \c true if the option is found
      */
    bool get_option(int option, std::string& value) const;

    /** Set an integer option
      *
      * \param option  the option name
      * \param value   the integer value
      */
    void set_option(const std::string& option, int value);
    /** Set a double option
      *
      * \param option  the option name
      * \param value   the double value
      */
    void set_option(const std::string& option, double value);
    /** Set a string option
      *
      * \param option  the option name
      * \param value   the string value
      */
    void set_option(const std::string& option, const std::string value);
    /** Set an integer option
      *
      * \param option  the option integer id
      * \param value   the integer value
      */
    void set_option(int option, int value);
    /** Set a double option
      *
      * \param option  the option integer id
      * \param value   the double value
      */
    void set_option(int option, double value);
    /** Set a string option
      *
      * \param option  the option integer id
      * \param value   the string value
      */
    void set_option(int option, const std::string value);

};


//
// operator<<
//
std::ostream& operator<<(std::ostream& ostr, const Model& arg);
std::ostream& operator<<(std::ostream& ostr, const NLPModel& arg);

/**
  * Read a problem from a JPOF file
  *
  * A JPOF file is a JSON file that contains a compact representation of
  * a model including decision variables and mutable parameters.  This function
  * fills the \c params data since mutable parameters are not stored in 
  * a Coek model object.
  *
  * \param filename   the JPOF file that is read
  * \param params     a map of parameter names to Parameter objects
  *
  * \returns a model object
  */
Model read_problem_from_jpof_file(const std::string& filename, std::map<std::string,Parameter>& params);
/**
  * Parse a string that defines a model in JPOF format
  *
  * A JPOF is a JSON format that represents a compact representation of
  * a model including decision variables and mutable parameters.  This function
  * fills the \c params data since mutable parameters are not stored in 
  * a Coek model object.
  *
  * \param jpof   the string that contains the JPOF data 
  * \param params     a map of parameter names to Parameter objects
  *
  * \returns a model object
  */
Model read_problem_from_jpof_string(const std::string& jpof, std::map<std::string,Parameter>& params);

}

#endif
