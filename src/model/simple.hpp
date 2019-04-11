#pragma once

#include "model.hpp"
#include "expr/expr_manager_objects.hpp"

namespace coek {

namespace modelexpr_default {

//
// An object that manages model expressions
//

class ModelExpr_default : public coek::ModelExpr<ExprManager_Objects>
{
public:

    std::list<expr_t> objectives;
    std::list<expr_t> inequalities;
    std::list<expr_t> equalities;

    void print(std::ostream& ostr);

    void add_objective(expr_t expr)
        { objectives.push_back(expr); }

    void add_inequality(expr_t expr)
        { inequalities.push_back(expr); }

    void add_equality(expr_t expr)
        { equalities.push_back(expr); }

    expr_t var(bool _binary, bool _integer, double lb, double ub, double init)
      { return var(_binary, _integer, lb, ub, init, ""); }
    expr_t var(bool _binary, bool _integer, double lb, double ub, double init, const char* name)
      { return manager._var(_binary, _integer, lb, ub, init, name); }

    expr_t param(int _value, bool _mutable)
      { return param(_value, _mutable, ""); }
    expr_t param(int _value, bool _mutable, const char* name)
      { return manager._param(_value, _mutable, name); }

    expr_t param(double _value, bool _mutable)
      { return param(_value, _mutable, ""); }
    expr_t param(double _value, bool _mutable, const char* name)
      { return manager._param(_value, _mutable, name); }

};

}

}
