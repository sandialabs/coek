#include "base_terms.hpp"
#include "expr_terms.hpp"

namespace coek {

std::shared_ptr<ConstantTerm> ZeroConstant = std::make_shared<ConstantTerm>(0);
std::shared_ptr<ConstantTerm> OneConstant = std::make_shared<ConstantTerm>(1);
std::shared_ptr<ConstantTerm> NegativeOneConstant = std::make_shared<ConstantTerm>(-1);

expr_pointer_t BaseExpressionTerm::const_mult(double coef, const expr_pointer_t& repn)
{
    CREATE_POINTER(TimesTerm, CREATE_POINTER(ConstantTerm, coef), repn);
}

expr_pointer_t BaseExpressionTerm::negate(const expr_pointer_t& repn)
{
    return CREATE_POINTER(NegateTerm, repn);
}

void expr_to_list(BaseExpressionTerm*, std::list<std::string>&);

std::list<std::string> BaseExpressionTerm::to_list()
{
    std::list<std::string> tmp;
    expr_to_list(this, tmp);
    return tmp;
}

//
// ConstantTerm
//

expr_pointer_t ConstantTerm::negate(const expr_pointer_t&)
{
    return CREATE_POINTER(ConstantTerm, -1 * value);
}

}  // namespace coek
