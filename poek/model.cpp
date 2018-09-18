
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
builds_df.resize(objectives.size() + inequalities.size() + equalities.size());
int nb=0;

int i=0;
for (std::list<Expression*>::iterator it=objectives.begin(); it != objectives.end(); ++it) {
    build_expression(*it, builds_f[nb]);

    /*
    std::map<Variable*, NumericValue*> ad;
    reverse_ad(*it, ad);
    builds_df[nb].resize(variables.size());
    unsigned int j=0;
    for (variables_iterator_type IT=variables.begin(); IT != variables.end(); IT++) {
        std::pair<int,int> index(i,j);
        df_map[index] = ad[*IT];
        build_expression(ad[*IT], builds_df[nb][j]);
        j++;
        }
    */
    nb++;
    i++;
    }

for (std::list<Expression*>::iterator it=inequalities.begin(); it != inequalities.end(); ++it) {
    build_expression(*it, builds_f[nb]);

    /*
    InequalityExpression* curr = static_cast<InequalityExpression*>(*it);
    std::map<Variable*, NumericValue*> ad;
    reverse_ad(curr->body, ad);
    builds_df[nb].resize(variables.size());
    unsigned int j=0;
    for (variables_iterator_type IT=variables.begin(); IT != variables.end(); IT++) {
        std::pair<int,int> index(i,j);
        df_map[index] = ad[*IT];
        build_expression(ad[*IT], builds_df[nb][j]);
        j++;
        }
    */
    nb++;
    i++;
    }

for (std::list<Expression*>::iterator it=equalities.begin(); it != equalities.end(); ++it) {
    build_expression(*it, builds_f[nb]);

    /*
    EqualityExpression* curr = static_cast<EqualityExpression*>(*it);
    std::map<Variable*, NumericValue*> ad;
    reverse_ad(curr->body, ad);
    builds_df[nb].resize(variables.size());
    unsigned int j=0;
    for (variables_iterator_type IT=variables.begin(); IT != variables.end(); IT++) {
        std::pair<int,int> index(i,j);
        df_map[index] = ad[*IT];
        build_expression(ad[*IT], builds_df[nb][j]);
        j++;
        }
    */
    nb++;
    i++;
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

void Model1::_compute_df(std::vector<double>& df, unsigned int i)
{
assert(i < builds_df.size());
std::vector< std::list<NumericValue*> >& builds = builds_df[i];

for (int j=0; j<builds.size(); j++) {
    std::list<NumericValue*>& tmp = builds[j];
    ///std::cout << "NVAR " << variables.size() << std::endl;

    double ans = 0.0;
    ///std::cout << "HERE " << tmp.size() << std::endl << std::endl;
    for (std::list<NumericValue*>::iterator it=tmp.begin(); it != tmp.end(); it++) {
        ans = (*it)->compute_value();
        ///std::cout << "ANS " << ans << std::endl;
        ///(*it)->print(std::cout);
        ///std::cout << std::endl;
        }

    df[j] = ans;
    }
}


void Model1::_compute_c(std::vector<double>& c)
{
}

void Model1::_compute_dc(std::vector<double>& dc, unsigned int i)
{
}


void Model1::reverse_ad(NumericValue* root, std::map<Variable*, NumericValue*>& ad)
{
/***
//
// Use a topological sort
//
// NOTE: We could generalize this to compute AD simultaneously for the Jacobian, simply
// by passing in a list of "roots".  But for now, let's keep it simple.
//

// Default is zero, if the variable does not exist in this expression
for (variables_iterator_type it=variables.begin(); it != variables.end(); it++)
    ad[*it] = &ZeroParameter;

if (root->is_parameter())
    return;

if (root->is_variable()) {
    ad[static_cast<Variable*>(root)] = &OneParameter;
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
std::map<NumericValue*, NumericValue*> partial;
partial[root] = &OneParameter;
queue.push_back(static_cast<Expression*>(root));

while (queue.size() > 0) {
    ///std::cout << "TODO " << queue.size() << std::endl;
    //
    // Get the front of the queue
    //
    Expression* curr = queue.front();
    queue.pop_front();

    ///std::cout << "CURR " << curr << " ";
    ///curr->print(std::cout);
    ///std::cout << std::endl;
    //
    // Iterate over children.  Create partial and add them to the 
    // queue
    //
    for (unsigned int i=0; i<curr->num_sub_expressions(); i++) {
        NumericValue* _partial = curr->partial(i);
        NumericValue* child = curr->expression(i);
        if (child->is_expression() || child->is_variable()) {
            ///std::cout << "i " << i << "  ";
            ///std::cout << std::flush;

            ///child->print(std::cout);
            ///std::cout << std::endl;
            ///std::cout << child->is_expression() << " " << child->is_variable() << " " << child << " D=" << D[child] << std::endl;
            ///std::cout << std::flush;

            D[child]--;
            if (D[child] == 0) {
                ///std::cout << "PUSH" << std::endl;
                if (child->is_expression())
                    queue.push_back(static_cast<Expression*>(child));
                }
            ///std::cout << "HERE" << std::endl << std::flush;
            if (partial.find(child) == partial.end())
                partial[child] = times(partial[curr], _partial);
            else
                partial[child] = plus(partial[child], times(partial[curr], _partial));

            ///std::cout << "PARTIAL" << std::endl << std::flush;
            ///child->print(std::cout);
            ///std::cout << " :  ";
            ///partial[child]->print(std::cout);
            ///std::cout << std::endl;
            ///std::cout << std::endl;
            }
        }
    }

for (variables_iterator_type it=variables.begin(); it != variables.end(); it++) {
    ad[*it] = partial[*it];

    ///(*it)->print(std::cout);
    ///std::cout << " :  ";
    ///ad[*it]->print(std::cout);
    ///std::cout << std::endl;
    }
***/
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

