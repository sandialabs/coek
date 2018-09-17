
#include <vector>
#include <list>
#include <map>
#include <set>
#include <iostream>

#include "expr_types.h"


//
// The base model class that defines the API used by Python
//
class Model 
{
public:

    std::list<Expression*> objectives;
    std::list<Expression*> inequalities;
    std::list<Expression*> equalities;

    std::set<Variable*, bool(*)(const Variable*, const Variable*)> variables;
    typedef std::set<Variable*, bool(*)(const Variable*, const Variable*)>::iterator variables_iterator_type;

    Model(void) : variables(variable_comparator) {}

    int num_variables() {return variables.size();}

    virtual void build() = 0;

    virtual void set_variables(std::vector<double>& x);

    virtual double compute_f(std::vector<double>& x)
        {return compute_f(x, 0);}

    virtual double compute_f(std::vector<double>& x, unsigned int i)
        {
        set_variables(x);
        return _compute_f(i);
        }

    void compute_df(std::vector<double>& x, std::vector<double>& df)
        {return compute_df(x, df, 0);}

    void compute_df(std::vector<double>& x, std::vector<double>& df, unsigned int i)
        {
        set_variables(x);
        return _compute_df(df, i);
        }

    void compute_c(std::vector<double>& x, std::vector<double>& c);

    //double compute_dc(std::vector<double>& x, unsigned int i);

    //void compute_J(std::vector<double>& x, std::vector<double>& J);

    virtual double _compute_f(unsigned int i) = 0;

    virtual void _compute_df(std::vector<double>& df, unsigned int i) = 0;

    virtual void print(std::ostream& ostr, int df);

};


//
// A "first" extension to support calculation of functions, constraints and gradients.
//
class Model1 : public Model
{
public:

    //std::map<int,Expression*,int> f;
    std::map<std::pair<int,int>,NumericValue*> df_map;
    std::vector< std::list<NumericValue*> > builds_f;
    std::vector< std::vector< std::list<NumericValue*> > > builds_df;

    Model1() : Model() {}

    void build();

    double _compute_f(unsigned int i);

    void _compute_df(std::vector<double>& df, unsigned int i);

    void print(std::ostream& ostr, int df);

protected:

    void build_expression(NumericValue* root, std::list<NumericValue*>& curr_build);

    void reverse_ad(NumericValue* root, std::map<Variable*, NumericValue*>& ad);

};

