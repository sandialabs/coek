#pragma once

#include "coek/api/expression.hpp"
#include "coek/api/indexed_container.hpp"

namespace coek {

//
// This wrapper class allows the use of forward references to
// SubExpressionMap in the declaration of model classes.
//
class SubExpressionMap : public IndexedComponent_Map<SubExpression> {
   public:
    explicit SubExpressionMap(size_t n) : IndexedComponent_Map<SubExpression>(n) {}

    explicit SubExpressionMap(const std::vector<size_t>& _shape)
        : IndexedComponent_Map<SubExpression>(_shape)
    {
    }

    explicit SubExpressionMap(const std::initializer_list<size_t>& _shape)
        : IndexedComponent_Map<SubExpression>(_shape)
    {
    }

#ifdef COEK_WITH_COMPACT_MODEL
    explicit SubExpressionMap(ConcreteSet& arg) : IndexedComponent_Map<SubExpression>(arg) {}
#endif

    const std::string& name() const { return repn->_name; }

    SubExpressionMap& name(const std::string& str)
    {
        repn->_name = str;
        return *this;
    }
};

inline SubExpressionMap subexpression(size_t n) { return SubExpressionMap(n); }
inline SubExpressionMap subexpression(const std::vector<size_t>& shape)
{
    return SubExpressionMap(shape);
}
inline SubExpressionMap subexpression(const std::initializer_list<size_t>& shape)
{
    return SubExpressionMap(shape);
}
#ifdef COEK_WITH_COMPACT_MODEL
inline SubExpressionMap subexpression(ConcreteSet& shape) { return SubExpressionMap(shape); }
inline SubExpressionMap subexpression(ConcreteSet&& shape) { return SubExpressionMap(shape); }
#endif

inline SubExpressionMap subexpression(const std::string& name, size_t n)
{
    return subexpression(n).name(name);
}
inline SubExpressionMap subexpression(const std::string& name, const std::vector<size_t>& shape)
{
    return subexpression(shape).name(name);
}
inline SubExpressionMap subexpression(const std::string& name,
                                      const std::initializer_list<size_t>& shape)
{
    return subexpression(shape).name(name);
}
#ifdef COEK_WITH_COMPACT_MODEL
inline SubExpressionMap subexpression(const std::string& name, ConcreteSet& shape)
{
    return SubExpressionMap(shape).name(name);
}
inline SubExpressionMap subexpression(const std::string& name, ConcreteSet&& shape)
{
    return SubExpressionMap(shape).name(name);
}
#endif

}  // namespace coek
