//#include <iostream>
#include <cmath>
#include "varray.hpp"
#include "ast_term.hpp"


namespace coek {


class IndexedVariableTerm : public VariableTerm
{
public:

    int varray_index;
    VariableArray* varray;

public:

    IndexedVariableTerm(double _lb, double _ub, double _value, bool _binary, bool _integer, bool _fixed, int _i, VariableArray* _varray)
        : VariableTerm(_lb, _ub, _value, _binary, _integer)
        { varray_index=_i; varray=_varray; fixed=_fixed; }

    std::string get_name();
};


void initialize_index_map(std::vector<int>& dimen, std::vector<int>& tmp, std::size_t curr, std::size_t& ctr, std::map<std::vector<int>,int>& index_map)
{
if (curr == (dimen.size()-1)) {
    for (int i=0; i<dimen[curr]; i++) {
        tmp[curr] = i;
        index_map[tmp] = ctr++;
        }
    }
else {
    for (int i=0; i<dimen[curr]; i++) {
        tmp[curr] = i;
        initialize_index_map(dimen, tmp, curr+1, ctr, index_map);
        }
    }
}


std::string IndexedVariableTerm::get_name()
{
if (varray->name == "")
    return get_simple_name();

if (varray->order.size() == 0) {
    return varray->name + "[" + std::to_string(varray_index) + "]";
    }
else {
    std::string index_str;
    int total=varray_index;
    for (std::size_t i=0; i<varray->order.size(); i++) {
        int val= total / varray->order[i];
        total -= val*varray->order[i];
        if (i>0)
            index_str += ", ";
        index_str += std::to_string(val);
        }

    return varray->name + "[" + index_str + "]";
    }
}


VariableArray::VariableArray(std::vector<int>& _dimen, double init, double lb, double ub, 
                bool binary, bool integer, bool fixed, std::string _name)
{
dimen = _dimen;
//
//  Initialize variable array
//
int n=1;
for (auto it=dimen.begin(); it != dimen.end(); ++it)
    n *= *it;
name = _name;
initialize(n, init, lb, ub, binary, integer, fixed);
//
//  Initialize index_map
//
std::vector<int> tmp(dimen.size());
std::size_t ctr=0;
initialize_index_map(dimen, tmp, 0, ctr, index_map);
//
//  Compute index order
//
order.resize(dimen.size());
int i=dimen.size()-1;
int oval = 1;
while (i >= 0) {
    order[i] = oval;
    oval *= dimen[i--];
    }
}

VariableArray::VariableArray(int n, double lb, double ub, double init,
                bool binary, bool integer, bool fixed, std::string _name)
{
initialize(n, lb, ub, init, binary, integer, fixed);
name = _name;
}

void VariableArray::initialize(int n, double lb, double ub, double init,
            bool binary, bool integer, bool fixed)
{
if (std::isnan(lb))
    lb = -COEK_INFINITY;
if (std::isnan(ub))
    ub = COEK_INFINITY;

variables.resize(n);
for (int i=0; i<n; i++) {
    auto tmp = CREATE_POINTER(IndexedVariableTerm, lb, ub, init, binary, integer, fixed, i, this);
    tmp->index = ++VariableTerm::count;
    variables[i] = Variable(tmp);
    }
}

std::string VariableArray::get_name()
{
if (name == "")
    return "x";
else
    return name;
}

}
