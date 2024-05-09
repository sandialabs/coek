#include "../ast/base_terms.hpp"
#include "../ast/constraint_terms.hpp"
#include "../ast/value_terms.hpp"
#include "expression.hpp"
#include "constraint.hpp"
#include "objective.hpp"
#include "expression_visitor.hpp"

namespace coek {

void to_QuadraticExpr(const expr_pointer_t& expr, QuadraticExpr& repn);
void to_MutableNLPExpr(const expr_pointer_t& expr, MutableNLPExpr& repn);

//
// QuadraticExpr
//

void QuadraticExpr::reset()
{
    constval = 0;
    linear_vars.resize(0);
    linear_coefs.resize(0);
    quadratic_lvars.resize(0);
    quadratic_rvars.resize(0);
    quadratic_coefs.resize(0);
}

void QuadraticExpr::collect_terms(const Expression& expr) { to_QuadraticExpr(expr.repn, *this); }

void QuadraticExpr::collect_terms(const Objective& expr) { to_QuadraticExpr(expr.repn, *this); }

void QuadraticExpr::collect_terms(const Constraint& expr) { to_QuadraticExpr(expr.repn, *this); }

//
// MutableNLPExpr
//

bool MutableNLPExpr::varterm_compare::operator()(const VariableRepn& lhs,
                                                 const VariableRepn& rhs) const
{
    return lhs->index < rhs->index;
}

void MutableNLPExpr::collect_terms(Expression& expr) { to_MutableNLPExpr(expr.repn, *this); }

void MutableNLPExpr::collect_terms(Objective& expr) { to_MutableNLPExpr(expr.repn, *this); }

void MutableNLPExpr::collect_terms(Constraint& expr) { to_MutableNLPExpr(expr.repn, *this); }

}  // namespace coek
