#include "compact_terms.hpp"

namespace coek {

//
// ParameterRefTerm
//

ParameterRefTerm::ParameterRefTerm(const std::vector<refarg_types>& _indices,
                                   const std::string& _name, void* _param)
    : indices(_indices), name(_name), param(_param)
{
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
}

expr_pointer_t create_varref(const std::vector<refarg_types>& indices, const std::string& name,
                             void* var)
{
    return CREATE_POINTER(VariableRefTerm, indices, name, var);
}

//
// SetRefTerm
//

SetRefTerm::SetRefTerm(const expr_pointer_t& repn) : body(repn) {}

}  // namespace coek
