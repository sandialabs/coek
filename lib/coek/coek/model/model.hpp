#pragma once

#ifdef __has_include
#    if __has_include(<version>)
#        include <version>
#    endif
#endif
#if __cpp_lib_variant
#    include <variant>
#endif

#include <coek/api/constants.hpp>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

namespace coek {

class Parameter;
class PythonVariableArray;
class Variable;
class Expression;
class Objective;
class Constraint;
#ifdef COEK_WITH_COMPACT_MODEL
class ParameterMap;
class VariableMap;
#endif
#if __cpp_lib_variant
class ParameterArray;
class VariableArray;
class ConstraintMap;
#endif
class ModelRepn;

//
// Coek Model
//
// TODO - Define SimpeModel and AbstractModel.  Operations like get_constraint(i) may not make sense
// for an abstract model, and even when they do, these are probably more expensive operations.
//

/**
 * An optimization model that contains objectives and constraints.
 *
 * Variables are added to this model to facilitate error checking and
 * enable efficient processing of model expressions.
 */
class Model {
   public:
    /** A constant used to indicate a minimization objective */
    static const bool minimize;
    /** A constant used to indicate a maximization objective */
    static const bool maximize;
    /** A constant that indicates an infinite value */
    static const double inf;

    /** The different policies for name generation. */
    enum NameGeneration { simple, lazy, eager };

    std::shared_ptr<ModelRepn> repn;

   public:
    /** Constructs an empty optimization model */
    Model();

    /** Get the name of the model */
    std::string name() const;
    /** Set the name of the model */
    void name(const std::string& name);

#if __cpp_lib_variant
    ParameterArray& add_parameter(ParameterArray& param);
    ParameterArray& add(ParameterArray& param);
    ParameterArray& add(ParameterArray&& param);
#endif
#ifdef COEK_WITH_COMPACT_MODEL
    ParameterMap& add_parameter(ParameterMap& param);
    ParameterMap& add(ParameterMap& param);
    ParameterMap& add(ParameterMap&& param);
#endif

    //
    // Single Variables
    //
    /** \returns the number of variables in the model */
    size_t num_variables() const;
    /** \returns a list of variable names in the model */
    std::set<std::string> variable_names();

    /** Create a variable and add it to the model with standard arguments.
     *
     * This specifies an unbounded,
     * continuous decision variable.
     *
     * \returns the variable object
     */
    Variable add_variable();
    /** Create a variable and add it to the model with standard arguments.
     *
     * This specifies an unbounded,
     * continuous decision variable.
     *
     * \returns the variable object
     */
    Variable add_variable(const std::string& name);
    /** Add a variable to the model.
     *
     * \returns the variable
     */
    Variable& add_variable(Variable& var);
    Variable& add(Variable& var);
    Variable& add(Variable&& var);

    //
    // Variable arrays and maps
    //
    void add_variable(PythonVariableArray& var);
#if __cpp_lib_variant
    VariableArray& add_variable(VariableArray& var);
    // VariableArray& add_variable(VariableArray&& var);
    VariableArray& add(VariableArray& var);
    VariableArray& add(VariableArray&& var);
#    ifdef COEK_WITH_COMPACT_MODEL
    VariableMap& add_variable(VariableMap& var);
    // VariableMap& add_variable(VariableMap&& var);
    VariableMap& add(VariableMap& var);
    VariableMap& add(VariableMap&& var);
#    endif
#endif

    /** \returns the variable with the specified id */
    Variable get_variable(size_t i);
    /** \returns the variable with the specified name */
    Variable get_variable(const std::string& name);
    /** \returns the vector of variables added to this model */
    std::vector<Variable>& get_variables();
    /** \returns a map from variable names to variables */
    std::map<std::string, Variable>& get_variables_by_name();

    //
    // Objectives
    //
    /** \returns the number of objecives in the model */
    size_t num_objectives() const;
    /** \returns the number of active objecives in the model */
    size_t num_active_objectives() const;
    /** \returns a list of objective names in the model */
    std::set<std::string> objective_names();

    /**
     * Create an objective in the model.
     *
     * \param expr   the objective expression
     *
     * \returns the objective
     */
    Objective add_objective(const Expression& expr);
    /**
     * Create a named objective in the model.
     *
     * \param name   the objective name
     * \param expr   the objective expression
     *
     * \returns the objective
     */
    Objective add_objective(const std::string& name, const Expression& expr);
    Objective& add(Objective& var);
    Objective& add(Objective&& var);

    /** \returns the i-th objective that was added */
    Objective get_objective(size_t i = 0);
    /** \returns the objective with the specified name */
    Objective get_objective(const std::string& name);
    /** \returns a map from objective names to objectives */
    std::map<std::string, Objective>& get_objectives_by_name();

    //
    // Constraint
    //
    /** \returns the number of constraints in the model */
    size_t num_constraints() const;
    /** \returns the number of constraints in the model */
    size_t num_active_constraints() const;
    /** \returns a list of constraint names in the model */
    std::set<std::string> constraint_names();

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
    Constraint& add(Constraint& expr);
    Constraint& add(Constraint&& expr);
#if __cpp_lib_variant
    void add_constraint(ConstraintMap& expr);
    void add(ConstraintMap& expr);
#endif

    /** \returns the i-th constraint that was added */
    Constraint get_constraint(size_t i);
    /** \returns the constraint with the specified name */
    Constraint get_constraint(const std::string& name);
    /** \returns a map from constraint names to constraints */
    std::map<std::string, Constraint>& get_constraints_by_name();

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
    void write(const std::string& filename);
    /** Write the model to the specified file
     *
     * \param filename  the output file
     * \param varmap  varmap[i] contains the id of the i-th variable in this model
     * \param conmap  conmap[i] contains the id of the i-th constraint in this model
     */
    void write(const std::string& filename, std::map<size_t, size_t>& varmap,
               std::map<size_t, size_t>& conmap);
    /** Print the equations in the model to \c std::cout */
    void print_equations(bool active = true) const;
    /** Print the equations in the model to the specified output stream */
    void print_equations(std::ostream& ostr, bool active = true) const;
    /** Print the values in the model to \c std::cout */
    void print_values();
    /** Print the values in the model to the specified output stream */
    void print_values(std::ostream& ostr);

    friend std::ostream& operator<<(std::ostream& ostr, const Model& arg);

    void generate_names();
    void name_generation(Model::NameGeneration value);
    Model::NameGeneration name_generation();
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
Model read_problem_from_jpof_file(const std::string& filename,
                                  std::map<std::string, Parameter>& params);
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
Model read_problem_from_jpof_string(const std::string& jpof,
                                    std::map<std::string, Parameter>& params);

}  // namespace coek
