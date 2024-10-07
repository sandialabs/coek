#include <unordered_map>

#include "coek/ast/compact_terms.hpp"
#include "coek/api/data_assoc_array_repn.hpp"
#include "coek/compact/coek_sets.hpp"
#include "coek/compact/data_map.hpp"
#include "coek/compact/index_sequence.hpp"
#include "coek/model/model.hpp"
#include "coek/model/model_repn.hpp"

namespace coek {

class DataMapRepn : public DataAssocArrayRepn {
   public:
    SequenceContext context;
    ConcreteSet index_set;
    IndexSequence index_sequence;
    std::unordered_map<IndexVector, size_t> index_map;

   public:
    DataMapRepn(const ConcreteSet& _arg) : context(_arg), index_set(_arg), index_sequence(context)
    {
#ifdef CUSTOM_INDEXVECTOR
        cache.resize((size() + 1) * (dim() + 1));
#endif
    }

    DataMapRepn(const SequenceContext& _arg) : context(_arg), index_sequence(context)
    {
        index_set = context.index_set();
#ifdef CUSTOM_INDEXVECTOR
        cache.resize((size() + 1) * (dim() + 1));
#endif
    }

    virtual ~DataMapRepn() {}

    std::shared_ptr<ConstantTerm> index(const IndexVector& args);

    void expand();

    size_t dim() { return index_set.dim(); }

    size_t size() { return index_set.size(); }

    std::string get_name(size_t index);
};

void DataMapRepn::expand()
{
    if (first_expand) {
        first_expand = false;
        // DataAssocArrayRepn::expand();

        size_t _dim = dim();
        IndexVector x(_dim);
        size_t i = 0;

        auto it = index_sequence.begin();
        auto end = index_sequence.end();
        while (it != end) {
            auto expr = value_template.value_expression().expand();
            values.emplace_back(CREATE_POINTER(ConstantTerm, expr.value()));

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
// DataMap
//

DataMap::DataMap(const ConcreteSet& arg)
{
    repn = std::make_shared<DataMapRepn>(arg);
    repn->resize_index_vectors(tmp, reftmp);
}

DataMap::DataMap(const SequenceContext& arg)
{
    repn = std::make_shared<DataMapRepn>(arg);
    repn->resize_index_vectors(tmp, reftmp);
}

std::shared_ptr<DataAssocArrayRepn> DataMap::get_repn() { return repn; }
const std::shared_ptr<DataAssocArrayRepn> DataMap::get_repn() const { return repn; }

Expression DataMap::index(const IndexVector& args)
{
    Expression tmp(repn->index(args));
    return tmp;
}

std::shared_ptr<ConstantTerm> DataMapRepn::index(const IndexVector& args)
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
    return values[curr->second];
}

void DataMap::index_error(size_t i)
{
    auto _repn = repn.get();
    std::string err = "Unexpected index value: " + _repn->value_template.name() + " is an "
                      + std::to_string(tmp.size()) + "-D data map but is being indexed with "
                      + std::to_string(i) + " indices.";
    throw std::runtime_error(err);
}

DataMap& DataMap::value(double value)
{
    repn->value(value);
    return *this;
}

DataMap& DataMap::value(const Expression& value)
{
    repn->value(value);
    return *this;
}

DataMap& DataMap::name(const std::string& name)
{
    repn->name(name);
    return *this;
}

//
// OTHER
//

DataMap data(const ConcreteSet& arg) { return DataMap(arg); }

DataMap data(const SequenceContext& arg) { return DataMap(arg); }

DataMap& Model::add_data(DataMap& data)
{
    data.repn->expand();
    repn->data_maps.push_back(data);
    return data;
}

}  // namespace coek
