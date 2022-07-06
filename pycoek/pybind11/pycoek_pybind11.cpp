//
// pycoek:  A Python Module that wraps Coek objects
//

#include <iostream>
#include <typeinfo>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include "coek/ast/base_terms.hpp"
#include "coek/ast/value_terms.hpp"
//#include "coek/api/expression.hpp"
//#include "coek/api/intrinsic_fn.hpp"
//#include "coek/coek_model.hpp"
#include "coek/coek.hpp"

namespace py = pybind11;

PYBIND11_MAKE_OPAQUE(std::map<int,int>)


namespace coek {

py::list to_nested_list(std::list<std::string>::iterator& it, std::list<std::string>::iterator& end)
{
py::list tmp;

while (it != end) {
    if (*it == "]") {
        it++;
        return tmp;
        }
    else if (*it == "[") {
        it++;
        tmp.append( to_nested_list(it, end) );
        }
    else {
        tmp.append(*it);
        it++;
        }
    }
return tmp;
}

template <typename TYPE>
void parse_varargs(py::kwargs kwargs, const char* name, TYPE& lb, TYPE _default)
{
lb = _default;
try {
    auto _lb = kwargs[name];
    if (_lb.is_none())
        return;
    lb = _lb.cast<TYPE>();
    }
catch (std::exception& err) {
    }
}


#if 0
class VariableArray;


class _IndexedVariableTerm : public VariableTerm
{
public:

    int varray_index;
    VariableArray* varray;

public:

    _IndexedVariableTerm(double _lb, double _ub, double _value, bool _binary, bool _integer, bool _fixed, int _i, VariableArray* _varray)
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


template<typename K, typename V>
class MapKeyIterator : public std::map<K,V>::const_iterator
{
public:
    typedef typename std::map<K,V>::const_iterator T;

    MapKeyIterator() : T() {}
    MapKeyIterator(T iter) : T(iter) {}
    const K* operator->()
    {
        return &(T::operator->()->first);
    }
    const K& operator*()
    {
        return T::operator*().first;
    }
};

template<typename T>
class VecKeyIterator : public T
{
public:

    int curr;

    VecKeyIterator() : T() {curr=-1;}
    VecKeyIterator(T iter) : T(iter) {curr=-1;}
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
public:

    std::vector<Variable> variables;
    std::string name;
    std::map<std::vector<int>,int> index_map;
    std::vector<int> dimen;
    std::vector<int> order;

public:

    VariableArray(std::vector<int>& _dimen, py::kwargs kwargs)
        {
        dimen = _dimen;
        //
        //  Initialize variable array
        //
        int n=1;
        for (auto it=dimen.begin(); it != dimen.end(); ++it)
            n *= *it;
        initialize(n, kwargs);
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

    VariableArray(int n, py::kwargs kwargs)
        { initialize(n, kwargs); }

    void initialize(int n, py::kwargs kwargs)
        {
        double lb, ub, value;
        bool binary, integer, fixed;
        parse_varargs<double>(kwargs, "lb", lb, -COEK_INFINITY);
        parse_varargs<double>(kwargs, "ub", ub,  COEK_INFINITY);
        parse_varargs<double>(kwargs, "value", value, NAN);
        parse_varargs<bool>(kwargs, "binary", binary, false);
        parse_varargs<bool>(kwargs, "integer", integer, false);
        parse_varargs<bool>(kwargs, "fixed", fixed, false);
        if (kwargs.contains("name"))
            name = kwargs["name"].cast<py::str>();

        variables.resize(n);
        for (int i=0; i<n; i++) {
            auto tmp = CREATE_POINTER(_IndexedVariableTerm, lb, ub, value, binary, integer, fixed, i, this);
            variables[i] = Variable(tmp);
            }
        }

    Variable& get(int i)
        { return variables[i]; }

    Variable& get(std::vector<int>& index)
        { return variables[index_map[index]]; }

    std::string get_name()
        {
        if (name == "")
            return "x";
        else
            return name;
        }

    typedef MapKeyIterator<std::vector<int>,int> map_key_t;
    typedef VecKeyIterator<std::vector<Variable>::const_iterator> vec_key_t;
    map_key_t indexed_begin() const { return map_key_t(index_map.cbegin()); }
    map_key_t indexed_end() const { return map_key_t(index_map.cend()); }
    vec_key_t unindexed_begin() const { return vec_key_t(variables.cbegin()); }
    vec_key_t unindexed_end() const { return vec_key_t(variables.cend()); }
};


std::string _IndexedVariableTerm::get_name()
{
if (varray->dimen.size() == 0) {
    if (varray->name == "")
        return "x[" + std::to_string(index) + "]";
    else
        return varray->name + "[" + std::to_string(varray_index) + "]";
    }
else {
    std::string index_str;
    int total=varray_index;
    for (std::size_t i=0; i<varray->dimen.size(); i++) {
        int val= total / varray->order[i];
        //std::cerr << total << " " << varray->order[i] << " " << val << std::endl;
        total -= val*varray->order[i];
        if (i>0)
            index_str += ", ";
        index_str += std::to_string(val);
        }

    if (varray->name == "")
        return "x[" + index_str + "]";
    else
        return varray->name + "[" + index_str + "]";
    }
}


VariableArray* variable_fn(std::vector<int>& dimen, py::kwargs kwargs)
{ return new VariableArray(dimen, kwargs); }

VariableArray* variable_fn(int n, py::kwargs kwargs)
{ return new VariableArray(n, kwargs); }
#endif



template <class TYPE>
void set_kwargs(TYPE& var, py::kwargs kwargs)
{
double lb, ub, value;
bool binary, integer, fixed;
parse_varargs<double>(kwargs, "lb", lb, -COEK_INFINITY);
parse_varargs<double>(kwargs, "ub", ub,  COEK_INFINITY);
parse_varargs<double>(kwargs, "value", value, NAN);
parse_varargs<bool>(kwargs, "binary", binary, false);
parse_varargs<bool>(kwargs, "integer", integer, false);
parse_varargs<bool>(kwargs, "fixed", fixed, false);

try {
    var.value(value).bounds(lb, ub).fixed(fixed);
    if (binary)
        var.within(coek::Boolean);
    else if (integer)
        var.within(coek::Integers);
    if (kwargs.contains("name")) {
        auto _name = kwargs["name"];
        if (not _name.is_none()) {
            auto name = _name.cast<py::str>();
            var.name(name);
            }
        }
    }
catch (std::exception& err) {
    throw;
    //std::cerr << "HUH?" << std::endl;
    //std::cerr << typeid(err).name() << std::endl;
    }
}


VariableArray variable_fn(std::vector<int>& dimen, py::kwargs kwargs)
{
VariableArray tmp(dimen);
set_kwargs(tmp, kwargs);
return tmp;
}

VariableArray variable_fn(int n, py::kwargs kwargs)
{
if (n < 0)
    throw std::invalid_argument("Cannot initialize variable array with negative length");

VariableArray tmp(static_cast<size_t>(n));
set_kwargs(tmp, kwargs);
return tmp;
}

Variable variable_fn(py::kwargs kwargs)
{
Variable tmp;
set_kwargs(tmp, kwargs);
return tmp;
}

#if 0
#ifdef COEK_WITH_COMPACT_MODEL
coek::Expression ConcreteIndexedVariable_getitem(coek::ConcreteIndexedVariable& x, py::tuple args) {
                                std::vector<coek::refarg_types>& refarg = x.reftmp;
#if 0
                                std::vector<coek::set_types> setarg;
#else
                                coek::IndexVector& setarg = x.tmp;
#endif
                                size_t i=0;
                                size_t j=0;
                                for (py::handle h : args) {
                                    bool value=false;
                                    try {
                                        int tmp = h.cast<int>();
                                        //std::cout << "INT " << tmp << std::endl;
                                        refarg[j++] = tmp;
                                        setarg[i++] = tmp;
                                        value=true;
                                        }
                                    catch (std::exception& e) {
                                        }
#if 0
                                    if (!value) {
                                        try {
                                            double tmp = h.cast<double>();
                                            //std::cout << "DOUBLE " << tmp << std::endl;
                                            refarg.push_back(tmp);
                                            setarg.push_back(tmp);
                                            value=true;
                                            }
                                        catch (std::exception& e) {
                                            }
                                        }
#endif
                                    if (!value) {
                                        try {
                                            coek::Expression tmp = h.cast<coek::IndexParameter>();
                                            //std::cout << "REFARG " << tmp << std::endl;
                                            refarg[j++] = tmp.repn;
                                            value=true;
                                            }
                                        catch (std::exception& e) {
                                            }
                                        }
                                    if (!value) {
                                        try {
                                            coek::Expression tmp = h.cast<coek::Expression>();
                                            //std::cout << "EXPR " << std::endl;
                                            refarg[j++] = tmp.repn;
                                            value=true;
                                            }
                                        catch (std::exception& e) {
                                            }
                                        }
#if 0
                                    if (!value) {
                                        try {
                                            // Cast to strings last, so that we don't coerce to the class string description
                                            std::string tmp = h.cast<py::str>();
                                            //std::cout << "STRING " << tmp << std::endl;
                                            refarg.push_back(tmp);
                                            setarg.push_back(tmp);
                                            value=true;
                                            }
                                        catch (std::exception& e) {
                                            }
                                        }
#endif
                                    }
                                //std::cout << "HERE " << refarg.size() << " " << setarg.size() << std::endl;
                                if (j > i)
                                    return x.create_varref(refarg);
                                else
                                    return x.index(setarg);
                                }
#endif
#endif
}

#if 0
namespace {

const std::string Parameter_get_name(const coek::Parameter& param)
{ return param.name(); }

double Parameter_get_value(const coek::Parameter& param)
{ return param.value(); }

void Parameter_set_value(coek::Parameter& param, double value)
{ param.value(value); }


const std::string IndexParameter_get_name(const coek::IndexParameter& param)
{ return param.name(); }

double IndexParameter_get_value(const coek::IndexParameter& param)
{
double value;
auto flag = param.get_value(value);
if (flag)
    return value;
return NAN;
}

void IndexParameter_set_value(coek::IndexParameter& param, double value)
{ param.value(value); }


const std::string Variable_get_name(const coek::Variable& var)
{ return var.name(); }

double Variable_get_value(const coek::Variable& var)
{ return var.value(); }

void Variable_set_value(coek::Variable& var, double value)
{ var.value(value); }

double Variable_get_lb(const coek::Variable& var)
{ return var.lower(); }

void Variable_set_lb(coek::Variable& var, double value)
{ var.lower(value); }

double Variable_get_ub(const coek::Variable& var)
{ return var.upper(); }

void Variable_set_ub(coek::Variable& var, double value)
{ var.upper(value); }

bool Variable_get_fixed(const coek::Variable& var)
{ return var.fixed(); }

void Variable_set_fixed(coek::Variable& var, bool value)
{ var.fixed(value); }

}
#endif

PYBIND11_MODULE(pycoek_pybind11, m) {

    py::bind_map<std::map<int, int>>(m, "STLMapIntInt");

    m.doc() = "A Python module that wraps Coek";

    m.def("variable_",[](int n, py::kwargs kw) {return coek::variable_fn(n, kw);});
    m.def("variable_",[](std::vector<int>& dimen, py::kwargs kw) {return coek::variable_fn(dimen, kw);});
    m.def("variable_",[](py::kwargs kw) {return coek::variable_fn(kw);});
    m.def("affine_expression",[](std::vector<double>& coef, std::vector<coek::Variable>& var, double offset) {return affine_expression(coef, var, offset);});
    m.def("affine_expression",[](std::vector<double>& coef, std::vector<coek::Variable>& var) {return affine_expression(coef, var, 0);});
    m.def("affine_expression",[](std::vector<coek::Variable>& var, double offset) {return affine_expression(var, offset);});
    m.def("affine_expression",[](std::vector<coek::Variable>& var) {return affine_expression(var, 0);});

#ifdef COEK_WITH_COMPACT_MODEL
    m.def("SetOf",[](std::vector<int>& arg) {return coek::SetOf(arg);});
    m.def("RangeSet",[](int start, int stop, int step=1) {return coek::RangeSet(start, stop, step);});
    m.def("RangeSet",[](int start, int stop) {return coek::RangeSet(start, stop);});

    #if 0
    m.def("IndexedVariable",[](const coek::ConcreteSet& arg, double lb=-COEK_INFINITY, double ub=COEK_INFINITY, double value=0.0) {return coek::IndexedVariable(arg, lb, ub, value);});
    m.def("IndexedVariable",[](const coek::ConcreteSet& arg, double lb=-COEK_INFINITY, double ub=COEK_INFINITY) {return coek::IndexedVariable(arg, lb, ub);});
    m.def("IndexedVariable",[](const coek::ConcreteSet& arg) {return coek::IndexedVariable(arg);});
    m.def("IndexedVariable",[](const coek::ConcreteSet& arg, const std::string& name) {return coek::IndexedVariable(arg, name);});
    m.def("IndexedVariable",[](const coek::ConcreteSet& arg, double lb, double ub, double value, const std::string& name) {return coek::IndexedVariable(arg, lb, ub, value, name);});
    m.def("IndexedVariable",[](const coek::ConcreteSet& arg, double lb, double ub, double value, bool binary, bool integer, const std::string& name) {return coek::IndexedVariable(arg, lb, ub, value, binary, integer, name);});
    m.def("IndexedVariable",[](const coek::ConcreteSet& arg, double lb, double ub, double value, bool binary, bool integer) {return coek::IndexedVariable(arg, lb, ub, value, binary, integer);});
    #endif

    //
    // ConcreteSet
    //
    py::class_<coek::ConcreteSet>(m, "ConcreteSet")
        .def("dim",&coek::ConcreteSet::dim)
        .def("__add__", [](coek::ConcreteSet& x, coek::ConcreteSet& y){return x+y;})
        .def("__sub__", [](coek::ConcreteSet& x, coek::ConcreteSet& y){return x-y;})
        .def("__mul__", [](coek::ConcreteSet& x, coek::ConcreteSet& y){return x*y;})
        .def("__and__", [](coek::ConcreteSet& x, coek::ConcreteSet& y){return x & y;})
        .def("__or__", [](coek::ConcreteSet& x, coek::ConcreteSet& y){return x | y;})
        .def("__xor__", [](coek::ConcreteSet& x, coek::ConcreteSet& y){return x ^ y;})
        ;

    #if 0
    py::class_<coek::ConcreteIndexedVariable>(m, "ConcreteIndexedVariable")
        .def("__call__", [](coek::ConcreteIndexedVariable& x, py::args args) {return coek::ConcreteIndexedVariable_getitem(x,args);})
        .def("__getitem__", [](coek::ConcreteIndexedVariable& x, py::tuple args) {return coek::ConcreteIndexedVariable_getitem(x,args);})
        .def("__getitem__", [](coek::ConcreteIndexedVariable& x, py::args args) {return coek::ConcreteIndexedVariable_getitem(x,args);})
        ;
    #endif
#endif

    //
    // Parameter
    //
    py::class_<coek::Parameter>(m, "parameter")
        .def(py::init<>())
        .def(py::init<const std::string&>())
        .def("get_name",[](const coek::Parameter& x){return x.name();})
        .def_property_readonly("name",[](const coek::Parameter& x){return x.name();})
        .def_property("value", [](const coek::Parameter& x){return x.value();}, [](coek::Parameter& x, double value){x.value(value);})
        .def("is_constraint",[](const coek::Parameter& ){return false;})

        .def("__neg__", [](const coek::Parameter& x){return -x;})
        .def("__pos__", [](const coek::Parameter& x){return +x;})

        .def("__add__", [](const coek::Parameter& x, const coek::Expression& y){return x+y;})
        .def("__add__", [](const coek::Parameter& x, const coek::Variable& y){return x+y;})
        .def("__add__", [](const coek::Parameter& x, const coek::Parameter& y){return x+y;})
        .def("__add__", [](const coek::Parameter& x, const coek::IndexParameter& y){return x+y;})
        .def(py::self + int())
        .def(py::self + double())
        .def("__radd__", [](const coek::Parameter& x, int y){return y+x;})
        .def("__radd__", [](const coek::Parameter& x, double y){return y+x;})

        .def("__sub__", [](const coek::Parameter& x, const coek::Expression& y){return x-y;})
        .def("__sub__", [](const coek::Parameter& x, const coek::Variable& y){return x-y;})
        .def("__sub__", [](const coek::Parameter& x, const coek::Parameter& y){return x-y;})
        .def("__sub__", [](const coek::Parameter& x, const coek::IndexParameter& y){return x-y;})
        .def(py::self - int())
        .def(py::self - double())
        .def("__rsub__", [](const coek::Parameter& x, int y){return y-x;})
        .def("__rsub__", [](const coek::Parameter& x, double y){return y-x;})

        .def("__mul__", [](const coek::Parameter& x, const coek::Expression& y){return x*y;})
        .def("__mul__", [](const coek::Parameter& x, const coek::Variable& y){return x*y;})
        .def("__mul__", [](const coek::Parameter& x, const coek::Parameter& y){return x*y;})
        .def("__mul__", [](const coek::Parameter& x, const coek::IndexParameter& y){return x*y;})
        .def(py::self * int())
        .def(py::self * double())
        .def("__rmul__", [](const coek::Parameter& x, int y){return y*x;})
        .def("__rmul__", [](const coek::Parameter& x, double y){return y*x;})

        .def("__truediv__", [](const coek::Parameter& x, const coek::Expression& y){return x/y;})
        .def("__truediv__", [](const coek::Parameter& x, const coek::Variable& y){return x/y;})
        .def("__truediv__", [](const coek::Parameter& x, const coek::Parameter& y){return x/y;})
        .def("__truediv__", [](const coek::Parameter& x, const coek::IndexParameter& y){return x/y;})
        .def(py::self / int())
        .def(py::self / double())
        .def("__rtruediv__", [](const coek::Parameter& x, int y){return y/x;})
        .def("__rtruediv__", [](const coek::Parameter& x, double y){return y/x;})

        .def("__pow__", [](const coek::Parameter& x, const coek::Expression& y){return pow(x,y);})
        .def("__pow__", [](const coek::Parameter& x, const coek::Variable& y){return pow(x,y);})
        .def("__pow__", [](const coek::Parameter& x, const coek::Parameter& y){return pow(x,y);})
        .def("__pow__", [](const coek::Parameter& x, const coek::IndexParameter& y){return pow(x,y);})
        .def("__pow__", [](const coek::Parameter& x, int y){return pow(x,y);})
        .def("__pow__", [](const coek::Parameter& x, double y){return pow(x,y);})
        .def("__rpow__", [](const coek::Parameter& x, int y){return pow(y,x);})
        .def("__rpow__", [](const coek::Parameter& x, double y){return pow(y,x);})

        .def("__lt__", [](const coek::Parameter& x, const coek::Expression& y){return x<y;})
        .def("__lt__", [](const coek::Parameter& x, const coek::Variable& y){return x<y;})
        .def("__lt__", [](const coek::Parameter& x, const coek::Parameter& y){return x<y;})
        .def("__lt__", [](const coek::Parameter& x, const coek::IndexParameter& y){return x<y;})
        .def(py::self < int())
        .def(py::self < double())

        .def("__le__", [](const coek::Parameter& x, const coek::Expression& y){return x<=y;})
        .def("__le__", [](const coek::Parameter& x, const coek::Variable& y){return x<=y;})
        .def("__le__", [](const coek::Parameter& x, const coek::Parameter& y){return x<=y;})
        .def("__le__", [](const coek::Parameter& x, const coek::IndexParameter& y){return x<=y;})
        .def(py::self <= int())
        .def(py::self <= double())

        .def("__gt__", [](const coek::Parameter& x, const coek::Expression& y){return x>y;})
        .def("__gt__", [](const coek::Parameter& x, const coek::Variable& y){return x>y;})
        .def("__gt__", [](const coek::Parameter& x, const coek::Parameter& y){return x>y;})
        .def("__gt__", [](const coek::Parameter& x, const coek::IndexParameter& y){return x>y;})
        .def(py::self > int())
        .def(py::self > double())

        .def("__ge__", [](const coek::Parameter& x, const coek::Expression& y){return x>=y;})
        .def("__ge__", [](const coek::Parameter& x, const coek::Variable& y){return x>=y;})
        .def("__ge__", [](const coek::Parameter& x, const coek::Parameter& y){return x>=y;})
        .def("__ge__", [](const coek::Parameter& x, const coek::IndexParameter& y){return x>=y;})
        .def(py::self >= int())
        .def(py::self >= double())

        .def("__eq__", [](const coek::Parameter& x, const coek::Expression& y){return x==y;})
        .def("__eq__", [](const coek::Parameter& x, const coek::Variable& y){return x==y;})
        .def("__eq__", [](const coek::Parameter& x, const coek::Parameter& y){return x==y;})
        .def("__eq__", [](const coek::Parameter& x, const coek::IndexParameter& y){return x==y;})
        .def(py::self == int())
        .def(py::self == double())

        .def("__abs__", [](coek::Parameter& x){return abs(x);})
        ;

    //
    // IndexParameter
    //
    py::class_<coek::IndexParameter>(m, "index")
        .def(py::init<const std::string&>())
        .def("get_name",[](const coek::IndexParameter& x){return x.name();})
        .def("get_value",[](const coek::IndexParameter& x)
                {
                double value=0;
                auto flag = x.get_value(value);
                if (flag)
                    return (double)NAN;
                return value;
                })
        .def("set_value",[](coek::IndexParameter& x, double value){x.value(value);})
        .def("is_constraint",[](const coek::IndexParameter& ){return false;})

        .def("__neg__", [](const coek::IndexParameter& x){return -x;})
        .def("__pos__", [](const coek::IndexParameter& x){return +x;})

        .def("__add__", [](const coek::IndexParameter& x, const coek::Expression& y){return x+y;})
        .def("__add__", [](const coek::IndexParameter& x, const coek::Variable& y){return x+y;})
        .def("__add__", [](const coek::IndexParameter& x, const coek::Parameter& y){return x+y;})
        .def("__add__", [](const coek::IndexParameter& x, const coek::IndexParameter& y){return x+y;})
        .def(py::self + int())
        .def(py::self + double())
        .def("__radd__", [](const coek::IndexParameter& x, int y){return y+x;})
        .def("__radd__", [](const coek::IndexParameter& x, double y){return y+x;})

        .def("__sub__", [](const coek::IndexParameter& x, const coek::Expression& y){return x-y;})
        .def("__sub__", [](const coek::IndexParameter& x, const coek::Variable& y){return x-y;})
        .def("__sub__", [](const coek::IndexParameter& x, const coek::Parameter& y){return x-y;})
        .def("__sub__", [](const coek::IndexParameter& x, const coek::IndexParameter& y){return x-y;})
        .def(py::self - int())
        .def(py::self - double())
        .def("__rsub__", [](const coek::IndexParameter& x, int y){return y-x;})
        .def("__rsub__", [](const coek::IndexParameter& x, double y){return y-x;})

        .def("__mul__", [](const coek::IndexParameter& x, const coek::Expression& y){return x*y;})
        .def("__mul__", [](const coek::IndexParameter& x, const coek::Variable& y){return x*y;})
        .def("__mul__", [](const coek::IndexParameter& x, const coek::Parameter& y){return x*y;})
        .def("__mul__", [](const coek::IndexParameter& x, const coek::IndexParameter& y){return x*y;})
        .def(py::self * int())
        .def(py::self * double())
        .def("__rmul__", [](const coek::IndexParameter& x, int y){return y*x;})
        .def("__rmul__", [](const coek::IndexParameter& x, double y){return y*x;})

        .def("__truediv__", [](const coek::IndexParameter& x, const coek::Expression& y){return x/y;})
        .def("__truediv__", [](const coek::IndexParameter& x, const coek::Variable& y){return x/y;})
        .def("__truediv__", [](const coek::IndexParameter& x, const coek::Parameter& y){return x/y;})
        .def("__truediv__", [](const coek::IndexParameter& x, const coek::IndexParameter& y){return x/y;})
        .def(py::self / int())
        .def(py::self / double())
        .def("__rtruediv__", [](const coek::IndexParameter& x, int y){return y/x;})
        .def("__rtruediv__", [](const coek::IndexParameter& x, double y){return y/x;})

        .def("__pow__", [](const coek::IndexParameter& x, const coek::Expression& y){return pow(x,y);})
        .def("__pow__", [](const coek::IndexParameter& x, const coek::Variable& y){return pow(x,y);})
        .def("__pow__", [](const coek::IndexParameter& x, const coek::Parameter& y){return pow(x,y);})
        .def("__pow__", [](const coek::IndexParameter& x, const coek::IndexParameter& y){return pow(x,y);})
        .def("__pow__", [](const coek::IndexParameter& x, int y){return pow(x,y);})
        .def("__pow__", [](const coek::IndexParameter& x, double y){return pow(x,y);})
        .def("__rpow__", [](const coek::IndexParameter& x, int y){return pow(y,x);})
        .def("__rpow__", [](const coek::IndexParameter& x, double y){return pow(y,x);})

        .def("__lt__", [](const coek::IndexParameter& x, const coek::Expression& y){return x<y;})
        .def("__lt__", [](const coek::IndexParameter& x, const coek::Variable& y){return x<y;})
        .def("__lt__", [](const coek::IndexParameter& x, const coek::Parameter& y){return x<y;})
        .def("__lt__", [](const coek::IndexParameter& x, const coek::IndexParameter& y){return x<y;})
        .def(py::self < int())
        .def(py::self < double())

        .def("__le__", [](const coek::IndexParameter& x, const coek::Expression& y){return x<=y;})
        .def("__le__", [](const coek::IndexParameter& x, const coek::Variable& y){return x<=y;})
        .def("__le__", [](const coek::IndexParameter& x, const coek::Parameter& y){return x<=y;})
        .def("__le__", [](const coek::IndexParameter& x, const coek::IndexParameter& y){return x<=y;})
        .def(py::self <= int())
        .def(py::self <= double())

        .def("__gt__", [](const coek::IndexParameter& x, const coek::Expression& y){return x>y;})
        .def("__gt__", [](const coek::IndexParameter& x, const coek::Variable& y){return x>y;})
        .def("__gt__", [](const coek::IndexParameter& x, const coek::Parameter& y){return x>y;})
        .def("__gt__", [](const coek::IndexParameter& x, const coek::IndexParameter& y){return x>y;})
        .def(py::self > int())
        .def(py::self > double())

        .def("__ge__", [](const coek::IndexParameter& x, const coek::Expression& y){return x>=y;})
        .def("__ge__", [](const coek::IndexParameter& x, const coek::Variable& y){return x>=y;})
        .def("__ge__", [](const coek::IndexParameter& x, const coek::Parameter& y){return x>=y;})
        .def("__ge__", [](const coek::IndexParameter& x, const coek::IndexParameter& y){return x>=y;})
        .def(py::self >= int())
        .def(py::self >= double())

        .def("__eq__", [](const coek::IndexParameter& x, const coek::Expression& y){return x==y;})
        .def("__eq__", [](const coek::IndexParameter& x, const coek::Variable& y){return x==y;})
        .def("__eq__", [](const coek::IndexParameter& x, const coek::Parameter& y){return x==y;})
        .def("__eq__", [](const coek::IndexParameter& x, const coek::IndexParameter& y){return x==y;})
        .def(py::self == int())
        .def(py::self == double())

        .def("__abs__", [](coek::IndexParameter& x){return abs(x);})
        ;

    //
    // Variable
    //
    py::class_<coek::Variable>(m, "variable_single")
        .def(py::init<>())
        .def("get_name",[](const coek::Variable& x){return x.name();})
        .def_property_readonly("name",[](const coek::Variable& x){return x.name();})
        .def_property("value", [](const coek::Variable& x){return x.value();}, [](coek::Variable& x, double value){x.value(value);})
        .def_property("lb", [](const coek::Variable& x){return x.lower();}, [](coek::Variable& x, double value){x.lower(value);})
        .def_property("ub", [](const coek::Variable& x){return x.upper();}, [](coek::Variable& x, double value){x.upper(value);})
        .def_property("fixed", [](const coek::Variable& x){return x.fixed();}, [](coek::Variable& x, bool value){x.fixed(value);})
        .def_property_readonly("id", &coek::Variable::id)
        .def("is_constraint",[](const coek::Variable& ){return false;})

        .def("__neg__", [](const coek::Variable& x){return -x;})
        .def("__pos__", [](const coek::Variable& x){return +x;})

        .def("__add__", [](const coek::Variable& x, const coek::Expression& y){return x+y;})
        .def("__add__", [](const coek::Variable& x, const coek::Variable& y){return x+y;})
        .def("__add__", [](const coek::Variable& x, const coek::Parameter& y){return x+y;})
        .def("__add__", [](const coek::Variable& x, const coek::IndexParameter& y){return x+y;})
        .def(py::self + int())
        .def(py::self + double())
        .def("__radd__", [](const coek::Variable& x, int y){return y+x;})
        .def("__radd__", [](const coek::Variable& x, double y){return y+x;})

        .def("__sub__", [](const coek::Variable& x, const coek::Expression& y){return x-y;})
        .def("__sub__", [](const coek::Variable& x, const coek::Variable& y){return x-y;})
        .def("__sub__", [](const coek::Variable& x, const coek::Parameter& y){return x-y;})
        .def("__sub__", [](const coek::Variable& x, const coek::IndexParameter& y){return x-y;})
        .def(py::self - int())
        .def(py::self - double())
        .def("__rsub__", [](const coek::Variable& x, int y){return y-x;})
        .def("__rsub__", [](const coek::Variable& x, double y){return y-x;})

        .def("__mul__", [](const coek::Variable& x, const coek::Expression& y){return x*y;})
        .def("__mul__", [](const coek::Variable& x, const coek::Variable& y){return x*y;})
        .def("__mul__", [](const coek::Variable& x, const coek::Parameter& y){return x*y;})
        .def("__mul__", [](const coek::Variable& x, const coek::IndexParameter& y){return x*y;})
        .def(py::self * int())
        .def(py::self * double())
        .def("__rmul__", [](const coek::Variable& x, int y){return y*x;})
        .def("__rmul__", [](const coek::Variable& x, double y){return y*x;})

        .def("__truediv__", [](const coek::Variable& x, const coek::Expression& y){return x/y;})
        .def("__truediv__", [](const coek::Variable& x, const coek::Variable& y){return x/y;})
        .def("__truediv__", [](const coek::Variable& x, const coek::Parameter& y){return x/y;})
        .def("__truediv__", [](const coek::Variable& x, const coek::IndexParameter& y){return x/y;})
        .def(py::self / int())
        .def(py::self / double())
        .def("__rtruediv__", [](const coek::Variable& x, int y){return y/x;})
        .def("__rtruediv__", [](const coek::Variable& x, double y){return y/x;})

        .def("__pow__", [](const coek::Variable& x, const coek::Expression& y){return pow(x,y);})
        .def("__pow__", [](const coek::Variable& x, const coek::Variable& y){return pow(x,y);})
        .def("__pow__", [](const coek::Variable& x, const coek::Parameter& y){return pow(x,y);})
        .def("__pow__", [](const coek::Variable& x, const coek::IndexParameter& y){return pow(x,y);})
        .def("__pow__", [](const coek::Variable& x, int y){return pow(x,y);})
        .def("__pow__", [](const coek::Variable& x, double y){return pow(x,y);})
        .def("__rpow__", [](const coek::Variable& x, int y){return pow(y,x);})
        .def("__rpow__", [](const coek::Variable& x, double y){return pow(y,x);})

        .def("__lt__", [](const coek::Variable& x, const coek::Expression& y){return x<y;})
        .def("__lt__", [](const coek::Variable& x, const coek::Variable& y){return x<y;})
        .def("__lt__", [](const coek::Variable& x, const coek::Parameter& y){return x<y;})
        .def("__lt__", [](const coek::Variable& x, const coek::IndexParameter& y){return x<y;})
        .def(py::self < int())
        .def(py::self < double())

        .def("__le__", [](const coek::Variable& x, const coek::Expression& y){return x<=y;})
        .def("__le__", [](const coek::Variable& x, const coek::Variable& y){return x<=y;})
        .def("__le__", [](const coek::Variable& x, const coek::Parameter& y){return x<=y;})
        .def("__le__", [](const coek::Variable& x, const coek::IndexParameter& y){return x<=y;})
        .def(py::self <= int())
        .def(py::self <= double())

        .def("__gt__", [](const coek::Variable& x, const coek::Expression& y){return x>y;})
        .def("__gt__", [](const coek::Variable& x, const coek::Variable& y){return x>y;})
        .def("__gt__", [](const coek::Variable& x, const coek::Parameter& y){return x>y;})
        .def("__gt__", [](const coek::Variable& x, const coek::IndexParameter& y){return x>y;})
        .def(py::self > int())
        .def(py::self > double())

        .def("__ge__", [](const coek::Variable& x, const coek::Expression& y){return x>=y;})
        .def("__ge__", [](const coek::Variable& x, const coek::Variable& y){return x>=y;})
        .def("__ge__", [](const coek::Variable& x, const coek::Parameter& y){return x>=y;})
        .def("__ge__", [](const coek::Variable& x, const coek::IndexParameter& y){return x>=y;})
        .def(py::self >= int())
        .def(py::self >= double())

        .def("__eq__", [](const coek::Variable& x, const coek::Expression& y){return x==y;})
        .def("__eq__", [](const coek::Variable& x, const coek::Variable& y){return x==y;})
        .def("__eq__", [](const coek::Variable& x, const coek::Parameter& y){return x==y;})
        .def("__eq__", [](const coek::Variable& x, const coek::IndexParameter& y){return x==y;})
        .def(py::self == int())
        .def(py::self == double())

        .def("__abs__", [](coek::Variable& x){return abs(x);})

        //.def("__bool__", [](coek::Variable& x){return x.get_value() != 0;})
        //.def("__int__", [](coek::Variable& x){return int(x.get_value());})
        //.def("__float__", [](coek::Variable& x){return x.get_value();})
        ;

    //
    // VariableArray
    //
    py::class_<coek::VariableArray>(m, "variable_array")
        .def(py::init<>())
        //.def(py::init<std::vector<int>&, py::kwargs>())
        .def("__getitem__", [](coek::VariableArray& va, int i){return va(i);})
        .def("__getitem__", [](coek::VariableArray& va, std::vector<int>& index)
                {
                coek::IndexVector::vecdata_t *data = new coek::IndexVector::vecdata_t [index.size()];
                coek::IndexVector tmp(data, index.size());
                for (size_t i=0; i<index.size(); ++i)
                    tmp[i] = static_cast<size_t>(index[i]);
                return va.index(tmp);
                })
        .def_property_readonly("name", [](coek::VariableArray& ) -> py::object {
                return py::cast<std::string>("x");
                #if 0
                if (x.name().size() == 0)
                    return py::cast<std::string>("x");
                else
                    return py::cast<std::string>(x.name());
                #endif
                })
        .def("is_constraint",[](const coek::VariableArray& ){return false;})
        .def("__iter__", [](const coek::VariableArray& /*va*/) 
                {
                #if 0
                // TODO - revisit this
                if (va.dim() == 0)
                     return py::make_iterator(va.unindexed_begin(), va.unindexed_end());
                else
                     return py::make_iterator(va.indexed_begin(), va.indexed_end());
                #endif
                },
                py::keep_alive<0, 1>())
        ;

    //
    // Expression
    //
    py::class_<coek::Expression>(m, "expression")
        .def(py::init<double>())
        .def(py::init<int>())
        .def(py::init<coek::Parameter&>())
        .def(py::init<coek::IndexParameter&>())
        .def(py::init<coek::Variable&>())
        .def_property_readonly("value", &coek::Expression::value)
        .def("is_constraint",[](const coek::Expression& ){return false;})

        .def("__neg__", [](const coek::Expression& x){return -x;})
        .def("__pos__", [](const coek::Expression& x){return +x;})

        .def("__add__", [](const coek::Expression& x, const coek::Expression& y){return x+y;})
        .def("__add__", [](const coek::Expression& x, const coek::Variable& y){return x+y;})
        .def("__add__", [](const coek::Expression& x, const coek::Parameter& y){return x+y;})
        .def("__add__", [](const coek::Expression& x, const coek::IndexParameter& y){return x+y;})
        .def(py::self + int())
        .def(py::self + double())
        .def("__radd__", [](const coek::Expression& x, int y){return y+x;})
        .def("__radd__", [](const coek::Expression& x, double y){return y+x;})

        .def("__sub__", [](const coek::Expression& x, const coek::Expression& y){return x-y;})
        .def("__sub__", [](const coek::Expression& x, const coek::Variable& y){return x-y;})
        .def("__sub__", [](const coek::Expression& x, const coek::Parameter& y){return x-y;})
        .def("__sub__", [](const coek::Expression& x, const coek::IndexParameter& y){return x-y;})
        .def(py::self - int())
        .def(py::self - double())
        .def("__rsub__", [](const coek::Expression& x, int y){return y-x;})
        .def("__rsub__", [](const coek::Expression& x, double y){return y-x;})

        .def("__mul__", [](const coek::Expression& x, const coek::Expression& y){return x*y;})
        .def("__mul__", [](const coek::Expression& x, const coek::Variable& y){return x*y;})
        .def("__mul__", [](const coek::Expression& x, const coek::Parameter& y){return x*y;})
        .def("__mul__", [](const coek::Expression& x, const coek::IndexParameter& y){return x*y;})
        .def(py::self * int())
        .def(py::self * double())
        .def("__rmul__", [](const coek::Expression& x, int y){return y*x;})
        .def("__rmul__", [](const coek::Expression& x, double y){return y*x;})

        .def("__truediv__", [](const coek::Expression& x, const coek::Expression& y){return x/y;})
        .def("__truediv__", [](const coek::Expression& x, const coek::Variable& y){return x/y;})
        .def("__truediv__", [](const coek::Expression& x, const coek::Parameter& y){return x/y;})
        .def("__truediv__", [](const coek::Expression& x, const coek::IndexParameter& y){return x/y;})
        .def(py::self / int())
        .def(py::self / double())
        .def("__rtruediv__", [](const coek::Expression& x, int y){return y/x;})
        .def("__rtruediv__", [](const coek::Expression& x, double y){return y/x;})

        .def("__pow__", [](const coek::Expression& x, const coek::Expression& y){return pow(x,y);})
        .def("__pow__", [](const coek::Expression& x, const coek::Variable& y){return pow(x,y);})
        .def("__pow__", [](const coek::Expression& x, const coek::Parameter& y){return pow(x,y);})
        .def("__pow__", [](const coek::Expression& x, const coek::IndexParameter& y){return pow(x,y);})
        .def("__pow__", [](const coek::Expression& x, int y){return pow(x,y);})
        .def("__pow__", [](const coek::Expression& x, double y){return pow(x,y);})
        .def("__rpow__", [](const coek::Expression& x, int y){return pow(y,x);})
        .def("__rpow__", [](const coek::Expression& x, double y){return pow(y,x);})

        .def("__lt__", [](const coek::Expression& x, const coek::Expression& y){return x<y;})
        .def("__lt__", [](const coek::Expression& x, const coek::Variable& y){return x<y;})
        .def("__lt__", [](const coek::Expression& x, const coek::Parameter& y){return x<y;})
        .def("__lt__", [](const coek::Expression& x, const coek::IndexParameter& y){return x<y;})
        .def(py::self < int())
        .def(py::self < double())

        .def("__le__", [](const coek::Expression& x, const coek::Expression& y){return x<=y;})
        .def("__le__", [](const coek::Expression& x, const coek::Variable& y){return x<=y;})
        .def("__le__", [](const coek::Expression& x, const coek::Parameter& y){return x<=y;})
        .def("__le__", [](const coek::Expression& x, const coek::IndexParameter& y){return x<=y;})
        .def(py::self <= int())
        .def(py::self <= double())

        .def("__gt__", [](const coek::Expression& x, const coek::Expression& y){return x>y;})
        .def("__gt__", [](const coek::Expression& x, const coek::Variable& y){return x>y;})
        .def("__gt__", [](const coek::Expression& x, const coek::Parameter& y){return x>y;})
        .def("__gt__", [](const coek::Expression& x, const coek::IndexParameter& y){return x>y;})
        .def(py::self > int())
        .def(py::self > double())

        .def("__ge__", [](const coek::Expression& x, const coek::Expression& y){return x>=y;})
        .def("__ge__", [](const coek::Expression& x, const coek::Variable& y){return x>=y;})
        .def("__ge__", [](const coek::Expression& x, const coek::Parameter& y){return x>=y;})
        .def("__ge__", [](const coek::Expression& x, const coek::IndexParameter& y){return x>=y;})
        .def(py::self >= int())
        .def(py::self >= double())

        .def("__eq__", [](const coek::Expression& x, const coek::Expression& y){return x==y;})
        .def("__eq__", [](const coek::Expression& x, const coek::Variable& y){return x==y;})
        .def("__eq__", [](const coek::Expression& x, const coek::Parameter& y){return x==y;})
        .def("__eq__", [](const coek::Expression& x, const coek::IndexParameter& y){return x==y;})
        .def(py::self == int())
        .def(py::self == double())

        .def("__iadd__", [](coek::Expression& x, const coek::Expression& y){return x += y;})
        .def("__iadd__", [](coek::Expression& x, const coek::Variable& y){return x += y;})
        .def("__iadd__", [](coek::Expression& x, const coek::Parameter& y){return x += y;})
        .def("__iadd__", [](coek::Expression& x, const coek::IndexParameter& y){return x += y;})
        .def("__iadd__", [](coek::Expression& x, int y){return x += y;})
        .def("__iadd__", [](coek::Expression& x, double y){return x += y;})

        .def("__isub__", [](coek::Expression& x, const coek::Expression& y){return x -= y;})
        .def("__isub__", [](coek::Expression& x, const coek::Variable& y){return x -= y;})
        .def("__isub__", [](coek::Expression& x, const coek::Parameter& y){return x -= y;})
        .def("__isub__", [](coek::Expression& x, const coek::IndexParameter& y){return x -= y;})
        .def("__isub__", [](coek::Expression& x, int y){return x -= y;})
        .def("__isub__", [](coek::Expression& x, double y){return x -= y;})

        .def("__imul__", [](coek::Expression& x, const coek::Expression& y){return x *= y;})
        .def("__imul__", [](coek::Expression& x, const coek::Variable& y){return x *= y;})
        .def("__imul__", [](coek::Expression& x, const coek::Parameter& y){return x *= y;})
        .def("__imul__", [](coek::Expression& x, const coek::IndexParameter& y){return x *= y;})
        .def("__imul__", [](coek::Expression& x, int y){return x *= y;})
        .def("__imul__", [](coek::Expression& x, double y){return x *= y;})

        .def("__itruediv__", [](coek::Expression& x, const coek::Expression& y){return x /= y;})
        .def("__itruediv__", [](coek::Expression& x, const coek::Variable& y){return x /= y;})
        .def("__itruediv__", [](coek::Expression& x, const coek::Parameter& y){return x /= y;})
        .def("__itruediv__", [](coek::Expression& x, const coek::IndexParameter& y){return x /= y;})
        .def("__itruediv__", [](coek::Expression& x, int y){return x /= y;})
        .def("__itruediv__", [](coek::Expression& x, double y){return x /= y;})

        .def("__abs__", [](coek::Expression& x){return coek::abs(x);})

        //.def("__bool__", [](coek::Expression& x){return x.get_value() != 0;})
        //.def("__int__", [](coek::Expression& x){return int(x.get_value());})
        //.def("__float__", [](coek::Expression& x){return x.get_value();})

        .def("to_list", [](coek::Expression& x){
                                auto tmp = x.to_list();
                                auto begin = tmp.begin();
                                auto end = tmp.end();
                                if (begin == end)
                                    return py::list();
                                if (tmp.size() == 1) {
                                    auto ans = py::list();
                                    ans.append( *begin );
                                    return ans;
                                    }
                                begin++;
                                return coek::to_nested_list(begin, end);})
        .def("expand", [](coek::Expression& x) {return x.expand();})
        ;

    //
    // Objective
    //
    py::class_<coek::Objective>(m, "objective")
        .def("__init__", [](){throw std::runtime_error("Cannot create an empty objective.");})
        .def_property_readonly("value", [](coek::Objective& c){return c.value();})
        .def_property_readonly("id", &coek::Objective::id)
        .def_property_readonly("name", [](coek::Objective& c){return std::string("o")+std::to_string(c.id());})

        .def("to_list", [](coek::Objective& x){
                                auto tmp = x.to_list();
                                auto begin = tmp.begin();
                                auto end = tmp.end();
                                if (begin == end)
                                    return py::list();
                                if (tmp.size() == 1) {
                                    auto ans = py::list();
                                    ans.append( *begin );
                                    return ans;
                                    }
                                begin++;
                                return coek::to_nested_list(begin, end);})
        ;

    //
    // Constraint
    //
    py::class_<coek::Constraint>(m, "constraint")
        .def("__init__", [](){throw std::runtime_error("Cannot create an empty constraint.");})
        .def_property_readonly("value", [](coek::Constraint& c){return c.body().value();})
        .def_property_readonly("lb", [](coek::Constraint& c){return c.lower().value();})
        .def_property_readonly("ub", [](coek::Constraint& c){return c.upper().value();})
        .def_property_readonly("id", &coek::Constraint::id)
        .def_property_readonly("name", [](coek::Constraint& c){return std::string("c")+std::to_string(c.id());})
        .def("is_feasible", &coek::Constraint::is_feasible)
        .def("is_constraint",[](const coek::Constraint& ){return true;})
        .def("__eq__", [](){throw std::runtime_error("Cannot create a constraint from a boolean expression.");})
        .def("__le__", [](){throw std::runtime_error("Cannot create a constraint from a boolean expression.");})
        .def("__lt__", [](){throw std::runtime_error("Cannot create a constraint from a boolean expression.");})
        .def("__ge__", [](){throw std::runtime_error("Cannot create a constraint from a boolean expression.");})
        .def("__gt__", [](){throw std::runtime_error("Cannot create a constraint from a boolean expression.");})

        .def("to_list", [](coek::Constraint& x){
                                auto tmp = x.to_list();
                                auto begin = tmp.begin();
                                auto end = tmp.end();
                                if (begin == end)
                                    return py::list();
                                if (tmp.size() == 1) {
                                    auto ans = py::list();
                                    ans.append( *begin );
                                    return ans;
                                    }
                                begin++;
                                return coek::to_nested_list(begin, end);})
        .def("expand", [](coek::Constraint& x) {return x.expand();})
        ;

    //
    // Intrinsics
    //
    m.def("ceil", &coek::ceil);
    m.def("ceil", [](coek::Variable& x){return coek::ceil(x);});
    m.def("ceil", [](coek::Parameter& x){return coek::ceil(x);});
    m.def("ceil", [](coek::IndexParameter& x){return coek::ceil(x);});
    m.def("floor", &coek::floor);
    m.def("floor", [](coek::Variable& x){return coek::floor(x);});
    m.def("floor", [](coek::Parameter& x){return coek::floor(x);});
    m.def("floor", [](coek::IndexParameter& x){return coek::floor(x);});
    m.def("exp", &coek::exp);
    m.def("exp", [](coek::Variable& x){return coek::exp(x);});
    m.def("exp", [](coek::Parameter& x){return coek::exp(x);});
    m.def("exp", [](coek::IndexParameter& x){return coek::exp(x);});
    m.def("log", &coek::log);
    m.def("log", [](coek::Variable& x){return coek::log(x);});
    m.def("log", [](coek::Parameter& x){return coek::log(x);});
    m.def("log", [](coek::IndexParameter& x){return coek::log(x);});
    m.def("log10", &coek::log10);
    m.def("log10", [](coek::Variable& x){return coek::log10(x);});
    m.def("log10", [](coek::Parameter& x){return coek::log10(x);});
    m.def("log10", [](coek::IndexParameter& x){return coek::log10(x);});
    m.def("sqrt", &coek::sqrt);
    m.def("sqrt", [](coek::Variable& x){return coek::sqrt(x);});
    m.def("sqrt", [](coek::Parameter& x){return coek::sqrt(x);});
    m.def("sqrt", [](coek::IndexParameter& x){return coek::sqrt(x);});
    m.def("sin", &coek::sin);
    m.def("sin", [](coek::Variable& x){return coek::sin(x);});
    m.def("sin", [](coek::Parameter& x){return coek::sin(x);});
    m.def("sin", [](coek::IndexParameter& x){return coek::sin(x);});
    m.def("cos", &coek::cos);
    m.def("cos", [](coek::Variable& x){return coek::cos(x);});
    m.def("cos", [](coek::Parameter& x){return coek::cos(x);});
    m.def("cos", [](coek::IndexParameter& x){return coek::cos(x);});
    m.def("tan", &coek::tan);
    m.def("tan", [](coek::Variable& x){return coek::tan(x);});
    m.def("tan", [](coek::Parameter& x){return coek::tan(x);});
    m.def("tan", [](coek::IndexParameter& x){return coek::tan(x);});
    m.def("sinh", &coek::sinh);
    m.def("sinh", [](coek::Variable& x){return coek::sinh(x);});
    m.def("sinh", [](coek::Parameter& x){return coek::sinh(x);});
    m.def("sinh", [](coek::IndexParameter& x){return coek::sinh(x);});
    m.def("cosh", &coek::cosh);
    m.def("cosh", [](coek::Variable& x){return coek::cosh(x);});
    m.def("cosh", [](coek::Parameter& x){return coek::cosh(x);});
    m.def("cosh", [](coek::IndexParameter& x){return coek::cosh(x);});
    m.def("tanh", &coek::tanh);
    m.def("tanh", [](coek::Variable& x){return coek::tanh(x);});
    m.def("tanh", [](coek::Parameter& x){return coek::tanh(x);});
    m.def("tanh", [](coek::IndexParameter& x){return coek::tanh(x);});
    m.def("asin", &coek::asin);
    m.def("asin", [](coek::Variable& x){return coek::asin(x);});
    m.def("asin", [](coek::Parameter& x){return coek::asin(x);});
    m.def("asin", [](coek::IndexParameter& x){return coek::asin(x);});
    m.def("acos", &coek::acos);
    m.def("acos", [](coek::Variable& x){return coek::acos(x);});
    m.def("acos", [](coek::Parameter& x){return coek::acos(x);});
    m.def("acos", [](coek::IndexParameter& x){return coek::acos(x);});
    m.def("atan", &coek::atan);
    m.def("atan", [](coek::Variable& x){return coek::atan(x);});
    m.def("atan", [](coek::Parameter& x){return coek::atan(x);});
    m.def("atan", [](coek::IndexParameter& x){return coek::atan(x);});
    m.def("asinh", &coek::asinh);
    m.def("asinh", [](coek::Variable& x){return coek::asinh(x);});
    m.def("asinh", [](coek::Parameter& x){return coek::asinh(x);});
    m.def("asinh", [](coek::IndexParameter& x){return coek::asinh(x);});
    m.def("acosh", &coek::acosh);
    m.def("acosh", [](coek::Variable& x){return coek::acosh(x);});
    m.def("acosh", [](coek::Parameter& x){return coek::acosh(x);});
    m.def("acosh", [](coek::IndexParameter& x){return coek::acosh(x);});
    m.def("atanh", &coek::atanh);
    m.def("atanh", [](coek::Variable& x){return coek::atanh(x);});
    m.def("atanh", [](coek::Parameter& x){return coek::atanh(x);});
    m.def("atanh", [](coek::IndexParameter& x){return coek::atanh(x);});

    //
    // Model
    //
    py::class_<coek::Model>(m, "model")
        .def(py::init<>())
#if 0
        .def("add_variable",[](coek::Model& m, py::kwargs kw)
            {
            coek::Variable v = coek::variable_fn(kw);
            return m.add_variable(v);
            })
#endif
        .def("add_variable_", [](coek::Model& m, coek::Variable& v)
            {return m.add_variable(v);})
        .def("add_variable_", [](coek::Model& m, coek::VariableArray& v)
            { m.add(v); })
        .def("add_objective", [](coek::Model& m, double f)
            {coek::Expression e(f);
            return m.add_objective(e);})
        .def("add_objective", [](coek::Model& m, const coek::Expression& e){return m.add_objective(e);})
        .def("add_objective", [](coek::Model& m, const coek::Parameter& e){return m.add_objective(e);})
        .def("add_objective", [](coek::Model& m, const coek::Variable& e){return m.add_objective(e);})
        .def("add_objective", [](coek::Model& m, const coek::Expression& e, bool sense){return m.add(coek::Objective().expr(e).sense(sense));})
        .def("add_objective", [](coek::Model& m, const coek::Variable& e, bool sense){return m.add(coek::Objective().expr(e).sense(sense));})
        .def("add_constraint", [](coek::Model& m, coek::Constraint& c){return m.add(c);})

        .def("get_objective", [](coek::Model& m){return m.get_objective();})
        .def("get_objective", [](coek::Model& m, int i)
            {
            if (i < 0)
                throw std::invalid_argument("Index to objective must be non-negative");
            return m.get_objective(static_cast<size_t>(i));
            })
        .def("get_constraint", [](coek::Model& m, int i)
            {
            if (i < 0)
                throw std::invalid_argument("Index to objective must be non-negative");
            return m.get_constraint(static_cast<size_t>(i));
            })
        .def("num_variables", [](coek::Model& m){return m.num_variables();})
        .def("num_objectives", [](coek::Model& m){return m.num_objectives();})
        .def("num_constraints", [](coek::Model& m){return m.num_constraints();})

        .def("write", [](coek::Model& m, const std::string& s){m.write(s);})
        //.def("write", [](coek::Model& m, const std::string& s, std::map<int,int>& varmap, std::map<int,int>& conmap){m.write(s,varmap,conmap);})
        .def("print_equations", [](coek::Model& m){m.print_equations();})
        .def("print_values", [](coek::Model& m){m.print_values();})
        ;

    //
    // Functions for Compact Expressions
    //
#ifdef COEK_WITH_COMPACT_MODEL
    m.def("Sum",[](const coek::Expression& expr, const coek::SequenceContext& context) {return coek::Sum(context, expr);});
#if 0
    m.def("Sum",[](const coek::Variable& expr, const coek::SequenceContext& context) {return coek::Sum(expr, context);});
#endif

    m.def("Forall",[](py::args args){
                                std::vector<coek::IndexParameter> indices;
                                for (py::handle h : args)
                                    indices.push_back(h.cast<coek::IndexParameter>());
                                return coek::Forall(indices);
                                });
    //
    // SequenceContext
    //
/*
    py::class_<coek::SequenceContext>(m, "SequenceContext")
        .def("Forall", [](coek::SequenceContext& x, py::args args){
                                std::vector<coek::IndexParameter> indices;
                                for (py::handle h : args)
                                    indices.push_back(h.cast<coek::IndexParameter>());
                                return x.Forall(indices);
                                })
        .def("In", [](coek::SequenceContext& x, coek::ConcreteSet& context) {return x.In(context);})
        .def("ST", [](coek::SequenceContext& x, coek::Constraint& con) {return x.ST(con);})
        .def("Where", [](coek::SequenceContext& x, coek::Constraint& con) {return x.Where(con);})
        ;

    py::class_<coek::ExpressionSequence>(m, "ExpressionSequence")
        .def(py::init<coek::Expression&,coek::SequenceContext&>())
        .def("__iter__", [](const coek::ExpressionSequence& seq) 
            {return py::make_iterator(seq.begin(), seq.end());})
        ;

    py::class_<coek::ConstraintSequence>(m, "ConstraintSequence")
        .def(py::init<coek::Constraint&,coek::SequenceContext&>())
        .def("__iter__", [](const coek::ConstraintSequence& seq) 
            {return py::make_iterator(seq.begin(), seq.end());})
        ;
*/

    //
    // CompactModel
    //
    py::class_<coek::CompactModel>(m, "compact_model")
        .def(py::init<>())
#if 0
        .def("add_variable_",[](coek::CompactModel& m, py::kwargs kw)
            {
            coek::Variable v = coek::variable_fn(kw);
            return m.add_variable(v);
            })
#endif
        .def("add_variable_", [](coek::CompactModel& m, coek::Variable& v)
            {m.add_variable(v);})
        #if 0
        .def("add_variable_", [](coek::CompactModel& m, coek::ConcreteIndexedVariable& v)
            {m.add_variable(v);})
        #endif
        .def("add_variable_", [](coek::CompactModel& m, coek::VariableArray& v) { m.add(v); })

        .def("add_objective", [](coek::CompactModel& m, double f)
            {coek::Expression e(f);
            return m.add_objective(e);})
        .def("add_objective", [](coek::CompactModel& m, const coek::Expression& e)
            {return m.add_objective(e);})
        .def("add_objective", [](coek::CompactModel& m, const coek::Parameter& e)
            {return m.add_objective(e);})
        .def("add_objective", [](coek::CompactModel& m, const coek::Variable& e)
            {return m.add_objective(e);})
        .def("add_objective", [](coek::CompactModel& m, double f, bool sense)
            {coek::Expression e(f);
            return m.add(coek::Objective().expr(e).sense(sense));})
        .def("add_objective", [](coek::CompactModel& m, const coek::Expression& e, bool sense)
            {return m.add(coek::Objective().expr(e).sense(sense));})
        .def("add_objective", [](coek::CompactModel& m, const coek::Parameter& e, bool sense)
            {return m.add(coek::Objective().expr(e).sense(sense));})
        .def("add_objective", [](coek::CompactModel& m, const coek::Variable& e, bool sense)
            {return m.add(coek::Objective().expr(e).sense(sense));})

        /*
        .def("add_objective", [](coek::CompactModel& m, double f, const coek::SequenceContext& context)
            {coek::Expression e(f);
            return m.add_objective(context,e);})
        .def("add_objective", [](coek::CompactModel& m, const coek::Expression& e, const coek::SequenceContext& context)
            {return m.add_objective(context,e);})
        .def("add_objective", [](coek::CompactModel& m, const coek::Parameter& e, const coek::SequenceContext& context)
            {return m.add_objective(context,e);})
        .def("add_objective", [](coek::CompactModel& m, const coek::Variable& e, const coek::SequenceContext& context)
            {return m.add_objective(context,e);})
        .def("add_objective", [](coek::CompactModel& m, double f, const coek::SequenceContext& context, bool sense)
            {coek::Expression e(f);
            return m.add_objective(context, e, sense);})
        .def("add_objective", [](coek::CompactModel& m, const coek::Expression& e, const coek::SequenceContext& context, bool sense)
            {return m.add_objective(context, e, sense);})
        .def("add_objective", [](coek::CompactModel& m, const coek::Parameter& e, const coek::SequenceContext& context, bool sense)
            {return m.add_objective(context, e, sense);})
        .def("add_objective", [](coek::CompactModel& m, const coek::Variable& e, const coek::SequenceContext& context, bool sense)
            {return m.add_objective(context, e, sense);})
        */

        .def("add_constraint", [](coek::CompactModel& m, const coek::Constraint& c)
            {return m.add_constraint(c);})
        /*
        .def("add_constraint", [](coek::CompactModel& m, const coek::Constraint& c, const coek::SequenceContext& context)
            {return m.add_constraint(context,c);})
        */

        //.def("write", [](coek::CompactModel& m, const std::string& s, std::map<int,int>& varmap, std::map<int,int>& conmap){m.write(s,varmap,conmap);})
        .def("write", [](coek::CompactModel& m, const std::string& s){m.write(s);})
        ;
#endif

    //
    // Solver
    //
    py::class_<coek::Solver>(m, "solver")
        .def(py::init<>())
        .def(py::init<std::string&>())
        .def("initialize", &coek::Solver::initialize)
        .def_property_readonly("available", &coek::Solver::available)
        .def("solve", [](coek::Solver& s, coek::Model& m){return s.solve(m);})
        .def("load", [](coek::Solver& s, coek::Model& m){return s.load(m);})
#ifdef COEK_WITH_COMPACT_MODEL
        .def("solve", [](coek::Solver& s, coek::CompactModel& m){return s.solve(m);})
        .def("load", [](coek::Solver& s, coek::CompactModel& m){return s.load(m);})
#endif
        .def("resolve", &coek::Solver::resolve)

        .def("set_option", [](coek::Solver& s, const std::string& o, std::string& v){s.set_option(o,v);})
        .def("set_option", [](coek::Solver& s, const std::string& o, int v){s.set_option(o,v);})
        .def("set_option", [](coek::Solver& s, const std::string& o, double v){s.set_option(o,v);})
        .def("get_option", [](coek::Solver& s, const std::string& o){std::string v; s.get_option(o,v); return v;})
        .def("get_option", [](coek::Solver& s, const std::string& o){int v; s.get_option(o,v); return v;})
        .def("get_option", [](coek::Solver& s, const std::string& o){double v; s.get_option(o,v); return v;})
        ;

    //
    // NLPModel
    //
    py::class_<coek::NLPModel>(m, "nlp_model")
        .def(py::init<>())
        .def(py::init<coek::Model&,std::string>())
        .def(py::init<coek::Model&,std::string,bool>())

        .def("num_variables", &coek::NLPModel::num_variables)
        .def("num_objectives", &coek::NLPModel::num_objectives)
        .def("num_constraints", &coek::NLPModel::num_constraints)
        .def("get_variable", &coek::NLPModel::get_variable)
        //.def("set_variable", &coek::NLPModel::set_variable)
        .def("get_constraint", &coek::NLPModel::get_constraint)

        .def("compute_f", [](coek::NLPModel& m){return m.compute_f(0);})
        .def("compute_f", [](coek::NLPModel& m, unsigned int i){return m.compute_f(i);})
        .def("compute_c", [](coek::NLPModel& m)
                {
                std::vector<double> c(m.num_constraints());
                m.compute_c(c);
                return c;
                })
        .def("compute_df", [](coek::NLPModel& m)
                {
                std::vector<double> df(m.num_variables());
                m.compute_df(df,0);
                return df;
                })
        .def("compute_df", [](coek::NLPModel& m, unsigned int i)
                {
                std::vector<double> df(m.num_variables());
                m.compute_df(df,i);
                return df;
                })
        .def("compute_dc", [](coek::NLPModel& m, unsigned int i)
                {
                std::vector<double> dc(m.num_variables());
                m.compute_dc(dc,i);
                return dc;
                })
        //.def("compute_H", [](coek::NLPModel& m){return m.compute_f();})
        //.def("compute_J", [](coek::NLPModel& m){return m.compute_f();})
        .def("write", [](coek::NLPModel& m, const std::string& s){m.write(s);})
        //.def("write", [](coek::NLPModel& m, const std::string& s, std::map<int,int>& varmap, std::map<int,int>& conmap){m.write(s,varmap,conmap);})
        ;

    //
    // NLPSolver
    //
    py::class_<coek::NLPSolver>(m, "nlp_solver")
        .def(py::init<>())
        .def(py::init<std::string&>())
        .def("initialize", &coek::NLPSolver::initialize)
        .def("solve", &coek::NLPSolver::solve)
        .def("load", &coek::NLPSolver::load)
        .def("resolve", &coek::NLPSolver::resolve)

        .def("set_option", [](coek::NLPSolver& s, const std::string& o, std::string& v){s.set_option(o,v);})
        .def("set_option", [](coek::NLPSolver& s, const std::string& o, int v){s.set_option(o,v);})
        .def("set_option", [](coek::NLPSolver& s, const std::string& o, double v){s.set_option(o,v);})
        .def("get_option", [](coek::NLPSolver& s, const std::string& o){std::string v; s.get_option(o,v); return v;})
        .def("get_option", [](coek::NLPSolver& s, const std::string& o){int v; s.get_option(o,v); return v;})
        .def("get_option", [](coek::NLPSolver& s, const std::string& o){double v; s.get_option(o,v); return v;})
        ;

    m.def("inequality", [](int lower, const coek::Expression& body, int upper){return inequality(lower,body,upper);});
    m.def("inequality", [](double lower, const coek::Expression& body, double upper){return inequality(lower,body,upper);});
    m.def("inequality", [](const coek::Expression lower, const coek::Expression& body, const coek::Expression upper){return inequality(lower,body,upper);});
    m.def("inequality", [](int lower, const coek::Expression& body, int upper, bool strict){return inequality(lower,body,upper,strict);});
    m.def("inequality", [](double lower, const coek::Expression& body, double upper, bool strict){return inequality(lower,body,upper,strict);});
    m.def("inequality", [](const coek::Expression lower, const coek::Expression& body, const coek::Expression upper, bool strict){return inequality(lower,body,upper,strict);});
    m.def("inequality", [](int lower, const coek::Variable& body, int upper){return inequality(lower,body,upper);});
    m.def("inequality", [](double lower, const coek::Variable& body, double upper){return inequality(lower,body,upper);});
    m.def("inequality", [](const coek::Expression lower, const coek::Variable& body, const coek::Expression upper){return inequality(lower,body,upper);});
    m.def("inequality", [](int lower, const coek::Variable& body, int upper, bool strict){return inequality(lower,body,upper,strict);});
    m.def("inequality", [](double lower, const coek::Variable& body, double upper, bool strict){return inequality(lower,body,upper,strict);});
    m.def("inequality", [](const coek::Expression lower, const coek::Variable& body, const coek::Expression upper, bool strict){return inequality(lower,body,upper,strict);});

}

