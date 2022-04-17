#pragma once

#include <map>
#include <cassert>
#include <string>
#include <vector>
#include <variant>
#include <unordered_map>
#include <memory>
#include "coek/compact/coek_sets.hpp"
#include "coek/api/expression.hpp"
#include "coek/util/index_vector.hpp"
#include "coek/util/template_utils.hpp"


namespace coek {

class BaseExpressionTerm;

typedef BaseExpressionTerm* expr_pointer_t;
typedef std::variant<int,expr_pointer_t> refarg_types;


template <class TYPE>
class IndexedComponentRepn
{
public:

    IndexVectorCache cache;
    std::map<size_t, std::string> names;
    std::map<IndexVector, size_t> index;
    ConcreteSet concrete_set;
    std::vector<size_t> shape;
    size_t _size;
    size_t _dim;
    std::vector<TYPE> value;
    std::string _name;

public:

    IndexedComponentRepn(size_t n)
        : shape({n}), _size(n), _dim(1)
        { cache.resize(size()); }

    IndexedComponentRepn(const std::vector<size_t>& _shape)
        : shape(_shape), _size(1), _dim(_shape.size())
        {
        for (auto n : shape)
            _size *= n;
        cache.resize(size());
        }

    IndexedComponentRepn(const std::initializer_list<size_t>& _shape)
        : shape(_shape), _size(1), _dim(_shape.size())
        {
        for (auto n : shape)
            _size *= n;
        cache.resize(size());
        }

    IndexedComponentRepn(ConcreteSet& _arg)
        : concrete_set(_arg), _size(_arg.size()), _dim(_arg.dim())
        { cache.resize((dim()+2)*size()); }

    size_t dim()
        {return _dim;}

    size_t size()
        {return _size;}

    void resize_index_vectors(IndexVector& tmp, std::vector<refarg_types>& reftmp)
            {
            tmp = cache.alloc(dim());
            reftmp.resize(dim());
            }
};


template <class TYPE>
class IndexedComponent
{
public:

    std::shared_ptr<IndexedComponentRepn<TYPE>> repn;
    IndexVector tmp;
    std::vector<refarg_types> reftmp;

public:

    typename std::vector<TYPE>::iterator begin()
        { return repn->value.begin(); }
    typename std::vector<TYPE>::iterator end()
        { return repn->value.end(); }

    size_t size()
        { return repn->size(); }
    size_t dim()
        { return repn->dim(); }

    virtual TYPE index(const IndexVector& args) = 0;

    //Expression create_varref(const std::vector<refarg_types>& indices);

    std::string name()
        { return repn->_name; }
};


template <class TYPE>
class IndexedComponent_Map : public IndexedComponent<TYPE>
{
public:

    TYPE index(const IndexVector& args);
    void index_error(size_t i);

    IndexedComponent_Map<TYPE>& name(const std::string& str)
        {
        this->repn->_name = str;
        return *this;
        }

public:

    /// Collect arguments with int, size_t or parameter indices

    void collect_args(size_t i, size_t arg)
        { this->tmp[i] = static_cast<int>(arg); }

    void collect_args(size_t i, int arg)
        { this->tmp[i] = arg; }

    void collect_args(size_t i, const Parameter& arg)
        { this->tmp[i] = static_cast<int>(arg.value()); }


    template <typename... ARGTYPES>
    void collect_args(size_t i, size_t arg, const ARGTYPES&... args)
        {
        this->tmp[i] = static_cast<int>(arg);
        collect_args(i+1, args...);
        }

    template <typename... ARGTYPES>
    void collect_args(size_t i, int arg, const ARGTYPES&... args)
        {
        this->tmp[i] = arg;
        collect_args(i+1, args...);
        }

    template <typename... ARGTYPES>
    void collect_args(size_t i, const Parameter& arg, const ARGTYPES&... args)
        {
        this->tmp[i] = static_cast<int>(arg.value());
        collect_args(i+1, args...);
        }

    template <typename... ARGTYPES>
    TYPE operator()(const ARGTYPES&... args)
        {
        const size_t nargs = count_args(args...);
        if (this->dim() != nargs)
            index_error(nargs);
        collect_args(static_cast<size_t>(0), args...);
        return index(this->tmp);
        }

    TYPE operator()(int i)
        {
        if (this->dim() != 1)
            index_error(1);
        this->tmp[0] = i;
        return index(this->tmp);
        }

    TYPE operator()(int i, int j)
        {
        if (this->dim() != 2)
            index_error(2);
        this->tmp[0] = i;
        this->tmp[1] = j;
        return index(this->tmp);
        }

public:

    explicit IndexedComponent_Map(size_t n)
        {
        this->repn = std::make_shared<IndexedComponentRepn<TYPE>>(n);
        this->repn->resize_index_vectors(this->tmp,this->reftmp);
        }

    explicit IndexedComponent_Map(const std::vector<size_t>& _shape)
        {
        this->repn = std::make_shared<IndexedComponentRepn<TYPE>>(_shape);
        this->repn->resize_index_vectors(this->tmp,this->reftmp);
        }

    explicit IndexedComponent_Map(const std::initializer_list<size_t>& _shape)
        {
        this->repn = std::make_shared<IndexedComponentRepn<TYPE>>(_shape);
        this->repn->resize_index_vectors(this->tmp,this->reftmp);
        }

    explicit IndexedComponent_Map(ConcreteSet& arg)
        {
        this->repn = std::make_shared<IndexedComponentRepn<TYPE>>(arg);
        this->repn->resize_index_vectors(this->tmp,this->reftmp);
        }

    ~IndexedComponent_Map() {}

};


template <class TYPE>
TYPE IndexedComponent_Map<TYPE>::index(const IndexVector& args)
{
assert(dim() == args.size());

auto curr = this->repn->index.find(this->tmp);
if (curr == this->repn->index.end()) {
    std::string err = "Unknown index value: "+this->repn->_name+"(";
    for (size_t i=0; i<args.size(); i++) {
        if (i > 0)
            err += ",";
        err += std::to_string(args[i]);
        }
    err += ")";
    throw std::runtime_error(err); 
    }
return this->repn->value[curr->second];
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
