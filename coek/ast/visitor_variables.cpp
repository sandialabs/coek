#include "visitor_fns.hpp"
#include "visitor.hpp"
#include "base_terms.hpp"
#include "constraint_terms.hpp"
#include "expr_terms.hpp"
#include "value_terms.hpp"
#ifdef COEK_WITH_COMPACT_MODEL
#include "compact_terms.hpp"
#endif


namespace coek {

namespace {

class VariablesVisitor : public Visitor
{
public:

    std::unordered_set<VariableTerm*>& vars;
    std::set<VariableTerm*>& fixed_vars;
    std::set<ParameterTerm*>& params;

public:

    VariablesVisitor(std::unordered_set<VariableTerm*>& _vars,
                    std::set<VariableTerm*>& _fixed_vars,
                    std::set<ParameterTerm*>& _params)
        : vars(_vars), fixed_vars(_fixed_vars), params(_params) {}

    void visit(ConstantTerm& )
        {}

    void visit(ParameterTerm& arg)
        { params.insert(&arg); }

    void visit(IndexParameterTerm& )
        {}

    void visit(VariableTerm& arg)
        {
        if (arg.fixed)  fixed_vars.insert(&arg);
        else            vars.insert(&arg);
        }

#ifdef COEK_WITH_COMPACT_MODEL
    void visit(VariableRefTerm& )
        {
        throw std::runtime_error("Attempting to find variables in an abstract expression!");
        }
#endif

    void visit(IndexedVariableTerm& arg)
        {
        if (arg.fixed)  fixed_vars.insert(&arg);
        else            vars.insert(&arg);
        }

    void visit(MonomialTerm& arg)
        {
        if (arg.var->fixed) fixed_vars.insert(arg.var);
        else                vars.insert(arg.var);
        }

    void visit(InequalityTerm& arg)
        {arg.body->accept(*this);}

    void visit(EqualityTerm& arg)
        {arg.body->accept(*this);}

    void visit(ObjectiveTerm& arg)
        {arg.body->accept(*this);}

    void visit(NegateTerm& arg)
        {arg.body->accept(*this);}

    void visit(PlusTerm& arg)
        {
        std::vector<expr_pointer_t>& vec = *(arg.data);
        for (auto it=vec.begin(); it != vec.end(); ++it)
           (*it)->accept(*this);
        }
            
    void visit(TimesTerm& arg)
        {
        arg.lhs->accept(*this);
        arg.rhs->accept(*this);
        }

    void visit(DivideTerm& arg)
        {
        arg.lhs->accept(*this);
        arg.rhs->accept(*this);
        }

    void visit(AbsTerm& arg)
        {arg.body->accept(*this);}
    void visit(CeilTerm& arg)
        {arg.body->accept(*this);}
    void visit(FloorTerm& arg)
        {arg.body->accept(*this);}
    void visit(ExpTerm& arg)
        {arg.body->accept(*this);}
    void visit(LogTerm& arg)
        {arg.body->accept(*this);}
    void visit(Log10Term& arg)
        {arg.body->accept(*this);}
    void visit(SqrtTerm& arg)
        {arg.body->accept(*this);}
    void visit(SinTerm& arg)
        {arg.body->accept(*this);}
    void visit(CosTerm& arg)
        {arg.body->accept(*this);}
    void visit(TanTerm& arg)
        {arg.body->accept(*this);}
    void visit(SinhTerm& arg)
        {arg.body->accept(*this);}
    void visit(CoshTerm& arg)
        {arg.body->accept(*this);}
    void visit(TanhTerm& arg)
        {arg.body->accept(*this);}
    void visit(ASinTerm& arg)
        {arg.body->accept(*this);}
    void visit(ACosTerm& arg)
        {arg.body->accept(*this);}
    void visit(ATanTerm& arg)
        {arg.body->accept(*this);}
    void visit(ASinhTerm& arg)
        {arg.body->accept(*this);}
    void visit(ACoshTerm& arg)
        {arg.body->accept(*this);}
    void visit(ATanhTerm& arg)
        {arg.body->accept(*this);}
    void visit(PowTerm& arg)
        {
        arg.lhs->accept(*this);
        arg.rhs->accept(*this);
        }
};

}

void find_vars_and_params(expr_pointer_t expr, 
                    std::unordered_set<VariableTerm*>& vars,
                    std::set<VariableTerm*>& fixed_vars,
                    std::set<ParameterTerm*>& params
                    )
{
// GCOVR_EXCL_START
if (expr == 0)
    return;
// GCOVR_EXCL_STOP

VariablesVisitor visitor(vars, fixed_vars, params);
expr->accept(visitor);
}

void find_variables(expr_pointer_t expr, std::unordered_set<VariableTerm*>& vars)
{
std::set<VariableTerm*> fixed_vars;
std::set<ParameterTerm*> params;
find_vars_and_params(expr, vars, fixed_vars, params);
}

}
