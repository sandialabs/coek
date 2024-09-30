#pragma once

#include <variant>
#include <vector>
#include <memory>

#include "base_terms.hpp"
#include "value_terms.hpp"

namespace coek {

class ParameterAssocArrayRepn;
class VariableAssocArrayRepn;

typedef std::variant<int, expr_pointer_t> refarg_types;

class ParameterRefTerm : public BaseParameterTerm {
   public:
    std::vector<refarg_types> indices;
    std::string name;
    std::shared_ptr<ParameterAssocArrayRepn> param;

   public:
    ParameterRefTerm(const std::vector<refarg_types>& _indices, const std::string& _name,
                     std::shared_ptr<ParameterAssocArrayRepn>& _param);

    double _eval() const { throw std::runtime_error("Cannot evaluate a ParameterRefTerm"); }
    term_id id() { return ParameterRefTerm_id; }
};

class VariableRefTerm : public BaseVariableTerm {
   public:
    std::vector<refarg_types> indices;
    std::string name;
    std::shared_ptr<VariableAssocArrayRepn> var;

   public:
    VariableRefTerm(const std::vector<refarg_types>& _indices, const std::string& _name,
                    std::shared_ptr<VariableAssocArrayRepn>& _var);

    double _eval() const { throw std::runtime_error("Cannot evaluate a VariableRefTerm"); }
    term_id id() { return VariableRefTerm_id; }
};

class SetRefTerm : public BaseExpressionTerm {
   public:
    expr_pointer_t body;

   public:
    explicit SetRefTerm(const expr_pointer_t& repn);
};

}  // namespace coek
