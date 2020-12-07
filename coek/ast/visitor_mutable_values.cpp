#include "visitor_fns.hpp"
#include "visitor.hpp"
#include "base_terms.hpp"
#include "constraint_terms.hpp"
#include "objective_terms.hpp"
#include "expr_terms.hpp"
#include "value_terms.hpp"
#include "compact_terms.hpp"


namespace coek {

namespace {

class MutableValuesVisitor : public Visitor
{
public:

    std::unordered_set<VariableTerm*>& fixed_vars;
    std::unordered_set<ParameterTerm*>& params;

public:

    MutableValuesVisitor(std::unordered_set<VariableTerm*>& _fixed_vars,
                         std::unordered_set<ParameterTerm*>& _params)
        : fixed_vars(_fixed_vars), params(_params) {}

    void visit(ConstantTerm& )
        {}

    void visit(ParameterTerm& arg)
        {params.insert(&arg);}

    void visit(IndexParameterTerm& )
        {}

    void visit(VariableTerm& arg)
        { if (arg.fixed) fixed_vars.insert(&arg); }

    void visit(VariableRefTerm& )
        {}

    void visit(IndexedVariableTerm& arg)
        { if (arg.fixed) fixed_vars.insert(&arg); }

    void visit(MonomialTerm& arg)
        { if (arg.var->fixed) fixed_vars.insert(arg.var); }

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

void mutable_values(expr_pointer_t expr, std::unordered_set<VariableTerm*>& fixed_vars,
                                         std::unordered_set<ParameterTerm*>& params)
{
// GCOVR_EXCL_START
if (expr == 0)
    return;
// GCOVR_EXCL_STOP

MutableValuesVisitor visitor(fixed_vars, params);
expr->accept(visitor);
}

}
