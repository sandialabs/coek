#include <iomanip>
#include <map>
#include "expr/ast_visitors.hpp"
#include "coek_model.hpp"

#define EPSILON 1e-12

namespace coek {

void check_that_expression_variables_are_declared(Model& model, const std::set<unsigned int>& var_ids);


namespace {

void format(std::ostream& ostr, double value)
{
ostr << std::setprecision(16) << value;
}



class PrintExpr : public Visitor
{
public:

    std::ostream& ostr;
    const std::unordered_map<int,int>& varmap;

public:

    PrintExpr(std::ostream& _ostr, const std::unordered_map<int,int>& _varmap)
        : ostr(_ostr), varmap(_varmap) {}

    void visit(ConstantTerm& arg);
    void visit(ParameterTerm& arg);
    void visit(IndexParameterTerm& arg);
    void visit(VariableTerm& arg);
    void visit(VariableRefTerm& arg);
    void visit(IndexedVariableTerm& arg);
    void visit(MonomialTerm& arg);
    void visit(InequalityTerm& arg);
    void visit(EqualityTerm& arg);
    void visit(ObjectiveTerm& arg);
    void visit(NegateTerm& arg);
    void visit(PlusTerm& arg);
    void visit(TimesTerm& arg);
    void visit(DivideTerm& arg);
    void visit(AbsTerm& arg);
    void visit(CeilTerm& arg);
    void visit(FloorTerm& arg);
    void visit(ExpTerm& arg);
    void visit(LogTerm& arg);
    void visit(Log10Term& arg);
    void visit(SqrtTerm& arg);
    void visit(SinTerm& arg);
    void visit(CosTerm& arg);
    void visit(TanTerm& arg);
    void visit(SinhTerm& arg);
    void visit(CoshTerm& arg);
    void visit(TanhTerm& arg);
    void visit(ASinTerm& arg);
    void visit(ACosTerm& arg);
    void visit(ATanTerm& arg);
    void visit(ASinhTerm& arg);
    void visit(ACoshTerm& arg);
    void visit(ATanhTerm& arg);
    void visit(PowTerm& arg);
};


void PrintExpr::visit(ConstantTerm& arg)
{
ostr << "n";
format(ostr, arg.value);
ostr << '\n';
}

void PrintExpr::visit(ParameterTerm& arg)
{
ostr << "n";
format(ostr, arg.value);
ostr << '\n';
}

void PrintExpr::visit(IndexParameterTerm& arg)
{ throw std::runtime_error("Cannot write an NL file using an abstract expression!"); }

void PrintExpr::visit(VariableTerm& arg)
{ 
if (arg.fixed)
    ostr << "n" << arg.value << '\n';
else
    ostr << "v" << varmap.at(arg.index) << '\n';
}

void PrintExpr::visit(VariableRefTerm& arg)
{ throw std::runtime_error("Cannot write an NL file using an abstract expression!"); }

void PrintExpr::visit(IndexedVariableTerm& arg)
{ ostr << "v" << varmap.at(arg.index) << '\n'; }

void PrintExpr::visit(MonomialTerm& arg)
{
ostr << "o2" << '\n';
ostr << "n";
format(ostr, arg.coef);
ostr << '\n';
if (arg.var->fixed)
    ostr << "n" << arg.var->value << '\n';
else
    ostr << "v" << varmap.at(arg.var->index) << '\n';
}

void PrintExpr::visit(InequalityTerm& arg)
{ arg.body->accept(*this); }

void PrintExpr::visit(EqualityTerm& arg)
{ arg.body->accept(*this); }

void PrintExpr::visit(ObjectiveTerm& arg)
{ arg.body->accept(*this); }

void PrintExpr::visit(NegateTerm& arg)
{
ostr << "o16\n";
arg.body->accept(*this);
}

void PrintExpr::visit(PlusTerm& arg)
{
if (arg.n == 2)
    ostr << "o0\n";
else
    ostr << "o54\n" << arg.n << '\n';
std::vector<expr_pointer_t>& vec = *(arg.data);
for (size_t i=0; i<arg.n; ++i)
    vec[i]->accept(*this);
}

void PrintExpr::visit(TimesTerm& arg)
{
ostr << "o2\n";
arg.lhs->accept(*this);
arg.rhs->accept(*this);
}

void PrintExpr::visit(DivideTerm& arg)
{
ostr << "o3\n";
arg.lhs->accept(*this);
arg.rhs->accept(*this);
}

#define PrintExpr_FN(FN, TERM)\
void PrintExpr::visit(TERM& arg)\
{\
ostr << #FN << '\n';\
arg.body->accept(*this);\
}

PrintExpr_FN(o15, AbsTerm)
PrintExpr_FN(o14, CeilTerm)
PrintExpr_FN(o13, FloorTerm)
PrintExpr_FN(o44, ExpTerm)
PrintExpr_FN(o43, LogTerm)
PrintExpr_FN(o42, Log10Term)
PrintExpr_FN(o39, SqrtTerm)
PrintExpr_FN(o41, SinTerm)
PrintExpr_FN(o46, CosTerm)
PrintExpr_FN(o38, TanTerm)
PrintExpr_FN(o40, SinhTerm)
PrintExpr_FN(o45, CoshTerm)
PrintExpr_FN(o37, TanhTerm)
PrintExpr_FN(o51, ASinTerm)
PrintExpr_FN(o53, ACosTerm)
PrintExpr_FN(o49, ATanTerm)
PrintExpr_FN(o50, ASinhTerm)
PrintExpr_FN(o52, ACoshTerm)
PrintExpr_FN(o47, ATanhTerm)

void PrintExpr::visit(PowTerm& arg)
{
ostr << "o5\n";
arg.lhs->accept(*this);
arg.rhs->accept(*this);
}


void print_expr(std::ostream& ostr, const MutableNLPExpr& repn, const std::unordered_map<int,int>& varmap, bool objective=false)
{
bool nonlinear = not repn.nonlinear.is_constant();
bool quadratic = repn.quadratic_coefs.size() > 0;

double cval = repn.constval.get_value();
if (not nonlinear)
    cval += repn.nonlinear.get_value();

std::map<std::pair<int,int>,double> term;
if (quadratic) {
    for (size_t i=0; i<repn.quadratic_coefs.size(); ++i) {
        int lhs = varmap.at(repn.quadratic_lvars[i]->index);
        int rhs = varmap.at(repn.quadratic_rvars[i]->index);
        if (rhs < lhs)
            std::swap(lhs,rhs);
        auto key = std::pair<int,int>(lhs, rhs);
        if (term.find(key) == term.end())
            term[key] = repn.quadratic_coefs[i].get_value();
        else
            term[key] += repn.quadratic_coefs[i].get_value();
        }
    }

// Compute the number of terms in the sum
int ctr=0;
if (objective and (fabs(cval) > EPSILON))
    ++ctr;
if (nonlinear)
    ++ctr;
if (quadratic)
    ctr += term.size();

// Write the sum header
if (ctr == 0)
    return;
else if (ctr == 2)
    ostr << "o0\n";
else if (ctr > 2)
    ostr << "o54\n" << ctr << '\n';

// Write terms in the sum
if (quadratic) {
    for (auto it=term.begin(); it != term.end(); ++it) {
        double coef = it->second;
        if (coef != 1) {
            ostr << "o2\n";
            ostr << "n";
            format(ostr, coef);
            ostr << '\n';
            }
        ostr << "o2\n";
        ostr << "v" << it->first.first << '\n';
        ostr << "v" << it->first.second << '\n';
        }
    }
if (nonlinear) {
    PrintExpr visitor(ostr, varmap);
    repn.nonlinear.repn->accept(visitor);
    }
if (objective and (fabs(cval) > EPSILON)) {
    ostr << "n";
    format(ostr, cval);
    ostr << '\n';
    }
}

}



// TODO - Reorder constraints to have nonlinear before linear
// TODO - Reorder variables per the AMPL solver hookup logic
void write_nl_problem(Model& model, std::ostream& ostr, std::map<int,int>& invvarmap, std::map<int,int>& invconmap)
{
if (model.repn->objectives.size() == 0) {
    std::cerr << "Error writing NL file: No objectives specified!" << std::endl;
    return;
    }
if (model.repn->objectives.size() > 1) {
    std::cerr << "Error writing NL file: More than one objective defined!" << std::endl;
    return;
    }

//
// Process Model to Create NL Header
//
std::map<unsigned int,Variable> varobj;
std::set<unsigned int> vars;
std::set<unsigned int> nonlinear_vars_obj;
std::set<unsigned int> nonlinear_vars_con;
int num_inequalities=0;
int num_ranges=0;
int num_equalities=0;
int nonl_objectives=0;
int nonl_constraints=0;

std::set<unsigned int> linear_vars;
int num_linear_binary_vars=0;
int num_linear_integer_vars=0;
int num_nonlinear_obj_int_vars=0;
int num_nonlinear_con_int_vars=0;
int num_nonlinear_both_int_vars=0;

int nnz_Jacobian=0;
int nnz_gradient=0;

// Objectives
std::vector<MutableNLPExpr> o_expr(model.repn->objectives.size());
int ctr=0;
for (auto it=model.repn->objectives.begin(); it != model.repn->objectives.end(); ++it, ++ctr) {
    o_expr[ctr].collect_terms(*it);
    if ((o_expr[ctr].quadratic_coefs.size() > 0) or (not o_expr[ctr].nonlinear.is_constant()))
        ++nonl_objectives;
    for (auto it=o_expr[ctr].linear_vars.begin(); it != o_expr[ctr].linear_vars.end(); ++it) {
        auto var = *it;
        linear_vars.insert(var->index);
        vars.insert(var->index);
        varobj[var->index] = var;
        }
    for (auto it=o_expr[ctr].quadratic_lvars.begin(); it != o_expr[ctr].quadratic_lvars.end(); ++it) {
        auto var = *it;
        nonlinear_vars_obj.insert(var->index);
        vars.insert(var->index);
        varobj[var->index] = var;
        }
    for (auto it=o_expr[ctr].quadratic_rvars.begin(); it != o_expr[ctr].quadratic_rvars.end(); ++it) {
        auto var = *it;
        nonlinear_vars_obj.insert(var->index);
        vars.insert(var->index);
        varobj[var->index] = var;
        }
    for (auto it=o_expr[ctr].nonlinear_vars.begin(); it != o_expr[ctr].nonlinear_vars.end(); ++it) {
        auto var = *it;
        nonlinear_vars_obj.insert(var->index);
        vars.insert(var->index);
        varobj[var->index] = var;
        }
    }

// Since we have just one objective, the # of variables is the # of nonzeros in gradients
nnz_gradient=vars.size();

// Constraints
std::vector<MutableNLPExpr> c_expr(model.repn->constraints.size());
std::vector<int> r(model.repn->constraints.size());
std::vector<double> rval(2*model.repn->constraints.size());
ctr=0;
for (auto it=model.repn->constraints.begin(); it != model.repn->constraints.end(); ++it, ++ctr) {
    invconmap[ctr] = it->id();
    c_expr[ctr].collect_terms(*it);

    double bodyconst = c_expr[ctr].constval.get_value();
    if (it->is_inequality()) {
        ++num_inequalities;
        if (it->repn->lower and it->repn->upper) {
            double lower = it->repn->lower->eval() - bodyconst;
            double upper = it->repn->upper->eval() - bodyconst;
            if (fabs(upper-lower) < EPSILON) {
                ++num_equalities;
                r[ctr] = 4;
                rval[2*ctr] = lower;
                }
            else {
                ++num_ranges;
                r[ctr] = 0;
                rval[2*ctr] = lower;
                rval[2*ctr+1] = upper;
                }
            }
        else if (it->repn->lower) {
            r[ctr] = 2;
            rval[2*ctr] = it->repn->lower->eval() - bodyconst;
            }
        else if (it->repn->upper) {
            r[ctr] = 1;
            rval[2*ctr] = it->repn->upper->eval() - bodyconst;
            }
        else {
            r[ctr] = 3;
            }
        }
    else {
        ++num_equalities;
        r[ctr] = 4;
        rval[2*ctr] = it->repn->lower->eval() - bodyconst;
        }
    if ((c_expr[ctr].quadratic_coefs.size() > 0) or (not c_expr[ctr].nonlinear.is_constant()))
        ++nonl_constraints;

    std::set<int> curr_vars;

    for (auto it=c_expr[ctr].linear_vars.begin(); it != c_expr[ctr].linear_vars.end(); ++it) {
        auto var = *it;
        linear_vars.insert(var->index);
        vars.insert(var->index);
        varobj[var->index] = var;
        curr_vars.insert(var->index);
        }
    for (auto it=c_expr[ctr].quadratic_lvars.begin(); it != c_expr[ctr].quadratic_lvars.end(); ++it) {
        auto var = *it;
        nonlinear_vars_con.insert(var->index);
        vars.insert(var->index);
        varobj[var->index] = var;
        curr_vars.insert(var->index);
        }
    for (auto it=c_expr[ctr].quadratic_rvars.begin(); it != c_expr[ctr].quadratic_rvars.end(); ++it) {
        auto var = *it;
        nonlinear_vars_con.insert(var->index);
        vars.insert(var->index);
        varobj[var->index] = var;
        curr_vars.insert(var->index);
        }
    for (auto it=c_expr[ctr].nonlinear_vars.begin(); it != c_expr[ctr].nonlinear_vars.end(); ++it) {
        auto var = *it;
        nonlinear_vars_con.insert(var->index);
        vars.insert(var->index);
        varobj[var->index] = var;
        curr_vars.insert(var->index);
        }

    // Add Jacobian terms for each constraint
    nnz_Jacobian += curr_vars.size();
    }

check_that_expression_variables_are_declared(model, vars);

for (auto it=linear_vars.begin(); it != linear_vars.end(); ++it) {
    auto& var = varobj[*it];
    if (var.is_binary())
        ++num_linear_binary_vars;
    else if (var.is_integer())
        ++num_linear_integer_vars;
    }

int nonlinear_vars_both=0;
for (auto it=nonlinear_vars_obj.begin(); it != nonlinear_vars_obj.end(); ++it) {
    bool flag = varobj[*it].is_binary() or varobj[*it].is_integer();
    if (flag)
       ++num_nonlinear_obj_int_vars;
    if (nonlinear_vars_con.find(*it) != nonlinear_vars_con.end()) {
        ++nonlinear_vars_both;
        if (flag)
           ++num_nonlinear_both_int_vars;
        }
    }
for (auto it=nonlinear_vars_con.begin(); it != nonlinear_vars_con.end(); ++it) {
    if (varobj[*it].is_binary() or varobj[*it].is_integer())
       ++num_nonlinear_con_int_vars;
    }

// Map Variable index to NL variable ID (0 ... n_vars-1)
std::unordered_map<int,int> varmap;
ctr = 0;
for (auto it=vars.begin(); it != vars.end(); ++it) {
    invvarmap[ctr] = *it;
    varmap[*it] = ctr;
    ++ctr;
    }
if (vars.size() != varmap.size()) {
    std::cerr << "Error writing NL file: Variables with duplicate index values detected!" << std::endl;
    return;
    }

// Compute linear Jacobian and Gradient values
std::vector<std::set<int>> k_count(vars.size());
std::vector<std::map<int,double>> G(o_expr.size());
std::vector<std::map<int,double>> J(c_expr.size());

ctr=0;
for (auto it=o_expr.begin(); it != o_expr.end(); ++it, ++ctr) {
    for (auto jt=it->quadratic_lvars.begin(); jt!= it->quadratic_lvars.end(); ++jt) {
        G[ctr][ varmap[(*jt)->index] ] = 0;
        }
    for (auto jt=it->quadratic_rvars.begin(); jt!= it->quadratic_rvars.end(); ++jt) {
        G[ctr][ varmap[(*jt)->index] ] = 0;
        }
    for (auto jt=it->nonlinear_vars.begin(); jt!= it->nonlinear_vars.end(); ++jt) {
        G[ctr][ varmap[(*jt)->index] ] = 0;
        }
    for (size_t j=0; j<it->linear_coefs.size(); ++j) {
        auto index = varmap[it->linear_vars[j]->index];
        if (G[ctr].find(index) == G[ctr].end())
            G[ctr][index] = it->linear_coefs[j].get_value();
        else
            G[ctr][index] += it->linear_coefs[j].get_value();
        }
    }
ctr=0;
for (auto it=c_expr.begin(); it != c_expr.end(); ++it, ++ctr) {
    for (auto jt=it->quadratic_lvars.begin(); jt!= it->quadratic_lvars.end(); ++jt) {
        int index = varmap[(*jt)->index];
        k_count[ index ].insert(ctr);
        J[ctr][ index ] = 0;
        }
    for (auto jt=it->quadratic_rvars.begin(); jt!= it->quadratic_rvars.end(); ++jt) {
        int index = varmap[(*jt)->index];
        k_count[ index ].insert(ctr);
        J[ctr][ index ] = 0;
        }
    for (auto jt=it->nonlinear_vars.begin(); jt!= it->nonlinear_vars.end(); ++jt) {
        int index = varmap[(*jt)->index];
        k_count[ index ].insert(ctr);
        J[ctr][ index ] = 0;
        }
    for (size_t j=0; j<it->linear_coefs.size(); ++j) {
        int index = varmap[it->linear_vars[j]->index];
        if (J[ctr].find(index) == J[ctr].end()) {
            k_count[ index ].insert(ctr);
            J[ctr][index] = it->linear_coefs[j].get_value();
            }
        else
            J[ctr][index] += it->linear_coefs[j].get_value();
        }
    }

//
// Write NL Header
//
// This API seems poorly documented.  Is the 2005 paper the defining reference?  Pyomo writes a header that doesn't conform to it...
//
ostr << "g3 1 1 0 # unnamed problem generated by COEK\n";
ostr << " " << vars.size() << " " << (num_inequalities+num_equalities) << " 1 " << num_ranges << " " << num_equalities << " 0 # vars, constraints, objectives, ranges, eqns, lcons\n";
ostr << " " << nonl_constraints << " " << nonl_objectives << " # nonlinear constraints, objectives\n";
ostr << " 0 0 # network constraints: nonlinear, linear\n";
ostr << " " << nonlinear_vars_con.size() << " " << nonlinear_vars_obj.size() << " " << nonlinear_vars_both << " # nonlinear vars in constraints, objectives, both\n";
ostr << " 0 0 0 1 # linear network variables; functions; arith, flags\n";
ostr << " " << num_linear_binary_vars << " " << num_linear_integer_vars << " " << num_nonlinear_both_int_vars << " " << num_nonlinear_con_int_vars << " " << num_nonlinear_obj_int_vars << " # discrete variables: binary, integer, nonlinear (b,c,o)\n";
ostr << " " << nnz_Jacobian << " " << nnz_gradient << " # nonzeros in Jacobian, gradients\n";
ostr << " 0 0 # max name lengths: constraints, variables\n";
ostr << " 0 0 0 0 0 # common exprs: b,c,o,c1,o1\n";

//
// "C" section - nonlinear constraint segments
//
ctr = 0;
for (auto it=c_expr.begin(); it != c_expr.end(); ++it, ++ctr) {
    if ((not it->nonlinear.is_constant()) or (it->quadratic_coefs.size() > 0)) {
        ostr << "C" << ctr << '\n';
        print_expr(ostr, *it, varmap);
        }
    else {
        ostr << "C" << ctr << '\n';
        ostr << "n0\n";
        }
    }

//
// "O" section - nonlinear objective segments
//
ctr=0;
for (auto it=o_expr.begin(); it != o_expr.end(); ++it, ++ctr) {
    bool sense = model.repn->objectives[ctr].sense();
    if ((not it->nonlinear.is_constant()) or (it->quadratic_coefs.size() > 0)) {
        if (sense == Model::minimize)
            ostr << "O" << ctr << " 0\n";
        else
            ostr << "O" << ctr << " 1\n";
        print_expr(ostr, *it, varmap, true);
        }
    else {
        if (sense == Model::minimize)
            ostr << "O" << ctr << " 0\n";
        else
            ostr << "O" << ctr << " 1\n";
        ostr << "n" << it->constval.get_value() << '\n';
        }
    }

//
// "x" section - primal initial values
//
{
std::map<int, double> values;
ctr=0;
for (auto it=vars.begin(); it != vars.end(); ++it, ++ctr) {
    auto tmp = varobj[*it].get_value();
    if (not std::isnan(tmp))
        values[ctr] = tmp;
    }
if (values.size() > 0) {
    ostr << "x" << values.size() << '\n';
    for (auto it=values.begin(); it != values.end(); ++it)
        ostr << it->first << " " << it->second << '\n';
    }
}

//
// "r" section - bounds on constraints
//

if (model.repn->constraints.size() > 0) {
    ostr << "r\n";
    ctr = 0;
    for (auto it=model.repn->constraints.begin(); it != model.repn->constraints.end(); ++it, ++ctr) {
        switch (r[ctr]) {
            case 0:
                ostr << "0 ";
                format(ostr, rval[2*ctr]);
                ostr << " ";
                format(ostr, rval[2*ctr+1]);
                break;
            case 1:
                ostr << "1 ";
                format(ostr, rval[2*ctr]);
                break;
            case 2:
                ostr << "2 ";
                format(ostr, rval[2*ctr]);
                break;
            case 3:
                ostr << "3";
                break;
            case 4:
                ostr << "4 ";
                format(ostr, rval[2*ctr]);
                break;
            };
        ostr << '\n';
        }
    }

//
// "b" section - bounds on variables
//
ostr << "b\n";
for (auto it=vars.begin(); it != vars.end(); ++it) {
    auto var = varobj[*it];
    double lb = var.get_lb();
    double ub = var.get_ub();
    if (lb == -COEK_INFINITY) {
        if (ub == COEK_INFINITY) {
            ostr << "3\n";
            }
        else {
            ostr << "1 ";
            format(ostr, ub);
            ostr << '\n';
            }
        }
    else {
        if (ub == COEK_INFINITY) {
            ostr << "2 ";
            format(ostr, lb);
            ostr << '\n';
            }
        else {
            if (fabs(ub-lb) < EPSILON) {
                ostr << "4 ";
                format(ostr, lb);
                }
            else {
                ostr << "0 ";
                format(ostr, lb);
                ostr << " ";
                format(ostr, ub);
                }
            ostr << '\n';
            }
        }
    }

//
// "k" section - Jacobian column counts
//
ostr << "k" << (k_count.size()-1) << '\n';
ctr = 0;
for (size_t i=0; i<(k_count.size()-1); ++i) {
    ctr += k_count[i].size();
    ostr << ctr << '\n';
    }

//
// "J" section - Jacobian sparsity, linear terms
//
for (size_t i=0; i<J.size(); ++i) {
    if (J[i].size() == 0) continue;
    ostr << "J" << i << " " << J[i].size() << '\n';
    for (auto it=J[i].begin(); it!=J[i].end(); ++it) {
        ostr << it->first << " " << it->second << '\n';
        }
    }

//
// "G" section - Gradient sparsity, linear terms
//
for (size_t i=0; i<G.size(); ++i) {
    if (G[i].size() == 0) continue;
    ostr << "G" << i << " " << G[i].size() << '\n';
    for (auto it=G[i].begin(); it!=G[i].end(); ++it) {
        ostr << it->first << " " << it->second << '\n';
        }
    }

ostr << std::flush;
}

}
