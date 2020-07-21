#include "compact_terms.hpp"

namespace coek {

#if 0
//
// IndexParameterTerm
//

expr_pointer_t IndexParameterTerm::negate(const expr_pointer_t& repn)
{
return CREATE_POINTER(NegateTerm, repn);
}

expr_pointer_t create_abstract_parameter(const std::string& name)
{
return CREATE_POINTER(IndexParameterTerm, name);
}
#endif

//
// VariableRefTerm
//

VariableRefTerm::VariableRefTerm(const std::vector<refarg_types>& _indices, const std::string& _name, void* _var)
    : indices(_indices), name(_name), var(_var)
{
for (auto it=indices.begin(); it != indices.end(); ++it) {
    if (auto ival = std::get_if<expr_pointer_t>(&(*it))) {
        OWN_POINTER(*ival);
        }
    }
}

VariableRefTerm::~VariableRefTerm()
{
for (auto it=indices.begin(); it != indices.end(); ++it) {
    if (auto ival = std::get_if<expr_pointer_t>(&(*it))) {
        DISOWN_POINTER(*ival);
        }
    }
}

expr_pointer_t create_varref(const std::vector<refarg_types>& indices, const std::string& name, void* var)
{
return CREATE_POINTER(VariableRefTerm, indices, name, var);
}

//
// SetRefTerm
//

SetRefTerm::SetRefTerm(const expr_pointer_t& repn)
    : body(repn)
{
OWN_POINTER(body);
}

SetRefTerm::~SetRefTerm()
{
DISOWN_POINTER(body);
}

}
