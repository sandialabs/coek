#include "coek/compact/variable_assoc_array_repn.hpp"
#include "coek/compact/variable_array.hpp"
#include "coek/model/model.hpp"

namespace coek {

class VariableArrayRepn : public VariableAssocArrayRepn
{
public:

    std::vector<size_t> shape;
    size_t _size;

public:
  
    VariableArrayRepn(size_t n)
        : shape({n}), _size(n)
        { cache.resize((size()+1)*2); }
    
    VariableArrayRepn(const std::vector<size_t>& _shape)
        : shape(_shape), _size(1)
        {
        for (auto n : shape)
            _size *= n;
        cache.resize((size()+1)*(dim()+1));
        }

    VariableArrayRepn(const std::initializer_list<size_t>& _shape)
        : shape(_shape), _size(1)
        {
        for (auto n : shape)
            _size *= n;
        cache.resize((size()+1)*(dim()+1));
        }

    void setup();

    size_t dim()
        {return shape.size();}

    size_t size()
        {return _size;}

};

void VariableArrayRepn::setup()
{
VariableAssocArrayRepn::setup();

auto name = variable_template.name();
for (size_t i=0; i<size(); i++) {
    std::string _name = name+std::to_string(i);
    names.push_back(_name + ")");
    }
}

//
// VariableArray
//

VariableArray::VariableArray(size_t n)
{
repn = std::make_shared<VariableArrayRepn>(n);
repn->resize_index_vectors(tmp,reftmp);
}

VariableArray::VariableArray(const std::vector<size_t>& shape)
{
repn = std::make_shared<VariableArrayRepn>(shape);
repn->resize_index_vectors(tmp,reftmp);
}

VariableArray::VariableArray(const std::initializer_list<size_t>& shape)
{
repn = std::make_shared<VariableArrayRepn>(shape);
repn->resize_index_vectors(tmp,reftmp);
}

VariableAssocArrayRepn* VariableArray::get_repn()
{ return repn.get(); }

Variable VariableArray::index(const IndexVector& args)
{
auto _repn = repn.get();
auto& shape = _repn->shape;
assert (args.size() == shape.size());

if (_repn->call_setup)
    _repn->setup();

// We know that the args[i] values are nonnegative b.c. we have asserted that while
// processing these arguments
size_t ndx = static_cast<size_t>(args[0]);
for (size_t i=1; i<args.size(); i++) {
    ndx = ndx*shape[i-1] + args[i];
    }

if (ndx > size()) {
    std::string err = "Unknown index value: "+_repn->variable_template.name()+"(";
    for (size_t i=0; i<args.size(); i++) {
        if (i > 0)
            err += ",";
        err += std::to_string(args[i]);
        }
    err += ")";
    throw std::runtime_error(err);
    }

return _repn->values[ndx];
}

void VariableArray::index_error(size_t i)
{
auto _repn = repn.get();
std::string err = "Unexpected index value: "+_repn->variable_template.name()+
        " is an "+std::to_string(tmp.size())+
        "-D variable array but is being indexed with "+std::to_string(i)+" indices.";
throw std::runtime_error(err);
}

VariableArray& VariableArray::value(double value)
{
repn->value(value);
return *this;
}

VariableArray& VariableArray::value(const Expression& value)
{
repn->value(value);
return *this;
}

VariableArray& VariableArray::lower(double value)
{
repn->lower(value);
return *this;
}

VariableArray& VariableArray::lower(const Expression& value)
{
repn->lower(value);
return *this;
}

VariableArray& VariableArray::upper(double value)
{
repn->upper(value);
return *this;
}

VariableArray& VariableArray::upper(const Expression& value)
{
repn->upper(value);
return *this;
}

VariableArray& VariableArray::bounds(double lb, double ub)
{
repn->bounds(lb,ub);
return *this;
}

VariableArray& VariableArray::bounds(const Expression& lb, double ub)
{
repn->bounds(lb,ub);
return *this;
}

VariableArray& VariableArray::bounds(double lb, const Expression& ub)
{
repn->bounds(lb,ub);
return *this;
}

VariableArray& VariableArray::bounds(const Expression& lb, const Expression& ub)
{
repn->bounds(lb,ub);
return *this;
}

VariableArray& VariableArray::name(const std::string& name)
{
repn->name(name);
return *this;
}

VariableArray& VariableArray::within(VariableTypes vtype)
{
repn->within(vtype);
return *this;
}

//
// OTHER
//

VariableArray variable(size_t n)
{ return VariableArray(n); }

VariableArray variable(const std::vector<size_t>& shape)
{ return VariableArray(shape); }

VariableArray variable(const std::initializer_list<size_t>& shape)
{ return VariableArray(shape); }

VariableArray& Model::add_variable(VariableArray& vars)
{
if (vars.repn->call_setup)
    vars.repn->setup();
for (auto& var: vars.repn->values)
    add_variable(var);
return vars;
}

}
