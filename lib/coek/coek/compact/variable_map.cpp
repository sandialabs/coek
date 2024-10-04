
#include <unordered_map>

#include "coek/api/variable_assoc_array_repn.hpp"
#include "coek/ast/compact_terms.hpp"
#include "coek/compact/coek_sets.hpp"
#include "coek/compact/variable_map.hpp"
#include "coek/model/model.hpp"
#include "coek/model/model_repn.hpp"

namespace coek {

// void xyz();

//
// VariableMapRepn
//

class VariableMapRepn : public VariableAssocArrayRepn {
   public:
    std::unordered_map<IndexVector, size_t> index_map;
    ConcreteSet concrete_set;

   public:
    VariableMapRepn(const ConcreteSet& _arg) : concrete_set(_arg)
    {
#ifdef CUSTOM_INDEXVECTOR
        cache.resize((size() + 1) * (dim() + 1));
#endif
    }

    VariableMapRepn(const SequenceContext& _arg) : concrete_set(_arg.index_set())
    {
#ifdef CUSTOM_INDEXVECTOR
        cache.resize((size() + 1) * (dim() + 1));
#endif
    }

    virtual ~VariableMapRepn() {}

    std::shared_ptr<VariableTerm> index(const IndexVector& args);

    void expand();

    // TODO - evaluate whether it is reasonable to use const_cast here
    size_t dim() { return const_cast<ConcreteSet&>(concrete_set).dim(); }

    size_t size() { return const_cast<ConcreteSet&>(concrete_set).size(); }

    void generate_names();
};

void VariableMapRepn::generate_names()
{
    // xyz();
    //  If no name has been provided to this map object,
    //  then we do not try to generate names.  The default/simple
    //  variable names will be used.
    auto name = value_template.name();
    if (name == "")
        return;

    expand();

    size_t _dim = dim();
#ifdef CUSTOM_INDEXVECTOR
    std::vector<int> x_data(_dim);
    IndexVector x(&(x_data[0]), _dim);
#else
    IndexVector x(_dim);
#endif
    for (auto& indices : concrete_set) {
        for (size_t j = 0; j < _dim; j++)
            x[j] = indices[j];
        if (indices.size() == 1) {
            auto tmp = indices[0];
            values[index_map[x]].name(name + "[" + std::to_string(tmp) + "]");
        }
        else {
            std::string _name = name + "[";
            auto tmp = indices[0];
            _name += std::to_string(tmp);
            for (size_t j = 1; j < indices.size(); j++) {
                auto tmp = indices[j];
                _name += "," + std::to_string(tmp);
            }
            values[index_map[x]].name(_name + "]");
        }
    }
}

void VariableMapRepn::expand()
{
    if (first_expand) {
        VariableAssocArrayRepn::expand();

        size_t _dim = dim();
        size_t i = 0;
#ifdef CUSTOM_INDEXVECTOR
        for (auto& vec : concrete_set) {
            auto x = cache.alloc(_dim);
            for (size_t j = 0; j < _dim; j++)
                x[j] = vec[j];
            index_map[x] = i++;
        }
#else
        IndexVector x(_dim);
        for (auto& vec : concrete_set) {
            for (size_t j = 0; j < _dim; j++)
                x[j] = vec[j];
            index_map[x] = i++;
        }
#endif
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

std::shared_ptr<VariableAssocArrayRepn> VariableMap::get_repn() { return repn; }

const std::shared_ptr<VariableAssocArrayRepn> VariableMap::get_repn() const { return repn; }

Variable VariableMap::index(const IndexVector& args) { return repn->index(args); }

std::shared_ptr<VariableTerm> VariableMapRepn::index(const IndexVector& args)
{
    assert(dim() == args.size());

    // auto _repn = repn.get();
    expand();

    auto curr = index_map.find(args);
    if (curr == index_map.end()) {
        std::string err = "Unknown index value: " + value_template.name() + "[";
        for (size_t i = 0; i < args.size(); i++) {
            if (i > 0)
                err += ",";
            err += std::to_string(args[i]);
        }
        err += "]";
        throw std::runtime_error(err);
    }
    return values[curr->second].repn;
}

void VariableMap::index_error(size_t i)
{
    auto _repn = repn.get();
    std::string err = "Unexpected index value: " + _repn->value_template.name() + " is an "
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

std::string VariableMap::name() { return repn->name(); }

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
    vars.repn->expand();
    if (repn->name_generation_policy == Model::NameGeneration::eager)
        vars.generate_names();
    repn->variable_maps.push_back(vars);
    for (auto& var : vars.repn->values)
        add_variable(var);
    return vars;
}

}  // namespace coek
