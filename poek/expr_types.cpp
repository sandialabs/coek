
#include <set>
#include <list>

#include "expr_types.h"

TypedParameter<int> ZeroParameter(0,false);
TypedParameter<int> OneParameter(1,false);


int Variable::nvariables = 0;
double Variable::_nan = nan(0);

bool variable_comparator(const Variable* lhs, const Variable* rhs)
{
return lhs->index < rhs->index;
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

std::set<Variable*, bool(*)(const Variable*, const Variable*)> variables(variable_comparator);
typedef std::set<Variable*, bool(*)(const Variable*, const Variable*)>::iterator variables_iterator_type;

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

for (variables_iterator_type it=variables.begin(); it != variables.end(); it++) {
    diff[*it] = partial[*it];

    ///(*it)->print(std::cout);
    ///std::cout << " :  ";
    ///ad[*it]->print(std::cout);
    ///std::cout << std::endl;
    }
}

