#include "base_terms.hpp"

#include "constraint_terms.hpp"
#include "expr_terms.hpp"

namespace coek {

void write_expr(expr_pointer_t, std::ostream&);

// GCOVR_EXCL_START
#ifdef WITH_AST_ENV
bool ASTEnvironment::debug = true;

ASTEnvironment env;

ASTEnvironment::~ASTEnvironment()
{
    if (debug) {
        std::cout << "Memory Check: ";
        if (undeleted_memory()) {
            std::cout << "ERROR" << std::endl;
            write(std::cout);
            std::cout << std::endl << std::flush;
        }
        else
            std::cout << "OK" << std::endl;
    }
}

bool ASTEnvironment::undeleted_memory()
{
    return (data.size() > num_global_constants);
#    if 0
unsigned int count=0;
for (cache_t::iterator it=data.begin(); it != data.end(); ++it)
    count += it->second;
return (count > num_global_constants);
#    endif
}

void ASTEnvironment::write(std::ostream& ostr)
{
    for (cache_t::iterator it = data.begin(); it != data.end(); ++it) {
        ostr << "  " << it->second << " : ";
        write_expr(it->first, ostr);
        ostr << std::endl;
    }
    ostr << std::flush;
}

bool ASTEnvironment::check_memory()
{
    bool flag = undeleted_memory();
    if (!flag) return true;

    std::cout << "ERROR: Detected Undeleted Memory" << std::endl;
    write(std::cout);
    std::cout << std::endl << std::flush;

    reset();
    return false;
}

void ASTEnvironment::reset()
{
    data.clear();
    cache(&OneConstant, 1);
    cache(&ZeroConstant, 1);
    cache(&NegativeOneConstant, 1);
    cache(&EmptyConstraint, 1);
}
#else

std::shared_ptr<ConstantTerm> ZeroConstant = std::make_shared<ConstantTerm>(0);
std::shared_ptr<ConstantTerm> OneConstant = std::make_shared<ConstantTerm>(1);
std::shared_ptr<ConstantTerm> NegativeOneConstant = std::make_shared<ConstantTerm>(-1);
#endif
// GCOVR_EXCL_STOP

expr_pointer_t BaseExpressionTerm::const_mult(double coef, const expr_pointer_t& repn)
{
    expr_pointer_t lhs = CREATE_POINTER(ConstantTerm, coef);
    return CREATE_POINTER(TimesTerm, lhs, repn);
}

expr_pointer_t BaseExpressionTerm::negate(const expr_pointer_t& repn)
{
    // SHARED_PTR
    // return std::static_pointer_cast<BaseExpressionTerm>( std::make_shared<NegateTerm>(repn) );
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
    // SHARED_PTR
    // return std::static_pointer_cast<BaseExpressionTerm>( std::make_shared<ConstantTerm>(-1*value)
    // );
    return CREATE_POINTER(ConstantTerm, -1 * value);
}

}  // namespace coek
