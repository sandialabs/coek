#include "compact_terms.hpp"
#include "../api/parameter_assoc_array_repn.hpp"
#include "../api/variable_assoc_array_repn.hpp"

namespace coek {

//
// ParameterRefTerm
//

ParameterRefTerm::ParameterRefTerm(const std::vector<refarg_types>& _indices,
                                   const std::string& _name, std::shared_ptr<ParameterAssocArrayRepn>& _param)
    : indices(_indices), name(_name), param(_param)
{
}

expr_pointer_t create_paramref(const std::vector<refarg_types>& indices, const std::string& name,
                               std::shared_ptr<ParameterAssocArrayRepn>& param)
{
    return CREATE_POINTER(ParameterRefTerm, indices, name, param);
}

//
// VariableRefTerm
//

VariableRefTerm::VariableRefTerm(const std::vector<refarg_types>& _indices,
                                 const std::string& _name, std::shared_ptr<VariableAssocArrayRepn>& _var)
    : indices(_indices), name(_name), var(_var)
{
}

expr_pointer_t create_varref(const std::vector<refarg_types>& indices, const std::string& name,
                             std::shared_ptr<VariableAssocArrayRepn>& var)
{
    return CREATE_POINTER(VariableRefTerm, indices, name, var);
}

//
// SetRefTerm
//

SetRefTerm::SetRefTerm(const expr_pointer_t& repn) : body(repn) {}

}  // namespace coek
