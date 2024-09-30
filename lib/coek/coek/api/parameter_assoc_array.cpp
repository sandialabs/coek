#include "coek/api/parameter_assoc_array.hpp"

#include <cmath>
#include <variant>

#include "coek/ast/compact_terms.hpp"

namespace coek {

//
// ParameterAssocArrayRepn
//

ParameterAssocArrayRepn::ParameterAssocArrayRepn() { parameter_template.name("p"); }

void ParameterAssocArrayRepn::resize_index_vectors(IndexVector& tmp_,
                                                   std::vector<refarg_types>& reftmp_)
{
    auto dim_ = dim();
    #ifdef CUSTOM_INDEXVECTOR
    tmp = cache.alloc(dim_);
    tmp_ = cache.alloc(dim_);
    #else
    tmp.resize(dim_);
    tmp_.resize(dim_);
    #endif
    reftmp.resize(dim_);
    reftmp_.resize(dim_);
}

void ParameterAssocArrayRepn::expand()
{
    if (first_expand) {
        auto value = std::make_shared<ConstantTerm>(
            parameter_template.value_expression().expand().value());
        for (size_t i = 0; i < size(); i++) {
            values.emplace_back(CREATE_POINTER(ParameterTerm, value));
        }
        first_expand = false;
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
                               std::shared_ptr<ParameterAssocArrayRepn>& var);

Expression ParameterAssocArray::create_paramref(const std::vector<refarg_types>& args)
{
    auto repn = get_repn();
    return coek::create_paramref(args, repn->parameter_template.name(), repn);
}
#endif

//
// OTHER
//

#ifdef COEK_WITH_COMPACT_MODEL
expr_pointer_t get_concrete_param(ParameterRefTerm& paramref)
{
    //* param = static_cast<ParameterAssocArray*>(paramref.param);
    auto param = std::dynamic_pointer_cast<ParameterAssocArrayRepn>(paramref.param);

    std::vector<int> index;
    //for (auto it = paramref.indices.begin(); it != paramref.indices.end(); ++it) {
    //    refarg_types& reftmp = *it;
    for (auto& reftmp : paramref.indices) {
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
