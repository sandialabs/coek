#pragma once

#include "../ast/constraint_terms.hpp"

namespace coek {

expr_pointer_t convert_expr_template(const expr_pointer_t& expr);

std::shared_ptr<ConstraintTerm> convert_con_template(const std::shared_ptr<ConstraintTerm>& expr);

}  // namespace coek
