//
// pycoek:  A Python Module that wraps Coek objects
//

// TODO: Replace __truediv__ in earlier versions of Python

#include <iostream>
#include <typeinfo>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include "ast_term.hpp"
#include "coek_model.hpp"

namespace py = pybind11;

PYBIND11_MAKE_OPAQUE(std::map<int,int>);


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
        //for (std::size_t j=0; j<tmp.size(); j++)
        //    std::cerr << tmp[j] << " ";
        //std::cerr << " : " << ctr << std::endl;
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


template<typename T>
class MapKeyIterator : public T
{
public:
    MapKeyIterator() : T() {}
    MapKeyIterator(T iter) : T(iter) {}
    auto* operator->()
    {
        return &(T::operator->()->first);
    }
    auto& operator*()
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
    auto* operator->()
    {
        curr++; return &curr;
    }
    auto& operator*()
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
        double lb, ub, init;
        bool binary, integer, fixed;
        parse_varargs<double>(kwargs, "lb", lb, -COEK_INFINITY);
        parse_varargs<double>(kwargs, "ub", ub,  COEK_INFINITY);
        parse_varargs<double>(kwargs, "initial", init, NAN);
        parse_varargs<bool>(kwargs, "binary", binary, false);
        parse_varargs<bool>(kwargs, "integer", integer, false);
        parse_varargs<bool>(kwargs, "fixed", fixed, false);
        if (kwargs.contains("name"))
            name = kwargs["name"].cast<py::str>();

        variables.resize(n);
        for (int i=0; i<n; i++) {
            auto tmp = CREATE_POINTER(_IndexedVariableTerm, lb, ub, init, binary, integer, fixed, i, this);
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

    typedef MapKeyIterator<std::map<std::vector<int>,int>::const_iterator> map_key_t;
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

Variable variable_fn(py::kwargs kwargs)
{
double lb, ub, init;
bool binary, integer, fixed;
parse_varargs<double>(kwargs, "lb", lb, -COEK_INFINITY);
parse_varargs<double>(kwargs, "ub", ub,  COEK_INFINITY);
parse_varargs<double>(kwargs, "initial", init, NAN);
parse_varargs<bool>(kwargs, "binary", binary, false);
parse_varargs<bool>(kwargs, "integer", integer, false);
parse_varargs<bool>(kwargs, "fixed", fixed, false);

Variable tmp;
try {
    if (kwargs.contains("name")) {
        auto _name = kwargs["name"];
        if (not _name.is_none()) {
            auto name = _name.cast<py::str>();
            tmp.initialize(lb, ub, init, binary, integer, fixed, name);
            return tmp;
            }
        }
    tmp.initialize(lb, ub, init, binary, integer, fixed);
    return tmp;
    }
catch (std::exception& err) {
    throw;
    //std::cerr << "HUH?" << std::endl;
    //std::cerr << typeid(err).name() << std::endl;
    }
}

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
}

PYBIND11_MODULE(pycoek, m) {

    py::bind_map<std::map<int, int>>(m, "STLMapIntInt");

    m.doc() = "A Python module that wraps Coek";

    m.def("_variable",[](int n, py::kwargs kw) {return coek::variable_fn(n, kw);});
    m.def("_variable",[](std::vector<int>& dimen, py::kwargs kw) {return coek::variable_fn(dimen, kw);});
    m.def("_variable",[](py::kwargs kw) {return coek::variable_fn(kw);});
    m.def("affine_expression",[](std::vector<double>& coef, std::vector<coek::Variable>& var, double offset) {return affine_expression(coef, var, offset);});
    m.def("affine_expression",[](std::vector<double>& coef, std::vector<coek::Variable>& var) {return affine_expression(coef, var, 0);});
    m.def("affine_expression",[](std::vector<coek::Variable>& var, double offset) {return affine_expression(var, offset);});
    m.def("affine_expression",[](std::vector<coek::Variable>& var) {return affine_expression(var, 0);});

    m.def("SetOf",[](std::vector<int>& arg) {return coek::SetOf(arg);});
    m.def("RangeSet",[](int start, int stop, int step=1) {return coek::RangeSet(start, stop, step);});
    m.def("RangeSet",[](int start, int stop) {return coek::RangeSet(start, stop);});

    m.def("Sum",[](const coek::ExpressionSequence& seq) {return Sum(seq);});

    m.def("IndexedVariable",[](const coek::ConcreteSet& arg, double lb=-COEK_INFINITY, double ub=COEK_INFINITY, double value=0.0) {return coek::IndexedVariable(arg, lb, ub, value);});
    m.def("IndexedVariable",[](const coek::ConcreteSet& arg, double lb=-COEK_INFINITY, double ub=COEK_INFINITY) {return coek::IndexedVariable(arg, lb, ub);});
    m.def("IndexedVariable",[](const coek::ConcreteSet& arg) {return coek::IndexedVariable(arg);});
    m.def("IndexedVariable",[](const coek::ConcreteSet& arg, const std::string& name) {return coek::IndexedVariable(arg, name);});
    m.def("IndexedVariable",[](const coek::ConcreteSet& arg, double lb, double ub, double value, const std::string& name) {return coek::IndexedVariable(arg, lb, ub, value, name);});
    m.def("IndexedVariable",[](const coek::ConcreteSet& arg, double lb, double ub, double value, bool binary, bool integer, const std::string& name) {return coek::IndexedVariable(arg, lb, ub, value, binary, integer, name);});
    m.def("IndexedVariable",[](const coek::ConcreteSet& arg, double lb, double ub, double value, bool binary, bool integer) {return coek::IndexedVariable(arg, lb, ub, value, binary, integer);});

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

    py::class_<coek::ConcreteIndexedVariable>(m, "ConcreteIndexedVariable")
        .def("__call__", [](coek::ConcreteIndexedVariable& x, py::args args) {return coek::ConcreteIndexedVariable_getitem(x,args);})
        .def("__getitem__", [](coek::ConcreteIndexedVariable& x, py::tuple args) {return coek::ConcreteIndexedVariable_getitem(x,args);})
        .def("__getitem__", [](coek::ConcreteIndexedVariable& x, py::args args) {return coek::ConcreteIndexedVariable_getitem(x,args);})
        ;

    //
    // Parameter
    //
    py::class_<coek::Parameter>(m, "parameter")
        .def(py::init<double>())
        .def(py::init<double, const std::string&>())
        .def("get_name",&coek::Parameter::get_name)
        .def_property("value", &coek::Parameter::get_value, &coek::Parameter::set_value)
        .def("is_constraint",[](const coek::Parameter& x){return false;})

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
        .def("get_name",&coek::IndexParameter::get_name)
        .def("get_value",&coek::IndexParameter::get_name)
        .def("set_value",&coek::IndexParameter::get_name)
        .def("is_constraint",[](const coek::IndexParameter& x){return false;})

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
        .def(py::init<double>())
        .def(py::init<double, double, double, const std::string&>())
        .def(py::init<double, double, double, bool, bool>())
        .def_property_readonly("name",&coek::Variable::get_name)
        .def_property("value", &coek::Variable::get_value, &coek::Variable::set_value)
        .def_property("initial", &coek::Variable::get_initial, &coek::Variable::set_initial)
        .def_property("lb", &coek::Variable::get_lb, &coek::Variable::set_lb)
        .def_property("ub", &coek::Variable::get_ub, &coek::Variable::set_ub)
        .def_property("fixed", &coek::Variable::get_fixed, &coek::Variable::set_fixed)
        .def_property_readonly("id", &coek::Variable::id)
        .def("is_constraint",[](const coek::Variable& x){return false;})

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
        .def(py::init<int, py::kwargs>())
        .def(py::init<std::vector<int>&, py::kwargs>())
        .def("__getitem__", [](coek::VariableArray& va, int i){return va.get(i);})
        .def("__getitem__", [](coek::VariableArray& va, std::vector<int>& index){return va.get(index);})
        .def_property_readonly("name", [](coek::VariableArray& x) -> py::object {
                if (x.name.size() == 0)
                    return py::cast<std::string>("x");
                else
                    return py::cast<std::string>(x.get_name());
                })
        .def("is_constraint",[](const coek::VariableArray& x){return false;})
        .def("__iter__", [](const coek::VariableArray& va) 
                {
                if (va.dimen.size() == 0)
                     return py::make_iterator(va.unindexed_begin(), va.unindexed_end());
                else
                     return py::make_iterator(va.indexed_begin(), va.indexed_end());
                },
                py::keep_alive<0, 1>())
        ;

    //
    // ExpressionSequenceAux
    //
    py::class_<coek::ExpressionSequenceAux>(m, "ExpressionSequenceAux")
        .def("In",[](coek::ExpressionSequenceAux& self, const coek::ConcreteSet& A){return self.in(A);})
        //.def("ST",[](const coek::ExpressionSequenceAux& self, const coek::ConcreteSet& A){return self.in(A);})
        //.def("Where",[](const coek::ExpressionSequenceAux& self, const coek::ConcreteSet& A){return self.in(A);})
        ;

    //
    // ExpressionSequence
    //
    py::class_<coek::ExpressionSequence>(m, "ExpressionSequence")
        .def("Forall", [](coek::ExpressionSequence& x, py::args args){
                                std::vector<coek::IndexParameter> indices;
                                for (py::handle h : args)
                                    indices.push_back(h.cast<coek::IndexParameter>());
                                return x.forall(indices);
                                })
        .def("__iter__", [](coek::ExpressionSequence& x) {return py::make_iterator(x.begin(), x.end());})
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
        .def_property_readonly("value", &coek::Expression::get_value)
        .def("is_constraint",[](const coek::Expression& x){return false;})

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
        .def("Forall", [](coek::Expression& x, py::args args){
                                std::vector<coek::IndexParameter> indices;
                                for (py::handle h : args)
                                    indices.push_back(h.cast<coek::IndexParameter>());
                                return x.forall(indices);
                                })
        .def("expand", [](coek::Expression& x) {return x.expand();})
        ;

    //
    // ConstraintSequenceAux
    //
    py::class_<coek::ConstraintSequenceAux>(m, "ConstraintSequenceAux")
        .def("In",[](coek::ConstraintSequenceAux& self, const coek::ConcreteSet& A){return self.in(A);})
        //.def("ST",[](const coek::ExpressionSequenceAux& self, const coek::ConcreteSet& A){return self.in(A);})
        //.def("Where",[](const coek::ExpressionSequenceAux& self, const coek::ConcreteSet& A){return self.in(A);})
        ;

    //
    // ConstraintSequence
    //
    py::class_<coek::ConstraintSequence>(m, "ConstraintSequence")
        .def("Forall", [](coek::ConstraintSequence& x, py::args args){
                                std::vector<coek::IndexParameter> indices;
                                for (py::handle h : args)
                                    indices.push_back(h.cast<coek::IndexParameter>());
                                return x.forall(indices);
                                })
        .def("__iter__", [](coek::ConstraintSequence& x) {return py::make_iterator(x.begin(), x.end());})
        ;

    //
    // Constraint
    //
    py::class_<coek::Constraint>(m, "constraint")
        .def("__init__", [](){throw std::runtime_error("Cannot create an empty constraint.");})
        .def_property_readonly("value", &coek::Constraint::get_value)
        .def_property_readonly("id", &coek::Constraint::id)
        .def_property_readonly("name", [](coek::Constraint& c){return std::string("c")+std::to_string(c.id());})
        .def("is_feasible", &coek::Constraint::is_feasible)
        .def("is_constraint",[](const coek::Constraint& x){return true;})
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
        .def("Forall", [](coek::Constraint& x, py::args args){
                                std::vector<coek::IndexParameter> indices;
                                for (py::handle h : args)
                                    indices.push_back(h.cast<coek::IndexParameter>());
                                return x.forall(indices);
                                })
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
        .def("add", [](coek::Model& m, const coek::Expression& e){m.add(e);})
        .def("add", [](coek::Model& m, const coek::Parameter& e){m.add(e);})
        .def("add", [](coek::Model& m, const coek::Variable& e){m.add(e);})
        .def("add", [](coek::Model& m, const coek::Expression& e, bool sense){m.add(e, sense);})
        .def("add", [](coek::Model& m, const coek::Variable& e, bool sense){m.add(e, sense);})
        .def("add", [](coek::Model& m, const coek::Constraint& c){m.add(c);})
        .def("write", [](coek::Model& m, const std::string& s){m.write(s);})
        .def("write", [](coek::Model& m, const std::string& s, std::map<int,int>& varmap, std::map<int,int>& conmap){m.write(s,varmap,conmap);})
        .def("use", [](coek::Model& m, coek::Variable& v){m.addVariable(v);})
        .def("use", [](coek::Model& m, coek::VariableArray& v)
            {
            for (auto it=v.variables.begin(); it != v.variables.end(); ++it)
                m.addVariable(*it);
            })
        .def("get_objective", [](coek::Model& m){return m.repn->objectives[0];})
        .def("get_objective", [](coek::Model& m, int i){return m.repn->objectives[i];})
        .def("get_constraint", [](coek::Model& m, int i){return m.repn->constraints[i];})
        .def("num_variables", [](coek::Model& m){return m.repn->variables.size();})
        .def("num_objectives", [](coek::Model& m){return m.repn->objectives.size();})
        .def("num_constraints", [](coek::Model& m){return m.repn->constraints.size();})
        .def("display", [](coek::Model& m){std::cout << m << std::endl;})
        ;

    //
    // CompactModel
    //
    py::class_<coek::CompactModel>(m, "compact_model")
        .def(py::init<>())
        .def("add", [](coek::CompactModel& m, const coek::Expression& e){m.add(e);})
        .def("add", [](coek::CompactModel& m, const coek::Parameter& e){m.add(e);})
        .def("add", [](coek::CompactModel& m, const coek::Variable& e){m.add(e);})
        .def("add", [](coek::CompactModel& m, const coek::Variable& e, bool sense){m.add(e, sense);})
        .def("add", [](coek::CompactModel& m, const coek::Expression& e, bool sense){m.add(e, sense);})
        .def("add", [](coek::CompactModel& m, const coek::ExpressionSequence& e){m.add(e);})
        .def("add", [](coek::CompactModel& m, const coek::ExpressionSequence& e, bool sense){m.add(e, sense);})

        .def("add", [](coek::CompactModel& m, const coek::Constraint& c){m.add(c);})
        .def("add", [](coek::CompactModel& m, const coek::ConstraintSequence& c){m.add(c);})

        .def("write", [](coek::CompactModel& m, const std::string& s, std::map<int,int>& varmap, std::map<int,int>& conmap){m.write(s,varmap,conmap);})
        .def("write", [](coek::CompactModel& m, const std::string& s){m.write(s);})

        .def("use", [](coek::CompactModel& m, coek::Variable& v){m.addVariable(v);})
        .def("use", [](coek::CompactModel& m, coek::ConcreteIndexedVariable& v){m.addVariable(v);})
        .def("use", [](coek::CompactModel& m, coek::VariableArray& v)
            {
            for (auto it=v.variables.begin(); it != v.variables.end(); ++it)
                m.addVariable(*it);
            })
        ;

    //
    // Solver
    //
    py::class_<coek::Solver>(m, "solver")
        .def(py::init<>())
        .def(py::init<std::string&>())
        .def("initialize", &coek::Solver::initialize)
        .def("solve", [](coek::Solver& s, coek::Model& m){return s.solve(m);})
        .def("solve", [](coek::Solver& s, coek::CompactModel& m){return s.solve(m);})
        .def("load", [](coek::Solver& s, coek::Model& m){return s.load(m);})
        .def("load", [](coek::Solver& s, coek::CompactModel& m){return s.load(m);})
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
        .def("set_variable", &coek::NLPModel::set_variable)
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
        .def("write", [](coek::NLPModel& m, const std::string& s, std::map<int,int>& varmap, std::map<int,int>& conmap){m.write(s,varmap,conmap);})
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

}

