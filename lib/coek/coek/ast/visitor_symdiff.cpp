#include <map>
#include <set>

#include "base_terms.hpp"
#include "constraint_terms.hpp"
#include "expr_terms.hpp"
#include "value_terms.hpp"
#include "visitor.hpp"
//#include "visitor_fns.hpp"
#include "../util/cast_utils.hpp"
#if __cpp_lib_variant
#    include "compact_terms.hpp"
#endif

#include "ast_operators.hpp"

// TODO - Reconsider the coverage exclusions below.  These were added
//        because there were parts of the code that were not being
//        used, or which were not being exercised.  But in both cases these
//        were optimizations that should not impact the logical correctness
//        of the code.

//#define DEBUG_DIFF

namespace coek {

namespace {

class PartialData {
   public:
    size_t i;
    expr_pointer_t partial;
};

// -----------------------------------------------------------------------------------------

// Not executed when doing symbolic differentiation
// GCOVR_EXCL_START
void visit_ConstantTerm(const expr_pointer_t& /*expr*/, PartialData& data) {data.partial = ZEROCONST;}

void visit_ParameterTerm(const expr_pointer_t& /*expr*/, PartialData& data) {data.partial = ZEROCONST;}

void visit_IndexParameterTerm(const expr_pointer_t& /*expr*/, PartialData& /*data*/)
{
    throw std::runtime_error(
        "Cannot differentiate an expression using an abstract parameter term.");
}

void visit_VariableTerm(const expr_pointer_t& /*expr*/, PartialData& data) {data.partial = ONECONST;}

#if __cpp_lib_variant
void visit_ParameterRefTerm(const expr_pointer_t& /*expr*/, PartialData& data) {data.partial = ZEROCONST;}

//
// We assume for now that a user cannot differentiate with respect to an referenced
// variable.  It's not clear that we can infer whether two referenced variables are the same,
// given that they may be indexed by equations.
//
void visit_VariableRefTerm(const expr_pointer_t& /*expr*/, PartialData& data) {data.partial = ZEROCONST;}
#endif

void visit_IndexedVariableTerm(const expr_pointer_t& /*expr*/, PartialData& data) {data.partial = ONECONST;}

void visit_MonomialTerm(const expr_pointer_t& expr, PartialData& data)
{
auto tmp = safe_pointer_cast<MonomialTerm>(expr);
data.partial = CREATE_POINTER(ConstantTerm, tmp->coef);
}

void visit_InequalityTerm(const expr_pointer_t& /*expr*/, PartialData& /*data*/) {}

void visit_EqualityTerm(const expr_pointer_t& /*expr*/, PartialData& /*data*/) {}

void visit_ObjectiveTerm(const expr_pointer_t& /*expr*/, PartialData& /*data*/) {}
// GCOVR_EXCL_STOP

void visit_NegateTerm(const expr_pointer_t& /*expr*/, PartialData& data) {data.partial = NEGATIVEONECONST;}

void visit_PlusTerm(const expr_pointer_t& /*expr*/, PartialData& data) {data.partial = ONECONST;}

void visit_TimesTerm(const expr_pointer_t& expr, PartialData& data)
{
auto tmp = safe_pointer_cast<TimesTerm>(expr);
    if (data.i == 0)
        data.partial = tmp->rhs;
    else
        data.partial = tmp->lhs;
}

void visit_DivideTerm(const expr_pointer_t& expr, PartialData& data)
{
auto tmp = safe_pointer_cast<TimesTerm>(expr);
    if (data.i == 0)
        data.partial = divide(ONECONST, tmp->rhs);
    else
        data.partial = divide(times(NEGATIVEONECONST, tmp->lhs), times(tmp->rhs, tmp->rhs));
}

// GCOVR_EXCL_START
void visit_AbsTerm(const expr_pointer_t& /*expr*/, PartialData& /*data*/) {}

void visit_CeilTerm(const expr_pointer_t& /*expr*/, PartialData& /*data*/) {}

void visit_FloorTerm(const expr_pointer_t& /*expr*/, PartialData& /*data*/) {}
// GCOVR_EXCL_STOP

void visit_ExpTerm(const expr_pointer_t& expr, PartialData& data)
{
auto tmp = safe_pointer_cast<ExpTerm>(expr);
data.partial = CREATE_POINTER(ExpTerm, tmp->body);
//data.partial = tmp->body;
}

void visit_LogTerm(const expr_pointer_t& expr, PartialData& data) 
{
auto tmp = safe_pointer_cast<LogTerm>(expr);
data.partial = divide(ONECONST, tmp->body);
}

void visit_Log10Term(const expr_pointer_t& expr, PartialData& data) 
{
auto tmp = safe_pointer_cast<Log10Term>(expr);
    data.partial = divide(ONECONST, times(CREATE_POINTER(ConstantTerm, log(10.0)), tmp->body));
}

void visit_SqrtTerm(const expr_pointer_t& expr, PartialData& data) 
{
auto tmp = safe_pointer_cast<SqrtTerm>(expr);
    data.partial = intrinsic_pow(tmp->body, CREATE_POINTER(ConstantTerm, -0.5));
}

void visit_SinTerm(const expr_pointer_t& expr, PartialData& data) 
{
auto tmp = safe_pointer_cast<SinTerm>(expr);
data.partial = intrinsic_cos(tmp->body);
}

void visit_CosTerm(const expr_pointer_t& expr, PartialData& data) 
{
auto tmp = safe_pointer_cast<CosTerm>(expr);
    data.partial = intrinsic_sin(tmp->body);
    data.partial = data.partial->negate(data.partial);
}

void visit_TanTerm(const expr_pointer_t& expr, PartialData& data) 
{
auto tmp = safe_pointer_cast<TanTerm>(expr);
    data.partial = divide(ONECONST,
                     intrinsic_pow(intrinsic_cos(tmp->body), CREATE_POINTER(ConstantTerm, 2.0)));
}

void visit_SinhTerm(const expr_pointer_t& expr, PartialData& data) 
{
auto tmp = safe_pointer_cast<SinhTerm>(expr);
    data.partial = intrinsic_cosh(tmp->body);
}

void visit_CoshTerm(const expr_pointer_t& expr, PartialData& data) 
{
    // sinh(x)
auto tmp = safe_pointer_cast<CoshTerm>(expr);
    data.partial = intrinsic_sinh(tmp->body);
}

void visit_TanhTerm(const expr_pointer_t& expr, PartialData& data) 
{
auto tmp = safe_pointer_cast<TanhTerm>(expr);
    // 1 - tan(x)^2
    data.partial = plus(
        ONECONST, times(NEGATIVEONECONST,
                        intrinsic_pow(intrinsic_tan(tmp->body), CREATE_POINTER(ConstantTerm, 2.0))));
}

void visit_ASinTerm(const expr_pointer_t& expr, PartialData& data) 
{
auto tmp = safe_pointer_cast<ASinTerm>(expr);
    // 1/sqrt(1-x^2)
    data.partial = divide(ONECONST, intrinsic_sqrt(minus(ONECONST, times(tmp->body, tmp->body))));
}

void visit_ACosTerm(const expr_pointer_t& expr, PartialData& data) 
{
auto tmp = safe_pointer_cast<ACosTerm>(expr);
    // -1/sqrt(1-x^2)
    data.partial = CREATE_POINTER(
        NegateTerm, divide(ONECONST, intrinsic_sqrt(minus(ONECONST, times(tmp->body, tmp->body)))));
}

void visit_ATanTerm(const expr_pointer_t& expr, PartialData& data) 
{
auto tmp = safe_pointer_cast<ATanTerm>(expr);
    // 1/(1+x^2)
    data.partial = divide(ONECONST, plus(ONECONST, times(tmp->body, tmp->body)));
}

void visit_ASinhTerm(const expr_pointer_t& expr, PartialData& data) 
{
auto tmp = safe_pointer_cast<ASinhTerm>(expr);
    // 1/sqrt(1+x^2)
    data.partial = divide(ONECONST, intrinsic_sqrt(plus(ONECONST, times(tmp->body, tmp->body))));
}

void visit_ACoshTerm(const expr_pointer_t& expr, PartialData& data) 
{
auto tmp = safe_pointer_cast<ACoshTerm>(expr);
    // 1/sqrt(x^2-1)
    data.partial = divide(ONECONST, intrinsic_sqrt(minus(times(tmp->body, tmp->body), ONECONST)));
}

void visit_ATanhTerm(const expr_pointer_t& expr, PartialData& data) 
{
auto tmp = safe_pointer_cast<ATanhTerm>(expr);
    // 1/(1-x^2)
    data.partial = divide(ONECONST, minus(times(tmp->body, tmp->body), ONECONST));
}

void visit_PowTerm(const expr_pointer_t& expr, PartialData& data) 
{
auto tmp = safe_pointer_cast<PowTerm>(expr);
    // x^y
    expr_pointer_t base = tmp->lhs;
    expr_pointer_t exp = tmp->rhs;
    if (data.i == 0)
        // y x^(y-1)
        data.partial = times(exp, intrinsic_pow(base, plus(exp, NEGATIVEONECONST)));
    else
        // log(y) x^y
        data.partial = times(intrinsic_log(base), expr);
}

/*
// d curr / d child_i
expr_pointer_t compute_partial(const expr_pointer_t& curr, size_t i, PartialData& visitor)
{
    visitor.i = i;
    curr->accept(visitor);
    return visitor.partial;
}
*/

#define VISIT_CASE(TERM)          \
    case TERM##_id:               \
        visit_##TERM(expr, data); \
        break

expr_pointer_t compute_partial(const expr_pointer_t& expr, size_t i, PartialData& data)
{
    data.i = i;

    switch (expr->id()) {
        VISIT_CASE(ConstantTerm);
        VISIT_CASE(ParameterTerm);
        VISIT_CASE(IndexParameterTerm);
        VISIT_CASE(VariableTerm);
        VISIT_CASE(IndexedVariableTerm);
#ifdef COEK_WITH_COMPACT_MODEL
        VISIT_CASE(VariableRefTerm);
        VISIT_CASE(ParameterRefTerm);
#endif
        VISIT_CASE(MonomialTerm);
        VISIT_CASE(InequalityTerm);
        VISIT_CASE(EqualityTerm);
        VISIT_CASE(ObjectiveTerm);
        VISIT_CASE(NegateTerm);
        VISIT_CASE(PlusTerm);
        VISIT_CASE(TimesTerm);
        VISIT_CASE(DivideTerm);
        VISIT_CASE(AbsTerm);
        VISIT_CASE(CeilTerm);
        VISIT_CASE(FloorTerm);
        VISIT_CASE(ExpTerm);
        VISIT_CASE(LogTerm);
        VISIT_CASE(Log10Term);
        VISIT_CASE(SqrtTerm);
        VISIT_CASE(SinTerm);
        VISIT_CASE(CosTerm);
        VISIT_CASE(TanTerm);
        VISIT_CASE(SinhTerm);
        VISIT_CASE(CoshTerm);
        VISIT_CASE(TanhTerm);
        VISIT_CASE(ASinTerm);
        VISIT_CASE(ACosTerm);
        VISIT_CASE(ATanTerm);
        VISIT_CASE(ASinhTerm);
        VISIT_CASE(ACoshTerm);
        VISIT_CASE(ATanhTerm);
        VISIT_CASE(PowTerm);

        // GCOVR_EXCL_START
        default:
        throw std::runtime_error(
        "Error in find_variables visitor!  Visiting unexpected expression term "
        + std::to_string(expr->id()));
        // GCOVR_EXCL_STOP
        };

    return data.partial;
}
        
bool variable_comparator(const std::shared_ptr<VariableTerm>& lhs,
                         const std::shared_ptr<VariableTerm>& rhs)
{
    return lhs->index < rhs->index;
}

typedef std::set<std::shared_ptr<VariableTerm>, bool (*)(const std::shared_ptr<VariableTerm>&,
                                                         const std::shared_ptr<VariableTerm>&)>
    ordered_variableset_t;
typedef ordered_variableset_t::iterator ordered_variableset_iterator_t;

}  // namespace

void symbolic_diff_all(const expr_pointer_t& root, std::map<std::shared_ptr<VariableTerm>, expr_pointer_t>& diff)
{
    //
    // Default is zero, if the variable does not exist in this expression
    //
    if (root->is_constant() or root->is_parameter())
        return;

    else if (root->is_variable()) {
        auto tmp = std::dynamic_pointer_cast<VariableTerm>(root);
        if (!tmp->fixed) diff[tmp] = ONECONST;
        return;
    }

    else if (root->is_monomial()) {
        auto tmp = std::dynamic_pointer_cast<MonomialTerm>(root);
        if (!tmp->var->fixed) diff[tmp->var] = CREATE_POINTER(ConstantTerm, tmp->coef);
        return;
    }

    //
    // Use a topological sort
    //

    ordered_variableset_t variables(variable_comparator);

    //
    // Compute in-degree
    //
    std::map<expr_pointer_t, int> D;
    D[root] = 0;
    {
        std::list<ExpressionTerm*> queue;
        auto tmp = std::dynamic_pointer_cast<ExpressionTerm>(root);
        queue.push_back(tmp.get());
        while (queue.size() > 0) {
            ExpressionTerm* curr = queue.back();
            queue.pop_back();
            for (unsigned int i = 0; i < curr->num_expressions(); i++) {
                expr_pointer_t child = curr->expression(i);
                if (D.find(child) == D.end())
                    D[child] = 1;
                else
                    D[child] += 1;
                if (child->is_expression()) {
                    auto tmp = std::dynamic_pointer_cast<ExpressionTerm>(child);
                    queue.push_back(tmp.get());
                }
                else if (child->is_variable()) {
                    auto tmp = std::dynamic_pointer_cast<VariableTerm>(child);
                    variables.insert(tmp);
                }
                else if (child->is_monomial()) {
                    auto tmp = std::dynamic_pointer_cast<MonomialTerm>(child);
                    variables.insert(tmp->var);
                }
            }
        }
    }

    //
    // Process nodes, and add them to the queue when
    // they have been reached by all parents.
    //
    std::list<std::shared_ptr<ExpressionTerm>> queue2;
    PartialData visitor;
    std::map<expr_pointer_t, expr_pointer_t> partial;
    partial[root] = ONECONST;
    auto ttmp = std::dynamic_pointer_cast<ExpressionTerm>(root);
    queue2.push_back(ttmp);

    while (queue2.size() > 0) {
        //
        // Get the front of the queue2
        //
        auto curr = queue2.front();
        queue2.pop_front();

#ifdef DEBUG_DIFF
        std::cout << "CURR " << curr << " ";
        write_expr(curr, std::cout);
        std::cout << std::endl;
#endif
        //
        // Iterate over children.  Create partial and add them to the
        // queue2
        //
        for (size_t i = 0; i < curr->num_expressions(); i++) {
            expr_pointer_t _partial = compute_partial(curr, i, visitor);
            expr_pointer_t child = curr->expression(i);
            if (false) {
                partial[child] = ZEROCONST;
            }
            else {
#ifdef DEBUG_DIFF
                std::cout << "i " << i << "  ";
                std::cout << std::flush;

                write_expr(curr, std::cout);
                std::cout << std::endl;
                std::cout << child->is_expression() << " " << child->is_variable() << " "
                          << child->is_monomial() << " " << child << " D=" << D[child] << std::endl;
                std::cout << std::flush;
#endif

                D[child]--;
                if (D[child] == 0) {
#ifdef DEBUG_DIFF
                    std::cout << "PUSH " << child->is_expression() << std::endl;
#endif
                    if (child->is_expression()) {
                        auto tmp = std::dynamic_pointer_cast<ExpressionTerm>(child);
                        queue2.push_back(tmp);
                    }
                }
#ifdef DEBUG_DIFF
                std::cout << "HERE" << std::endl << std::flush;
#endif
                if (partial.find(child) == partial.end())
                    partial[child] = times_(partial[curr], _partial);
                else
                    partial[child] = plus_(partial[child], times_(partial[curr], _partial));
                //
                // A monomial object contains a variable, but its a leaf.  Hence, we need
                // to explicitly insert the partial[] value.
                //
                if (child->is_monomial()) {
                    auto tmp = std::dynamic_pointer_cast<MonomialTerm>(child);
                    bool varflag = (partial.find(tmp->var) == partial.end());
                    // if (partial[child] == ZEROCONST)
                    //     partial[tmp->var] = ZEROCONST;
                    // else if (tmp->coef == 1)
                    if (tmp->coef == 1) {
                        if (varflag)
                            partial[tmp->var] = partial[child];
                        else
                            partial[tmp->var] = plus_(partial[tmp->var], partial[child]);
                    }
                    else if (partial[child]->is_constant()) {
                        auto _rhs = std::dynamic_pointer_cast<ConstantTerm>(partial[child]);
                        if (varflag)
                            partial[tmp->var]
                                = CREATE_POINTER(ConstantTerm, tmp->coef * _rhs->value);
                        else
                            partial[tmp->var]
                                = plus_(partial[tmp->var],
                                        CREATE_POINTER(ConstantTerm, tmp->coef * _rhs->value));
                    }
                    else {
                        if (varflag)
                            partial[tmp->var]
                                = times_(CREATE_POINTER(ConstantTerm, tmp->coef), partial[child]);
                        else
                            partial[tmp->var] = plus_(
                                partial[tmp->var],
                                times_(CREATE_POINTER(ConstantTerm, tmp->coef), partial[child]));
                    }
                }

#ifdef DEBUG_DIFF
                std::cout << "PARTIAL" << std::endl << std::flush;
                write_expr(child, std::cout);
                std::cout << " :  ";
                write_expr(partial[child], std::cout);
                std::cout << " :  ";
                write_expr(_partial, std::cout);
                std::cout << std::endl;
                std::cout << std::endl;
#endif
            }
        }
    }

    for (ordered_variableset_iterator_t it = variables.begin(); it != variables.end(); it++) {
        diff[*it] = partial[*it];

#ifdef DEBUG_DIFF
        write_expr(*it, std::cout);
        std::cout << " :  ";
        write_expr(diff[*it], std::cout);
        std::cout << std::endl;
#endif
    }
}

}  // namespace coek
