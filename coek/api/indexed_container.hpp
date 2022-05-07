#pragma once

#include <map>
#include <cassert>
#include <string>
#include <vector>
#include <variant>
#include <unordered_map>
#include <memory>
#ifdef COEK_WITH_COMPACT_MODEL
#include "coek/compact/coek_sets.hpp"
#endif
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
    std::map<IndexVector, std::string> names;
    std::map<IndexVector, TYPE> value;
    size_t _dim;
    std::string _name;

public:

    IndexedComponentRepn(size_t dim)
        : _dim(dim)
        { }

    size_t dim()
        {return _dim;}

    size_t size()
        {return value.size();}

    void resize_index_vectors(IndexVector& tmp, std::vector<refarg_types>& reftmp)
        {
        tmp = cache.alloc(dim());
        reftmp.resize(dim());
        }

    virtual bool valid_index(const IndexVector& args) = 0;
};


template <class TYPE>
class IndexedComponent
{
public:

    std::shared_ptr<IndexedComponentRepn<TYPE>> repn;
    IndexVector tmp;
    std::vector<refarg_types> reftmp;

public:

    typename std::map<IndexVector, TYPE>::iterator begin()
        { return repn->value.begin(); }
    typename std::map<IndexVector, TYPE>::iterator end()
        { return repn->value.end(); }

    size_t size()
        { return repn->size(); }
    size_t dim()
        { return repn->dim(); }

    virtual TYPE& index(const IndexVector& args) = 0;

    //Expression create_varref(const std::vector<refarg_types>& indices);

    std::string name()
        { return repn->_name; }
};


template <class TYPE>
class IndexedComponent_Map : public IndexedComponent<TYPE>
{
public:

    TYPE& index(const IndexVector& args);
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
    TYPE& operator()(const ARGTYPES&... args)
        {
        const size_t nargs = count_args(args...);
        if (this->dim() != nargs)
            index_error(nargs);
        collect_args(static_cast<size_t>(0), args...);
        return index(this->tmp);
        }

    TYPE& operator()(int i);

    TYPE& operator()(int i, int j);

public:

    explicit IndexedComponent_Map(size_t n);

    explicit IndexedComponent_Map(const std::vector<size_t>& _shape);

    explicit IndexedComponent_Map(const std::initializer_list<size_t>& _shape);

    #ifdef COEK_WITH_COMPACT_MODEL
    explicit IndexedComponent_Map(ConcreteSet& arg);
    #endif

    ~IndexedComponent_Map() {}

};

}
