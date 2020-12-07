#include <map>
#include <set>

#include "visitor_fns.hpp"
#include "visitor.hpp"
#include "base_terms.hpp"
#include "constraint_terms.hpp"
#include "objective_terms.hpp"
#include "expr_terms.hpp"
#include "value_terms.hpp"
#include "compact_terms.hpp"

#include "ast_operators.hpp"

// TODO - Reconsider the coverage exclusions below.  These were added
//        because there were parts of the code that were not being
//        used, or which were not being exercised.  But in both cases these
//        were optimizations that should not impact the logical correctness
//        of the code.

//#define DEBUG_DIFF

namespace coek {

namespace {

class PartialVisitor : public Visitor
{
public:

    size_t i;
    expr_pointer_t partial;

public:

    PartialVisitor() : partial(0) {}

    void visit(ConstantTerm& arg);
    void visit(ParameterTerm& arg);
    void visit(IndexParameterTerm& arg);
    void visit(VariableTerm& arg);
    void visit(VariableRefTerm& arg);
    void visit(IndexedVariableTerm& arg);
    void visit(MonomialTerm& arg);
    void visit(InequalityTerm& arg);
    void visit(EqualityTerm& arg);
    void visit(ObjectiveTerm& arg);
    void visit(NegateTerm& arg);
    void visit(PlusTerm& arg);
    void visit(TimesTerm& arg);
    void visit(DivideTerm& arg);
    void visit(AbsTerm& arg);
    void visit(CeilTerm& arg);
    void visit(FloorTerm& arg);
    void visit(ExpTerm& arg);
    void visit(LogTerm& arg);
    void visit(Log10Term& arg);
    void visit(SqrtTerm& arg);
    void visit(SinTerm& arg);
    void visit(CosTerm& arg);
    void visit(TanTerm& arg);
    void visit(SinhTerm& arg);
    void visit(CoshTerm& arg);
    void visit(TanhTerm& arg);
    void visit(ASinTerm& arg);
    void visit(ACosTerm& arg);
    void visit(ATanTerm& arg);
    void visit(ASinhTerm& arg);
    void visit(ACoshTerm& arg);
    void visit(ATanhTerm& arg);
    void visit(PowTerm& arg);
};


// Not executed when doing symbolic differentiation
// GCOVR_EXCL_START
void PartialVisitor::visit(ConstantTerm& )
{ partial = ZEROCONST; }

void PartialVisitor::visit(ParameterTerm& )
{ partial = ZEROCONST; }

void PartialVisitor::visit(IndexParameterTerm& )
{ throw std::runtime_error("Cannot differentiate an expression using an abstract parameter term."); }

void PartialVisitor::visit(VariableTerm& )
{ partial = ONECONST; }

void PartialVisitor::visit(VariableRefTerm& )
{
//
// We assume for now that a user cannot differentiate with respect to an referenced
// variable.  It's not clear that we can infer whether two referenced variables are the same,
// given that they may be indexed by equations.
//
partial = ZEROCONST;
}

void PartialVisitor::visit(IndexedVariableTerm& )
{ partial = ONECONST; }

void PartialVisitor::visit(MonomialTerm& arg)
{ partial = CREATE_POINTER(ConstantTerm, arg.coef); }

void PartialVisitor::visit(InequalityTerm& )
{ partial = 0; }

void PartialVisitor::visit(EqualityTerm& )
{ partial = 0; }

void PartialVisitor::visit(ObjectiveTerm& )
{ partial = 0; }
// GCOVR_EXCL_STOP

void PartialVisitor::visit(NegateTerm& )
{ partial = NEGATIVEONECONST; }

void PartialVisitor::visit(PlusTerm& )
{ partial = ONECONST; }

void PartialVisitor::visit(TimesTerm& arg)
{
if (i == 0)
    partial = arg.rhs;
else
    partial = arg.lhs;
}

void PartialVisitor::visit(DivideTerm& arg)
{
if (i == 0)
    partial = divide(ONECONST, arg.rhs);
else 
    partial = divide(times(NEGATIVEONECONST, arg.lhs), times(arg.rhs, arg.rhs));
}

// GCOVR_EXCL_START
void PartialVisitor::visit(AbsTerm& )
{ partial = 0; }

void PartialVisitor::visit(CeilTerm& )
{ partial = 0; }

void PartialVisitor::visit(FloorTerm& )
{ partial = 0; }
// GCOVR_EXCL_STOP

void PartialVisitor::visit(ExpTerm& arg)
{
partial = &arg; 
}

void PartialVisitor::visit(LogTerm& arg)
{
partial = divide(ONECONST, arg.body); 
}

void PartialVisitor::visit(Log10Term& arg)
{
partial = divide(ONECONST, times(CREATE_POINTER(ConstantTerm, log(10.0)), arg.body) ); 
}

void PartialVisitor::visit(SqrtTerm& arg)
{
partial = intrinsic_pow(arg.body, CREATE_POINTER(ConstantTerm,-0.5));
}

void PartialVisitor::visit(SinTerm& arg)
{
partial = intrinsic_cos(arg.body);
}

void PartialVisitor::visit(CosTerm& arg)
{
partial = intrinsic_sin(arg.body);
partial = partial->negate(partial);
}

void PartialVisitor::visit(TanTerm& arg)
{
partial = divide(ONECONST, intrinsic_pow(intrinsic_cos(arg.body), CREATE_POINTER(ConstantTerm, 2.0)));
}

void PartialVisitor::visit(SinhTerm& arg)
{
// cosh(x)
partial = intrinsic_cosh(arg.body);
}

void PartialVisitor::visit(CoshTerm& arg)
{
// sinh(x)
partial = intrinsic_sinh(arg.body);
}

void PartialVisitor::visit(TanhTerm& arg)
{
// 1 - tan(x)^2
partial = plus(ONECONST, times(NEGATIVEONECONST, 
                intrinsic_pow(intrinsic_tan(arg.body), CREATE_POINTER(ConstantTerm, 2.0))));
}

void PartialVisitor::visit(ASinTerm& arg)
{
// 1/sqrt(1-x^2)
partial = divide(ONECONST, intrinsic_sqrt(minus(ONECONST, times(arg.body, arg.body))));
}

void PartialVisitor::visit(ACosTerm& arg)
{
// -1/sqrt(1-x^2)
partial = CREATE_POINTER(NegateTerm, divide(ONECONST, intrinsic_sqrt(minus(ONECONST, times(arg.body, arg.body)))));
}

void PartialVisitor::visit(ATanTerm& arg)
{
// 1/(1+x^2)
partial = divide(ONECONST, plus(ONECONST, times(arg.body, arg.body)));
}

void PartialVisitor::visit(ASinhTerm& arg)
{
// 1/sqrt(1+x^2)
partial = divide(ONECONST, intrinsic_sqrt(plus(ONECONST, times(arg.body, arg.body))));
}

void PartialVisitor::visit(ACoshTerm& arg)
{
// 1/sqrt(x^2-1)
partial = divide(ONECONST, intrinsic_sqrt(minus(times(arg.body, arg.body), ONECONST)));
}

void PartialVisitor::visit(ATanhTerm& arg)
{
// 1/(1-x^2)
partial = divide(ONECONST, minus(times(arg.body, arg.body), ONECONST));
}

void PartialVisitor::visit(PowTerm& arg)
{
// x^y
expr_pointer_t base = arg.lhs;
expr_pointer_t exp = arg.rhs;
if (i==0)
    // y x^(y-1) 
    partial = times(exp, intrinsic_pow(base, plus(exp, NEGATIVEONECONST)));
else
    // log(y) x^y 
    partial = times(intrinsic_log(base), &arg);
}

// d curr / d child_i
expr_pointer_t compute_partial(expr_pointer_t curr, size_t i, PartialVisitor& visitor)
{
visitor.i = i;
curr->accept(visitor);
return visitor.partial;
}

bool variable_comparator(const VariableTerm* lhs, const VariableTerm* rhs)
{ return lhs->index < rhs->index; }


typedef std::set<VariableTerm*, bool(*)(const VariableTerm*, const VariableTerm*)> ordered_variableset_t;
typedef ordered_variableset_t::iterator ordered_variableset_iterator_t;

#if 0
expr_pointer_t times_(expr_pointer_t lhs, expr_pointer_t rhs)
{
// GCOVR_EXCL_START
if (lhs == ZEROCONST)
    return ZEROCONST;
if (lhs == ONECONST)
    return rhs;
if (rhs == ZEROCONST)
    return ZEROCONST;
if (rhs == ONECONST)
    return lhs;
if (lhs->is_constant() and rhs->is_constant()) {
    auto _lhs = dynamic_cast<ConstantTerm*>(lhs);
    auto _rhs = dynamic_cast<ConstantTerm*>(rhs);
    auto ans = CREATE_POINTER(ConstantTerm, _lhs->value * _rhs->value);
    DISCARD_POINTER(_lhs);
    DISCARD_POINTER(_rhs);
    return ans;
    }
// GCOVR_EXCL_STOP
return times(lhs, rhs);
}

expr_pointer_t plus_(expr_pointer_t lhs, expr_pointer_t rhs)
{
// GCOVR_EXCL_START
if (lhs == ZEROCONST) 
    return rhs;
if (rhs == ZEROCONST)
    return lhs;
if (lhs->is_constant() and rhs->is_constant()) {
    auto _lhs = dynamic_cast<ConstantTerm*>(lhs);
    auto _rhs = dynamic_cast<ConstantTerm*>(rhs);
    auto ans = CREATE_POINTER(ConstantTerm, _lhs->value + _rhs->value);
    DISCARD_POINTER(_lhs);
    DISCARD_POINTER(_rhs);
    return ans;
    }
// GCOVR_EXCL_STOP
return plus(lhs, rhs);
}
#endif

}



void symbolic_diff_all(expr_pointer_t root, std::map<VariableTerm*, expr_pointer_t>& diff)
{
//
// Default is zero, if the variable does not exist in this expression
//
if (root->is_parameter())
    return;

else if (root->is_variable()) {
    VariableTerm* tmp = dynamic_cast<VariableTerm*>(root);
    if (! tmp->fixed)
        diff[tmp] = ONECONST;
    return;
    }

else if (root->is_monomial()) {
    MonomialTerm* tmp = dynamic_cast<MonomialTerm*>(root);
    if (! tmp->var->fixed)
        diff[tmp->var] = CREATE_POINTER(ConstantTerm, tmp->coef);
    return;
    }

//
// Use a topological sort
//

ordered_variableset_t variables(variable_comparator);

//
// Compute in-degree
//
std::map<expr_pointer_t,int> D;
std::list<ExpressionTerm*> queue;
D[root] = 0;
queue.push_back(dynamic_cast<ExpressionTerm*>(root));
while(queue.size() > 0) {
    ExpressionTerm* curr = queue.back();
    queue.pop_back();
    for (unsigned int i=0; i<curr->num_expressions(); i++) {
        expr_pointer_t child = curr->expression(i);
        if (D.find(child) == D.end())
            D[child] = 1;
        else
            D[child] += 1;
        if (child->is_expression())
            queue.push_back(dynamic_cast<ExpressionTerm*>(child));
        else if (child->is_variable())
            variables.insert(dynamic_cast<VariableTerm*>(child));
        else if (child->is_monomial())
            variables.insert(dynamic_cast<MonomialTerm*>(child)->var);
        }
    }

//
// Process nodes, and add them to the queue when 
// they have been reached by all parents.
//
PartialVisitor visitor;
std::map<expr_pointer_t, expr_pointer_t> partial;
partial[root] = ONECONST;
queue.push_back(dynamic_cast<ExpressionTerm*>(root));

while (queue.size() > 0) {
    #ifdef DEBUG_DIFF
    std::cout << "TODO " << queue.size() << std::endl;
    #endif
    //
    // Get the front of the queue
    //
    ExpressionTerm* curr = queue.front();
    queue.pop_front();

    #ifdef DEBUG_DIFF
    std::cout << "CURR " << curr << " ";
    write_expr(curr, std::cout);
    std::cout << std::endl;
    #endif
    //
    // Iterate over children.  Create partial and add them to the 
    // queue
    //
    for (size_t i=0; i<curr->num_expressions(); i++) {
        expr_pointer_t _partial = compute_partial(curr, i, visitor);
        expr_pointer_t child = curr->expression(i);
        //if (child->is_constant()) {
        if (false) {
            partial[child] = ZEROCONST;
            }
        else {
            #ifdef DEBUG_DIFF
            std::cout << "i " << i << "  ";
            std::cout << std::flush;

            write_expr(curr, std::cout);
            std::cout << std::endl;
            std::cout << child->is_expression() << " " << child->is_variable() << " " << child->is_monomial() << " " << child << " D=" << D[child] << std::endl;
            std::cout << std::flush;
            #endif
  
            D[child]--;
            if (D[child] == 0) {
                #ifdef DEBUG_DIFF
                std::cout << "PUSH " << child->is_expression() << std::endl;
                #endif
                if (child->is_expression())
                    queue.push_back(dynamic_cast<ExpressionTerm*>(child));
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
                MonomialTerm* tmp = dynamic_cast<MonomialTerm*>(child);
                bool varflag = (partial.find(tmp->var) == partial.end());
                //if (partial[child] == ZEROCONST)
                //    partial[tmp->var] = ZEROCONST;
                //else if (tmp->coef == 1)
                if (tmp->coef == 1) {
                    if (varflag)
                        partial[tmp->var] = partial[child];
                    else
                        partial[tmp->var] = plus_(partial[tmp->var], partial[child]);
                    }
                else if (partial[child]->is_constant()) {
                    auto _rhs = dynamic_cast<ConstantTerm*>(partial[child]);
                    if (varflag)
                        partial[tmp->var] = CREATE_POINTER(ConstantTerm, tmp->coef * _rhs->value);
                    else
                        partial[tmp->var] = plus_(partial[tmp->var], CREATE_POINTER(ConstantTerm, tmp->coef * _rhs->value));
                    }
                else {
                    if (varflag)
                        partial[tmp->var] = times_(CREATE_POINTER(ConstantTerm,tmp->coef), partial[child]);
                    else
                        partial[tmp->var] = plus_(partial[tmp->var], times_(CREATE_POINTER(ConstantTerm,tmp->coef), partial[child]));
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

for (ordered_variableset_iterator_t it=variables.begin(); it != variables.end(); it++) {
    diff[*it] = partial[*it];

    #ifdef DEBUG_DIFF
    write_expr(*it, std::cout);
    std::cout << " :  ";
    write_expr(diff[*it], std::cout);
    std::cout << std::endl;
    #endif
    }
//
// Discard all partial expressions that aren't w.r.t. a variable
//
for (std::map<expr_pointer_t, expr_pointer_t>::iterator it= partial.begin(); it != partial.end(); ++it) {
    if (! it->first->is_variable()) {
        DISCARD_POINTER(it->second);
        }
#if 0
    WEH - This should not be called.
    else {
        auto tmp = dynamic_cast<VariableTerm*>(it->first);
        if (variables.find(tmp) != variables.end())
            DISCARD_POINTER(it->second);
        }
#endif
    }
}

}
