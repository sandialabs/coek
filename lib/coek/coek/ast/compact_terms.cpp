#include "compact_terms.hpp"

namespace coek {

//
// ParameterRefTerm
//

ParameterRefTerm::ParameterRefTerm(const std::vector<refarg_types>& _indices,
                                   const std::string& _name, void* _param)
    : indices(_indices), name(_name), param(_param)
{
    for (auto& value : indices) {
        if (auto ival = std::get_if<expr_pointer_t>(&value)) {
            OWN_POINTER(*ival);
        }
    }
}

ParameterRefTerm::~ParameterRefTerm()
{
    for (auto& value : indices) {
        if (auto ival = std::get_if<expr_pointer_t>(&value)) {
            DISOWN_POINTER(*ival);
        }
    }
}

expr_pointer_t create_paramref(const std::vector<refarg_types>& indices, const std::string& name,
                               void* param)
{
    return CREATE_POINTER(ParameterRefTerm, indices, name, param);
}

//
// VariableRefTerm
//

VariableRefTerm::VariableRefTerm(const std::vector<refarg_types>& _indices,
                                 const std::string& _name, void* _var)
    : indices(_indices), name(_name), var(_var)
{
    for (auto& value : indices) {
        if (auto ival = std::get_if<expr_pointer_t>(&value)) {
            OWN_POINTER(*ival);
        }
    }
}

VariableRefTerm::~VariableRefTerm()
{
    for (auto& value : indices) {
        if (auto ival = std::get_if<expr_pointer_t>(&value)) {
            DISOWN_POINTER(*ival);
        }
    }
}

expr_pointer_t create_varref(const std::vector<refarg_types>& indices, const std::string& name,
                             void* var)
{
    return CREATE_POINTER(VariableRefTerm, indices, name, var);
}

//
// SetRefTerm
//

SetRefTerm::SetRefTerm(const expr_pointer_t& repn) : body(repn) { OWN_POINTER(body); }

SetRefTerm::~SetRefTerm() { DISOWN_POINTER(body); }

}  // namespace coek
