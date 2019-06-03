#include "expr/expr_types.hpp"
#include "expr/expr_manager_objects.hpp"


ExprManager_Objects::~ExprManager_Objects(void)
{
for (std::map<numval_t,ExprRepn*>::iterator it=repn.begin(); it != repn.end(); it++)
    delete it->second;
for (std::list<NumericValue*>::iterator it=owned.begin(); it != owned.end(); it++)
    delete *it;
}

ExprManager_Objects::numval_t ExprManager_Objects::negate(numval_t expr)
{
NumericValue* e = static_cast<NumericValue*>(expr);
Expression* tmp = new NegExpression(e->manager, e);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::plus(numval_t lhs, numval_t rhs)
{
if (lhs == &ZeroParameter)
    return rhs;
if (rhs == &ZeroParameter)
    return lhs;

assert(lhs->manager == rhs->manager);

NumericValue* tmp;
if (lhs->is_parameter() and !lhs->is_mutable_parameter() and rhs->is_parameter() and !rhs->is_mutable_parameter())
    tmp = new TypedParameter<double>(lhs->manager, lhs->_value + rhs->_value, false);
else
    tmp = new AddExpression(lhs->manager, lhs,rhs);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::minus(numval_t lhs, numval_t rhs)
{
if (lhs == &ZeroParameter)
    return this->negate(rhs);
if (rhs == &ZeroParameter)
    return lhs;

assert(lhs->manager == rhs->manager);

NumericValue* tmp;
if (lhs->is_parameter() and !lhs->is_mutable_parameter() and rhs->is_parameter() and !rhs->is_mutable_parameter())
    tmp = new TypedParameter<double>(lhs->manager, lhs->_value - rhs->_value, false);
else
    tmp = new SubExpression(lhs->manager, lhs, rhs);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::times(numval_t lhs, numval_t rhs)
{
if (lhs == &ZeroParameter or rhs == &ZeroParameter)
    return &ZeroParameter;
if (lhs == &OneParameter)
    return rhs;
if (rhs == &OneParameter)
    return lhs;
if (lhs == &NegativeOneParameter)
    return this->negate(rhs);
if (rhs == &NegativeOneParameter)
    return this->negate(lhs);

assert(lhs->manager == rhs->manager);

NumericValue* tmp;
if (lhs->is_parameter() and !lhs->is_mutable_parameter() and rhs->is_parameter() and !rhs->is_mutable_parameter())
    tmp = new TypedParameter<double>(lhs->manager, lhs->_value * rhs->_value, false);
else
    tmp = new MulExpression(lhs->manager, lhs, rhs);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::divide(numval_t lhs, numval_t rhs)
{
if (lhs == &ZeroParameter)
    return &ZeroParameter;
if (rhs == &OneParameter)
    return lhs;
if (rhs == &NegativeOneParameter)
    return this->negate(lhs);

assert(lhs->manager == rhs->manager);

if (rhs->is_parameter() and !rhs->is_mutable_parameter() and (rhs->_value==0.0))
   return 0;

NumericValue* tmp;
if (lhs->is_parameter() and !lhs->is_mutable_parameter() and rhs->is_parameter() and !rhs->is_mutable_parameter()) {
    tmp = new TypedParameter<double>(lhs->manager, lhs->_value / rhs->_value, false);
    }
else {
    tmp = new DivExpression(lhs->manager, lhs, rhs);
    }
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::abs(numval_t expr)
{
Expression* tmp = new AbsExpression(expr->manager, expr);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::pow(numval_t lhs, numval_t rhs)
{
if (lhs == &ZeroParameter)
    return &ZeroParameter;
if (lhs == &OneParameter)
    return &OneParameter;
if (rhs == &ZeroParameter)
    return &OneParameter;
if (rhs == &OneParameter)
    return lhs;
assert(lhs->manager == rhs->manager);

Expression* tmp = new PowExpression(lhs->manager, lhs, rhs);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::ceil(numval_t expr)
{
Expression* tmp = new CeilExpression(expr->manager, expr);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::floor(numval_t expr)
{
Expression* tmp = new FloorExpression(expr->manager, expr);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::exp(numval_t expr)
{
Expression* tmp = new ExpExpression(expr->manager, expr);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::log(numval_t expr)
{
Expression* tmp = new LogExpression(expr->manager, expr);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::log10(numval_t expr)
{
Expression* tmp = new Log10Expression(expr->manager, expr);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::sqrt(numval_t expr)
{
Expression* tmp = new SqrtExpression(expr->manager, expr);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::sin(numval_t expr)
{
Expression* tmp = new SinExpression(expr->manager, expr);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::cos(numval_t expr)
{
Expression* tmp = new CosExpression(expr->manager, expr);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::tan(numval_t expr)
{
Expression* tmp = new TanExpression(expr->manager, expr);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::sinh(numval_t expr)
{
Expression* tmp = new SinhExpression(expr->manager, expr);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::cosh(numval_t expr)
{
Expression* tmp = new CoshExpression(expr->manager, expr);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::tanh(numval_t expr)
{
Expression* tmp = new TanhExpression(expr->manager, expr);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::asin(numval_t expr)
{
Expression* tmp = new AsinExpression(expr->manager, expr);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::acos(numval_t expr)
{
Expression* tmp = new AcosExpression(expr->manager, expr);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::atan(numval_t expr)
{
Expression* tmp = new AtanExpression(expr->manager, expr);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::asinh(numval_t expr)
{
Expression* tmp = new AsinhExpression(expr->manager, expr);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::acosh(numval_t expr)
{
Expression* tmp = new AcoshExpression(expr->manager, expr);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::atanh(numval_t expr)
{
Expression* tmp = new AtanhExpression(expr->manager, expr);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::inequality(numval_t expr, bool strict)
{
Expression* tmp = new InequalityExpression(expr->manager, expr, strict);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::equality(numval_t expr)
{
Expression* tmp = new EqualityExpression(expr->manager, expr);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::_var(bool _binary, bool _integer, double lb, double ub, double init, const char* name)
{
Variable* tmp = new Variable(this, _binary, _integer, lb, ub, init, name);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::_param(int _value, bool _mutable, const char* name)
{
if (! _mutable) {
    if (_value == 0)
       return &ZeroParameter;
    if (_value == 1)
       return &OneParameter;
    if (_value == -1)
       return &NegativeOneParameter;
    }

Parameter* tmp = new TypedParameter<int>(this, _value, _mutable, name);
owned.push_back(tmp);
return tmp;
}

ExprManager_Objects::numval_t ExprManager_Objects::_param(double _value, bool _mutable, const char* name)
{
if (! _mutable) {
    if (_value == 0.0)
       return &ZeroParameter;
    if (_value == 1.0)
       return &OneParameter;
    if (_value == -1.0)
       return &NegativeOneParameter;
    }

Parameter* tmp = new TypedParameter<double>(this, _value, _mutable, name);
owned.push_back(tmp);
return tmp;
}

std::list<std::string> ExprManager_Objects::expr_to_list(apival_t e, bool values)
{
NumericValue* tmp = static_cast<NumericValue*>(e);
return ::expr_to_list(tmp, values);
}

double ExprManager_Objects::constant(apival_t e)
{
NumericValue* e_ = static_cast<NumericValue*>(e);
ExprRepn* repn = get_repn(e_);
return repn->constval;
}

int ExprManager_Objects::nlinear_vars(apival_t e)
{
NumericValue* e_ = static_cast<NumericValue*>(e);
ExprRepn* repn = get_repn(e_);
return repn->linear_vars.size();
}

apival_t ExprManager_Objects::linear_var(apival_t e, int i)
{
NumericValue* e_ = static_cast<NumericValue*>(e);
ExprRepn* repn = get_repn(e_);
if ((i < 0) or (i >= repn->linear_vars.size()))
   return 0;
return repn->linear_vars[i];
}

double ExprManager_Objects::linear_coef(apival_t e, int i)
{
NumericValue* e_ = static_cast<NumericValue*>(e);
ExprRepn* repn = get_repn(e_);
return repn->linear_coefs[i];
}

ExprRepn* ExprManager_Objects::get_repn(numval_t e)
{
std::map<numval_t,ExprRepn*>::iterator it = repn.find(e);
if (it != repn.end())
   return it->second;

ExprRepn* ans = new ExprRepn();
ans->initialize(e);
repn[e] = ans;
return ans;
}


/*
// Forward reference
void collect_terms(NumericValue* e, ExprRepn& repn);

void collect_terms_(Parameter* e, ExprRepn& repn)
{ repn.constval = e->value(); }

void collect_terms_(Variable* e, ExprRepn& repn)
{
repn.linear_vars.push_back(e);
repn.linear_coefs.push_back(1.0);
}

void collect_terms_(InequalityExpression* e, ExprRepn& repn)
{ collect_terms(e->body, repn); }

void collect_terms_(EqualityExpression* e, ExprRepn& repn)
{ collect_terms(e->body, repn); }

void collect_terms_(AddExpression* e, ExprRepn& repn)
{
ExprRepn lhs;
collect_terms(e->lhs, lhs);
repn.constval += lhs.constval;
for (size_t i=0; i<lhs.linear_vars.size(); i++) {
    repn.linear_vars.push_back(lhs.linear_vars[i]);
    repn.linear_coefs.push_back(lhs.linear_coefs[i]);
    }

ExprRepn rhs;
collect_terms(e->rhs, rhs);
repn.constval += rhs.constval;
for (size_t i=0; i<rhs.linear_vars.size(); i++) {
    repn.linear_vars.push_back(rhs.linear_vars[i]);
    repn.linear_coefs.push_back(rhs.linear_coefs[i]);
    }
}

void collect_terms_(SubExpression* e, ExprRepn& repn)
{
ExprRepn lhs;
collect_terms(e->lhs, lhs);
repn.constval += lhs.constval;
for (size_t i=0; i<lhs.linear_vars.size(); i++) {
    repn.linear_vars.push_back(lhs.linear_vars[i]);
    repn.linear_coefs.push_back(lhs.linear_coefs[i]);
    }

ExprRepn rhs;
collect_terms(e->rhs, rhs);
repn.constval -= rhs.constval;
for (size_t i=0; i<rhs.linear_vars.size(); i++) {
    repn.linear_vars.push_back(rhs.linear_vars[i]);
    repn.linear_coefs.push_back(-1*rhs.linear_coefs[i]);
    }
}

void collect_terms_(MulExpression* e, ExprRepn& repn)
{
ExprRepn lhs;
collect_terms(e->lhs, lhs);
if (lhs.linear_vars.size() == 0) {
    if (lhs.constval == 0.0)
        return;
    ExprRepn rhs;
    collect_terms(e->rhs, rhs);
    repn.constval += lhs.constval * rhs.constval;
    for (size_t i=0; i<rhs.linear_vars.size(); i++) {
        repn.linear_vars.push_back(rhs.linear_vars[i]);
        repn.linear_coefs.push_back(lhs.constval * rhs.linear_coefs[i]);
        }
    }
else {
    ExprRepn rhs;
    collect_terms(e->rhs, rhs);
    if (rhs.linear_vars.size() == 0) {
        if (rhs.constval == 0.0)
            return;
        repn.constval += lhs.constval * rhs.constval;
        for (size_t i=0; i<lhs.linear_vars.size(); i++) {
            repn.linear_vars.push_back(lhs.linear_vars[i]);
            repn.linear_coefs.push_back(lhs.linear_coefs[i] * rhs.constval);
            }
        }
    else {
        throw std::runtime_error("Nonlinear expressions are not currently supported.");
        }
    }
}

void collect_terms(NumericValue* e, ExprRepn& repn)
{
// TODO
//std::cout << typeid(*e).name() << std::endl;

if (e->is_parameter())
    collect_terms_(dynamic_cast<Parameter*>(e), repn);

else if (typeid(*e) == typeid(Variable))
    collect_terms_(dynamic_cast<Variable*>(e), repn);

else if (typeid(*e) == typeid(InequalityExpression))
    collect_terms_(dynamic_cast<InequalityExpression*>(e), repn);

else if (typeid(*e) == typeid(EqualityExpression))
    collect_terms_(dynamic_cast<EqualityExpression*>(e), repn);

else if (typeid(*e) == typeid(MulExpression))
    collect_terms_(dynamic_cast<MulExpression*>(e), repn);

else if (typeid(*e) == typeid(AddExpression))
    collect_terms_(dynamic_cast<AddExpression*>(e), repn);

else if (typeid(*e) == typeid(SubExpression))
    collect_terms_(dynamic_cast<SubExpression*>(e), repn);

else {
    std::string str = "Unknown expression term: ";
    str += typeid(*e).name();
    str += " ( ";
    std::ostringstream ostr;
    e->print(ostr);
    str += ostr.str();
    str += " )";
    throw std::runtime_error(str.c_str());
    }
}
*/

//void ExprRepn::initialize(NumericValue* e)
//{
//e->collect_terms(*this);
// TODO
/*
Parameter
Variable
Constraint

AbsExpression
NegExpression
CeilExpression
FlooExpression
ExpExpression
LogExpression
Log10Expression
SqrtExpression
SinExpression
CosExpression
TanExpression
AsinExpression
AcosExpression
AtanExpression
SinhExpression
CoshExpression
TanhExpression
AsinhExpression
AcoshExpression
AtanhExpression

PowExpression
AddExpression
SubExpression
MulExpression
DivExpression
*/
//}

