
#include "model.h"


///
/// MODEL
///


void Model::set_variables(std::vector<double>& x)
{
assert(x.size() == variables.size());
int j=0;
for (variables_iterator_type it=variables.begin(); it != variables.end(); it++) {
    (*it)->_value = x[j];
    j++;
    }
}


///
/// MODEL1
///

void Model1::build_expression(NumericValue* root, std::list<NumericValue*>& curr_build)
{
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
// A topological sort to construct the build
//
std::list<NumericValue*> todo;
std::set<NumericValue*> seen;

todo.push_back(root);
while (todo.size() > 0) {
    //std::cout << "TODO " << todo.size() << std::endl;
    //
    // Get the end of the stack
    //
    NumericValue* curr = todo.back();
    //
    // If the current node is seen, then append the list.  All nodes that 
    // this expression depend on are ahead of it in the list.
    //
    if (seen.find(curr) != seen.end()) {
        //std::cout << "SEEN" << std::endl;
        //curr->print(std::cout);
        //std::cout << std::endl;
        curr_build.push_back(curr);
        todo.pop_back();
        }
    //
    // Otherwise, mark the node as seen and process its children.
    //
    else {
        //std::cout << "CURR " << curr->num_sub_expressions() << std::endl;
        //root->print(std::cout);
        //std::cout << std::endl;
        Expression* _curr = static_cast<Expression*>(curr);
        seen.insert(_curr);
        for (unsigned int i=0; i<_curr->num_sub_expressions(); i++) {
            //std::cout << "i " << i << "  ";
            //std::cout << std::flush;
            NumericValue* child = _curr->expression(i);
            //child->print(std::cout);
            //std::cout << std::endl;
            //std::cout << child->is_expression() << " " << child->is_variable() << std::endl;
            //std::cout << std::flush;
            if (child->is_expression()) {
                Expression* tmp = static_cast<Expression*>(child);
                if (seen.find(tmp) == seen.end()) {
                    todo.push_back(tmp);
                    }
                }
            else if (child->is_variable()) {
                //std::cout << "VAR ";
                //child->print(std::cout);
                //std::cout << std::endl;
                variables.insert( static_cast<Variable*>(child) );
                //for (variables_iterator_type it=variables.begin(); it != variables.end(); it++) {
                //    (*it)->print(std::cout);
                //    std::cout << " " << (*it) << " ";
                //    }
                //std::cout << std::endl;
                }
            /// ELSE, ignore constants
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

    std::map<Variable*, NumericValue*> ad;
    reverse_ad(*it, ad);
    builds_df[nb].resize(variables.size());
    unsigned int j=0;
    for (variables_iterator_type it=variables.begin(); it != variables.end(); it++) {
        std::pair<int,int> index(i,j);
        df_map[index] = ad[*it];
        build_expression(ad[*it], builds_df[nb][j]);
        j++;
        }
    nb++;
    i++;
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

//std::cout << "NVAR " << variables.size() << std::endl;
double ans = 0.0;
//std::cout << "HERE " << tmp.size() << std::endl << std::endl;
for (std::list<NumericValue*>::iterator it=tmp.begin(); it != tmp.end(); it++) {
    ans = (*it)->compute_value();
    //std::cout << "ANS " << ans << std::endl;
    //(*it)->print(std::cout);
    //std::cout << std::endl;
    }

return ans;
}

void Model1::_compute_df(std::vector<double>& df, unsigned int i)
{
assert(i < builds_df.size());
std::vector< std::list<NumericValue*> >& builds = builds_df[i];

for (int j=0; j<builds.size(); j++) {
    std::list<NumericValue*>& tmp = builds[j];
    //std::cout << "NVAR " << variables.size() << std::endl;

    double ans = 0.0;
    //std::cout << "HERE " << tmp.size() << std::endl << std::endl;
    for (std::list<NumericValue*>::iterator it=tmp.begin(); it != tmp.end(); it++) {
        ans = (*it)->compute_value();
        //std::cout << "ANS " << ans << std::endl;
        //(*it)->print(std::cout);
        //std::cout << std::endl;
        }

    df[j] = ans;
    }
}

//
// These next two functions probably don't belong in this file
//
NumericValue* plus(NumericValue* lhs, NumericValue* rhs)
{
if (lhs == &ZeroParameter)
    return rhs;
if (rhs == &ZeroParameter)
    return lhs;
if (lhs->is_parameter() and rhs->is_parameter())
    return new TypedParameter<double>(lhs->_value + rhs->_value, false);
return new AddExpression<NumericValue*,NumericValue*>(lhs,rhs);
}

NumericValue* times(NumericValue* lhs, NumericValue* rhs)
{
if ((lhs == &ZeroParameter) || (rhs == &ZeroParameter))
    return &ZeroParameter;
if (lhs == &OneParameter)
    return rhs;
if (rhs == &OneParameter)
    return lhs;
if (lhs->is_parameter() and rhs->is_parameter())
    return new TypedParameter<double>(lhs->_value * rhs->_value, false);
return new MulExpression<NumericValue*,NumericValue*>(lhs,rhs);
}


void Model1::reverse_ad(Expression* root, std::map<Variable*, NumericValue*>& ad)
{
//
// Use a breadth-first-search to construct the expressions needed for AD
//
// NOTE: We could generalize this to compute AD simultaneously for the Jacobian, simply
// by passing in a list of "roots".  But for now, let's keep it simple.
//
std::map<NumericValue*, NumericValue*> partial;
std::list<Expression*> queue;
std::set<NumericValue*> seen;

// Default is zero, if the variable does not exist in this expression
for (variables_iterator_type it=variables.begin(); it != variables.end(); it++)
    ad[*it] = &ZeroParameter;

if (! root->is_parameter()) {
    partial[root] = &OneParameter;
    if (! root->is_variable()) {
        queue.push_back(static_cast<Expression*>(root));
        seen.insert(root);
        }
    }

while (queue.size() > 0) {
    std::cout << "TODO " << queue.size() << std::endl;
    //
    // Get the front of the queue
    //
    Expression* curr = queue.front();
    queue.pop_front();

    std::cout << "CURR " << curr << " ";
    curr->print(std::cout);
    std::cout << std::endl;
    //
    // Iterate over children.  Create partial and add them to the 
    // queue
    //
    for (unsigned int i=0; i<curr->num_sub_expressions(); i++) {
        NumericValue* _partial = curr->partial(i);
        NumericValue* child = curr->expression(i);
        if (child->is_expression() || child->is_variable()) {
            std::cout << "i " << i << "  ";
            std::cout << std::flush;

            child->print(std::cout);
            std::cout << std::endl;
            std::cout << child->is_expression() << " " << child->is_variable() << " " << child << std::endl;
            std::cout << std::flush;

            if (seen.find(child) == seen.end()) {
                std::cout << "PUSH" << std::endl;
                if (child->is_expression()) {
                    queue.push_back(static_cast<Expression*>(child));
                    seen.insert(child);
                    }
                partial[child] = times(partial[curr], _partial);
                }
            else {
                partial[child] = plus(partial[child], times(partial[curr], _partial));
                }

            std::cout << "PARTIAL" << std::endl;
            child->print(std::cout);
            std::cout << " :  ";
            partial[child]->print(std::cout);
            std::cout << std::endl;
            std::cout << std::endl;
            /// ELSE, ignore constants
            }
        }
    }

for (variables_iterator_type it=variables.begin(); it != variables.end(); it++) {
    ad[*it] = partial[*it];

    (*it)->print(std::cout);
    std::cout << " :  ";
    ad[*it]->print(std::cout);
    std::cout << std::endl;
    }
}
