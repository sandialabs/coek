#pragma once

#include <string>
#include <vector>

#include "coek/api/expression.hpp"
#include "coek/model/model.hpp"

namespace coek {

class Model;

class NLPModelRepn {
   public:
    Model model;
    std::map<size_t, VariableRepn> used_variables;
    std::map<VariableRepn, size_t> fixed_variables;
    std::map<ParameterRepn, size_t> parameters;

   public:
    NLPModelRepn() {}
    NLPModelRepn(Model& _model) : model(_model) {}
    virtual ~NLPModelRepn() {}

    virtual void initialize() = 0;
    virtual void reset(void) = 0;

    virtual size_t num_variables() const = 0;
    virtual size_t num_objectives() const = 0;
    virtual size_t num_constraints() const = 0;
    virtual size_t num_nonzeros_Jacobian() const = 0;
    virtual size_t num_nonzeros_Hessian_Lagrangian() const = 0;

    virtual VariableRepn get_variable(size_t i);
    virtual void set_variable(size_t i, const VariableRepn v);

    virtual void set_variables(std::vector<double>& x) = 0;
    virtual void set_variables(const double* x, size_t n) = 0;

    // TODO - Deprecate these methods!  The AD package may change the objective/constraint
    // expressions
    virtual Objective get_objective(size_t i);
    virtual Constraint get_constraint(size_t i);

    virtual bool has_constraint_lower(size_t i);
    virtual bool has_constraint_upper(size_t i);
    virtual double get_constraint_lower(size_t i);
    virtual double get_constraint_upper(size_t i);

    virtual void print_equations(std::ostream& ostr) = 0;
    virtual void print_values(std::ostream& ostr) = 0;

    virtual double compute_f(size_t i) = 0;
    virtual void compute_df(double& f, std::vector<double>& df, size_t i) = 0;
    virtual void compute_H(std::vector<double>& w, std::vector<double>& H) = 0;
    virtual void compute_c(std::vector<double>& c) = 0;
    virtual void compute_dc(std::vector<double>& dc, size_t i) = 0;
    virtual void compute_J(std::vector<double>& J) = 0;

    virtual void get_J_nonzeros(std::vector<size_t>& jrow, std::vector<size_t>& jcol) = 0;
    virtual void get_H_nonzeros(std::vector<size_t>& hrow, std::vector<size_t>& hcol) = 0;
    // Returns true if the Hessian representation is column-major order, and false otherwise
    virtual bool column_major_hessian() = 0;

   public:
    void find_used_variables();

   public:
    /** Get the value of an boolean option
     *
     * The option value is returned by reference if it has
     * a value.
     *
     * \param option  the option name
     * \param value   a boolean value that is passed by reference
     *
     * \returns \c true if the option is found
     */
    virtual bool get_option(const std::string& /*option*/, bool& /*value*/) const { return false; }
    /** Get the value of an integer option
     *
     * The option value is returned by reference if it has
     * a value.
     *
     * \param option  the option name
     * \param value   an integer value that is passed by reference
     *
     * \returns \c true if the option is found
     */
    virtual bool get_option(const std::string& /*option*/, int& /*value*/) const { return false; }
    /** Get the value of a double option
     *
     * The option value is returned by reference if it has
     * a value.
     *
     * \param option  the option name
     * \param value   a double value that is passed by reference
     *
     * \returns \c true if the option is found
     */
    virtual bool get_option(const std::string& /*option*/, double& /*value*/) const
    {
        return false;
    }
    /** Get the value of a string option
     *
     * The option value is returned by reference if it has
     * a value.
     *
     * \param option  the option name
     * \param value   a string value that is passed by reference
     *
     * \returns \c true if the option is found
     */
    virtual bool get_option(const std::string& /*option*/, std::string& /*value*/) const
    {
        return false;
    }

    /** Set a boolean option
     *
     * \param option  the option name
     * \param value   the boolean value
     */
    virtual void set_option(const std::string& /*option*/, bool /*value*/) {}
    /** Set an integer option
     *
     * \param option  the option name
     * \param value   the integer value
     */
    virtual void set_option(const std::string& /*option*/, int /*value*/) {}
    /** Set a double option
     *
     * \param option  the option name
     * \param value   the double value
     */
    virtual void set_option(const std::string& /*option*/, double /*value*/) {}
    /** Set a string option
     *
     * \param option  the option name
     * \param value   the string value
     */
    virtual void set_option(const std::string& /*option*/, const std::string& /*value*/) {}

    /** Set a string option
     *
     * \param option  the option name
     * \param value   the string value
     */
    void set_option(const std::string& option, const char* value)
    {
        this->set_option(option, std::string(value));
    }
};

NLPModelRepn* create_NLPModelRepn(Model& model, const std::string& name);
NLPModelRepn* create_NLPModelRepn(const std::string& name);

}  // namespace coek
