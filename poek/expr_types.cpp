
#include <set>
#include <list>
#include <cmath>

#include "expr_types.h"

TypedParameter<int> ZeroParameter(0,false);
TypedParameter<int> OneParameter(1,false);
TypedParameter<int> NegativeOneParameter(-1,false);


int Variable::nvariables = 0;
double Variable::_nan = NAN;

bool variable_comparator(const Variable* lhs, const Variable* rhs)
{
return lhs->index < rhs->index;
}


NumericValue* PowExpression::partial(unsigned int i)
{
NumericValue* base = this->lhs;
NumericValue* exp =  this->rhs;
if (i==0)
    return new MulExpression<NumericValue*,NumericValue*>( exp, new PowExpression( base, new SubExpression(exp, &OneParameter) ) );
else
    return new MulExpression<NumericValue*,NumericValue*>( new LogExpression( base ), this );
}


void PowExpression::compute_adjoint()
{
double base = this->lhs->_value;
double exp = this->rhs->_value;

this->lhs->adjoint += this->adjoint * exp * pow(base, exp-1);
this->rhs->adjoint += this->adjoint * log(base) * this->_value;
}


//
// Should these functions be included in the header?
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



void symbolic_diff_all(NumericValue* root, std::map<Variable*, NumericValue*>& diff)
{
//
// Use a topological sort
//
// NOTE: We could generalize this to compute AD simultaneously for the Jacobian, simply
// by passing in a list of "roots".  But for now, let's keep it simple.
//

// Default is zero, if the variable does not exist in this expression

if (root->is_parameter())
    return;

if (root->is_variable()) {
    diff[static_cast<Variable*>(root)] = &OneParameter;
    return;
    }

ordered_variable_t variables(variable_comparator);

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
        else if (child->is_variable())
            variables.insert(static_cast<Variable*>(child));
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

for (ordered_variable_iterator_t it=variables.begin(); it != variables.end(); it++) {
    diff[*it] = partial[*it];

    ///(*it)->print(std::cout);
    ///std::cout << " :  ";
    ///diff[*it]->print(std::cout);
    ///std::cout << std::endl;
    }
}


void build_expression(NumericValue* root, std::list<NumericValue*>& build, ordered_variable_t& variables)
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
    build.push_back(root);
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
    build.push_front(curr);

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


double compute_expression_value(NumericValue* root)
{
//std::cout << "X1" << std::endl << std::flush;
std::list<NumericValue*> build;
ordered_variable_t variables(variable_comparator);

build_expression(root, build, variables);
//std::cout << "X2" << std::endl << std::flush;

double ans = 0;
for (std::list<NumericValue*>::iterator it=build.begin(); it != build.end(); it++)
    ans = (*it)->compute_value();
//std::cout << "X3" << std::endl << std::flush;

return ans;
}




void walk_expression_tree(NumericValue* root, void(*enter_callback)(void*,void*,void*), void(*exit_callback)(void*,void*,void*), void* visitor)
{
if (!root->is_expression()) {
    (*enter_callback)(root, 0, visitor);
    (*exit_callback)(root, 0, visitor);
    return;
    }

std::map<NumericValue*,NumericValue*> parent;
std::list<NumericValue*> stack;
stack.push_back(root);
parent[root] = 0;

while (stack.size() > 0) {
    NumericValue* curr = stack.back();
    stack.pop_back();
    if (curr == 0) {
        (*exit_callback)(curr, parent[curr], visitor);
        continue;
        }
    (*enter_callback)(curr, parent[curr], visitor);
    if (curr->is_expression()) {
        stack.push_back( 0 );
        Expression* _curr = static_cast<Expression*>(curr);
        //for (unsigned int i=0; i<curr->num_sub_expressions(); i++) {
        for (int i=_curr->num_sub_expressions()-1; i>=0; i--) {
            NumericValue* child = _curr->expression(i);
            parent[child] = curr;
            if (! child->is_expression())
                stack.push_back( 0 );
            stack.push_back( static_cast<Expression*>(child) );
            }
        }
    }
}
