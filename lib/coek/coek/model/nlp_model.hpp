#pragma once

#include <coek/model/model.hpp>

namespace coek {

class NLPModelRepn;
class OptionCacheRepn;

class OptionCache {
   public:
    std::shared_ptr<OptionCacheRepn> options;

    public:

    OptionCache();

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
    bool get_option(const std::string& option, int& value) const;
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
    bool get_option(const std::string& option, double& value) const;
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
    bool get_option(const std::string& option, std::string& value) const;

    /** Set an integer option
     *
     * \param option  the option name
     * \param value   the integer value
     */
    void set_option(const std::string& option, int value);
    /** Set a double option
     *
     * \param option  the option name
     * \param value   the double value
     */
    void set_option(const std::string& option, double value);
    /** Set a string option
     *
     * \param option  the option name
     * \param value   the string value
     */
    void set_option(const std::string& option, const std::string value);
};


/**
 * An optimization model that provides a view of a coek::Model
 * object that support computations needed for NLP solvers.
 */
class NLPModel : public OptionCache {
   public:
    std::shared_ptr<NLPModelRepn> repn;

   public:
    /** Create an empty NLP model view */
    NLPModel();
    /** Create a NLP model view
     *
     * \param model  a coek::Model
     * \param type  the type of AD used for computations
     */
    NLPModel(Model& model, std::string type);

    /** Initialize the NLP model view
     *
     * \param model  a coek::Model
     * \param type  the type of AD used for computations
     */
    void initialize(Model& model, std::string type);

    /** TODO - maybe this should be called 'update' */
    void reset();

    /** \returns the number of variables in the model */
    size_t num_variables() const;
    /** \returns the number of objectives in the model */
    size_t num_objectives() const;
    /** \returns the number of constraints in the model */
    size_t num_constraints() const;
    /** \returns the number of nonzeros in the Jacobian */
    size_t num_nonzeros_Jacobian() const;
    /** \returns the number of nonzeros in the Hesian Lagrangian */
    size_t num_nonzeros_Hessian_Lagrangian() const;

    /** \returns the i-th variable in the model view */
    Variable get_variable(size_t i);

    /** Sets the value of variables used in the model view */
    void set_variable_view(std::vector<double>& x);
    /** Sets the value of variables used in the model view */
    void set_variable_view(const double* x, size_t n);

    /** \returns the i-th objective in the model view */
    Objective get_objective(size_t i);
    /** \returns the i-th constraint in the model view */
    Constraint get_constraint(size_t i);

    /**
     * Return the row and column indices of the Jacobian nonzeros.
     *
     * \param jrow   vector that is filled with row indices
     * \param jcol   vector that is filled with column indices
     */
    void get_J_nonzeros(std::vector<size_t>& jrow, std::vector<size_t>& jcol);
    /**
     * Return the row and column indices of the Jacobian nonzeros.
     *
     * \param hrow   vector that is filled with row indices
     * \param hcol   vector that is filled with column indices
     *
     * \returns a boolean that is true if the data is column-major ordered.
     */
    void get_H_nonzeros(std::vector<size_t>& hrow, std::vector<size_t>& hcol);

    bool column_major_hessian();

    /** Write the model to the specified file */
    void write(std::string filename);
    /** Write the model to the specified file
     *
     * \param filename  the output file
     * \param varmap  varmap[i] contains the id of the i-th variable in this model
     * \param conmap  conmap[i] contains the id of the i-th constraint in this model
     */
    void write(std::string filename, std::map<size_t, size_t>& varmap,
               std::map<size_t, size_t>& conmap);
    /** Print the equations in the model to \c std::cout */
    void print_equations() const;
    /** Print the equations in the model to the specified output stream */
    void print_equations(std::ostream& ostr) const;
    /** Print the values in the model to \c std::cout */
    void print_values() const;
    /** Print the values in the model to the specified output stream */
    void print_values(std::ostream& ostr) const;

    friend std::ostream& operator<<(std::ostream& ostr, const NLPModel& arg);

   public:
    /**
     * Compute the value of the i-th objective function.
     *
     * This method uses the variable values stored in the model.
     *
     * \param i   objective index (default is 0)
     *
     * \returns value of the specified objective function.
     */
    double compute_f(size_t i = 0);
    /**
     * Compute the value of the 0-th objective function.
     *
     * This method stores the variable values x in the model.
     *
     * \param x   variable values that are stored in the model
     *
     * \returns value of the specified objective function.
     */
    double compute_f(std::vector<double>& x)
    {
        set_variable_view(x);
        return compute_f(0);
    }
    /**
     * Compute the value of the i-th objective function.
     *
     * This method stores the variable values x in the model.
     *
     * \param x   variable values that are stored in the model
     * \param i   objective index (default is 0)
     *
     * \returns value of the specified objective function.
     */
    double compute_f(std::vector<double>& x, size_t i)
    {
        set_variable_view(x);
        return compute_f(i);
    }

    /**
     * Compute the value and gradient of the i-th objective function.
     *
     * This method uses the variable values stored in the model.
     *
     * \param f   reference that stores the objective value
     * \param df   reference that stores the gradient
     * \param i   objective index
     */
    void compute_df(double& f, std::vector<double>& df, size_t i);
    /**
     * Compute the value and gradient of the 0-th objective function.
     *
     * This method uses the variable values stored in the model.
     *
     * \param f   reference that stores the objective value
     * \param df   reference that stores the gradient
     */
    void compute_df(double& f, std::vector<double>& df) { compute_df(f, df, 0); }
    /**
     * Compute the gradient of the 0-th objective function.
     *
     * This method uses the variable values stored in the model.
     *
     * \param df   reference that stores the gradient
     */
    void compute_df(std::vector<double>& df)
    {
        double f;
        compute_df(f, df, 0);
    }
    /**
     * Compute the gradient of the i-th objective function.
     *
     * This method uses the variable values stored in the model.
     *
     * \param df   reference that stores the gradient
     * \param i   objective index
     */
    void compute_df(std::vector<double>& df, size_t i)
    {
        double f;
        compute_df(f, df, i);
    }
    /**
     * Compute the gradient of the i-th objective function.
     *
     * This method stores the variable values in the model.
     *
     * \param x   variable values that are stored in the model
     * \param df   reference that stores the gradient
     * \param i   objective index
     */
    void compute_df(std::vector<double>& x, std::vector<double>& df, size_t i)
    {
        set_variable_view(x);
        double f;
        compute_df(f, df, i);
    }
    /**
     * Compute the gradient of the 0-th objective function.
     *
     * This method stores the variable values in the model.
     *
     * \param x   variable values that are stored in the model
     * \param df   reference that stores the gradient
     */
    void compute_df(std::vector<double>& x, std::vector<double>& df)
    {
        set_variable_view(x);
        double f;
        compute_df(f, df, 0);
    }

    /**
     * Compute the Hessian of Lagrangian
     *
     * This method uses the variable values stored in the model.
     *
     * \param w   weights in the Lagrangian
     * \param H   reference that stores the Hessian
     */
    void compute_H(std::vector<double>& w, std::vector<double>& H);
    /**
     * Compute the Hessian of Lagrangian
     *
     * This method stores the variable values in the model.
     *
     * \param x   variable values that are stored in the model
     * \param w   weights in the Lagrangian
     * \param H   reference that stores the Hessian
     */
    void compute_H(std::vector<double>& x, std::vector<double>& w, std::vector<double>& H)
    {
        set_variable_view(x);
        compute_H(w, H);
    }

    /**
     * Compute constraint values
     *
     * This method uses the variable values stored in the model.
     *
     * \param c   reference that stores the constraint values
     */
    void compute_c(std::vector<double>& c);
    /**
     * Compute constraint values
     *
     * This method stores the variable values in the model.
     *
     * \param x   variable values that are stored in the model
     * \param c   reference that stores the constraint values
     */
    void compute_c(std::vector<double>& x, std::vector<double>& c)
    {
        set_variable_view(x);
        compute_c(c);
    }

    /**
     * Compute the gradient of the i-th constraint function.
     *
     * This method uses the variable values stored in the model.
     *
     * \param dc   reference that stores the gradient
     * \param i   constraint index
     */
    void compute_dc(std::vector<double>& dc, size_t i);
    /**
     * Compute the gradient of the i-th constraint function.
     *
     * This method stores the variable values in the model.
     *
     * \param x   variable values that are stored in the model
     * \param dc   reference that stores the gradient
     * \param i   constraint index
     */
    void compute_dc(std::vector<double>& x, std::vector<double>& dc, size_t i)
    {
        set_variable_view(x);
        compute_dc(dc, i);
    }

    /**
     * Compute the Jacobian
     *
     * This method uses the variable values stored in the model.
     *
     * \param J   reference that stores the Jacobian
     */
    void compute_J(std::vector<double>& J);
    /**
     * Compute the Jacobian
     *
     * This method stores the variable values in the model.
     *
     * \param x   variable values that are stored in the model
     * \param J   reference that stores the Jacobian
     */
    void compute_J(std::vector<double>& x, std::vector<double>& J)
    {
        set_variable_view(x);
        compute_J(J);
    }
};

//
// operator<<
//
std::ostream& operator<<(std::ostream& ostr, const NLPModel& arg);
}  // namespace coek
