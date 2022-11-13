#pragma once

#include <map>
#include <set>
#include <unordered_set>
#include "base_terms.hpp"

namespace coek {

class VariableTerm;
class ParameterTerm;

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

void symbolic_diff_all(const expr_pointer_t& expr, std::map<VariableTerm*, expr_pointer_t>& diff);

void mutable_values(const expr_pointer_t& expr, std::unordered_set<VariableTerm*>& fixed_vars,
                    std::unordered_set<ParameterTerm*>& params);

void find_vars_and_params(const expr_pointer_t& expr,
                          std::unordered_set<std::shared_ptr<VariableTerm>>& vars,
                          std::set<std::shared_ptr<VariableTerm>>& fixed_vars,
                          std::set<std::shared_ptr<ParameterTerm>>& params);

void find_variables(const expr_pointer_t& expr,
                    std::unordered_set<std::shared_ptr<VariableTerm>>& variables);
}  // namespace coek
