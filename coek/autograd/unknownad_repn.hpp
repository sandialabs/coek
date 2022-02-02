#pragma once

#include <string>
#include "autograd.hpp"


namespace coek {

//
// An extension model that uses the CppAD library for autograd.
//
class UnknownAD_Repn : public NLPModelRepn
{
public:

    std::string ad_type;

public:

    UnknownAD_Repn(const std::string& ad_type_) : ad_type(ad_type_) {}

    void initialize(bool /*sparse_JH*/=true) {}     // GCOVR_EXCL_LINE

    void reset(void) {}

    size_t num_variables() const
        { throw std::runtime_error("Error accessing uninitialized NLPModel"); }

    size_t num_objectives() const
        { throw std::runtime_error("Error accessing uninitialized NLPModel"); }

    size_t num_constraints() const
        { throw std::runtime_error("Error accessing uninitialized NLPModel"); }

    size_t num_nonzeros_Jacobian() const
        { throw std::runtime_error("Error accessing uninitialized NLPModel"); }

    size_t num_nonzeros_Hessian_Lagrangian() const
        { throw std::runtime_error("Error accessing uninitialized NLPModel"); }

    void set_variables(std::vector<double>& )
        { throw std::runtime_error("Error accessing uninitialized NLPModel"); }
    void set_variables(const double* , size_t )
        { throw std::runtime_error("Error accessing uninitialized NLPModel"); }

    void get_J_nonzeros(std::vector<size_t>& , std::vector<size_t>& )
        { throw std::runtime_error("Error accessing uninitialized NLPModel"); }
    void get_H_nonzeros(std::vector<size_t>& , std::vector<size_t>& )
        { throw std::runtime_error("Error accessing uninitialized NLPModel"); }

    void print_equations(std::ostream& ) const
        { throw std::runtime_error("Error accessing uninitialized NLPModel"); }
    void print_values(std::ostream& ) const
        { throw std::runtime_error("Error accessing uninitialized NLPModel"); }

public:

    double compute_f(unsigned int )
        { throw std::runtime_error("Error accessing uninitialized NLPModel"); }

    void compute_df(double& , std::vector<double>& , unsigned int )
        { throw std::runtime_error("Error accessing uninitialized NLPModel"); }

    void compute_c(std::vector<double>& )
        { throw std::runtime_error("Error accessing uninitialized NLPModel"); }

    void compute_dc(std::vector<double>& , unsigned int )
        { throw std::runtime_error("Error accessing uninitialized NLPModel"); }

    void compute_H(std::vector<double>& , std::vector<double>& )
        { throw std::runtime_error("Error accessing uninitialized NLPModel"); }

    void compute_J(std::vector<double>& )
        { throw std::runtime_error("Error accessing uninitialized NLPModel"); }

/*
public:

    //void build_expression(expr_pointer_t root, std::vector<CppAD::AD<double> >& ADvars, CppAD::AD<double>& range, std::unordered_map<VariableTerm*,int>& _used_variables);

*/
};

}
