#pragma once

#include <variant>
#include <vector>

#include "base_terms.hpp"
#include "value_terms.hpp"

namespace coek {

typedef std::variant<int, expr_pointer_t> refarg_types;

class ParameterRefTerm : public BaseParameterTerm {
   public:
    std::vector<refarg_types> indices;
    std::string name;
    void* param;

   public:
    ParameterRefTerm(const std::vector<refarg_types>& _indices, const std::string& _name,
                     void* _param);

    double _eval() const { throw std::runtime_error("Cannot evaluate a ParameterRefTerm"); }
    term_id id() { return ParameterRefTerm_id; }
};

class VariableRefTerm : public BaseVariableTerm {
   public:
    std::vector<refarg_types> indices;
    std::string name;
    void* var;

   public:
    VariableRefTerm(const std::vector<refarg_types>& _indices, const std::string& _name,
                    void* _var);

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
