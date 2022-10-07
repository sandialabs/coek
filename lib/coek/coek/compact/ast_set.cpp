#include "ast_set.hpp"

#include <cassert>

namespace coek {

namespace ast {

class DefaultProductSetIteratorRepn : public SetIteratorRepnBase<std::vector<set_types>> {
   public:
    ProductSet* ref;
    std::vector<IndexParameter> indices;
    bool done;
    std::vector<size_t> sizes;
    std::vector<set_types> _value;
    std::vector<size_t> curr;

   public:
    DefaultProductSetIteratorRepn(ProductSet* _ref, const std::vector<IndexParameter>& _indices,
                                  bool _done = false)
        : ref(_ref), indices(_indices), done(_done)
    {
        if (indices.size() != ref->dim()) throw std::runtime_error("Bad number of indices");
        if (!done) initialize();
    }

    DefaultProductSetIteratorRepn(ProductSet* _ref, bool _done = false) : ref(_ref), done(_done)
    {
        indices.resize(ref->dim());
        if (!done) initialize();
    }

    void initialize()
    {
        for (auto it = ref->simple_sets.begin(); it != ref->simple_sets.end(); ++it)
            sizes.push_back((*it)->size());
        assert(sizes.size() == ref->dim());
        _value.resize(ref->dim());
        for (size_t i = 0; i < ref->dim(); i++) {
            ref->simple_sets[i]->get_value(0, _value[i], indices[i]);
        }
        curr.assign(ref->dim(), 0);
    }

    void next()
    {
        if (done) return;

        size_t i_ = 0;
        while (i_ < curr.size()) {
            size_t i = curr.size() - 1 - i_;
            size_t val = ++curr[i];
            if (val < sizes[i]) {
                ref->simple_sets[i]->get_value(curr[i], _value[i], indices[i]);
                break;
            }
            curr[i] = 0;
            ref->simple_sets[i]->get_value(0, _value[i], indices[i]);
            i_++;
        }

        done = (i_ == curr.size());
    }

    bool equals(const SetIteratorRepnBase<std::vector<set_types>>* repn) const
    {
        auto _repn = dynamic_cast<const DefaultProductSetIteratorRepn*>(repn);
        return (done == _repn->done);
    }

    bool not_equals(const SetIteratorRepnBase<std::vector<set_types>>* repn) const
    {
        auto _repn = dynamic_cast<const DefaultProductSetIteratorRepn*>(repn);
        return (done != _repn->done);
    }

    std::vector<set_types>& value() { return _value; }

    const std::vector<set_types>& value() const { return _value; }
};

ProductSet::ProductSet(const std::shared_ptr<BaseSetExpression>& _lhs,
                       const std::shared_ptr<BaseSetExpression>& _rhs)
    : BaseSetExpression()
{
    this->_finite = _lhs->_finite and _rhs->_finite;
    this->_countable = _lhs->_countable and _rhs->_countable;
    initialized = _lhs->initialized and _rhs->initialized;
    simple_sets.push_back(_lhs);
    simple_sets.push_back(_rhs);
    _dim = 2;
}

ProductSet::ProductSet(const std::shared_ptr<ProductSet>& _lhs,
                       const std::shared_ptr<BaseSetExpression>& _rhs)
    : BaseSetExpression()
{
    this->_finite = _lhs->_finite and _rhs->_finite;
    this->_countable = _lhs->_countable and _rhs->_countable;
    initialized = _lhs->initialized and _rhs->initialized;
    simple_sets = _lhs->simple_sets;
    simple_sets.push_back(_rhs);
    _dim = _lhs->dim() + 1;
}

ProductSet::ProductSet(const std::shared_ptr<BaseSetExpression>& _lhs,
                       const std::shared_ptr<ProductSet>& _rhs)
    : BaseSetExpression()
{
    this->_finite = _lhs->_finite and _rhs->_finite;
    this->_countable = _lhs->_countable and _rhs->_countable;
    initialized = _lhs->initialized and _rhs->initialized;
    _dim = _rhs->dim() + 1;
    simple_sets.push_back(_lhs);
    for (auto it = _rhs->simple_sets.begin(); it != _rhs->simple_sets.end(); ++it)
        simple_sets.push_back(*it);
}

ProductSet::ProductSet(const std::shared_ptr<ProductSet>& _lhs,
                       const std::shared_ptr<ProductSet>& _rhs)
    : BaseSetExpression()
{
    this->_finite = _lhs->_finite and _rhs->_finite;
    this->_countable = _lhs->_countable and _rhs->_countable;
    initialized = _lhs->initialized and _rhs->initialized;
    _dim = _lhs->dim() + _rhs->dim();
    for (auto it = _lhs->simple_sets.begin(); it != _lhs->simple_sets.end(); ++it)
        simple_sets.push_back(*it);
    for (auto it = _rhs->simple_sets.begin(); it != _rhs->simple_sets.end(); ++it)
        simple_sets.push_back(*it);
}

ProductSetIterator ProductSet::begin()
{
    auto tmp = std::make_shared<DefaultProductSetIteratorRepn>(this);
    return ProductSetIterator(tmp);
}

ProductSetIterator ProductSet::begin(const std::vector<IndexParameter>& indices)
{
    auto tmp = std::make_shared<DefaultProductSetIteratorRepn>(this, indices);
    return ProductSetIterator(tmp);
}

ProductSetIterator ProductSet::end()
{
    auto tmp = std::make_shared<DefaultProductSetIteratorRepn>(this, true);
    return ProductSetIterator(tmp);
}

std::shared_ptr<SetIteratorRepnBase<std::vector<set_types>>> ProductSet::begin_NDiterator()
{
    auto tmp = std::make_shared<DefaultProductSetIteratorRepn>(this);
    return tmp;
}

std::shared_ptr<SetIteratorRepnBase<std::vector<set_types>>> ProductSet::begin_NDiterator(
    const std::vector<IndexParameter>& indices)
{
    auto tmp = std::make_shared<DefaultProductSetIteratorRepn>(this, indices);
    return tmp;
}

std::shared_ptr<SetIteratorRepnBase<std::vector<set_types>>> ProductSet::end_NDiterator()
{
    auto tmp = std::make_shared<DefaultProductSetIteratorRepn>(this, true);
    return tmp;
}

void ProductSet::initialize()
{
    for (auto it = simple_sets.begin(); it != simple_sets.end(); ++it) (*it)->initialize();
}

size_t ProductSet::size()
{
    size_t _size = 1;
    for (auto it = simple_sets.begin(); it != simple_sets.end(); ++it) _size *= (*it)->size();
    return _size;
}

std::shared_ptr<BaseSetExpression> ProductSet::set_union(
    const std::shared_ptr<BaseSetExpression>& lhs,
    const std::shared_ptr<BaseSetExpression>& rhs) const
{
    if (rhs->dim() != _dim)
        throw std::runtime_error(
            "Cannot take the union of product sets with different dimensions.");
    auto tmp = std::make_shared<ProductSetUnion>(std::dynamic_pointer_cast<ProductSet>(lhs),
                                                 std::dynamic_pointer_cast<ProductSet>(rhs));
    return tmp;
}

std::shared_ptr<BaseSetExpression> ProductSet::set_intersection(
    const std::shared_ptr<BaseSetExpression>& lhs,
    const std::shared_ptr<BaseSetExpression>& rhs) const
{
    if (rhs->dim() != _dim)
        throw std::runtime_error(
            "Cannot take the intersection of product sets with different dimensions.");
    auto tmp = std::make_shared<ProductSetIntersection>(std::dynamic_pointer_cast<ProductSet>(lhs),
                                                        std::dynamic_pointer_cast<ProductSet>(rhs));
    return tmp;
}

std::shared_ptr<BaseSetExpression> ProductSet::set_difference(
    const std::shared_ptr<BaseSetExpression>& lhs,
    const std::shared_ptr<BaseSetExpression>& rhs) const
{
    if (rhs->dim() != _dim)
        throw std::runtime_error(
            "Cannot take the difference of product sets with different dimensions.");
    auto tmp = std::make_shared<ProductSetDifference>(std::dynamic_pointer_cast<ProductSet>(lhs),
                                                      std::dynamic_pointer_cast<ProductSet>(rhs));
    return tmp;
}

std::shared_ptr<BaseSetExpression> ProductSet::set_symmetric_difference(
    const std::shared_ptr<BaseSetExpression>& lhs,
    const std::shared_ptr<BaseSetExpression>& rhs) const
{
    if (rhs->dim() != _dim)
        throw std::runtime_error(
            "Cannot take the symmetric difference of product sets with different dimensions.");
    auto tmp = std::make_shared<ProductSetSymmetricDifference>(
        std::dynamic_pointer_cast<ProductSet>(lhs), std::dynamic_pointer_cast<ProductSet>(rhs));
    return tmp;
}

std::shared_ptr<BaseSetExpression> ProductSet::set_product(
    const std::shared_ptr<BaseSetExpression>& lhs,
    const std::shared_ptr<BaseSetExpression>& rhs) const
{
    if (rhs->dim() == 1) {
        auto tmp = std::make_shared<ProductSet>(std::dynamic_pointer_cast<ProductSet>(lhs), rhs);
        return tmp;
    }
    else {
        auto tmp = std::make_shared<ProductSet>(std::dynamic_pointer_cast<ProductSet>(lhs),
                                                std::dynamic_pointer_cast<ProductSet>(rhs));
        return tmp;
    }
}

bool ProductSet::contains(const std::vector<set_types>& arg)
{
    assert(arg.size() == _dim);

    auto ait = arg.begin();
    for (auto it = simple_sets.begin(); it != simple_sets.end(); ++it) {
        const set_types tmp = *ait;
        if (not(*it)->contains_any(tmp)) return false;
#if 0
    const set_types* tmp = &*ait;
    if (auto pval = std::get_if<int>(tmp)) {
        if (not (*it)->contains_any(*pval))
            return false;
        }
    else if (auto pval = std::get_if<double>(tmp)) {
        if (not (*it)->contains_any(*pval))
            return false;
        }
    else if (auto pval = std::get_if<std::string>(tmp)) {
        if (not (*it)->contains_any(*pval))
            return false;
        }
    else
        throw std::runtime_error("contains() called with an empty variant value.");
#endif
        ++ait;
    }
    return true;
}

bool ProductSet::contains_any(const std::any& arg)
{
    std::vector<set_types> val = std::any_cast<std::vector<set_types>>(arg);
    return contains(val);
}

std::any ProductSet::value(size_t /*i*/) { return 0; }

void ProductSet::get_value(size_t /*i*/, set_types& /*arg*/, IndexParameter& /*index*/)
{
    throw std::runtime_error("get_value() cannot be called from a ProductSet.");
}

class FiniteProductSetIteratorRepn : public SetIteratorRepnBase<std::vector<set_types>> {
   public:
    typename std::vector<std::vector<set_types>>::iterator iterator;

   public:
    FiniteProductSetIteratorRepn(
        const typename std::vector<std::vector<set_types>>::iterator& _iterator)
        : iterator(_iterator)
    {
    }

    void next() { iterator++; }

    bool equals(const SetIteratorRepnBase<std::vector<set_types>>* repn) const
    {
        auto _repn = dynamic_cast<const FiniteProductSetIteratorRepn*>(repn);
        return (_repn->iterator == iterator);
    }

    bool not_equals(const SetIteratorRepnBase<std::vector<set_types>>* repn) const
    {
        auto _repn = dynamic_cast<const FiniteProductSetIteratorRepn*>(repn);
        return (_repn->iterator != iterator);
    }

    std::vector<set_types>& value() { return *iterator; }

    const std::vector<set_types>& value() const { return *iterator; }
};

ProductSetIterator FiniteProductSet::begin()
{
    auto tmp = std::make_shared<FiniteProductSetIteratorRepn>(data.begin());
    return ProductSetIterator(tmp);
}

ProductSetIterator FiniteProductSet::end()
{
    auto tmp = std::make_shared<FiniteProductSetIteratorRepn>(data.end());
    return ProductSetIterator(tmp);
}

bool FiniteProductSet::contains(const std::vector<set_types>& arg) const
{
    return !(set_data.find(arg) == set_data.end());
}

void FiniteProductSet::add_unique(const std::vector<set_types>& arg)
{
    data.push_back(arg);
    set_data.insert(arg);
}

std::any FiniteProductSet::value(size_t i)
{
    if (not this->initialized) throw std::runtime_error("Getting value() of uninitialized set.");
    if (i >= data.size())
        throw std::runtime_error(std::string("Requested set index that is too large: i=")
                                 + std::to_string(i) + std::string(" size=")
                                 + std::to_string(data.size()));
    return data[i];
}

ProductSetUnion::ProductSetUnion(const std::shared_ptr<ProductSet>& _lhs,
                                 const std::shared_ptr<ProductSet>& _rhs)
    : lhs(_lhs), rhs(_rhs)
{
    assert(_lhs->dim() == _rhs->dim());
    _dim = _lhs->dim();
    this->_finite = lhs->_finite and rhs->_finite;
    this->_countable = lhs->_countable and rhs->_countable;
}

void ProductSetUnion::initialize()
{
    if (this->initialized) return;

    lhs->initialize();
    for (auto it = lhs->begin(); it != lhs->end(); ++it) this->add_unique(*it);

    rhs->initialize();
    for (auto it = rhs->begin(); it != rhs->end(); ++it) {
        if (not this->contains(*it)) this->add_unique(*it);
    }

    this->initialized = true;
    assert(this->data.size() == this->set_data.size());
}

ProductSetIntersection::ProductSetIntersection(const std::shared_ptr<ProductSet>& _lhs,
                                               const std::shared_ptr<ProductSet>& _rhs)
    : lhs(_lhs), rhs(_rhs)
{
    assert(_lhs->dim() == _rhs->dim());
    _dim = _lhs->dim();
    this->_finite = lhs->_finite or rhs->_finite;
    this->_countable = lhs->_countable or rhs->_countable;
}

void ProductSetIntersection::initialize()
{
    if (this->initialized) return;

    lhs->initialize();
    rhs->initialize();
    if (lhs->size() <= rhs->size()) {
        for (auto it = lhs->begin(); it != lhs->end(); ++it) {
            if (rhs->contains(*it)) this->add_unique(*it);
        }
    }
    else {
        for (auto it = rhs->begin(); it != rhs->end(); ++it) {
            if (lhs->contains(*it)) this->add_unique(*it);
        }
    }

    this->initialized = true;
    assert(this->data.size() == this->set_data.size());
}

ProductSetDifference::ProductSetDifference(const std::shared_ptr<ProductSet>& _lhs,
                                           const std::shared_ptr<ProductSet>& _rhs)
    : lhs(_lhs), rhs(_rhs)
{
    assert(_lhs->dim() == _rhs->dim());
    _dim = _lhs->dim();
    this->_finite = lhs->_finite;
    this->_countable = lhs->_countable;
}

void ProductSetDifference::initialize()
{
    if (this->initialized) return;

    lhs->initialize();
    rhs->initialize();

    for (auto it = lhs->begin(); it != lhs->end(); ++it) {
        if (not rhs->contains(*it)) this->add_unique(*it);
    }

    this->initialized = true;
    assert(this->data.size() == this->set_data.size());
}

ProductSetSymmetricDifference::ProductSetSymmetricDifference(
    const std::shared_ptr<ProductSet>& _lhs, const std::shared_ptr<ProductSet>& _rhs)
    : lhs(_lhs), rhs(_rhs)
{
    assert(_lhs->dim() == _rhs->dim());
    _dim = _lhs->dim();
    this->_finite = lhs->_finite and rhs->_finite;
    this->_countable = lhs->_countable and rhs->_countable;
}

void ProductSetSymmetricDifference::initialize()
{
    if (this->initialized) return;

    lhs->initialize();
    rhs->initialize();

    for (auto it = lhs->begin(); it != lhs->end(); ++it) {
        if (not rhs->contains(*it)) this->add_unique(*it);
    }
    for (auto it = rhs->begin(); it != rhs->end(); ++it) {
        if (not lhs->contains(*it)) this->add_unique(*it);
    }

    this->initialized = true;
    assert(this->data.size() == this->set_data.size());
}

}  // namespace ast

}  // namespace coek
