#include <unordered_map>
#include "coek/ast/compact_terms.hpp"
#include "coek/api/variable_assoc_array_repn.hpp"
#include "coek/compact/variable_map.hpp"
#include "coek/model/model.hpp"


namespace coek {

//
// VariableMapRepn
//

class VariableMapRepn : public VariableAssocArrayRepn
{
public:

    std::unordered_map<IndexVector, size_t> index;
    ConcreteSet concrete_set;
    bool names_generated=false;

public:

    VariableMapRepn(const ConcreteSet& _arg)
        : concrete_set(_arg)
        { cache.resize((size()+1)*(dim()+1)); }

    VariableMapRepn(const SequenceContext& _arg)
        : concrete_set(_arg.index_set())
        { cache.resize((size()+1)*(dim()+1)); }

    void setup(); 

    // TODO - evaluate whether it is reasonable to use const_cast here
    size_t dim() const
        {return const_cast<ConcreteSet&>(concrete_set).dim();}

    size_t size() const
        {return const_cast<ConcreteSet&>(concrete_set).size();}

    std::string get_name(size_t index);
};

std::string VariableMapRepn::get_name(size_t idx)
{
if (call_setup)
    setup();

if (not names_generated) {
    names_generated = true;

    auto name = variable_template.name();
    size_t _dim = dim();
    int x_data [_dim];
    IndexVector x(x_data, _dim);
    for (auto& indices : concrete_set) {
        for (size_t j=0; j<_dim; j++)
            x[j] = indices[j];
        if (indices.size() == 1) {
            auto tmp = indices[0];
            values[index[x]].name(name + "[" + std::to_string(tmp) + "]");
            }
        else {
              std::string _name = name + "[";
              auto tmp = indices[0];
              _name += std::to_string(tmp);
              for (size_t j=1; j<indices.size(); j++) {
                  auto tmp = indices[j];
                  _name += "," + std::to_string(tmp);
                  }
              values[index[x]].name(_name + "]");
              }
          }
    }

return values[idx].name();
}

void VariableMapRepn::setup()
{
VariableAssocArrayRepn::setup();

size_t _dim = dim();
size_t i=0;
for (auto& vec : concrete_set) {
    auto x = cache.alloc(_dim);
    for (size_t j=0; j<_dim; j++)
        x[j] = vec[j];
    index[x] = i++;
    }
}

//
// VariableMap
//

VariableMap::VariableMap(const ConcreteSet& arg)
{
repn = std::make_shared<VariableMapRepn>(arg);
repn->resize_index_vectors(tmp,reftmp);
}

VariableMap::VariableMap(const SequenceContext& arg)
{
repn = std::make_shared<VariableMapRepn>(arg);
repn->resize_index_vectors(tmp,reftmp);
}

VariableAssocArrayRepn* VariableMap::get_repn()
{ return repn.get(); }

const VariableAssocArrayRepn* VariableMap::get_repn() const
{ return repn.get(); }

Variable VariableMap::index(const IndexVector& args)
{
assert(dim() == args.size());

auto _repn = repn.get();
if (_repn->call_setup)
    _repn->setup();

auto curr = _repn->index.find(tmp);
if (curr == _repn->index.end()) {
    std::string err = "Unknown index value: "+_repn->variable_template.name()+"[";
    for (size_t i=0; i<args.size(); i++) {
        if (i > 0)
            err += ",";
        err += std::to_string(args[i]);
        }
    err += "]";
    throw std::runtime_error(err);
    }
return _repn->values[curr->second];
}

void VariableMap::index_error(size_t i)
{
auto _repn = repn.get();
std::string err = "Unexpected index value: "+_repn->variable_template.name()+
    " is an "+std::to_string(tmp.size())+
    "-D variable map but is being indexed with "+std::to_string(i)+" indices.";
throw std::runtime_error(err);
}
  
VariableMap& VariableMap::value(double value)
{
repn->value(value);
return *this;
}

VariableMap& VariableMap::value(const Expression& value)
{
repn->value(value);
return *this;
}

VariableMap& VariableMap::lower(double value)
{
repn->lower(value);
return *this;
}

VariableMap& VariableMap::lower(const Expression& value)
{
repn->lower(value);
return *this;
}

VariableMap& VariableMap::upper(double value)
{
repn->upper(value);
return *this;
}

VariableMap& VariableMap::upper(const Expression& value)
{
repn->upper(value);
return *this;
}

VariableMap& VariableMap::bounds(double lb, double ub)
{
repn->bounds(lb,ub);
return *this;
}

VariableMap& VariableMap::bounds(const Expression& lb, double ub)
{
repn->bounds(lb,ub);
return *this;
}

VariableMap& VariableMap::bounds(double lb, const Expression& ub)
{
repn->bounds(lb,ub);
return *this;
}

VariableMap& VariableMap::bounds(const Expression& lb, const Expression& ub)
{
repn->bounds(lb,ub);
return *this;
}

VariableMap& VariableMap::name(const std::string& name)
{
repn->name(name);
return *this;
}

VariableMap& VariableMap::within(VariableTypes vtype)
{
repn->within(vtype);
return *this;
}

//
// OTHER
// 

VariableMap variable(const ConcreteSet& arg)
{ return VariableMap(arg); }

VariableMap variable(const SequenceContext& arg)
{ return VariableMap(arg); }


VariableMap& Model::add_variable(VariableMap& vars)
{
if (vars.repn->call_setup)
    vars.repn->setup();
for (auto& var: vars.repn->values)
    add_variable(var);
return vars;
}

/*
VariableMap& Model::add_variable(VariableMap&& vars)
{
if (vars.repn->call_setup)
    vars.repn->setup();
for (auto& var: vars.repn->values)
    add_variable(var);
return vars;
}
*/

}