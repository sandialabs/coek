#include <cmath>
#include "varray.hpp"
#include "value_terms.hpp"


namespace coek {

namespace local {

class LocalIndexedVariableTerm : public VariableTerm
{
public:

    size_t varray_index;
    PythonVariableArray* varray;

public:

    //LocalIndexedVariableTerm(double _lb, double _ub, double _value, bool _binary, bool _integer, bool _fixed, size_t _i, VariableArray* _varray)
    LocalIndexedVariableTerm(const expr_pointer_t& _lb, const expr_pointer_t& _ub, const expr_pointer_t& _value, bool _binary, bool _integer, bool _fixed, size_t _i, PythonVariableArray* _varray)
        : VariableTerm(_lb, _ub, _value, _binary, _integer)
        { varray_index=_i; varray=_varray; fixed=_fixed; }

    std::string get_name();
};

void initialize_index_map(std::vector<size_t>& dimen, std::vector<size_t>& tmp, std::size_t curr, std::size_t& ctr, std::map<std::vector<size_t>,size_t>& index_map)
{
if (curr == (dimen.size()-1)) {
    for (size_t i=0; i<dimen[curr]; i++) {
        tmp[curr] = i;
        index_map[tmp] = ctr++;
        }
    }
else {
    for (size_t i=0; i<dimen[curr]; i++) {
        tmp[curr] = i;
        initialize_index_map(dimen, tmp, curr+1, ctr, index_map);
        }
    }
}


std::string LocalIndexedVariableTerm::get_name()
{
if (varray->name == "")
    return get_simple_name();

if (varray->order.size() == 0) {
    return varray->name + "[" + std::to_string(varray_index) + "]";
    }
else {
    std::string index_str;
    size_t total=varray_index;
    for (std::size_t i=0; i<varray->order.size(); i++) {
        size_t val= total / varray->order[i];
        total -= val*varray->order[i];
        if (i>0)
            index_str += ", ";
        index_str += std::to_string(val);
        }

    return varray->name + "[" + index_str + "]";
    }
}

}


PythonVariableArray::PythonVariableArray(std::vector<size_t>& _dimen, std::string _name, double init, double lb, double ub, bool binary, bool integer, bool fixed)
{
dimen = _dimen;
//
//  Initialize variable array
//
size_t n=1;
for (auto it=dimen.begin(); it != dimen.end(); ++it)
    n *= *it;
name = _name;
initialize(n, init, lb, ub, binary, integer, fixed);
//
//  Initialize index_map
//
std::vector<size_t> tmp(dimen.size());
std::size_t ctr=0;
local::initialize_index_map(dimen, tmp, 0, ctr, index_map);
//
//  Compute index order
//
order.resize(dimen.size());
size_t i=dimen.size()-1;
size_t oval = 1;
while (i > 0) {
    order[i] = oval;
    oval *= dimen[i--];
    }
order[i] = oval;
}

PythonVariableArray::PythonVariableArray(size_t n, std::string _name, double lb, double ub, double init,
                bool binary, bool integer, bool fixed)
{
initialize(n, lb, ub, init, binary, integer, fixed);
name = _name;
}

void PythonVariableArray::initialize(size_t n, double lb, double ub, double init,
            bool binary, bool integer, bool fixed)
{
if (std::isnan(lb))
    lb = -COEK_INFINITY;
if (std::isnan(ub))
    ub = COEK_INFINITY;

variables.resize(n);
for (size_t i=0; i<n; i++) {
    auto tmp = CREATE_POINTER(local::LocalIndexedVariableTerm, 
                    CREATE_POINTER(ConstantTerm,lb), 
                    CREATE_POINTER(ConstantTerm,ub), 
                    CREATE_POINTER(ConstantTerm,init), 
                    binary, integer, fixed, i, this);
    tmp->index = ++VariableTerm::count;
    variables[i] = Variable(tmp);
    }
}

std::string PythonVariableArray::get_name()
{
if (name == "")
    return "x";
else
    return name;
}

}
