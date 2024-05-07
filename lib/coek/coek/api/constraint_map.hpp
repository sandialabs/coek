#pragma once

#include "coek/api/constraint.hpp"
#include "coek/api/indexed_container.hpp"

namespace coek {

//
// This wrapper class allows the use of forward references to
// ConstraintMap in the declaration of model classes.
//
class ConstraintMap : public IndexedComponent_Map<Constraint> {
   public:
    explicit ConstraintMap(size_t n) : IndexedComponent_Map<Constraint>(n) {}

    explicit ConstraintMap(const std::vector<size_t>& _shape)
        : IndexedComponent_Map<Constraint>(_shape)
    {
    }

    explicit ConstraintMap(const std::initializer_list<size_t>& _shape)
        : IndexedComponent_Map<Constraint>(_shape)
    {
    }

#ifdef COEK_WITH_COMPACT_MODEL
    explicit ConstraintMap(ConcreteSet& arg) : IndexedComponent_Map<Constraint>(arg) {}
#endif

    virtual ~ConstraintMap() {}

    const std::string& name() const { return repn->_name; }

    ConstraintMap& name(const std::string& str)
    {
        repn->_name = str;
        return *this;
    }
};

inline ConstraintMap constraint(size_t n) { return ConstraintMap(n); }
inline ConstraintMap constraint(const std::vector<size_t>& shape) { return ConstraintMap(shape); }
inline ConstraintMap constraint(const std::initializer_list<size_t>& shape)
{
    return ConstraintMap(shape);
}
#ifdef COEK_WITH_COMPACT_MODEL
inline ConstraintMap constraint(ConcreteSet& shape) { return ConstraintMap(shape); }
inline ConstraintMap constraint(ConcreteSet&& shape) { return ConstraintMap(shape); }
#endif

inline ConstraintMap constraint(const std::string& name, size_t n)
{
    return constraint(n).name(name);
}
inline ConstraintMap constraint(const std::string& name, const std::vector<size_t>& shape)
{
    return constraint(shape).name(name);
}
inline ConstraintMap constraint(const std::string& name, const std::initializer_list<size_t>& shape)
{
    return constraint(shape).name(name);
}
#ifdef COEK_WITH_COMPACT_MODEL
inline ConstraintMap constraint(const std::string& name, ConcreteSet& shape)
{
    return ConstraintMap(shape).name(name);
}
inline ConstraintMap constraint(const std::string& name, ConcreteSet&& shape)
{
    return ConstraintMap(shape).name(name);
}
#endif

}  // namespace coek
