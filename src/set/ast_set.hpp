#pragma once

#include <memory>
#include <any>
//#include <variant>
#include <iostream>
#include <vector>
#include <set>
#include <cmath>
#include <assert.h>
#include "coek_expr.hpp"


namespace coek {

//typedef typename std::variant<int, double, std::string> set_types;
typedef int set_types;

template <typename TYPE>
class SetIteratorRepnBase
{
public:

    virtual void next() = 0;
    virtual bool equals(const SetIteratorRepnBase<TYPE>* repn) const = 0;
    virtual bool not_equals(const SetIteratorRepnBase<TYPE>* repn) const = 0;
    virtual TYPE& value() = 0;
    virtual const TYPE& value() const = 0;
};

class BaseSetExpression
{
public:

    bool _finite;
    bool _countable;
    bool initialized;

public:

    BaseSetExpression() 
        : _finite(false), _countable(false), initialized(false) {}

    virtual ~BaseSetExpression() {}

    virtual std::shared_ptr<SetIteratorRepnBase<std::vector<set_types>>> begin_NDiterator() = 0;
    virtual std::shared_ptr<SetIteratorRepnBase<std::vector<set_types>>> begin_NDiterator(const std::vector<IndexParameter>& index) = 0;
    virtual std::shared_ptr<SetIteratorRepnBase<std::vector<set_types>>> end_NDiterator() = 0;

    virtual unsigned int dim() = 0;

    virtual void initialize() = 0;

    bool finite()
        { return _finite; }

    bool countable()
        { return _countable; }

    virtual bool empty()
        {
        if (!initialized)
            throw std::runtime_error("Testing empty() on uninitialized set.");
        return size() == 0;
        }

    virtual size_t size() = 0;

    virtual std::shared_ptr<BaseSetExpression> set_union(const std::shared_ptr<BaseSetExpression>& lhs, const std::shared_ptr<BaseSetExpression>& rhs) const = 0;

    virtual std::shared_ptr<BaseSetExpression> set_intersection(const std::shared_ptr<BaseSetExpression>& lhs, const std::shared_ptr<BaseSetExpression>& rhs) const = 0;

    virtual std::shared_ptr<BaseSetExpression> set_difference(const std::shared_ptr<BaseSetExpression>& lhs, const std::shared_ptr<BaseSetExpression>& rhs) const = 0;

    virtual std::shared_ptr<BaseSetExpression> set_symmetric_difference(const std::shared_ptr<BaseSetExpression>& lhs, const std::shared_ptr<BaseSetExpression>& rhs) const = 0;

    virtual std::shared_ptr<BaseSetExpression> set_product(const std::shared_ptr<BaseSetExpression>& lhs, const std::shared_ptr<BaseSetExpression>& rhs) const = 0;

    virtual bool contains_any(const std::any& arg) = 0;
    virtual std::any value(size_t i) = 0;
    virtual void get_value(size_t i, set_types& arg, IndexParameter& index) = 0;
};

namespace ast {

class SetRef : public BaseSetExpression
{
public:

    std::shared_ptr<BaseSetExpression> indexed_set;

public:

    std::shared_ptr<SetIteratorRepnBase<std::vector<set_types>>> begin_NDiterator()
        { throw std::runtime_error("Cannot create an iterator for a SetRef"); }
    std::shared_ptr<SetIteratorRepnBase<std::vector<set_types>>> begin_NDiterator(const IndexParameter& index)
        { throw std::runtime_error("Cannot create an iterator for a SetRef"); }
    std::shared_ptr<SetIteratorRepnBase<std::vector<set_types>>> end_NDiterator()
        { throw std::runtime_error("Cannot create an iterator for a SetRef"); }

    unsigned int dim()
        { return indexed_set->dim(); }

    void initialize()
        { throw std::runtime_error("Cannot initialize via a SetRef"); }

    size_t size()
        { throw std::runtime_error("Cannot get the size via a SetRef"); }

    std::shared_ptr<BaseSetExpression> set_union(const std::shared_ptr<BaseSetExpression>& lhs, const std::shared_ptr<BaseSetExpression>& rhs) const;

    std::shared_ptr<BaseSetExpression> set_intersection(const std::shared_ptr<BaseSetExpression>& lhs, const std::shared_ptr<BaseSetExpression>& rhs) const;

    std::shared_ptr<BaseSetExpression> set_difference(const std::shared_ptr<BaseSetExpression>& lhs, const std::shared_ptr<BaseSetExpression>& rhs) const;

    std::shared_ptr<BaseSetExpression> set_symmetric_difference(const std::shared_ptr<BaseSetExpression>& lhs, const std::shared_ptr<BaseSetExpression>& rhs) const;

    std::shared_ptr<BaseSetExpression> set_product(const std::shared_ptr<BaseSetExpression>& lhs, const std::shared_ptr<BaseSetExpression>& rhs) const;

    bool contains_any(const std::any& arg)
        { throw std::runtime_error("Cannot call contains_any() via a SetRef"); }
    std::any value(size_t i)
        { throw std::runtime_error("Cannot call value() via a SetRef"); }
    void get_value(size_t i, set_types& arg, IndexParameter& index)
        { throw std::runtime_error("Cannot call get_value() via a SetRef"); }
};

/// -------------------------------------------------------------

template <typename TYPE>
class SimpleSetIterator
{
public:

    std::shared_ptr<SetIteratorRepnBase<TYPE>> repn;

public:

    typedef TYPE& reference;
    typedef const TYPE& const_reference;

    SimpleSetIterator(const std::shared_ptr<SetIteratorRepnBase<TYPE>>& _repn)
        : repn(_repn) {}

    SimpleSetIterator& operator++()
        { repn->next(); return *this; }

    bool operator==(const SimpleSetIterator& other) const
        { return repn->equals(other.repn.get()); }

    bool operator!=(const SimpleSetIterator& other) const
        { return repn->not_equals(other.repn.get()); }

    reference operator*()
        { return repn->value(); }

    const_reference operator*() const
        { return repn->value(); }
};

template <typename TYPE>
class SimpleSetUnion;

template <typename TYPE>
class SimpleSetIntersection;

template <typename TYPE>
class SimpleSetDifference;

template <typename TYPE>
class SimpleSetSymmetricDifference;

class ProductSet;

/*
class BaseSimpleSet : public BaseSetExpression
{
public:

    virtual bool contains(const TYPE& arg) const = 0;

    unsigned int dim()
        { return 1; }

};
*/

template <typename TYPE>
class SimpleSet : public BaseSetExpression
{
public:

    virtual SimpleSetIterator<TYPE> begin() = 0;

    virtual SimpleSetIterator<TYPE> end() = 0;

    virtual bool contains(const TYPE& arg) const = 0;

    virtual bool contains_any(const std::any& arg)
        {
        TYPE val = std::any_cast<TYPE>(arg);
        return contains(val);
        }

    unsigned int dim()
        { return 1; }

    virtual void add_unique(const TYPE& arg) = 0;

    std::shared_ptr<BaseSetExpression> set_union(const std::shared_ptr<BaseSetExpression>& lhs, const std::shared_ptr<BaseSetExpression>& rhs) const
        {
        if (rhs->dim() > 1)
            throw std::runtime_error("Cannot take the union of sets with different dimensions.");
        auto tmp = std::make_shared<SimpleSetUnion<TYPE>>(
                        std::dynamic_pointer_cast<SimpleSet<TYPE>>(lhs),
                        std::dynamic_pointer_cast<SimpleSet<TYPE>>(rhs) );
        return tmp;
        }

    std::shared_ptr<BaseSetExpression> set_intersection(const std::shared_ptr<BaseSetExpression>& lhs, const std::shared_ptr<BaseSetExpression>& rhs) const
        {
        if (rhs->dim() > 1)
            throw std::runtime_error("Cannot take the intersection of sets with different dimensions.");
        auto tmp = std::make_shared<SimpleSetIntersection<TYPE>>(
                        std::dynamic_pointer_cast<SimpleSet<TYPE>>(lhs),
                        std::dynamic_pointer_cast<SimpleSet<TYPE>>(rhs) );
        return tmp;
        }

    std::shared_ptr<BaseSetExpression> set_difference(const std::shared_ptr<BaseSetExpression>& lhs, const std::shared_ptr<BaseSetExpression>& rhs) const
        {
        if (rhs->dim() > 1)
            throw std::runtime_error("Cannot take the difference of sets with different dimensions.");
        auto tmp = std::make_shared<SimpleSetDifference<TYPE>>(
                        std::dynamic_pointer_cast<SimpleSet<TYPE>>(lhs),
                        std::dynamic_pointer_cast<SimpleSet<TYPE>>(rhs) );
        return tmp;
        }

    std::shared_ptr<BaseSetExpression> set_symmetric_difference(const std::shared_ptr<BaseSetExpression>& lhs, const std::shared_ptr<BaseSetExpression>& rhs) const
        {
        if (rhs->dim() > 1)
            throw std::runtime_error("Cannot take the symmetric difference of sets with different dimensions.");
        auto tmp = std::make_shared<SimpleSetSymmetricDifference<TYPE>>(
                        std::dynamic_pointer_cast<SimpleSet<TYPE>>(lhs),
                        std::dynamic_pointer_cast<SimpleSet<TYPE>>(rhs) );
        return tmp;
        }

    std::shared_ptr<BaseSetExpression> set_product(const std::shared_ptr<BaseSetExpression>& lhs, const std::shared_ptr<BaseSetExpression>& rhs) const
        {
        if (rhs->dim() == 1) {
            auto tmp = std::make_shared<ProductSet>(lhs, rhs);
            return tmp;
            }
        else {
            auto tmp = std::make_shared<ProductSet>(lhs, std::dynamic_pointer_cast<ProductSet>(rhs) );
            return tmp;
            }
        }
};

/// -------------------------------------------------------------

template <typename ITTYPE>
inline void set_index_value(IndexParameter& index, const ITTYPE& val)
{ index.set_value(val); }

template <>
inline void set_index_value(IndexParameter& index, const set_types& val)
{
index.set_value(val);
#if 0
if (auto ival = std::get_if<int>(&val))
    index.set_value(*ival);
else if (auto sval = std::get_if<std::string>(&val))
    index.set_value(*sval);
else if (auto dval = std::get_if<double>(&val))
    index.set_value(*dval);
#endif
}


template <typename TYPE>
class FiniteSetIteratorRepn : public SetIteratorRepnBase<TYPE>
{
public:

    IndexParameter index;
    typename std::vector<TYPE>::iterator iterator;
    typename std::vector<TYPE>::iterator end_iterator;

public:

    FiniteSetIteratorRepn(const typename std::vector<TYPE>::iterator& _iterator, const typename std::vector<TYPE>::iterator& _end_iterator, const IndexParameter& _index)
        : index(_index), iterator(_iterator), end_iterator(_end_iterator)
        {
        set_index_value(index, *iterator);
        }

    FiniteSetIteratorRepn(const typename std::vector<TYPE>::iterator& _iterator, const typename std::vector<TYPE>::iterator& _end_iterator)
        : iterator(_iterator), end_iterator(_end_iterator)
        { }

    void next()
        {
        iterator++;
        if (iterator != end_iterator)
            set_index_value(index, *iterator);
        }

    bool equals(const SetIteratorRepnBase<TYPE>* repn) const
        {
        auto _repn = dynamic_cast<const FiniteSetIteratorRepn<TYPE>*>(repn);
        return (_repn->iterator == iterator);
        }

    bool not_equals(const SetIteratorRepnBase<TYPE>* repn) const
        {
        auto _repn = dynamic_cast<const FiniteSetIteratorRepn<TYPE>*>(repn);
        return (_repn->iterator != iterator);
        }

    TYPE& value()
        { return *iterator; }

    const TYPE& value() const
        { return *iterator; }
};


template <typename TYPE>
class FiniteNDSetIteratorRepn : public SetIteratorRepnBase<std::vector<set_types>>
{
public:

    IndexParameter index;
    typename std::vector<TYPE>::iterator iterator;
    typename std::vector<TYPE>::iterator end;
    std::vector<set_types> vec;

public:

    FiniteNDSetIteratorRepn(const typename std::vector<TYPE>::iterator& _iterator, const typename std::vector<TYPE>::iterator& _end, const IndexParameter& _index)
        : index(_index), iterator(_iterator), end(_end)
        {
        vec.resize(1);
        set_index_value(index, *iterator);
        }

    FiniteNDSetIteratorRepn(const typename std::vector<TYPE>::iterator& _iterator, const typename std::vector<TYPE>::iterator& _end)
        : iterator(_iterator), end(_end)
        {
        vec.resize(1);
        set_index_value(index, *iterator);
        }

    FiniteNDSetIteratorRepn(const typename std::vector<TYPE>::iterator& _iterator)
        : iterator(_iterator), end(_iterator)
        {
        vec.resize(1);
        }

    void next()
        {
        iterator++;
        if (iterator != end)
            set_index_value(index, *iterator);
        }

    bool equals(const SetIteratorRepnBase<std::vector<set_types>>* repn) const
        {
        auto _repn = dynamic_cast<const FiniteNDSetIteratorRepn<TYPE>*>(repn);
        return (_repn->iterator == iterator);
        }

    bool not_equals(const SetIteratorRepnBase<std::vector<set_types>>* repn) const
        {
        auto _repn = dynamic_cast<const FiniteNDSetIteratorRepn<TYPE>*>(repn);
        return (_repn->iterator != iterator);
        }

    std::vector<set_types>& value()
        {
        vec[0] = *iterator;
        return vec;
        }

    const std::vector<set_types>& value() const
        {
        throw std::runtime_error("Cannot return const values.");
        return vec;
        }
};


template <typename TYPE>
class FiniteSimpleSet : public SimpleSet<TYPE>
{
public:

    std::vector<TYPE> data;
    std::set<TYPE> set_data;

public:

    FiniteSimpleSet()
        { this->_finite = true; this->_countable=true; }

    void initialize()
        {
        this->initialized = true;
        return;
        }

    SimpleSetIterator<TYPE> begin()
        {
        auto tmp = std::make_shared<FiniteSetIteratorRepn<TYPE>>(data.begin(), data.end());
        return SimpleSetIterator<TYPE>(tmp);
        }

    SimpleSetIterator<TYPE> end()
        {
        auto tmp = std::make_shared<FiniteSetIteratorRepn<TYPE>>(data.end(), data.end());
        return SimpleSetIterator<TYPE>(tmp);
        }

    std::shared_ptr<SetIteratorRepnBase<std::vector<set_types>>> begin_NDiterator()
        {
        auto tmp = std::make_shared<FiniteNDSetIteratorRepn<TYPE>>(data.begin(), data.end());
        return tmp;
        }

    std::shared_ptr<SetIteratorRepnBase<std::vector<set_types>>> begin_NDiterator(const std::vector<IndexParameter>& indices)
        {
        auto tmp = std::make_shared<FiniteNDSetIteratorRepn<TYPE>>(data.begin(), data.end(), indices[0]);
        return tmp;
        }
    std::shared_ptr<SetIteratorRepnBase<std::vector<set_types>>> end_NDiterator()
        {
        auto tmp = std::make_shared<FiniteNDSetIteratorRepn<TYPE>>(data.end());
        return tmp;
        }

    bool contains(const TYPE& arg) const
        { return !(set_data.find(arg) == set_data.end()); }

    size_t size()
        { return data.size(); }

    void add_unique(const TYPE& arg)
        { data.push_back(arg); set_data.insert(arg); }

    std::any value(size_t i)
        {
        if (not this->initialized)
            throw std::runtime_error("Getting value() of uninitialized set.");
        if (i >= data.size())
            throw std::runtime_error(std::string("Requested set index that is too large: i=") + std::to_string(i) + std::string(" size=") + std::to_string(data.size()));
#if 0
        // BAD - Don't set the index parameter here
        if (this->index.repn)
            this->index.repn->value = data[i];
#endif
        return data[i];
        }

    void get_value(size_t i, set_types& arg, IndexParameter& index)
        {
        if (not this->initialized)
            throw std::runtime_error("Getting value() of uninitialized set.");
        if (i >= data.size())
            throw std::runtime_error(std::string("Requested set index that is too large: i=") + std::to_string(i) + std::string(" size=") + std::to_string(data.size()));
        arg = data[i];
        if (index.repn)
            index.repn->value = data[i];
        }
};

template <typename TYPE>
class SimpleSetUnion : public FiniteSimpleSet<TYPE>
{
public:

    std::shared_ptr<SimpleSet<TYPE>> lhs;
    std::shared_ptr<SimpleSet<TYPE>> rhs;

public:

    SimpleSetUnion(const std::shared_ptr<SimpleSet<TYPE>>& _lhs, const std::shared_ptr<SimpleSet<TYPE>>& _rhs)
        : lhs(_lhs), rhs(_rhs)
        {
        this->_finite = lhs->_finite and rhs->_finite;
        this->_countable = lhs->_countable and rhs->_countable;
        }

    void initialize()
        {
        if (this->initialized) return;

        lhs->initialize();
        for (auto it=lhs->begin(); it != lhs->end(); ++it)
            this->add_unique(*it);

        rhs->initialize();
        for (auto it=rhs->begin(); it != rhs->end(); ++it) {
            if (not this->contains(*it))
                this->add_unique(*it);
            }

        this->initialized = true;
        assert(this->data.size() == this->set_data.size());
        }
};

template <typename TYPE>
class SimpleSetIntersection : public FiniteSimpleSet<TYPE>
{
public:

    std::shared_ptr<SimpleSet<TYPE>> lhs;
    std::shared_ptr<SimpleSet<TYPE>> rhs;

public:

    SimpleSetIntersection(const std::shared_ptr<SimpleSet<TYPE>>& _lhs, const std::shared_ptr<SimpleSet<TYPE>>& _rhs)
        : lhs(_lhs), rhs(_rhs)
        {
        this->_finite = lhs->_finite or rhs->_finite;
        this->_countable = lhs->_countable or rhs->_countable;
        }

    void initialize()
        {
        if (this->initialized) return;

        lhs->initialize();
        rhs->initialize();
        if (lhs->size() <= rhs->size()) {
            for (auto it=lhs->begin(); it != lhs->end(); ++it) {
                if (rhs->contains(*it))
                    this->add_unique(*it);
                }
            }
        else {
            for (auto it=rhs->begin(); it != rhs->end(); ++it) {
                if (lhs->contains(*it))
                    this->add_unique(*it);
                }
            }

        this->initialized = true;
        assert(this->data.size() == this->set_data.size());
        }

};

template <typename TYPE>
class SimpleSetDifference : public FiniteSimpleSet<TYPE>
{
public:

    std::shared_ptr<SimpleSet<TYPE>> lhs;
    std::shared_ptr<SimpleSet<TYPE>> rhs;

public:

    SimpleSetDifference(const std::shared_ptr<SimpleSet<TYPE>>& _lhs, const std::shared_ptr<SimpleSet<TYPE>>& _rhs)
        : lhs(_lhs), rhs(_rhs)
        {
        this->_finite = lhs->_finite;
        this->_countable = lhs->_countable;
        }

    void initialize()
        {
        if (this->initialized) return;

        lhs->initialize();
        rhs->initialize();

        for (auto it=lhs->begin(); it != lhs->end(); ++it) {
            if (not rhs->contains(*it))
                this->add_unique(*it);
            }

        this->initialized = true;
        assert(this->data.size() == this->set_data.size());
        }
};

template <typename TYPE>
class SimpleSetSymmetricDifference : public FiniteSimpleSet<TYPE>
{
public:

    std::shared_ptr<SimpleSet<TYPE>> lhs;
    std::shared_ptr<SimpleSet<TYPE>> rhs;

public:

    SimpleSetSymmetricDifference(const std::shared_ptr<SimpleSet<TYPE>>& _lhs, const std::shared_ptr<SimpleSet<TYPE>>& _rhs)
        : lhs(_lhs), rhs(_rhs)
        {
        this->_finite = lhs->_finite and rhs->_finite;
        this->_countable = lhs->_countable and rhs->_countable;
        }

    void initialize()
        {
        if (this->initialized) return;

        lhs->initialize();
        rhs->initialize();

        //std::cout << "SYMDIFF" << std::endl;
        for (auto it=lhs->begin(); it != lhs->end(); ++it) {
            //TYPE tmp = *it;
            //std::cout << "RHS " << tmp << " " << rhs->contains(tmp) << std::endl;
            if (not rhs->contains(*it))
                this->add_unique(*it);
            }
        for (auto it=rhs->begin(); it != rhs->end(); ++it) {
            //TYPE tmp = *it;
            //std::cout << "LHS " << tmp << " " << lhs->contains(tmp) << std::endl;
            if (not lhs->contains(*it))
                this->add_unique(*it);
            }

        this->initialized = true;
        assert(this->data.size() == this->set_data.size());
        }
};

/// -------------------------------------------------------------

template <typename TYPE>
class ListSet : public FiniteSimpleSet<TYPE>
{
public:

    const std::initializer_list<TYPE>& raw;

public:
    
    ListSet(const std::initializer_list<TYPE>& _raw)
        : FiniteSimpleSet<TYPE>(), raw(_raw)
        { initialize(); }

    void initialize()
        {
        if (this->initialized) return;
        for (auto it=raw.begin(); it != raw.end(); ++it)
            this->add_unique(*it);
        this->initialized = true;
        }
};

template <typename TYPE>
class SetSet : public FiniteSimpleSet<TYPE>
{
public:

    const std::set<TYPE>& raw;

public:
    
    SetSet(const std::set<TYPE>& _raw)
        : FiniteSimpleSet<TYPE>(), raw(_raw)
        { initialize(); }

    void initialize()
        {
        if (this->initialized) return;
        for (auto it=raw.begin(); it != raw.end(); ++it)
            this->add_unique(*it);
        this->initialized = true;
        }
};

template <typename TYPE>
class VectorSet : public FiniteSimpleSet<TYPE>
{
public:

    const std::vector<TYPE>& raw;

public:
    
    VectorSet(const std::vector<TYPE>& _raw) 
        : FiniteSimpleSet<TYPE>(), raw(_raw)
        { initialize(); }

    void initialize()
        {
        if (this->initialized) return;
        for (auto it=raw.begin(); it != raw.end(); ++it) {
            if (!this->contains(*it))
                this->add_unique(*it);
            }
        this->initialized = true;
        }
};

/// -------------------------------------------------------------

template <typename TYPE>
class RangeSetIteratorRepn : public SetIteratorRepnBase<TYPE>
{
public:

    IndexParameter index;
    TYPE start;
    TYPE step;
    size_t count;
    TYPE _value;

public:

    RangeSetIteratorRepn(TYPE _start, TYPE _step, size_t _count, const IndexParameter& _index)
        : index(_index), start(_start), step(_step), count(_count)
        {
        _value = start;
        index.repn->value = _value;
        }

    RangeSetIteratorRepn(TYPE _start, TYPE _step, size_t _count)
        : start(_start), step(_step), count(_count)
        {
        _value = start;
        }

    void next()
        {
        _value += step;
        if (index.repn)
            index.repn->value = _value;
        count++;
        }

    bool equals(const SetIteratorRepnBase<TYPE>* repn) const
        {
        auto _repn = dynamic_cast<const RangeSetIteratorRepn<TYPE>*>(repn);
        return (_repn->count == count);
        }

    bool not_equals(const SetIteratorRepnBase<TYPE>* repn) const
        {
        auto _repn = dynamic_cast<const RangeSetIteratorRepn<TYPE>*>(repn);
        return (_repn->count != count);
        }

    TYPE& value()
        { return _value; }

    const TYPE& value() const
        { return _value; }
};

template <typename TYPE>
class RangeNDSetIteratorRepn : public SetIteratorRepnBase<std::vector<set_types>>
{
public:

    IndexParameter index;
    TYPE start;
    TYPE step;
    size_t count;
    TYPE _value;
    std::vector<set_types> vec;

public:

    RangeNDSetIteratorRepn(TYPE _start, TYPE _step, size_t _count, const IndexParameter& _index)
        : index(_index), start(_start), step(_step), count(_count)
        {
        _value = start;
        vec.resize(1);
        index.repn->value = _value;
        }

    RangeNDSetIteratorRepn(TYPE _start, TYPE _step, size_t _count)
        : start(_start), step(_step), count(_count)
        {
        _value = start;
        vec.resize(1);
        }

    void next()
        {
        _value += step;
        if (index.repn)
            index.repn->value = _value;
        count++;
        }

    bool equals(const SetIteratorRepnBase<std::vector<set_types>>* repn) const
        {
        auto _repn = dynamic_cast<const RangeNDSetIteratorRepn<TYPE>*>(repn);
        return (_repn->count == count);
        }

    bool not_equals(const SetIteratorRepnBase<std::vector<set_types>>* repn) const
        {
        auto _repn = dynamic_cast<const RangeNDSetIteratorRepn<TYPE>*>(repn);
        return (_repn->count != count);
        }

    std::vector<set_types>& value()
        {
        vec[0] = _value;
        return vec;
        }

    const std::vector<set_types>& value() const
        {
        throw std::runtime_error("Cannot return const values.");
        return vec;
        }
};

template <typename TYPE>
class RangeSet : public SimpleSet<TYPE>
{
public:

    TYPE start;
    TYPE stop;
    TYPE step;
    TYPE tolerance;
    size_t nelements;

public:

    RangeSet(TYPE _start, TYPE _stop, TYPE _step=1)
        : SimpleSet<TYPE>(), start(_start), stop(_stop), step(_step), tolerance(1e-7),
          nelements(0)
        {
        this->_finite = true;
        this->_countable = true;
        nelements = std::rint((stop-start)/step);
        if ((nelements == 0) or (fabs(start+step*nelements-stop) <= tolerance))
            nelements++;
        this->initialized = true;
        }

    SimpleSetIterator<TYPE> begin()
        {
        auto tmp = std::make_shared<RangeSetIteratorRepn<TYPE>>(start, step, 0);
        return SimpleSetIterator<TYPE>(tmp);
        }

    SimpleSetIterator<TYPE> end()
        { return SimpleSetIterator<TYPE>(std::make_shared<RangeSetIteratorRepn<TYPE>>(start, step, nelements)); }

    std::shared_ptr<SetIteratorRepnBase<std::vector<set_types>>> begin_NDiterator()
        {
        auto tmp = std::make_shared<RangeNDSetIteratorRepn<TYPE>>(start, step, 0);
        return tmp;
        }
    std::shared_ptr<SetIteratorRepnBase<std::vector<set_types>>> begin_NDiterator(const std::vector<IndexParameter>& index)
        {
        auto tmp = std::make_shared<RangeNDSetIteratorRepn<TYPE>>(start, step, 0, index[0]);
        return tmp;
        }
    std::shared_ptr<SetIteratorRepnBase<std::vector<set_types>>> end_NDiterator()
        {
        auto tmp = std::make_shared<RangeNDSetIteratorRepn<TYPE>>(start, step, nelements);
        return tmp;
        }

    bool contains(const TYPE& arg) const
        {
        int guess = std::rint((arg-start)/step);
        return (fabs(arg - (start+step*guess)) <= tolerance) and (start <= (arg+tolerance)) and (arg <= (stop+tolerance));
        }

    void initialize()
        { }

    size_t size()
        { return nelements; }

    void add_unique(const TYPE&)
        { throw std::runtime_error(std::string("Cannot add elements to a range set")); }

    std::any value(size_t i)
        {
        if (i >= nelements)
            throw std::runtime_error(std::string("Requested set index that is too large: i=") + std::to_string(i) + std::string(" size=") + std::to_string(nelements));
        TYPE ans = start+step*i;
        return ans;
        }

    void get_value(size_t i, set_types& arg, IndexParameter& index)
        {
        if (i >= nelements)
            throw std::runtime_error(std::string("Requested set index that is too large: i=") + std::to_string(i) + std::string(" size=") + std::to_string(nelements));
        TYPE ans = start+step*i;
        arg = ans;
        if (index.repn)
            index.repn->value = ans;
        }
};

/// -------------------------------------------------------------

/*
class ProductSetIteratorRepn
{
public:

    virtual void next() = 0;
    virtual bool equals(const ProductSetIteratorRepn* repn) const = 0;
    virtual bool not_equals(const ProductSetIteratorRepn* repn) const = 0;
    virtual std::vector<set_types>& value() = 0;
    virtual const std::vector<set_types>& value() const = 0;
};
*/

class ProductSetIterator
{
public:

    typedef SetIteratorRepnBase<std::vector<set_types>> ProductSetIteratorRepn;
    std::shared_ptr<ProductSetIteratorRepn> repn;

public:

    typedef std::vector<set_types>& reference;
    typedef const std::vector<set_types>& const_reference;

    ProductSetIterator(const std::shared_ptr<ProductSetIteratorRepn>& _repn)
        : repn(_repn) {}

    ProductSetIterator& operator++()
        { repn->next(); return *this; }

    bool operator==(const ProductSetIterator& other) const
        { return repn->equals(other.repn.get()); }

    bool operator!=(const ProductSetIterator& other) const
        { return repn->not_equals(other.repn.get()); }

    reference operator*()
        { return repn->value(); }

    const_reference operator*() const
        { return repn->value(); }
};


class ProductSet : public BaseSetExpression
{
public:

    unsigned int _dim;
    std::vector<std::shared_ptr<BaseSetExpression>> simple_sets;

public:

    ProductSet()
        : BaseSetExpression()
        { }

    ProductSet(const std::shared_ptr<BaseSetExpression>& _lhs, const std::shared_ptr<BaseSetExpression>& _rhs);
    ProductSet(const std::shared_ptr<ProductSet>& _lhs, const std::shared_ptr<BaseSetExpression>& _rhs);
    ProductSet(const std::shared_ptr<BaseSetExpression>& _lhs, const std::shared_ptr<ProductSet>& _rhs);
    ProductSet(const std::shared_ptr<ProductSet>& _lhs, const std::shared_ptr<ProductSet>& _rhs);

    virtual ProductSetIterator begin();
    virtual ProductSetIterator begin(const std::vector<IndexParameter>& index);
    virtual ProductSetIterator end();

    std::shared_ptr<SetIteratorRepnBase<std::vector<set_types>>> begin_NDiterator();
    std::shared_ptr<SetIteratorRepnBase<std::vector<set_types>>> begin_NDiterator(const std::vector<IndexParameter>& index);
    std::shared_ptr<SetIteratorRepnBase<std::vector<set_types>>> end_NDiterator();

    virtual void add_unique(const std::vector<set_types>& arg)
        { throw std::runtime_error("Cannot add a value to a simple product set."); }

    unsigned int dim()
        { return _dim; }

    void initialize();

    size_t size();

    std::shared_ptr<BaseSetExpression> set_union(const std::shared_ptr<BaseSetExpression>& lhs, const std::shared_ptr<BaseSetExpression>& rhs) const;

    std::shared_ptr<BaseSetExpression> set_intersection(const std::shared_ptr<BaseSetExpression>& lhs, const std::shared_ptr<BaseSetExpression>& rhs) const;

    std::shared_ptr<BaseSetExpression> set_difference(const std::shared_ptr<BaseSetExpression>& lhs, const std::shared_ptr<BaseSetExpression>& rhs) const;

    std::shared_ptr<BaseSetExpression> set_symmetric_difference(const std::shared_ptr<BaseSetExpression>& lhs, const std::shared_ptr<BaseSetExpression>& rhs) const;

    std::shared_ptr<BaseSetExpression> set_product(const std::shared_ptr<BaseSetExpression>& lhs, const std::shared_ptr<BaseSetExpression>& rhs) const;

    bool contains(const std::vector<set_types>& arg);

    bool contains_any(const std::any& arg);

    std::any value(size_t i);

    void get_value(size_t i, set_types& arg, IndexParameter& index);
};

class FiniteProductSet : public ProductSet
{
public:

    std::vector<std::vector<set_types>> data;
    std::set<std::vector<set_types>> set_data;

public:

    FiniteProductSet()
        { this->_finite = true; this->_countable=true; }

    ProductSetIterator begin();

    ProductSetIterator end();

    bool contains(const std::vector<set_types>& arg) const;

    size_t size()
        { return data.size(); }

    void add_unique(const std::vector<set_types>& arg);

    std::any value(size_t i);
};

class ProductSetUnion : public FiniteProductSet
{
public:

    std::shared_ptr<ProductSet> lhs;
    std::shared_ptr<ProductSet> rhs;

public:

    ProductSetUnion(const std::shared_ptr<ProductSet>& _lhs, const std::shared_ptr<ProductSet>& _rhs);

    void initialize();
};

class ProductSetIntersection : public FiniteProductSet
{
public:

    std::shared_ptr<ProductSet> lhs;
    std::shared_ptr<ProductSet> rhs;

public:

    ProductSetIntersection(const std::shared_ptr<ProductSet>& _lhs, const std::shared_ptr<ProductSet>& _rhs);

    void initialize();
};

class ProductSetDifference : public FiniteProductSet
{
public:

    std::shared_ptr<ProductSet> lhs;
    std::shared_ptr<ProductSet> rhs;

public:

    ProductSetDifference(const std::shared_ptr<ProductSet>& _lhs, const std::shared_ptr<ProductSet>& _rhs);

    void initialize();
};

class ProductSetSymmetricDifference : public FiniteProductSet
{
public:

    std::shared_ptr<ProductSet> lhs;
    std::shared_ptr<ProductSet> rhs;

public:

    ProductSetSymmetricDifference(const std::shared_ptr<ProductSet>& _lhs, const std::shared_ptr<ProductSet>& _rhs);

    void initialize();
};


}

}
