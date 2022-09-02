#include <any>
#include "../ast/base_terms.hpp"
#include "coek/compact/coek_sets.hpp"
#include "coek/compact/ast_set.hpp"

namespace coek {

expr_pointer_t create_abstract_parameter(const std::string& name);


class SetIteratorRepn : public SetIteratorRepnBase<std::vector<set_types>>
{
/*
public:

    SetIteratorRepn(const std::shared_ptr<SetIteratorRepnBase<std::vector<set_types>>>& _repn)
        : SetIteratorRepnBase<std::vector<set_types>>(_repn) {}
*/
};

//
// SetIterator
//

SetIterator::SetIterator()
{}

SetIterator::SetIterator(const std::shared_ptr<SetIteratorRepn>& _repn)
    : repn(_repn)
{}

SetIterator& SetIterator::operator++()
{ repn->next(); return *this; }

bool SetIterator::operator==(const SetIterator& other) const
{ return repn->equals(other.repn.get()); }

bool SetIterator::operator!=(const SetIterator& other) const
{ return repn->not_equals(other.repn.get()); }

SetIterator::reference SetIterator::operator*()
{ return repn->value(); }

SetIterator::const_reference SetIterator::operator*() const
{ return repn->value(); }

//
// SetBase
//

SetBase::SetBase()
{}

SetBase::SetBase(const SetRepn& _repn)
  : repn(_repn)
{}

SetBase::~SetBase()
{}

//
// SetCore
//

SetCore::SetCore()
{}

SetCore::SetCore(const SetRepn& _repn)
  : SetBase(_repn)
{}

SetCore::~SetCore()
{}

ConcreteSet SetCore::initialize()
{
repn->initialize();
return repn;
}

size_t SetCore::dim()
{ return repn->dim(); }

//
// AbstractSet
//

AbstractSet::AbstractSet(size_t dim)
{
if (dim == 0)
    throw std::runtime_error("Cannot declare an abstract set with less than one dimension.");
if (dim == 1)
    repn = std::make_shared<ast::FiniteSimpleSet<set_types>>();    
}

AbstractSet::AbstractSet(const SetRepn& _repn)
  : SetCore(_repn)
{}

AbstractSet::~AbstractSet()
{
}

Expression AbstractSet::index(const std::string& name)
{
return create_abstract_parameter(name);
}

AbstractSet AbstractSet::set_union(const AbstractSet& arg) const
{
SetRepn tmp = repn->set_union(repn, arg.repn);
return tmp;
}

AbstractSet AbstractSet::set_intersection(const AbstractSet& arg) const
{
SetRepn tmp = repn->set_intersection(repn, arg.repn);
return tmp;
}

AbstractSet AbstractSet::set_difference(const AbstractSet& arg) const
{
SetRepn tmp = repn->set_difference(repn, arg.repn);
return tmp;
}

AbstractSet AbstractSet::set_symmetric_difference(const AbstractSet& arg) const
{
SetRepn tmp = repn->set_symmetric_difference(repn, arg.repn);
return tmp;
}

AbstractSet AbstractSet::set_product(const AbstractSet& arg)
{
SetRepn tmp = repn->set_product(repn, arg.repn);
return tmp;
}

AbstractSet AbstractSet::set_union(const ConcreteSet& arg) const
{
SetRepn tmp = repn->set_union(repn, arg.repn);
return tmp;
}

AbstractSet AbstractSet::set_intersection(const ConcreteSet& arg) const
{
SetRepn tmp = repn->set_intersection(repn, arg.repn);
return tmp;
}

AbstractSet AbstractSet::set_difference(const ConcreteSet& arg) const
{
SetRepn tmp = repn->set_difference(repn, arg.repn);
return tmp;
}

AbstractSet AbstractSet::set_symmetric_difference(const ConcreteSet& arg) const
{
SetRepn tmp = repn->set_symmetric_difference(repn, arg.repn);
return tmp;
}

AbstractSet AbstractSet::set_product(const ConcreteSet& arg)
{
SetRepn tmp = repn->set_product(repn, arg.repn);
return tmp;
}

//
// ConcreteSet
//

ConcreteSet::ConcreteSet()
{}

ConcreteSet::ConcreteSet(const SetRepn& _repn)
  : SetCore(_repn)
{}

ConcreteSet::ConcreteSet(const ConcreteSet& other)
  : SetCore(other.repn)
{}

ConcreteSet::~ConcreteSet()
{}

ConcreteSet& ConcreteSet::operator=(const ConcreteSet& other)
{
repn = other.repn;
return *this;
}

bool ConcreteSet::finite()
{ return repn->finite(); }

bool ConcreteSet::countable()
{ return repn->countable(); }

bool ConcreteSet::empty()
{ return repn->empty(); }

size_t ConcreteSet::size()
{ return repn->size(); }

SetIterator ConcreteSet::begin()
{
auto tmp = repn->begin_NDiterator();
// BAD
return std::static_pointer_cast<SetIteratorRepn>(tmp);
}

SetIterator ConcreteSet::begin(const std::initializer_list<IndexParameter>& indices)
{
if (repn->dim() != indices.size())
    throw std::runtime_error("ConcreteSet::begin - Passed "+std::to_string(indices.size())+" index parameters into "+std::to_string(repn->dim())+"-D set");

std::vector<IndexParameter> ivec(indices);
auto tmp = repn->begin_NDiterator(ivec);
// BAD
return std::static_pointer_cast<SetIteratorRepn>(tmp);
}

SetIterator ConcreteSet::begin(const std::vector<IndexParameter>& indices)
{
if (repn->dim() != indices.size())
    throw std::runtime_error("ConcreteSet::begin - Passed "+std::to_string(indices.size())+" index parameters into "+std::to_string(repn->dim())+"-D set");

auto tmp = repn->begin_NDiterator(indices);
// BAD
return std::static_pointer_cast<SetIteratorRepn>(tmp);
}

SetIterator ConcreteSet::end()
{
auto tmp = repn->end_NDiterator();
// BAD
return std::static_pointer_cast<SetIteratorRepn>(tmp);
}

#if 0
void ConcreteSet::create_indices(std::vector<std::string>& names)
{
assert(repn->dim() == names.size());
unsigned int dim=0;
for (auto it=names.begin(); it != names.end(); ++it)
    indices.push_back( SetIndex( dim++, *it, repn ) );
}

void ConcreteSet::create_index(std::string& name)
{
assert(repn->dim() == 1);
indices.push_back( SetIndex( 0, name, repn ) );
}
#endif

bool ConcreteSet::is_disjoint(const ConcreteSet& arg) const
{
auto tmp = set_intersection(arg).initialize();
return tmp.size() == 0;
}

bool ConcreteSet::is_subset(const ConcreteSet& arg) const
{ 
auto tmp = set_difference(arg).initialize();
return tmp.size() == 0;
}

bool ConcreteSet::is_superset(const ConcreteSet& arg) const
{
auto tmp = arg.set_difference(*this).initialize();
return tmp.size() == 0;
}

bool ConcreteSet::contains(const std::any& arg)
{ return repn->contains_any(arg); }

std::any ConcreteSet::operator[](size_t i)
{ return value(i); }

std::any ConcreteSet::value(size_t i)
{ return repn->value(i); }

AbstractSet ConcreteSet::set_union(const AbstractSet& arg) const
{
SetRepn tmp = repn->set_union(repn, arg.repn);
return tmp;
}

AbstractSet ConcreteSet::set_intersection(const AbstractSet& arg) const
{
SetRepn tmp = repn->set_intersection(repn, arg.repn);
return tmp;
}

AbstractSet ConcreteSet::set_difference(const AbstractSet& arg) const
{
SetRepn tmp = repn->set_difference(repn, arg.repn);
return tmp;
}

AbstractSet ConcreteSet::set_symmetric_difference(const AbstractSet& arg) const
{
SetRepn tmp = repn->set_symmetric_difference(repn, arg.repn);
return tmp;
}

AbstractSet ConcreteSet::set_product(const AbstractSet& arg)
{
SetRepn tmp = repn->set_product(repn, arg.repn);
return tmp;
}

ConcreteSet ConcreteSet::set_union(const ConcreteSet& arg) const
{
SetRepn tmp = repn->set_union(repn, arg.repn);
tmp->initialize();
return tmp;
}

ConcreteSet ConcreteSet::set_intersection(const ConcreteSet& arg) const
{
SetRepn tmp = repn->set_intersection(repn, arg.repn);
tmp->initialize();
return tmp;
}

ConcreteSet ConcreteSet::set_difference(const ConcreteSet& arg) const
{
SetRepn tmp = repn->set_difference(repn, arg.repn);
tmp->initialize();
return tmp;
}

ConcreteSet ConcreteSet::set_symmetric_difference(const ConcreteSet& arg) const
{
SetRepn tmp = repn->set_symmetric_difference(repn, arg.repn);
tmp->initialize();
return tmp;
}

ConcreteSet ConcreteSet::set_product(const ConcreteSet& arg)
{
SetRepn tmp = repn->set_product(repn, arg.repn);
tmp->initialize();
return tmp;
}

ConcreteSet& ConcreteSet::operator*=(const ConcreteSet& arg)
{
repn = repn->set_product(repn, arg.repn);
repn->initialize();
return *this;
}

/// ------------------------------------------------------------

ConcreteSet SetOf(const std::vector<int>& arg)
{
SetRepn tmp = std::make_shared<ast::VectorSet<int>>(arg);
return tmp;
}

#if 0
ConcreteSet SetOf(const std::vector<double>& arg)
{
SetRepn tmp = std::make_shared<ast::VectorSet<double>>(arg);
return tmp;
}

ConcreteSet SetOf(const std::vector<std::string>& arg)
{
SetRepn tmp = std::make_shared<ast::VectorSet<std::string>>(arg);
return tmp;
}
#endif

ConcreteSet SetOf(const std::set<int>& arg)
{
SetRepn tmp = std::make_shared<ast::SetSet<int>>(arg);
return tmp;
}

#if 0
ConcreteSet SetOf(const std::set<double>& arg)
{
SetRepn tmp = std::make_shared<ast::SetSet<double>>(arg);
return tmp;
}

ConcreteSet SetOf(const std::set<std::string>& arg)
{
SetRepn tmp = std::make_shared<ast::SetSet<std::string>>(arg);
return tmp;
}
#endif

ConcreteSet SetOf(const std::initializer_list<int>& arg)
{
SetRepn tmp = std::make_shared<ast::ListSet<int>>(arg);
return tmp;
}

#if 0
ConcreteSet SetOf(const std::initializer_list<double>& arg)
{
SetRepn tmp = std::make_shared<ast::ListSet<double>>(arg);
return tmp;
}

ConcreteSet SetOf(const std::initializer_list<std::string>& arg)
{
SetRepn tmp = std::make_shared<ast::ListSet<std::string>>(arg);
return tmp;
}
#endif

ConcreteSet RangeSet(int start, int stop, int step)
{
SetRepn tmp = std::make_shared<ast::RangeSet<int>>(start, stop, step);
return tmp;
}

#if 0
ConcreteSet RangeSet(double start, double stop, double step)
{
SetRepn tmp = std::make_shared<ast::RangeSet<double>>(start, stop, step);
return tmp;
}
#endif

}
