#pragma once

#include <type_traits>

namespace coek {

template <class TYPE>
class IndexedComponentRepn_multiarray : public IndexedComponentRepn<TYPE> {
   public:
    std::vector<size_t> shape;

   public:
    IndexedComponentRepn_multiarray(size_t n) : IndexedComponentRepn<TYPE>(1), shape({n})
    {
#ifdef CUSTOM_INDEXVECTOR
        this->cache.resize(2 * (n + 1) * 2);
#endif
    }

    IndexedComponentRepn_multiarray(const std::vector<size_t>& _shape)
        : IndexedComponentRepn<TYPE>(_shape.size()), shape(_shape)
    {
        size_t _size = 1;
        for (auto n : shape)
            _size *= n;
#ifdef CUSTOM_INDEXVECTOR
        this->cache.resize(2 * (_size + 1) * (_shape.size() + 1));
#endif
    }

    IndexedComponentRepn_multiarray(const std::initializer_list<size_t>& _shape)
        : IndexedComponentRepn<TYPE>(_shape.size()), shape(_shape)
    {
        size_t _size = 1;
        for (auto n : shape)
            _size *= n;
#ifdef CUSTOM_INDEXVECTOR
        this->cache.resize(2 * (_size + 1) * (_shape.size() + 1));
#endif
    }

    virtual ~IndexedComponentRepn_multiarray() {}

    bool valid_index(const IndexVector& args)
    {
        for (size_t i = 0; i < this->_dim; ++i) {
            size_t tmp = static_cast<size_t>(args[i]);
            if (tmp >= shape[i])
                return false;
        }
        return true;
    }

    void generate_names();
};

template <class TYPE>
void IndexedComponentRepn_multiarray<TYPE>::generate_names()
{
    // If no name has been provided to this array object,
    // then we do not try to generate names.  The default/simple
    // names will be used.
    auto name = this->_name;
    if (name.size() == 0)
        return;

    for (auto& component : this->value) {
        std::string tmp = "[";

        bool comma = false;
        auto shape_size = shape.size();
        for (size_t i = 0; i < shape_size; ++i) {
            if (comma)
                tmp += ",";
            else
                comma = true;
            tmp += std::to_string(component.first[i]);
        }

        tmp += "]";

        component.second.name(name + tmp);
    }
}

#ifdef COEK_WITH_COMPACT_MODEL
template <class TYPE>
class IndexedComponentRepn_setindex : public IndexedComponentRepn<TYPE> {
   public:
    ConcreteSet concrete_set;
    std::vector<int> tmp;

   public:
    IndexedComponentRepn_setindex(ConcreteSet& _arg)
        : IndexedComponentRepn<TYPE>(_arg.dim()), concrete_set(_arg)
    {
#    ifdef CUSTOM_INDEXVECTOR
        this->cache.resize(2 * (this->dim() + 1) * (_arg.size() + 1));
#    endif
        this->tmp.resize(this->dim());
    }

    virtual ~IndexedComponentRepn_setindex() {}

    bool valid_index(const IndexVector& args)
    {
        if (tmp.size() == 1)
            return this->concrete_set.contains(args[0]);
        else {
            for (size_t i = 0; i < tmp.size(); i++)
                tmp[i] = args[i];
            return this->concrete_set.contains(tmp);
        }
    }

    void generate_names();
};

template <class TYPE>
void IndexedComponentRepn_setindex<TYPE>::generate_names()
{
    // If no name has been provided to this map object,
    // then we do not try to generate names.  The default/simple
    // names will be used.
    auto name = this->_name;
    if (name == "")
        return;

    size_t _dim = this->dim();
#    ifdef CUSTOM_INDEXVECTOR
    std::vector<int> x_data(_dim);
    IndexVector x(&(x_data[0]), _dim);
#    else
    IndexVector x(_dim);
#    endif
    for (auto& indices : concrete_set) {
        for (size_t j = 0; j < _dim; j++)
            x[j] = indices[j];
        if (indices.size() == 1) {
            auto tmp = indices[0];
            this->value[x].name(name + "[" + std::to_string(tmp) + "]");
        }
        else {
            std::string _name = name + "[";
            auto tmp = indices[0];
            _name += std::to_string(tmp);
            for (size_t j = 1; j < indices.size(); j++) {
                auto tmp = indices[j];
                _name += "," + std::to_string(tmp);
            }
            this->value[x].name(_name + "]");
        }
    }
}
#endif

template <class TYPE>
TYPE& IndexedComponent_Map<TYPE>::operator()(int i)
{
    if (this->dim() != 1)
        index_error(1);
    this->tmp[0] = i;
    return index(this->tmp);
}

template <class TYPE>
TYPE& IndexedComponent_Map<TYPE>::operator()(int i, int j)
{
    if (this->dim() != 2)
        index_error(2);
    this->tmp[0] = i;
    this->tmp[1] = j;
    return index(this->tmp);
}

template <class TYPE>
IndexedComponent_Map<TYPE>::IndexedComponent_Map(size_t n)
{
    this->repn = std::make_shared<IndexedComponentRepn_multiarray<TYPE>>(n);
    this->repn->resize_index_vectors(this->tmp, this->reftmp);
}

template <class TYPE>
IndexedComponent_Map<TYPE>::IndexedComponent_Map(const std::vector<size_t>& _shape)
{
    this->repn = std::make_shared<IndexedComponentRepn_multiarray<TYPE>>(_shape);
    this->repn->resize_index_vectors(this->tmp, this->reftmp);
}

template <class TYPE>
IndexedComponent_Map<TYPE>::IndexedComponent_Map(const std::initializer_list<size_t>& _shape)
{
    this->repn = std::make_shared<IndexedComponentRepn_multiarray<TYPE>>(_shape);
    this->repn->resize_index_vectors(this->tmp, this->reftmp);
}

#ifdef COEK_WITH_COMPACT_MODEL
template <class TYPE>
IndexedComponent_Map<TYPE>::IndexedComponent_Map(ConcreteSet& arg)
{
    this->repn = std::make_shared<IndexedComponentRepn_setindex<TYPE>>(arg);
    this->repn->resize_index_vectors(this->tmp, this->reftmp);
}
#endif

template <class TYPE>
TYPE& IndexedComponent_Map<TYPE>::index(const IndexVector& args)
{
    assert(this->dim() == args.size());

    if (!(this->repn->valid_index(this->tmp))) {
        std::string err = "Unexpected index value: " + this->repn->_name + "(";
        for (size_t i = 0; i < args.size(); i++) {
            if (i > 0)
                err += ",";
            err += std::to_string(args[i]);
        }
        err += ")";
        throw std::runtime_error(err);
    }

    auto curr = this->repn->value.find(args);
    if (curr == this->repn->value.end()) {
#ifdef CUSTOM_INDEXVECTOR
        auto _args = this->repn->cache.clone(args);
#else
        auto& _args = args;
#endif
        TYPE tmp;
        auto& res = this->repn->value[_args] = tmp;
        return res;
    }
    else
        return curr->second;
}

template <class TYPE>
void IndexedComponent_Map<TYPE>::index_error(size_t i)
{
    std::string err = "Unexpected index value: " + this->repn->_name + " is an "
                      + std::to_string(this->tmp.size())
                      + "-D variable map but is being indexed with " + std::to_string(i)
                      + " indices.";
    throw std::runtime_error(err);
}

}  // namespace coek
