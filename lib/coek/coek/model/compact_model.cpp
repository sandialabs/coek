
#include "coek/util/string_utils.hpp"
#include "coek/ast/varray.hpp"
#include "coek/api/constraint.hpp"
#include "coek/api/objective.hpp"
#include "coek/model/compact_model.hpp"
#include "model_repn.hpp"

namespace coek {

#ifdef COEK_WITH_COMPACT_MODEL
void write_lp_problem(CompactModel& model, const std::string& fname,
                      std::map<size_t, size_t>& varmap, std::map<size_t, size_t>& conmap);
void write_lp_problem_ostream(CompactModel& model, const std::string& fname,
                              std::map<size_t, size_t>& varmap, std::map<size_t, size_t>& conmap);
#    ifdef WITH_FMTLIB
void write_lp_problem_fmtlib(CompactModel& model, const std::string& fname,
                             std::map<size_t, size_t>& varmap, std::map<size_t, size_t>& conmap);
#    endif

//
// CompactModel
//

CompactModel::CompactModel() { repn = std::make_shared<CompactModelRepn>(); }

CompactModel::CompactModel(const CompactModel& other) { repn = other.repn; }

CompactModel::~CompactModel() {}

std::string CompactModel::name() const { return repn->name; }

void CompactModel::name(const std::string& name) { repn->name = name; }

// Data

//Parameter& CompactModel::add(Parameter& params) { return add_parameter(params); }

DataArray& CompactModel::add(DataArray&& data) { return add_data(data); }

DataArray& CompactModel::add(DataArray& data) { return add_data(data); }

DataMap& CompactModel::add(DataMap& data) { return add_data(data); }

DataMap& CompactModel::add(DataMap&& data) { return add_data(data); }

#if 0
Parameter& CompactModel::add_data(Parameter& data)
{
    repn->data.push_back(data);
    return data;
}
#endif

DataArray& CompactModel::add_data(DataArray& data)
{
    repn->data.push_back(data);
    return data;
}

DataMap& CompactModel::add_data(DataMap& data)
{
    repn->data.push_back(data);
    return data;
}

// Parameter

Parameter& CompactModel::add(Parameter& params) { return add_parameter(params); }

ParameterArray& CompactModel::add(ParameterArray&& params) { return add_parameter(params); }

ParameterArray& CompactModel::add(ParameterArray& params) { return add_parameter(params); }

ParameterMap& CompactModel::add(ParameterMap& params) { return add_parameter(params); }

ParameterMap& CompactModel::add(ParameterMap&& params) { return add_parameter(params); }

Parameter& CompactModel::add_parameter(Parameter& param)
{
    repn->parameters.push_back(param);
    repn->parameter_names.push_back("");
    return param;
}

ParameterArray& CompactModel::add_parameter(ParameterArray& params)
{
    repn->parameters.push_back(params);
    repn->parameter_names.push_back("");
    return params;
}

ParameterMap& CompactModel::add_parameter(ParameterMap& params)
{
    repn->parameters.push_back(params);
    repn->parameter_names.insert(repn->parameter_names.end(), params.size(), "");
    return params;
}

// Variable

Variable CompactModel::add_variable()
{
    Variable tmp;
    repn->variables.push_back(tmp);
    repn->variable_names.push_back("");
    return tmp;  // repn->variables.back();
}

Variable CompactModel::add_variable(const std::string& name)
{
    Variable tmp(name);
    repn->variables.push_back(tmp);
    repn->variable_names.push_back(name);
    return tmp;  // repn->variables.back();
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
    for (auto& var : varray.variables) {
        repn->variables.push_back(var);
        repn->variable_names.push_back("");
    }
}

VariableMap& CompactModel::add_variable(VariableMap& vars)
{
    repn->variables.push_back(vars);
    repn->variable_names.push_back("");
    return vars;
}

VariableMap& CompactModel::add_variable(VariableMap&& vars)
{
    repn->variables.push_back(vars);
    repn->variable_names.push_back("");
    return vars;
}

VariableArray& CompactModel::add_variable(VariableArray& vars)
{
    repn->variables.push_back(vars);
    repn->variable_names.push_back("");
    return vars;
}

VariableArray& CompactModel::add_variable(VariableArray&& vars)
{
#    if 0
    for (auto& var : vars)
        repn->variables.push_back(var);
    // repn->variables.insert(repn->variables.end(), vars.begin(), vars.end());
#    endif
    repn->variables.push_back(vars);
    repn->variable_names.push_back("");
    return vars;
}

Objective CompactModel::add_objective(const Expression& expr)
{
    auto obj = objective(expr);
    repn->objectives.push_back(obj);
    return obj;
}

Objective CompactModel::add_objective(const std::string& name, const Expression& expr)
{
    auto obj = objective(name, expr);
    repn->objectives.push_back(obj);
    return obj;
}

Objective& CompactModel::add(Objective& obj)
{
    repn->objectives.push_back(obj);
    return obj;
}

Objective& CompactModel::add(Objective&& obj)
{
    repn->objectives.push_back(obj);
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

#    if __cpp_lib_variant
void CompactModel::add(ConstraintMap& expr) { add_constraint(expr); }
#    endif

void CompactModel::add_constraint(const Constraint& expr, const SequenceContext& context)
{
    ConstraintSequence seq(context, expr);
    repn->constraints.push_back(seq);
}

void CompactModel::add_constraint(const std::string& name, const Constraint& expr,
                                  const SequenceContext& context)
{
    ConstraintSequence seq(name, context, expr);
    repn->constraints.push_back(seq);
}

void CompactModel::add(ConstraintSequence& seq) { repn->constraints.push_back(seq); }

void CompactModel::add(ConstraintSequence&& seq) { repn->constraints.push_back(seq); }

Model CompactModel::expand()
{
    // std::cout << "CompactModel::expand()" << std::endl;
    Model model;

    for (auto& val : repn->data) {
        #if 0
        if (auto eval = std::get_if<Parameter>(&val)) {
            // NOTE: Are we expanding this data in place?  Do we need to create a copy
            //      of this parameter within all expressions?
            Expression value = eval->value_expression().expand();
            eval->value(value.value());
            // model.add_parameter(*eval);
        }
        else 
        #endif
        if (auto eval = std::get_if<DataMap>(&val)) {
            eval->expand();
            #if 0
            for (auto data : *eval) {
                // NOTE: Are we changing the values of these maps in place?
                Expression value = data.value_expression().expand();
                data.value(value.value());
            }
            #endif
            model.repn->data_maps.push_back(*eval);
        }
        else if (auto eval = std::get_if<DataArray>(&val)) {
            eval->expand();
            #if 0
            for (auto data : *eval) {
                // NOTE: Are we changing the values of these maps in place?
                Expression value = data.value_expression().expand();
                data.value(value.value());
            }
            #endif
            model.repn->data_arrays.push_back(*eval);
        }
    }

    for (auto& val : repn->parameters) {
        if (auto eval = std::get_if<Parameter>(&val)) {
            // NOTE: Are we expanding this parameter in place?  Do we need to create a copy
            //      of this parameter within all expressions?
            Expression value = eval->value_expression().expand();
            eval->value(value.value());
            // model.add_parameter(*eval);
        }
        else if (auto eval = std::get_if<ParameterMap>(&val)) {
            eval->expand();
            for (auto param : *eval) {
                // NOTE: Are we changing the values of these maps in place?
                Expression value = param.value_expression().expand();
                param.value(value.value());
            }
            model.repn->parameter_maps.push_back(*eval);
        }
        else if (auto eval = std::get_if<ParameterArray>(&val)) {
            eval->expand();
            for (auto param : *eval) {
                // NOTE: Are we changing the values of these maps in place?
                Expression value = param.value_expression().expand();
                param.value(value.value());
            }
            model.repn->parameter_arrays.push_back(*eval);
        }
    }

    // std::cout << "VARIABLES " << repn->variables.size() << std::endl;
    for (auto& val : repn->variables) {
        if (auto eval = std::get_if<Variable>(&val)) {
            auto var = eval->expand();
            model.add_variable(var);
        }
        else if (auto eval = std::get_if<VariableSequence>(&val)) {
            for (auto jt = eval->begin(); jt != eval->end(); ++jt) {
                // std::cerr << "VARIABLE " << jt->name() << std::endl;
                Expression lb = jt->lower_expression().expand();
                jt->lower(lb.value());
                Expression ub = jt->upper_expression().expand();
                jt->upper(ub.value());
                Expression value = jt->value_expression().expand();
                jt->value(value.value());
                model.add_variable(*jt);
            }
        }
        else if (auto eval = std::get_if<VariableMap>(&val)) {
            // std::cout << "VARIABLE_MAP " << eval->name() << " " << eval->size() << std::endl;
            eval->expand();
            model.add_variable(*eval);
        }
        else if (auto eval = std::get_if<VariableArray>(&val)) {
            // std::cerr << "VARIABLE_ARRAY " << eval->name() << " " << eval->size() << std::endl;
            eval->expand();
            model.add_variable(*eval);
        }
    }

    for (auto& val : repn->objectives) {
        if (auto eval = std::get_if<Objective>(&val)) {
            Expression e = eval->expr().expand();
            model.add_objective(e).sense(eval->sense());
        }
        else {
            auto& seq = std::get<ObjectiveSequence>(val);
            for (auto jt = seq.begin(); jt != seq.end(); ++jt) {
                model.repn->objectives.push_back(*jt);
            }
        }
    }

    for (auto& val : repn->constraints) {
        if (auto cval = std::get_if<Constraint>(&val)) {
            Constraint c = cval->expand();
            model.repn->constraints.push_back(c);
        }
        else {
            auto& seq = std::get<ConstraintSequence>(val);
            for (auto jt = seq.begin(); jt != seq.end(); ++jt) {
                model.repn->constraints.push_back(*jt);
            }
        }
    }
    return model;
}

void CompactModel::write(const std::string& fname)
{
    std::map<size_t, size_t> varmap;
    std::map<size_t, size_t> conmap;
    write(fname, varmap, conmap);
}

void CompactModel::write(const std::string& fname, std::map<size_t, size_t>& varmap,
                         std::map<size_t, size_t>& conmap)
{
    if (ends_with(fname, ".lp")) {
        write_lp_problem(*this, fname, varmap, conmap);
        return;
    }

    else if (ends_with(fname, ".ostrlp")) {
        write_lp_problem_ostream(*this, fname, varmap, conmap);
        return;
    }

#    ifdef WITH_FMTLIB
    else if (ends_with(fname, ".fmtlp")) {
        write_lp_problem_fmtlib(*this, fname, varmap, conmap);
        return;
    }
#    endif

    Model model = expand();
    model.write(fname, varmap, conmap);
}
#endif

}  // namespace coek
