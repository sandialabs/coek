#pragma once

#include "coek/api/expression.hpp"
#include "coek/api/indexed_container.hpp"

namespace coek {

//
// This wrapper class allows the use of forward references to
// NamedExpressionMap in the declaration of model classes.
//
class NamedExpressionMap : public IndexedComponent_Map<NamedExpression> {
   public:
    explicit NamedExpressionMap(size_t n) : IndexedComponent_Map<NamedExpression>(n) {}

    explicit NamedExpressionMap(const std::vector<size_t>& _shape)
        : IndexedComponent_Map<NamedExpression>(_shape)
    {
    }

    explicit NamedExpressionMap(const std::initializer_list<size_t>& _shape)
        : IndexedComponent_Map<NamedExpression>(_shape)
    {
    }

#ifdef COEK_WITH_COMPACT_MODEL
    explicit NamedExpressionMap(ConcreteSet& arg) : IndexedComponent_Map<NamedExpression>(arg) {}
#endif

    const std::string& name() const { return repn->_name; }

    NamedExpressionMap& name(const std::string& str)
    {
        repn->_name = str;
        return *this;
    }
};

inline NamedExpressionMap named_expression(size_t n) { return NamedExpressionMap(n); }
inline NamedExpressionMap named_expression(const std::vector<size_t>& shape) { return NamedExpressionMap(shape); }
inline NamedExpressionMap named_expression(const std::initializer_list<size_t>& shape)
{
    return NamedExpressionMap(shape);
}
#ifdef COEK_WITH_COMPACT_MODEL
inline NamedExpressionMap named_expression(ConcreteSet& shape) { return NamedExpressionMap(shape); }
inline NamedExpressionMap named_expression(ConcreteSet&& shape) { return NamedExpressionMap(shape); }
#endif

inline NamedExpressionMap named_expression(const std::string& name, size_t n)
{
    return named_expression(n).name(name);
}
inline NamedExpressionMap named_expression(const std::string& name, const std::vector<size_t>& shape)
{
    return named_expression(shape).name(name);
}
inline NamedExpressionMap named_expression(const std::string& name, const std::initializer_list<size_t>& shape)
{
    return named_expression(shape).name(name);
}
#ifdef COEK_WITH_COMPACT_MODEL
inline NamedExpressionMap named_expression(const std::string& name, ConcreteSet& shape)
{
    return NamedExpressionMap(shape).name(name);
}
inline NamedExpressionMap named_expression(const std::string& name, ConcreteSet&& shape)
{
    return NamedExpressionMap(shape).name(name);
}
#endif

}  // namespace coek
