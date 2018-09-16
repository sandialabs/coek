
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

void Model1::build_expression(Expression* root, std::list<Expression*>& curr_build)
{
//std::cout << "BUILD" << std::endl;
//root->print(std::cout);
//std::cout << std::endl;
//std::cout << std::flush;

//
// A topological sort to construct the build
//
std::list<Expression*> todo;
std::set<Expression*> seen;

todo.push_back(root);
while (todo.size() > 0) {
    //std::cout << "TODO " << todo.size() << std::endl;
    //
    // Get the end of the stack
    //
    Expression* curr = todo.back();
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
        seen.insert(curr);
        for (unsigned int i=0; i<curr->num_sub_expressions(); i++) {
            //std::cout << "i " << i << "  ";
            //std::cout << std::flush;
            NumericValue* child = curr->expression(i);
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
                variables.insert( static_cast<Variable*>(child) );
                }
            /// ELSE, ignore constants
            }
        }
    }
}


void Model1::build()
{
builds.resize(objectives.size() + inequalities.size() + equalities.size());
int nb=0;

for (std::list<Expression*>::iterator it=objectives.begin(); it != objectives.end(); ++it) {
    build_expression(*it, builds[nb]);
    nb++;
    }
for (std::list<Expression*>::iterator it=inequalities.begin(); it != inequalities.end(); ++it) {
    build_expression(*it, builds[nb]);
    nb++;
    }
for (std::list<Expression*>::iterator it=equalities.begin(); it != equalities.end(); ++it) {
    build_expression(*it, builds[nb]);
    nb++;
    }
}

double Model1::_compute_f(unsigned int i)
{
assert(i < builds.size());
std::list<Expression*>& tmp = builds[i];

double ans = 0.0;
//std::cout << "HERE " << tmp.size() << std::endl << std::endl;
for (std::list<Expression*>::iterator it=tmp.begin(); it != tmp.end(); it++) {
    ans = (*it)->compute_value();
    //std::cout << "ANS " << ans << std::endl;
    //(*it)->print(std::cout);
    //std::cout << std::endl;
    }

return ans;
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
std::list<NumericValue*> queue;

partial[root] = &OneParameter;
queue.push_back(root);
while (queue.size() > 0) {
    std::cout << "TODO " << queue.size() << std::endl;
    //
    // Get the front of the queue
    //
    NumericValue* curr = queue.front();
    queue.pop_front();
    //
    if (! curr->is_expression()) {
        }
    else {
        Expression* _curr = static_cast<Expression*>(curr);
        //
        // Iterate over children.  Create partial and add them to the 
        // queue
        //
        for (unsigned int i=0; i<_curr->num_sub_expressions(); i++) {
            std::cout << "i " << i << "  ";
            std::cout << std::flush;

            //NumericValue* partial = _curr->partial(i);
            NumericValue* child = _curr->expression(i);

            child->print(std::cout);
            std::cout << std::endl;
            std::cout << child->is_expression() << " " << child->is_variable() << std::endl;
            std::cout << std::flush;

            if (child->is_expression()) {
                //Expression* tmp = static_cast<Expression*>(child);
                }
            else if (child->is_variable()) {
                }
            /// ELSE, ignore constants?
            }
        }
    }

for (variables_iterator_type it=variables.begin(); it != variables.end(); it++) {
    ad[*it] = partial[*it];
    }
}
