#pragma once

#include <variant>
#include <vector>
#include <memory>

#include "base_terms.hpp"
#include "value_terms.hpp"
#include "assoc_array_base.hpp"

namespace coek {

typedef std::variant<int, expr_pointer_t> refarg_types;

class DataRefTerm : public BaseParameterTerm {
   public:
    std::vector<refarg_types> indices;
    std::string name;
    std::shared_ptr<AssocArrayBase<ConstantTerm>> data;

   public:
    DataRefTerm(const std::vector<refarg_types>& _indices, const std::string& _name,
                     std::shared_ptr<AssocArrayBase<ConstantTerm>>& _data);

    double _eval() const { throw std::runtime_error("Cannot evaluate a DataRefTerm"); }
    term_id id() { return DataRefTerm_id; }

    expr_pointer_t get_concrete_data();
};

class ParameterRefTerm : public BaseParameterTerm {
   public:
    std::vector<refarg_types> indices;
    std::string name;
    std::shared_ptr<AssocArrayBase<ParameterTerm>> param;

   public:
    ParameterRefTerm(const std::vector<refarg_types>& _indices, const std::string& _name,
                     std::shared_ptr<AssocArrayBase<ParameterTerm>>& _param);

    double _eval() const { throw std::runtime_error("Cannot evaluate a ParameterRefTerm"); }
    term_id id() { return ParameterRefTerm_id; }

    expr_pointer_t get_concrete_parameter();
};

class VariableRefTerm : public BaseVariableTerm {
   public:
    std::vector<refarg_types> indices;
    std::string name;
    std::shared_ptr<AssocArrayBase<VariableTerm>> var;

   public:
    VariableRefTerm(const std::vector<refarg_types>& _indices, const std::string& _name,
                    std::shared_ptr<AssocArrayBase<VariableTerm>>& _var);

    double _eval() const { throw std::runtime_error("Cannot evaluate a VariableRefTerm"); }
    term_id id() { return VariableRefTerm_id; }

    expr_pointer_t get_concrete_variable();
};

class SetRefTerm : public BaseExpressionTerm {
   public:
    expr_pointer_t body;

   public:
    explicit SetRefTerm(const expr_pointer_t& repn);
};

}  // namespace coek
