#include "coek/compact/variable_assoc_array.hpp"
#include <cmath>
#include "../ast/compact_terms.hpp"


namespace coek {

expr_pointer_t create_varref(const std::vector<refarg_types>& indices, const std::string& name, void* var);


//
// VariableAssocArrayRepn
//

VariableAssocArrayRepn::VariableAssocArrayRepn()
    : call_setup(true)
    {}

void VariableAssocArrayRepn::resize_index_vectors(IndexVector& tmp, std::vector<refarg_types>& reftmp)
{
tmp = cache.alloc(dim());
reftmp.resize(dim());
}

std::string VariableAssocArrayRepn::get_name(size_t index)
{
if (call_setup)
    setup();
if (index >= names.size())
    throw std::runtime_error("Asking for name with an index that is greater than the number of elements in the index set.");
return names[index];
}

void VariableAssocArrayRepn::setup()
{
call_setup=false;

auto vtype = variable_template.within();
bool binary = (vtype == Boolean) or (vtype == Binary);
bool integer = vtype == Integers;
for (size_t i=0; i<size(); i++) {
    auto lower = variable_template.lower_expression().expand().value();
    auto upper = variable_template.upper_expression().expand().value();
    auto value = variable_template.value_expression().expand().value();
    values.emplace_back(CREATE_POINTER(IndexedVariableTerm, 
                                CREATE_POINTER(ConstantTerm, lower),
                                CREATE_POINTER(ConstantTerm, upper),
                                CREATE_POINTER(ConstantTerm, value),
                                binary, integer, i, this));
    }
}

void VariableAssocArrayRepn::value(double value)
{
variable_template.value(value);
if (values.size() > 0) {
    for (auto& var: values)
        var.value(value);
    }
}

void VariableAssocArrayRepn::value(const Expression& value)
{
variable_template.value(value);
if (values.size() > 0) {
    for (auto& var: values)
        var.value(value);
    }
}

void VariableAssocArrayRepn::lower(double value)
{
variable_template.lower(value);
if (values.size() > 0) {
    for (auto& var: values)
        var.lower(value);
    }
}

void VariableAssocArrayRepn::lower(const Expression& value)
{
variable_template.lower(value);
if (values.size() > 0) {
    for (auto& var: values)
        var.lower(value);
    }
}

void VariableAssocArrayRepn::upper(double value)
{
variable_template.upper(value);
if (values.size() > 0) {
    for (auto& var: values)
        var.upper(value);
    }
}

void VariableAssocArrayRepn::upper(const Expression& value)
{
variable_template.upper(value);
if (values.size() > 0) {
    for (auto& var: values)
        var.upper(value);
    }
}

void VariableAssocArrayRepn::bounds(double lb, double ub)
{
variable_template.bounds(lb,ub);
if (values.size() > 0) {
    for (auto& var: values)
        var.bounds(lb,ub);
    }
}

void VariableAssocArrayRepn::bounds(const Expression& lb, double ub)
{
variable_template.bounds(lb,ub);
if (values.size() > 0) {
    for (auto& var: values)
        var.bounds(lb,ub);
    }
}

void VariableAssocArrayRepn::bounds(double lb, const Expression& ub)
{
variable_template.bounds(lb,ub);
if (values.size() > 0) {
    for (auto& var: values)
        var.bounds(lb,ub);
    }
}

void VariableAssocArrayRepn::bounds(const Expression& lb, const Expression& ub)
{
variable_template.bounds(lb,ub);
if (values.size() > 0) {
    for (auto& var: values)
        var.bounds(lb,ub);
    }
}

void VariableAssocArrayRepn::name(const std::string& name)
{
variable_template.name(name);
if (values.size() > 0) {
    for (auto& var: values)
        var.name(name);
    }
}

void VariableAssocArrayRepn::within(VariableTypes vtype)
{
variable_template.within(vtype);
if (values.size() > 0) {
    for (auto& var: values)
        var.within(vtype);
    }
}

//
// VariableAssocArray
//

size_t VariableAssocArray::size()
{ return get_repn()->size(); }

size_t VariableAssocArray::dim()
{ return get_repn()->dim(); }

std::vector<Variable>::iterator VariableAssocArray::begin()
{ return get_repn()->values.begin(); }

std::vector<Variable>::iterator VariableAssocArray::end()
{ return get_repn()->values.end(); }

Expression VariableAssocArray::create_varref(const std::vector<refarg_types>& args)
{ return coek::create_varref(args, get_repn()->variable_template.name(), this); }

//
// OTHER
//

std::string IndexedVariableTerm::get_name()
{
VariableAssocArrayRepn* _var = static_cast<VariableAssocArrayRepn*>(var);
return _var->get_name(vindex);
}

expr_pointer_t get_concrete_var(VariableRefTerm& varref)
{
VariableAssocArray* var = static_cast<VariableAssocArray*>(varref.var);

std::vector<int> index;
for (auto it=varref.indices.begin(); it != varref.indices.end(); ++it) {
    refarg_types& reftmp = *it;
    if (auto ival = std::get_if<int>(&reftmp))
        index.push_back(*ival);
    else {
        expr_pointer_t eval = std::get<expr_pointer_t>(reftmp);
        double vald = eval->eval();
        long int vali = std::lround(vald);
        assert(fabs(vald-vali) < 1e-7);
        index.push_back(static_cast<int>(vali));
        }
    }

IndexVector& tmp = var->tmp;
for (size_t i=0; i<index.size(); i++)
  tmp[i] = index[i];

Expression e = var->index(tmp);
return e.repn;
}

}
