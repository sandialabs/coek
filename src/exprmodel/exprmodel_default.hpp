#pragma once

#include <vector>
#include "expr/expr_manager_objects.hpp"
#include "exprmodel/exprmodel_base.hpp"

namespace coek {

namespace exprmodel_default {

//
// An object that manages model expressions
//

class ExprModel : public coek::base::ExprModel<ExprManager_Objects>
{
public:

    std::vector<expr_t> objectives;
    std::vector<expr_t> inequalities;
    std::vector<expr_t> equalities;

    void print(std::ostream& ostr);

    void add_objective(expr_t expr)
        { objectives.push_back(expr); }

    void add_inequality(expr_t expr)
        { inequalities.push_back(expr); }

    void add_equality(expr_t expr)
        { equalities.push_back(expr); }

    expr_t get_objective(unsigned int n)
        { return objectives[n]; }

    expr_t get_constraint(unsigned int n)
        {
        if (n < inequalities.size())
            return inequalities[n];
        return equalities[n - inequalities.size()];
        }

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
