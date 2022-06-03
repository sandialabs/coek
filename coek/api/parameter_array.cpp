#include "coek/api/parameter_assoc_array_repn.hpp"
#include "coek/api/parameter_array.hpp"
#include "coek/model/model.hpp"

namespace coek {

class ParameterArrayRepn : public ParameterAssocArrayRepn
{
public:

    std::vector<size_t> shape;
    size_t _size;

public:
  
    ParameterArrayRepn(size_t n)
        : shape({n}), _size(n)
        { cache.resize((size()+1)*2); }
    
    ParameterArrayRepn(const std::vector<size_t>& _shape)
        : shape(_shape), _size(1)
        {
        for (auto n : shape)
            _size *= n;
        cache.resize((size()+1)*(dim()+1));
        }

    ParameterArrayRepn(const std::initializer_list<size_t>& _shape)
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

void ParameterArrayRepn::setup()
{
ParameterAssocArrayRepn::setup();

auto name = parameter_template.name();
for (size_t i=0; i<size(); i++) {
    std::string _name = name+"("+std::to_string(i)+")";
    names.push_back(_name);
    }
}

//
// ParameterArray
//

ParameterArray::ParameterArray(size_t n)
{
repn = std::make_shared<ParameterArrayRepn>(n);
repn->resize_index_vectors(tmp,reftmp);
}

ParameterArray::ParameterArray(const std::vector<size_t>& shape)
{
repn = std::make_shared<ParameterArrayRepn>(shape);
repn->resize_index_vectors(tmp,reftmp);
}

ParameterArray::ParameterArray(const std::initializer_list<size_t>& shape)
{
repn = std::make_shared<ParameterArrayRepn>(shape);
repn->resize_index_vectors(tmp,reftmp);
}

ParameterAssocArrayRepn* ParameterArray::get_repn()
{ return repn.get(); }

Parameter ParameterArray::index(const IndexVector& args)
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
    std::string err = "Unknown index value: "+_repn->parameter_template.name()+"(";
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

void ParameterArray::index_error(size_t i)
{
auto _repn = repn.get();
std::string err = "Unexpected index value: "+_repn->parameter_template.name()+
        " is an "+std::to_string(tmp.size())+
        "-D parameter array but is being indexed with "+std::to_string(i)+" indices.";
throw std::runtime_error(err);
}

ParameterArray& ParameterArray::value(double value)
{
repn->value(value);
return *this;
}

ParameterArray& ParameterArray::value(const Expression& value)
{
repn->value(value);
return *this;
}

ParameterArray& ParameterArray::name(const std::string& name)
{
repn->name(name);
return *this;
}

//
// OTHER
//

ParameterArray parameter(size_t n)
{ return ParameterArray(n); }

ParameterArray parameter(const std::vector<size_t>& shape)
{ return ParameterArray(shape); }

ParameterArray parameter(const std::initializer_list<size_t>& shape)
{ return ParameterArray(shape); }

}
