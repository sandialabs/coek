#pragma once

#include <assert.h>

#include <memory>
#include <vector>

#include "../ast/varray.hpp"
#include "coek/api/constraint.hpp"
#include "coek/api/expression.hpp"

namespace coek {

namespace rule {

class Expression {
   public:
    virtual coek::Expression value() = 0;
};

class Constant : public Expression {
   public:
    double val;

    Constant(double _val) : val(_val) {}

    coek::Expression value() { return coek::Expression(val); }
};

class IndexVariable : public Expression {
   public:
    coek::Parameter parameter;

    void set_value(unsigned int i) { parameter.value(i); }

    coek::Expression value() { return parameter; }
};

class VariableRef : public Expression {
   public:
    std::shared_ptr<PythonVariableArray> variable;
    std::shared_ptr<Expression> index;

   public:
    VariableRef(std::shared_ptr<PythonVariableArray> _varray, std::shared_ptr<Expression> _index)
        : variable(_varray), index(_index)
    {
    }

    coek::Expression value()
    {
        auto val = index->value();
        // TODO - fix?
        size_t _index = static_cast<size_t>(val.value());
        assert(_index < variable->variables.size());
        return variable->variables[_index];
    }
};

/*
class CompoundStatement : public Statement
{
public:
    std::vector<std::shared_ptr<Statement>> statement;
};

class Assignment : public Statement
{
public:

    std::shared_ptr<RuleVariable> variable;
    std::shared_ptr<Expression> value;
};
*/
}  // namespace rule

class IndexSet {
   public:
    std::vector<std::shared_ptr<rule::IndexVariable>> index;

    virtual void reset() = 0;
    virtual bool next() = 0;
};

namespace rule {

class RangeSet : public IndexSet {
   protected:
    unsigned int i;

   public:
    unsigned int N;

   public:
    RangeSet(unsigned int N_, std::shared_ptr<IndexVariable> i) : N(N_) { index.push_back(i); }

    void reset() { i = 0; }

    bool next()
    {
        i++;
        if (i == N)
            return false;
        index[0]->set_value(i);
        return true;
    }
};

#if 0
class Summation : public Statement
{
public:

    std::shared_ptr<SetExpression> set_expr;
    std::shared_ptr<Expression> arg;
    
    coek::Expression value();
};
#endif

class Plus : public Expression {
   public:
    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;

    Plus(std::shared_ptr<Expression> _lhs, std::shared_ptr<Expression> _rhs) : lhs(_lhs), rhs(_rhs)
    {
    }

    coek::Expression value() { return lhs->value() + rhs->value(); }
};

class Times : public Expression {
   public:
    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;

    Times(std::shared_ptr<Expression> _lhs, std::shared_ptr<Expression> _rhs) : lhs(_lhs), rhs(_rhs)
    {
    }

    coek::Expression value() { return lhs->value() * rhs->value(); }
};

}  // namespace rule

class ConstraintTemplate {
   public:
    std::shared_ptr<rule::Expression> body;

    ConstraintTemplate(std::shared_ptr<rule::Expression> _body) : body(_body) {}

    virtual coek::Constraint value() = 0;
};

namespace rule {

class Equals : public ConstraintTemplate {
   public:
    Equals(std::shared_ptr<Expression> _body) : ConstraintTemplate(_body) {}

    coek::Constraint value() { return body->value() == 0; }
};

}  // namespace rule
}  // namespace coek
