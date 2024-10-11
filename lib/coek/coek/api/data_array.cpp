#include "coek/ast/value_terms.hpp"
#include "coek/api/data_array.hpp"
#include "coek/api/data_assoc_array_repn.hpp"
#include "coek/model/model.hpp"
#include "coek/model/model_repn.hpp"

namespace coek {

class DataArrayRepn : public DataAssocArrayRepn {
   public:
    std::vector<size_t> shape;
    size_t _size;

   public:
    DataArrayRepn(size_t n) : shape({n}), _size(n)
    {
#ifdef CUSTOM_INDEXVECTOR
        cache.resize(2 * (size() + 1) * 2);
#endif
    }

    DataArrayRepn(const std::vector<size_t>& _shape) : shape(_shape), _size(1)
    {
        for (auto n : shape)
            _size *= n;
#ifdef CUSTOM_INDEXVECTOR
        cache.resize(2 * (size() + 1) * (dim() + 1));
#endif
    }

    DataArrayRepn(const std::initializer_list<size_t>& _shape) : shape(_shape), _size(1)
    {
        for (auto n : shape)
            _size *= n;
#ifdef CUSTOM_INDEXVECTOR
        cache.resize(2 * (size() + 1) * (dim() + 1));
#endif
    }

    virtual ~DataArrayRepn() {}

    std::shared_ptr<ConstantTerm> index(const IndexVector& args);

    size_t dim() { return shape.size(); }

    size_t size() { return _size; }

    std::string get_name(std::string name, size_t index);
};

std::string DataArrayRepn::get_name(std::string name, size_t index)
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

//
// DataArray
//

DataArray::DataArray(size_t n)
{
    repn = std::make_shared<DataArrayRepn>(n);
    repn->resize_index_vectors(tmp, reftmp);
}

DataArray::DataArray(const std::vector<size_t>& shape)
{
    repn = std::make_shared<DataArrayRepn>(shape);
    repn->resize_index_vectors(tmp, reftmp);
}

DataArray::DataArray(const std::initializer_list<size_t>& shape)
{
    repn = std::make_shared<DataArrayRepn>(shape);
    repn->resize_index_vectors(tmp, reftmp);
}

std::shared_ptr<DataAssocArrayRepn> DataArray::get_repn() { return repn; }

const std::shared_ptr<DataAssocArrayRepn> DataArray::get_repn() const { return repn; }

Expression DataArray::index(const IndexVector& args)
{
    Expression tmp(repn->index(args));
    return tmp;
}

std::shared_ptr<ConstantTerm> DataArrayRepn::index(const IndexVector& args)
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

    return values[ndx];
}

void DataArray::index_error(size_t i)
{
    auto _repn = repn.get();
    std::string err = "Unexpected index value: " + _repn->value_template.name() + " is an "
                      + std::to_string(tmp.size()) + "-D data array but is being indexed with "
                      + std::to_string(i) + " indices.";
    throw std::runtime_error(err);
}

DataArray& DataArray::value(double value)
{
    repn->value(value);
    return *this;
}

DataArray& DataArray::value(const Expression& value)
{
    repn->value(value);
    return *this;
}

DataArray& DataArray::name(const std::string& name)
{
    repn->name(name);
    return *this;
}

std::string DataArray::name() const { return repn->value_template.repn->name; }

//
// OTHER
//

DataArray data(size_t n) { return DataArray(n); }

DataArray data(const std::vector<size_t>& shape) { return DataArray(shape); }

DataArray data(const std::initializer_list<size_t>& shape) { return DataArray(shape); }

DataArray& Model::add_data(DataArray& data)
{
    data.repn->expand();
    repn->data_arrays.push_back(data);
    return data;
}

}  // namespace coek
