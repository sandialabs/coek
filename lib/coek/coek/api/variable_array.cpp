
#include "coek/ast/value_terms.hpp"
#include "coek/api/variable_assoc_array_repn.hpp"
#include "coek/model/model.hpp"
#include "coek/model/model_repn.hpp"
#include "coek/api/variable_array.hpp"

namespace coek {

class VariableArrayRepn : public VariableAssocArrayRepn {
   public:
    std::vector<size_t> shape;
    size_t _size;

   public:
    VariableArrayRepn(size_t n) : shape({n}), _size(n) { cache.resize((size() + 1) * 2); }

    VariableArrayRepn(const std::vector<size_t>& _shape) : shape(_shape), _size(1)
    {
        for (auto n : shape) _size *= n;
        cache.resize((size() + 1) * (dim() + 1));
    }

/*
    VariableArrayRepn(const std::vector<int>& _shape) : _size(1)
    {
        shape.resize(_shape.size());
        for (size_t i = 0; i < shape.size(); ++i) shape[i] = static_cast<size_t>(_shape[i]);
        for (auto n : shape) _size *= n;
        cache.resize((size() + 1) * (dim() + 1));
    }
*/

    VariableArrayRepn(const std::initializer_list<size_t>& _shape) : shape(_shape), _size(1)
    {
        for (auto n : shape) _size *= n;
        cache.resize((size() + 1) * (dim() + 1));
    }

    size_t dim() const { return shape.size(); }

    size_t size() const { return _size; }

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
        for (size_t i = 1; i < shape.size(); ++i) name += "," + std::to_string(tmp[i]);
    }

    name += "]";
    return name;
}

void VariableArrayRepn::generate_names()
{
    // If no name has been provided to this array object,
    // then we do not try to generate names.  The default/simple
    // variable names will be used.
    std::string name = variable_template.name();
    if (name == "") return;

    setup();

    size_t ctr = 0;
    for (auto& var : values) var.name(get_name(name, ctr++));
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

VariableAssocArrayRepn* VariableArray::get_repn() { return repn.get(); }

const VariableAssocArrayRepn* VariableArray::get_repn() const { return repn.get(); }

Variable VariableArray::index(const IndexVector& args)
{
    auto _repn = repn.get();
    auto& shape = _repn->shape;
    assert(args.size() == shape.size());

    _repn->setup();

    // We know that the args[i] values are nonnegative b.c. we have asserted that while
    // processing these arguments
    size_t ndx = static_cast<size_t>(args[0]);
    for (size_t i = 1; i < args.size(); i++) ndx = ndx * shape[i] + static_cast<size_t>(args[i]);

    if (ndx > size()) {
        std::string err = "Unknown index value: " + _repn->variable_template.name() + "[";
        for (size_t i = 0; i < args.size(); i++) {
            if (i > 0) err += ",";
            err += std::to_string(args[i]);
        }
        err += "]";
        throw std::runtime_error(err);
    }

    return _repn->values[ndx];
}

void VariableArray::index_error(size_t i)
{
    auto _repn = repn.get();
    std::string err = "Unexpected index value: " + _repn->variable_template.name() + " is an "
                      + std::to_string(tmp.size()) + "-D variable array but is being indexed with "
                      + std::to_string(i) + " indices.";
    throw std::runtime_error(err);
}

VariableArray::const_iterator VariableArray::cbegin() const noexcept { return repn->values.begin(); }

VariableArray::const_iterator VariableArray::cend() const noexcept { return repn->values.end(); }

VariableArray::iterator VariableArray::begin() noexcept { return repn->values.begin(); }

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
    repn->variable_template.name(name);
    repn->name(name);
    return *this;
}

std::string VariableArray::name() const { return repn->variable_template.repn->name; }

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
    vars.repn->setup();
    if (repn->name_generation_policy == Model::NameGeneration::eager)
        vars.generate_names();
    else if (repn->name_generation_policy == Model::NameGeneration::lazy)
        repn->variable_arrays.push_back(vars);
    for (auto& var : vars.repn->values) add_variable(var);
    return vars;
}

}  // namespace coek
