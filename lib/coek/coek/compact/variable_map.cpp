#include <unordered_map>

#include "coek/api/variable_assoc_array_repn.hpp"
#include "coek/ast/compact_terms.hpp"
#include "coek/compact/coek_sets.hpp"
#include "coek/compact/variable_map.hpp"
#include "coek/compact/index_sequence.hpp"
#include "coek/model/model.hpp"
#include "coek/model/model_repn.hpp"

namespace coek {

// void xyz();

//
// VariableMapRepn
//

class VariableMapRepn : public VariableAssocArrayRepn {
   public:
    SequenceContext context;
    ConcreteSet index_set;
    IndexSequence index_sequence;
    std::unordered_map<IndexVector, size_t> index_map;

   public:
    VariableMapRepn(const ConcreteSet& _arg)
        : context(_arg), index_set(_arg), index_sequence(context)
    {
#ifdef CUSTOM_INDEXVECTOR
        cache.resize((size() + 1) * (dim() + 1));
#endif
    }

    VariableMapRepn(const SequenceContext& _arg) : context(_arg), index_sequence(context)
    {
        index_set = context.index_set();
#ifdef CUSTOM_INDEXVECTOR
        cache.resize((size() + 1) * (dim() + 1));
#endif
    }

    virtual ~VariableMapRepn() {}

    std::shared_ptr<VariableTerm> index(const IndexVector& args);

    void expand();

    // TODO - evaluate whether it is reasonable to use const_cast here
    size_t dim() { return index_set.dim(); }

    size_t size() { return index_set.size(); }

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

void VariableMapRepn::expand()
{
    if (first_expand) {
        first_expand = false;
        // VariableAssocArrayRepn::expand();

        size_t _dim = dim();
        IndexVector x(_dim);
        size_t i = 0;

        auto it = index_sequence.begin();
        auto end = index_sequence.end();
        while (it != end) {
            auto vtype = value_template.within();
            bool binary = (vtype == Boolean) or (vtype == Binary);
            bool integer = vtype == Integers;
            auto lower = std::make_shared<ConstantTerm>(
                value_template.lower_expression().expand().value());
            auto upper = std::make_shared<ConstantTerm>(
                value_template.upper_expression().expand().value());
            auto value = std::make_shared<ConstantTerm>(
                value_template.value_expression().expand().value());
            values.emplace_back(CREATE_POINTER(VariableTerm, lower, upper, value, binary, integer));

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
