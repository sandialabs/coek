#pragma once

#include "model.hpp"
#include "model/simple.hpp"


class Simple_ADModel : public Simple_ExprModel, public ADModel
{
public:

    std::vector<Variable*> variables;
    typedef std::set<Variable*, bool(*)(const Variable*, const Variable*)>::iterator variables_iterator_type;
    std::vector<std::vector<int> > J_rc;
    std::vector<std::vector<Variable*> > J;
    std::vector< std::list<NumericValue*> > builds_f;

    void build();

    int num_variables()
        { return variables.size(); }

    int num_constraints()
        { return inequalities.size() + equalities.size(); }

    int num_nonzeros_Jacobian();

    void set_variables(std::vector<double>& x);

    void set_variables(const double* x, int n);

    void print(std::ostream& ostr);

    void compute_adjoints(unsigned int i);

protected:

    double _compute_f(unsigned int i);

    void _compute_df(double& f, std::vector<double>& df, unsigned int i);

    void _compute_c(std::vector<double>& c);

    void _compute_dc(std::vector<double>& dc, unsigned int i);

    void _compute_Hv(std::vector<double>& v, std::vector<double>& Hv, unsigned int i);

};

