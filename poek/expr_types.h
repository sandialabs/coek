
#include <cmath>
#include <iostream>


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

    virtual NumericValue* partial(unsigned int i) = 0;
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

    NumericValue* partial(unsigned int i);
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

extern TypedParameter<int> ZeroParameter;
extern TypedParameter<int> OneParameter;

inline NumericValue* Parameter::partial(unsigned int i)
{return &ZeroParameter;}



class Variable : public NumericValue
{
public:

    static double _nan;
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

    NumericValue* partial(unsigned int i)
        {return &OneParameter;}

};



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


bool variable_comparator(const Variable* lhs, const Variable* rhs);


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

    NumericValue* partial(unsigned int i)
        {return &OneParameter;}

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

    NumericValue* partial(unsigned int i) {return 0;}

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


/*
template <typename LHS, typename RHS>
NumericValue* MulExpression_partial<LHS,RHS>(unsigned int i, NumericValue* this)
{
if (i == 0)
    return rhs;
elif (i == 1)
    return lhs;
return 0;
}


template <typename LHS>
NumericValue* MulExpression_partial<LHS,int>(unsigned int i, NumericValue* this)
{
if (i == 0)
    return rhs;
elif (i == 1)
    return &ZeroParameter;
return 0;
}
*/
