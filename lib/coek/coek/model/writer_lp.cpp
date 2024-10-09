#include <cmath>
#include <exception>
#include <fstream>
#include <map>
#include <string>

#ifdef WITH_CALIPER
#    include <caliper/cali.h>
#else
#    define CALI_CXX_MARK_FUNCTION
#    define CALI_MARK_BEGIN(X)
#    define CALI_MARK_END(X)
#endif
#ifdef WITH_FMTLIB
#    include <fmt/compile.h>
#    include <fmt/core.h>
#    include <fmt/os.h>
#endif

#include "../ast/value_terms.hpp"
#include "../ast/visitor_fns.hpp"
#include "coek/api/expression.hpp"
#include "coek/api/constraint.hpp"
#include "coek/api/expression_visitor.hpp"
#include "coek/api/objective.hpp"
#include "coek/model/model.hpp"
#ifdef COEK_WITH_COMPACT_MODEL
#    include "coek/compact/constraint_sequence.hpp"
#    include "coek/compact/objective_sequence.hpp"
#    include "coek/model/compact_model.hpp"
#endif
#include "model_repn.hpp"

#define EPSILON 1e-12

namespace coek {

namespace {

inline size_t get_vid_value(const std::unordered_map<size_t, size_t>& vid, size_t id)
{
#if __cplusplus >= 201703L
    // C++-17
    if (auto it{vid.find(id)}; it != vid.end())
        return it->second;
#else
    auto it = vid.find(id);
    if (it != vid.end())
        return it->second;
#endif
    throw std::runtime_error(
        "Model expressions contain variable that is not declared in the model.");
}

//
// Print expression
//

void print_repn(std::ostream& ostr, const QuadraticExpr& repn,
                const std::unordered_map<size_t, size_t>& vid)
{
    CALI_CXX_MARK_FUNCTION;

    if (repn.linear_coefs.size() > 0) {
        std::map<size_t, double> vval;
        size_t i = 0;
        for (auto& it : repn.linear_vars) {
            vval[get_vid_value(vid, it->index)] += repn.linear_coefs[i];
            i++;
        }

        for (const auto& it : vval) {
            double tmp = it.second;
            if (tmp > 0)
                ostr << "+" << tmp << " x" << it.first << "\n";
            else if (tmp < 0)
                ostr << tmp << " x" << it.first << "\n";
        }
    }

    if (repn.quadratic_coefs.size() > 0) {
        std::map<std::pair<int, int>, double> qval;
        for (size_t ii = 0; ii < repn.quadratic_coefs.size(); ++ii) {
            size_t lindex = get_vid_value(vid, repn.quadratic_lvars[ii]->index);
            size_t rindex = get_vid_value(vid, repn.quadratic_rvars[ii]->index);

            if (lindex < rindex)
                qval[{lindex, rindex}] += repn.quadratic_coefs[ii];
            else
                qval[{rindex, lindex}] += repn.quadratic_coefs[ii];
        }

        ostr << "+ [\n";
        for (const auto& it : qval) {
            const std::pair<size_t, size_t>& tmp = it.first;
            double val = it.second;
            if (val != 0) {
                if (tmp.first == tmp.second) {
                    if (val > 0)
                        ostr << "+";
                    ostr << val << " x" << tmp.first << " ^ 2\n";
                }
                else {
                    if (val > 0)
                        ostr << "+";
                    ostr << val << " x" << tmp.first << " * x" << tmp.second << "\n";
                }
            }
        }
        ostr << "]\n";
    }
}

#ifdef WITH_FMTLIB
constexpr auto print_repn_fmt1 = FMT_COMPILE("{:+} x{}\n");
constexpr auto print_repn_fmt_x2 = FMT_COMPILE("{:+} x{} ^ 2\n");
constexpr auto print_repn_fmt_x_x = FMT_COMPILE("{:+} x{} * x{}\n");

void print_repn(fmt::ostream& ostr, const QuadraticExpr& repn,
                const std::unordered_map<size_t, size_t>& vid)
{
    CALI_CXX_MARK_FUNCTION;

    if (repn.linear_coefs.size() > 0) {
        std::map<size_t, double> vval;
        size_t i = 0;
        for (auto& it : repn.linear_vars) {
            vval[get_vid_value(vid, it->index)] += repn.linear_coefs[i];
            i++;
        }

        for (const auto& it : vval) {
            double tmp = it.second;
            if (tmp != 0)
                ostr.print(fmt::format(print_repn_fmt1, tmp, it.first));
        }
    }

    if (repn.quadratic_coefs.size() > 0) {
        std::map<std::pair<int, int>, double> qval;
        for (size_t ii = 0; ii < repn.quadratic_coefs.size(); ++ii) {
            size_t lindex = get_vid_value(vid, repn.quadratic_lvars[ii]->index);
            size_t rindex = get_vid_value(vid, repn.quadratic_rvars[ii]->index);

            if (lindex < rindex)
                qval[{lindex, rindex}] += repn.quadratic_coefs[ii];
            else
                qval[{rindex, lindex}] += repn.quadratic_coefs[ii];
        }

        ostr.print("+ [\n");
        for (const auto& it : qval) {
            const std::pair<int, int>& tmp = it.first;
            double val = it.second;
            if (val != 0) {
                if (tmp.first == tmp.second)
                    ostr.print(fmt::format(print_repn_fmt_x2, val, tmp.first));
                else
                    ostr.print(fmt::format(print_repn_fmt_x_x, val, tmp.first, tmp.second));
            }
        }
        ostr.print("]\n");
    }
}
#endif

class LPWriter {
   public:
#ifdef COEK_WITH_COMPACT_MODEL
    Model model_handle;
#endif
    bool one_var_constant;
    std::unordered_map<size_t, size_t> vid;
    std::vector<Variable> variables;
    std::map<size_t, Variable> bvars;
    std::map<size_t, Variable> ivars;
    std::map<size_t, size_t>& invvarmap;
    std::map<size_t, size_t>& invconmap;

    QuadraticExpr expr;

    LPWriter(std::map<size_t, size_t>& _invvarmap, std::map<size_t, size_t>& _invconmap)
        : one_var_constant(false), invvarmap(_invvarmap), invconmap(_invconmap)
    {
    }

    template <class StreamType, class ModelType>
    void write(StreamType& ostr, ModelType& model);

    template <class StreamType>
    void print_objectives(StreamType& ostr, Model& model);
    template <class StreamType>
    void print_constraints(StreamType& ostr, Model& model);
    void collect_variables(Model& model);

#ifdef COEK_WITH_COMPACT_MODEL
    template <class StreamType>
    void print_objectives(StreamType& ostr, CompactModel& model);
    template <class StreamType>
    void print_constraints(StreamType& ostr, CompactModel& model);
    void collect_variables(CompactModel& model);
#endif

    void print_header(std::ostream& ostr);
    void print_objective(std::ostream& ostr, const Objective& obj);
    void print_st(std::ostream& ostr);
    void print_constraint(std::ostream& ostr, const Constraint& c, size_t ctr);
    void print_bounds(std::ostream& ostr);

#ifdef WITH_FMTLIB
    void print_header(fmt::ostream& ostr);
    void print_objective(fmt::ostream& ostr, const Objective& obj);
    void print_st(fmt::ostream& ostr);
    void print_constraint(fmt::ostream& ostr, const Constraint& c, size_t ctr);
    void print_bounds(fmt::ostream& ostr);
#endif
};

//
// Print objectives
//

template <class StreamType>
void LPWriter::print_objectives(StreamType& ostr, Model& model)
{
    CALI_CXX_MARK_FUNCTION;

    unsigned int n_obj = 0;
    for (auto& obj : model.repn->objectives) {
        if (obj.active()) {
            print_objective(ostr, obj);
            ++n_obj;
        }
    }
    if (n_obj == 0) {
        throw std::runtime_error("Error writing LP file: No objectives specified!");
    }
    if (n_obj > 1) {
        throw std::runtime_error("Error writing LP file: More than one objective defined!");
    }
}

#ifdef COEK_WITH_COMPACT_MODEL
template <class StreamType>
void LPWriter::print_objectives(StreamType& ostr, CompactModel& model)
{
    CALI_CXX_MARK_FUNCTION;

    unsigned int n_obj = 0;
    for (auto& val : model.repn->objectives) {
        if (auto eval = std::get_if<Objective>(&val)) {
            auto obj = objective().expr(eval->expr().expand()).sense(eval->sense());
            print_objective(ostr, obj);
            ++n_obj;
        }
        else if (auto eval = std::get_if<ObjectiveSequence>(&val)) {
            for (auto& jt : *eval) {
                print_objective(ostr, jt);
                ++n_obj;
            }
        }
    }
    if (n_obj == 0) {
        throw std::runtime_error("Error writing LP file: No objectives specified!");
    }
    if (n_obj > 1) {
        throw std::runtime_error("Error writing LP file: More than one objective defined!");
    }
}
#endif

template <class StreamType>
void LPWriter::print_constraints(StreamType& ostr, Model& model)
{
    size_t ctr = 0;
    for (auto& it : model.repn->constraints) {
        invconmap[it.id()] = ctr;
        print_constraint(ostr, it, ctr);
        ++ctr;
    }
}

#ifdef COEK_WITH_COMPACT_MODEL
template <class StreamType>
void LPWriter::print_constraints(StreamType& ostr, CompactModel& model)
{
    size_t ctr = 0;
    for (auto& val : model.repn->constraints) {
        if (auto cval = std::get_if<Constraint>(&val)) {
            Constraint c = cval->expand();
            invconmap[c.id()] = ctr;
            print_constraint(ostr, c, ctr);
            ++ctr;
        }
        else if (auto cval = std::get_if<ConstraintSequence>(&val)) {
            for (auto& jt : *cval) {
                invconmap[jt.id()] = ctr;
                print_constraint(ostr, jt, ctr);
                ++ctr;
            }
        }
        else if (auto cval = std::get_if<ConstraintMap>(&val)) {
            //for (auto jt=cval->begin(); jt != cval->end(); ++jt) {
            for (auto& [k,v] : *cval) {
                invconmap[v.id()] = ctr;
                print_constraint(ostr, v, ctr);
                ++ctr;
            }
        }
    }
}
#endif

//
// Collect variables
//

void LPWriter::collect_variables(Model& model)
{
    size_t ctr = 0;
    for (auto& it : model.repn->variables) {
        vid[it.id()] = ctr;
        invvarmap[ctr] = it.id();
        ++ctr;

        auto v = it.repn;
        if (v->fixed)  // Don't report fixed binary or integer variables
            continue;
        variables.push_back(it);
        if (v->binary)
            bvars[vid[v->index]] = v;
        if (v->integer)
            ivars[vid[v->index]] = v;
    }
}

#ifdef COEK_WITH_COMPACT_MODEL
void LPWriter::collect_variables(CompactModel& model)
{
    model_handle = model.expand_data();
    size_t ctr = 0;
    for (auto& val : model.repn->variables) {
        if (auto eval = std::get_if<Variable>(&val)) {
            if (eval->fixed())
                continue;
            Expression lb = eval->lower_expression().expand();
            Expression ub = eval->upper_expression().expand();
            Expression value = eval->value_expression().expand();
            auto tmp = variable(eval->name())
                           .lower(lb.value())
                           .upper(ub.value())
                           .value(value.value())
                           .within(eval->within());
            variables.push_back(tmp);
            if (tmp.is_binary())
                bvars[vid[tmp.id()]] = tmp.repn;
            if (tmp.is_integer())
                ivars[vid[tmp.id()]] = tmp.repn;

            vid[eval->id()] = ctr;
            invvarmap[ctr] = variables.size();
            ++ctr;
        }
        else if (auto eval = std::get_if<VariableSequence>(&val)) {
            for (auto& jt : *eval) {
                if (jt.fixed())
                    continue;
                variables.push_back(jt);
                if (jt.is_binary())
                    bvars[vid[jt.id()]] = jt.repn;
                if (jt.is_integer())
                    ivars[vid[jt.id()]] = jt.repn;

                vid[jt.id()] = ctr;
                invvarmap[ctr] = variables.size();
                ++ctr;
            }
        }
        else if (auto eval = std::get_if<VariableMap>(&val)) {
            eval->expand();
            for (auto& jt : *eval) {
                if (jt.fixed())
                    continue;
                variables.push_back(jt);
                if (jt.is_binary())
                    bvars[vid[jt.id()]] = jt.repn;
                if (jt.is_integer())
                    ivars[vid[jt.id()]] = jt.repn;

                vid[jt.id()] = ctr;
                invvarmap[ctr] = variables.size();
                ++ctr;
            }
        }
        else if (auto eval = std::get_if<VariableArray>(&val)) {
            eval->expand();
            for (auto& jt : *eval) {
                if (jt.fixed())
                    continue;
                variables.push_back(jt);
                if (jt.is_binary())
                    bvars[vid[jt.id()]] = jt.repn;
                if (jt.is_integer())
                    ivars[vid[jt.id()]] = jt.repn;

                vid[jt.id()] = ctr;
                invvarmap[ctr] = variables.size();
                ++ctr;
            }
        }
    }
}
#endif

//
// Main writer
//

template <class StreamType, class ModelType>
void LPWriter::write(StreamType& ostr, ModelType& model)
{
    CALI_CXX_MARK_FUNCTION;

    // Create variable ID map
    collect_variables(model);

    // Create file
    print_header(ostr);

    try {
        print_objectives(ostr, model);

        print_st(ostr);

        print_constraints(ostr, model);

        print_bounds(ostr);
    }
    catch (std::exception& e) {
        throw std::runtime_error(std::string("Error writing LP file: ") + e.what());
    }
}

//
//
// std::ostream print methods
//
//
void LPWriter::print_header(std::ostream& ostr) { ostr << "\\* LP File Generated by COEK *\\\n\n"; }

void LPWriter::print_objective(std::ostream& ostr, const Objective& obj)
{
    if (obj.sense())
        ostr << "\nminimize\n\n";
    else
        ostr << "\nmaximize\n\n";
    ostr << "obj:\n";

    expr.reset();
    expr.collect_terms(obj);
    print_repn(ostr, expr, vid);
    double tmp = expr.constval;
    if (tmp != 0) {
        one_var_constant = true;
        if (tmp > 0)
            ostr << "+";
        ostr << tmp << " ONE_VAR_CONSTANT\n";
    }
}

void LPWriter::print_st(std::ostream& ostr) { ostr << "\nsubject to\n\n"; }

void LPWriter::print_constraint(std::ostream& ostr, const Constraint& c, size_t ctr)
{
    CALI_CXX_MARK_FUNCTION;

    CALI_MARK_BEGIN("collect_terms");
    expr.reset();
    expr.collect_terms(c);
    double tmp = expr.constval;

    auto lower = c.lower();
    auto upper = c.upper();

    ostr << "c" << ctr << ":\n";
    ++ctr;
    CALI_MARK_END("collect_terms");

    bool is_equality
        = not c.is_inequality()
          or (lower.repn and upper.repn and (::fabs(lower.value() - upper.value()) < EPSILON));

    if (not is_equality) {
        // CALI_MARK_BEGIN("IF");
        if (lower.repn) {
            ostr << lower.value() - tmp;
            ostr << " <= ";
        }
        print_repn(ostr, expr, vid);
        if (upper.repn) {
            ostr << " <= ";
            ostr << upper.value() - tmp;
        }
        ostr << "\n\n";
        // CALI_MARK_END("IF");
    }
    else {
        print_repn(ostr, expr, vid);
        CALI_MARK_BEGIN("ELSE");
        ostr << "= ";
        ostr << lower.value() - tmp;
        ostr << "\n\n";
        CALI_MARK_END("ELSE");
    }
}

void LPWriter::print_bounds(std::ostream& ostr)
{
    if (one_var_constant) {
        ostr << "c_ONE_VAR_CONSTANT:\n";
        ostr << "ONE_VAR_CONSTANT = 1.0\n";
        ostr << "\n";
    }

    ostr << "\nbounds\n";
    for (auto& v : variables) {
        if (v.is_binary())  // TODO - Confirm that this optimization is helpful
            continue;

        auto lb = v.lower();
        if (lb <= -COEK_INFINITY)
            ostr << "-inf";
        else
            ostr << lb;
        ostr << " <= x" << vid[v.id()] << " <= ";
        auto ub = v.upper();
        if (ub >= COEK_INFINITY)
            ostr << "inf\n";
        else
            ostr << ub << '\n';
    }

    if (bvars.size() > 0) {
        ostr << "\nbinary\n";
        for (auto& it : bvars)
            ostr << "x" << it.first << "\n";
    }

    if (ivars.size() > 0) {
        ostr << "\ninteger\n";
        for (auto& it : ivars)
            ostr << "x" << it.first << "\n";
    }

    ostr << "\nend\n";
}

#ifdef WITH_FMTLIB
//
//
// fmt::ostream print methods
//
//
void LPWriter::print_header(fmt::ostream& ostr)
{
    ostr.print("\\* LP File Generated by COEK *\\\n\n");
}

void LPWriter::print_objective(fmt::ostream& ostr, const Objective& obj)
{
    if (obj.sense())
        ostr.print("\nminimize\n\n");
    else
        ostr.print("\nmaximize\n\n");
    ostr.print("obj:\n");

    expr.reset();
    expr.collect_terms(obj);
    print_repn(ostr, expr, vid);
    double tmp = expr.constval;
    if (tmp != 0) {
        one_var_constant = true;
        ostr.print("{:+} ONE_VAR_CONSTANT\n", tmp);
    }
}

void LPWriter::print_st(fmt::ostream& ostr) { ostr.print("\nsubject to\n\n"); }

constexpr auto con_fmt = FMT_COMPILE("c{}:\n");
constexpr auto lt_fmt1 = FMT_COMPILE("{} <= ");
constexpr auto lt_fmt2 = FMT_COMPILE(" <= {}");
constexpr auto eq_fmt = FMT_COMPILE("= {}\n\n");

void LPWriter::print_constraint(fmt::ostream& ostr, const Constraint& c, size_t ctr)
{
    CALI_CXX_MARK_FUNCTION;

    CALI_MARK_BEGIN("collect_terms");
    expr.reset();
    expr.collect_terms(c);
    double tmp = expr.constval;

    auto lower = c.lower();
    auto upper = c.upper();

    ostr.print(fmt::format(con_fmt, ctr));
    ++ctr;
    CALI_MARK_END("collect_terms");

    bool is_equality
        = not c.is_inequality()
          or (lower.repn and upper.repn and (::fabs(lower.value() - upper.value()) < EPSILON));

    if (not is_equality) {
        // CALI_MARK_BEGIN("IF");
        if (lower.repn) {
            ostr.print(fmt::format(lt_fmt1, lower.value() - tmp));
        }
        print_repn(ostr, expr, vid);
        if (upper.repn) {
            ostr.print(fmt::format(lt_fmt2, upper.value() - tmp));
        }
        ostr.print("\n\n");
        // CALI_MARK_END("IF");
    }
    else {
        print_repn(ostr, expr, vid);
        // CALI_MARK_BEGIN("ELSE");
        ostr.print(fmt::format(eq_fmt, lower.value() - tmp));
        // CALI_MARK_END("ELSE");
    }
}

constexpr auto _fmt_x = FMT_COMPILE(" <= x{} <= ");
constexpr auto _fmt_lb = FMT_COMPILE("{}");
constexpr auto _fmt_ub = FMT_COMPILE("{}\n");
constexpr auto _fmt_xval = FMT_COMPILE("x{}\n");

void LPWriter::print_bounds(fmt::ostream& ostr)
{
    if (one_var_constant) {
        ostr.print("c_ONE_VAR_CONSTANT:\n");
        ostr.print("ONE_VAR_CONSTANT = 1.0\n");
        ostr.print("\n");
    }

    ostr.print("\nbounds\n");
    for (auto& v : variables) {
        if (v.is_binary())  // TODO - Confirm that this optimization is helpful
            continue;

        auto lb = v.lower();
        if (lb <= -COEK_INFINITY)
            ostr.print("-inf");
        else
            ostr.print(fmt::format(_fmt_lb, lb));
        ostr.print(fmt::format(_fmt_x, vid[v.id()]));
        auto ub = v.upper();
        if (ub >= COEK_INFINITY)
            ostr.print("inf\n");
        else
            ostr.print(fmt::format(_fmt_ub, ub));
    }

    if (bvars.size() > 0) {
        ostr.print("\nbinary\n");
        for (auto& it : bvars)
            ostr.print(fmt::format(_fmt_xval, it.first));
    }

    if (ivars.size() > 0) {
        ostr.print("\ninteger\n");
        for (auto& it : ivars)
            ostr.print(fmt::format(_fmt_xval, it.first));
    }

    ostr.print("\nend\n");  // << std::endl;
}
#endif

}  // namespace

void write_lp_problem_ostream(Model& model, const std::string& fname,
                              std::map<size_t, size_t>& invvarmap,
                              std::map<size_t, size_t>& invconmap)
{
    std::ofstream ostr(fname);
    LPWriter writer(invvarmap, invconmap);
    try {
        writer.write(ostr, model);
        ostr.close();
    }
    catch (std::exception& e) {
        ostr.close();
        throw;
    }
}

#ifdef COEK_WITH_COMPACT_MODEL
void write_lp_problem_ostream(CompactModel& model, const std::string& fname,
                              std::map<size_t, size_t>& invvarmap,
                              std::map<size_t, size_t>& invconmap)
{
    std::ofstream ostr(fname);
    LPWriter writer(invvarmap, invconmap);
    try {
        writer.write(ostr, model);
        ostr.close();
    }
    catch (std::exception& e) {
        ostr.close();
        throw;
    }
}
#endif

#ifdef WITH_FMTLIB
void write_lp_problem_fmtlib(Model& model, const std::string& fname,
                             std::map<size_t, size_t>& invvarmap,
                             std::map<size_t, size_t>& invconmap)
{
    auto ostr = fmt::output_file(fname, fmt::file::WRONLY | fmt::file::CREATE | FMT_POSIX(O_TRUNC));
    LPWriter writer(invvarmap, invconmap);
    try {
        writer.write(ostr, model);
        ostr.close();
    }
    catch (std::exception& e) {
        ostr.close();
        throw;
    }
}

void write_lp_problem(Model& model, const std::string& fname, std::map<size_t, size_t>& invvarmap,
                      std::map<size_t, size_t>& invconmap)
{
    write_lp_problem_fmtlib(model, fname, invvarmap, invconmap);
}

#    ifdef COEK_WITH_COMPACT_MODEL
void write_lp_problem_fmtlib(CompactModel& model, const std::string& fname,
                             std::map<size_t, size_t>& invvarmap,
                             std::map<size_t, size_t>& invconmap)
{
    auto ostr = fmt::output_file(fname, fmt::file::WRONLY | fmt::file::CREATE | FMT_POSIX(O_TRUNC));
    LPWriter writer(invvarmap, invconmap);
    try {
        writer.write(ostr, model);
        ostr.close();
    }
    catch (std::exception& e) {
        ostr.close();
        throw;
    }
}

void write_lp_problem(CompactModel& model, const std::string& fname,
                      std::map<size_t, size_t>& varmap, std::map<size_t, size_t>& conmap)
{
    write_lp_problem_fmtlib(model, fname, varmap, conmap);
}
#    endif
#else

void write_lp_problem(Model& model, const std::string& fname, std::map<size_t, size_t>& invvarmap,
                      std::map<size_t, size_t>& invconmap)
{
    write_lp_problem_ostream(model, fname, invvarmap, invconmap);
}

#    ifdef COEK_WITH_COMPACT_MODEL
void write_lp_problem(CompactModel& model, const std::string& fname,
                      std::map<size_t, size_t>& varmap, std::map<size_t, size_t>& conmap)
{
    write_lp_problem_ostream(model, fname, varmap, conmap);
}
#    endif
#endif

}  // namespace coek
