#pragma once

#include <map>
#include <unordered_map>
#include <vector>
#include <variant>

namespace coek
{

class Objective;
class Constraint;
class Variable;
class ObjectiveSequence;
class ConstraintSequence;

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

class CompactModelRepn
{
public:

    std::vector<std::variant<Objective, ObjectiveSequence>> objectives;
    std::vector<std::variant<Constraint, ConstraintSequence>> constraints;
    std::vector<Variable> variables;
    //std::vector<std::variant<Variable, VariableSequence>> variables;
    //std::shared_ptr<coek::ModelVariables> varmap_data;
};

}
