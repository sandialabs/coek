#ifndef COEK_COMPACT_SEQUENCE_CONTExT_H
#define COEK_COMPACT_SEQUENCE_CONTExT_H
#pragma once

#include <list>
#include <vector>


namespace coek {

class IndexParameter;
class ConcreteSet;
class Constraint;


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

}
#endif
