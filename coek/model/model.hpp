#pragma once

#include <string>
#include <map>
#include <set>
#include <vector>
#include <coek/api/constants.hpp>

namespace coek {

class Parameter;
class VariableArray;
class Variable;
class Expression;
class Objective;
class Constraint;
#ifdef COEK_WITH_COMPACT_MODEL
class ConcreteIndexedVariable;
#endif
class ModelRepn;

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
      * When default values are used, this specifies a
      * continuous decision variable.
      *
      * \param lb   the lower bound
      * \param ub   the upper bound
      * \param value   the initial value
      * \param binary   a boolean flag that indicates whether the variable is boolean (default is \c false)
      * \param integer   a boolean flag that indicates whether the variable is a general integer (default is \c false)
      *
      * \returns the variable object
      */
    Variable add_variable(const Expression& lb, const Expression& ub, const Expression& value, bool binary=false, bool integer=false);
    /** Create a variable and add it to the model with standard arguments.
      *
      * This method accepts one or more standard options.  The first argument is the 
      * name of the variable. When default values are used, this specifies a
      * continuous decision variable.
      *
      * \param name   the name of variable
      * \param lb   the lower bound
      * \param ub   the upper bound
      * \param value   the initial value
      * \param binary   a boolean flag that indicates whether the variable is boolean (default is \c false)
      * \param integer   a boolean flag that indicates whether the variable is a general integer (default is \c false)
      *
      * \returns the variable object
      */
    Variable add_variable(const std::string& name, const Expression& lb, const Expression& ub, const Expression& value, bool binary=false, bool integer=false);
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
    Variable get_variable(size_t i);
    /** \returns the variable with the specified name */
    Variable get_variable(const std::string& name);
    /** \returns the vector of variables added to this model */
    std::vector<Variable>& get_variables();

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
    Objective get_objective(size_t i=0);
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
    Constraint get_constraint(size_t i);
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
    void write(std::string filename, std::map<size_t,size_t>& varmap, std::map<size_t,size_t>& conmap);
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


//
// operator<<
//
std::ostream& operator<<(std::ostream& ostr, const Model& arg);

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
