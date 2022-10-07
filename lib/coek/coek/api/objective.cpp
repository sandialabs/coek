#include "objective.hpp"

#include "../ast/base_terms.hpp"
#include "../ast/constraint_terms.hpp"
#include "../ast/visitor_fns.hpp"
#include "expression.hpp"

namespace coek {

Objective::Objective()
{
    Expression e(0);
    repn = CREATE_POINTER(ObjectiveTerm, e.repn, true);
    OWN_POINTER(repn);
}

Objective::Objective(const ObjectiveRepn& _repn) : repn(_repn) { OWN_POINTER(repn); }

Objective::Objective(const Objective& expr)
{
    repn = expr.repn;
    OWN_POINTER(repn);
}

Objective::~Objective() { DISOWN_POINTER(repn); }

Objective& Objective::operator=(const Objective& expr)
{
    DISOWN_POINTER(repn);
    repn = expr.repn;
    OWN_POINTER(repn);
    return *this;
}

unsigned int Objective::id() const { return repn->index; }

double Objective::value() const { return repn->eval(); }

Objective& Objective::expr(const Expression& body)
{
    DISOWN_POINTER(repn->body);
    OWN_POINTER(body.repn);
    repn->body = body.repn;
    return *this;
}

Expression Objective::expr() const { return repn->body; }

Objective& Objective::sense(bool sense)
{
    repn->sense = sense;
    return *this;
}

bool Objective::sense() const { return repn->sense; }

Objective& Objective::name(const std::string& name)
{
    repn->name = name;
    return *this;
}

std::string Objective::name() const { return repn->name; }

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

Objective objective() { return Objective(); }

Objective objective(const std::string& name)
{
    Objective tmp;
    return tmp.name(name);
}

Objective objective(const Expression& expr)
{
    Objective tmp;
    return tmp.expr(expr);
}

Objective objective(const std::string& name, const Expression& expr)
{
    Objective tmp;
    return tmp.name(name).expr(expr);
}

}  // namespace coek
