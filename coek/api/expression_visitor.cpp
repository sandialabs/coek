#include "../ast/value_terms.hpp"
#include "../ast/constraint_terms.hpp"
#include "../ast/visitor_fns.hpp"
#include "expression.hpp"
#include "objective.hpp"
#include "constraint.hpp"
#include "expression_visitor.hpp"


namespace coek {

//
// QuadraticExpr
//

void to_QuadraticExpr(expr_pointer_t expr, QuadraticExpr& repn);

void QuadraticExpr::collect_terms(const Expression& expr)
{ to_QuadraticExpr(expr.repn, *this); }

void QuadraticExpr::collect_terms(const Objective& expr)
{ to_QuadraticExpr(expr.repn, *this); }

void QuadraticExpr::collect_terms(const Constraint& expr)
{ to_QuadraticExpr(expr.repn, *this); }

//
// MutableNLPExpr
//

bool MutableNLPExpr::varterm_compare::operator() (const VariableTerm* lhs, const VariableTerm* rhs) const
{ return lhs->index < rhs->index; }

void to_MutableNLPExpr(expr_pointer_t expr,
                    MutableNLPExpr& repn);

void MutableNLPExpr::collect_terms(Expression& expr)
{ to_MutableNLPExpr(expr.repn, *this); }

void MutableNLPExpr::collect_terms(Objective& expr)
{ to_MutableNLPExpr(expr.repn, *this); }

void MutableNLPExpr::collect_terms(Constraint& expr)
{ to_MutableNLPExpr(expr.repn, *this); }

}
