#include "abstract/expr_rule.hpp"
#include "coek.hpp"
#include "expr/varray.hpp"

using namespace std;

int main(int argc, char** argv)
{
    coek::Model model;

    auto o = coek::Variable(0, 1, 0, "o");
    model.addVariable(o);

    model.add(o);

    auto x = std::make_shared<coek::VariableArray>(10, 0.0, 1.0, 0.0, false, false, false, "x");
    auto y = std::make_shared<coek::VariableArray>(10, 0.0, 1.0, 0.0, false, false, false, "y");
    auto z = std::make_shared<coek::VariableArray>(10, 0.0, 1.0, 0.0, false, false, false, "z");
    model.addVariable(*x);
    model.addVariable(*y);
    model.addVariable(*z);

    auto i = std::make_shared<coek::rule::IndexVariable>();
    auto xi = std::make_shared<coek::rule::VariableRef>(x, i);
    auto yi = std::make_shared<coek::rule::VariableRef>(y, i);
    auto zi = std::make_shared<coek::rule::VariableRef>(z, i);

    auto e1 = std::make_shared<coek::rule::Plus>(xi, yi);
    auto e2 = std::make_shared<coek::rule::Plus>(e1, zi);
    auto c = std::make_shared<coek::rule::Equals>(e2);

    auto rset = std::make_shared<coek::rule::RangeSet>(10, i);

    model.add(*rset, *c);
    model.write("template.lp");

    return 0;
}
