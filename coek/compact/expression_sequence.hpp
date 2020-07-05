#ifndef COEK_COMPACT_EXPRESSION_SEQUENCE_H
#define COEK_COMPACT_EXPRESSION_SEQUENCE_H
#pragma once

#include <vector>
#include "coek/api/expression.hpp"

namespace coek {

class ExpressionSequenceRepn;
class ExpressionSeqIteratorRepn;
//class IndexParameter;
//class Expression;
class Constraint;


class ExpressionSeqIterator
{
public:

    std::shared_ptr<ExpressionSeqIteratorRepn> repn;

    typedef Expression* pointer;
    typedef const Expression* const_pointer;
    typedef Expression& reference;
    typedef const Expression& const_reference;

public:

    ExpressionSeqIterator();
    ExpressionSeqIterator(ExpressionSequenceRepn* seq, bool end=false);

    ExpressionSeqIterator& operator++();

    bool operator==(const ExpressionSeqIterator& other) const;
    bool operator!=(const ExpressionSeqIterator& other) const;

    reference operator*();
    const_reference operator*() const;
    pointer operator->();
    const_pointer operator->() const;
};


class ExpressionSequence
{
public:

  std::shared_ptr<ExpressionSequenceRepn> repn;

public:

    ExpressionSequence(const std::shared_ptr<ExpressionSequenceRepn>& _repn);

    template <typename... TYPES>
    ExpressionSequenceAux Forall(const TYPES&... args)
        {
        std::vector<IndexParameter> arg;
        collect_args(args..., arg);
        return Forall(arg);
        }
    
    ExpressionSequenceAux Forall(const std::vector<IndexParameter>& params);
    ExpressionSequence ST(const Constraint& con);
    ExpressionSequence Where(const Constraint& con);

    ExpressionSeqIterator begin();
    ExpressionSeqIterator end();

protected:
    
    void collect_args(const IndexParameter& arg, std::vector<IndexParameter>& _arg)
        {
        _arg.emplace_back(arg);
        }

    template <typename... TYPES>
    void collect_args(const IndexParameter& arg, const TYPES&... args, std::vector<IndexParameter>&  _arg)
        {
        _arg.emplace_back(arg);
        collect_args(args..., _arg);
        }
};


Expression Sum(const ExpressionSequence& seq);
}
#endif
