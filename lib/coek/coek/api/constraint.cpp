#include "../ast/base_terms.hpp"
#include "../ast/constraint_terms.hpp"
#include "../ast/expr_terms.hpp"
#include "../ast/value_terms.hpp"
#include "../ast/visitor_fns.hpp"
#include "../ast/ast_operators.hpp"
#include "expression.hpp"
#include "constraint.hpp"

namespace coek {

#ifdef COEK_WITH_COMPACT_MODEL
std::shared_ptr<ConstraintTerm> convert_con_template(std::shared_ptr<ConstraintTerm> con);
#endif

//
// Constraint
//

Constraint::Constraint() : repn(EmptyConstraintRepn) {}

Constraint::Constraint(const ConstraintRepn& _repn) : repn(_repn) {}

Constraint::Constraint(const Constraint& expr) : repn(expr.repn) {}

Constraint& Constraint::operator=(const Constraint& expr)
{
    repn = expr.repn;
    return *this;
}

unsigned int Constraint::id() const { return repn->index; }

bool Constraint::is_inequality() const { return repn->is_inequality(); }

bool Constraint::is_equality() const { return repn->is_equality(); }

bool Constraint::is_feasible() const { return repn->is_feasible(); }

Expression Constraint::lower() const { return repn->lower; }

Expression Constraint::body() const { return repn->body; }

Expression Constraint::upper() const { return repn->upper; }

bool Constraint::has_lower()
{
    if (repn->lower)
        return true;
    else
        return false;
}

bool Constraint::has_upper()
{
    if (repn->upper)
        return true;
    else
        return false;
}

Constraint& Constraint::name(const std::string& name)
{
    repn->name = name;
    return *this;
}

std::string Constraint::name() const { return repn->get_name(); }

Constraint Constraint::expand()
{
#ifdef COEK_WITH_COMPACT_MODEL
    return convert_con_template(repn);
#else
    return repn;
#endif
}

std::list<std::string> Constraint::to_list() const
{
    std::list<std::string> tmp;
    expr_to_list(repn, tmp);
    return tmp;
}

std::ostream& operator<<(std::ostream& ostr, const Constraint& arg)
{
    write_expr(arg.repn, ostr);
    return ostr;
}

//
// operator<
//
Constraint operator<(int lhs, const Parameter& rhs) { return less_than(lhs, rhs.repn, true); }
Constraint operator<(int lhs, const IndexParameter& rhs) { return less_than(lhs, rhs.repn, true); }
Constraint operator<(int lhs, const Variable& rhs) { return less_than(lhs, rhs.repn, true); }
Constraint operator<(int lhs, const Expression& rhs) { return less_than(lhs, rhs.repn, true); }
Constraint operator<(double lhs, const Parameter& rhs) { return less_than(lhs, rhs.repn, true); }
Constraint operator<(double lhs, const IndexParameter& rhs)
{
    return less_than(lhs, rhs.repn, true);
}
Constraint operator<(double lhs, const Variable& rhs) { return less_than(lhs, rhs.repn, true); }
Constraint operator<(double lhs, const Expression& rhs) { return less_than(lhs, rhs.repn, true); }

Constraint operator<(const Parameter& lhs, double rhs) { return less_than(lhs.repn, rhs, true); }
Constraint operator<(const Parameter& lhs, int rhs) { return less_than(lhs.repn, rhs, true); }
Constraint operator<(const Parameter& lhs, const Parameter& rhs)
{
    return less_than(lhs.repn, rhs.repn, true);
}
Constraint operator<(const Parameter& lhs, const IndexParameter& rhs)
{
    return less_than(lhs.repn, rhs.repn, true);
}
Constraint operator<(const Parameter& lhs, const Variable& rhs)
{
    return less_than(lhs.repn, rhs.repn, true);
}
Constraint operator<(const Parameter& lhs, const Expression& rhs)
{
    return less_than(lhs.repn, rhs.repn, true);
}

Constraint operator<(const IndexParameter& lhs, double rhs)
{
    return less_than(lhs.repn, rhs, true);
}
Constraint operator<(const IndexParameter& lhs, int rhs) { return less_than(lhs.repn, rhs, true); }
Constraint operator<(const IndexParameter& lhs, const Parameter& rhs)
{
    return less_than(lhs.repn, rhs.repn, true);
}
Constraint operator<(const IndexParameter& lhs, const IndexParameter& rhs)
{
    return less_than(lhs.repn, rhs.repn, true);
}
Constraint operator<(const IndexParameter& lhs, const Variable& rhs)
{
    return less_than(lhs.repn, rhs.repn, true);
}
Constraint operator<(const IndexParameter& lhs, const Expression& rhs)
{
    return less_than(lhs.repn, rhs.repn, true);
}

Constraint operator<(const Variable& lhs, double rhs) { return less_than(lhs.repn, rhs, true); }
Constraint operator<(const Variable& lhs, int rhs) { return less_than(lhs.repn, rhs, true); }
Constraint operator<(const Variable& lhs, const Parameter& rhs)
{
    return less_than(lhs.repn, rhs.repn, true);
}
Constraint operator<(const Variable& lhs, const IndexParameter& rhs)
{
    return less_than(lhs.repn, rhs.repn, true);
}
Constraint operator<(const Variable& lhs, const Variable& rhs)
{
    return less_than(lhs.repn, rhs.repn, true);
}
Constraint operator<(const Variable& lhs, const Expression& rhs)
{
    return less_than(lhs.repn, rhs.repn, true);
}

Constraint operator<(const Expression& lhs, double rhs) { return less_than(lhs.repn, rhs, true); }
Constraint operator<(const Expression& lhs, int rhs) { return less_than(lhs.repn, rhs, true); }
Constraint operator<(const Expression& lhs, const Parameter& rhs)
{
    return less_than(lhs.repn, rhs.repn, true);
}
Constraint operator<(const Expression& lhs, const IndexParameter& rhs)
{
    return less_than(lhs.repn, rhs.repn, true);
}
Constraint operator<(const Expression& lhs, const Variable& rhs)
{
    return less_than(lhs.repn, rhs.repn, true);
}
Constraint operator<(const Expression& lhs, const Expression& rhs)
{
    return less_than(lhs.repn, rhs.repn, true);
}

//
// operator<=
//
Constraint operator<=(int lhs, const Parameter& rhs) { return less_than(lhs, rhs.repn, false); }
Constraint operator<=(int lhs, const IndexParameter& rhs)
{
    return less_than(lhs, rhs.repn, false);
}
Constraint operator<=(int lhs, const Variable& rhs) { return less_than(lhs, rhs.repn, false); }
Constraint operator<=(int lhs, const Expression& rhs) { return less_than(lhs, rhs.repn, false); }
Constraint operator<=(double lhs, const Parameter& rhs) { return less_than(lhs, rhs.repn, false); }
Constraint operator<=(double lhs, const IndexParameter& rhs)
{
    return less_than(lhs, rhs.repn, false);
}
Constraint operator<=(double lhs, const Variable& rhs) { return less_than(lhs, rhs.repn, false); }
Constraint operator<=(double lhs, const Expression& rhs) { return less_than(lhs, rhs.repn, false); }

Constraint operator<=(const Parameter& lhs, double rhs) { return less_than(lhs.repn, rhs, false); }
Constraint operator<=(const Parameter& lhs, int rhs) { return less_than(lhs.repn, rhs, false); }
Constraint operator<=(const Parameter& lhs, const Parameter& rhs)
{
    return less_than(lhs.repn, rhs.repn, false);
}
Constraint operator<=(const Parameter& lhs, const IndexParameter& rhs)
{
    return less_than(lhs.repn, rhs.repn, false);
}
Constraint operator<=(const Parameter& lhs, const Variable& rhs)
{
    return less_than(lhs.repn, rhs.repn, false);
}
Constraint operator<=(const Parameter& lhs, const Expression& rhs)
{
    return less_than(lhs.repn, rhs.repn, false);
}

Constraint operator<=(const IndexParameter& lhs, double rhs)
{
    return less_than(lhs.repn, rhs, false);
}
Constraint operator<=(const IndexParameter& lhs, int rhs)
{
    return less_than(lhs.repn, rhs, false);
}
Constraint operator<=(const IndexParameter& lhs, const Parameter& rhs)
{
    return less_than(lhs.repn, rhs.repn, false);
}
Constraint operator<=(const IndexParameter& lhs, const IndexParameter& rhs)
{
    return less_than(lhs.repn, rhs.repn, false);
}
Constraint operator<=(const IndexParameter& lhs, const Variable& rhs)
{
    return less_than(lhs.repn, rhs.repn, false);
}
Constraint operator<=(const IndexParameter& lhs, const Expression& rhs)
{
    return less_than(lhs.repn, rhs.repn, false);
}

Constraint operator<=(const Variable& lhs, double rhs) { return less_than(lhs.repn, rhs, false); }
Constraint operator<=(const Variable& lhs, int rhs) { return less_than(lhs.repn, rhs, false); }
Constraint operator<=(const Variable& lhs, const Parameter& rhs)
{
    return less_than(lhs.repn, rhs.repn, false);
}
Constraint operator<=(const Variable& lhs, const IndexParameter& rhs)
{
    return less_than(lhs.repn, rhs.repn, false);
}
Constraint operator<=(const Variable& lhs, const Variable& rhs)
{
    return less_than(lhs.repn, rhs.repn, false);
}
Constraint operator<=(const Variable& lhs, const Expression& rhs)
{
    return less_than(lhs.repn, rhs.repn, false);
}

Constraint operator<=(const Expression& lhs, double rhs) { return less_than(lhs.repn, rhs, false); }
Constraint operator<=(const Expression& lhs, int rhs) { return less_than(lhs.repn, rhs, false); }
Constraint operator<=(const Expression& lhs, const Parameter& rhs)
{
    return less_than(lhs.repn, rhs.repn, false);
}
Constraint operator<=(const Expression& lhs, const IndexParameter& rhs)
{
    return less_than(lhs.repn, rhs.repn, false);
}
Constraint operator<=(const Expression& lhs, const Variable& rhs)
{
    return less_than(lhs.repn, rhs.repn, false);
}
Constraint operator<=(const Expression& lhs, const Expression& rhs)
{
    return less_than(lhs.repn, rhs.repn, false);
}

//
// operator>
//
Constraint operator>(int lhs, const Parameter& rhs) { return less_than(rhs.repn, lhs, true); }
Constraint operator>(int lhs, const IndexParameter& rhs) { return less_than(rhs.repn, lhs, true); }
Constraint operator>(int lhs, const Variable& rhs) { return less_than(rhs.repn, lhs, true); }
Constraint operator>(int lhs, const Expression& rhs) { return less_than(rhs.repn, lhs, true); }
Constraint operator>(double lhs, const Parameter& rhs) { return less_than(rhs.repn, lhs, true); }
Constraint operator>(double lhs, const IndexParameter& rhs)
{
    return less_than(rhs.repn, lhs, true);
}
Constraint operator>(double lhs, const Variable& rhs) { return less_than(rhs.repn, lhs, true); }
Constraint operator>(double lhs, const Expression& rhs) { return less_than(rhs.repn, lhs, true); }

Constraint operator>(const Parameter& lhs, double rhs) { return less_than(rhs, lhs.repn, true); }
Constraint operator>(const Parameter& lhs, int rhs) { return less_than(rhs, lhs.repn, true); }
Constraint operator>(const Parameter& lhs, const Parameter& rhs)
{
    return less_than(rhs.repn, lhs.repn, true);
}
Constraint operator>(const Parameter& lhs, const IndexParameter& rhs)
{
    return less_than(rhs.repn, lhs.repn, true);
}
Constraint operator>(const Parameter& lhs, const Variable& rhs)
{
    return less_than(rhs.repn, lhs.repn, true);
}
Constraint operator>(const Parameter& lhs, const Expression& rhs)
{
    return less_than(rhs.repn, lhs.repn, true);
}

Constraint operator>(const IndexParameter& lhs, double rhs)
{
    return less_than(rhs, lhs.repn, true);
}
Constraint operator>(const IndexParameter& lhs, int rhs) { return less_than(rhs, lhs.repn, true); }
Constraint operator>(const IndexParameter& lhs, const Parameter& rhs)
{
    return less_than(rhs.repn, lhs.repn, true);
}
Constraint operator>(const IndexParameter& lhs, const IndexParameter& rhs)
{
    return less_than(rhs.repn, lhs.repn, true);
}
Constraint operator>(const IndexParameter& lhs, const Variable& rhs)
{
    return less_than(rhs.repn, lhs.repn, true);
}
Constraint operator>(const IndexParameter& lhs, const Expression& rhs)
{
    return less_than(rhs.repn, lhs.repn, true);
}

Constraint operator>(const Variable& lhs, double rhs) { return less_than(rhs, lhs.repn, true); }
Constraint operator>(const Variable& lhs, int rhs) { return less_than(rhs, lhs.repn, true); }
Constraint operator>(const Variable& lhs, const Parameter& rhs)
{
    return less_than(rhs.repn, lhs.repn, true);
}
Constraint operator>(const Variable& lhs, const IndexParameter& rhs)
{
    return less_than(rhs.repn, lhs.repn, true);
}
Constraint operator>(const Variable& lhs, const Variable& rhs)
{
    return less_than(rhs.repn, lhs.repn, true);
}
Constraint operator>(const Variable& lhs, const Expression& rhs)
{
    return less_than(rhs.repn, lhs.repn, true);
}

Constraint operator>(const Expression& lhs, double rhs) { return less_than(rhs, lhs.repn, true); }
Constraint operator>(const Expression& lhs, int rhs) { return less_than(rhs, lhs.repn, true); }
Constraint operator>(const Expression& lhs, const Parameter& rhs)
{
    return less_than(rhs.repn, lhs.repn, true);
}
Constraint operator>(const Expression& lhs, const IndexParameter& rhs)
{
    return less_than(rhs.repn, lhs.repn, true);
}
Constraint operator>(const Expression& lhs, const Variable& rhs)
{
    return less_than(rhs.repn, lhs.repn, true);
}
Constraint operator>(const Expression& lhs, const Expression& rhs)
{
    return less_than(rhs.repn, lhs.repn, true);
}

//
// operator>=
//
Constraint operator>=(int lhs, const Parameter& rhs) { return less_than(rhs.repn, lhs, false); }
Constraint operator>=(int lhs, const IndexParameter& rhs)
{
    return less_than(rhs.repn, lhs, false);
}
Constraint operator>=(int lhs, const Variable& rhs) { return less_than(rhs.repn, lhs, false); }
Constraint operator>=(int lhs, const Expression& rhs) { return less_than(rhs.repn, lhs, false); }
Constraint operator>=(double lhs, const Parameter& rhs) { return less_than(rhs.repn, lhs, false); }
Constraint operator>=(double lhs, const IndexParameter& rhs)
{
    return less_than(rhs.repn, lhs, false);
}
Constraint operator>=(double lhs, const Variable& rhs) { return less_than(rhs.repn, lhs, false); }
Constraint operator>=(double lhs, const Expression& rhs) { return less_than(rhs.repn, lhs, false); }

Constraint operator>=(const Parameter& lhs, double rhs) { return less_than(rhs, lhs.repn, false); }
Constraint operator>=(const Parameter& lhs, int rhs) { return less_than(rhs, lhs.repn, false); }
Constraint operator>=(const Parameter& lhs, const Parameter& rhs)
{
    return less_than(rhs.repn, lhs.repn, false);
}
Constraint operator>=(const Parameter& lhs, const IndexParameter& rhs)
{
    return less_than(rhs.repn, lhs.repn, false);
}
Constraint operator>=(const Parameter& lhs, const Variable& rhs)
{
    return less_than(rhs.repn, lhs.repn, false);
}
Constraint operator>=(const Parameter& lhs, const Expression& rhs)
{
    return less_than(rhs.repn, lhs.repn, false);
}

Constraint operator>=(const IndexParameter& lhs, double rhs)
{
    return less_than(rhs, lhs.repn, false);
}
Constraint operator>=(const IndexParameter& lhs, int rhs)
{
    return less_than(rhs, lhs.repn, false);
}
Constraint operator>=(const IndexParameter& lhs, const Parameter& rhs)
{
    return less_than(rhs.repn, lhs.repn, false);
}
Constraint operator>=(const IndexParameter& lhs, const IndexParameter& rhs)
{
    return less_than(rhs.repn, lhs.repn, false);
}
Constraint operator>=(const IndexParameter& lhs, const Variable& rhs)
{
    return less_than(rhs.repn, lhs.repn, false);
}
Constraint operator>=(const IndexParameter& lhs, const Expression& rhs)
{
    return less_than(rhs.repn, lhs.repn, false);
}

Constraint operator>=(const Variable& lhs, double rhs) { return less_than(rhs, lhs.repn, false); }
Constraint operator>=(const Variable& lhs, int rhs) { return less_than(rhs, lhs.repn, false); }
Constraint operator>=(const Variable& lhs, const Parameter& rhs)
{
    return less_than(rhs.repn, lhs.repn, false);
}
Constraint operator>=(const Variable& lhs, const IndexParameter& rhs)
{
    return less_than(rhs.repn, lhs.repn, false);
}
Constraint operator>=(const Variable& lhs, const Variable& rhs)
{
    return less_than(rhs.repn, lhs.repn, false);
}
Constraint operator>=(const Variable& lhs, const Expression& rhs)
{
    return less_than(rhs.repn, lhs.repn, false);
}

Constraint operator>=(const Expression& lhs, double rhs) { return less_than(rhs, lhs.repn, false); }
Constraint operator>=(const Expression& lhs, int rhs) { return less_than(rhs, lhs.repn, false); }
Constraint operator>=(const Expression& lhs, const Parameter& rhs)
{
    return less_than(rhs.repn, lhs.repn, false);
}
Constraint operator>=(const Expression& lhs, const IndexParameter& rhs)
{
    return less_than(rhs.repn, lhs.repn, false);
}
Constraint operator>=(const Expression& lhs, const Variable& rhs)
{
    return less_than(rhs.repn, lhs.repn, false);
}
Constraint operator>=(const Expression& lhs, const Expression& rhs)
{
    return less_than(rhs.repn, lhs.repn, false);
}

//
// operator==
//
Constraint operator==(int lhs, const Parameter& rhs) { return equal(lhs, rhs.repn); }
Constraint operator==(int lhs, const IndexParameter& rhs) { return equal(lhs, rhs.repn); }
Constraint operator==(int lhs, const Variable& rhs) { return equal(lhs, rhs.repn); }
Constraint operator==(int lhs, const Expression& rhs) { return equal(lhs, rhs.repn); }
Constraint operator==(double lhs, const Parameter& rhs) { return equal(lhs, rhs.repn); }
Constraint operator==(double lhs, const IndexParameter& rhs) { return equal(lhs, rhs.repn); }
Constraint operator==(double lhs, const Variable& rhs) { return equal(lhs, rhs.repn); }
Constraint operator==(double lhs, const Expression& rhs) { return equal(lhs, rhs.repn); }

Constraint operator==(const Parameter& lhs, double rhs) { return equal(lhs.repn, rhs); }
Constraint operator==(const Parameter& lhs, int rhs) { return equal(lhs.repn, rhs); }
Constraint operator==(const Parameter& lhs, const Parameter& rhs)
{
    return equal(lhs.repn, rhs.repn);
}
Constraint operator==(const Parameter& lhs, const IndexParameter& rhs)
{
    return equal(lhs.repn, rhs.repn);
}
Constraint operator==(const Parameter& lhs, const Variable& rhs)
{
    return equal(lhs.repn, rhs.repn);
}
Constraint operator==(const Parameter& lhs, const Expression& rhs)
{
    return equal(lhs.repn, rhs.repn);
}

Constraint operator==(const IndexParameter& lhs, double rhs) { return equal(lhs.repn, rhs); }
Constraint operator==(const IndexParameter& lhs, int rhs) { return equal(lhs.repn, rhs); }
Constraint operator==(const IndexParameter& lhs, const Parameter& rhs)
{
    return equal(lhs.repn, rhs.repn);
}
Constraint operator==(const IndexParameter& lhs, const IndexParameter& rhs)
{
    return equal(lhs.repn, rhs.repn);
}
Constraint operator==(const IndexParameter& lhs, const Variable& rhs)
{
    return equal(lhs.repn, rhs.repn);
}
Constraint operator==(const IndexParameter& lhs, const Expression& rhs)
{
    return equal(lhs.repn, rhs.repn);
}

Constraint operator==(const Variable& lhs, double rhs) { return equal(lhs.repn, rhs); }
Constraint operator==(const Variable& lhs, int rhs) { return equal(lhs.repn, rhs); }
Constraint operator==(const Variable& lhs, const Parameter& rhs)
{
    return equal(lhs.repn, rhs.repn);
}
Constraint operator==(const Variable& lhs, const IndexParameter& rhs)
{
    return equal(lhs.repn, rhs.repn);
}
Constraint operator==(const Variable& lhs, const Variable& rhs)
{
    return equal(lhs.repn, rhs.repn);
}
Constraint operator==(const Variable& lhs, const Expression& rhs)
{
    return equal(lhs.repn, rhs.repn);
}

Constraint operator==(const Expression& lhs, double rhs) { return equal(lhs.repn, rhs); }
Constraint operator==(const Expression& lhs, int rhs) { return equal(lhs.repn, rhs); }
Constraint operator==(const Expression& lhs, const Parameter& rhs)
{
    return equal(lhs.repn, rhs.repn);
}
Constraint operator==(const Expression& lhs, const IndexParameter& rhs)
{
    return equal(lhs.repn, rhs.repn);
}
Constraint operator==(const Expression& lhs, const Variable& rhs)
{
    return equal(lhs.repn, rhs.repn);
}
Constraint operator==(const Expression& lhs, const Expression& rhs)
{
    return equal(lhs.repn, rhs.repn);
}

Constraint inequality(int lower, const Expression& body, int upper, bool strict)
{
    std::shared_ptr<ConstraintTerm> tmp
        = CREATE_POINTER(InequalityTerm, CREATE_POINTER(ConstantTerm, lower), body.repn,
                         CREATE_POINTER(ConstantTerm, upper), strict);
    return tmp;
}

Constraint inequality(double lower, const Expression& body, double upper, bool strict)
{
    std::shared_ptr<ConstraintTerm> tmp
        = CREATE_POINTER(InequalityTerm, CREATE_POINTER(ConstantTerm, lower), body.repn,
                         CREATE_POINTER(ConstantTerm, upper), strict);
    return tmp;
}

Constraint inequality(const Expression& lower, const Expression& body, const Expression& upper,
                      bool strict)
{
    std::shared_ptr<ConstraintTerm> tmp
        = CREATE_POINTER(InequalityTerm, lower.repn, body.repn, upper.repn, strict);
    return tmp;
}

}  // namespace coek
