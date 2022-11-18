#include "../api/constants.hpp"
#include "value_terms.hpp"
#include "expr_terms.hpp"

namespace coek {

//
// ParameterTerm
//

unsigned int ParameterTerm::count = 0;

ParameterTerm::ParameterTerm()
{
    non_variable = true;
    value = std::make_shared<ConstantTerm>(0.0);
    index = count++;
}

ParameterTerm::ParameterTerm(const expr_pointer_t& _value) : value(_value)
{
    non_variable = true;
    index = count++;
}

expr_pointer_t ParameterTerm::negate(const expr_pointer_t& repn)
{
    return std::make_shared<NegateTerm>(repn);
}

void ParameterTerm::set_value(double val) { value = std::make_shared<ConstantTerm>(val); }

void ParameterTerm::set_value(const expr_pointer_t val) { value = val; }

//
// IndexParameterTerm
//

expr_pointer_t IndexParameterTerm::negate(const expr_pointer_t& repn)
{
    return std::make_shared<NegateTerm>(repn);
}

expr_pointer_t create_abstract_parameter(const std::string& name)
{
    return std::make_shared<IndexParameterTerm>(name);
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

std::shared_ptr<ConstantTerm> VariableTerm::negative_infinity
    = std::make_shared<ConstantTerm>(-COEK_INFINITY);
std::shared_ptr<ConstantTerm> VariableTerm::positive_infinity
    = std::make_shared<ConstantTerm>(COEK_INFINITY);
std::shared_ptr<ConstantTerm> VariableTerm::nan = std::make_shared<ConstantTerm>(COEK_NAN);

unsigned int VariableTerm::count = 0;

VariableTerm::VariableTerm(const expr_pointer_t& _lb, const expr_pointer_t& _ub,
                           const expr_pointer_t& _value, bool _binary, bool _integer)
    : value(_value), lb(_lb), ub(_ub), binary(_binary), integer(_integer), fixed(false)
{
    index = count++;
}

expr_pointer_t VariableTerm::const_mult(double coef, const expr_pointer_t& repn)
{
    return std::make_shared<MonomialTerm>(coef, std::dynamic_pointer_cast<VariableTerm>(repn));
}

expr_pointer_t VariableTerm::negate(const expr_pointer_t& repn)
{
    return std::make_shared<MonomialTerm>(-1, std::dynamic_pointer_cast<VariableTerm>(repn));
}

void VariableTerm::set_lb(double val) { lb = std::make_shared<ConstantTerm>(val); }

void VariableTerm::set_lb(const expr_pointer_t val) { lb = val; }

void VariableTerm::set_ub(double val) { ub = std::make_shared<ConstantTerm>(val); }

void VariableTerm::set_ub(const expr_pointer_t val) { ub = val; }

void VariableTerm::set_value(double val) { value = std::make_shared<ConstantTerm>(val); }

void VariableTerm::set_value(const expr_pointer_t val) { value = val; }

//
// MonomialTerm
//

MonomialTerm::MonomialTerm(double lhs, const std::shared_ptr<VariableTerm>& rhs)
    : coef(lhs), var(rhs)
{
}

expr_pointer_t MonomialTerm::negate(const expr_pointer_t&)
{
    return std::make_shared<MonomialTerm>(-1 * coef, var);
}

}  // namespace coek
