#pragma once

#include <cassert>
#include <string>
#include <vector>
#include <variant>
#include "coek/api/expression.hpp"


namespace coek {

class BaseExpressionTerm;
class IndexVectorCache;

typedef BaseExpressionTerm* expr_pointer_t;
typedef std::variant<int,expr_pointer_t> refarg_types;


class IndexVector
{
public:

    typedef int vecdata_t;
    size_t len;
    vecdata_t* data;

public:

    IndexVector()
        : len(0), data(0)
        {}

    explicit IndexVector(vecdata_t* _data)
        : len(static_cast<size_t>(_data[0])), data(_data+1)
        {}

    explicit IndexVector(vecdata_t* _data, size_t _len)
        : len(_len), data(_data)
        {}

    IndexVector(const IndexVector& other)
        : len(other.len), data(other.data)
        {}

    IndexVector& operator=(const IndexVector& other)
        {len = other.len; data=other.data; return *this;}

    size_t size() const
        { return len; }

    vecdata_t& operator[](size_t i)
        { return data[i]; }

    const vecdata_t& operator[](size_t i) const
        { return data[i]; }

    bool operator<(const IndexVector& other) const
        {
        for (size_t i=0; i<len; i++) {
            if (data[i] < other.data[i])
                return true;
            else if (data[i] > other.data[i])
                return false;
            }
        return true;
        }

    bool operator==(const IndexVector& other) const
        {
        if (len == 1)
            return data[0] == other.data[0];
        else if (len == 2) {
            return (data[0] == other.data[0]) and (data[1] == other.data[1]);
            }
        vecdata_t* curr = data;
        vecdata_t* end  = data+len;
        vecdata_t* _other = other.data;

        for (; curr != end; ++curr, ++_other) {
            if (*curr != *_other)
                return false;
            }
        return true;
        }
};


class IndexVectorCache
{
public:

    typedef typename IndexVector::vecdata_t vecdata_t;
    size_t len;
    vecdata_t* data;
    size_t curr;

public:

    IndexVectorCache()
        : len(0), data(0), curr(0)
        {}
    IndexVectorCache(size_t _len)
        : len(_len), curr(0)
        { data = new vecdata_t[len]; }

    IndexVectorCache(const IndexVectorCache& other) = delete;

    ~IndexVectorCache()
        { if (len) delete [] data; }

    void resize(size_t _len)
        {
        assert(len == 0);
        len = _len;
        data = new vecdata_t[len];
        }

    IndexVector alloc(size_t _len)
        {
        assert((curr+_len+1) <= len);
        auto tmp = data+curr;
        *tmp = static_cast<vecdata_t>(_len);
        curr += _len+1;
        return IndexVector(tmp);
        }
};


class VariableAssocArrayRepn
{
public:

    IndexVectorCache cache;
    std::vector<std::string> names;
    std::vector<Variable> values;
    Variable variable_template;
    bool call_setup;

public:

    VariableAssocArrayRepn();

    virtual void setup();

    virtual size_t dim() = 0;
    virtual size_t size() = 0;

    void resize_index_vectors(IndexVector& tmp, std::vector<refarg_types>& reftmp);

    std::string get_name(size_t index);

    /** Set the initial variable value. \returns the variable object. */
    void value(double value);
    /** Set the initial variable value. \returns the variable object. */
    void value(const Expression& value);

    /** Set the lower bound. \returns the variable object. */
    void lower(double value);
    /** Set the lower bound. \returns the variable object. */
    void lower(const Expression& value);

    /** Set the upper bound. \returns the variable object. */
    void upper(double value);
    /** Set the upper bound. \returns the variable object. */
    void upper(const Expression& value);

    /** Set the upper and lower bounds. \returns the variable object. */
    void bounds(double lb, double ub);
    /** Set the upper and lower bounds. \returns the variable object. */
    void bounds(const Expression& lb, double ub);
    /** Set the upper and lower bounds. \returns the variable object. */
    void bounds(double lb, const Expression& ub);
    /** Set the upper and lower bounds. \returns the variable object. */
    void bounds(const Expression& lb, const Expression& ub);

    /** Set the name of the variable. \returns the variable object */
    void name(const std::string& name);

    /** Set the variable type. \returns the variable object */
    void within(VariableTypes vtype);
};

}
