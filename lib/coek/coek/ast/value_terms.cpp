#include "value_terms.hpp"

#include "expr_terms.hpp"

namespace coek {

//
// ParameterTerm
//

ParameterTerm::ParameterTerm() : indexed(false)
{
    non_variable = true;
    OWN_POINTER(value = CREATE_POINTER(ConstantTerm, 0.0));
}

ParameterTerm::ParameterTerm(const expr_pointer_t& _value, bool _indexed)
    : value(_value), indexed(_indexed)
{
    non_variable = true;
    if (_value) OWN_POINTER(_value);
}

ParameterTerm::~ParameterTerm()
{
    if (value) DISOWN_POINTER(value);
}

expr_pointer_t ParameterTerm::negate(const expr_pointer_t& repn)
{
    // SHARED_PTR
    // return std::static_pointer_cast<BaseExpressionTerm>( std::make_shared<NegateTerm>(repn) );
    return CREATE_POINTER(NegateTerm, repn);
}

void ParameterTerm::set_value(double val)
{
    if (value) DISOWN_POINTER(value);
    OWN_POINTER(value = CREATE_POINTER(ConstantTerm, val));
}

void ParameterTerm::set_value(const expr_pointer_t val)
{
    if (value) DISOWN_POINTER(value);
    OWN_POINTER(value = val);
}

//
// IndexParameterTerm
//

expr_pointer_t IndexParameterTerm::negate(const expr_pointer_t& repn)
{
    return CREATE_POINTER(NegateTerm, repn);
}

expr_pointer_t create_abstract_parameter(const std::string& name)
{
    return CREATE_POINTER(IndexParameterTerm, name);
}

double IndexParameterTerm::eval() const
{
    if (type == 0)
        throw std::runtime_error("Accessing the value of an uninitialized abstract parameter");
    if (type == 1) return double_value;
    if (type == 2) return int_value;
    throw std::runtime_error("Accessing the value of a non-numeric abstract parameter: "
                             + string_value);
}

void IndexParameterTerm::set_value(double value)
{
    type = 1;
    double_value = value;
}

void IndexParameterTerm::set_value(int value)
{
    type = 2;
    int_value = value;
}

void IndexParameterTerm::set_value(const std::string& value)
{
    type = 3;
    string_value = value;
}

bool IndexParameterTerm::get_value(double& value)
{
    if (type != 1) return false;
    value = double_value;
    return true;
}

bool IndexParameterTerm::get_value(int& value)
{
    if (type != 2) return false;
    value = int_value;
    return true;
}

bool IndexParameterTerm::get_value(std::string& value)
{
    if (type != 3) return false;
    value = string_value;
    return true;
}

//
// VariableTerm
//

unsigned int VariableTerm::count = 0;

VariableTerm::VariableTerm(const expr_pointer_t& _lb, const expr_pointer_t& _ub,
                           const expr_pointer_t& _value, bool _binary, bool _integer, bool _indexed)
    : value(_value),
      lb(_lb),
      ub(_ub),
      binary(_binary),
      integer(_integer),
      fixed(false),
      indexed(_indexed)
{
    index = count++;
    if (_lb) OWN_POINTER(_lb);
    if (_ub) OWN_POINTER(_ub);
    if (_value) OWN_POINTER(_value);
}

VariableTerm::~VariableTerm()
{
    if (lb) DISOWN_POINTER(lb);
    if (ub) DISOWN_POINTER(ub);
    if (value) DISOWN_POINTER(value);
}

expr_pointer_t VariableTerm::const_mult(double coef, const expr_pointer_t& repn)
{
    VariableTerm* var = dynamic_cast<VariableTerm*>(repn);
    return CREATE_POINTER(MonomialTerm, coef, var);
}

expr_pointer_t VariableTerm::negate(const expr_pointer_t& repn)
{
    // SHARED_PTR
    // var_pointer_t var = std::dynamic_pointer_cast<VariableTerm>(repn);
    // return std::static_pointer_cast<BaseExpressionTerm>( std::make_shared<MonomialTerm>(-1, var)
    // );
    VariableTerm* var = dynamic_cast<VariableTerm*>(repn);
    return CREATE_POINTER(MonomialTerm, -1, var);
}

void VariableTerm::set_lb(double val)
{
    if (lb) DISOWN_POINTER(lb);
    OWN_POINTER(lb = CREATE_POINTER(ConstantTerm, val));
}

void VariableTerm::set_lb(const expr_pointer_t val)
{
    if (lb) DISOWN_POINTER(lb);
    OWN_POINTER(lb = val);
}

void VariableTerm::set_ub(double val)
{
    if (ub) DISOWN_POINTER(ub);
    OWN_POINTER(ub = CREATE_POINTER(ConstantTerm, val));
}

void VariableTerm::set_ub(const expr_pointer_t val)
{
    if (ub) DISOWN_POINTER(ub);
    OWN_POINTER(ub = val);
}

void VariableTerm::set_value(double val)
{
    if (value) DISOWN_POINTER(value);
    OWN_POINTER(value = CREATE_POINTER(ConstantTerm, val));
}

void VariableTerm::set_value(const expr_pointer_t val)
{
    if (value) DISOWN_POINTER(value);
    OWN_POINTER(value = val);
}

//
// MonomialTerm
//

MonomialTerm::MonomialTerm(double lhs, VariableTerm* rhs) : coef(lhs), var(rhs)
{
    OWN_POINTER(var);
}

MonomialTerm::~MonomialTerm() { DISOWN_POINTER(var); }

expr_pointer_t MonomialTerm::negate(const expr_pointer_t&)
{
    // SHARED_PTR
    // return std::static_pointer_cast<BaseExpressionTerm>( std::make_shared<MonomialTerm>(-1*coef,
    // var) );
    return CREATE_POINTER(MonomialTerm, -1 * coef, var);
}

}  // namespace coek
