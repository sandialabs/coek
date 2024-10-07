#include <unordered_map>

#include "coek/ast/compact_terms.hpp"
#include "coek/api/parameter_assoc_array_repn.hpp"
#include "coek/compact/coek_sets.hpp"
#include "coek/compact/parameter_map.hpp"
#include "coek/compact/index_sequence.hpp"
#include "coek/model/model.hpp"
#include "coek/model/model_repn.hpp"

namespace coek {

class ParameterMapRepn : public ParameterAssocArrayRepn {
   public:
    SequenceContext context;
    ConcreteSet index_set;
    IndexSequence index_sequence;
    std::unordered_map<IndexVector, size_t> index_map;
    bool names_generated = false;

   public:
    ParameterMapRepn(const ConcreteSet& _arg) : context(_arg), index_set(_arg), index_sequence(context)
    {
#ifdef CUSTOM_INDEXVECTOR
        cache.resize((size() + 1) * (dim() + 1));
#endif
    }

    //ParameterMapRepn(const SequenceContext& _arg) : concrete_set(_arg.index_set())
    ParameterMapRepn(const SequenceContext& _arg) : context(_arg), index_sequence(context)
    {
        index_set = context.index_set();
#ifdef CUSTOM_INDEXVECTOR
        cache.resize((size() + 1) * (dim() + 1));
#endif
    }

    virtual ~ParameterMapRepn() {}

    std::shared_ptr<ParameterTerm> index(const IndexVector& args);

    void expand();

    size_t dim() { return index_set.dim(); }

    size_t size() { return index_set.size(); }

    std::string get_name(size_t index);

    void generate_names();
};

void ParameterMapRepn::generate_names()
{
    // If no name has been provided to this map object,
    // then we do not try to generate names.  The default/simple
    // parameter names will be used.
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
    for (auto& indices : index_set) {
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

void ParameterMapRepn::expand()
{
    if (first_expand) {
        first_expand = false;
        //ParameterAssocArrayRepn::expand();

        size_t _dim = dim();
        IndexVector x(_dim);
        size_t i = 0;

        auto it = index_sequence.begin();
        auto end = index_sequence.end();
        while (it != end) {
            auto expr = value_template.value_expression().expand();
            values.emplace_back(CREATE_POINTER(ParameterTerm, CREATE_POINTER(ConstantTerm, expr.value())));

            auto& vec = *it;
            assert(vec.size() == _dim);
            for (size_t j = 0; j < _dim; j++)
                vec[j].get_value(x[j]);
            index_map[x] = i++;
            ++it;
            }
        assert(index_map.size() == index_set.size());
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

ParameterMap::ParameterMap(const SequenceContext& arg)
{
    repn = std::make_shared<ParameterMapRepn>(arg);
    repn->resize_index_vectors(tmp, reftmp);
}

std::shared_ptr<ParameterAssocArrayRepn> ParameterMap::get_repn() { return repn; }
const std::shared_ptr<ParameterAssocArrayRepn> ParameterMap::get_repn() const { return repn; }

Parameter ParameterMap::index(const IndexVector& args) { return repn->index(args); }

std::shared_ptr<ParameterTerm> ParameterMapRepn::index(const IndexVector& args)
{
    assert(dim() == args.size());
    expand();

    // std::cerr << "HERE " << index_map.size() << std::endl;
    // std::cerr << "HERE " << args << std::endl;
    // for (auto& [k,v]:index_map)
    //     std::cerr << "HERE " << k << " " << v << std::endl;

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

void ParameterMap::index_error(size_t i)
{
    auto _repn = repn.get();
    std::string err = "Unexpected index value: " + _repn->value_template.name() + " is an "
                      + std::to_string(tmp.size()) + "-D parameter map but is being indexed with "
                      + std::to_string(i) + " indices.";
    throw std::runtime_error(err);
}

ParameterMap& ParameterMap::generate_names()
{
    repn->generate_names();
    return *this;
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

ParameterMap parameter(const SequenceContext& arg) { return ParameterMap(arg); }

ParameterMap& Model::add_parameter(ParameterMap& params)
{
    params.repn->expand();
    if (repn->name_generation_policy == Model::NameGeneration::eager)
        params.generate_names();
    repn->parameter_maps.push_back(params);
    return params;
}

}  // namespace coek
