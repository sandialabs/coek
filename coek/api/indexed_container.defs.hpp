#pragma once


namespace coek {

template <class TYPE>
class IndexedComponentRepn_multiarray : public IndexedComponentRepn<TYPE>
{
public:

    std::vector<size_t> shape;

public:

    IndexedComponentRepn_multiarray(size_t n)
        : IndexedComponentRepn<TYPE>(1),
          shape({n})
        {
        this->cache.resize((n+1)*2);
        }

    IndexedComponentRepn_multiarray(const std::vector<size_t>& _shape)
        : IndexedComponentRepn<TYPE>(_shape.size()),
          shape(_shape)
        {
        size_t _size = 1;
        for (auto n : shape)
            _size *= n;
        this->cache.resize((_size+1)*(_shape.size()+1));
        }

    IndexedComponentRepn_multiarray(const std::initializer_list<size_t>& _shape)
        : IndexedComponentRepn<TYPE>(_shape.size()),
          shape(_shape)
        {
        size_t _size = 1;
        for (auto n : shape)
            _size *= n;
        this->cache.resize((_size+1)*(_shape.size()+1));
        }

    bool valid_index(const IndexVector& args)
        {
        for (size_t i=0; i<this->_dim; ++i) {
            size_t tmp = static_cast<size_t>(args[i]);
            if ((tmp < 0) or (tmp >= shape[i]))
                return false;
            }
        return true;
        }
};


#ifdef COEK_WITH_COMPACT_MODEL
template <class TYPE>
class IndexedComponentRepn_setindex : public IndexedComponentRepn<TYPE>
{
public:

    ConcreteSet concrete_set;

public:

    IndexedComponentRepn_setindex(ConcreteSet& _arg)
        : IndexedComponentRepn<TYPE>(_arg.dim()),
          concrete_set(_arg)
        {
        this->cache.resize((this->dim()+1) * (_arg.size()+1));
        }

    bool valid_index(const IndexVector& args)
        {
        auto it = this->value.find(args);
        return !(it == this->value.end());
        }
};
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
        this->repn->resize_index_vectors(this->tmp,this->reftmp);
        }

template <class TYPE>
IndexedComponent_Map<TYPE>::IndexedComponent_Map(const std::vector<size_t>& _shape)
        {
        this->repn = std::make_shared<IndexedComponentRepn_multiarray<TYPE>>(_shape);
        this->repn->resize_index_vectors(this->tmp,this->reftmp);
        }

template <class TYPE>
IndexedComponent_Map<TYPE>::IndexedComponent_Map(const std::initializer_list<size_t>& _shape)
        {
        this->repn = std::make_shared<IndexedComponentRepn_multiarray<TYPE>>(_shape);
        this->repn->resize_index_vectors(this->tmp,this->reftmp);
        }

#ifdef COEK_WITH_COMPACT_MODEL
template <class TYPE>
IndexedComponent_Map<TYPE>::IndexedComponent_Map(ConcreteSet& arg)
    {
    this->repn = std::make_shared<IndexedComponentRepn_setindex<TYPE>>(arg);
    this->repn->resize_index_vectors(this->tmp,this->reftmp);
    }
#endif

template <class TYPE>
TYPE& IndexedComponent_Map<TYPE>::index(const IndexVector& args)
{
assert(this->dim() == args.size());

if (!(this->repn->valid_index(this->tmp))) {
    std::string err = "Unexpected index value: "+this->repn->_name+"(";
    for (size_t i=0; i<args.size(); i++) {
        if (i > 0)
            err += ",";
        err += std::to_string(args[i]);
        }
    err += ")";
    throw std::runtime_error(err); 
    }

auto curr = this->repn->value.find(args);
if (curr == this->repn->value.end()) {
    auto _args = this->repn->cache.clone(args);
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
std::string err = "Unexpected index value: "+this->repn->_name+
    " is an "+std::to_string(this->tmp.size())+
    "-D variable map but is being indexed with "+std::to_string(i)+" indices.";
throw std::runtime_error(err);
}

}
