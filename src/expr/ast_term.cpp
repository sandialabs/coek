#include <cstdio>
#include <memory>
#include <sstream>
#include <cassert>
#include "ast_term.hpp"

namespace coek {


void write_expr(expr_pointer_t, std::ostream& );


DummyConstraintTerm::DummyConstraintTerm()
    : ConstraintTerm()
{
refcount=2;
body = this;
#ifdef WITH_AST_ENV
env.cache(this);
#endif
}


// GCOVR_EXCL_START
#ifdef WITH_AST_ENV
bool ASTEnvironment::debug = true;

ASTEnvironment env;

ASTEnvironment::~ASTEnvironment()
{
if (debug) {
    std::cout << "Memory Check: ";
    if (undeleted_memory()) {
        std::cout << "ERROR" << std::endl;
        write(std::cout);
        std::cout << std::endl << std::flush;
        }
    else
        std::cout << "OK" << std::endl;
    }
}

bool ASTEnvironment::undeleted_memory()
{
return (data.size() > num_global_constants);
#if 0
unsigned int count=0;
for (cache_t::iterator it=data.begin(); it != data.end(); ++it)
    count += it->second;
return (count > num_global_constants);
#endif
}

void ASTEnvironment::write(std::ostream& ostr)
{
for (cache_t::iterator it=data.begin(); it != data.end(); ++it) {
    ostr << "  " << it->second << " : ";
    write_expr(it->first, ostr);
    ostr << std::endl;
    }
ostr << std::flush;
}

bool ASTEnvironment::check_memory()
{
bool flag = undeleted_memory();
if (!flag)
    return true;

std::cout << "ERROR: Detected Undeleted Memory" << std::endl;
write(std::cout);
std::cout << std::endl << std::flush;

reset();
return false;
}

void ASTEnvironment::reset()
{
data.clear();
cache(&OneConstant, 1);
cache(&ZeroConstant, 1);
cache(&NegativeOneConstant, 1);
cache(&DummyConstraint, 1);
}
#else


ConstantTerm ZeroConstant(0, 1);
ConstantTerm OneConstant(1, 1);
ConstantTerm NegativeOneConstant(-1, 1);
DummyConstraintTerm DummyConstraint;
#endif
// GCOVR_EXCL_STOP


expr_pointer_t BaseExpressionTerm::const_mult(double coef, const expr_pointer_t& repn)
{
expr_pointer_t lhs = CREATE_POINTER(ConstantTerm, coef);
return CREATE_POINTER(TimesTerm, lhs, repn);
}

expr_pointer_t BaseExpressionTerm::negate(const expr_pointer_t& repn)
{
// SHARED_PTR
//return std::static_pointer_cast<BaseExpressionTerm>( std::make_shared<NegateTerm>(repn) );
return CREATE_POINTER(NegateTerm, repn);
}

//
// ConstantTerm
//

expr_pointer_t ConstantTerm::negate(const expr_pointer_t& repn)
{
// SHARED_PTR
//return std::static_pointer_cast<BaseExpressionTerm>( std::make_shared<ConstantTerm>(-1*value) );
return CREATE_POINTER(ConstantTerm, -1*value);
}

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

int VariableTerm::count = 0;

VariableTerm::VariableTerm(double _lb, double _ub, double _value, bool _binary, bool _integer, bool _indexed)
    : initialize(_value),
      value(_value),
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

expr_pointer_t MonomialTerm::negate(const expr_pointer_t& repn)
{
// SHARED_PTR
//return std::static_pointer_cast<BaseExpressionTerm>( std::make_shared<MonomialTerm>(-1*coef, var) );
return CREATE_POINTER(MonomialTerm, -1*coef, var);
}

//
// ConstraintTerm
//
int ConstraintTerm::count = 0;

ConstraintTerm::ConstraintTerm()
    : body(0)
{
index = count++;
}

ConstraintTerm::ConstraintTerm(const expr_pointer_t& repn)
    : body(repn)
{
OWN_POINTER(body);
}

ConstraintTerm::~ConstraintTerm()
{
if (body)
    DISOWN_POINTER(body);
}

//
// UnaryTerm
//

UnaryTerm::UnaryTerm(const expr_pointer_t& repn)
    : body(repn)
{
OWN_POINTER(body);
}

UnaryTerm::~UnaryTerm()
{
DISOWN_POINTER(body);
}

BinaryTerm::BinaryTerm(const expr_pointer_t& _lhs, const expr_pointer_t& _rhs)
    : lhs(_lhs), rhs(_rhs)
{
OWN_POINTER(lhs);
OWN_POINTER(rhs);
}

BinaryTerm::~BinaryTerm()
{
DISOWN_POINTER(lhs);
DISOWN_POINTER(rhs);
}

NAryPrefixTerm::~NAryPrefixTerm()
{
if (data->size() == n) {
    shared_t::iterator end = data->end();
    for (shared_t::iterator it=data->begin(); it != end; ++it)
        DISOWN_POINTER(*it);
    data->resize(0);
    }
}

void NAryPrefixTerm::initialize(const expr_pointer_t& lhs, const expr_pointer_t& rhs)
{
data = std::make_shared<shared_t>();
data->push_back(lhs);
data->push_back(rhs);
//n = data->size();
n = 2;
OWN_POINTER(lhs);
OWN_POINTER(rhs);
}

void NAryPrefixTerm::initialize(NAryPrefixTerm* lhs, const expr_pointer_t& rhs)
{
data = lhs->data;
data->push_back(rhs);
n = data->size();
OWN_POINTER(rhs);
}

void NAryPrefixTerm::push_back(const expr_pointer_t& rhs)
{
assert(n == data->size());
data->push_back(rhs);
n = data->size();
OWN_POINTER(rhs);
}

//
// PlusTerm
//

PlusTerm::PlusTerm(const expr_pointer_t& lhs, const expr_pointer_t& rhs)
{
PlusTerm* _lhs = dynamic_cast<PlusTerm*>(lhs);
if (_lhs && (_lhs->n == _lhs->data->size())) {
    //std::cout << "PLUS-Extend" << std::endl;
    initialize(_lhs, rhs);
    }
else {
    //std::cout << "PLUS-NEW" << std::endl;
    initialize(lhs, rhs);
    }
}

PlusTerm::PlusTerm(const expr_pointer_t& lhs, const expr_pointer_t& rhs, bool dummy)
{
initialize(lhs, rhs);
}

//
// VariableRefTerm
//

VariableRefTerm::VariableRefTerm(const std::vector<refarg_types>& _indices, const std::string& _name, void* _var)
    : indices(_indices), name(_name), var(_var)
{
for (auto it=indices.begin(); it != indices.end(); ++it) {
    if (auto ival = std::get_if<expr_pointer_t>(&(*it))) {
        OWN_POINTER(*ival);
        }
    }
}

VariableRefTerm::~VariableRefTerm()
{
for (auto it=indices.begin(); it != indices.end(); ++it) {
    if (auto ival = std::get_if<expr_pointer_t>(&(*it))) {
        DISOWN_POINTER(*ival);
        }
    }
}

expr_pointer_t create_varref(const std::vector<refarg_types>& indices, const std::string& name, void* var)
{
return CREATE_POINTER(VariableRefTerm, indices, name, var);
}

//
// SetRefTerm
//

SetRefTerm::SetRefTerm(const expr_pointer_t& repn)
    : body(repn)
{
OWN_POINTER(body);
}

SetRefTerm::~SetRefTerm()
{
DISOWN_POINTER(body);
}


}

