#include "coek/api/parameter_assoc_array.hpp"

#include <cmath>
#include <variant>

#include "coek/ast/compact_terms.hpp"

namespace coek {

//
// ParameterAssocArrayRepn
//

ParameterAssocArrayRepn::ParameterAssocArrayRepn() { parameter_template.name("p"); }

void ParameterAssocArrayRepn::resize_index_vectors(IndexVector& tmp,
                                                   std::vector<refarg_types>& reftmp)
{
    tmp = cache.alloc(dim());
    reftmp.resize(dim());
}

void ParameterAssocArrayRepn::setup()
{
    if (first_setup) {
        auto value = std::make_shared<ConstantTerm>(
            parameter_template.value_expression().expand().value());
        for (size_t i = 0; i < size(); i++) {
            values.emplace_back(CREATE_POINTER(ParameterTerm, value));
        }
        first_setup = false;
    }
}

void ParameterAssocArrayRepn::value(double value)
{
    parameter_template.value(value);
    if (values.size() > 0) {
        Expression e(value);
        for (auto& var : values)
            var.value(e);
    }
}

void ParameterAssocArrayRepn::value(const Expression& value)
{
    parameter_template.value(value);
    if (values.size() > 0) {
        for (auto& var : values)
            var.value(value);
    }
}

void ParameterAssocArrayRepn::name(const std::string& name)
{
    parameter_template.name(name);
    if (values.size() > 0) {
        // If the string is empty, then we reset the names of all variables
        if (name.size() == 0) {
            for (auto& var : values)
                var.name(name);
        }
        // Otherwise, we re-generate the names
        else
            generate_names();
    }
}

//
// ParameterAssocArray
//

size_t ParameterAssocArray::size() { return get_repn()->size(); }

size_t ParameterAssocArray::dim() { return get_repn()->dim(); }

std::vector<Parameter>::iterator ParameterAssocArray::begin() { return get_repn()->values.begin(); }

std::vector<Parameter>::iterator ParameterAssocArray::end() { return get_repn()->values.end(); }

#ifdef COEK_WITH_COMPACT_MODEL
expr_pointer_t create_paramref(const std::vector<refarg_types>& indices, const std::string& name,
                               void* var);

Expression ParameterAssocArray::create_paramref(const std::vector<refarg_types>& args)
{
    return coek::create_paramref(args, get_repn()->parameter_template.name(), this);
}
#endif

//
// OTHER
//

#ifdef COEK_WITH_COMPACT_MODEL
expr_pointer_t get_concrete_param(ParameterRefTerm& paramref)
{
    ParameterAssocArray* param = static_cast<ParameterAssocArray*>(paramref.param);

    std::vector<int> index;
    for (auto it = paramref.indices.begin(); it != paramref.indices.end(); ++it) {
        refarg_types& reftmp = *it;
        if (auto ival = std::get_if<int>(&reftmp))
            index.push_back(*ival);
        else {
            expr_pointer_t eval = std::get<expr_pointer_t>(reftmp);
            double vald = eval->eval();
            long int vali = std::lround(vald);
            assert(fabs(vald - vali) < 1e-7);
            index.push_back(static_cast<int>(vali));
        }
    }

    IndexVector& tmp = param->tmp;
    for (size_t i = 0; i < index.size(); i++)
        tmp[i] = index[i];

    Expression e = param->index(tmp);
    return e.repn;
}
#endif

}  // namespace coek
