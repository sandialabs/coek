#include <map>
#include <exception>
#include <string>
#include <fstream>

#include "../ast/visitor_fns.hpp"
#include "../ast/value_terms.hpp"
#include "coek/api/expression.hpp"
#include "coek/api/objective.hpp"
#include "coek/api/constraint.hpp"
#include "coek/api/expression_visitor.hpp"
#include "coek/compact/objective_sequence.hpp"
#include "coek/compact/constraint_sequence.hpp"
#include "coek/coek_model.hpp"
#ifdef WITH_CALIPER
#include <caliper/cali.h>
#else
#define CALI_CXX_MARK_FUNCTION
#define CALI_MARK_BEGIN(X)
#define CALI_MARK_END(X)
#endif
#ifdef WITH_FMTLIB
#include <fmt/core.h>
#include <fmt/os.h>
#endif

namespace coek {

namespace {


struct UnknownVariable : public std::exception
{
	const char * what () const throw ()
    {
    	return "Unknown variable encountered";
    }
};

inline unsigned int get_vid_value(const std::unordered_map<unsigned int,unsigned int>& vid, unsigned int id)
{
if (auto it{ vid.find(id) };  it != vid.end() )
    return it->second;
throw UnknownVariable();
}


void print_repn(std::ostream& ostr, const QuadraticExpr& repn, const std::unordered_map<unsigned int,unsigned int>& vid)
{
CALI_CXX_MARK_FUNCTION;

if (repn.linear_coefs.size() > 0) {
    std::map<unsigned int,double> vval;
    unsigned int i=0;
    for (std::vector<VariableTerm*>::const_iterator it=repn.linear_vars.begin(); it != repn.linear_vars.end(); ++it, ++i) {
        unsigned int index = get_vid_value(vid, (*it)->index);

        std::map<unsigned int,double>::iterator curr = vval.find(index);
        if (curr == vval.end())
            vval[index ] = repn.linear_coefs[i];
        else
            vval[index ] += repn.linear_coefs[i];
        }

    for (std::map<unsigned int,double>::iterator it=vval.begin(); it != vval.end(); ++it) {
        i = it->first;
        double tmp = it->second;
        if (tmp > 0)
            ostr << "+" << tmp << " x(" << i << ")\n";
        else if (tmp < 0)
            ostr << tmp << " x(" << i << ")\n";
        }
    }

if (repn.quadratic_coefs.size() > 0) {
    std::map<std::pair<int,int>,double> qval;
    for (size_t ii=0; ii<repn.quadratic_coefs.size(); ++ii) {
        unsigned int lindex = get_vid_value(vid, repn.quadratic_lvars[ii]->index);
        unsigned int rindex = get_vid_value(vid, repn.quadratic_rvars[ii]->index);

        std::pair<int,int> tmp;
        if (lindex < rindex)
            tmp = std::pair<unsigned int,unsigned int>(lindex, rindex);
        else
            tmp = std::pair<unsigned int,unsigned int>(rindex, lindex);

        auto curr = qval.find(tmp);
        if (curr == qval.end())
            qval[ tmp ] = repn.quadratic_coefs[ii];
        else
            qval[ tmp ] += repn.quadratic_coefs[ii];
        }

    ostr << "+ [\n";
    for (auto it=qval.begin(); it != qval.end(); ++it) {
        const std::pair<unsigned int,unsigned int>& tmp = it->first;
        double val = it->second;
        if (tmp.first == tmp.second) {
            if (val > 0)
                ostr << "+" << val << " x(" << tmp.first << ") ^ 2\n";
            else if (val < 0)
                ostr << val << " x(" << tmp.first << ") ^ 2\n";
            }
        else {
            if (val > 0)
                ostr << "+" << val << " x(" << tmp.first << ") * x(" << tmp.second << ")\n";
            else if (val < 0)
                ostr << val << " x(" << tmp.first << ") * x(" << tmp.second << ")\n";
            }
        }
    ostr << "]\n";
    }
}

void print_objective(std::ostream& ostr, const Objective& obj, bool& one_var_constant, const std::unordered_map<unsigned int,unsigned int>& vid)
{
CALI_CXX_MARK_FUNCTION;

QuadraticExpr expr;
expr.collect_terms(obj);
print_repn(ostr, expr, vid);
double tmp = expr.constval;
if (tmp != 0) {
    one_var_constant=true;
    if (tmp > 0)
        ostr << "+";
    ostr << tmp << " ONE_VAR_CONSTANT\n";
    }
}

void print_constraint(std::ostream& ostr, const Constraint& c, unsigned int ctr, const std::unordered_map<unsigned int,unsigned int>& vid)
{
CALI_CXX_MARK_FUNCTION;

CALI_MARK_BEGIN("collect_terms");
QuadraticExpr expr;
expr.collect_terms(c);
double tmp = expr.constval;

auto lower = c.lower();
auto upper = c.upper();

ostr << "c" << ctr << ":\n";
++ctr;
CALI_MARK_END("collect_terms");
if (c.is_inequality()) {
    //CALI_MARK_BEGIN("IF");
    if (lower.repn) {
        ostr << lower.get_value() - tmp;
        ostr << " <= ";
        }
    print_repn(ostr, expr, vid);
    if (upper.repn) {
        ostr << " <= ";
        ostr << upper.get_value() - tmp;
        }
    ostr << "\n\n";
    //CALI_MARK_END("IF");
    }
else {
    print_repn(ostr, expr, vid);
    CALI_MARK_BEGIN("ELSE");
    ostr << "= ";
    ostr << lower.get_value() - tmp;
    ostr << "\n\n";
    CALI_MARK_END("ELSE");
    }
}

}

void write_lp_problem_ostream(Model& model, std::string& fname, std::map<int,int>& invvarmap, std::map<int,int>& invconmap)
{
std::ofstream ostr(fname);

CALI_CXX_MARK_FUNCTION;

if (model.repn->objectives.size() == 0) {
    std::cerr << "Error writing LP file: No objectives specified!" << std::endl;
    return;
    }
if (model.repn->objectives.size() > 1) {
    std::cerr << "Error writing LP file: More than one objective defined!" << std::endl;
    return;
    }

// Create variable ID map
std::unordered_map<unsigned int,unsigned int> vid;
{
unsigned int ctr=0;
for(std::vector<Variable>::iterator it=model.repn->variables.begin(); it != model.repn->variables.end(); ++it) {
    vid[(*it).id()] = ctr;
    invvarmap[ctr] = (*it).id();
    ++ctr;
    }
}

// Create file
ostr << "\\* LP File Generated by COEK *\\\n\n";
ostr << "\nminimize\n\n";

bool one_var_constant=false;
try {
    //
    // Objectives and Constraints
    //
    ostr << "obj:\n";
    print_objective(ostr, model.repn->objectives[0], one_var_constant, vid);

    ostr << "\nsubject to\n\n";
    unsigned int ctr=0;
    for (std::vector<Constraint>::iterator it=model.repn->constraints.begin(); it != model.repn->constraints.end(); ++it) {
        invconmap[it->id()] = ctr;
        print_constraint(ostr, *it, ctr, vid);
        ++ctr;
        }
    }
catch (UnknownVariable& e)
    {
    // TODO - More comprehensive error reporting here.
    throw std::runtime_error("Error writing LP file: Unexpected variable encountered in a model expression");
    }
catch (std::exception& e)
    {
    throw std::runtime_error(std::string("Unknown error writing LP file: ") + e.what());
    }

if (one_var_constant) {
    ostr << "c_ONE_VAR_CONSTANT:\n";
    ostr << "ONE_VAR_CONSTANT = 1.0\n";
    ostr << "\n";
    }

std::map<unsigned int,VariableTerm*> bvars;
std::map<unsigned int,VariableTerm*> ivars;
ostr << "\nbounds\n";
for(std::vector<Variable>::iterator it=model.repn->variables.begin(); it != model.repn->variables.end(); ++it) {
    VariableTerm* v = it->repn;
    if (v->lb <= -COEK_INFINITY)
        ostr << "-inf";
    else
        ostr << v->lb;
    ostr << " <= x(" << vid[v->index] << ") <= ";
    if (v->ub >= COEK_INFINITY)
        ostr << "inf\n";
    else
        ostr << v->ub << '\n';
    if (v->binary)
        bvars[vid[v->index]] = v;
    if (v->integer)
        ivars[vid[v->index]] = v;
    }

if (bvars.size() > 0) {
    ostr << "\nbinary\n";
    for(std::map<unsigned int,VariableTerm*>::iterator it=bvars.begin(); it != bvars.end(); ++it)
        ostr << "x(" << it->first << ")\n";
    }

if (ivars.size() > 0) {
    ostr << "\ninteger\n";
    for(std::map<unsigned int,VariableTerm*>::iterator it=ivars.begin(); it != ivars.end(); ++it)
        ostr << "x(" << it->first << ")\n";
    }

ostr << "\nend\n";
ostr.close();
}


void write_lp_problem_ostream(CompactModel& model, std::string& fname, std::map<int,int>& varmap, std::map<int,int>& conmap)
{
std::ofstream ostr(fname);

CALI_CXX_MARK_FUNCTION;

if (model.objectives.size() == 0) {
    std::cerr << "Error writing LP file: No objectives specified!" << std::endl;
    return;
    }

// Create variable ID map
std::unordered_map<unsigned int,unsigned int> vid;
{
unsigned int ctr=0;
for(std::vector<Variable>::iterator it=model.variables.begin(); it != model.variables.end(); ++it) {
    vid[(*it).id()] = ctr;
    varmap[ctr] = (*it).id();
    ++ctr;
    }
}

// Create file

ostr << "\\* LP File Generated by COEK *\\\n\n";
ostr << "\nminimize\n\n";

bool one_var_constant=false;
try {
    ostr << "obj:\n";
    int nobj=0;

    for (auto it=model.objectives.begin(); it != model.objectives.end(); ++it) {
        auto& val = *it;
        if (std::get_if<Objective>(&val)) {
        //if (auto eval = std::get_if<Objective>(&val)) {
            /* WEH - TODO rework to work with objectives
            Objective obj = eval->expand();
            //Objective obj(e, model.sense[nobj]);
            print_objective(ostr, obj, one_var_constant, vid);
            ++nobj;
            */
            }
        else {
#if 0
            auto& seq = std::get<ExpressionSequence>(val);
            for (auto jt=seq.begin(); jt != seq.end(); ++jt) {
                print_objective(ostr, *jt, one_var_constant, vid);
                ++nobj;
                }
#endif
            std::cerr << "Compact models can only have a single objective!" << std::endl;
            return;
            }
        }
    if (nobj > 1) {
        std::cerr << "Error writing LP file: More than one objective defined!" << std::endl;
        return;
        }

    ostr << "\nsubject to\n\n";

    //
    // Simple contraints
    //
    unsigned int ctr=0;
    for (auto it=model.constraints.begin(); it != model.constraints.end(); ++it) {
        auto& val = *it;
        if (auto cval = std::get_if<Constraint>(&val)) {
            Constraint c = cval->expand();
            conmap[c.id()] = ctr;
            print_constraint(ostr, c, ctr, vid);
            ++ctr;
            }
        else {
            auto& seq = std::get<ConstraintSequence>(val);
            for (auto jt=seq.begin(); jt != seq.end(); ++jt) {
                conmap[jt->id()] = ctr;
                print_constraint(ostr, *jt, ctr, vid);
                ++ctr;
                }
            }
        }
    }
catch (UnknownVariable& e)
    {
    // TODO - More comprehensive error reporting here.
    throw std::runtime_error("Error writing LP file: Unexpected variable encountered in a model expression");
    }
catch (std::exception& e)
    {
    throw std::runtime_error(std::string("Unknown error writing LP file: ") + e.what());
    }

if (one_var_constant) {
    ostr << "c_ONE_VAR_CONSTANT:\n";
    ostr << "ONE_VAR_CONSTANT = 1.0\n";
    ostr << '\n';
    }

std::map<unsigned int,VariableTerm*> bvars;
std::map<unsigned int,VariableTerm*> ivars;
ostr << "\nbounds\n";
for(std::vector<Variable>::iterator it=model.variables.begin(); it != model.variables.end(); ++it) {
    VariableTerm* v = it->repn;
    if (v->lb <= -COEK_INFINITY)
        ostr << "-inf";
    else
        ostr << v->lb;
    ostr << " <= x(" << vid[v->index] << ") <= ";
    if (v->ub >= COEK_INFINITY)
        ostr << "inf\n";
    else
        ostr << v->ub << '\n';
    if (v->binary)
        bvars[vid[v->index]] = v;
    if (v->integer)
        ivars[vid[v->index]] = v;
    }

if (bvars.size() > 0) {
    ostr << "\nbinary\n";
    for(std::map<unsigned int,VariableTerm*>::iterator it=bvars.begin(); it != bvars.end(); ++it)
        ostr << "x(" << it->first << ")\n";
    }

if (ivars.size() > 0) {
    ostr << "\ninteger\n";
    for(std::map<unsigned int,VariableTerm*>::iterator it=ivars.begin(); it != ivars.end(); ++it)
        ostr << "x(" << it->first << ")\n";
    }

ostr << "\nend\n";
ostr.close();
}


#ifdef WITH_FMTLIB
void print_repn(fmt::ostream& ostr, const QuadraticExpr& repn, const std::unordered_map<unsigned int, unsigned int>& vid)
{
CALI_CXX_MARK_FUNCTION;

if (repn.linear_coefs.size() > 0) {
    std::map<unsigned int,double> vval;
    unsigned int i=0;
    for (auto it=repn.linear_vars.begin(); it != repn.linear_vars.end(); ++it, ++i) {
        unsigned int index = get_vid_value(vid, (*it)->index);

        if (auto it{ vval.find(index) };  it != vval.end() )
            it->second += repn.linear_coefs[i];
        else
            vval[index ] = repn.linear_coefs[i];
        }

    for (auto it=vval.begin(); it != vval.end(); ++it) {
        i = it->first;
        double tmp = it->second;
        //if (tmp > 0)
        //    ostr.print("+{} x({})\n", tmp, i);     // << "+" << tmp << " x(" << i << ")\n";
        //else if (tmp < 0)
        if (tmp != 0)
            ostr.print("{:+} x({})\n", tmp, i);      // << tmp << " x(" << i << ")\n";
        }
    }

if (repn.quadratic_coefs.size() > 0) {
    std::map<std::pair<int,int>,double> qval;
    for (size_t ii=0; ii<repn.quadratic_coefs.size(); ++ii) {
        unsigned int lindex = get_vid_value(vid, repn.quadratic_lvars[ii]->index);
        unsigned int rindex = get_vid_value(vid, repn.quadratic_rvars[ii]->index);

        std::pair<int,int> tmp;
        if (lindex < rindex)
            tmp = std::pair<unsigned int,unsigned int>(lindex, rindex);
        else
            tmp = std::pair<unsigned int,unsigned int>(rindex, lindex);

        if (auto it{ qval.find(tmp) };  it != qval.end() )
            it->second += repn.quadratic_coefs[ii];
        else
            qval[ tmp ] = repn.quadratic_coefs[ii];
        }

    ostr.print("+ [\n");    // << "+ [\n";
    for (auto it=qval.begin(); it != qval.end(); ++it) {
        const std::pair<int,int>& tmp = it->first;
        double val = it->second;
        if (tmp.first == tmp.second) {
            //if (val > 0)
            //    ostr.print("+{} x({}) ^ 2\n", val, tmp.first);      // << "+" << val << " x(" << tmp.first << ") ^ 2\n";
            //else if (val < 0)
            //    ostr.print("{} x({}) ^ 2\n", val, tmp.first);       // << val << " x(" << tmp.first << ") ^ 2\n";
            if (val != 0)
                ostr.print("{:+} x({}) ^ 2\n", val, tmp.first);       // << val << " x(" << tmp.first << ") ^ 2\n";
            }
        else {
            //if (val > 0)
            //    ostr.print("+{} x({}) * x({})\n", val, tmp.first, tmp.second);  // << "+" << val << " x(" << tmp.first << ") * x(" << tmp.second << ")\n";
            //else if (val < 0)
            //    ostr.print("{} x({}) * x({})\n", val, tmp.first, tmp.second);   // << val << " x(" << tmp.first << ") * x(" << tmp.second << ")\n";
            if (val != 0)
                ostr.print("{:+} x({}) * x({})\n", val, tmp.first, tmp.second);
            }
        }
    ostr.print("]\n");  // << "]\n";
    }
}

void print_objective(fmt::ostream& ostr, const Objective& obj, bool& one_var_constant, const std::unordered_map<unsigned int,unsigned int>& vid)
{
CALI_CXX_MARK_FUNCTION;

QuadraticExpr expr;
expr.collect_terms(obj);
print_repn(ostr, expr, vid);
double tmp = expr.constval;
if (tmp != 0) {
    one_var_constant=true;
    //if (tmp > 0)
    //    ostr.print("+");
    ostr.print("{:+} ONE_VAR_CONSTANT\n", tmp);   // << tmp << " ONE_VAR_CONSTANT\n";
    }
}

void print_constraint(fmt::ostream& ostr, const Constraint& c, unsigned int ctr, const std::unordered_map<unsigned int,unsigned int>& vid)
{
CALI_CXX_MARK_FUNCTION;

CALI_MARK_BEGIN("collect_terms");
QuadraticExpr expr;
expr.collect_terms(c);
double tmp = expr.constval;

auto lower = c.lower();
auto upper = c.upper();

ostr.print("c{}:\n", ctr);  // << "c" << ctr << ":\n";
++ctr;
CALI_MARK_END("collect_terms");
if (c.is_inequality()) {
    //CALI_MARK_BEGIN("IF");
    if (lower.repn) {
        ostr.print("{} <= ", lower.get_value() - tmp);      // << lower.get_value() - tmp << " <= ";
        }
    print_repn(ostr, expr, vid);
    if (upper.repn) {
        ostr.print(" <= {}", upper.get_value() - tmp);      // << " <= " << upper.get_value() - tmp;
        }
    ostr.print("\n\n");
    //CALI_MARK_END("IF");
    }
else {
    print_repn(ostr, expr, vid);
    CALI_MARK_BEGIN("ELSE");
    ostr.print("= {}\n\n", lower.get_value() - tmp);        // << "= " << lower.get_value() - tmp << "\n\n";
    CALI_MARK_END("ELSE");
    }
}


void write_lp_problem_fmtlib(Model& model, std::string& fname, std::map<int,int>& invvarmap, std::map<int,int>& invconmap)
{
auto ostr = fmt::output_file(fname, fmt::file::WRONLY | fmt::file::CREATE | FMT_POSIX(O_TRUNC));

CALI_CXX_MARK_FUNCTION;

if (model.repn->objectives.size() == 0) {
    std::cerr << "Error writing LP file: No objectives specified!" << std::endl;
    return;
    }
if (model.repn->objectives.size() > 1) {
    std::cerr << "Error writing LP file: More than one objective defined!" << std::endl;
    return;
    }

// Create variable ID map
std::unordered_map<unsigned int, unsigned int> vid;
{
unsigned int ctr=0;
for(std::vector<Variable>::iterator it=model.repn->variables.begin(); it != model.repn->variables.end(); ++it) {
    vid[(*it).id()] = ctr;
    invvarmap[ctr] = (*it).id();
    ++ctr;
    }
}

// Create file
ostr.print("\\* LP File Generated by COEK *\\\n\n");
ostr.print("\nminimize\n\n");

bool one_var_constant=false;
try {
    //
    // Objectives and Constraints
    //
    ostr.print("obj:\n");
    print_objective(ostr, model.repn->objectives[0], one_var_constant, vid);

    ostr.print("\nsubject to\n\n");
    unsigned int ctr=0;
    for (std::vector<Constraint>::iterator it=model.repn->constraints.begin(); it != model.repn->constraints.end(); ++it) {
        invconmap[it->id()] = ctr;
        print_constraint(ostr, *it, ctr, vid);
        ++ctr;
        }
    }
catch (UnknownVariable& e)
    {
    // TODO - More comprehensive error reporting here.
    throw std::runtime_error("Error writing LP file: Unexpected variable encountered in a model expression");
    }
catch (std::exception& e)
    {
    throw std::runtime_error(std::string("Unknown error writing LP file: ") + e.what());
    }

if (one_var_constant) {
    ostr.print("c_ONE_VAR_CONSTANT:\n");
    ostr.print("ONE_VAR_CONSTANT = 1.0\n");
    ostr.print("\n");
    }

std::map<unsigned int,VariableTerm*> bvars;
std::map<unsigned int,VariableTerm*> ivars;
ostr.print("\nbounds\n");
for(std::vector<Variable>::iterator it=model.repn->variables.begin(); it != model.repn->variables.end(); ++it) {
    VariableTerm* v = it->repn;
    if (v->lb <= -COEK_INFINITY)
        ostr.print("-inf");
    else
        ostr.print("{}", v->lb);
    ostr.print(" <= x({}) <= ", vid[v->index]);     // << " <= x(" << vid[v->index] << ") <= ";
    if (v->ub >= COEK_INFINITY)
        ostr.print("inf\n");
    else
        ostr.print("{}\n", v->ub);                  // << v->ub << '\n';
    if (v->binary)
        bvars[vid[v->index]] = v;
    if (v->integer)
        ivars[vid[v->index]] = v;
    }

if (bvars.size() > 0) {
    ostr.print("\nbinary\n");
    for(std::map<unsigned int,VariableTerm*>::iterator it=bvars.begin(); it != bvars.end(); ++it)
        ostr.print("x({})\n", it->first);           // << "x(" << it->first << ")\n";
    }

if (ivars.size() > 0) {
    ostr.print("\ninteger\n");
    for(std::map<unsigned int,VariableTerm*>::iterator it=ivars.begin(); it != ivars.end(); ++it)
        ostr.print("x({})\n", it->first);           // << "x(" << it->first << ")\n";
    }

ostr.print("\nend\n");    // << std::endl; 
ostr.close();
}


void write_lp_problem_fmtlib(CompactModel& model, std::string& fname, std::map<int,int>& varmap, std::map<int,int>& conmap)
{
auto ostr = fmt::output_file(fname, fmt::file::WRONLY | fmt::file::CREATE | FMT_POSIX(O_TRUNC));

CALI_CXX_MARK_FUNCTION;

if (model.objectives.size() == 0) {
    std::cerr << "Error writing LP file: No objectives specified!" << std::endl;
    return;
    }

// Create variable ID map
std::unordered_map<unsigned int, unsigned int> vid;
{
unsigned int ctr=0;
for(std::vector<Variable>::iterator it=model.variables.begin(); it != model.variables.end(); ++it) {
    vid[(*it).id()] = ctr;
    varmap[ctr] = (*it).id();
    ++ctr;
    }
}

// Create file

ostr.print("\\* LP File Generated by COEK *\\\n\n");
ostr.print("\nminimize\n\n");

bool one_var_constant=false;
try {
    ostr.print("obj:\n");
    int nobj=0;

    for (auto it=model.objectives.begin(); it != model.objectives.end(); ++it) {
        auto& val = *it;
        if (std::get_if<Objective>(&val)) {
        //if (auto eval = std::get_if<Objective>(&val)) {
            /* WEH - TODO rework to work with objectives
            Objective obj = eval->expand();
            //Objective obj(e, model.sense[nobj]);
            print_objective(ostr, obj, one_var_constant, vid);
            ++nobj;
            */
            }
        else {
#if 0
            auto& seq = std::get<ExpressionSequence>(val);
            for (auto jt=seq.begin(); jt != seq.end(); ++jt) {
                print_objective(ostr, *jt, one_var_constant, vid);
                ++nobj;
                }
#endif
            std::cerr << "Compact models can only have a single objective!" << std::endl;
            return;
            }
        }
    if (nobj > 1) {
        std::cerr << "Error writing LP file: More than one objective defined!" << std::endl;
        return;
        }

    ostr.print("\nsubject to\n\n");

    //
    // Simple contraints
    //
    unsigned int ctr=0;
    for (auto it=model.constraints.begin(); it != model.constraints.end(); ++it) {
        auto& val = *it;
        if (auto cval = std::get_if<Constraint>(&val)) {
            Constraint c = cval->expand();
            conmap[c.id()] = ctr;
            print_constraint(ostr, c, ctr, vid);
            ++ctr;
            }
        else {
            auto& seq = std::get<ConstraintSequence>(val);
            for (auto jt=seq.begin(); jt != seq.end(); ++jt) {
                conmap[jt->id()] = ctr;
                print_constraint(ostr, *jt, ctr, vid);
                ++ctr;
                }
            }
        }
    }
catch (UnknownVariable& e)
    {
    // TODO - More comprehensive error reporting here.
    throw std::runtime_error("Error writing LP file: Unexpected variable encountered in a model expression");
    }
catch (std::exception& e)
    {
    throw std::runtime_error(std::string("Unknown error writing LP file: ") + e.what());
    }

if (one_var_constant) {
    ostr.print("c_ONE_VAR_CONSTANT:\n");
    ostr.print("ONE_VAR_CONSTANT = 1.0\n");
    ostr.print("\n");
    }

std::map<unsigned int,VariableTerm*> bvars;
std::map<unsigned int,VariableTerm*> ivars;
ostr.print("\nbounds\n");
for(std::vector<Variable>::iterator it=model.variables.begin(); it != model.variables.end(); ++it) {
    VariableTerm* v = it->repn;
    if (v->lb <= -COEK_INFINITY)
        ostr.print("-inf");
    else
        ostr.print("{}", v->lb);
    ostr.print(" <= x({}) <= ", vid[v->index]);     // << " <= x(" << vid[v->index] << ") <= ";
    if (v->ub >= COEK_INFINITY)
        ostr.print("inf\n");
    else
        ostr.print("{}\n", v->ub);
    if (v->binary)
        bvars[vid[v->index]] = v;
    if (v->integer)
        ivars[vid[v->index]] = v;
    }

if (bvars.size() > 0) {
    ostr.print("\nbinary\n");
    for(std::map<unsigned int,VariableTerm*>::iterator it=bvars.begin(); it != bvars.end(); ++it)
        ostr.print("x({})\n", it->first);       // << "x(" << it->first << ")\n";
    }

if (ivars.size() > 0) {
    ostr.print("\ninteger\n");
    for(std::map<unsigned int,VariableTerm*>::iterator it=ivars.begin(); it != ivars.end(); ++it)
        ostr.print("x({})\n", it->first);       // << "x(" << it->first << ")\n";
    }

ostr.print("\nend\n");      // << std::endl;
ostr.close();
}


void write_lp_problem(Model& model, std::string& fname, std::map<int,int>& invvarmap, std::map<int,int>& invconmap)
{ write_lp_problem_fmtlib(model, fname, invvarmap, invconmap); }

void write_lp_problem(CompactModel& model, std::string& fname, std::map<int,int>& varmap, std::map<int,int>& conmap)
{ write_lp_problem_fmtlib(model, fname, varmap, conmap); }
#else

void write_lp_problem(Model& model, std::string& fname, std::map<int,int>& invvarmap, std::map<int,int>& invconmap)
{ write_lp_problem_ostream(model, fname, invvarmap, invconmap); }

void write_lp_problem(CompactModel& model, std::string& fname, std::map<int,int>& varmap, std::map<int,int>& conmap)
{ write_lp_problem_ostream(model, fname, varmap, conmap); }
#endif

}
