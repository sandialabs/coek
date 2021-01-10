#ifndef __coek_sets_hpp
#define __coek_sets_hpp

#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <set>
#include <any>
#include <string>
#include <map>
#include "coek/api/expression.hpp"


namespace coek {

class SetIteratorRepn;
class BaseSetExpression;
typedef std::shared_ptr<BaseSetExpression> SetRepn;
class AbstractSet;
class ConcreteSet;

//typedef typename std::variant<int, double, std::string> set_types;
typedef int set_types;

std::ostream& operator<<(std::ostream& ostr, const ConcreteSet& arg);


class SetIterator
{
public:

    std::shared_ptr<SetIteratorRepn> repn;

    typedef std::vector<set_types>& reference;
    typedef const std::vector<set_types>& const_reference;

public:

    SetIterator();
    SetIterator(const std::shared_ptr<SetIteratorRepn>& _repn);

    SetIterator& operator++();

    bool operator==(const SetIterator& other) const;
    bool operator!=(const SetIterator& other) const;

    reference operator*();
    const_reference operator*() const;
};

class SetBase
{
public:

    SetRepn repn;

public:

    SetBase();
    SetBase(const SetRepn& repn);
    virtual ~SetBase();
};

class SetCore  : public SetBase
{
public:

    SetCore();
    SetCore(const SetRepn& repn);
    virtual ~SetCore();

    unsigned int dim();
    ConcreteSet initialize();
};

class AbstractSet : public SetCore
{
public:

    AbstractSet(unsigned int);
    AbstractSet(const SetRepn& repn);
    virtual ~AbstractSet();

    Expression index(const std::string& name);

    AbstractSet set_union(const AbstractSet& arg) const;
    AbstractSet set_intersection(const AbstractSet& arg) const;
    AbstractSet set_difference(const AbstractSet& arg) const;
    AbstractSet set_symmetric_difference(const AbstractSet& arg) const;
    AbstractSet set_product(const AbstractSet& arg);

    AbstractSet set_union(const ConcreteSet& arg) const;
    AbstractSet set_intersection(const ConcreteSet& arg) const;
    AbstractSet set_difference(const ConcreteSet& arg) const;
    AbstractSet set_symmetric_difference(const ConcreteSet& arg) const;
    AbstractSet set_product(const ConcreteSet& arg);

    AbstractSet operator+(const AbstractSet& arg);
    AbstractSet operator|(const AbstractSet& arg);
    AbstractSet operator&(const AbstractSet& arg);
    AbstractSet operator-(const AbstractSet& arg);
    AbstractSet operator^(const AbstractSet& arg);
    AbstractSet operator*(const AbstractSet& arg);

    AbstractSet operator+(const ConcreteSet& arg);
    AbstractSet operator|(const ConcreteSet& arg);
    AbstractSet operator&(const ConcreteSet& arg);
    AbstractSet operator-(const ConcreteSet& arg);
    AbstractSet operator^(const ConcreteSet& arg);
    AbstractSet operator*(const ConcreteSet& arg);
};

class ConcreteSet : public SetCore
{
public:

    ConcreteSet();
    ConcreteSet(const SetRepn& repn);
    ConcreteSet(const ConcreteSet& repn);
    virtual ~ConcreteSet();

    bool finite();
    bool countable();
    bool empty();
    size_t size();

    bool is_disjoint(const ConcreteSet& arg) const;
    bool is_subset(const ConcreteSet& arg) const;
    bool is_superset(const ConcreteSet& arg) const;

    template <typename TYPE>
    bool contains(const TYPE& arg)
        {
        std::any tmp = arg;
        return contains(tmp);
        }
    bool contains(const std::any& arg);

    std::any operator[](size_t i);
    std::any value(size_t i);

    template <typename TYPE>
    void value(size_t i, TYPE& arg)
        {
        if (not countable())
            throw std::runtime_error("Cannot get the i-th element of an uncountable set.");
        auto ans = value(i);
        arg = std::any_cast<TYPE>(ans);
        }
    
    SetIterator begin();
    SetIterator begin(const std::initializer_list<IndexParameter>& indices);
    SetIterator begin(const std::vector<IndexParameter>& indices);
    SetIterator end();

    AbstractSet set_union(const AbstractSet& arg) const;
    AbstractSet set_intersection(const AbstractSet& arg) const;
    AbstractSet set_difference(const AbstractSet& arg) const;
    AbstractSet set_symmetric_difference(const AbstractSet& arg) const;
    AbstractSet set_product(const AbstractSet& arg);

    ConcreteSet set_union(const ConcreteSet& arg) const;
    ConcreteSet set_intersection(const ConcreteSet& arg) const;
    ConcreteSet set_difference(const ConcreteSet& arg) const;
    ConcreteSet set_symmetric_difference(const ConcreteSet& arg) const;
    ConcreteSet set_product(const ConcreteSet& arg);

    AbstractSet operator+(const AbstractSet& arg);
    AbstractSet operator|(const AbstractSet& arg);
    AbstractSet operator&(const AbstractSet& arg);
    AbstractSet operator-(const AbstractSet& arg);
    AbstractSet operator^(const AbstractSet& arg);
    AbstractSet operator*(const AbstractSet& arg);

    ConcreteSet operator+(const ConcreteSet& arg);
    ConcreteSet operator|(const ConcreteSet& arg);
    ConcreteSet operator&(const ConcreteSet& arg);
    ConcreteSet operator-(const ConcreteSet& arg);
    ConcreteSet operator^(const ConcreteSet& arg);
    ConcreteSet operator*(const ConcreteSet& arg);

    friend std::ostream& operator<<(std::ostream& ostr, const ConcreteSet& arg);
};

//
// Inlined methods
//
inline AbstractSet AbstractSet::operator+(const AbstractSet& arg)
{ return this->set_union(arg); }
inline AbstractSet AbstractSet::operator|(const AbstractSet& arg)
{ return this->set_union(arg); }
inline AbstractSet AbstractSet::operator&(const AbstractSet& arg)
{ return this->set_intersection(arg); }
inline AbstractSet AbstractSet::operator-(const AbstractSet& arg)
{ return this->set_difference(arg); }
inline AbstractSet AbstractSet::operator^(const AbstractSet& arg)
{ return this->set_symmetric_difference(arg); }
inline AbstractSet AbstractSet::operator*(const AbstractSet& arg)
{ return this->set_product(arg); }

inline AbstractSet AbstractSet::operator+(const ConcreteSet& arg)
{ return this->set_union(arg); }
inline AbstractSet AbstractSet::operator|(const ConcreteSet& arg)
{ return this->set_union(arg); }
inline AbstractSet AbstractSet::operator&(const ConcreteSet& arg)
{ return this->set_intersection(arg); }
inline AbstractSet AbstractSet::operator-(const ConcreteSet& arg)
{ return this->set_difference(arg); }
inline AbstractSet AbstractSet::operator^(const ConcreteSet& arg)
{ return this->set_symmetric_difference(arg); }
inline AbstractSet AbstractSet::operator*(const ConcreteSet& arg)
{ return this->set_product(arg); }

inline AbstractSet ConcreteSet::operator+(const AbstractSet& arg)
{ return this->set_union(arg); }
inline AbstractSet ConcreteSet::operator|(const AbstractSet& arg)
{ return this->set_union(arg); }
inline AbstractSet ConcreteSet::operator&(const AbstractSet& arg)
{ return this->set_intersection(arg); }
inline AbstractSet ConcreteSet::operator-(const AbstractSet& arg)
{ return this->set_difference(arg); }
inline AbstractSet ConcreteSet::operator^(const AbstractSet& arg)
{ return this->set_symmetric_difference(arg); }
inline AbstractSet ConcreteSet::operator*(const AbstractSet& arg)
{ return this->set_product(arg); }

inline ConcreteSet ConcreteSet::operator+(const ConcreteSet& arg)
{ return this->set_union(arg); }
inline ConcreteSet ConcreteSet::operator|(const ConcreteSet& arg)
{ return this->set_union(arg); }
inline ConcreteSet ConcreteSet::operator&(const ConcreteSet& arg)
{ return this->set_intersection(arg); }
inline ConcreteSet ConcreteSet::operator-(const ConcreteSet& arg)
{ return this->set_difference(arg); }
inline ConcreteSet ConcreteSet::operator^(const ConcreteSet& arg)
{ return this->set_symmetric_difference(arg); }
inline ConcreteSet ConcreteSet::operator*(const ConcreteSet& arg)
{ return this->set_product(arg); }

//
// Logical set operations
//
inline bool operator<(const ConcreteSet& lhs, const ConcreteSet& rhs)
    { return lhs.is_subset(rhs); }
inline bool operator>(const ConcreteSet& lhs, const ConcreteSet& rhs)
    { return rhs.is_subset(lhs); }
inline bool operator<=(const ConcreteSet& lhs, const ConcreteSet& rhs)
    { return rhs.is_superset(lhs); }
inline bool operator>=(const ConcreteSet& lhs, const ConcreteSet& rhs)
    { return lhs.is_superset(rhs); }

bool operator==(const ConcreteSet& lhs, const ConcreteSet& rhs);
bool operator!=(const ConcreteSet& lhs, const ConcreteSet& rhs);


template <typename... Ts>
AbstractSet SetOf();

ConcreteSet SetOf(const std::vector<int>& arg);
//ConcreteSet SetOf(const std::vector<double>& arg);
//ConcreteSet SetOf(const std::vector<std::string>& arg);
ConcreteSet SetOf(const std::set<int>& arg);
//ConcreteSet SetOf(const std::set<double>& arg);
//ConcreteSet SetOf(const std::set<std::string>& arg);
ConcreteSet SetOf(const std::initializer_list<int>& arg);
//ConcreteSet SetOf(const std::initializer_list<double>& arg);
//ConcreteSet SetOf(const std::initializer_list<std::string>& arg);

ConcreteSet RangeSet(int start, int stop, int step=1);
//ConcreteSet RangeSet(double start, double stop, double step=1.0);

}

#endif
