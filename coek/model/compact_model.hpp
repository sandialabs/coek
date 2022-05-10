#pragma once

#ifdef COEK_WITH_COMPACT_MODEL

#include <coek/model/model.hpp>
#include <variant>
#include <coek/api/variable_array.hpp>
#include <coek/compact/coek_sets.hpp>
#include <coek/compact/variable_map.hpp>

namespace coek {

class VariableMap;
class VariableArray;
class ObjectiveMap;
class CompactConstraintMap;
class SequenceContext;
class ConstraintSequence;
class CompactModelRepn;


class CompactVariableMap
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


class CompactConstraintMap
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

    Variable& add_variable(Variable& var);
    void add_variable(PythonVariableArray& var);
    VariableMap& add_variable(VariableMap& var);
    VariableArray& add_variable(VariableArray& var);

    Variable& add(Variable& var)
        {return add_variable(var);}
    VariableMap& add(VariableMap& var)
        {return add_variable(var);}
    VariableArray& add(VariableArray& var)
        {return add_variable(var);}

    /**
      * Create an objective and add it to the model.
      *
      * \param expr   the objective expression
      *
      * \returns the objective
      */
    Objective add_objective(const Expression& expr);
    /**
      * Create a named objective and add it to the model.
      *
      * \param name   the objective name
      * \param expr   the objective expression
      *
      * \returns the objective
      */
    Objective add_objective(const std::string& name, const Expression& expr);

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
    void add_constraint(const Constraint& expr, const SequenceContext& context);
    /**
      * Add constraints to the model.
      *
      * \param name the constraint name
      * \param context  a sequence of indices that are used to index the constraints
      * \param expr     the constraint expression
      *
      * \returns a map to the constraints
      */
    void add_constraint(const std::string& name, const Constraint& expr, const SequenceContext& context);

    void add(ConstraintSequence& seq);

    Model expand();

    void write(std::string filename);
    void write(std::string filename, std::map<size_t,size_t>& varmap, std::map<size_t,size_t>& conmap);
};

}

#endif
