#pragma once

#include <list>
#include <memory>
#include <vector>

namespace coek {

class IndexParameter;
class ConcreteSet;
class Constraint;
class SequenceContextRepn;


/** Collect template arguments. */
inline void collect_index_parameters(std::vector<IndexParameter>& indices, const IndexParameter& arg)
    {
    indices.emplace_back(arg);
    }


/** Collect template arguments. */
template <typename... TYPES>
void collect_index_parameters(std::vector<IndexParameter>& indices, const IndexParameter& arg, const TYPES&... args)
    {
    indices.emplace_back(arg);
    collect_index_parameters(indices, args...);
    }


class Context
{
public:

    std::vector<IndexParameter> indices;

    ConcreteSet index_set;

    std::list<Constraint> index_values;
    std::list<Constraint> index_constraints;

public:

    Context(const std::vector<IndexParameter>& _indices);
};


class SequenceContext
{
public:

    std::shared_ptr<SequenceContextRepn> repn;

public:

    SequenceContext(const std::shared_ptr<SequenceContextRepn>& _repn);
    SequenceContext In(const ConcreteSet& _index_set);
    SequenceContext ST(const Constraint& con);
    SequenceContext Where(const Constraint& con);

    size_t size() const;

    Context& operator[](size_t i);

    /** \returns a modifier to specify the indices used in this expression */
    template <typename... TYPES>
    SequenceContext Forall(const TYPES&... args)
        {
        std::vector<IndexParameter> indices;
        collect_index_parameters(indices, args...);
        return Forall(indices);
        }


    /** \returns a modifier to specify the indices used in this expression */
    SequenceContext Forall(const std::vector<IndexParameter>& indices);

    ConcreteSet index_set() const;
};


/** \returns a modifier to specify the indices used in this expression */
SequenceContext Forall(const std::vector<IndexParameter>& indices);


/** \returns a modifier to specify the indices used in this expression */
template <typename... TYPES>
SequenceContext Forall(const TYPES&... args)
    {
    std::vector<IndexParameter> indices;
    collect_index_parameters(indices, args...);
    return Forall(indices);
    }
}
