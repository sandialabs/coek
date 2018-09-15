
#include <vector>
#include <list>
#include <map>
#include <set>
#include <iostream>

#include "expr_types.h"


class Model 
{
public:

    std::list<Expression*> objectives;
    std::list<Expression*> inequalities;
    std::list<Expression*> equalities;

    std::map<Expression*,int> f;
    std::map<Expression*,int> df;
    std::vector< std::list<Expression*> > builds;
    std::set<Variable*, bool(*)(const Variable*, const Variable*)> variables;
    typedef std::set<Variable*, bool(*)(const Variable*, const Variable*)>::iterator variables_iterator_type;

    Model(void) : variables(variable_comparator) {}

    void print(std::ostream& ostr)
        {
        ostr << "MODEL" << std::endl;

        ostr << "  Objectives" << std::endl;
        for (std::list<Expression*>::iterator it=objectives.begin(); it != objectives.end(); ++it) {
            ostr << "    ";
            (*it)->print(ostr);
            ostr << std::endl;
            }
        ostr << std::endl;

        ostr << "  Inequality Constraints" << std::endl;
        for (std::list<Expression*>::iterator it=inequalities.begin(); it != inequalities.end(); ++it) {
            ostr << "    ";
            (*it)->print(ostr);
            ostr << std::endl;
            }
        ostr << std::endl;

        ostr << "  Equality Constraints" << std::endl;
        for (std::list<Expression*>::iterator it=equalities.begin(); it != equalities.end(); ++it) {
            ostr << "    ";
            (*it)->print(ostr);
            ostr << std::endl;
            }
        }

    void build();

    void set_variables(std::vector<double>& x);

    double compute_f(std::vector<double>& x)
        {return compute_f(x, 0);}

    double compute_f(std::vector<double>& x, unsigned int i)
        {
        set_variables(x);
        return _compute_f(i);
        }

    //void compute_df(std::vector<double>& x, std::vector<double>& df)
    //    {return compute_df(x, 0);}

    //void compute_df(std::vector<double>& x, std::vector<double>& df, unsigned int i);

    //void compute_c(std::vector<double>& x, std::vector<double>& c);

    //double compute_dc(std::vector<double>& x, unsigned int i);

    //void compute_J(std::vector<double>& x, std::vector<double>& J);

    double _compute_f(unsigned int i);

protected:

    void build_expression(Expression* root, std::list<Expression*>& curr_build);
};

