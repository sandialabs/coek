#include "coek/ast/value_terms.hpp"
#include "coek/api/parameter_array.hpp"
#include "coek/api/parameter_assoc_array_repn.hpp"
#include "coek/model/model.hpp"
#include "coek/model/model_repn.hpp"

namespace coek {

class ParameterArrayRepn : public ParameterAssocArrayRepn {
   public:
    std::vector<size_t> shape;
    size_t _size;

   public:
    ParameterArrayRepn(size_t n) : shape({n}), _size(n)
    {
#ifdef CUSTOM_INDEXVECTOR
        cache.resize(2 * (size() + 1) * 2);
#endif
    }

    ParameterArrayRepn(const std::vector<size_t>& _shape) : shape(_shape), _size(1)
    {
        for (auto n : shape)
            _size *= n;
#ifdef CUSTOM_INDEXVECTOR
        cache.resize(2 * (size() + 1) * (dim() + 1));
#endif
    }

    ParameterArrayRepn(const std::initializer_list<size_t>& _shape) : shape(_shape), _size(1)
    {
        for (auto n : shape)
            _size *= n;
#ifdef CUSTOM_INDEXVECTOR
        cache.resize(2 * (size() + 1) * (dim() + 1));
#endif
    }

    virtual ~ParameterArrayRepn() {}

    std::shared_ptr<ParameterTerm> index(const IndexVector& args);

    size_t dim() { return shape.size(); }

    size_t size() { return _size; }

    std::string get_name(std::string name, size_t index);

    void generate_names();
};

std::string ParameterArrayRepn::get_name(std::string name, size_t index)
{
    name += "[";

    if (shape.size() == 1) {
        name += std::to_string(index);
    }

    else if (shape.size() > 1) {
        std::vector<size_t> tmp(shape.size());
        for (size_t i = 1; i <= shape.size(); ++i) {
            size_t j = shape.size() - i;
            tmp[j] = index % shape[j];
            index = index / shape[j];
        }
        name += std::to_string(tmp[0]);
        for (size_t i = 1; i < shape.size(); ++i)
            name += "," + std::to_string(tmp[i]);
    }

    name += "]";
    return name;
}

void ParameterArrayRepn::generate_names()
{
    // If no name has been provided to this array object,
    // then we do not try to generate names.  The default/simple
    // parameter names will be used.
    std::string name = value_template.name();
    if (name.size() == 0)
        return;

    expand();

    size_t ctr = 0;
    for (auto& param : values)
        param.name(get_name(name, ctr++));
}

//
// ParameterArray
//

ParameterArray::ParameterArray(size_t n)
{
    repn = std::make_shared<ParameterArrayRepn>(n);
    repn->resize_index_vectors(tmp, reftmp);
}

ParameterArray::ParameterArray(const std::vector<size_t>& shape)
{
    repn = std::make_shared<ParameterArrayRepn>(shape);
    repn->resize_index_vectors(tmp, reftmp);
}

ParameterArray::ParameterArray(const std::initializer_list<size_t>& shape)
{
    repn = std::make_shared<ParameterArrayRepn>(shape);
    repn->resize_index_vectors(tmp, reftmp);
}

std::shared_ptr<ParameterAssocArrayRepn> ParameterArray::get_repn() { return repn; }

Parameter ParameterArray::index(const IndexVector& args) { return repn->index(args); }

std::shared_ptr<ParameterTerm> ParameterArrayRepn::index(const IndexVector& args)
{
    // auto _repn = repn.get();
    // auto& shape = _repn->shape;
    assert(args.size() == shape.size());

    expand();

    // We know that the args[i] values are nonnegative b.c. we have asserted that while
    // processing these arguments
    size_t ndx = static_cast<size_t>(args[0]);
    for (size_t i = 1; i < args.size(); i++)
        ndx = ndx * shape[i] + static_cast<size_t>(args[i]);

    if (ndx > size()) {
        // TODO - Can't we do better than this check?  Do we check if each index is in the correct
        // range?
        std::string err = "Unknown index value: " + value_template.name() + "[";
        for (size_t i = 0; i < args.size(); i++) {
            if (i > 0)
                err += ",";
            err += std::to_string(args[i]);
        }
        err += "]";
        throw std::runtime_error(err);
    }

    return values[ndx].repn;
}

void ParameterArray::index_error(size_t i)
{
    auto _repn = repn.get();
    std::string err = "Unexpected index value: " + _repn->value_template.name() + " is an "
                      + std::to_string(tmp.size()) + "-D parameter array but is being indexed with "
                      + std::to_string(i) + " indices.";
    throw std::runtime_error(err);
}

ParameterArray& ParameterArray::generate_names()
{
    repn->generate_names();
    return *this;
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

ParameterArray parameter(size_t n) { return ParameterArray(n); }

ParameterArray parameter(const std::vector<size_t>& shape) { return ParameterArray(shape); }

ParameterArray parameter(const std::initializer_list<size_t>& shape)
{
    return ParameterArray(shape);
}

ParameterArray& Model::add_parameter(ParameterArray& params)
{
    params.repn->expand();
    if (repn->name_generation_policy == Model::NameGeneration::eager)
        params.generate_names();
    else if (repn->name_generation_policy == Model::NameGeneration::lazy)
        repn->parameter_arrays.push_back(params);
    return params;
}

}  // namespace coek
