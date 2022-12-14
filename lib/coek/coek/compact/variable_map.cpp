
#include <unordered_map>

#include "coek/api/variable_assoc_array_repn.hpp"
#include "coek/ast/compact_terms.hpp"
#include "coek/compact/coek_sets.hpp"
#include "coek/compact/variable_map.hpp"
#include "coek/model/model.hpp"
#include "coek/model/model_repn.hpp"

namespace coek {

//
// VariableMapRepn
//

class VariableMapRepn : public VariableAssocArrayRepn {
   public:
    std::unordered_map<IndexVector, size_t> index;
    ConcreteSet concrete_set;

   public:
    VariableMapRepn(const ConcreteSet& _arg) : concrete_set(_arg)
    {
        cache.resize((size() + 1) * (dim() + 1));
    }

    VariableMapRepn(const SequenceContext& _arg) : concrete_set(_arg.index_set())
    {
        cache.resize((size() + 1) * (dim() + 1));
    }

    void setup();

    // TODO - evaluate whether it is reasonable to use const_cast here
    size_t dim() const { return const_cast<ConcreteSet&>(concrete_set).dim(); }

    size_t size() const { return const_cast<ConcreteSet&>(concrete_set).size(); }

    void generate_names();
};

void VariableMapRepn::generate_names()
{
    // If no name has been provided to this map object,
    // then we do not try to generate names.  The default/simple
    // variable names will be used.
    auto name = variable_template.name();
    if (name == "") return;

    setup();

    size_t _dim = dim();
    std::vector<int> x_data(_dim);
    IndexVector x(&(x_data[0]), _dim);
    for (auto& indices : concrete_set) {
        for (size_t j = 0; j < _dim; j++) x[j] = indices[j];
        if (indices.size() == 1) {
            auto tmp = indices[0];
            values[index[x]].name(name + "[" + std::to_string(tmp) + "]");
        }
        else {
            std::string _name = name + "[";
            auto tmp = indices[0];
            _name += std::to_string(tmp);
            for (size_t j = 1; j < indices.size(); j++) {
                auto tmp = indices[j];
                _name += "," + std::to_string(tmp);
            }
            values[index[x]].name(_name + "]");
        }
    }
}

void VariableMapRepn::setup()
{
    if (first_setup) {
        VariableAssocArrayRepn::setup();

        size_t _dim = dim();
        size_t i = 0;
        for (auto& vec : concrete_set) {
            auto x = cache.alloc(_dim);
            for (size_t j = 0; j < _dim; j++) x[j] = vec[j];
            index[x] = i++;
        }
    }
}

//
// VariableMap
//

VariableMap::VariableMap(const ConcreteSet& arg)
{
    repn = std::make_shared<VariableMapRepn>(arg);
    repn->resize_index_vectors(tmp, reftmp);
}

VariableMap::VariableMap(const SequenceContext& arg)
{
    repn = std::make_shared<VariableMapRepn>(arg);
    repn->resize_index_vectors(tmp, reftmp);
}

VariableAssocArrayRepn* VariableMap::get_repn() { return repn.get(); }

const VariableAssocArrayRepn* VariableMap::get_repn() const { return repn.get(); }

Variable VariableMap::index(const IndexVector& args)
{
    assert(dim() == args.size());

    auto _repn = repn.get();
    _repn->setup();

    auto curr = _repn->index.find(tmp);
    if (curr == _repn->index.end()) {
        std::string err = "Unknown index value: " + _repn->variable_template.name() + "[";
        for (size_t i = 0; i < args.size(); i++) {
            if (i > 0) err += ",";
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
    std::string err = "Unexpected index value: " + _repn->variable_template.name() + " is an "
                      + std::to_string(tmp.size()) + "-D variable map but is being indexed with "
                      + std::to_string(i) + " indices.";
    throw std::runtime_error(err);
}

VariableMap& VariableMap::generate_names()
{
    repn->generate_names();
    return *this;
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
    repn->bounds(lb, ub);
    return *this;
}

VariableMap& VariableMap::bounds(const Expression& lb, double ub)
{
    repn->bounds(lb, ub);
    return *this;
}

VariableMap& VariableMap::bounds(double lb, const Expression& ub)
{
    repn->bounds(lb, ub);
    return *this;
}

VariableMap& VariableMap::bounds(const Expression& lb, const Expression& ub)
{
    repn->bounds(lb, ub);
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

VariableMap variable(const ConcreteSet& arg) { return VariableMap(arg); }

VariableMap variable(const SequenceContext& arg) { return VariableMap(arg); }

VariableMap& Model::add_variable(VariableMap& vars)
{
    vars.repn->setup();
    if (repn->name_generation_policy == Model::NameGeneration::eager)
        vars.generate_names();
    else if (repn->name_generation_policy == Model::NameGeneration::lazy)
        repn->variable_maps.push_back(vars);
    for (auto& var : vars.repn->values) add_variable(var);
    return vars;
}

}  // namespace coek
