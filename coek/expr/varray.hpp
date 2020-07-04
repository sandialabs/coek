#pragma once

//
// Defining the VariableArray class, which is currently used to
// allocate arrays of Variable objects quickly from Python.
//
#include <map>
#include <vector>
#include "coek/api/expression.hpp"


namespace coek {

template<typename T>
class MapKeyIterator : public T::const_iterator
{
public:
    typedef typename T::const_iterator base_t;

    MapKeyIterator() : base_t() {}
    MapKeyIterator(base_t iter) : base_t(iter) {}
    typename T::key_type* operator->()
    {
        return &(base_t::operator->()->first);
    }
    typename T::key_type& operator*()
    {
        return base_t::operator*().first;
    }
};


template<typename T>
class VecKeyIterator : public T::const_iterator
{
public:

    int curr;

    VecKeyIterator() : T::const_iterator() {curr=-1;}
    VecKeyIterator(typename T::const_iterator iter) 
        : T::const_iterator(iter)
        {curr=-1;}
    int* operator->()
    {
        curr++; return &curr;
    }
    int& operator*()
    {
        curr++; return curr;
    }
};


class VariableArray
{
private:

    void initialize(int n, double lb, double ub, double init,
                bool binary, bool integer, bool fixed);

public:

    std::vector<Variable> variables;
    std::string name;
    std::map<std::vector<int>,int> index_map;
    std::vector<int> dimen;
    std::vector<int> order;

public:

    VariableArray(std::vector<int>& _dimen, std::string _name, double lb, double ub, double init,
                bool binary, bool integer, bool fixed);
    VariableArray(int n, std::string _name, double lb, double ub, double init,
                bool binary, bool integer, bool fixed);

    Variable& operator[](int i)
        { return variables[i]; }

    Variable& get(int i)
        { return variables[i]; }

    Variable& get(std::vector<int>& index)
        { return variables[index_map[index]]; }

    std::string get_name();

    typedef MapKeyIterator<std::map<std::vector<int>,int> > map_key_t;
    typedef VecKeyIterator<std::vector<Variable> > vec_key_t;
    map_key_t indexed_begin() const { return map_key_t(index_map.cbegin()); }
    map_key_t indexed_end() const { return map_key_t(index_map.cend()); }
    vec_key_t unindexed_begin() const { return vec_key_t(variables.cbegin()); }
    vec_key_t unindexed_end() const { return vec_key_t(variables.cend()); }
};

}
