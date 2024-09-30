#include "visitor_fns.hpp"

namespace coek {

#ifdef COEK_WITH_COMPACT_MODEL
expr_pointer_t convert_expr_template(const expr_pointer_t& expr);
#endif

expr_pointer_t expand_expr(const expr_pointer_t& repn)
{
#ifdef COEK_WITH_COMPACT_MODEL
    return convert_expr_template(repn);
#else
    return repn;
#endif
}

} // namespace coek
