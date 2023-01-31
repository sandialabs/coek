#pragma once

#include <map>
#include <set>
#include <unordered_set>
#include "base_terms.hpp"

namespace coek {

class VariableTerm;
class ParameterTerm;
class SubExpressionTerm;

void expr_to_list(BaseExpressionTerm* expr, std::list<std::string>& repr);
inline void expr_to_list(const expr_pointer_t& expr, std::list<std::string>& repr)
{
    expr_to_list(expr.get(), repr);
}

void write_expr(BaseExpressionTerm* expr, std::ostream& ostr);
inline void write_expr(const expr_pointer_t& expr, std::ostream& ostr)
{
    write_expr(expr.get(), ostr);
}

void symbolic_diff_all(const expr_pointer_t& expr,
                       std::map<std::shared_ptr<VariableTerm>, expr_pointer_t>& diff);

#ifdef DEBUG
double evaluate_expr_debug(const expr_pointer_t& expr,
                           std::map<std::shared_ptr<SubExpressionTerm>, double>& subexpr_value,
                           size_t& num_visits);
#endif
double evaluate_expr(const expr_pointer_t& expr,
                     std::map<std::shared_ptr<SubExpressionTerm>, double>& subexpr_value);
double evaluate_expr(const BaseExpressionTerm* expr,
                     std::map<std::shared_ptr<SubExpressionTerm>, double>& subexpr_value);

#ifdef DEBUG
void mutable_values_debug(
    const expr_pointer_t& expr, std::unordered_set<std::shared_ptr<VariableTerm>>& fixed_vars,
    std::unordered_set<std::shared_ptr<ParameterTerm>>& params,
    std::unordered_set<std::shared_ptr<SubExpressionTerm>>& visited_subexpressions,
    size_t& num_visits);
#endif
void mutable_values(const expr_pointer_t& expr,
                    std::unordered_set<std::shared_ptr<VariableTerm>>& fixed_vars,
                    std::unordered_set<std::shared_ptr<ParameterTerm>>& params,
                    std::unordered_set<std::shared_ptr<SubExpressionTerm>>& visited_subexpressions);

#ifdef DEBUG
void find_vars_and_params_debug(
    const expr_pointer_t& expr, std::unordered_set<std::shared_ptr<VariableTerm>>& vars,
    std::set<std::shared_ptr<VariableTerm>>& fixed_vars,
    std::set<std::shared_ptr<ParameterTerm>>& params,
    std::set<std::shared_ptr<SubExpressionTerm>>& visited_subexpressions, size_t& num_visits);
#endif
void find_vars_and_params(const expr_pointer_t& expr,
                          std::unordered_set<std::shared_ptr<VariableTerm>>& vars,
                          std::set<std::shared_ptr<VariableTerm>>& fixed_vars,
                          std::set<std::shared_ptr<ParameterTerm>>& params,
                          std::set<std::shared_ptr<SubExpressionTerm>>& visited_subexpressions);
void find_variables(const expr_pointer_t& expr,
                    std::unordered_set<std::shared_ptr<VariableTerm>>& variables);

expr_pointer_t simplify_expr(
    const expr_pointer_t& expr,
    std::map<std::shared_ptr<SubExpressionTerm>, expr_pointer_t>& subexpr_value);
expr_pointer_t simplify_expr(const expr_pointer_t& expr);
}  // namespace coek
