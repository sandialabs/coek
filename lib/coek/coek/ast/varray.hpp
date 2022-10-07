#pragma once

//
// Defining the VariableArray class, which is currently used to
// allocate arrays of Variable objects quickly from Python.
//
#include <map>
#include <vector>

#include "coek/api/expression.hpp"

namespace coek {

template <typename T>
class MapKeyIterator : public T::const_iterator {
   public:
    typedef typename T::const_iterator base_t;

    MapKeyIterator() : base_t() {}
    MapKeyIterator(base_t iter) : base_t(iter) {}
    typename T::key_type* operator->() { return &(base_t::operator->()->first); }
    typename T::key_type& operator*() { return base_t::operator*().first; }
};

template <typename T>
class VecKeyIterator : public T::const_iterator {
   public:
    int curr;

    VecKeyIterator() : T::const_iterator() { curr = -1; }
    VecKeyIterator(typename T::const_iterator iter) : T::const_iterator(iter) { curr = -1; }
    int* operator->()
    {
        curr++;
        return &curr;
    }
    int& operator*()
    {
        curr++;
        return curr;
    }
};

class PythonVariableArray {
   private:
    void initialize(size_t n, double lb, double ub, double init, bool binary, bool integer,
                    bool fixed);

   public:
    std::vector<Variable> variables;
    std::string name;
    std::map<std::vector<size_t>, size_t> index_map;
    std::vector<size_t> dimen;
    std::vector<size_t> order;

   public:
    PythonVariableArray(std::vector<size_t>& _dimen, std::string _name, double lb, double ub,
                        double init, bool binary, bool integer, bool fixed);
    PythonVariableArray(size_t n, std::string _name, double lb, double ub, double init, bool binary,
                        bool integer, bool fixed);

    Variable& operator[](size_t i) { return variables[i]; }

    Variable& get(size_t i) { return variables[i]; }

    Variable& get(std::vector<size_t>& index) { return variables[index_map[index]]; }

    std::string get_name();

    typedef MapKeyIterator<std::map<std::vector<size_t>, size_t> > map_key_t;
    typedef VecKeyIterator<std::vector<Variable> > vec_key_t;
    map_key_t indexed_begin() const { return map_key_t(index_map.cbegin()); }
    map_key_t indexed_end() const { return map_key_t(index_map.cend()); }
    vec_key_t unindexed_begin() const { return vec_key_t(variables.cbegin()); }
    vec_key_t unindexed_end() const { return vec_key_t(variables.cend()); }
};

}  // namespace coek
