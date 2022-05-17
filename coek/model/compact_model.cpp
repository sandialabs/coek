#include "../util/endswith.hpp"
#include "../ast/varray.hpp"
#include "coek/api/objective.hpp"
#include "coek/api/constraint.hpp"
#include "coek/model/compact_model.hpp"
//#ifdef COEK_WITH_COMPACT_MODEL
//#include "coek/compact/coek_exprterm.hpp"
//#endif
#include "model_repn.hpp"


namespace coek {

#ifdef COEK_WITH_COMPACT_MODEL
void write_lp_problem(CompactModel& model, std::string& fname, std::map<size_t,size_t>& varmap, std::map<size_t,size_t>& conmap);
void write_lp_problem_ostream(CompactModel& model, std::string& fname, std::map<size_t,size_t>& varmap, std::map<size_t,size_t>& conmap);
#ifdef WITH_FMTLIB
void write_lp_problem_fmtlib(CompactModel& model, std::string& fname, std::map<size_t,size_t>& varmap, std::map<size_t,size_t>& conmap);
#endif


//
// CompactModel
//

CompactModel::CompactModel()
{ repn = std::make_shared<CompactModelRepn>(); }

CompactModel::CompactModel(const CompactModel& other)
{ repn = other.repn; }

CompactModel::~CompactModel()
{}

Variable CompactModel::add_variable()
{
Variable tmp;
repn->variables.push_back(tmp);
repn->variable_names.push_back("");
return tmp; //repn->variables.back();
}

Variable CompactModel::add_variable(const std::string& name)
{
Variable tmp(name);
repn->variables.push_back(tmp);
repn->variable_names.push_back(name);
return tmp; //repn->variables.back();
}

Variable& CompactModel::add_variable(Variable& var)
{
repn->variables.push_back(var);
repn->variable_names.push_back("");
return var;
}

Variable& CompactModel::add_variable(Variable&& var)
{
repn->variables.push_back(var);
repn->variable_names.push_back("");
return var;
}

void CompactModel::add_variable(PythonVariableArray& varray)
{
for (auto it=varray.variables.begin(); it != varray.variables.end(); it++) {
    repn->variables.push_back(*it);
    repn->variable_names.push_back("");
    }
}

VariableMap& CompactModel::add_variable(VariableMap& vars)
{
auto end = vars.end();
for (auto it=vars.begin(); it != end; ++it) {
    repn->variables.push_back(*it);
    repn->variable_names.push_back("");
    }
return vars;
}

VariableMap& CompactModel::add_variable(VariableMap&& vars)
{
auto end = vars.end();
for (auto it=vars.begin(); it != end; ++it) {
    repn->variables.push_back(*it);
    repn->variable_names.push_back("");
    }
return vars;
}

VariableArray& CompactModel::add_variable(VariableArray& vars)
{
auto end = vars.end();
for (auto it=vars.begin(); it != end; ++it) {
    repn->variables.push_back(*it);
    repn->variable_names.push_back("");
    }
return vars;
}

VariableArray& CompactModel::add_variable(VariableArray&& vars)
{
auto end = vars.end();
for (auto it=vars.begin(); it != end; ++it) {
    repn->variables.push_back(*it);
    repn->variable_names.push_back("");
    }
return vars;
}

Objective CompactModel::add_objective(const Expression& expr)
{
auto obj = objective(expr);
repn->objectives.push_back( obj );
return obj;
}

Objective CompactModel::add_objective(const std::string& name, const Expression& expr)
{
auto obj = objective(name, expr);
repn->objectives.push_back( obj );
return obj;
}

Objective& CompactModel::add(Objective& obj)
{
repn->objectives.push_back( obj );
return obj;
}

Objective& CompactModel::add(Objective&& obj)
{
repn->objectives.push_back( obj );
return obj;
}

Constraint CompactModel::add_constraint(const Constraint& expr)
{
repn->constraints.push_back(expr);
return expr;
}

Constraint CompactModel::add_constraint(const std::string& name, const Constraint& expr)
{
repn->constraints.push_back(expr);
std::get<Constraint>(repn->constraints.back()).name(name);
return expr;
}

Constraint& CompactModel::add(Constraint& expr)
{
repn->constraints.push_back(expr);
return expr;
}

Constraint& CompactModel::add(Constraint&& expr)
{
repn->constraints.push_back(expr);
return expr;
}

#if __cpp_lib_variant
void CompactModel::add(ConstraintMap& expr)
{ add_constraint(expr); }
#endif

void CompactModel::add_constraint(const Constraint& expr, const SequenceContext& context)
{
ConstraintSequence seq(context, expr);
repn->constraints.push_back(seq);
}

void CompactModel::add_constraint(const std::string& name, const Constraint& expr, const SequenceContext& context)
{
ConstraintSequence seq(name, context, expr);
repn->constraints.push_back(seq);
}

void CompactModel::add(ConstraintSequence& seq)
{ repn->constraints.push_back(seq); }

void CompactModel::add(ConstraintSequence&& seq)
{ repn->constraints.push_back(seq); }


Model CompactModel::expand()
{
Model model;

for (auto it=repn->variables.begin(); it != repn->variables.end(); ++it) {
    auto& val = *it;
    if (auto eval = std::get_if<Variable>(&val)) {
        Expression lb = eval->lower_expression().expand();
        eval->lower(lb.value());
        Expression ub = eval->upper_expression().expand();
        eval->upper(ub.value());
        Expression value = eval->value_expression().expand();
        eval->value(value.value());
        model.add_variable(*eval);
        }
    else {
        auto& seq = std::get<VariableSequence>(val);
        for (auto jt=seq.begin(); jt != seq.end(); ++jt) {
            model.repn->variables.push_back(*jt);
            }
        }
    }

for (auto it=repn->objectives.begin(); it != repn->objectives.end(); ++it) {
    auto& val = *it;
    if (auto eval = std::get_if<Objective>(&val)) {
        Expression e = eval->expr().expand();
        model.add_objective(e).
                    sense(eval->sense());
        }
    else {
        auto& seq = std::get<ObjectiveSequence>(val);
        for (auto jt=seq.begin(); jt != seq.end(); ++jt) {
            model.repn->objectives.push_back(*jt);
            }
        }
    }

for (auto it=repn->constraints.begin(); it != repn->constraints.end(); ++it) {
    auto& val = *it;
    if (auto cval = std::get_if<Constraint>(&val)) {
        Constraint c = cval->expand();
        model.repn->constraints.push_back(c);
        }
    else {
        auto& seq = std::get<ConstraintSequence>(val);
        for (auto jt=seq.begin(); jt != seq.end(); ++jt) {
            model.repn->constraints.push_back(*jt);
            }
        }
    }
return model;
}

void CompactModel::write(std::string fname)
{
std::map<size_t,size_t> varmap;
std::map<size_t,size_t> conmap;
write(fname, varmap, conmap);
}

void CompactModel::write(std::string fname, std::map<size_t,size_t>& varmap, std::map<size_t,size_t>& conmap)
{
if (endsWith(fname, ".lp")) {
    write_lp_problem(*this, fname, varmap, conmap);
    return;
    }

else if (endsWith(fname, ".ostrlp")) {
    write_lp_problem_ostream(*this, fname, varmap, conmap);
    return;
    }

#ifdef WITH_FMTLIB
else if (endsWith(fname, ".fmtlp")) {
    write_lp_problem_fmtlib(*this, fname, varmap, conmap);
    return;
    }
#endif

Model model = expand();
model.write(fname, varmap, conmap);
}
#endif

}
