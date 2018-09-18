
#include "model.h"


///
/// MODEL
///


void Model::set_variables(std::vector<double>& x)
{
///std::cout << "x=" << x.size() << "  vars=" << variables.size() << std::endl << std::flush;
assert(x.size() == variables.size());
int j=0;
for (variables_iterator_type it=variables.begin(); it != variables.end(); it++) {
    (*it)->_value = x[j];
    j++;
    }
}

void Model::print(std::ostream& ostr, int df)
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



///
/// MODEL1
///

void Model1::build_expression(NumericValue* root, std::list<NumericValue*>& curr_build)
{
//
// A topological sort to construct the build
//

//std::cout << "BUILD" << std::endl;
//root->print(std::cout);
//std::cout << std::endl;
//std::cout << std::flush;

if (root->is_variable() || root->is_parameter()) {
    if (root->is_variable())
        variables.insert( static_cast<Variable*>(root) );
    curr_build.push_back(root);
    return;
    }

//
// Compute in-degree
//
std::map<NumericValue*,int> D;
std::list<Expression*> queue;
D[root] = 0;
queue.push_back(static_cast<Expression*>(root));
while(queue.size() > 0) {
    Expression* curr = queue.back();
    queue.pop_back();
    for (unsigned int i=0; i<curr->num_sub_expressions(); i++) {
        NumericValue* child = curr->expression(i);
        if (D.find(child) == D.end())
            D[child] = 1;
        else
            D[child] += 1;
        if (child->is_expression())
            queue.push_back(static_cast<Expression*>(child));
        }
    }
//
// Process nodes, and add them to the queue when 
// they have been reached by all parents.
//
queue.push_back(static_cast<Expression*>(root));
while (queue.size() > 0) {
    ///std::cout << "TODO " << queue.size() << std::endl;
    //
    // Get the front of the queue
    //
    Expression* curr = queue.front();
    queue.pop_front();
    curr_build.push_front(curr);

    ///std::cout << "CURR " << curr << " ";
    ///curr->print(std::cout);
    ///std::cout << std::endl;
    //
    for (unsigned int i=0; i<curr->num_sub_expressions(); i++) {
        NumericValue* child = curr->expression(i);
        if (child->is_expression()) {
            ///std::cout << "i " << i << "  ";
            ///std::cout << std::flush;

            ///child->print(std::cout);
            ///std::cout << std::endl;
            ///std::cout << child->is_expression() << " " << child->is_variable() << " " << child << " D=" << D[child] << std::endl;
            ///std::cout << std::flush;

            D[child]--;
            if (D[child] == 0) {
                ///std::cout << "PUSH" << std::endl;
                queue.push_back(static_cast<Expression*>(child));
                }
            }
        else if (child->is_variable()) {
            variables.insert( static_cast<Variable*>(child) );
            }
        }
    }
}


void Model1::build()
{
builds_f.resize(objectives.size() + inequalities.size() + equalities.size());
int nb=0;

for (std::list<Expression*>::iterator it=objectives.begin(); it != objectives.end(); ++it) {
    build_expression(*it, builds_f[nb]);
    nb++;
    }

for (std::list<Expression*>::iterator it=inequalities.begin(); it != inequalities.end(); ++it) {
    build_expression(*it, builds_f[nb]);
    nb++;
    }

for (std::list<Expression*>::iterator it=equalities.begin(); it != equalities.end(); ++it) {
    build_expression(*it, builds_f[nb]);
    nb++;
    }
}

double Model1::_compute_f(unsigned int i)
{
assert(i < builds_f.size());
std::list<NumericValue*>& tmp = builds_f[i];

///std::cout << "NVAR " << variables.size() << std::endl;
double ans = 0.0;
///std::cout << "HERE " << tmp.size() << std::endl << std::endl;
for (std::list<NumericValue*>::iterator it=tmp.begin(); it != tmp.end(); it++) {
    ans = (*it)->compute_value();
    ///std::cout << "ANS " << ans << std::endl;
    ///(*it)->print(std::cout);
    ///std::cout << std::endl;
    }

return ans;
}

void Model1::_compute_df(double& f, std::vector<double>& df, unsigned int i)
{
assert(i < builds_f.size());
assert(variables.size() == df.size());

/// _compute_f + set _dvalue=0
std::list<NumericValue*>& tmp = builds_f[i];
double ans = 0.0;
for (std::list<NumericValue*>::iterator it=tmp.begin(); it != tmp.end(); it++) {
    NumericValue* tmp = *it;
    ans = tmp->compute_value();
    tmp->_dvalue = 0;
    }
f = ans;

/// Set _dvalue for variables
for (variables_iterator_type it=variables.begin(); it != variables.end(); it++)
    (*it)->_dvalue = 0;

/// Compute reverse AD
std::list<NumericValue*>::reverse_iterator rit=tmp.rbegin();
(*rit)->_dvalue = 1;    // SEED VALUE FOR AD
for ( ; rit != tmp.rend(); rit++)
    (*rit)->compute_partial();

/// Retrieve _dvalue from variables
int j=0;
for (variables_iterator_type it=variables.begin(); it != variables.end(); it++)
    df[j++] = (*it)->_dvalue;
}


void Model1::_compute_c(std::vector<double>& c)
{
assert((inequalities.size() + equalities.size()) == c.size());

int i=objectives.size();
for (int j=0; j<c.size(); j++, i++) {
    std::list<NumericValue*>& tmp = builds_f[i];
    double ans = 0.0;
    for (std::list<NumericValue*>::iterator it=tmp.begin(); it != tmp.end(); it++) {
        NumericValue* tmp = *it;
        ans = tmp->compute_value();
        }
    c[j] = ans;
    }
}

void Model1::_compute_dc(std::vector<double>& dc, unsigned int i)
{
assert(variables.size() == dc.size());
assert(i < (inequalities.size() + equalities.size()));

/// _compute_c + set _dvalue=0
std::list<NumericValue*>& tmp = builds_f[objectives.size() + i];
double ans = 0.0;
for (std::list<NumericValue*>::iterator it=tmp.begin(); it != tmp.end(); it++) {
    NumericValue* tmp = *it;
    ans = tmp->compute_value();
    tmp->_dvalue = 0;
    }
//c[i] = ans;

/// Set _dvalue for variables
for (variables_iterator_type it=variables.begin(); it != variables.end(); it++)
    (*it)->_dvalue = 0;

/// Compute reverse AD
std::list<NumericValue*>::reverse_iterator rit=tmp.rbegin();
(*rit)->_dvalue = 1;    // SEED VALUE FOR AD
for ( ; rit != tmp.rend(); rit++)
    (*rit)->compute_partial();

/// Retrieve _dvalue from variables
int j=0;
for (variables_iterator_type it=variables.begin(); it != variables.end(); it++)
    dc[j++] = (*it)->_dvalue;
}


void Model1::print(std::ostream& ostr, int df)
{
Model::print(ostr, df);

if ((variables.size() > 0) && df) {
    ostr << std::endl;

    ostr << "  DF" << std::endl;
    int k = 0;
    for (std::list<Expression*>::iterator it=objectives.begin(); it != objectives.end(); ++it) {
        std::map<Variable*, NumericValue*> diff;
        symbolic_diff_all(*it,  diff);
        ostr << "    (Objective " << k << ")" << std::endl;
        for (variables_iterator_type IT=variables.begin(); IT != variables.end(); IT++) {
            ostr << "    ";
            (*IT)->print(ostr);
            ostr << " :  ";
            if (diff.find(*IT) == diff.end())
                ostr << "0" << std::endl;
            else {
                diff[*IT]->print(ostr);
                ostr << std::endl;
                }
            }
        ostr << std::endl;
        k++;
        }

    ostr << "  DC" << std::endl;
    k = 0;
    for (std::list<Expression*>::iterator it=inequalities.begin(); it != inequalities.end(); ++it) {
        std::map<Variable*, NumericValue*> diff;
        InequalityExpression* tmp = static_cast<InequalityExpression*>(*it);
        symbolic_diff_all(tmp->body,  diff);
        ostr << "    (Inequality " << k << ")" << std::endl;
        for (variables_iterator_type IT=variables.begin(); IT != variables.end(); IT++) {
            ostr << "    ";
            (*IT)->print(ostr);
            ostr << " :  ";
            if (diff.find(*IT) == diff.end())
                ostr << "0" << std::endl;
            else {
                diff[*IT]->print(ostr);
                ostr << std::endl;
                }
            }
        ostr << std::endl;
        k++;
        }

    ostr << "  DC" << std::endl;
    k = 0;
    for (std::list<Expression*>::iterator it=equalities.begin(); it != equalities.end(); ++it) {
        std::map<Variable*, NumericValue*> diff;
        EqualityExpression* tmp = static_cast<EqualityExpression*>(*it);
        symbolic_diff_all(tmp->body,  diff);
        ostr << "    (Equality " << k << ")" << std::endl;
        for (variables_iterator_type IT=variables.begin(); IT != variables.end(); IT++) {
            ostr << "    ";
            (*IT)->print(ostr);
            ostr << " :  ";
            if (diff.find(*IT) == diff.end())
                ostr << "0" << std::endl;
            else {
                diff[*IT]->print(ostr);
                ostr << std::endl;
                }
            }
        ostr << std::endl;
        k++;
        }

    }
}

