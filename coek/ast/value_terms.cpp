#include "value_terms.hpp"
#include "expr_terms.hpp"


namespace coek {

#if 0
//
// ConstantTerm
//

expr_pointer_t ConstantTerm::negate(const expr_pointer_t& repn)
{
// SHARED_PTR
//return std::static_pointer_cast<BaseExpressionTerm>( std::make_shared<ConstantTerm>(-1*value) );
return CREATE_POINTER(ConstantTerm, -1*value);
}
#endif

//
// ParameterTerm
//

expr_pointer_t ParameterTerm::negate(const expr_pointer_t& repn)
{
// SHARED_PTR
//return std::static_pointer_cast<BaseExpressionTerm>( std::make_shared<NegateTerm>(repn) );
return CREATE_POINTER(NegateTerm, repn);
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
if (type == 1)
    return double_value;
if (type == 2)
    return int_value;
throw std::runtime_error("Accessing the value of a non-numeric abstract parameter: "+string_value);
}

void IndexParameterTerm::set_value(double value)
{ type = 1; double_value = value; }

void IndexParameterTerm::set_value(int value)
{ type = 2; int_value = value; }

void IndexParameterTerm::set_value(const std::string& value)
{ type = 3; string_value = value; }

void IndexParameterTerm::get_value(double& value)
{
if (type != 1)
    throw std::runtime_error("No double value stored in index parameter.");
value = double_value;
}

void IndexParameterTerm::get_value(int& value)
{
if (type != 2)
    throw std::runtime_error("No integer value stored in index parameter.");
value = int_value;
}

void IndexParameterTerm::get_value(std::string& value)
{
if (type != 3)
    throw std::runtime_error("No string value stored in index parameter.");
value = string_value;
}

//
// VariableTerm
//

unsigned int VariableTerm::count = 0;

VariableTerm::VariableTerm(double _lb, double _ub, double _value, bool _binary, bool _integer, bool _indexed)
    : value(_value),
      lb(_lb),
      ub(_ub),
      binary(_binary),
      integer(_integer),
      fixed(false),
      indexed(_indexed)
{
index = count++;
}

expr_pointer_t VariableTerm::const_mult(double coef, const expr_pointer_t& repn)
{
VariableTerm* var = dynamic_cast<VariableTerm*>(repn);
return CREATE_POINTER(MonomialTerm, coef, var);
}

expr_pointer_t VariableTerm::negate(const expr_pointer_t& repn)
{
// SHARED_PTR
//var_pointer_t var = std::dynamic_pointer_cast<VariableTerm>(repn);
//return std::static_pointer_cast<BaseExpressionTerm>( std::make_shared<MonomialTerm>(-1, var) );
VariableTerm* var = dynamic_cast<VariableTerm*>(repn);
return CREATE_POINTER(MonomialTerm, -1, var);
}




//
// MonomialTerm
//

MonomialTerm::MonomialTerm(double lhs, VariableTerm* rhs)
    : coef(lhs), var(rhs)
{
OWN_POINTER(var);
}

MonomialTerm::~MonomialTerm()
{
DISOWN_POINTER(var);
}

expr_pointer_t MonomialTerm::negate(const expr_pointer_t& )
{
// SHARED_PTR
//return std::static_pointer_cast<BaseExpressionTerm>( std::make_shared<MonomialTerm>(-1*coef, var) );
return CREATE_POINTER(MonomialTerm, -1*coef, var);
}

}
