#pragma once

#include <map>
#include <unordered_map>
#include <vector>
#include <variant>
#ifdef COEK_WITH_COMPACT_MODEL
#include "coek/compact/objective_sequence.hpp"
#include "coek/compact/constraint_sequence.hpp"
#include "coek/compact/variable_sequence.hpp"
#endif

namespace coek
{
class Objective;
class Constraint;
class Variable;
class VariableMap;
class ObjectiveMap;
class ConstraintMap;

//
// ModelRepn
//

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
// CompactModelRepn
//
#ifdef COEK_WITH_COMPACT_MODEL
class CompactModelRepn
{
public:

    std::vector<std::variant<Objective, ObjectiveSequence>> objectives;
    std::vector<std::string> objective_names;
    std::vector<std::variant<Constraint, ConstraintSequence>> constraints;
    std::vector<std::string> constraint_names;
    std::vector<std::variant<Variable, VariableSequence>> variables;
    std::vector<std::string> variable_names;
    std::map<std::string, std::variant<VariableMap, ObjectiveMap, ConstraintMap>> mapped_data;
};
#endif

}
