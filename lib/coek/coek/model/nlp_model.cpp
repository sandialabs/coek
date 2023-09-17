#include "coek/model/nlp_model.hpp"

#include "coek/api/constraint.hpp"
#include "coek/api/objective.hpp"
#include "coek/autograd/autograd.hpp"
#include "model_repn.hpp"

namespace coek {

//
// OptionCacheRepn
//

class OptionCacheRepn {
   public:
    std::map<std::string, std::string> string_options;
    std::map<std::string, int> integer_options;
    std::map<std::string, double> double_options;
};

//
// OptionCache
//

OptionCache::OptionCache() { options = std::make_shared<OptionCacheRepn>(); }

bool OptionCache::get_option(const std::string& option, int& value) const
{
    auto it = options->integer_options.find(option);
    if (it == options->integer_options.end()) return false;
    value = it->second;
    return true;
}

bool OptionCache::get_option(const std::string& option, double& value) const
{
    auto it = options->double_options.find(option);
    if (it == options->double_options.end()) return false;
    value = it->second;
    return true;
}

bool OptionCache::get_option(const std::string& option, std::string& value) const
{
    auto it = options->string_options.find(option);
    if (it == options->string_options.end()) return false;
    value = it->second;
    return true;
}

void OptionCache::set_option(const std::string& option, int value)
{
    options->integer_options[option] = value;
}

void OptionCache::set_option(const std::string& option, double value)
{
    options->double_options[option] = value;
}

void OptionCache::set_option(const std::string& option, const std::string value)
{
    options->string_options[option] = value;
}

//
// NLPModel
//

NLPModel::NLPModel()
{
    std::shared_ptr<NLPModelRepn> tmp(create_NLPModelRepn("default"));
    repn = tmp;
}

NLPModel::NLPModel(Model& model, std::string type) { initialize(model, type); }

void NLPModel::initialize(Model& model, std::string type)
{
    std::shared_ptr<NLPModelRepn> tmp(create_NLPModelRepn(model, type));
    repn = tmp;

    for (const auto& ioption : options->integer_options)
        repn->set_option(ioption.first, ioption.second);
    for (const auto& doption : options->double_options)
        repn->set_option(doption.first, doption.second);
    for (const auto& soption : options->string_options)
        repn->set_option(soption.first, soption.second);

    repn->initialize();
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

bool NLPModel::has_constraint_lower(size_t i) { return repn->has_constraint_lower(i); }
bool NLPModel::has_constraint_upper(size_t i) { return repn->has_constraint_upper(i); }
double NLPModel::get_constraint_lower(size_t i) { return repn->get_constraint_lower(i); }
double NLPModel::get_constraint_upper(size_t i) { return repn->get_constraint_upper(i); }

void NLPModel::get_J_nonzeros(std::vector<size_t>& jrow, std::vector<size_t>& jcol)
{
    repn->get_J_nonzeros(jrow, jcol);
}

void NLPModel::get_H_nonzeros(std::vector<size_t>& hrow, std::vector<size_t>& hcol)
{
    repn->get_H_nonzeros(hrow, hcol);
}

bool NLPModel::column_major_hessian() { return repn->column_major_hessian(); }

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
