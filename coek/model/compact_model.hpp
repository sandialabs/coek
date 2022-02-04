#pragma once

#ifdef COEK_WITH_COMPACT_MODEL

#include <coek/model/model.hpp>
#include <variant>
#include <coek/compact/coek_sets.hpp>
#include <coek/compact/coek_indexed.hpp>

namespace coek {

class VariableMap;
class ObjectiveMap;
class ConstraintMap;
class SequenceContext;
class CompactModelRepn;


class VariableMap
{
public:

    std::shared_ptr<ModelRepn> repn;

public:

    /** \returns the i-th variable defined in this map */
    Variable operator()(size_t i);

    /** \returns the variable associated with the index parameters */
    template <typename... TYPES>
    Variable operator[](const TYPES&... args)
        {
        std::vector<IndexParameter> indices;
        collect_index_parameters(indices, args...);
        return operator[](indices);
        }

    /** \returns the variable associated with the index parameters */
    Variable operator[](const std::vector<IndexParameter>& indices);

};


class ObjectiveMap
{
public:

    std::shared_ptr<ModelRepn> repn;

public:

    /** \returns the i-th objective defined in this map */
    Objective operator()(size_t i);

    /** \returns the objective associated with the index parameters */
    template <typename... TYPES>
    Objective operator[](const TYPES&... args)
        {
        std::vector<IndexParameter> indices;
        collect_index_parameters(indices, args...);
        return operator[](indices);
        }

    /** \returns the objective associated with the index parameters */
    Objective operator[](const std::vector<IndexParameter>& indices);

};


class ConstraintMap
{
public:

    std::shared_ptr<ModelRepn> repn;

public:

    /** \returns the i-th constraint defined in this map */
    Constraint operator()(size_t i);

    /** \returns the constraint associated with the index parameters */
    template <typename... TYPES>
    Constraint operator[](const TYPES&... args)
        {
        std::vector<IndexParameter> indices;
        collect_index_parameters(indices, args...);
        return operator[](indices);
        }

    /** \returns the constraint associated with the index parameters */
    Constraint operator[](const std::vector<IndexParameter>& indices);

};


class CompactModel
{
public:

    std::shared_ptr<CompactModelRepn> repn;

public:

    /** Constructs an empty optimization model */
    CompactModel();
    CompactModel(const CompactModel& other);
    ~CompactModel();

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
    /** Create variables and add them to the model with standard arguments.
      *
      * This method accepts one or more standard options.  
      * When default values are used, this specifies a continuous decision variable.
      *
      * \param context   a sequence of indices that are used to index the variables
      * \param lb   the lower bound
      * \param ub   the upper bound
      * \param value   the initial value
      * \param binary   a boolean flag that indicates whether the variable is boolean (default is \c false)
      * \param integer   a boolean flag that indicates whether the variable is a general integer (default is \c false)
      *
      * \returns a map to the objectives
      */
    VariableMap add_variable(const Expression& lb, const Expression& ub, const Expression& value, bool binary, bool integer, const SequenceContext& context);
    /** Create variables and add them to the model with standard arguments.
      *
      * This method accepts one or more standard options.  
      * When default values are used, this specifies a continuous decision variable.
      *
      * \param name   the name of variable
      * \param context   a sequence of indices that are used to index the variables
      * \param lb   the lower bound
      * \param ub   the upper bound
      * \param value   the initial value
      * \param binary   a boolean flag that indicates whether the variable is boolean (default is \c false)
      * \param integer   a boolean flag that indicates whether the variable is a general integer (default is \c false)
      *
      * \returns a map to the objectives
      */
    VariableMap add_variable(const std::string& name, const Expression& lb, const Expression& ub, const Expression& value, bool binary, bool integer, const SequenceContext& context);

    Variable add_variable(Variable& var);
    void add_variable(VariableArray& var);
    void add_variable(ConcreteIndexedVariable& var);

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
      * \param _sense   A boolean that is \c true if the sense is minimize
      *
      * \returns the objective
      */
    Objective add_objective(const std::string& name, const Expression& expr, bool _sense=Model::minimize);
    /**
      * Create and add a sequence of objectives to the model.
      *
      * \param context   a sequence of indices that are used to index the objectives
      * \param expr   the objective expression
      * \param _sense   A boolean that is \c true if the sense is minimize
      *
      * \returns a map to the objectives
      */
    ObjectiveMap add_objective(const Expression& expr, const SequenceContext& context, bool _sense=Model::minimize);
    /**
      * Create and add a sequence of objectives to the model.
      *
      * \param name   the objective name
      * \param context   a sequence of indices that are used to index the objectives
      * \param expr   the objective expression
      * \param _sense   A boolean that is \c true if the sense is minimize
      *
      * \returns a map to the objectives
      */
    ObjectiveMap add_objective(const std::string& name, const Expression& expr, const SequenceContext& context, bool _sense=Model::minimize);

    /**
      * Add a constraint to the model.
      *
      * \param expr the constraint expression
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
    /**
      * Add constraints to the model.
      *
      * \param context  a sequence of indices that are used to index the constraints
      * \param expr     the constraint expression
      *
      * \returns a map to the constraints
      */
    ConstraintMap add_constraint(const Constraint& expr, const SequenceContext& context);
    /**
      * Add constraints to the model.
      *
      * \param name the constraint name
      * \param context  a sequence of indices that are used to index the constraints
      * \param expr     the constraint expression
      *
      * \returns a map to the constraints
      */
    ConstraintMap add_constraint(const std::string& name, const Constraint& expr, const SequenceContext& context);

    Model expand();

    void write(std::string filename);
    void write(std::string filename, std::map<size_t,size_t>& varmap, std::map<size_t,size_t>& conmap);
};

}

#endif
