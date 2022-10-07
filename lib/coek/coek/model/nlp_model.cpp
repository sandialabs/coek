#include "coek/model/nlp_model.hpp"

#include "coek/api/constraint.hpp"
#include "coek/api/objective.hpp"
#include "coek/autograd/autograd.hpp"
#include "model_repn.hpp"

namespace coek {

//
// NLPModel
//

NLPModel::NLPModel()
{
    std::shared_ptr<NLPModelRepn> tmp(create_NLPModelRepn("default"));
    repn = tmp;
}

NLPModel::NLPModel(Model& model, std::string type, bool sparse_JH)
{
    initialize(model, type, sparse_JH);
}

void NLPModel::initialize(Model& model, std::string type, bool sparse_JH)
{
    std::shared_ptr<NLPModelRepn> tmp(create_NLPModelRepn(model, type));
    repn = tmp;
    repn->initialize(sparse_JH);
}

void NLPModel::reset() { repn->reset(); }

size_t NLPModel::num_variables() const { return repn->num_variables(); }

size_t NLPModel::num_objectives() const { return repn->num_objectives(); }

size_t NLPModel::num_constraints() const { return repn->num_constraints(); }

size_t NLPModel::num_nonzeros_Jacobian() const { return repn->num_nonzeros_Jacobian(); }

size_t NLPModel::num_nonzeros_Hessian_Lagrangian() const
{
    return repn->num_nonzeros_Hessian_Lagrangian();
}

Variable NLPModel::get_variable(size_t i) { return repn->get_variable(i); }

void NLPModel::set_variable_view(std::vector<double>& x) { repn->set_variables(x); }

void NLPModel::set_variable_view(const double* x, size_t n) { repn->set_variables(x, n); }

Objective NLPModel::get_objective(size_t i) { return repn->get_objective(i); }

Constraint NLPModel::get_constraint(size_t i) { return repn->get_constraint(i); }

void NLPModel::get_J_nonzeros(std::vector<size_t>& jrow, std::vector<size_t>& jcol)
{
    repn->get_J_nonzeros(jrow, jcol);
}

void NLPModel::get_H_nonzeros(std::vector<size_t>& hrow, std::vector<size_t>& hcol)
{
    repn->get_H_nonzeros(hrow, hcol);
}

double NLPModel::compute_f(size_t i) { return repn->compute_f(i); }

void NLPModel::compute_df(double& f, std::vector<double>& df, size_t i)
{
    repn->compute_df(f, df, i);
}

void NLPModel::compute_H(std::vector<double>& w, std::vector<double>& H) { repn->compute_H(w, H); }

void NLPModel::compute_c(std::vector<double>& c) { repn->compute_c(c); }

void NLPModel::compute_dc(std::vector<double>& dc, size_t i) { repn->compute_dc(dc, i); }

void NLPModel::compute_J(std::vector<double>& J) { repn->compute_J(J); }

void NLPModel::write(std::string fname)
{
    std::map<size_t, size_t> varmap;
    std::map<size_t, size_t> conmap;
    write(fname, varmap, conmap);
}

void NLPModel::write(std::string fname, std::map<size_t, size_t>& varmap,
                     std::map<size_t, size_t>& conmap)
{
    repn->model.write(fname, varmap, conmap);
}

// GCOVR_EXCL_START
void NLPModel::print_equations() const { print_equations(std::cout); }

void NLPModel::print_values() const { print_values(std::cout); }
// GCOVR_EXCL_STOP

void NLPModel::print_equations(std::ostream& ostr) const { repn->print_equations(ostr); }

void NLPModel::print_values(std::ostream& ostr) const { repn->print_values(ostr); }

std::ostream& operator<<(std::ostream& ostr, const NLPModel& arg)
{
    arg.print_equations(ostr);
    return ostr;
}

}  // namespace coek
