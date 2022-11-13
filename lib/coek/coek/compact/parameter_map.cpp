#include "coek/compact/parameter_map.hpp"

#include <unordered_map>

#include "coek/api/parameter_assoc_array_repn.hpp"
#include "coek/ast/compact_terms.hpp"
#include "coek/model/model.hpp"

namespace coek {

class ParameterMapRepn : public ParameterAssocArrayRepn {
   public:
    ConcreteSet concrete_set;
    std::unordered_map<IndexVector, size_t> index;
    bool names_generated = false;

   public:
    ParameterMapRepn(const ConcreteSet& _arg) : concrete_set(_arg)
    {
        cache.resize((size() + 1) * (dim() + 1));
    }

    void setup();

    size_t dim() { return concrete_set.dim(); }

    size_t size() { return concrete_set.size(); }

    std::string get_name(size_t index);
};

std::string ParameterMapRepn::get_name(size_t idx)
{
    if (call_setup) setup();

    if (not names_generated) {
        names_generated = true;

        auto name = parameter_template.name();
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

    return values[idx].name();
}

void ParameterMapRepn::setup()
{
    ParameterAssocArrayRepn::setup();

    size_t _dim = dim();
    size_t i = 0;
    for (auto& vec : concrete_set) {
        auto x = cache.alloc(_dim);
        for (size_t j = 0; j < _dim; j++) x[j] = vec[j];
        index[x] = i++;
    }
}

//
// ParameterMap
//

ParameterMap::ParameterMap(const ConcreteSet& arg)
{
    repn = std::make_shared<ParameterMapRepn>(arg);
    repn->resize_index_vectors(tmp, reftmp);
}

ParameterAssocArrayRepn* ParameterMap::get_repn() { return repn.get(); }

Parameter ParameterMap::index(const IndexVector& args)
{
    assert(dim() == args.size());

    auto _repn = repn.get();
    if (_repn->call_setup) _repn->setup();

    auto curr = _repn->index.find(tmp);
    if (curr == _repn->index.end()) {
        std::string err = "Unknown index value: " + _repn->parameter_template.name() + "[";
        for (size_t i = 0; i < args.size(); i++) {
            if (i > 0) err += ",";
            err += std::to_string(args[i]);
        }
        err += "]";
        throw std::runtime_error(err);
    }
    return _repn->values[curr->second];
}

void ParameterMap::index_error(size_t i)
{
    auto _repn = repn.get();
    std::string err = "Unexpected index value: " + _repn->parameter_template.name() + " is an "
                      + std::to_string(tmp.size()) + "-D parameter map but is being indexed with "
                      + std::to_string(i) + " indices.";
    throw std::runtime_error(err);
}

ParameterMap& ParameterMap::value(double value)
{
    repn->value(value);
    return *this;
}

ParameterMap& ParameterMap::value(const Expression& value)
{
    repn->value(value);
    return *this;
}

ParameterMap& ParameterMap::name(const std::string& name)
{
    repn->name(name);
    return *this;
}

//
// OTHER
//

ParameterMap parameter(const ConcreteSet& arg) { return ParameterMap(arg); }

}  // namespace coek
