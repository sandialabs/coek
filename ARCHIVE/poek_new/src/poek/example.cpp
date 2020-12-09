#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include "coek_model.hpp"

namespace py = pybind11;


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


class VariableArray
{
public:

    std::vector<coek::Variable> variables;

public:

    VariableArray(int n, py::kwargs kwargs)
        {
        double lb, ub, init;
        bool binary, integer, fixed;
        parse_varargs<double>(kwargs, "lb", lb, -COEK_INFINITY);
        parse_varargs<double>(kwargs, "ub", ub,  COEK_INFINITY);
        parse_varargs<double>(kwargs, "initialize", init, lb);
        parse_varargs<bool>(kwargs, "binary", binary, false);
        parse_varargs<bool>(kwargs, "integer", integer, false);
        parse_varargs<bool>(kwargs, "fixed", fixed, false);

        variables.resize(n);
        for (int i=0; i<n; i++)
            variables[i].initialize(lb, ub, init, binary, integer, fixed);
        }

    coek::Variable& get(int i)
        { return variables[i]; }
};


VariableArray* variable_fn(int n, py::kwargs kwargs)
{ return new VariableArray(n, kwargs); }

coek::Variable variable_fn(py::kwargs kwargs)
{
double lb, ub, init;
bool binary, integer, fixed;
parse_varargs<double>(kwargs, "lb", lb, -COEK_INFINITY);
parse_varargs<double>(kwargs, "ub", ub,  COEK_INFINITY);
parse_varargs<double>(kwargs, "initialize", init, lb);
parse_varargs<bool>(kwargs, "binary", binary, false);
parse_varargs<bool>(kwargs, "integer", integer, false);
parse_varargs<bool>(kwargs, "fixed", fixed, false);

coek::Variable tmp;
try {
    auto _name = kwargs["name"];
    if (not _name.is_none()) {
        auto name = _name.cast<py::str>();
        tmp.initialize(lb, ub, init, binary, integer, fixed, name);
        }
    else
        tmp.initialize(lb, ub, init, binary, integer, fixed);
    }
catch (std::exception& err) {
    }
return tmp;
}


PYBIND11_MODULE(poek, m) {
    m.doc() = "poek module"; // optional module docstring

    m.def("variable",[](int n, py::kwargs kw) {return variable_fn(n,kw);});
    m.def("variable",[](py::kwargs kw) {return variable_fn(kw);});

    //
    // Parameter
    //
    py::class_<coek::Parameter>(m, "parameter")
        .def(py::init<double>())
        .def(py::init<double, const std::string&>())
        .def("get_name",&coek::Parameter::get_name)
        .def_property("value", &coek::Parameter::get_value, &coek::Parameter::set_value)

        .def("__neg__", [](const coek::Parameter& x){return +x;})
        .def("__pos__", [](const coek::Parameter& x){return -x;})

        .def(py::self + int())
        .def(py::self + double())
        .def("__add__", [](const coek::Parameter& x, const coek::Parameter& y){return x+y;})
        .def("__add__", [](const coek::Parameter& x, const coek::Variable& y){return x+y;})
        .def("__add__", [](const coek::Parameter& x, const coek::Expression& y){return x+y;})
        .def("__radd__", [](const coek::Parameter& x, int y){return y+x;})
        .def("__radd__", [](const coek::Parameter& x, double y){return y+x;})

        .def(py::self - int())
        .def(py::self - double())
        .def("__sub__", [](const coek::Parameter& x, const coek::Parameter& y){return x-y;})
        .def("__sub__", [](const coek::Parameter& x, const coek::Variable& y){return x-y;})
        .def("__sub__", [](const coek::Parameter& x, const coek::Expression& y){return x-y;})
        .def("__rsub__", [](const coek::Parameter& x, int y){return y-x;})
        .def("__rsub__", [](const coek::Parameter& x, double y){return y-x;})

        .def(py::self * int())
        .def(py::self * double())
        .def("__mul__", [](const coek::Parameter& x, const coek::Parameter& y){return x*y;})
        .def("__mul__", [](const coek::Parameter& x, const coek::Variable& y){return x*y;})
        .def("__mul__", [](const coek::Parameter& x, const coek::Expression& y){return x*y;})
        .def("__rmul__", [](const coek::Parameter& x, int y){return y*x;})
        .def("__rmul__", [](const coek::Parameter& x, double y){return y*x;})

        .def(py::self / int())
        .def(py::self / double())
        .def("__div__", [](const coek::Parameter& x, const coek::Parameter& y){return x/y;})
        .def("__div__", [](const coek::Parameter& x, const coek::Variable& y){return x/y;})
        .def("__div__", [](const coek::Parameter& x, const coek::Expression& y){return x/y;})
        .def("__rdiv__", [](const coek::Parameter& x, int y){return y/x;})
        .def("__rdiv__", [](const coek::Parameter& x, double y){return y/x;})

        .def("__pow__", [](const coek::Parameter& x, int y){return pow(x,y);})
        .def("__pow__", [](const coek::Parameter& x, double y){return pow(x,y);})
        .def("__pow__", [](const coek::Parameter& x, const coek::Parameter& y){return pow(x,y);})
        .def("__pow__", [](const coek::Parameter& x, const coek::Variable& y){return pow(x,y);})
        .def("__pow__", [](const coek::Parameter& x, const coek::Expression& y){return pow(x,y);})
        .def("__rpow__", [](const coek::Parameter& x, int y){return pow(y,x);})
        .def("__rpow__", [](const coek::Parameter& x, double y){return pow(y,x);})

        .def(py::self < int())
        .def(py::self < double())
        .def("__lt__", [](const coek::Parameter& x, const coek::Parameter& y){return x<y;})
        .def("__lt__", [](const coek::Parameter& x, const coek::Variable& y){return x<y;})
        .def("__lt__", [](const coek::Parameter& x, const coek::Expression& y){return x<y;})

        .def(py::self <= int())
        .def(py::self <= double())
        .def("__le__", [](const coek::Parameter& x, const coek::Parameter& y){return x<=y;})
        .def("__le__", [](const coek::Parameter& x, const coek::Variable& y){return x<=y;})
        .def("__le__", [](const coek::Parameter& x, const coek::Expression& y){return x<=y;})

        .def(py::self > int())
        .def(py::self > double())
        .def("__gt__", [](const coek::Parameter& x, const coek::Parameter& y){return x>y;})
        .def("__gt__", [](const coek::Parameter& x, const coek::Variable& y){return x>y;})
        .def("__gt__", [](const coek::Parameter& x, const coek::Expression& y){return x>y;})

        .def(py::self >= int())
        .def(py::self >= double())
        .def("__ge__", [](const coek::Parameter& x, const coek::Parameter& y){return x>=y;})
        .def("__ge__", [](const coek::Parameter& x, const coek::Variable& y){return x>=y;})
        .def("__ge__", [](const coek::Parameter& x, const coek::Expression& y){return x>=y;})

        .def(py::self == int())
        .def(py::self == double())
        .def("__eq__", [](const coek::Parameter& x, const coek::Parameter& y){return x==y;})
        .def("__eq__", [](const coek::Parameter& x, const coek::Variable& y){return x==y;})
        .def("__eq__", [](const coek::Parameter& x, const coek::Expression& y){return x==y;})

        .def("__abs__", [](coek::Parameter& x){return abs(x);})
        ;

    //
    // Variable
    //
    py::class_<coek::Variable>(m, "variable_single")
        .def(py::init<double>())
        .def(py::init<double, double, double, const std::string&>())
        .def(py::init<double, double, double, bool, bool>())
        .def("get_name",&coek::Variable::get_name)
        .def_property("value", &coek::Variable::get_value, &coek::Variable::set_value)
        .def_property("initial", &coek::Variable::get_initial, &coek::Variable::set_initial)
        .def_property("lb", &coek::Variable::get_lb, &coek::Variable::set_lb)
        .def_property("ub", &coek::Variable::get_ub, &coek::Variable::set_ub)
        .def_property("fixed", &coek::Variable::get_fixed, &coek::Variable::set_fixed)

        .def("__neg__", [](const coek::Variable& x){return +x;})
        .def("__pos__", [](const coek::Variable& x){return -x;})

        .def(py::self + int())
        .def(py::self + double())
        .def("__add__", [](const coek::Variable& x, const coek::Parameter& y){return x+y;})
        .def("__add__", [](const coek::Variable& x, const coek::Variable& y){return x+y;})
        .def("__add__", [](const coek::Variable& x, const coek::Expression& y){return x+y;})
        .def("__radd__", [](const coek::Variable& x, int y){return y+x;})
        .def("__radd__", [](const coek::Variable& x, double y){return y+x;})

        .def(py::self - int())
        .def(py::self - double())
        .def("__sub__", [](const coek::Variable& x, const coek::Parameter& y){return x-y;})
        .def("__sub__", [](const coek::Variable& x, const coek::Variable& y){return x-y;})
        .def("__sub__", [](const coek::Variable& x, const coek::Expression& y){return x-y;})
        .def("__rsub__", [](const coek::Variable& x, int y){return y-x;})
        .def("__rsub__", [](const coek::Variable& x, double y){return y-x;})

        .def(py::self * int())
        .def(py::self * double())
        .def("__mul__", [](const coek::Variable& x, const coek::Parameter& y){return x*y;})
        .def("__mul__", [](const coek::Variable& x, const coek::Variable& y){return x*y;})
        .def("__mul__", [](const coek::Variable& x, const coek::Expression& y){return x*y;})
        .def("__rmul__", [](const coek::Variable& x, int y){return y*x;})
        .def("__rmul__", [](const coek::Variable& x, double y){return y*x;})

        .def(py::self / int())
        .def(py::self / double())
        .def("__div__", [](const coek::Variable& x, const coek::Parameter& y){return x/y;})
        .def("__div__", [](const coek::Variable& x, const coek::Variable& y){return x/y;})
        .def("__div__", [](const coek::Variable& x, const coek::Expression& y){return x/y;})
        .def("__rdiv__", [](const coek::Variable& x, int y){return y/x;})
        .def("__rdiv__", [](const coek::Variable& x, double y){return y/x;})

        .def("__pow__", [](const coek::Variable& x, int y){return pow(x,y);})
        .def("__pow__", [](const coek::Variable& x, double y){return pow(x,y);})
        .def("__pow__", [](const coek::Variable& x, const coek::Parameter& y){return pow(x,y);})
        .def("__pow__", [](const coek::Variable& x, const coek::Variable& y){return pow(x,y);})
        .def("__pow__", [](const coek::Variable& x, const coek::Expression& y){return pow(x,y);})
        .def("__rpow__", [](const coek::Variable& x, int y){return pow(y,x);})
        .def("__rpow__", [](const coek::Variable& x, double y){return pow(y,x);})

        .def(py::self < int())
        .def(py::self < double())
        .def("__lt__", [](const coek::Variable& x, const coek::Parameter& y){return x<y;})
        .def("__lt__", [](const coek::Variable& x, const coek::Variable& y){return x<y;})
        .def("__lt__", [](const coek::Variable& x, const coek::Expression& y){return x<y;})

        .def(py::self <= int())
        .def(py::self <= double())
        .def("__le__", [](const coek::Variable& x, const coek::Parameter& y){return x<=y;})
        .def("__le__", [](const coek::Variable& x, const coek::Variable& y){return x<=y;})
        .def("__le__", [](const coek::Variable& x, const coek::Expression& y){return x<=y;})

        .def(py::self > int())
        .def(py::self > double())
        .def("__gt__", [](const coek::Variable& x, const coek::Parameter& y){return x>y;})
        .def("__gt__", [](const coek::Variable& x, const coek::Variable& y){return x>y;})
        .def("__gt__", [](const coek::Variable& x, const coek::Expression& y){return x>y;})

        .def(py::self >= int())
        .def(py::self >= double())
        .def("__ge__", [](const coek::Variable& x, const coek::Parameter& y){return x>=y;})
        .def("__ge__", [](const coek::Variable& x, const coek::Variable& y){return x>=y;})
        .def("__ge__", [](const coek::Variable& x, const coek::Expression& y){return x>=y;})

        .def(py::self == int())
        .def(py::self == double())
        .def("__eq__", [](const coek::Variable& x, const coek::Parameter& y){return x==y;})
        .def("__eq__", [](const coek::Variable& x, const coek::Variable& y){return x==y;})
        .def("__eq__", [](const coek::Variable& x, const coek::Expression& y){return x==y;})

        .def("__abs__", [](coek::Variable& x){return abs(x);})
        ;

    //
    // VariableArray
    //
    py::class_<VariableArray>(m, "variable_array")
        .def(py::init<int, py::kwargs>())
        .def("__getitem__", [](VariableArray& va, int i){return va.get(i);})
        ;

    //
    // Expression
    //
    py::class_<coek::Expression>(m, "expression")
        .def(py::init<double>())
        .def(py::init<int>())
        .def(py::init<coek::Parameter&>())
        .def(py::init<coek::Variable&>())
        .def_property_readonly("value", &coek::Expression::get_value)

        .def("__neg__", [](const coek::Expression& x){return +x;})
        .def("__pos__", [](const coek::Expression& x){return -x;})

        .def(py::self + int())
        .def(py::self + double())
        .def("__add__", [](const coek::Expression& x, const coek::Parameter& y){return x+y;})
        .def("__add__", [](const coek::Expression& x, const coek::Variable& y){return x+y;})
        .def("__add__", [](const coek::Expression& x, const coek::Expression& y){return x+y;})
        .def("__radd__", [](const coek::Expression& x, int y){return y+x;})
        .def("__radd__", [](const coek::Expression& x, double y){return y+x;})

        .def(py::self - int())
        .def(py::self - double())
        .def("__sub__", [](const coek::Expression& x, const coek::Parameter& y){return x-y;})
        .def("__sub__", [](const coek::Expression& x, const coek::Variable& y){return x-y;})
        .def("__sub__", [](const coek::Expression& x, const coek::Expression& y){return x-y;})
        .def("__rsub__", [](const coek::Expression& x, int y){return y-x;})
        .def("__rsub__", [](const coek::Expression& x, double y){return y-x;})

        .def(py::self * int())
        .def(py::self * double())
        .def("__mul__", [](const coek::Expression& x, const coek::Parameter& y){return x*y;})
        .def("__mul__", [](const coek::Expression& x, const coek::Variable& y){return x*y;})
        .def("__mul__", [](const coek::Expression& x, const coek::Expression& y){return x*y;})
        .def("__rmul__", [](const coek::Expression& x, int y){return y*x;})
        .def("__rmul__", [](const coek::Expression& x, double y){return y*x;})

        .def(py::self / int())
        .def(py::self / double())
        .def("__div__", [](const coek::Expression& x, const coek::Parameter& y){return x/y;})
        .def("__div__", [](const coek::Expression& x, const coek::Variable& y){return x/y;})
        .def("__div__", [](const coek::Expression& x, const coek::Expression& y){return x/y;})
        .def("__rdiv__", [](const coek::Expression& x, int y){return y/x;})
        .def("__rdiv__", [](const coek::Expression& x, double y){return y/x;})

        .def("__pow__", [](const coek::Expression& x, int y){return pow(x,y);})
        .def("__pow__", [](const coek::Expression& x, double y){return pow(x,y);})
        .def("__pow__", [](const coek::Expression& x, const coek::Parameter& y){return pow(x,y);})
        .def("__pow__", [](const coek::Expression& x, const coek::Variable& y){return pow(x,y);})
        .def("__pow__", [](const coek::Expression& x, const coek::Expression& y){return pow(x,y);})
        .def("__rpow__", [](const coek::Expression& x, int y){return pow(y,x);})
        .def("__rpow__", [](const coek::Expression& x, double y){return pow(y,x);})

        .def(py::self < int())
        .def(py::self < double())
        .def("__lt__", [](const coek::Expression& x, const coek::Parameter& y){return x<y;})
        .def("__lt__", [](const coek::Expression& x, const coek::Variable& y){return x<y;})
        .def("__lt__", [](const coek::Expression& x, const coek::Expression& y){return x<y;})

        .def(py::self <= int())
        .def(py::self <= double())
        .def("__le__", [](const coek::Expression& x, const coek::Parameter& y){return x<=y;})
        .def("__le__", [](const coek::Expression& x, const coek::Variable& y){return x<=y;})
        .def("__le__", [](const coek::Expression& x, const coek::Expression& y){return x<=y;})

        .def(py::self > int())
        .def(py::self > double())
        .def("__gt__", [](const coek::Expression& x, const coek::Parameter& y){return x>y;})
        .def("__gt__", [](const coek::Expression& x, const coek::Variable& y){return x>y;})
        .def("__gt__", [](const coek::Expression& x, const coek::Expression& y){return x>y;})

        .def(py::self >= int())
        .def(py::self >= double())
        .def("__ge__", [](const coek::Expression& x, const coek::Parameter& y){return x>=y;})
        .def("__ge__", [](const coek::Expression& x, const coek::Variable& y){return x>=y;})
        .def("__ge__", [](const coek::Expression& x, const coek::Expression& y){return x>=y;})

        .def(py::self == int())
        .def(py::self == double())
        .def("__eq__", [](const coek::Expression& x, const coek::Parameter& y){return x==y;})
        .def("__eq__", [](const coek::Expression& x, const coek::Variable& y){return x==y;})
        .def("__eq__", [](const coek::Expression& x, const coek::Expression& y){return x==y;})

        .def("__iadd__", [](coek::Expression& x, const coek::Expression& y){return x += y;})
        .def("__isub__", [](coek::Expression& x, const coek::Expression& y){return x -= y;})
        .def("__imul__", [](coek::Expression& x, const coek::Expression& y){return x *= y;})
        .def("__idiv__", [](coek::Expression& x, const coek::Expression& y){return x /= y;})

        .def("__abs__", [](coek::Expression& x){return coek::abs(x);})
        ;

    //
    // Constraint
    //
    py::class_<coek::Constraint>(m, "constraint")
        .def("__init__", [](){throw std::runtime_error("Cannot create an empty constraint.");})
        .def_property_readonly("value", &coek::Constraint::get_value)
        .def_property_readonly("get_feasible", &coek::Constraint::feasible)
        ;

    //
    // Intrinsics
    //
    m.def("ceil", &coek::ceil);
    m.def("floor", &coek::floor);
    m.def("exp", &coek::exp);
    m.def("log", &coek::log);
    m.def("log10", &coek::log10);
    m.def("sqrt", &coek::sqrt);
    m.def("sin", &coek::sin);
    m.def("cos", &coek::cos);
    m.def("tan", &coek::tan);
    m.def("sinh", &coek::sinh);
    m.def("cosh", &coek::cosh);
    m.def("tanh", &coek::tanh);
    m.def("asin", &coek::asin);
    m.def("acos", &coek::acos);
    m.def("atan", &coek::atan);
    m.def("asinh", &coek::asinh);
    m.def("acosh", &coek::acosh);
    m.def("atanh", &coek::atanh);

    //
    // Model
    //
    py::class_<coek::Model>(m, "model")
        .def(py::init<>())
        .def("add", [](coek::Model& m, const coek::Expression& e){m.add(e);})
        .def("add", [](coek::Model& m, const coek::Expression& e, bool sense){m.add(e, sense);})
        .def("add", [](coek::Model& m, const coek::Constraint& c){m.add(c);})
        .def("write", [](coek::Model& m, const std::string& s){m.write(s);})
        .def("use", [](coek::Model& m, coek::Variable& v){m.addVariable(v);})
        ;

    //
    // Solver
    //
    py::class_<coek::Solver>(m, "solver")
        .def(py::init<>())
        .def(py::init<std::string&>())
        .def("initialize", &coek::Solver::initialize)
        .def("solve", &coek::Solver::solve)
        .def("load", &coek::Solver::load)
        .def("resolve", &coek::Solver::resolve)

        .def("set_option", [](coek::Solver& s, const std::string& o, int v){s.set_option(o,v);})
        .def("set_option", [](coek::Solver& s, const std::string& o, double v){s.set_option(o,v);})
        .def("set_option", [](coek::Solver& s, const std::string& o, std::string& v){s.set_option(o,v);})
        .def("get_option", [](coek::Solver& s, const std::string& o){int v; s.get_option(o,v); return v;})
        .def("get_option", [](coek::Solver& s, const std::string& o){double v; s.get_option(o,v); return v;})
        .def("get_option", [](coek::Solver& s, const std::string& o){std::string v; s.get_option(o,v); return v;})
        ;

    //
    // NLPModel
    //
    py::class_<coek::NLPModel>(m, "nlp_model")
        .def(py::init<>())
        .def(py::init<coek::Model&,std::string,bool>())

        .def("num_variables", &coek::NLPModel::num_variables)
        .def("num_objectives", &coek::NLPModel::num_objectives)
        .def("num_constraints", &coek::NLPModel::num_constraints)
        .def("get_variable", &coek::NLPModel::get_variable)
        .def("set_variable", &coek::NLPModel::set_variable)

        .def("compute_f", [](coek::NLPModel& m, unsigned int i){return m.compute_f(i);})
        // TODO - NumPy API
        //.def("compute_df", [](coek::NLPModel& m){return m.compute_f();})
        //.def("compute_H", [](coek::NLPModel& m){return m.compute_f();})
        //.def("compute_c", [](coek::NLPModel& m){return m.compute_f();})
        //.def("compute_J", [](coek::NLPModel& m){return m.compute_f();})
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

        .def("set_option", [](coek::NLPSolver& s, const std::string& o, int v){s.set_option(o,v);})
        .def("set_option", [](coek::NLPSolver& s, const std::string& o, double v){s.set_option(o,v);})
        .def("set_option", [](coek::NLPSolver& s, const std::string& o, std::string& v){s.set_option(o,v);})
        .def("get_option", [](coek::NLPSolver& s, const std::string& o){int v; s.get_option(o,v); return v;})
        .def("get_option", [](coek::NLPSolver& s, const std::string& o){double v; s.get_option(o,v); return v;})
        .def("get_option", [](coek::NLPSolver& s, const std::string& o){std::string v; s.get_option(o,v); return v;})
        ;

}

