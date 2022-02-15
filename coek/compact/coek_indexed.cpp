#include <any>
#include <cmath>
#include "../ast/compact_terms.hpp"
#include "coek/compact/coek_indexed.hpp"


namespace std
{
    template<>
    struct hash<coek::IndexVector>
    {
        size_t operator()(const coek::IndexVector& a) const
        {
            hash<int> hasher;
            size_t h = 0;
            size_t a_size = a.size();
            for (size_t i = 0; i < a_size; ++i)
            {
                // From Boost
                h ^= hasher(a[i]) + 0x9e3779b9 + (h << 6) + (h >> 2);
            }
            return h;
        }
    };
}

namespace coek {

expr_pointer_t create_varref(const std::vector<refarg_types>& indices, const std::string& name, void* var);


#ifdef USING_INDEXVECTOR
class IndexVectorCache
{
public:

    size_t len;
    int* data;
    size_t curr;

public:

    IndexVectorCache()
        : len(0), data(0), curr(0)
        {}
    IndexVectorCache(size_t _len)
        : len(_len), curr(0)
        { data = new int[len]; }

    IndexVectorCache(const IndexVectorCache& other) = delete;

    ~IndexVectorCache()
        { if (len) delete [] data; }

    void resize(size_t _len)
        {
        assert(len == 0);
        len = _len;
        data = new int[len];
        }

    IndexVector alloc(size_t _len)
        {
        assert((curr+_len+1) <= len);
        auto tmp = data+curr;
        *tmp = static_cast<int>(_len);
        //IndexVector vec(tmp);
        curr += _len+1;
        return IndexVector(tmp);
        }
};
#endif

//
// De-reference varref
//
expr_pointer_t get_concrete_var(VariableRefTerm& varref)
{
ConcreteIndexedVariable* var = static_cast<ConcreteIndexedVariable*>(varref.var);

std::vector<int> index;
for (auto it=varref.indices.begin(); it != varref.indices.end(); ++it) {
    refarg_types& reftmp = *it;
    if (auto ival = std::get_if<int>(&reftmp))
        index.push_back(*ival);
    else {
        expr_pointer_t eval = std::get<expr_pointer_t>(reftmp);
        double vald = eval->eval();
        long int vali = std::lround(vald);
        assert(fabs(vald-vali) < 1e-7);
        index.push_back(vali);
        }
    }

IndexVector& tmp = var->tmp;
for (size_t i=0; i<index.size(); i++)
  tmp[i] = index[i];

Expression e = var->index(tmp);
return e.repn;
}

//
// ConcreteIndexedVariableRepn
//


class ConcreteIndexedVariableRepn
{
public:

#ifdef USING_INDEXVECTOR
    IndexVectorCache cache;
#endif
    ConcreteSet concrete_set;
    std::unordered_map<IndexVector, size_t> index;
    std::vector<std::string> names;
    std::vector<Variable> values;
    std::string name;
    Variable variable_template;
    bool call_setup;

public:

    ConcreteIndexedVariableRepn(const ConcreteSet& _arg)
        : concrete_set(_arg), call_setup(true)
        {
        size_t dim = concrete_set.dim();
        cache.resize((dim+2)*concrete_set.size());
        }

    ConcreteIndexedVariableRepn(const ConcreteSet& _arg, const std::string& _name)
        : concrete_set(_arg), name(_name), call_setup(true)
        {
        size_t dim = concrete_set.dim();
        cache.resize((dim+2)*concrete_set.size());
        }

    std::string get_name(size_t index)
        {
        if (call_setup)
            setup();
        if (index >= names.size())
            throw std::runtime_error("Asking for name with an index that is greater than the number of elements in the index set.");
        return names[index];
        }

    void setup()
        {
        call_setup=false;

        auto vtype = variable_template.within();
        bool binary = (vtype == Boolean) or (vtype == Binary);
        bool integer = vtype == Integers;
        for (size_t i=0; i<concrete_set.size(); i++) {
            auto lower = variable_template.lower_expression().expand().get_value();
            auto upper = variable_template.upper_expression().expand().get_value();
            auto value = variable_template.value_expression().expand().get_value();
            values.emplace_back(CREATE_POINTER(IndexedVariableTerm, 
                                        CREATE_POINTER(ConstantTerm, lower),
                                        CREATE_POINTER(ConstantTerm, upper),
                                        CREATE_POINTER(ConstantTerm, value),
                                        binary, integer, i, this));
            }

        for (auto it=concrete_set.begin(); it != concrete_set.end(); ++it) {
            auto indices = *it;
            if (indices.size() == 1) {
                auto tmp = indices[0];
#if 1
                names.push_back(name + "(" + std::to_string(tmp) + ")");
#else
                if (auto ival = std::get_if<int>(&tmp)) {
                    names.push_back(name + "(" + std::to_string(*ival) + ")");
                    }
                else if (auto dval = std::get_if<double>(&tmp)) {
                    names.push_back(name + "(" + std::to_string(*dval) + ")");
                    }
                else {
                    auto sval = std::get<std::string>(tmp);
                    names.push_back(name + "(" + sval + ")");
                    }
#endif
                }
            else {
                std::string _name = name + "(";
                auto tmp = indices[0];
#if 1
                _name += std::to_string(tmp);
#else
                if (auto ival = std::get_if<int>(&tmp)) {
                    _name += std::to_string(*ival);
                    }
                else if (auto dval = std::get_if<double>(&tmp)) {
                    _name += std::to_string(*dval);
                    }
                else {
                    auto sval = std::get<std::string>(tmp);
                    _name += sval;
                    }
#endif
                for (size_t j=1; j<indices.size(); j++) {
                    auto tmp = indices[j];
#if 1
                    _name += "," + std::to_string(tmp);
#else
                    if (auto ival = std::get_if<int>(&tmp)) {
                        _name += "," + std::to_string(*ival);
                        }
                    else if (auto dval = std::get_if<double>(&tmp)) {
                        _name += "," + std::to_string(*dval);
                        }
                    else {
                        auto sval = std::get<std::string>(tmp);
                        _name += "," + sval;
                        }
#endif
                    }
                names.push_back(_name + ")");
                }
            }
#ifdef USING_INDEXVECTOR
        size_t dim = concrete_set.dim();
        size_t i=0;
        for (auto it=concrete_set.begin(); it != concrete_set.end(); ++it) {
            auto& vec = *it;
            IndexVector x = cache.alloc(dim);
            for (size_t j=0; j<dim; j++)
                x[j] = vec[j];
            index[x] = i++;
            }
#else
        size_t i=0;
        for (auto it=concrete_set.begin(); it != concrete_set.end(); ++it)
            index[*it] = i++;
#endif
        }
};

std::string IndexedVariableTerm::get_name()
{
ConcreteIndexedVariableRepn* _var = static_cast<ConcreteIndexedVariableRepn*>(var);
return _var->get_name(vindex);
}


ConcreteIndexedVariable::ConcreteIndexedVariable(const ConcreteSet& arg)
{
repn = std::make_shared<ConcreteIndexedVariableRepn>(arg);
#ifdef USING_INDEXVECTOR
tmp = repn->cache.alloc(repn->concrete_set.dim());
#else
tmp.resize(repn->concrete_set.dim());
#endif
reftmp.resize(repn->concrete_set.dim());
}

ConcreteIndexedVariable::ConcreteIndexedVariable(const ConcreteSet& arg, const std::string& name)
{
repn = std::make_shared<ConcreteIndexedVariableRepn>(arg, name);
#ifdef USING_INDEXVECTOR
tmp = repn->cache.alloc(repn->concrete_set.dim());
#else
tmp.resize(repn->concrete_set.dim());
#endif
reftmp.resize(repn->concrete_set.dim());
}

ConcreteIndexedVariable& ConcreteIndexedVariable::value(double value)
{
repn->variable_template.value(value);
repn->call_setup = true;
return *this;
}

ConcreteIndexedVariable& ConcreteIndexedVariable::value(const Expression& value)
{
repn->variable_template.value(value);
repn->call_setup = true;
return *this;
}

ConcreteIndexedVariable& ConcreteIndexedVariable::lower(double value)
{
repn->variable_template.lower(value);
repn->call_setup = true;
return *this;
}

ConcreteIndexedVariable& ConcreteIndexedVariable::lower(const Expression& value)
{
repn->variable_template.lower(value);
repn->call_setup = true;
return *this;
}

ConcreteIndexedVariable& ConcreteIndexedVariable::upper(double value)
{
repn->variable_template.upper(value);
repn->call_setup = true;
return *this;
}

ConcreteIndexedVariable& ConcreteIndexedVariable::upper(const Expression& value)
{
repn->variable_template.upper(value);
repn->call_setup = true;
return *this;
}

ConcreteIndexedVariable& ConcreteIndexedVariable::name(const std::string& name)
{
repn->name = name;
repn->call_setup = true;
return *this;
}

ConcreteIndexedVariable& ConcreteIndexedVariable::within(VariableTypes vtype)
{
repn->variable_template.within(vtype);
repn->call_setup = true;
return *this;
}

size_t ConcreteIndexedVariable::size() const
{ return repn->concrete_set.size(); }

std::vector<Variable>::iterator ConcreteIndexedVariable::begin()
{ return repn->values.begin(); }

std::vector<Variable>::iterator ConcreteIndexedVariable::end()
{ return repn->values.end(); }

Expression ConcreteIndexedVariable::index(const IndexVector& args)
{
auto _repn = repn.get();
if (_repn->call_setup)
    _repn->setup();

auto curr = _repn->index.find(tmp);
if (curr == _repn->index.end()) {
    std::string err = "Unknown index value: "+_repn->name+"(";
    for (size_t i=0; i<args.size(); i++) {
        if (i > 0)
            err += ",";
#if 1
        err += std::to_string(args[i]);
#else
        if (auto ival = std::get_if<int>(&(args[i])))
            err += std::to_string(*ival);
        else if (auto dval = std::get_if<double>(&(args[i])))
            err += std::to_string(*dval);
        else if (auto sval = std::get_if<std::string>(&(args[i])))
            err += *sval;
#endif
        }
    err += ")";
    throw std::runtime_error(err);
    }
return _repn->values[curr->second];
}

Expression ConcreteIndexedVariable::create_varref(const std::vector<refarg_types>& args)
{ return coek::create_varref(args, repn->name, this); }

//
// AbstractIndexedVariable
//

AbstractIndexedVariable::AbstractIndexedVariable(const AbstractSet& _arg, const std::string& _name)
    : name(_name), abstract_set(_arg)
{}

ConcreteIndexedVariable AbstractIndexedVariable::initialize()
{
auto tmp = abstract_set.initialize();
return tmp;
}

Expression AbstractIndexedVariable::index(const std::vector<refarg_types>& args)
{ return create_varref(args, name, this); }

//
// Other
//

AbstractIndexedVariable variable(const std::string& name, const AbstractSet& arg)
{ return AbstractIndexedVariable(arg, name); }

ConcreteIndexedVariable variable(const ConcreteSet& arg)
{ return ConcreteIndexedVariable(arg); }

ConcreteIndexedVariable variable(const std::string& name, const ConcreteSet& arg)
{ return ConcreteIndexedVariable(arg, name); }

ConcreteIndexedVariable variable(size_t n)
{
auto arg = coek::RangeSet(0, n-1);
return ConcreteIndexedVariable(arg);
}

ConcreteIndexedVariable variable(const std::string& name, const std::vector<size_t>& dim)
{
assert(dim.size() > 0);
auto arg = coek::RangeSet(0, dim[0]-1);
if (dim.size() == 1) {
    return ConcreteIndexedVariable(arg);
    }
for (size_t i=1; i<dim.size(); i++)
    arg *= coek::RangeSet(0, dim[i]-1);
return ConcreteIndexedVariable(arg, name);
}

ConcreteIndexedVariable variable(const std::vector<size_t>& dim)
{
assert(dim.size() > 0);
auto arg = coek::RangeSet(0, dim[0]-1);
if (dim.size() == 1) {
    return ConcreteIndexedVariable(arg);
    }
for (size_t i=1; i<dim.size(); i++)
    arg *= coek::RangeSet(0, dim[i]-1);
return ConcreteIndexedVariable(arg);
}

}
