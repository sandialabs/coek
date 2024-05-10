#include "objective.hpp"

#include "../ast/base_terms.hpp"
#include "../ast/constraint_terms.hpp"
#include "../ast/visitor_fns.hpp"
#include "expression.hpp"

namespace coek {

Objective::Objective() { repn = CREATE_POINTER(ObjectiveTerm, ZEROCONST, true); }

Objective::Objective(const ObjectiveRepn& _repn) : repn(_repn) {}

Objective::Objective(const Objective& expr) : repn(expr.repn) {}

Objective& Objective::operator=(const Objective& expr)
{
    repn = expr.repn;
    return *this;
}

bool Objective::active() const { return repn->active; }

void Objective::activate() { repn->active = true; }

void Objective::deactivate() { repn->active = false; }

size_t Objective::id() const { return repn->index; }

double Objective::value() const { return repn->eval(); }

Objective& Objective::expr(const Expression& body)
{
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

std::string Objective::name() const { return repn->get_name(); }

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
