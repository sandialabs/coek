//
// pycoek:  A Python Module that wraps Coek objects
//

#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include <sstream>
#include <cmath>
#include <iostream>
#include <typeinfo>

#include "coek/coek.hpp"
#include "coek/ast/value_terms.hpp"

namespace py = pybind11;

PYBIND11_MAKE_OPAQUE(std::map<int, int>)

namespace coek {

QuadraticExpr generate_standard_repn(Expression& expr)
{
    QuadraticExpr orepn;
    orepn.collect_terms(expr);
    return orepn;
}

Expression sum(std::vector<Expression*> args)
{
    Expression res(0);
    for (Expression* arg : args) {
        res += *arg;
    }
    return res;
}

Expression expression_from_float(double val)
{
    Expression res(val);
    return res;
}

Expression expression_from_param(Parameter& val)
{
    Expression res(val);
    return res;
}

Expression expression_from_var(Variable& val)
{
    Expression res(val);
    return res;
}

Expression float_mul_param(double v, Parameter& other) { return v * other; }

Expression float_mul_var(double v, Variable& other) { return v * other; }

Expression float_mul_expression(double v, Expression& other) { return v * other; }

Expression float_add_param(double v, Parameter& other) { return v + other; }

Expression float_add_var(double v, Variable& other) { return v + other; }

Expression float_add_expression(double v, Expression& other) { return v + other; }

Expression float_sub_param(double v, Parameter& other) { return v - other; }

Expression float_sub_var(double v, Variable& other) { return v - other; }

Expression float_sub_expression(double v, Expression& other) { return v - other; }

Expression float_div_param(double v, Parameter& other) { return v / other; }

Expression float_div_var(double v, Variable& other) { return v / other; }

Expression float_div_expression(double v, Expression& other) { return v / other; }

Expression float_pow_param(double v, Parameter& other) { return pow(v, other); }

Expression float_pow_var(double v, Variable& other) { return pow(v, other); }

Expression float_pow_expression(double v, Expression& other) { return pow(v, other); }

Constraint float_eq_param(double v, Parameter& other) { return v == other; }

Constraint float_eq_var(double v, Variable& other) { return v == other; }

Constraint float_eq_expression(double v, Expression& other) { return v == other; }

Constraint float_le_param(double v, Parameter& other) { return v <= other; }

Constraint float_le_var(double v, Variable& other) { return v <= other; }

Constraint float_le_expression(double v, Expression& other) { return v <= other; }

Constraint float_ge_param(double v, Parameter& other) { return v >= other; }

Constraint float_ge_var(double v, Variable& other) { return v >= other; }

Constraint float_ge_expression(double v, Expression& other) { return v >= other; }

Expression var_mul_float(Variable& v, double other) { return v * other; }

Expression var_mul_param(Variable& v, Parameter& other) { return v * other; }

Expression var_mul_var(Variable& v, Variable& other) { return v * other; }

Expression var_mul_expression(Variable& v, Expression& other) { return v * other; }

Expression var_add_float(Variable& v, double other) { return v + other; }

Expression var_add_param(Variable& v, Parameter& other) { return v + other; }

Expression var_add_var(Variable& v, Variable& other) { return v + other; }

Expression var_add_expression(Variable& v, Expression& other) { return v + other; }

Expression var_sub_float(Variable& v, double other) { return v - other; }

Expression var_sub_param(Variable& v, Parameter& other) { return v - other; }

Expression var_sub_var(Variable& v, Variable& other) { return v - other; }

Expression var_sub_expression(Variable& v, Expression& other) { return v - other; }

Expression var_div_float(Variable& v, double other) { return v / other; }

Expression var_div_param(Variable& v, Parameter& other) { return v / other; }

Expression var_div_var(Variable& v, Variable& other) { return v / other; }

Expression var_div_expression(Variable& v, Expression& other) { return v / other; }

Expression var_pow_float(Variable& v, double other) { return pow(v, other); }

Expression var_pow_param(Variable& v, Parameter& other) { return pow(v, other); }

Expression var_pow_var(Variable& v, Variable& other) { return pow(v, other); }

Expression var_pow_expression(Variable& v, Expression& other) { return pow(v, other); }

Constraint var_eq_float(Variable& v, double other) { return v == other; }

Constraint var_eq_param(Variable& v, Parameter& other) { return v == other; }

Constraint var_eq_var(Variable& v, Variable& other) { return v == other; }

Constraint var_eq_expression(Variable& v, Expression& other) { return v == other; }

Constraint var_le_float(Variable& v, double other) { return v <= other; }

Constraint var_le_param(Variable& v, Parameter& other) { return v <= other; }

Constraint var_le_var(Variable& v, Variable& other) { return v <= other; }

Constraint var_le_expression(Variable& v, Expression& other) { return v <= other; }

Constraint var_ge_float(Variable& v, double other) { return v >= other; }

Constraint var_ge_param(Variable& v, Parameter& other) { return v >= other; }

Constraint var_ge_var(Variable& v, Variable& other) { return v >= other; }

Constraint var_ge_expression(Variable& v, Expression& other) { return v >= other; }

Expression expression_mul_float(Expression& v, double other) { return v * other; }

Expression expression_mul_param(Expression& v, Parameter& other) { return v * other; }

Expression expression_mul_var(Expression& v, Variable& other) { return v * other; }

Expression expression_mul_expression(Expression& v, Expression& other) { return v * other; }

Expression expression_add_float(Expression& v, double other) { return v + other; }

Expression expression_add_param(Expression& v, Parameter& other) { return v + other; }

Expression expression_add_var(Expression& v, Variable& other) { return v + other; }

Expression expression_add_expression(Expression& v, Expression& other) { return v + other; }

Expression expression_sub_float(Expression& v, double other) { return v - other; }

Expression expression_sub_param(Expression& v, Parameter& other) { return v - other; }

Expression expression_sub_var(Expression& v, Variable& other) { return v - other; }

Expression expression_sub_expression(Expression& v, Expression& other) { return v - other; }

Expression expression_div_float(Expression& v, double other) { return v / other; }

Expression expression_div_param(Expression& v, Parameter& other) { return v / other; }

Expression expression_div_var(Expression& v, Variable& other) { return v / other; }

Expression expression_div_expression(Expression& v, Expression& other) { return v / other; }

Expression expression_pow_float(Expression& v, double other) { return pow(v, other); }

Expression expression_pow_param(Expression& v, Parameter& other) { return pow(v, other); }

Expression expression_pow_var(Expression& v, Variable& other) { return pow(v, other); }

Expression expression_pow_expression(Expression& v, Expression& other) { return pow(v, other); }

Constraint expression_eq_float(Expression& v, double other) { return v == other; }

Constraint expression_eq_param(Expression& v, Parameter& other) { return v == other; }

Constraint expression_eq_var(Expression& v, Variable& other) { return v == other; }

Constraint expression_eq_expression(Expression& v, Expression& other) { return v == other; }

Constraint expression_le_float(Expression& v, double other) { return v <= other; }

Constraint expression_le_param(Expression& v, Parameter& other) { return v <= other; }

Constraint expression_le_var(Expression& v, Variable& other) { return v <= other; }

Constraint expression_le_expression(Expression& v, Expression& other) { return v <= other; }

Constraint expression_ge_float(Expression& v, double other) { return v >= other; }

Constraint expression_ge_param(Expression& v, Parameter& other) { return v >= other; }

Constraint expression_ge_var(Expression& v, Variable& other) { return v >= other; }

Constraint expression_ge_expression(Expression& v, Expression& other) { return v >= other; }

std::vector<Variable> copy_var(Variable& v, unsigned int num_copies)
{
    std::vector<Variable> res(num_copies);
    for (Variable& v2 : res) {
        v2.value(v.value());
        v2.lower(v.lower());
        v2.upper(v.upper());
        v2.within(v.within());
        v2.fixed(v.fixed());
    }
    return res;
}

py::object constraint_lb(Constraint& c)
{
    if (c.has_lower())
        return py::cast(c.lower().value());
    else
        return py::none();
}

py::object constraint_ub(Constraint& c)
{
    if (c.has_upper())
        return py::cast(c.upper().value());
    else
        return py::none();
}

Expression construct_linear_expression(std::vector<double> coefs, std::vector<Variable*> vars,
                                       double constant)
{
    Expression res(constant);
    for (unsigned int ndx = 0; ndx < coefs.size(); ++ndx) {
        res += ((coefs[ndx])) * (*(vars[ndx]));
    }
    return res;
}

double expression_eval(Expression& e, bool /*exception*/ = false) { return e.value(); }

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
            tmp.append(to_nested_list(it, end));
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

template <typename T>
class VecKeyIterator : public T {
   public:
    int curr;

    VecKeyIterator() : T() { curr = -1; }
    VecKeyIterator(T iter) : T(iter) { curr = -1; }
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
    parse_varargs<double>(kwargs, "ub", ub, COEK_INFINITY);
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
        // std::cerr << "HUH?" << std::endl;
        // std::cerr << typeid(err).name() << std::endl;
    }
}

#ifdef COEK_WITH_COMPACT_MODEL
template <class TYPE>
void set_kwargs_varmap(TYPE& var, py::kwargs kwargs)
{
    double lb, ub, value;
    bool binary, integer;
    parse_varargs<double>(kwargs, "lb", lb, -COEK_INFINITY);
    parse_varargs<double>(kwargs, "ub", ub, COEK_INFINITY);
    parse_varargs<double>(kwargs, "value", value, NAN);
    parse_varargs<bool>(kwargs, "binary", binary, false);
    parse_varargs<bool>(kwargs, "integer", integer, false);
    // parse_varargs<bool>(kwargs, "fixed", fixed, false);

    try {
        var.value(value).bounds(lb, ub);  //.fixed(fixed);
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
        // std::cerr << "HUH?" << std::endl;
        // std::cerr << typeid(err).name() << std::endl;
    }
}

VariableMap variable_fn(coek::ConcreteSet& index_set, py::kwargs kwargs)
{
    VariableMap tmp(index_set);
    set_kwargs_varmap(tmp, kwargs);
    return tmp;
}
#endif

VariableArray variable_fn(std::vector<int>& dimen, py::kwargs kwargs)
{
    std::vector<size_t> _dimen(dimen.size());
    for (size_t i = 0; i < dimen.size(); ++i) {
        assert(dimen[i] >= 0);
        _dimen[i] = static_cast<size_t>(dimen[i]);
    }
    VariableArray tmp(_dimen);
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

#ifdef COEK_WITH_COMPACT_MODEL
template <class T>
// coek::Expression
py::object Array_getitem(T& x, py::tuple args)
{
    std::vector<coek::refarg_types>& refarg = x.reftmp;
    coek::IndexVector& setarg = x.tmp;

    if (args.size() == 0)
        throw std::runtime_error("Missing index");

    bool pytuple = false;
    try {
        auto tmp = args[0].cast<py::tuple>();
        pytuple = true;
        // std::cout << "PYTUPLE " << std::endl;
    }
    catch (std::exception& e) {
        // std::cout << "PYTUPLE ERROR " << e.what() << std::endl;
    }

    size_t i = 0;
    size_t j = 0;
    if (pytuple) {
        auto tuple_ = args[0].cast<py::tuple>();
        for (py::handle h : tuple_) {
            bool value = false;
            if (!value) {
                try {
                    coek::Expression tmp = h.cast<coek::IndexParameter>();
                    // std::cout << "REFARG " << tmp << std::endl;
                    refarg[j++] = tmp.repn;
                    value = true;
                }
                catch (std::exception& e) {
                    // std::cout << "REFARG ERROR " << e.what() << std::endl;
                }
            }
            if (!value) {
                try {
                    coek::Expression tmp = h.cast<coek::Expression>();
                    // std::cout << "EXPR " << std::endl;
                    refarg[j++] = tmp.repn;
                    value = true;
                }
                catch (std::exception& e) {
                    // std::cout << "EXPR ERROR " << e.what() << std::endl;
                }
            }
            if (!value) {
                try {
                    int tmp = h.cast<int>();
                    // std::cout << "INT " << tmp << std::endl;
                    refarg[j++] = tmp;
                    setarg[i++] = tmp;
                    value = true;
                }
                catch (std::exception& e) {
                    // std::cout << "INT ERROR " << e.what() << std::endl;
                }
            }
            if (!value)
                throw std::runtime_error("Unknown argument " + std::to_string(j));
        }
    }
    else {
        bool value = false;
        if (!value) {
            try {
                coek::Expression tmp = args[0].cast<coek::IndexParameter>();
                // std::cout << "REFARG " << tmp << std::endl;
                refarg[j++] = tmp.repn;
                value = true;
            }
            catch (std::exception& e) {
                // std::cout << "REFARG ERROR " << e.what() << std::endl;
            }
        }
        if (!value) {
            try {
                coek::Expression tmp = args[0].cast<coek::Expression>();
                // std::cout << "EXPR " << std::endl;
                refarg[j++] = tmp.repn;
                value = true;
            }
            catch (std::exception& e) {
                // std::cout << "EXPR ERROR " << e.what() << std::endl;
            }
        }
        if (!value) {
            try {
                int tmp = args[0].cast<int>();
                // std::cout << "INT " << tmp << std::endl;
                refarg[j++] = tmp;
                setarg[i++] = tmp;
                value = true;
            }
            catch (std::exception& e) {
                // std::cout << "INT ERROR " << e.what() << std::endl;
            }
        }
        if (!value)
            throw std::runtime_error("Unknown argument " + std::to_string(j));
    }

    // std::cout << "HERE " << refarg.size() << " " << setarg.size() << std::endl;
    py::object result;
    if (j > i)
        result = py::cast<Expression>(x.create_varref(refarg));
    else
        result = py::cast<Variable>(x.index(setarg));
    return result;
}
#endif
}  // namespace coek

PYBIND11_MODULE(pycoek_pybind11, m)
{
    py::bind_map<std::map<int, int>>(m, "STLMapIntInt");

    m.doc() = "A Python module that wraps Coek";

    m.attr("inf") = COEK_INFINITY;

    // m.def("stop_here",coek::stop_here);
    m.def("to_string", [](int v) { return std::to_string(v); });
    m.def("to_string", [](double v) { return std::to_string(v); });
    m.def("variable_", [](int n, py::kwargs kw) { return coek::variable_fn(n, kw); });
    m.def("variable_",
          [](std::vector<int>& dimen, py::kwargs kw) { return coek::variable_fn(dimen, kw); });
    m.def("variable_", [](py::kwargs kw) { return coek::variable_fn(kw); });
#ifdef COEK_WITH_COMPACT_MODEL
    m.def("variable_", [](coek::ConcreteSet& index_set, py::kwargs kw) {
        return coek::variable_fn(index_set, kw);
    });
#endif
    m.def("affine_expression", [](std::vector<double>& coef, std::vector<coek::Variable>& var,
                                  double offset) { return affine_expression(coef, var, offset); });
    m.def("affine_expression", [](std::vector<double>& coef, std::vector<coek::Variable>& var) {
        return affine_expression(coef, var, 0);
    });
    m.def("affine_expression", [](std::vector<coek::Variable>& var, double offset) {
        return affine_expression(var, offset);
    });
    m.def("affine_expression",
          [](std::vector<coek::Variable>& var) { return affine_expression(var, 0); });

    m.def("sum", &coek::sum);
    m.def("construct_linear_expression", &coek::construct_linear_expression);
    m.def("generate_standard_repn", &coek::generate_standard_repn);
    m.def("copy_var", &coek::copy_var);
    m.def("expression_from_float", &coek::expression_from_float);
    m.def("expression_from_param", &coek::expression_from_param);
    m.def("expression_from_var", &coek::expression_from_var);
    m.def("float_mul_var", &coek::float_mul_var);
    m.def("float_mul_param", &coek::float_mul_param);
    m.def("float_mul_expression", &coek::float_mul_expression);
    m.def("float_add_var", &coek::float_add_var);
    m.def("float_add_param", &coek::float_add_param);
    m.def("float_add_expression", &coek::float_add_expression);
    m.def("float_sub_var", &coek::float_sub_var);
    m.def("float_sub_param", &coek::float_sub_param);
    m.def("float_sub_expression", &coek::float_sub_expression);
    m.def("float_div_var", &coek::float_div_var);
    m.def("float_div_param", &coek::float_div_param);
    m.def("float_div_expression", &coek::float_div_expression);
    m.def("float_pow_var", &coek::float_pow_var);
    m.def("float_pow_param", &coek::float_pow_param);
    m.def("float_pow_expression", &coek::float_pow_expression);
    m.def("float_eq_var", &coek::float_eq_var);
    m.def("float_eq_param", &coek::float_eq_param);
    m.def("float_eq_expression", &coek::float_eq_expression);
    m.def("float_le_var", &coek::float_le_var);
    m.def("float_le_param", &coek::float_le_param);
    m.def("float_le_expression", &coek::float_le_expression);
    m.def("float_ge_var", &coek::float_ge_var);
    m.def("float_ge_param", &coek::float_ge_param);
    m.def("float_ge_expression", &coek::float_ge_expression);
    m.def("var_mul_float", &coek::var_mul_float);
    m.def("var_mul_param", &coek::var_mul_param);
    m.def("var_mul_var", &coek::var_mul_var);
    m.def("var_mul_expression", &coek::var_mul_expression);
    m.def("var_add_float", &coek::var_add_float);
    m.def("var_add_param", &coek::var_add_param);
    m.def("var_add_var", &coek::var_add_var);
    m.def("var_add_expression", &coek::var_add_expression);
    m.def("var_sub_float", &coek::var_sub_float);
    m.def("var_sub_param", &coek::var_sub_param);
    m.def("var_sub_var", &coek::var_sub_var);
    m.def("var_sub_expression", &coek::var_sub_expression);
    m.def("var_div_float", &coek::var_div_float);
    m.def("var_div_param", &coek::var_div_param);
    m.def("var_div_var", &coek::var_div_var);
    m.def("var_div_expression", &coek::var_div_expression);
    m.def("var_pow_float", &coek::var_pow_float);
    m.def("var_pow_param", &coek::var_pow_param);
    m.def("var_pow_var", &coek::var_pow_var);
    m.def("var_pow_expression", &coek::var_pow_expression);
    m.def("var_eq_float", &coek::var_eq_float);
    m.def("var_eq_param", &coek::var_eq_param);
    m.def("var_eq_var", &coek::var_eq_var);
    m.def("var_eq_expression", &coek::var_eq_expression);
    m.def("var_le_float", &coek::var_le_float);
    m.def("var_le_param", &coek::var_le_param);
    m.def("var_le_var", &coek::var_le_var);
    m.def("var_le_expression", &coek::var_le_expression);
    m.def("var_ge_float", &coek::var_ge_float);
    m.def("var_ge_param", &coek::var_ge_param);
    m.def("var_ge_var", &coek::var_ge_var);
    m.def("var_ge_expression", &coek::var_ge_expression);
    m.def("expression_mul_float", &coek::expression_mul_float);
    m.def("expression_mul_param", &coek::expression_mul_param);
    m.def("expression_mul_var", &coek::expression_mul_var);
    m.def("expression_mul_expression", &coek::expression_mul_expression);
    m.def("expression_add_float", &coek::expression_add_float);
    m.def("expression_add_param", &coek::expression_add_param);
    m.def("expression_add_var", &coek::expression_add_var);
    m.def("expression_add_expression", &coek::expression_add_expression);
    m.def("expression_sub_float", &coek::expression_sub_float);
    m.def("expression_sub_param", &coek::expression_sub_param);
    m.def("expression_sub_var", &coek::expression_sub_var);
    m.def("expression_sub_expression", &coek::expression_sub_expression);
    m.def("expression_div_float", &coek::expression_div_float);
    m.def("expression_div_param", &coek::expression_div_param);
    m.def("expression_div_var", &coek::expression_div_var);
    m.def("expression_div_expression", &coek::expression_div_expression);
    m.def("expression_pow_float", &coek::expression_pow_float);
    m.def("expression_pow_param", &coek::expression_pow_param);
    m.def("expression_pow_var", &coek::expression_pow_var);
    m.def("expression_pow_expression", &coek::expression_pow_expression);
    m.def("expression_eq_float", &coek::expression_eq_float);
    m.def("expression_eq_param", &coek::expression_eq_param);
    m.def("expression_eq_var", &coek::expression_eq_var);
    m.def("expression_eq_expression", &coek::expression_eq_expression);
    m.def("expression_le_float", &coek::expression_le_float);
    m.def("expression_le_param", &coek::expression_le_param);
    m.def("expression_le_var", &coek::expression_le_var);
    m.def("expression_le_expression", &coek::expression_le_expression);
    m.def("expression_ge_float", &coek::expression_ge_float);
    m.def("expression_ge_param", &coek::expression_ge_param);
    m.def("expression_ge_var", &coek::expression_ge_var);
    m.def("expression_ge_expression", &coek::expression_ge_expression);

    //
    // Parameter
    //
    py::class_<coek::Parameter>(m, "parameter_single")
        .def(py::init<>())
        .def(py::init<const std::string&>())
        .def("get_name", [](const coek::Parameter& x) { return x.name(); })
        .def("set_name", [](coek::Parameter& x, const std::string& name) { x.name(name); })
        .def_property_readonly("name", [](const coek::Parameter& x) { return x.name(); })
        .def("set_value", [](coek::Parameter& x, double val) { x.value(val); })
        .def_property(
            "value", [](const coek::Parameter& x) { return x.value(); },
            [](coek::Parameter& x, double val) { x.value(val); })
        .def("is_constraint", [](const coek::Parameter&) { return false; })
        .def("is_expression_type", [](const coek::Parameter&) { return false; })
        .def("is_potentially_variable", [](const coek::Parameter&) { return false; })

        .def("__neg__", [](const coek::Parameter& x) { return -x; })
        .def("__pos__", [](const coek::Parameter& x) { return +x; })

        .def("__add__", [](const coek::Parameter& x, const coek::Expression& y) { return x + y; })
        .def("__add__", [](const coek::Parameter& x, const coek::Variable& y) { return x + y; })
        .def("__add__", [](const coek::Parameter& x, const coek::Parameter& y) { return x + y; })
        .def("__add__",
             [](const coek::Parameter& x, const coek::IndexParameter& y) { return x + y; })
        .def(py::self + int())
        .def(py::self + double())
        .def("__radd__", [](const coek::Parameter& x, int y) { return y + x; })
        .def("__radd__", [](const coek::Parameter& x, double y) { return y + x; })

        .def("__sub__", [](const coek::Parameter& x, const coek::Expression& y) { return x - y; })
        .def("__sub__", [](const coek::Parameter& x, const coek::Variable& y) { return x - y; })
        .def("__sub__", [](const coek::Parameter& x, const coek::Parameter& y) { return x - y; })
        .def("__sub__",
             [](const coek::Parameter& x, const coek::IndexParameter& y) { return x - y; })
        .def(py::self - int())
        .def(py::self - double())
        .def("__rsub__", [](const coek::Parameter& x, int y) { return y - x; })
        .def("__rsub__", [](const coek::Parameter& x, double y) { return y - x; })

        .def("__mul__", [](const coek::Parameter& x, const coek::Expression& y) { return x * y; })
        .def("__mul__", [](const coek::Parameter& x, const coek::Variable& y) { return x * y; })
        .def("__mul__", [](const coek::Parameter& x, const coek::Parameter& y) { return x * y; })
        .def("__mul__",
             [](const coek::Parameter& x, const coek::IndexParameter& y) { return x * y; })
        .def(py::self * int())
        .def(py::self * double())
        .def("__rmul__", [](const coek::Parameter& x, int y) { return y * x; })
        .def("__rmul__", [](const coek::Parameter& x, double y) { return y * x; })

        .def("__truediv__",
             [](const coek::Parameter& x, const coek::Expression& y) { return x / y; })
        .def("__truediv__", [](const coek::Parameter& x, const coek::Variable& y) { return x / y; })
        .def("__truediv__",
             [](const coek::Parameter& x, const coek::Parameter& y) { return x / y; })
        .def("__truediv__",
             [](const coek::Parameter& x, const coek::IndexParameter& y) { return x / y; })
        .def(py::self / int())
        .def(py::self / double())
        .def("__rtruediv__", [](const coek::Parameter& x, int y) { return y / x; })
        .def("__rtruediv__", [](const coek::Parameter& x, double y) { return y / x; })

        .def("__pow__",
             [](const coek::Parameter& x, const coek::Expression& y) { return pow(x, y); })
        .def("__pow__", [](const coek::Parameter& x, const coek::Variable& y) { return pow(x, y); })
        .def("__pow__",
             [](const coek::Parameter& x, const coek::Parameter& y) { return pow(x, y); })
        .def("__pow__",
             [](const coek::Parameter& x, const coek::IndexParameter& y) { return pow(x, y); })
        .def("__pow__", [](const coek::Parameter& x, int y) { return pow(x, y); })
        .def("__pow__", [](const coek::Parameter& x, double y) { return pow(x, y); })
        .def("__rpow__", [](const coek::Parameter& x, int y) { return pow(y, x); })
        .def("__rpow__", [](const coek::Parameter& x, double y) { return pow(y, x); })

        .def("__lt__", [](const coek::Parameter& x, const coek::Expression& y) { return x < y; })
        .def("__lt__", [](const coek::Parameter& x, const coek::Variable& y) { return x < y; })
        .def("__lt__", [](const coek::Parameter& x, const coek::Parameter& y) { return x < y; })
        .def("__lt__",
             [](const coek::Parameter& x, const coek::IndexParameter& y) { return x < y; })
        .def(py::self < int())
        .def(py::self < double())

        .def("__le__", [](const coek::Parameter& x, const coek::Expression& y) { return x <= y; })
        .def("__le__", [](const coek::Parameter& x, const coek::Variable& y) { return x <= y; })
        .def("__le__", [](const coek::Parameter& x, const coek::Parameter& y) { return x <= y; })
        .def("__le__",
             [](const coek::Parameter& x, const coek::IndexParameter& y) { return x <= y; })
        .def(py::self <= int())
        .def(py::self <= double())

        .def("__gt__", [](const coek::Parameter& x, const coek::Expression& y) { return x > y; })
        .def("__gt__", [](const coek::Parameter& x, const coek::Variable& y) { return x > y; })
        .def("__gt__", [](const coek::Parameter& x, const coek::Parameter& y) { return x > y; })
        .def("__gt__",
             [](const coek::Parameter& x, const coek::IndexParameter& y) { return x > y; })
        .def(py::self > int())
        .def(py::self > double())

        .def("__ge__", [](const coek::Parameter& x, const coek::Expression& y) { return x >= y; })
        .def("__ge__", [](const coek::Parameter& x, const coek::Variable& y) { return x >= y; })
        .def("__ge__", [](const coek::Parameter& x, const coek::Parameter& y) { return x >= y; })
        .def("__ge__",
             [](const coek::Parameter& x, const coek::IndexParameter& y) { return x >= y; })
        .def(py::self >= int())
        .def(py::self >= double())

        .def("__eq__", [](const coek::Parameter& x, const coek::Expression& y) { return x == y; })
        .def("__eq__", [](const coek::Parameter& x, const coek::Variable& y) { return x == y; })
        .def("__eq__", [](const coek::Parameter& x, const coek::Parameter& y) { return x == y; })
        .def("__eq__",
             [](const coek::Parameter& x, const coek::IndexParameter& y) { return x == y; })
        .def(py::self == int())
        .def(py::self == double())

        .def("__abs__", [](coek::Parameter& x) { return abs(x); });

    //
    // IndexParameter
    //
    py::class_<coek::IndexParameter>(m, "index")
        .def(py::init<const std::string&>())
        .def("get_name", [](const coek::IndexParameter& x) { return x.name(); })
        .def("set_name", [](coek::IndexParameter& x, const std::string& name) { x.name(name); })
        .def("get_value",
             [](const coek::IndexParameter& x) {
                 double value = 0;
                 auto flag = x.get_value(value);
                 if (flag)
                     return (double)NAN;
                 return value;
             })
        .def("set_value", [](coek::IndexParameter& x, double value) { x.value(value); })
        .def("is_constraint", [](const coek::IndexParameter&) { return false; })
        .def("is_expression_type", [](const coek::IndexParameter&) { return false; })
        .def("is_potentially_variable", [](const coek::IndexParameter&) { return false; })

        .def("__neg__", [](const coek::IndexParameter& x) { return -x; })
        .def("__pos__", [](const coek::IndexParameter& x) { return +x; })

        .def("__add__",
             [](const coek::IndexParameter& x, const coek::Expression& y) { return x + y; })
        .def("__add__",
             [](const coek::IndexParameter& x, const coek::Variable& y) { return x + y; })
        .def("__add__",
             [](const coek::IndexParameter& x, const coek::Parameter& y) { return x + y; })
        .def("__add__",
             [](const coek::IndexParameter& x, const coek::IndexParameter& y) { return x + y; })
        .def(py::self + int())
        .def(py::self + double())
        .def("__radd__", [](const coek::IndexParameter& x, int y) { return y + x; })
        .def("__radd__", [](const coek::IndexParameter& x, double y) { return y + x; })

        .def("__sub__",
             [](const coek::IndexParameter& x, const coek::Expression& y) { return x - y; })
        .def("__sub__",
             [](const coek::IndexParameter& x, const coek::Variable& y) { return x - y; })
        .def("__sub__",
             [](const coek::IndexParameter& x, const coek::Parameter& y) { return x - y; })
        .def("__sub__",
             [](const coek::IndexParameter& x, const coek::IndexParameter& y) { return x - y; })
        .def(py::self - int())
        .def(py::self - double())
        .def("__rsub__", [](const coek::IndexParameter& x, int y) { return y - x; })
        .def("__rsub__", [](const coek::IndexParameter& x, double y) { return y - x; })

        .def("__mul__",
             [](const coek::IndexParameter& x, const coek::Expression& y) { return x * y; })
        .def("__mul__",
             [](const coek::IndexParameter& x, const coek::Variable& y) { return x * y; })
        .def("__mul__",
             [](const coek::IndexParameter& x, const coek::Parameter& y) { return x * y; })
        .def("__mul__",
             [](const coek::IndexParameter& x, const coek::IndexParameter& y) { return x * y; })
        .def(py::self * int())
        .def(py::self * double())
        .def("__rmul__", [](const coek::IndexParameter& x, int y) { return y * x; })
        .def("__rmul__", [](const coek::IndexParameter& x, double y) { return y * x; })

        .def("__truediv__",
             [](const coek::IndexParameter& x, const coek::Expression& y) { return x / y; })
        .def("__truediv__",
             [](const coek::IndexParameter& x, const coek::Variable& y) { return x / y; })
        .def("__truediv__",
             [](const coek::IndexParameter& x, const coek::Parameter& y) { return x / y; })
        .def("__truediv__",
             [](const coek::IndexParameter& x, const coek::IndexParameter& y) { return x / y; })
        .def(py::self / int())
        .def(py::self / double())
        .def("__rtruediv__", [](const coek::IndexParameter& x, int y) { return y / x; })
        .def("__rtruediv__", [](const coek::IndexParameter& x, double y) { return y / x; })

        .def("__pow__",
             [](const coek::IndexParameter& x, const coek::Expression& y) { return pow(x, y); })
        .def("__pow__",
             [](const coek::IndexParameter& x, const coek::Variable& y) { return pow(x, y); })
        .def("__pow__",
             [](const coek::IndexParameter& x, const coek::Parameter& y) { return pow(x, y); })
        .def("__pow__",
             [](const coek::IndexParameter& x, const coek::IndexParameter& y) { return pow(x, y); })
        .def("__pow__", [](const coek::IndexParameter& x, int y) { return pow(x, y); })
        .def("__pow__", [](const coek::IndexParameter& x, double y) { return pow(x, y); })
        .def("__rpow__", [](const coek::IndexParameter& x, int y) { return pow(y, x); })
        .def("__rpow__", [](const coek::IndexParameter& x, double y) { return pow(y, x); })

        .def("__lt__",
             [](const coek::IndexParameter& x, const coek::Expression& y) { return x < y; })
        .def("__lt__", [](const coek::IndexParameter& x, const coek::Variable& y) { return x < y; })
        .def("__lt__",
             [](const coek::IndexParameter& x, const coek::Parameter& y) { return x < y; })
        .def("__lt__",
             [](const coek::IndexParameter& x, const coek::IndexParameter& y) { return x < y; })
        .def(py::self < int())
        .def(py::self < double())

        .def("__le__",
             [](const coek::IndexParameter& x, const coek::Expression& y) { return x <= y; })
        .def("__le__",
             [](const coek::IndexParameter& x, const coek::Variable& y) { return x <= y; })
        .def("__le__",
             [](const coek::IndexParameter& x, const coek::Parameter& y) { return x <= y; })
        .def("__le__",
             [](const coek::IndexParameter& x, const coek::IndexParameter& y) { return x <= y; })
        .def(py::self <= int())
        .def(py::self <= double())

        .def("__gt__",
             [](const coek::IndexParameter& x, const coek::Expression& y) { return x > y; })
        .def("__gt__", [](const coek::IndexParameter& x, const coek::Variable& y) { return x > y; })
        .def("__gt__",
             [](const coek::IndexParameter& x, const coek::Parameter& y) { return x > y; })
        .def("__gt__",
             [](const coek::IndexParameter& x, const coek::IndexParameter& y) { return x > y; })
        .def(py::self > int())
        .def(py::self > double())

        .def("__ge__",
             [](const coek::IndexParameter& x, const coek::Expression& y) { return x >= y; })
        .def("__ge__",
             [](const coek::IndexParameter& x, const coek::Variable& y) { return x >= y; })
        .def("__ge__",
             [](const coek::IndexParameter& x, const coek::Parameter& y) { return x >= y; })
        .def("__ge__",
             [](const coek::IndexParameter& x, const coek::IndexParameter& y) { return x >= y; })
        .def(py::self >= int())
        .def(py::self >= double())

        .def("__eq__",
             [](const coek::IndexParameter& x, const coek::Expression& y) { return x == y; })
        .def("__eq__",
             [](const coek::IndexParameter& x, const coek::Variable& y) { return x == y; })
        .def("__eq__",
             [](const coek::IndexParameter& x, const coek::Parameter& y) { return x == y; })
        .def("__eq__",
             [](const coek::IndexParameter& x, const coek::IndexParameter& y) { return x == y; })
        .def(py::self == int())
        .def(py::self == double())

        .def("__abs__", [](coek::IndexParameter& x) { return abs(x); });

    py::enum_<coek::VariableTypes>(m, "VariableTypes")
        .value("Reals", coek::VariableTypes::Reals)
        .value("Integers", coek::VariableTypes::Integers)
        .value("Boolean", coek::VariableTypes::Boolean)
        .value("Binary", coek::VariableTypes::Binary)
        .export_values();

    //
    // Variable
    //
    py::class_<coek::Variable>(m, "variable_single")
        .def(py::init<>())
#if 1
        // TODO - Check with Mike
        .def(py::init<const coek::VariableRepn&>())
#endif
        .def("get_name", [](const coek::Variable& x) { return x.name(); })
        .def("set_name", [](coek::Variable& x, const std::string& name) { x.name(name); })
        .def_property_readonly("name", [](const coek::Variable& x) { return x.name(); })
        .def_property(
            "value", [](const coek::Variable& x) { return x.value(); },
            [](coek::Variable& x, double value) { x.value(value); })
        .def_property(
            "lb", [](const coek::Variable& x) { return x.lower(); },
            [](coek::Variable& x, double value) { x.lower(value); })
        .def_property(
            "ub", [](const coek::Variable& x) { return x.upper(); },
            [](coek::Variable& x, double value) { x.upper(value); })
        .def_property(
            "fixed", [](const coek::Variable& x) { return x.fixed(); },
            [](coek::Variable& x, bool value) { x.fixed(value); })
        .def_property(
            "within", [](const coek::Variable& x) { return x.within(); },
            [](coek::Variable& x, coek::VariableTypes value) { x.within(value); })
        .def_property_readonly("id", &coek::Variable::id)
#if 1
        // TODO - Check with Mike
        //.def_readonly("repn", coek::Variable::repn)
        .def_property_readonly("repn", [](const coek::Variable& x) { return x.repn; })
#endif
        .def("is_constraint", [](const coek::Variable&) { return false; })
        .def("is_expression_type", [](const coek::Variable&) { return true; })
        .def("is_potentially_variable", [](const coek::Variable&) { return true; })
        .def("is_continuous", [](const coek::Variable& x) { return x.is_continuous(); })
        .def("is_integer", [](const coek::Variable& x) { return x.is_integer(); })
        .def("is_binary", [](const coek::Variable& x) { return x.is_binary(); })

        .def("__neg__", [](const coek::Variable& x) { return -x; })
        .def("__pos__", [](const coek::Variable& x) { return +x; })

        .def(
            "__add__", [](const coek::Variable& x, const coek::Expression& y) { return x + y; },
            py::is_operator())
        .def(
            "__add__", [](const coek::Variable& x, const coek::Variable& y) { return x + y; },
            py::is_operator())
        .def(
            "__add__", [](const coek::Variable& x, const coek::Parameter& y) { return x + y; },
            py::is_operator())
        .def(
            "__add__", [](const coek::Variable& x, const coek::IndexParameter& y) { return x + y; },
            py::is_operator())
        .def(py::self + int())
        .def(py::self + double())
        .def("__radd__", [](const coek::Variable& x, int y) { return y + x; })
        .def("__radd__", [](const coek::Variable& x, double y) { return y + x; })

        .def("__sub__", [](const coek::Variable& x, const coek::Expression& y) { return x - y; })
        .def("__sub__", [](const coek::Variable& x, const coek::Variable& y) { return x - y; })
        .def("__sub__", [](const coek::Variable& x, const coek::Parameter& y) { return x - y; })
        .def("__sub__",
             [](const coek::Variable& x, const coek::IndexParameter& y) { return x - y; })
        .def(py::self - int())
        .def(py::self - double())
        .def("__rsub__", [](const coek::Variable& x, int y) { return y - x; })
        .def("__rsub__", [](const coek::Variable& x, double y) { return y - x; })

        .def("__mul__", [](const coek::Variable& x, const coek::Expression& y) { return x * y; })
        .def("__mul__", [](const coek::Variable& x, const coek::Variable& y) { return x * y; })
        .def("__mul__", [](const coek::Variable& x, const coek::Parameter& y) { return x * y; })
        .def("__mul__",
             [](const coek::Variable& x, const coek::IndexParameter& y) { return x * y; })
        .def(py::self * int())
        .def(py::self * double())
        .def("__rmul__", [](const coek::Variable& x, int y) { return y * x; })
        .def("__rmul__", [](const coek::Variable& x, double y) { return y * x; })

        .def("__truediv__",
             [](const coek::Variable& x, const coek::Expression& y) { return x / y; })
        .def("__truediv__", [](const coek::Variable& x, const coek::Variable& y) { return x / y; })
        .def("__truediv__", [](const coek::Variable& x, const coek::Parameter& y) { return x / y; })
        .def("__truediv__",
             [](const coek::Variable& x, const coek::IndexParameter& y) { return x / y; })
        .def(py::self / int())
        .def(py::self / double())
        .def("__rtruediv__", [](const coek::Variable& x, int y) { return y / x; })
        .def("__rtruediv__", [](const coek::Variable& x, double y) { return y / x; })

        .def("__pow__",
             [](const coek::Variable& x, const coek::Expression& y) { return pow(x, y); })
        .def("__pow__", [](const coek::Variable& x, const coek::Variable& y) { return pow(x, y); })
        .def("__pow__", [](const coek::Variable& x, const coek::Parameter& y) { return pow(x, y); })
        .def("__pow__",
             [](const coek::Variable& x, const coek::IndexParameter& y) { return pow(x, y); })
        .def("__pow__", [](const coek::Variable& x, int y) { return pow(x, y); })
        .def("__pow__", [](const coek::Variable& x, double y) { return pow(x, y); })
        .def("__rpow__", [](const coek::Variable& x, int y) { return pow(y, x); })
        .def("__rpow__", [](const coek::Variable& x, double y) { return pow(y, x); })

        .def("__lt__", [](const coek::Variable& x, const coek::Expression& y) { return x < y; })
        .def("__lt__", [](const coek::Variable& x, const coek::Variable& y) { return x < y; })
        .def("__lt__", [](const coek::Variable& x, const coek::Parameter& y) { return x < y; })
        .def("__lt__", [](const coek::Variable& x, const coek::IndexParameter& y) { return x < y; })
        .def(py::self < int())
        .def(py::self < double())

        .def("__le__", [](const coek::Variable& x, const coek::Expression& y) { return x <= y; })
        .def("__le__", [](const coek::Variable& x, const coek::Variable& y) { return x <= y; })
        .def("__le__", [](const coek::Variable& x, const coek::Parameter& y) { return x <= y; })
        .def("__le__",
             [](const coek::Variable& x, const coek::IndexParameter& y) { return x <= y; })
        .def(py::self <= int())
        .def(py::self <= double())

        .def("__gt__", [](const coek::Variable& x, const coek::Expression& y) { return x > y; })
        .def("__gt__", [](const coek::Variable& x, const coek::Variable& y) { return x > y; })
        .def("__gt__", [](const coek::Variable& x, const coek::Parameter& y) { return x > y; })
        .def("__gt__", [](const coek::Variable& x, const coek::IndexParameter& y) { return x > y; })
        .def(py::self > int())
        .def(py::self > double())

        .def("__ge__", [](const coek::Variable& x, const coek::Expression& y) { return x >= y; })
        .def("__ge__", [](const coek::Variable& x, const coek::Variable& y) { return x >= y; })
        .def("__ge__", [](const coek::Variable& x, const coek::Parameter& y) { return x >= y; })
        .def("__ge__",
             [](const coek::Variable& x, const coek::IndexParameter& y) { return x >= y; })
        .def(py::self >= int())
        .def(py::self >= double())

        .def("__eq__", [](const coek::Variable& x, const coek::Expression& y) { return x == y; })
        .def("__eq__", [](const coek::Variable& x, const coek::Variable& y) { return x == y; })
        .def("__eq__", [](const coek::Variable& x, const coek::Parameter& y) { return x == y; })
        .def("__eq__",
             [](const coek::Variable& x, const coek::IndexParameter& y) { return x == y; })
        .def(py::self == int())
        .def(py::self == double())

        .def("__abs__", [](coek::Variable& x) { return abs(x); })

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
        .def("__len__", [](const coek::VariableArray& va) { return va.size(); })
        .def("__getitem__", [](coek::VariableArray& va, int i) { return va(i); })
        .def("__getitem__",
             [](coek::VariableArray& va, std::vector<int>& index) {
                 coek::IndexVector::vecdata_t* data
                     = new coek::IndexVector::vecdata_t[index.size()];
                 coek::IndexVector tmp(data, index.size());
                 for (size_t i = 0; i < index.size(); ++i)
                     tmp[i] = static_cast<size_t>(index[i]);
                 return va.index(tmp);
             })
        .def_property_readonly("name",
                               [](coek::VariableArray& x) -> py::object {
                                   if (x.name().size() == 0)
                                       return py::cast<std::string>("X");
                                   else
                                       return py::cast<std::string>(x.name());
                               })
        .def("is_constraint", [](const coek::VariableArray&) { return false; })
        .def("is_expression_type", [](const coek::VariableArray&) { return false; })
        .def("is_potentially_variable", [](const coek::VariableArray&) { return false; })
        .def("generate_names", [](coek::VariableArray& x) { return x.generate_names(); })
        .def(
            "__iter__",
            [](const coek::VariableArray& va) {
                typedef coek::VecKeyIterator<std::vector<coek::Variable>::const_iterator> vec_key_t;
                return py::make_iterator(vec_key_t(va.cbegin()), vec_key_t(va.cend()));
                /*
                if (va.dim() == 0)
                     return py::make_iterator(vec_key_t(va.begin()), vec_key_t(va.end()));
                else
                     return py::make_iterator(va.indexed_begin(), va.indexed_end());
                */
            },
            py::keep_alive<0, 1>());

    //
    // VariableMap
    //
#ifdef COEK_WITH_COMPACT_MODEL
    py::class_<coek::VariableMap>(m, "variable_map")
        .def("__len__", [](const coek::VariableMap& x) { return x.size(); })
        .def("__getitem__",
             [](coek::VariableMap& x, py::args args) {
                 return coek::Array_getitem<coek::VariableMap>(x, args);
             })
        .def("generate_names", [](coek::VariableMap& x) { return x.generate_names(); })
        //.def_property_readonly("name", [](coek::VariableMap& x) -> py::object {
        //        return py::cast<std::string>(x.name());
        //        })
        ;

    m.def("SetOf", [](std::vector<int>& arg) { return coek::SetOf(arg); });
    m.def("RangeSet",
          [](int start, int stop, int step = 1) { return coek::RangeSet(start, stop, step); });
    m.def("RangeSet", [](int start, int stop) { return coek::RangeSet(start, stop); });

#    if 0
    m.def("VariableMap",[](const coek::ConcreteSet& arg, double lb=-COEK_INFINITY, double ub=COEK_INFINITY, double value=0.0) {return coek::variable(arg).bounds(lb, ub).value(value);});
    m.def("VariableMap",[](const coek::ConcreteSet& arg, double lb=-COEK_INFINITY, double ub=COEK_INFINITY) {return coek::variable(arg).bounds(lb, ub);});
    m.def("VariableMap",[](const coek::ConcreteSet& arg) {return coek::VariableMap(arg);});
    m.def("VariableMap",[](const coek::ConcreteSet& arg, const std::string& name) {return coek::variable(arg).name(name);});
    m.def("VariableMap",[](const coek::ConcreteSet& arg, double lb, double ub, double value, const std::string& name) {return coek::variable(arg).bounds(lb, ub).value(value).name(name);});
    m.def("VariableMap",[](const coek::ConcreteSet& arg, double lb, double ub, double value, bool binary, bool integer, const std::string& name) 
                                    {coek::VariableTypes domain=coek::VariableTypes::Reals;
                                    if (binary) domain=coek::VariableTypes::Binary;
                                    else if (integer) domain=coek::VariableTypes::Integers;
                                    return coek::VariableMap(arg).bounds(lb, ub).value(value).within(domain).name(name);});
    m.def("VariableMap",[](const coek::ConcreteSet& arg, double lb, double ub, double value, bool binary, bool integer) 
                                    {coek::VariableTypes domain=coek::VariableTypes::Reals;
                                    if (binary) domain=coek::VariableTypes::Binary;
                                    else if (integer) domain=coek::VariableTypes::Integers;
                                    return coek::VariableMap(arg).bounds(lb, ub).value(value).within(domain);});
#    endif

    //
    // ConcreteSet
    //
    py::class_<coek::ConcreteSet>(m, "ConcreteSet")
        .def("dim", &coek::ConcreteSet::dim)
        .def("__add__", [](coek::ConcreteSet& x, coek::ConcreteSet& y) { return x + y; })
        .def("__sub__", [](coek::ConcreteSet& x, coek::ConcreteSet& y) { return x - y; })
        .def("__mul__", [](coek::ConcreteSet& x, coek::ConcreteSet& y) { return x * y; })
        .def("__and__", [](coek::ConcreteSet& x, coek::ConcreteSet& y) { return x & y; })
        .def("__or__", [](coek::ConcreteSet& x, coek::ConcreteSet& y) { return x | y; })
        .def("__xor__", [](coek::ConcreteSet& x, coek::ConcreteSet& y) { return x ^ y; });

#endif

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
        .def("is_constraint", [](const coek::Expression&) { return false; })
        .def("is_expression_type", [](const coek::Expression&) { return true; })
        .def("is_potentially_variable", [](const coek::Expression&) { return true; })
        .def("is_numeric_type", [](const coek::Expression&) { return true; })
        .def("__call__", &coek::expression_eval, py::arg("exception") = false)
        .def("is_variable_type", [](const coek::Expression&) { return false; })

        .def("__neg__", [](const coek::Expression& x) { return -x; })
        .def("__pos__", [](const coek::Expression& x) { return +x; })

        .def(
            "__add__", [](const coek::Expression& x, const coek::Expression& y) { return x + y; },
            py::is_operator())
        .def(
            "__add__", [](const coek::Expression& x, const coek::Variable& y) { return x + y; },
            py::is_operator())
        .def(
            "__add__", [](const coek::Expression& x, const coek::Parameter& y) { return x + y; },
            py::is_operator())
        .def(
            "__add__",
            [](const coek::Expression& x, const coek::IndexParameter& y) { return x + y; },
            py::is_operator())
        .def(py::self + int())
        .def(py::self + double())
        .def(
            "__radd__", [](const coek::Expression& x, int y) { return y + x; }, py::is_operator())
        .def(
            "__radd__", [](const coek::Expression& x, double y) { return y + x; },
            py::is_operator())

        .def(
            "__sub__", [](const coek::Expression& x, const coek::Expression& y) { return x - y; },
            py::is_operator())
        .def(
            "__sub__", [](const coek::Expression& x, const coek::Variable& y) { return x - y; },
            py::is_operator())
        .def(
            "__sub__", [](const coek::Expression& x, const coek::Parameter& y) { return x - y; },
            py::is_operator())
        .def(
            "__sub__",
            [](const coek::Expression& x, const coek::IndexParameter& y) { return x - y; },
            py::is_operator())
        .def(py::self - int())
        .def(py::self - double())
        .def(
            "__rsub__", [](const coek::Expression& x, int y) { return y - x; }, py::is_operator())
        .def(
            "__rsub__", [](const coek::Expression& x, double y) { return y - x; },
            py::is_operator())

        .def(
            "__mul__", [](const coek::Expression& x, const coek::Expression& y) { return x * y; },
            py::is_operator())
        .def(
            "__mul__", [](const coek::Expression& x, const coek::Variable& y) { return x * y; },
            py::is_operator())
        .def(
            "__mul__", [](const coek::Expression& x, const coek::Parameter& y) { return x * y; },
            py::is_operator())
        .def(
            "__mul__",
            [](const coek::Expression& x, const coek::IndexParameter& y) { return x * y; },
            py::is_operator())
        .def(py::self * int())
        .def(py::self * double())
        .def(
            "__rmul__", [](const coek::Expression& x, int y) { return y * x; }, py::is_operator())
        .def(
            "__rmul__", [](const coek::Expression& x, double y) { return y * x; },
            py::is_operator())

        .def(
            "__truediv__",
            [](const coek::Expression& x, const coek::Expression& y) { return x / y; },
            py::is_operator())
        .def(
            "__truediv__", [](const coek::Expression& x, const coek::Variable& y) { return x / y; },
            py::is_operator())
        .def(
            "__truediv__",
            [](const coek::Expression& x, const coek::Parameter& y) { return x / y; },
            py::is_operator())
        .def(
            "__truediv__",
            [](const coek::Expression& x, const coek::IndexParameter& y) { return x / y; },
            py::is_operator())
        .def(py::self / int())
        .def(py::self / double())
        .def(
            "__rtruediv__", [](const coek::Expression& x, int y) { return y / x; },
            py::is_operator())
        .def(
            "__rtruediv__", [](const coek::Expression& x, double y) { return y / x; },
            py::is_operator())

        .def(
            "__pow__",
            [](const coek::Expression& x, const coek::Expression& y) { return pow(x, y); },
            py::is_operator())
        .def(
            "__pow__", [](const coek::Expression& x, const coek::Variable& y) { return pow(x, y); },
            py::is_operator())
        .def(
            "__pow__",
            [](const coek::Expression& x, const coek::Parameter& y) { return pow(x, y); },
            py::is_operator())
        .def(
            "__pow__",
            [](const coek::Expression& x, const coek::IndexParameter& y) { return pow(x, y); },
            py::is_operator())
        .def(
            "__pow__", [](const coek::Expression& x, int y) { return pow(x, y); },
            py::is_operator())
        .def(
            "__pow__", [](const coek::Expression& x, double y) { return pow(x, y); },
            py::is_operator())
        .def(
            "__rpow__", [](const coek::Expression& x, int y) { return pow(y, x); },
            py::is_operator())
        .def(
            "__rpow__", [](const coek::Expression& x, double y) { return pow(y, x); },
            py::is_operator())

        .def(
            "__lt__", [](const coek::Expression& x, const coek::Expression& y) { return x < y; },
            py::is_operator())
        .def(
            "__lt__", [](const coek::Expression& x, const coek::Variable& y) { return x < y; },
            py::is_operator())
        .def(
            "__lt__", [](const coek::Expression& x, const coek::Parameter& y) { return x < y; },
            py::is_operator())
        .def(
            "__lt__",
            [](const coek::Expression& x, const coek::IndexParameter& y) { return x < y; },
            py::is_operator())
        .def(py::self < int())
        .def(py::self < double())

        .def(
            "__le__", [](const coek::Expression& x, const coek::Expression& y) { return x <= y; },
            py::is_operator())
        .def(
            "__le__", [](const coek::Expression& x, const coek::Variable& y) { return x <= y; },
            py::is_operator())
        .def(
            "__le__", [](const coek::Expression& x, const coek::Parameter& y) { return x <= y; },
            py::is_operator())
        .def(
            "__le__",
            [](const coek::Expression& x, const coek::IndexParameter& y) { return x <= y; },
            py::is_operator())
        .def(py::self <= int())
        .def(py::self <= double())

        .def(
            "__gt__", [](const coek::Expression& x, const coek::Expression& y) { return x > y; },
            py::is_operator())
        .def(
            "__gt__", [](const coek::Expression& x, const coek::Variable& y) { return x > y; },
            py::is_operator())
        .def(
            "__gt__", [](const coek::Expression& x, const coek::Parameter& y) { return x > y; },
            py::is_operator())
        .def(
            "__gt__",
            [](const coek::Expression& x, const coek::IndexParameter& y) { return x > y; },
            py::is_operator())
        .def(py::self > int())
        .def(py::self > double())

        .def(
            "__ge__", [](const coek::Expression& x, const coek::Expression& y) { return x >= y; },
            py::is_operator())
        .def(
            "__ge__", [](const coek::Expression& x, const coek::Variable& y) { return x >= y; },
            py::is_operator())
        .def(
            "__ge__", [](const coek::Expression& x, const coek::Parameter& y) { return x >= y; },
            py::is_operator())
        .def(
            "__ge__",
            [](const coek::Expression& x, const coek::IndexParameter& y) { return x >= y; },
            py::is_operator())
        .def(py::self >= int())
        .def(py::self >= double())

        .def(
            "__eq__", [](const coek::Expression& x, const coek::Expression& y) { return x == y; },
            py::is_operator())
        .def(
            "__eq__", [](const coek::Expression& x, const coek::Variable& y) { return x == y; },
            py::is_operator())
        .def(
            "__eq__", [](const coek::Expression& x, const coek::Parameter& y) { return x == y; },
            py::is_operator())
        .def(
            "__eq__",
            [](const coek::Expression& x, const coek::IndexParameter& y) { return x == y; },
            py::is_operator())
        .def(py::self == int())
        .def(py::self == double())

        .def(
            "__iadd__", [](coek::Expression& x, const coek::Expression& y) { return x += y; },
            py::is_operator())
        .def(
            "__iadd__", [](coek::Expression& x, const coek::Variable& y) { return x += y; },
            py::is_operator())
        .def(
            "__iadd__", [](coek::Expression& x, const coek::Parameter& y) { return x += y; },
            py::is_operator())
        .def(
            "__iadd__", [](coek::Expression& x, const coek::IndexParameter& y) { return x += y; },
            py::is_operator())
        .def(
            "__iadd__", [](coek::Expression& x, int y) { return x += y; }, py::is_operator())
        .def(
            "__iadd__", [](coek::Expression& x, double y) { return x += y; }, py::is_operator())

        .def(
            "__isub__", [](coek::Expression& x, const coek::Expression& y) { return x -= y; },
            py::is_operator())
        .def(
            "__isub__", [](coek::Expression& x, const coek::Variable& y) { return x -= y; },
            py::is_operator())
        .def(
            "__isub__", [](coek::Expression& x, const coek::Parameter& y) { return x -= y; },
            py::is_operator())
        .def(
            "__isub__", [](coek::Expression& x, const coek::IndexParameter& y) { return x -= y; },
            py::is_operator())
        .def(
            "__isub__", [](coek::Expression& x, int y) { return x -= y; }, py::is_operator())
        .def(
            "__isub__", [](coek::Expression& x, double y) { return x -= y; }, py::is_operator())

        .def(
            "__imul__", [](coek::Expression& x, const coek::Expression& y) { return x *= y; },
            py::is_operator())
        .def(
            "__imul__", [](coek::Expression& x, const coek::Variable& y) { return x *= y; },
            py::is_operator())
        .def(
            "__imul__", [](coek::Expression& x, const coek::Parameter& y) { return x *= y; },
            py::is_operator())
        .def(
            "__imul__", [](coek::Expression& x, const coek::IndexParameter& y) { return x *= y; },
            py::is_operator())
        .def(
            "__imul__", [](coek::Expression& x, int y) { return x *= y; }, py::is_operator())
        .def(
            "__imul__", [](coek::Expression& x, double y) { return x *= y; }, py::is_operator())

        .def(
            "__itruediv__", [](coek::Expression& x, const coek::Expression& y) { return x /= y; },
            py::is_operator())
        .def(
            "__itruediv__", [](coek::Expression& x, const coek::Variable& y) { return x /= y; },
            py::is_operator())
        .def(
            "__itruediv__", [](coek::Expression& x, const coek::Parameter& y) { return x /= y; },
            py::is_operator())
        .def(
            "__itruediv__",
            [](coek::Expression& x, const coek::IndexParameter& y) { return x /= y; },
            py::is_operator())
        .def(
            "__itruediv__", [](coek::Expression& x, int y) { return x /= y; }, py::is_operator())
        .def(
            "__itruediv__", [](coek::Expression& x, double y) { return x /= y; }, py::is_operator())

        .def(
            "__abs__", [](coek::Expression& x) { return coek::abs(x); }, py::is_operator())

        //.def("__bool__", [](coek::Expression& x){return x.get_value() != 0;})
        //.def("__int__", [](coek::Expression& x){return int(x.get_value());})
        //.def("__float__", [](coek::Expression& x){return x.get_value();})

        .def("to_list",
             [](coek::Expression& x) {
                 auto tmp = x.to_list();
                 auto begin = tmp.begin();
                 auto end = tmp.end();
                 if (begin == end)
                     return py::list();
                 if (tmp.size() == 1) {
                     auto ans = py::list();
                     ans.append(*begin);
                     return ans;
                 }
                 begin++;
                 return coek::to_nested_list(begin, end);
             })
        .def("expand", [](coek::Expression& x) { return x.expand(); });

    //
    // Objective
    //
    py::class_<coek::Objective>(m, "objective")
        .def(py::init<>())
        .def_property_readonly("value", [](coek::Objective& c) { return c.value(); })
        .def_property_readonly("id", &coek::Objective::id)
        .def_property_readonly(
            "name", [](coek::Objective& c) { return std::string("O") + std::to_string(c.id()); })

        .def("to_list", [](coek::Objective& x) {
            auto tmp = x.to_list();
            auto begin = tmp.begin();
            auto end = tmp.end();
            if (begin == end)
                return py::list();
            if (tmp.size() == 1) {
                auto ans = py::list();
                ans.append(*begin);
                return ans;
            }
            begin++;
            return coek::to_nested_list(begin, end);
        });

    //
    // Constraint
    //
    py::class_<coek::Constraint>(m, "constraint")
        .def(py::init<>())
        .def_property_readonly("value", [](coek::Constraint& c) { return c.body().value(); })
        .def("__call__", [](const coek::Constraint& c, py::kwargs) { return c.is_feasible(); })
        .def_property_readonly("lb", [](coek::Constraint& c) { return coek::constraint_lb(c); })
        .def_property_readonly("ub", [](coek::Constraint& c) { return coek::constraint_ub(c); })
        .def("lower", &coek::Constraint::lower)
        .def("body", &coek::Constraint::body)
        .def("upper", &coek::Constraint::upper)
        .def("has_lower", &coek::Constraint::has_lower)
        .def("has_upper", &coek::Constraint::has_upper)
        .def("is_inequality", &coek::Constraint::is_inequality)
        .def("is_equality", &coek::Constraint::is_equality)
        .def_property_readonly("id", &coek::Constraint::id)
        .def_property_readonly(
            "name", [](coek::Constraint& c) { return std::string("C") + std::to_string(c.id()); })
        .def("is_feasible", &coek::Constraint::is_feasible)
        .def("is_constraint", [](const coek::Constraint&) { return true; })
        .def("is_expression_type", [](const coek::Constraint&) { return false; })
        .def("__eq__",
             []() {
                 throw std::runtime_error("Cannot create a constraint from a boolean expression.");
             })
        .def("__le__",
             []() {
                 throw std::runtime_error("Cannot create a constraint from a boolean expression.");
             })
        .def("__lt__",
             []() {
                 throw std::runtime_error("Cannot create a constraint from a boolean expression.");
             })
        .def("__ge__",
             []() {
                 throw std::runtime_error("Cannot create a constraint from a boolean expression.");
             })
        .def("__gt__",
             []() {
                 throw std::runtime_error("Cannot create a constraint from a boolean expression.");
             })

        .def("to_list",
             [](coek::Constraint& x) {
                 auto tmp = x.to_list();
                 auto begin = tmp.begin();
                 auto end = tmp.end();
                 if (begin == end)
                     return py::list();
                 if (tmp.size() == 1) {
                     auto ans = py::list();
                     ans.append(*begin);
                     return ans;
                 }
                 begin++;
                 return coek::to_nested_list(begin, end);
             })
        .def("expand", [](coek::Constraint& x) { return x.expand(); });

#if 0
    // TODO - Check with Mike

    py::class_<coek::QuadraticExpr>(m, "QuadraticExpr")
        .def_readwrite("linear_vars", &coek::QuadraticExpr::linear_vars)
        .def_readwrite("linear_coefs", &coek::QuadraticExpr::linear_coefs)
        .def_readwrite("quadratic_lvars", &coek::QuadraticExpr::quadratic_lvars)
        .def_readwrite("quadratic_rvars", &coek::QuadraticExpr::quadratic_rvars)
        .def_readwrite("quadratic_coefs", &coek::QuadraticExpr::quadratic_coefs)
        .def_readwrite("constval", &coek::QuadraticExpr::constval)
        .def("is_constant", &coek::QuadraticExpr::is_constant)
        .def("is_linear", &coek::QuadraticExpr::is_linear)
        .def("is_quadratic", &coek::QuadraticExpr::is_quadratic);
#endif

#if 1
    py::class_<coek::VariableTerm>(m, "VariableTerm")
        .def_readonly("index", &coek::VariableTerm::index);
#endif

    //
    // Intrinsics
    //
    m.def("ceil", [](coek::Expression& x) { return coek::ceil(x); });
    m.def("ceil", [](coek::Variable& x) { return coek::ceil(x); });
    m.def("ceil", [](coek::Parameter& x) { return coek::ceil(x); });
    m.def("ceil", [](coek::IndexParameter& x) { return coek::ceil(x); });
    m.def("floor", [](coek::Expression& x) { return coek::floor(x); });
    m.def("floor", [](coek::Variable& x) { return coek::floor(x); });
    m.def("floor", [](coek::Parameter& x) { return coek::floor(x); });
    m.def("floor", [](coek::IndexParameter& x) { return coek::floor(x); });
    m.def("exp", [](coek::Expression& x) { return coek::exp(x); });
    m.def("exp", [](coek::Variable& x) { return coek::exp(x); });
    m.def("exp", [](coek::Parameter& x) { return coek::exp(x); });
    m.def("exp", [](coek::IndexParameter& x) { return coek::exp(x); });
    m.def("log", [](coek::Expression& x) { return coek::log(x); });
    m.def("log", [](coek::Variable& x) { return coek::log(x); });
    m.def("log", [](coek::Parameter& x) { return coek::log(x); });
    m.def("log", [](coek::IndexParameter& x) { return coek::log(x); });
    m.def("log10", [](coek::Expression& x) { return coek::log10(x); });
    m.def("log10", [](coek::Variable& x) { return coek::log10(x); });
    m.def("log10", [](coek::Parameter& x) { return coek::log10(x); });
    m.def("log10", [](coek::IndexParameter& x) { return coek::log10(x); });
    m.def("sqrt", [](coek::Expression& x) { return coek::sqrt(x); });
    m.def("sqrt", [](coek::Variable& x) { return coek::sqrt(x); });
    m.def("sqrt", [](coek::Parameter& x) { return coek::sqrt(x); });
    m.def("sqrt", [](coek::IndexParameter& x) { return coek::sqrt(x); });
    m.def("sin", [](coek::Expression& x) { return coek::sin(x); });
    m.def("sin", [](coek::Variable& x) { return coek::sin(x); });
    m.def("sin", [](coek::Parameter& x) { return coek::sin(x); });
    m.def("sin", [](coek::IndexParameter& x) { return coek::sin(x); });
    m.def("cos", [](coek::Expression& x) { return coek::cos(x); });
    m.def("cos", [](coek::Variable& x) { return coek::cos(x); });
    m.def("cos", [](coek::Parameter& x) { return coek::cos(x); });
    m.def("cos", [](coek::IndexParameter& x) { return coek::cos(x); });
    m.def("tan", [](coek::Expression& x) { return coek::tan(x); });
    m.def("tan", [](coek::Variable& x) { return coek::tan(x); });
    m.def("tan", [](coek::Parameter& x) { return coek::tan(x); });
    m.def("tan", [](coek::IndexParameter& x) { return coek::tan(x); });
    m.def("sinh", [](coek::Expression& x) { return coek::sinh(x); });
    m.def("sinh", [](coek::Variable& x) { return coek::sinh(x); });
    m.def("sinh", [](coek::Parameter& x) { return coek::sinh(x); });
    m.def("sinh", [](coek::IndexParameter& x) { return coek::sinh(x); });
    m.def("cosh", [](coek::Expression& x) { return coek::cosh(x); });
    m.def("cosh", [](coek::Variable& x) { return coek::cosh(x); });
    m.def("cosh", [](coek::Parameter& x) { return coek::cosh(x); });
    m.def("cosh", [](coek::IndexParameter& x) { return coek::cosh(x); });
    m.def("tanh", [](coek::Expression& x) { return coek::tanh(x); });
    m.def("tanh", [](coek::Variable& x) { return coek::tanh(x); });
    m.def("tanh", [](coek::Parameter& x) { return coek::tanh(x); });
    m.def("tanh", [](coek::IndexParameter& x) { return coek::tanh(x); });
    m.def("asin", [](coek::Expression& x) { return coek::asin(x); });
    m.def("asin", [](coek::Variable& x) { return coek::asin(x); });
    m.def("asin", [](coek::Parameter& x) { return coek::asin(x); });
    m.def("asin", [](coek::IndexParameter& x) { return coek::asin(x); });
    m.def("acos", [](coek::Expression& x) { return coek::acos(x); });
    m.def("acos", [](coek::Variable& x) { return coek::acos(x); });
    m.def("acos", [](coek::Parameter& x) { return coek::acos(x); });
    m.def("acos", [](coek::IndexParameter& x) { return coek::acos(x); });
    m.def("atan", [](coek::Expression& x) { return coek::atan(x); });
    m.def("atan", [](coek::Variable& x) { return coek::atan(x); });
    m.def("atan", [](coek::Variable& x) { return coek::atan(x); });
    m.def("atan", [](coek::Parameter& x) { return coek::atan(x); });
    m.def("atan", [](coek::IndexParameter& x) { return coek::atan(x); });
    m.def("asinh", [](coek::Expression& x) { return coek::asinh(x); });
    m.def("asinh", [](coek::Variable& x) { return coek::asinh(x); });
    m.def("asinh", [](coek::Parameter& x) { return coek::asinh(x); });
    m.def("asinh", [](coek::IndexParameter& x) { return coek::asinh(x); });
    m.def("acosh", [](coek::Expression& x) { return coek::acosh(x); });
    m.def("acosh", [](coek::Variable& x) { return coek::acosh(x); });
    m.def("acosh", [](coek::Parameter& x) { return coek::acosh(x); });
    m.def("acosh", [](coek::IndexParameter& x) { return coek::acosh(x); });
    m.def("atanh", [](coek::Expression& x) { return coek::atanh(x); });
    m.def("atanh", [](coek::Variable& x) { return coek::atanh(x); });
    m.def("atanh", [](coek::Parameter& x) { return coek::atanh(x); });
    m.def("atanh", [](coek::IndexParameter& x) { return coek::atanh(x); });

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
        .def("add_variable_", [](coek::Model& m, coek::Variable& v) { return m.add_variable(v); })
        .def("add_variable_", [](coek::Model& m, coek::VariableArray& v) { m.add(v); })
#ifdef COEK_WITH_COMPACT_MODEL
        .def("add_variable_", [](coek::Model& m, coek::VariableMap& v) { m.add(v); })
#endif
        .def("add_objective",
             [](coek::Model& m, double f) {
                 coek::Expression e(f);
                 return m.add_objective(e);
             })
        .def("add_objective",
             [](coek::Model& m, const coek::Expression& e) { return m.add_objective(e); })
        .def("add_objective",
             [](coek::Model& m, const coek::Parameter& e) { return m.add_objective(e); })
        .def("add_objective",
             [](coek::Model& m, const coek::Variable& e) { return m.add_objective(e); })
        .def("add_objective",
             [](coek::Model& m, const coek::Expression& e, bool sense) {
                 return m.add(coek::Objective().expr(e).sense(sense));
             })
        .def("add_objective",
             [](coek::Model& m, const coek::Variable& e, bool sense) {
                 return m.add(coek::Objective().expr(e).sense(sense));
             })
        .def("add_constraint", [](coek::Model& m, coek::Constraint& c) { return m.add(c); })

        .def("get_objective", [](coek::Model& m) { return m.get_objective(); })
        .def("get_objective",
             [](coek::Model& m, int i) {
                 if (i < 0)
                     throw std::invalid_argument("Index to objective must be non-negative");
                 return m.get_objective(static_cast<size_t>(i));
             })
        .def("get_constraint",
             [](coek::Model& m, int i) {
                 if (i < 0)
                     throw std::invalid_argument("Index to objective must be non-negative");
                 return m.get_constraint(static_cast<size_t>(i));
             })
        .def("num_variables", [](coek::Model& m) { return m.num_variables(); })
        .def("num_objectives", [](coek::Model& m) { return m.num_objectives(); })
        .def("num_constraints", [](coek::Model& m) { return m.num_constraints(); })

        .def("write", [](coek::Model& m, const std::string& s) { m.write(s); })
        //.def("write", [](coek::Model& m, const std::string& s, std::map<int,int>& varmap,
        // std::map<int,int>& conmap){m.write(s,varmap,conmap);})
        .def("print_equations_", [](coek::Model& m) { m.print_equations(); })
        .def("print_equations_",
             [](coek::Model& m, bool) {
                 std::stringstream ostr;
                 m.print_equations(ostr);
                 return ostr.str();
             })
        .def("print_values_", [](coek::Model& m) { m.print_values(); })
        .def("print_values_", [](coek::Model& m, bool) {
            std::stringstream ostr;
            m.print_values(ostr);
            return ostr.str();
        });

    //
    // Functions for Compact Expressions
    //
#ifdef COEK_WITH_COMPACT_MODEL
    m.def("Sum", [](const coek::Expression& expr, const coek::SequenceContext& context) {
        return coek::Sum(context, expr);
    });
#    if 0
    m.def("Sum",[](const coek::Variable& expr, const coek::SequenceContext& context) {return coek::Sum(expr, context);});
#    endif

    m.def("Forall", [](py::args args) {
        std::vector<coek::IndexParameter> indices;
        for (py::handle h : args)
            indices.push_back(h.cast<coek::IndexParameter>());
        return coek::Forall(indices);
    });
    //
    // SequenceContext
    //
    py::class_<coek::SequenceContext>(m, "SequenceContext")
        .def("Forall",
             [](coek::SequenceContext& x, py::args args) {
                 std::vector<coek::IndexParameter> indices;
                 for (py::handle h : args)
                     indices.push_back(h.cast<coek::IndexParameter>());
                 return x.Forall(indices);
             })
        .def("In",
             [](coek::SequenceContext& x, coek::ConcreteSet& context) { return x.In(context); })
        .def("ST", [](coek::SequenceContext& x, coek::Constraint& con) { return x.ST(con); })
        .def("Where", [](coek::SequenceContext& x, coek::Constraint& con) { return x.Where(con); });

    py::class_<coek::ExpressionSequence>(m, "ExpressionSequence")
        .def(py::init<const coek::SequenceContext&, const coek::Expression&>())
        .def("__iter__", [](const coek::ExpressionSequence& seq) {
            return py::make_iterator(seq.begin(), seq.end());
        });

    py::class_<coek::ConstraintSequence>(m, "ConstraintSequence")
        .def(py::init<const coek::SequenceContext&, const coek::Constraint&>())
        .def("__iter__", [](const coek::ConstraintSequence& seq) {
            return py::make_iterator(seq.begin(), seq.end());
        });

    //
    // CompactModel
    //
    py::class_<coek::CompactModel>(m, "compact_model")
        .def(py::init<>())
#    if 0
        .def("add_variable_",[](coek::CompactModel& m, py::kwargs kw)
            {
            coek::Variable v = coek::variable_fn(kw);
            return m.add_variable(v);
            })
#    endif
        .def("add_variable_", [](coek::CompactModel& m, coek::Variable& v) { m.add_variable(v); })
#    if 0
        .def("add_variable_", [](coek::CompactModel& m, coek::ConcreteIndexedVariable& v)
            {m.add_variable(v);})
#    endif
        .def("add_variable_", [](coek::CompactModel& m, coek::VariableArray& v) { m.add(v); })

        .def("add_objective",
             [](coek::CompactModel& m, double f) {
                 coek::Expression e(f);
                 return m.add_objective(e);
             })
        .def("add_objective",
             [](coek::CompactModel& m, const coek::Expression& e) { return m.add_objective(e); })
        .def("add_objective",
             [](coek::CompactModel& m, const coek::Parameter& e) { return m.add_objective(e); })
        .def("add_objective",
             [](coek::CompactModel& m, const coek::Variable& e) { return m.add_objective(e); })
        .def("add_objective",
             [](coek::CompactModel& m, double f, bool sense) {
                 coek::Expression e(f);
                 return m.add(coek::Objective().expr(e).sense(sense));
             })
        .def("add_objective",
             [](coek::CompactModel& m, const coek::Expression& e, bool sense) {
                 return m.add(coek::Objective().expr(e).sense(sense));
             })
        .def("add_objective",
             [](coek::CompactModel& m, const coek::Parameter& e, bool sense) {
                 return m.add(coek::Objective().expr(e).sense(sense));
             })
        .def("add_objective",
             [](coek::CompactModel& m, const coek::Variable& e, bool sense) {
                 return m.add(coek::Objective().expr(e).sense(sense));
             })

        /*
        .def("add_objective", [](coek::CompactModel& m, double f, const coek::SequenceContext&
        context) {coek::Expression e(f); return m.add_objective(context,e);}) .def("add_objective",
        [](coek::CompactModel& m, const coek::Expression& e, const coek::SequenceContext& context)
            {return m.add_objective(context,e);})
        .def("add_objective", [](coek::CompactModel& m, const coek::Parameter& e, const
        coek::SequenceContext& context) {return m.add_objective(context,e);}) .def("add_objective",
        [](coek::CompactModel& m, const coek::Variable& e, const coek::SequenceContext& context)
            {return m.add_objective(context,e);})
        .def("add_objective", [](coek::CompactModel& m, double f, const coek::SequenceContext&
        context, bool sense) {coek::Expression e(f); return m.add_objective(context, e, sense);})
        .def("add_objective", [](coek::CompactModel& m, const coek::Expression& e, const
        coek::SequenceContext& context, bool sense) {return m.add_objective(context, e, sense);})
        .def("add_objective", [](coek::CompactModel& m, const coek::Parameter& e, const
        coek::SequenceContext& context, bool sense) {return m.add_objective(context, e, sense);})
        .def("add_objective", [](coek::CompactModel& m, const coek::Variable& e, const
        coek::SequenceContext& context, bool sense) {return m.add_objective(context, e, sense);})
        */

        .def("add_constraint",
             [](coek::CompactModel& m, const coek::Constraint& c) { return m.add_constraint(c); })
        /*
        .def("add_constraint", [](coek::CompactModel& m, const coek::Constraint& c, const
        coek::SequenceContext& context) {return m.add_constraint(context,c);})
        */

        //.def("write", [](coek::CompactModel& m, const std::string& s, std::map<int,int>& varmap,
        // std::map<int,int>& conmap){m.write(s,varmap,conmap);})
        .def("write", [](coek::CompactModel& m, const std::string& s) { m.write(s); });
#endif

    //
    // SolverResults
    //
    py::class_<coek::SolverResults, std::shared_ptr<coek::SolverResults>>(m, "SolverResults")
        .def("__str__", [](const coek::SolverResults& r) { return coek::to_string(r); })
        .def_property_readonly("solver_name", [](coek::SolverResults& r) { return r.solver_name; })
        .def_property_readonly("solver_version",
                               [](coek::SolverResults& r) { return r.solver_version; })
        .def_property_readonly("objective_bound",
                               [](coek::SolverResults& r) {
                                   if (r.objective_bound.has_value())
                                       return r.objective_bound.value();
                                   else
                                       return nan("");
                               })
        .def_property_readonly(
            "termination_condition",
            [](coek::SolverResults& r) { return to_string(r.termination_condition); })
        .def_property_readonly("solution_status",
                               [](coek::SolverResults& r) { return to_string(r.solution_status); })
        .def_property_readonly("iteration_count",
                               [](coek::SolverResults& r) { return r.iteration_count; })
        .def_property_readonly("error_message",
                               [](coek::SolverResults& r) { return r.error_message; })
        .def_property_readonly("system_time", [](coek::SolverResults& r) { return r.system_time; })
        .def_property_readonly("objective_value",
                               [](coek::SolverResults& r) { return r.objective_value; });

    m.def("check_optimal_termination", [](const std::shared_ptr<coek::SolverResults>& x) {
        return coek::check_optimal_termination(x);
    });

    //
    // Solver
    //
    py::class_<coek::Solver>(m, "solver")
        .def(py::init<>())
        .def(py::init<std::string&>())
        .def("initialize", &coek::Solver::initialize)
        .def_property_readonly("available", &coek::Solver::available)
        .def("solve", [](coek::Solver& s, coek::Model& m) { return s.solve(m); })
        .def("load", [](coek::Solver& s, coek::Model& m) { return s.load(m); })
#ifdef COEK_WITH_COMPACT_MODEL
        .def("solve", [](coek::Solver& s, coek::CompactModel& m) { return s.solve(m); })
        .def("load", [](coek::Solver& s, coek::CompactModel& m) { return s.load(m); })
#endif
        .def("resolve", &coek::Solver::resolve)

        .def("set_option",
             [](coek::Solver& s, const std::string& o, std::string& v) { s.set_option(o, v); })
        .def("set_option", [](coek::Solver& s, const std::string& o, int v) { s.set_option(o, v); })
        .def("set_option",
             [](coek::Solver& s, const std::string& o, double v) { s.set_option(o, v); })
        .def("get_option",
             [](coek::Solver& s, const std::string& o) {
                 std::string v;
                 s.get_option(o, v);
                 return v;
             })
        .def("get_option",
             [](coek::Solver& s, const std::string& o) {
                 int v;
                 s.get_option(o, v);
                 return v;
             })
        .def("get_option", [](coek::Solver& s, const std::string& o) {
            double v;
            s.get_option(o, v);
            return v;
        });

    //
    // NLPModel
    //
    py::class_<coek::NLPModel>(m, "nlp_model")
        .def(py::init<>())
        .def(py::init<coek::Model&, std::string>())

        .def("num_variables", &coek::NLPModel::num_variables)
        .def("num_objectives", &coek::NLPModel::num_objectives)
        .def("num_constraints", &coek::NLPModel::num_constraints)
        .def("get_variable", &coek::NLPModel::get_variable)
        //.def("set_variable", &coek::NLPModel::set_variable)
        .def("get_constraint", &coek::NLPModel::get_constraint)

        .def("compute_f", [](coek::NLPModel& m) { return m.compute_f(0); })
        .def("compute_f", [](coek::NLPModel& m, unsigned int i) { return m.compute_f(i); })
        .def("compute_c",
             [](coek::NLPModel& m) {
                 std::vector<double> c(m.num_constraints());
                 m.compute_c(c);
                 return c;
             })
        .def("compute_df",
             [](coek::NLPModel& m) {
                 std::vector<double> df(m.num_variables());
                 m.compute_df(df, 0);
                 return df;
             })
        .def("compute_df",
             [](coek::NLPModel& m, unsigned int i) {
                 std::vector<double> df(m.num_variables());
                 m.compute_df(df, i);
                 return df;
             })
        .def("compute_dc",
             [](coek::NLPModel& m, unsigned int i) {
                 std::vector<double> dc(m.num_variables());
                 m.compute_dc(dc, i);
                 return dc;
             })
        //.def("compute_H", [](coek::NLPModel& m){return m.compute_f();})
        //.def("compute_J", [](coek::NLPModel& m){return m.compute_f();})
        .def("write", [](coek::NLPModel& m, const std::string& s) { m.write(s); })
        //.def("write", [](coek::NLPModel& m, const std::string& s, std::map<int,int>& varmap,
        // std::map<int,int>& conmap){m.write(s,varmap,conmap);})
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
        .def_property_readonly("available", &coek::NLPSolver::available)

        .def("set_option",
             [](coek::NLPSolver& s, const std::string& o, std::string& v) { s.set_option(o, v); })
        .def("set_option",
             [](coek::NLPSolver& s, const std::string& o, int v) { s.set_option(o, v); })
        .def("set_option",
             [](coek::NLPSolver& s, const std::string& o, double v) { s.set_option(o, v); })
        .def("get_option",
             [](coek::NLPSolver& s, const std::string& o) {
                 std::string v;
                 s.get_option(o, v);
                 return v;
             })
        .def("get_option",
             [](coek::NLPSolver& s, const std::string& o) {
                 int v;
                 s.get_option(o, v);
                 return v;
             })
        .def("get_option", [](coek::NLPSolver& s, const std::string& o) {
            double v;
            s.get_option(o, v);
            return v;
        });

    // (#, Expression, #)
    m.def("inequality", [](int lower, const coek::Expression& body, int upper) {
        return inequality(lower, body, upper);
    });
    m.def("inequality", [](double lower, const coek::Expression& body, double upper) {
        return inequality(lower, body, upper);
    });
    m.def("inequality",
          [](const coek::Expression lower, const coek::Expression& body,
             const coek::Expression upper) { return inequality(lower, body, upper); });

    // (#, Expression, #, strict)
    m.def("inequality", [](int lower, const coek::Expression& body, int upper, bool strict) {
        return inequality(lower, body, upper, strict);
    });
    m.def("inequality", [](double lower, const coek::Expression& body, double upper, bool strict) {
        return inequality(lower, body, upper, strict);
    });
    m.def("inequality", [](const coek::Expression lower, const coek::Expression& body,
                           const coek::Expression upper,
                           bool strict) { return inequality(lower, body, upper, strict); });

    // (#, Variable, #)
    m.def("inequality", [](int lower, const coek::Variable& body, int upper) {
        return inequality(lower, body, upper);
    });
    m.def("inequality", [](double lower, const coek::Variable& body, double upper) {
        return inequality(lower, body, upper);
    });
    m.def("inequality",
          [](const coek::Expression lower, const coek::Variable& body,
             const coek::Expression upper) { return inequality(lower, body, upper); });
    m.def("inequality", [](int lower, const coek::Variable& body, int upper, bool strict) {
        return inequality(lower, body, upper, strict);
    });

    // (#, Variable, #, strict)
    m.def("inequality", [](double lower, const coek::Variable& body, double upper, bool strict) {
        return inequality(lower, body, upper, strict);
    });
    m.def("inequality",
          [](const coek::Expression lower, const coek::Variable& body, const coek::Expression upper,
             bool strict) { return inequality(lower, body, upper, strict); });
}
