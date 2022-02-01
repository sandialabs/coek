#include "../ast/base_terms.hpp"
#include "../ast/constraint_terms.hpp"
#include "../ast/visitor_fns.hpp"
#include "expression.hpp"
#include "objective.hpp"


namespace coek {


Objective::Objective()
    : repn(DUMMYOBJECTIVE)
{ OWN_POINTER(repn); }

Objective::Objective(const ObjectiveRepn& _repn)
    : repn(_repn)
{ OWN_POINTER(repn); }

Objective::Objective(const Expression& arg, bool sense)
{
repn = CREATE_POINTER(ObjectiveTerm, arg.repn, sense);
OWN_POINTER(repn);
}

Objective::Objective(const std::string& name, const Expression& arg, bool sense)
{
repn = CREATE_POINTER(ObjectiveTerm, arg.repn, sense);
OWN_POINTER(repn);
repn->name = name;
}

Objective::Objective(const Objective& expr)
{
repn = expr.repn;
OWN_POINTER(repn);
}

Objective::~Objective()
{ DISOWN_POINTER(repn); }

Objective& Objective::operator=(const Objective& expr)
{
DISOWN_POINTER(repn);
repn = expr.repn;
OWN_POINTER(repn);
return *this;
}

unsigned int Objective::id() const
{ return repn->index; }

double Objective::get_value() const
{ return repn->eval(); }

void Objective::set_body(const Expression& body)
{
DISOWN_POINTER(repn->body);
OWN_POINTER(body.repn);
repn->body = body.repn;
}

Expression Objective::get_body() const
{ return repn->body; }

void Objective::set_sense(bool sense)
{ repn->sense = sense; }

bool Objective::get_sense() const
{ return repn->sense; }

void Objective::set_name(const std::string& name)
{ repn->name = name; }

std::string Objective::get_name() const
{ return repn->name; }

std::list<std::string> Objective::to_list() const
{
std::list<std::string> tmp;
expr_to_list(repn, tmp);
return tmp;
}

std::ostream& operator<<(std::ostream& ostr, const Objective& arg)
{
write_expr(arg.repn, ostr);
return ostr;
}

}
