
#include <cmath>
#include <vector>
#include <string>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <iostream>

extern "C" {

#include "expr_api.h"

}

double _nan = nan(0);

/*
class Parameter { };

template <typename TYPE>
class DerivedParameter : public Parameter
{
public:

    TYPE value;

    DerivedParameter(TYPE _value)
        { value = _value; }

};
*/


class NumericValue
{
public:

    double _value;

    virtual void print(std::ostream& ostr) = 0;

    virtual bool is_variable() { return false; }
    virtual bool is_expression() { return false; }
    virtual bool is_parameter() { return false; }

    virtual int size() {return 1;}

    virtual double value() = 0;

    virtual double compute_value() = 0;
};


class Parameter : public NumericValue
{
public:

    bool mutable_flag;

    Parameter(bool _mutable_flag) : NumericValue()
        {
        mutable_flag = _mutable_flag; 
        }

    virtual bool is_parameter() { return true; }

};


template <typename TYPE>
class TypedParameter : public Parameter
{
public:

    TYPE _tvalue;

    TypedParameter(TYPE __value, bool _mutable_flag) : Parameter(_mutable_flag)
        {_value = _tvalue = __value;}

    double value() {return _tvalue;}

    double compute_value() {return _tvalue;}

    void print(std::ostream& ostr)
        { ostr << _value; }

};


class Variable : public NumericValue
{
public:

    static int nvariables;

    double _value;
    bool binary;
    bool integer;
    int index;

    Variable(bool _binary, bool _integer) : NumericValue()
        {
        _value = _nan;
        binary = _binary; 
        integer = _integer; 
        index = nvariables++;
        }

    virtual double value() {return _value;}

    double compute_value() {return _value;}

    void print(std::ostream& ostr)
        { ostr << 'x' << index; }

    bool is_variable() { return true; }

};

int Variable::nvariables = 0;


class Expression: public NumericValue
{
public:

    double _value;

    bool is_expression() { return true; }

    void print(std::ostream& ostr) {ostr << "ERROR: Undefined print() method.";}

    virtual unsigned int num_sub_expressions() = 0;

    virtual NumericValue* expression(unsigned int i) = 0;

};


class InequalityExpression : public Expression
{
public:

    NumericValue* body;

    InequalityExpression(NumericValue* _body)
        {body = _body;}

    void print(std::ostream& ostr) {body->print(ostr); ostr << "  <=  0";}

    int size() {return body->size();}

    unsigned int num_sub_expressions() {return 1;}

    NumericValue* expression(unsigned int i) { if (i == 0) return body; else return 0; }

    double value() {return body->value();}

    double compute_value() {_value = body->_value; return _value;}

};


class EqualityExpression : public Expression
{
public:

    NumericValue* body;

    EqualityExpression(NumericValue* _body)
        {body = _body;}

    void print(std::ostream& ostr) {body->print(ostr); ostr << "  ==  0";}

    int size() {return body->size();}

    unsigned int num_sub_expressions() {return 1;}

    NumericValue* expression(unsigned int i) { if (i == 0) return body; else return 0; }

    double value() {return body->value();}

    double compute_value() {_value = body->_value; return _value;}

};


bool variable_comparator(const Variable* lhs, const Variable* rhs)
{
return lhs->index <= rhs->index;
}

/*
std::set<MyType, bool(*)(const MyType&, const MyType&)> mySet(&comparator);
*/


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


void Model::build_expression(Expression* root, std::list<Expression*>& curr_build)
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


void Model::build()
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


void Model::set_variables(std::vector<double>& x)
{
assert(x.size() == variables.size());
int j=0;
for (variables_iterator_type it=variables.begin(); it != variables.end(); it++) {
    (*it)->_value = x[j];
    j++;
    }
}

double Model::_compute_f(unsigned int i)
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


/*** GLOBAL DATA ***/

std::list<Model*> models;
std::list<Expression*> expressions;
std::list<Parameter*> parameters;
std::list<Variable*> variables;


/*** BINARY ***/

template <typename LHS, typename RHS>
class BinaryExpression : public Expression
{
public:

    LHS lhs;
    RHS rhs;

    BinaryExpression(LHS _lhs, RHS _rhs) : Expression()
        {lhs = _lhs; rhs = _rhs;}

    unsigned int num_sub_expressions() {return BinaryExpression_num_sub_expressions(this);}

    NumericValue* expression(unsigned int i) { return BinaryExpression_expression(i, this); }

    int size() {return BinaryExpression_size(this);}
};

template <typename LHS, typename RHS>
int BinaryExpression_size(BinaryExpression<LHS, RHS>* expr)
{ return expr->lhs->size() + expr->rhs->size() + 1; }

template <typename LHS>
int BinaryExpression_size(BinaryExpression<LHS, int>* expr)
{ return expr->lhs->size() + 2; }

template <typename LHS>
int BinaryExpression_size(BinaryExpression<LHS, double>* expr)
{ return expr->lhs->size() + 2; }

template <typename RHS>
int BinaryExpression_size(BinaryExpression<int, RHS>* expr)
{ return expr->rhs->size() + 2; }

template <typename RHS>
int BinaryExpression_size(BinaryExpression<double, RHS>* expr)
{ return expr->rhs->size() + 2; }


template <typename LHS, typename RHS>
unsigned int BinaryExpression_num_sub_expressions(BinaryExpression<LHS, RHS>* expr)
{ return 2; }

template <typename LHS>
unsigned int BinaryExpression_num_sub_expressions(BinaryExpression<LHS, int>* expr)
{ return 1; }

template <typename LHS>
unsigned int BinaryExpression_num_sub_expressions(BinaryExpression<LHS, double>* expr)
{ return 1; }

template <typename RHS>
unsigned int BinaryExpression_num_sub_expressions(BinaryExpression<int, RHS>* expr)
{ return 1; }

template <typename RHS>
unsigned int BinaryExpression_num_sub_expressions(BinaryExpression<double, RHS>* expr)
{ return 1; }


template <typename LHS, typename RHS>
NumericValue* BinaryExpression_expression(unsigned int i, BinaryExpression<LHS, RHS>* expr)
{
if (i == 0)
    return expr->lhs;
if (i == 1)
    return expr->rhs;
return 0;
}

template <typename LHS>
NumericValue* BinaryExpression_expression(unsigned int i, BinaryExpression<LHS, int>* expr)
{
if (i == 0)
    return expr->lhs;
return 0;
}

template <typename LHS>
NumericValue* BinaryExpression_expression(unsigned int i, BinaryExpression<LHS, double>* expr)
{
if (i == 0)
    return expr->lhs;
return 0;
}

template <typename RHS>
NumericValue* BinaryExpression_expression(unsigned int i, BinaryExpression<int, RHS>* expr)
{
if (i == 0)
    return expr->rhs;
return 0;
}

template <typename RHS>
NumericValue* BinaryExpression_expression(unsigned int i, BinaryExpression<double, RHS>* expr)
{
if (i == 0)
    return expr->rhs;
return 0;
}


/*** ADD ***/

template <typename LHS, typename RHS>
class AddExpression : public BinaryExpression<LHS,RHS>
{
public:

    AddExpression(LHS _lhs, RHS _rhs) : BinaryExpression<LHS,RHS>(_lhs,_rhs)
        {}

    void print(std::ostream& ostr) {AddExpression_print(ostr, this);}

    double value() {return AddExpression_value(this);}

    double compute_value() {this->_value = AddExpression_computevalue(this); return this->_value;}

};

template <typename LHS, typename RHS>
void AddExpression_print(std::ostream& ostr, AddExpression<LHS, RHS>* expr)
{ expr->lhs->print(ostr); ostr << " + "; expr->rhs->print(ostr); }

template <typename LHS>
void AddExpression_print(std::ostream& ostr, AddExpression<LHS, int>* expr)
{ expr->lhs->print(ostr); ostr << " + " << expr->rhs; }

template <typename LHS>
void AddExpression_print(std::ostream& ostr, AddExpression<LHS, double>* expr)
{ expr->lhs->print(ostr); ostr << " + " << expr->rhs; }

template <typename RHS>
void AddExpression_print(std::ostream& ostr, AddExpression<int, RHS>* expr)
{ ostr << expr->lhs << " + "; expr->rhs->print(ostr); }

template <typename RHS>
void AddExpression_print(std::ostream& ostr, AddExpression<double, RHS>* expr)
{ ostr << expr->lhs << " + "; expr->rhs->print(ostr); }


template <typename LHS, typename RHS>
double AddExpression_value(AddExpression<LHS, RHS>* expr)
{ return expr->lhs->value() + expr->rhs->value(); }

template <typename LHS>
double AddExpression_value(AddExpression<LHS, int>* expr)
{ return expr->lhs->value() + expr->rhs; }

template <typename LHS>
double AddExpression_value(AddExpression<LHS, double>* expr)
{ return expr->lhs->value() + expr->rhs; }

template <typename RHS>
double AddExpression_value(AddExpression<int, RHS>* expr)
{ return expr->lhs + expr->rhs->value(); }

template <typename RHS>
double AddExpression_value(AddExpression<double, RHS>* expr)
{ return expr->lhs + expr->rhs->value(); }


template <typename LHS, typename RHS>
double AddExpression_computevalue(AddExpression<LHS, RHS>* expr)
{ return expr->lhs->_value + expr->rhs->_value; }

template <typename LHS>
double AddExpression_computevalue(AddExpression<LHS, int>* expr)
{ return expr->lhs->_value + expr->rhs; }

template <typename LHS>
double AddExpression_computevalue(AddExpression<LHS, double>* expr)
{ return expr->lhs->_value + expr->rhs; }

template <typename RHS>
double AddExpression_computevalue(AddExpression<int, RHS>* expr)
{ return expr->lhs + expr->rhs->_value; }

template <typename RHS>
double AddExpression_computevalue(AddExpression<double, RHS>* expr)
{ return expr->lhs + expr->rhs->_value; }


template <typename LHS, typename RHS>
inline AddExpression<LHS, RHS>* AddExpression_create(LHS lhs, RHS rhs)
{ return new AddExpression<LHS, RHS>(lhs, rhs); }

/* __add__ */
extern "C" void* add_expr_int(void* lhs, int rhs)
{
if (rhs == 0) {
    return lhs;
    }
Expression* tmp;
NumericValue* e = static_cast<NumericValue*>(lhs);
int _rhs = static_cast<int>(rhs);
if (e->is_variable()) {
    Variable* _lhs = static_cast<Variable*>(lhs);
    tmp = AddExpression_create(_lhs, _rhs);
    }
else {
    Expression* _lhs = static_cast<Expression*>(lhs);
    tmp = AddExpression_create(_lhs, _rhs);
    }
expressions.push_back(tmp);
return tmp;
}

extern "C" void* add_expr_double(void* lhs, double rhs)
{
if (rhs == 0.0) {
    return lhs;
    }
Expression* tmp;
NumericValue* e = static_cast<NumericValue*>(lhs);
double _rhs = static_cast<double>(rhs);
if (e->is_variable()) {
    Variable* _lhs = static_cast<Variable*>(lhs);
    tmp = AddExpression_create(_lhs, _rhs);
    }
else {
    Expression* _lhs = static_cast<Expression*>(lhs);
    tmp = AddExpression_create(_lhs, _rhs);
    }
expressions.push_back(tmp);
return tmp;
}

void* add_expr_expression(void* lhs, void* rhs)
{
Expression* tmp;
NumericValue* _lhs = static_cast<NumericValue*>(lhs);
NumericValue* _rhs = static_cast<NumericValue*>(rhs);
if (_lhs->is_variable()) {
    Variable* __lhs = static_cast<Variable*>(lhs);
    if (_rhs->is_variable()) {
        Variable* __rhs = static_cast<Variable*>(rhs);
        tmp = AddExpression_create(__lhs, __rhs);
        }
    else {
        Expression* __rhs = static_cast<Expression*>(rhs);
        tmp = AddExpression_create(__lhs, __rhs);
        }
    }
else {
    Expression* __lhs = static_cast<Expression*>(lhs);
    if (_rhs->is_variable()) {
        Variable* __rhs = static_cast<Variable*>(rhs);
        tmp = AddExpression_create(__lhs, __rhs);
        }
    else {
        Expression* __rhs = static_cast<Expression*>(rhs);
        tmp = AddExpression_create(__lhs, __rhs);
        }
    }
expressions.push_back(tmp);
return tmp;
}


/* __radd__ */
extern "C" void* radd_expr_int(int lhs, void* rhs)
{
if (lhs == 0) {
    return rhs;
    }
Expression* tmp;
NumericValue* e = static_cast<NumericValue*>(rhs);
if (e->is_variable()) {
    Variable* _rhs = static_cast<Variable*>(rhs);
    tmp = new AddExpression<int,Variable*>(lhs, _rhs);
    }
else {
    Expression* _rhs = static_cast<Expression*>(rhs);
    tmp = new AddExpression<int,Expression*>(lhs, _rhs);
    }
expressions.push_back(tmp);
return tmp;
}

extern "C" void* radd_expr_double(double lhs, void* rhs)
{
if (lhs == 0.0) {
    return rhs;
    }
Expression* tmp;
NumericValue* e = static_cast<NumericValue*>(rhs);
if (e->is_variable()) {
    Variable* _rhs = static_cast<Variable*>(rhs);
    tmp = new AddExpression<double,Variable*>(lhs, _rhs);
    }
else {
    Expression* _rhs = static_cast<Expression*>(rhs);
    tmp = new AddExpression<double,Expression*>(lhs, _rhs);
    }
expressions.push_back(tmp);
return tmp;
}


/*** MUL ***/

template <typename LHS, typename RHS>
class MulExpression : public BinaryExpression<LHS,RHS>
{
public:

    MulExpression(LHS _lhs, RHS _rhs) : BinaryExpression<LHS,RHS>(_lhs,_rhs)
        {}

    void print(std::ostream& ostr) {MulExpression_print(ostr, this);}

    double value() {return MulExpression_value(this); }

    double compute_value() {this->_value = MulExpression_computevalue(this); return this->_value;}

};

template <typename LHS, typename RHS>
void MulExpression_print(std::ostream& ostr, MulExpression<LHS, RHS>* expr)
{
if (expr->lhs->is_variable())
    expr->lhs->print(ostr);
else {
    ostr << "("; expr->lhs->print(ostr); ostr << ")";
    }
ostr << "*";
if (expr->rhs->is_variable())
    expr->rhs->print(ostr);
else {
    ostr << "("; expr->rhs->print(ostr); ostr << ")";
    }
}

template <typename LHS>
void MulExpression_print(std::ostream& ostr, MulExpression<LHS, int>* expr)
{ 
if (expr->lhs->is_variable())
    expr->lhs->print(ostr);
else {
    ostr << "("; expr->lhs->print(ostr); ostr << ")";
    }
ostr << "*";
ostr << expr->rhs;
}

template <typename LHS>
void MulExpression_print(std::ostream& ostr, MulExpression<LHS, double>* expr)
{
if (expr->lhs->is_variable())
    expr->lhs->print(ostr);
else {
    ostr << "("; expr->lhs->print(ostr); ostr << ")";
    }
ostr << "*";
ostr << expr->rhs;
}

template <typename RHS>
void MulExpression_print(std::ostream& ostr, MulExpression<int, RHS>* expr)
{
ostr << expr->lhs;
ostr << "*";
if (expr->rhs->is_variable())
    expr->rhs->print(ostr);
else {
    ostr << "("; expr->rhs->print(ostr); ostr << ")";
    }
}

template <typename RHS>
void MulExpression_print(std::ostream& ostr, MulExpression<double, RHS>* expr)
{
ostr << expr->lhs;
ostr << "*";
if (expr->rhs->is_variable())
    expr->rhs->print(ostr);
else {
    ostr << "("; expr->rhs->print(ostr); ostr << ")";
    }
}


template <typename LHS, typename RHS>
double MulExpression_value(MulExpression<LHS, RHS>* expr)
{ return expr->lhs->value() * expr->rhs->value(); }

template <typename LHS>
double MulExpression_value(MulExpression<LHS, int>* expr)
{ return expr->lhs->value() * expr->rhs; }

template <typename LHS>
double MulExpression_value(MulExpression<LHS, double>* expr)
{ return expr->lhs->value() * expr->rhs; }

template <typename RHS>
double MulExpression_value(MulExpression<int, RHS>* expr)
{ return expr->lhs * expr->rhs->value(); }

template <typename RHS>
double MulExpression_value(MulExpression<double, RHS>* expr)
{ return expr->lhs * expr->rhs->value(); }



template <typename LHS, typename RHS>
double MulExpression_computevalue(MulExpression<LHS, RHS>* expr)
{ return expr->lhs->_value * expr->rhs->_value; }

template <typename LHS>
double MulExpression_computevalue(MulExpression<LHS, int>* expr)
{ return expr->lhs->_value * expr->rhs; }

template <typename LHS>
double MulExpression_computevalue(MulExpression<LHS, double>* expr)
{ return expr->lhs->_value * expr->rhs; }

template <typename RHS>
double MulExpression_computevalue(MulExpression<int, RHS>* expr)
{ return expr->lhs * expr->rhs->_value; }

template <typename RHS>
double MulExpression_computevalue(MulExpression<double, RHS>* expr)
{ return expr->lhs * expr->rhs->_value; }



template <typename LHS, typename RHS>
inline MulExpression<LHS, RHS>* MulExpression_create(LHS lhs, RHS rhs)
{ return new MulExpression<LHS, RHS>(lhs, rhs); }


/* __mul__ */
extern "C" void* mul_expr_int(void* lhs, int rhs)
{
if (rhs == 1) {
    return lhs;
    }
Expression* tmp;
NumericValue* e = static_cast<NumericValue*>(lhs);
int _rhs = static_cast<int>(rhs);
if (e->is_variable()) {
    Variable* _lhs = static_cast<Variable*>(lhs);
    tmp = MulExpression_create(_lhs, _rhs);
    }
else {
    Expression* _lhs = static_cast<Expression*>(lhs);
    tmp = MulExpression_create(_lhs, _rhs);
    }
expressions.push_back(tmp);
return tmp;
}

extern "C" void* mul_expr_double(void* lhs, double rhs)
{
if (rhs == 1.0) {
    return lhs;
    }
Expression* tmp;
NumericValue* e = static_cast<NumericValue*>(lhs);
double _rhs = static_cast<double>(rhs);
if (e->is_variable()) {
    Variable* _lhs = static_cast<Variable*>(lhs);
    tmp = MulExpression_create(_lhs, _rhs);
    }
else {
    Expression* _lhs = static_cast<Expression*>(lhs);
    tmp = MulExpression_create(_lhs, _rhs);
    }
expressions.push_back(tmp);
return tmp;
}

void* mul_expr_expression(void* lhs, void* rhs)
{
Expression* tmp;
NumericValue* _lhs = static_cast<NumericValue*>(lhs);
NumericValue* _rhs = static_cast<NumericValue*>(rhs);
if (_lhs->is_variable()) {
    Variable* __lhs = static_cast<Variable*>(lhs);
    if (_rhs->is_variable()) {
        Variable* __rhs = static_cast<Variable*>(rhs);
        tmp = MulExpression_create(__lhs, __rhs);
        }
    else {
        Expression* __rhs = static_cast<Expression*>(rhs);
        tmp = MulExpression_create(__lhs, __rhs);
        }
    }
else {
    Expression* __lhs = static_cast<Expression*>(lhs);
    if (_rhs->is_variable()) {
        Variable* __rhs = static_cast<Variable*>(rhs);
        tmp = MulExpression_create(__lhs, __rhs);
        }
    else {
        Expression* __rhs = static_cast<Expression*>(rhs);
        tmp = MulExpression_create(__lhs, __rhs);
        }
    }
expressions.push_back(tmp);
return tmp;
}


/* __rmul__ */
extern "C" void* rmul_expr_int(int lhs, void* rhs)
{
if (lhs == 1) {
    return rhs;
    }
Expression* tmp;
NumericValue* e = static_cast<NumericValue*>(rhs);
if (e->is_variable()) {
    Variable* _rhs = static_cast<Variable*>(rhs);
    tmp = new MulExpression<int,Variable*>(lhs, _rhs);
    }
else {
    Expression* _rhs = static_cast<Expression*>(rhs);
    tmp = new MulExpression<int,Expression*>(lhs, _rhs);
    }
expressions.push_back(tmp);
return tmp;
}

extern "C" void* rmul_expr_double(double lhs, void* rhs)
{
if (lhs == 1.0) {
    return rhs;
    }
Expression* tmp;
NumericValue* e = static_cast<NumericValue*>(rhs);
if (e->is_variable()) {
    Variable* _rhs = static_cast<Variable*>(rhs);
    tmp = new MulExpression<double,Variable*>(lhs, _rhs);
    }
else {
    Expression* _rhs = static_cast<Expression*>(rhs);
    tmp = new MulExpression<double,Expression*>(lhs, _rhs);
    }
expressions.push_back(tmp);
return tmp;
}


/*** MISC ***/


extern "C" void print_var(void* var)
{
Variable* _var = static_cast<Variable*>(var);
_var->print(std::cout);
std::cout << std::endl;
}

extern "C" void print_expr(void* expr)
{
Expression* _expr = static_cast<Expression*>(expr);
_expr->print(std::cout);
std::cout << std::endl;
}

extern "C" int expr_size(void* expr)
{
NumericValue* _expr = static_cast<NumericValue*>(expr);
return _expr->size();
}

extern "C" void* create_parameter_int(int value, int mutable_flag)
{
Parameter* tmp = new TypedParameter<int>(value, mutable_flag);
parameters.push_back(tmp);
return tmp;
}

extern "C" void* create_parameter_double(double value, int mutable_flag)
{
Parameter* tmp = new TypedParameter<double>(value, mutable_flag);
parameters.push_back(tmp);
return tmp;
}

extern "C" void* create_variable(int binary, int integer)
{
Variable* tmp = new Variable(binary, integer);
variables.push_back(tmp);
return tmp;
}

extern "C" int get_variable_index(void* ptr)
{
Variable* v = static_cast<Variable*>(ptr);
return v->index;
}

extern "C" void set_variable_value(void* ptr, double val)
{
Variable* v = static_cast<Variable*>(ptr);
v->_value = val;
}

extern "C" void* create_inequality(void* expr)
{
NumericValue* _expr = static_cast<NumericValue*>(expr);
InequalityExpression* tmp = new InequalityExpression(_expr);
expressions.push_back(tmp);
return tmp;
}

extern "C" void* create_equality(void* expr)
{
NumericValue* _expr = static_cast<NumericValue*>(expr);
EqualityExpression* tmp = new EqualityExpression(_expr);
expressions.push_back(tmp);
return tmp;
}

extern "C" void* create_model()
{
Model* tmp = new Model();
models.push_back(tmp);
return tmp;
}

extern "C" void add_objective(void* model, void* expr)
{
Model* tmp = static_cast<Model*>(model);
Expression* _expr = static_cast<Expression*>(expr);
tmp->objectives.push_back(_expr);
}

extern "C" void add_inequality(void* model, void* ineq)
{
Model* tmp = static_cast<Model*>(model);
Expression* _ineq = static_cast<Expression*>(ineq);
tmp->inequalities.push_back(_ineq);
}

extern "C" void add_equality(void* model, void* eq)
{
Model* tmp = static_cast<Model*>(model);
Expression* _eq = static_cast<Expression*>(eq);
tmp->equalities.push_back(_eq);
}

extern "C" void print_model(void* expr)
{
Model* tmp = static_cast<Model*>(expr);
tmp->print(std::cout);
}

extern "C" void build_model(void* expr)
{
Model* tmp = static_cast<Model*>(expr);
tmp->build();
}

extern "C" double compute_objective(void* model, int i)
{
Model* tmp = static_cast<Model*>(model);
return tmp->_compute_f(i);
}


