#pragma once

#include <map>
#include <unordered_map>
#include <variant>
#include <vector>
#include "coek/api/expression.hpp"
#include "coek/api/objective.hpp"
#include "coek/api/constraint.hpp"
#ifdef COEK_WITH_COMPACT_MODEL
#    include "coek/compact/variable_sequence.hpp"
#    include "coek/compact/objective_sequence.hpp"
#    include "coek/compact/constraint_sequence.hpp"
#endif

namespace coek {

//
// ModelRepn
//

class ModelRepn {
   public:
    std::vector<Objective> objectives;
    std::vector<Constraint> constraints;
    std::vector<Variable> variables;

    std::map<std::string, Objective> objectives_by_name;
    std::map<std::string, Constraint> constraints_by_name;
    std::map<std::string, Variable> variables_by_name;

    std::map<std::string, std::unordered_map<unsigned int, double>> vsuffix;
    std::map<std::string, std::unordered_map<unsigned int, double>> csuffix;
    std::map<std::string, std::unordered_map<unsigned int, double>> osuffix;
    std::map<std::string, double> msuffix;
};

#ifdef COEK_WITH_COMPACT_MODEL

class CompactVariableMap {
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

class ObjectiveMap {
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

class CompactConstraintMap {
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

//
// CompactModelRepn
//
class CompactModelRepn {
   public:
    std::vector<std::variant<Objective, ObjectiveSequence>> objectives;
    std::vector<std::string> objective_names;
    std::vector<std::variant<Constraint, ConstraintSequence>> constraints;
    std::vector<std::string> constraint_names;
    std::vector<std::variant<Variable, VariableSequence>> variables;
    std::vector<std::string> variable_names;
    std::map<std::string, std::variant<CompactVariableMap, ObjectiveMap, CompactConstraintMap>>
        mapped_data;
};

#endif

}  // namespace coek
