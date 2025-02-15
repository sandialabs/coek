#include "coek/ast/value_terms.hpp"
#include "coek/api/variable_array.hpp"
#include "coek/api/variable_assoc_array_repn.hpp"
#include "coek/model/model.hpp"
#include "coek/model/model_repn.hpp"

namespace coek {

class VariableArrayRepn : public VariableAssocArrayRepn {
   public:
    std::vector<size_t> shape;
    size_t _size;

   public:
    VariableArrayRepn(size_t n) : shape({n}), _size(n)
    {
#ifdef CUSTOM_INDEXVECTOR
        cache.resize(2 * (size() + 1) * 2);
#endif
    }

    VariableArrayRepn(const std::vector<size_t>& _shape) : shape(_shape), _size(1)
    {
        for (auto n : shape)
            _size *= n;
#ifdef CUSTOM_INDEXVECTOR
        cache.resize(2 * (size() + 1) * (dim() + 1));
#endif
    }

    VariableArrayRepn(const std::initializer_list<size_t>& _shape) : shape(_shape), _size(1)
    {
        for (auto n : shape)
            _size *= n;
#ifdef CUSTOM_INDEXVECTOR
        cache.resize(2 * (size() + 1) * (dim() + 1));
#endif
    }

    virtual ~VariableArrayRepn() {}

    std::shared_ptr<VariableTerm> index(const IndexVector& args);

    size_t dim() { return shape.size(); }

    size_t size() { return _size; }

    std::string get_name(std::string name, size_t index);

    void generate_names();
};

std::string VariableArrayRepn::get_name(std::string name, size_t index)
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

void VariableArrayRepn::generate_names()
{
    // If no name has been provided to this array object,
    // then we do not try to generate names.  The default/simple
    // variable names will be used.
    std::string name = value_template.name();
    if (name == "")
        return;

    expand();

    size_t ctr = 0;
    for (auto& var : values)
        var.name(get_name(name, ctr++));
}

//
// VariableArray
//

VariableArray::VariableArray(size_t n)
{
    repn = std::make_shared<VariableArrayRepn>(n);
    repn->resize_index_vectors(tmp, reftmp);
}

VariableArray::VariableArray(const std::vector<size_t>& shape)
{
    repn = std::make_shared<VariableArrayRepn>(shape);
    repn->resize_index_vectors(tmp, reftmp);
}

/*
VariableArray::VariableArray(const std::vector<int>& shape)
{
    repn = std::make_shared<VariableArrayRepn>(shape);
    repn->resize_index_vectors(tmp, reftmp);
}
*/

VariableArray::VariableArray(const std::initializer_list<size_t>& shape)
{
    repn = std::make_shared<VariableArrayRepn>(shape);
    repn->resize_index_vectors(tmp, reftmp);
}

std::shared_ptr<VariableAssocArrayRepn> VariableArray::get_repn() { return repn; }
const std::shared_ptr<VariableAssocArrayRepn> VariableArray::get_repn() const { return repn; }

Variable VariableArray::index(const IndexVector& args) { return repn->index(args); }

std::shared_ptr<VariableTerm> VariableArrayRepn::index(const IndexVector& args)
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

void VariableArray::index_error(size_t i)
{
    auto _repn = repn.get();
    std::string err = "Unexpected index value: " + _repn->value_template.name() + " is an "
                      + std::to_string(tmp.size()) + "-D variable array but is being indexed with "
                      + std::to_string(i) + " indices.";
    throw std::runtime_error(err);
}

VariableArray::const_iterator VariableArray::cbegin() const noexcept
{
    repn->expand();
    return repn->values.begin();
}

VariableArray::const_iterator VariableArray::cend() const noexcept { return repn->values.end(); }

VariableArray::iterator VariableArray::begin() noexcept
{
    repn->expand();
    return repn->values.begin();
}

VariableArray::iterator VariableArray::end() noexcept { return repn->values.end(); }

VariableArray& VariableArray::generate_names()
{
    repn->generate_names();
    return *this;
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
    repn->bounds(lb, ub);
    return *this;
}

VariableArray& VariableArray::bounds(const Expression& lb, double ub)
{
    repn->bounds(lb, ub);
    return *this;
}

VariableArray& VariableArray::bounds(double lb, const Expression& ub)
{
    repn->bounds(lb, ub);
    return *this;
}

VariableArray& VariableArray::bounds(const Expression& lb, const Expression& ub)
{
    repn->bounds(lb, ub);
    return *this;
}

VariableArray& VariableArray::fixed(bool value)
{
    repn->fixed(value);
    return *this;
}

VariableArray& VariableArray::name(const std::string& name)
{
    repn->value_template.name(name);
    repn->name(name);
    return *this;
}

std::string VariableArray::name() const { return repn->value_template.repn->name; }

VariableArray& VariableArray::within(VariableTypes vtype)
{
    repn->within(vtype);
    return *this;
}

//
// OTHER
//

VariableArray variable(size_t n) { return VariableArray(n); }

VariableArray variable(const std::vector<size_t>& shape) { return VariableArray(shape); }

VariableArray variable(const std::initializer_list<size_t>& shape) { return VariableArray(shape); }

VariableArray& Model::add_variable(VariableArray& vars)
{
    vars.expand();
    if (repn->name_generation_policy == Model::NameGeneration::eager)
        vars.generate_names();
    repn->variable_arrays.push_back(vars);
    for (auto& var : vars.repn->values)
        add_variable(var);
    return vars;
}

}  // namespace coek
