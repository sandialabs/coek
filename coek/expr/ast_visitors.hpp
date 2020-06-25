#pragma once

#include <list>
#include <string>
#include <map>
#include <set>
#include <unordered_set>
#include "ast_term.hpp"


namespace coek {


void expr_to_list(expr_pointer_t expr, std::list<std::string>& repr);
std::list<std::string> to_list(expr_pointer_t expr);

void write_expr(expr_pointer_t expr, std::ostream& ostr);

void symbolic_diff_all(expr_pointer_t expr, std::map<VariableTerm*, expr_pointer_t>& diff);

void mutable_values(expr_pointer_t expr, std::unordered_set<VariableTerm*>& fixed_vars,
                                         std::unordered_set<ParameterTerm*>& params);

void find_vars_and_params(expr_pointer_t expr, 
                std::unordered_set<VariableTerm*>& vars,
                std::set<VariableTerm*>& fixed_vars,
                std::set<ParameterTerm*>& params
                );

void find_variables(expr_pointer_t expr, std::unordered_set<VariableTerm*>& variables);
}
