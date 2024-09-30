#include <cmath>
#include "compact_terms.hpp"

namespace coek {

//
// ParameterRefTerm
//

ParameterRefTerm::ParameterRefTerm(const std::vector<refarg_types>& _indices,
                                   const std::string& _name, std::shared_ptr<AssocArrayBase<ParameterTerm>>& _param)
    : indices(_indices), name(_name), param(_param)
{
}

expr_pointer_t ParameterRefTerm::get_concrete_parameter()
{
    std::vector<int> index;
    for (auto& reftmp : indices) {
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
    //std::cerr << "HERE " << tmp.size() << " " << index.size() << std::endl;
    assert( tmp.size() == index.size() );
    for (size_t i = 0; i < index.size(); i++)
        tmp[i] = index[i];

    return param->index(tmp);
}

expr_pointer_t create_paramref(const std::vector<refarg_types>& indices, const std::string& name,
                               std::shared_ptr<AssocArrayBase<ParameterTerm>> param)
{
    return CREATE_POINTER(ParameterRefTerm, indices, name, param);
}

//
// VariableRefTerm
//

VariableRefTerm::VariableRefTerm(const std::vector<refarg_types>& _indices,
                                 const std::string& _name, std::shared_ptr<AssocArrayBase<VariableTerm>>& _var)
    : indices(_indices), name(_name), var(_var)
{
}

expr_pointer_t VariableRefTerm::get_concrete_variable()
{
    std::vector<int> index;
    for (auto& reftmp : indices) {
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

    IndexVector& tmp = var->tmp;
    //std::cerr << "HERE " << tmp.size() << " " << index.size() << std::endl;
    assert( tmp.size() == index.size() );
    for (size_t i = 0; i < index.size(); i++)
        tmp[i] = index[i];

    return var->index(tmp);
}

expr_pointer_t create_varref(const std::vector<refarg_types>& indices, const std::string& name,
                             std::shared_ptr<AssocArrayBase<VariableTerm>> var)
{
    return CREATE_POINTER(VariableRefTerm, indices, name, var);
}

//
// SetRefTerm
//

SetRefTerm::SetRefTerm(const expr_pointer_t& repn) : body(repn) {}

}  // namespace coek
