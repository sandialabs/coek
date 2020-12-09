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
