#include <cstdio>
#include <memory>
#include <sstream>
#include <fstream>
#include <map>
#include "coek/expr/ast_term.hpp"
#include "coek/expr/varray.hpp"
#include "coek/abstract/expr_rule.hpp"
#include "coek/autograd/autograd.hpp"
//#include "coek/coek_expr.hpp"
#include "coek/coek_model.hpp"
#include "coek/solvers/solver.hpp"
#include "coek/expr/coek_exprterm.hpp"



namespace coek {


expr_pointer_t convert_expr_template(expr_pointer_t expr);
ConstraintTerm* convert_con_template(ConstraintTerm* expr);


//
// Context
//

class Context
{
public:

    std::vector<IndexParameter> indices;

    ConcreteSet index_set;

    std::list<Constraint> index_values;
    std::list<Constraint> index_constraints;

public:

    Context(const std::vector<IndexParameter>& _indices)
        : indices(_indices)
        {}
};


//
// ExpressionSequenceRepn
//

class ExpressionSequenceRepn
{
public:

    std::vector<Context> context;
    Expression expression_template;

public:

    ExpressionSequenceRepn(const Expression& expr)
        : expression_template(expr)
        {}
    
};


//
// ExpressionSeqIteratorRepn
//

class ExpressionSeqIteratorRepn
{
public:

    ExpressionSequenceRepn* seq;
    std::vector<SetIterator> context_iter;
    size_t ncontexts;
    Expression converted_expr;
    bool done;

    typedef Expression* pointer;
    typedef const Expression* const_pointer;
    typedef Expression& reference;
    typedef const Expression& const_reference;

public:

    ExpressionSeqIteratorRepn(ExpressionSequenceRepn* _seq, bool end)
        : seq(_seq)
        {
        done = end;
        if (!done) {
            context_iter.resize(seq->context.size());
            
            auto cit = seq->context.begin();
            for (auto it = context_iter.begin(); it != context_iter.end(); ++it, ++cit) {
                *it = cit->index_set.begin(cit->indices);
                }
            converted_expr = convert_expr_template( seq->expression_template.repn );
            }
        ncontexts = context_iter.size();
        }

    void operator++()
        {
        int i = ncontexts-1;
        while (i >= 0) {
            ++context_iter[i];
            if (context_iter[i] == seq->context[i].index_set.end())
                {
                context_iter[i] = seq->context[i].index_set.begin(seq->context[i].indices);
                i--;
                }
            else
                break;
            }
        if (i < 0)
            done = true;
        else
            converted_expr = convert_expr_template(seq->expression_template.repn);
        }

    bool operator==(const ExpressionSeqIteratorRepn* other) const
        {
        if (done != other->done)
            return false;
        if (done)
            return true;
        // BAD - Other comparisons here?
        return true;
        }

    bool operator!=(const ExpressionSeqIteratorRepn* other) const
        {
        if (done == other->done)
            return false;
        if (other->done)
            return true;
        // BAD - Other comparisons here?
        return true;
        }

    reference operator*()
        { return converted_expr; }

    const_reference operator*() const
        { return converted_expr; }

    pointer operator->()
        { return &(converted_expr); }

    const_pointer operator->() const
        { return &(converted_expr); }
};

//
// ExpressionSeqIterator
//

ExpressionSeqIterator::ExpressionSeqIterator()
{}

ExpressionSeqIterator::ExpressionSeqIterator(ExpressionSequenceRepn* _seq, bool end)
{
repn = std::make_shared<ExpressionSeqIteratorRepn>(_seq, end);
}

ExpressionSeqIterator& ExpressionSeqIterator::operator++()
{
repn->operator++();
return *this;
}

bool ExpressionSeqIterator::operator==(const ExpressionSeqIterator& other) const
{
return repn->operator==(other.repn.get());
}

bool ExpressionSeqIterator::operator!=(const ExpressionSeqIterator& other) const
{
return repn->operator!=(other.repn.get());
}

ExpressionSeqIterator::reference ExpressionSeqIterator::operator*()
{
return repn->operator*();
}

ExpressionSeqIterator::const_reference ExpressionSeqIterator::operator*() const
{
return repn->operator*();
}

ExpressionSeqIterator::pointer ExpressionSeqIterator::operator->()
{
return repn->operator->();
}

ExpressionSeqIterator::const_pointer ExpressionSeqIterator::operator->() const
{
return repn->operator->();
}

//
// Expression
//

ExpressionSequenceAux Expression::forall(const std::vector<IndexParameter>& indices)
{
auto repn = std::make_shared<ExpressionSequenceRepn>(*this);
repn->context.emplace_back(indices);
return repn;
}

//
// ExpressionSequenceAux
//
ExpressionSequenceAux::ExpressionSequenceAux(const std::shared_ptr<ExpressionSequenceRepn>& _repn)
    : repn(_repn)
{}

ExpressionSequence ExpressionSequenceAux::in(const ConcreteSet& _index_set)
{
Context& curr = repn->context.back();
curr.index_set = _index_set;
return repn;
}

//
// ExpressionSequence
//
ExpressionSequence::ExpressionSequence(const std::shared_ptr<ExpressionSequenceRepn>& _repn)
    : repn(_repn)
{}

ExpressionSequenceAux ExpressionSequence::forall(const std::vector<IndexParameter>& params)
{
repn->context.emplace_back(params);
return repn;
}

ExpressionSequence ExpressionSequence::st(const Constraint& con)
{
auto curr = repn->context.back();
curr.index_constraints.push_back(con);
return repn;
}

ExpressionSequence ExpressionSequence::where(const Constraint& con)
{
// TODO - parse these constraints here and use a more explicit data structure
auto curr = repn->context.back();
curr.index_values.push_back(con);
return repn;
}

ExpressionSeqIterator ExpressionSequence::begin()
{
return ExpressionSeqIterator(repn.get(), false);
}

ExpressionSeqIterator ExpressionSequence::end()
{
return ExpressionSeqIterator(repn.get(), true);
}



//
// ConstraintSequenceRepn
//

class ConstraintSequenceRepn
{
public:

    std::vector<Context> context;
    Constraint constraint_template;

public:

    ConstraintSequenceRepn(const Constraint& con)
        : constraint_template(con)
        {}
};


//
// ConstraintSeqIteratorRepn
//

class ConstraintSeqIteratorRepn
{
public:

    ConstraintSequenceRepn* seq;
    std::vector<SetIterator> context_iter;
    size_t ncontexts;
    Constraint converted_con;
    bool done;

    typedef Constraint* pointer;
    typedef const Constraint* const_pointer;
    typedef Constraint& reference;
    typedef const Constraint& const_reference;

public:

    ConstraintSeqIteratorRepn(ConstraintSequenceRepn* _seq, bool end)
        : seq(_seq)
        {
        done = end;
        if (!done) {
            context_iter.resize(seq->context.size());

            auto cit = seq->context.begin();
            for (auto it = context_iter.begin(); it != context_iter.end(); ++it, ++cit) {
                *it = cit->index_set.begin(cit->indices);
                }
            converted_con = convert_con_template(seq->constraint_template.repn);
            }
        ncontexts = context_iter.size();
        }

    void operator++()
        {
        int i = ncontexts-1;
        while (i >= 0) {
            ++context_iter[i];
            if (context_iter[i] == seq->context[i].index_set.end())
                {
                context_iter[i] = seq->context[i].index_set.begin(seq->context[i].indices);
                i--;
                }
            else
                break;
            }
        if (i < 0)
            done = true;
        else
            converted_con = convert_con_template(seq->constraint_template.repn);
        }

    bool operator==(const ConstraintSeqIteratorRepn* other) const
        {
        if (done != other->done)
            return false;
        if (done)
            return true;
        // BAD - Other comparisons here?
        return true;
        }

    bool operator!=(const ConstraintSeqIteratorRepn* other) const
        {
        if (done == other->done)
            return false;
        if (other->done)
            return true;
        // BAD - Other comparisons here?
        return true;
        }

    reference operator*()
        { return converted_con; }

    const_reference operator*() const
        { return converted_con; }

    pointer operator->()
        { return &(converted_con); }

    const_pointer operator->() const
        { return &(converted_con); }
};

//
// ConstraintSeqIterator
//

ConstraintSeqIterator::ConstraintSeqIterator()
{}

ConstraintSeqIterator::ConstraintSeqIterator(ConstraintSequenceRepn* _seq, bool end)
{
repn = std::make_shared<ConstraintSeqIteratorRepn>(_seq, end);
}

ConstraintSeqIterator& ConstraintSeqIterator::operator++()
{
repn->operator++();
return *this;
}

bool ConstraintSeqIterator::operator==(const ConstraintSeqIterator& other) const
{
return repn->operator==(other.repn.get());
}

bool ConstraintSeqIterator::operator!=(const ConstraintSeqIterator& other) const
{
return repn->operator!=(other.repn.get());
}

ConstraintSeqIterator::reference ConstraintSeqIterator::operator*()
{
return repn->operator*();
}

ConstraintSeqIterator::const_reference ConstraintSeqIterator::operator*() const
{
return repn->operator*();
}

ConstraintSeqIterator::pointer ConstraintSeqIterator::operator->()
{
return repn->operator->();
}

ConstraintSeqIterator::const_pointer ConstraintSeqIterator::operator->() const
{
return repn->operator->();
}

//
// Constraint
//

ConstraintSequenceAux Constraint::forall(const std::vector<IndexParameter>& params)
{
auto repn = std::make_shared<ConstraintSequenceRepn>(*this);
repn->context.emplace_back(params);
return repn;
}

//
// ConstraintSequenceAux
//
ConstraintSequenceAux::ConstraintSequenceAux(const std::shared_ptr<ConstraintSequenceRepn>& _repn)
    : repn(_repn)
{}

ConstraintSequence ConstraintSequenceAux::in(const ConcreteSet& _index_set)
{
Context& curr = repn->context.back();
curr.index_set = _index_set;
return repn;
}

//
// ConstraintSequence
//
ConstraintSequence::ConstraintSequence(const std::shared_ptr<ConstraintSequenceRepn>& _repn)
    : repn(_repn)
{}

ConstraintSequenceAux ConstraintSequence::forall(const std::vector<IndexParameter>& params)
{
repn->context.emplace_back(params);
return repn;
}

ConstraintSequence ConstraintSequence::st(const Constraint& con)
{
auto curr = repn->context.back();
curr.index_constraints.push_back(con);
return repn;
}

ConstraintSequence ConstraintSequence::where(const Constraint& con)
{
// TODO - parse these constraints here and use a more explicit data structure
auto curr = repn->context.back();
curr.index_values.push_back(con);
return repn;
}

ConstraintSeqIterator ConstraintSequence::begin()
{
return ConstraintSeqIterator(repn.get(), false);
}

ConstraintSeqIterator ConstraintSequence::end()
{
return ConstraintSeqIterator(repn.get(), true);
}


#if 0
//
// ConstraintIteratorRepn
//

class ConstraintIteratorRepn
{
public:

    Model* model;
    std::vector<Constraint>::iterator constraints_iter;
    std::vector<ConstraintSequence>::iterator constraint_sequences_iter;
    ConstraintSeqIterator constraint_seq_iter;
    bool sequences;

    typedef Constraint* pointer;
    typedef const Constraint* const_pointer;
    typedef Constraint& reference;
    typedef const Constraint& const_reference;

public:

    ConstraintIteratorRepn(Model* _model, bool end)
        : model(_model), sequences(false)
        {
        if (end) {
            constraints_iter = model->constraints.end(); 
            constraint_sequences_iter = model->constraint_sequences.end(); 
            }
        else {
            constraints_iter = model->constraints.begin(); 
            constraint_sequences_iter = model->constraint_sequences.begin(); 
            if (constraint_sequences_iter != model->constraint_sequences.end())
                constraint_seq_iter = constraint_sequences_iter->begin();
            }
        }

    void operator++()
        {
        if (sequences) {
            ++constraint_seq_iter;
            if (constraint_seq_iter == constraint_sequences_iter->end())
                constraint_sequences_iter++;
            }
        else {
            ++constraints_iter;
            if (constraints_iter == model->constraints.end()) {
                sequences = true;
                if (constraint_sequences_iter != model->constraint_sequences.end())
                    constraint_seq_iter = constraint_sequences_iter->begin();
                }
            }
        }

    bool operator==(const ConstraintIteratorRepn* other) const
        {
        if (sequences)
            return (constraint_sequences_iter == other->constraint_sequences_iter);
        else
            return (constraints_iter == other->constraints_iter);
        }

    bool operator!=(const ConstraintIteratorRepn* other) const
        {
        if (sequences)
            return (constraint_sequences_iter == other->constraint_sequences_iter);
        else
            return (constraints_iter == other->constraints_iter);
        }

    reference operator*()
        {
        if (sequences)
            return *constraint_seq_iter;
        else
            return *constraints_iter;
        }

    const_reference operator*() const
        {
        if (sequences)
            return *constraint_seq_iter;
        else
            return *constraints_iter;
        }

    pointer operator->()
        {
        if (sequences)
            return constraint_seq_iter.operator->();
        else
            return constraints_iter.operator->();
        }

    const_pointer operator->() const
        {
        if (sequences)
            return constraint_seq_iter.operator->();
        else
            return constraints_iter.operator->();
        }
};
#endif


namespace visitors {

expr_pointer_t visit(SumExpressionTerm& arg)
{
auto it=arg.seq.begin();
if (it == arg.seq.end()) {
    return ZEROCONST;
    }

expr_pointer_t curr;
{
Expression e = *it;
++it;
for (; it != arg.seq.end(); ++it) {
    e += *it;
    }
curr = e.repn;
OWN_POINTER(curr);
}
FREE_POINTER(curr);
return curr;
}

}



//
// Sum
//
Expression Sum(const ExpressionSequence& seq)
{
Expression ans( CREATE_POINTER(SumExpressionTerm, seq) );
return ans;
}

//
// Model
//

bool Model::minimize = true;
bool Model::maximize = false;
double Model::inf = COEK_INFINITY;

std::ostream& operator<<(std::ostream& ostr, const Model& arg)
{
arg.print_equations(ostr);
return ostr;
}

void Model::print_equations() const
{ print_equations(std::cout); }

void Model::print_equations(std::ostream& ostr) const
{
ostr << "MODEL" << std::endl;
ostr << "  Objectives" << std::endl;
for (auto it=repn->objectives.begin(); it != repn->objectives.end(); ++it) {
    ostr << "    " << *it << std::endl;
    }
ostr << "  Constraints" << std::endl;
for (auto it=repn->constraints.begin(); it != repn->constraints.end(); ++it) {
    ostr << "    " << *it << std::endl;
    }
}

void Model::print_values() const
{ print_values(std::cout); }

void Model::print_values(std::ostream& ostr) const
{
ostr << "ERROR - Model::print_values is not implemented yet." << std::endl;
}

Model::Model()
{ repn = std::make_shared<ModelRepn>(); }

Model::~Model()
{}

Model::Model(const Model& other)
{ repn = other.repn; }

Model& Model::operator=(const Model& other)
{
repn = other.repn;
return *this;
}

Objective Model::add_objective(const Expression& expr, bool _sense)
{
Objective tmp(expr.repn, _sense);
repn->objectives.push_back(tmp);
return repn->objectives.back();
}

Constraint Model::add_constraint(const Constraint& expr)
{
repn->constraints.push_back(expr);
return expr;
}

Variable Model::add_variable(double lb, double ub, double value, bool binary, bool integer)
{
Variable tmp(lb,ub,value,binary,integer);
repn->variables.push_back(tmp);
return repn->variables.back();
}

Variable Model::add_variable(const std::string& name, double lb, double ub, double value, bool binary, bool integer)
{
Variable tmp(name,lb,ub,value,binary,integer);
repn->variables.push_back(tmp);
return repn->variables.back();
}

Variable Model::add_variable(Variable& var)
{
repn->variables.push_back(var);
return var;
}

void Model::add_variable(VariableArray& varray)
{
for (auto it=varray.variables.begin(); it != varray.variables.end(); it++) {
    repn->variables.push_back(*it);
    }
}

void Model::add_variable(ConcreteIndexedVariable& vars)
{
auto end = vars.end();
for (auto it=vars.begin(); it != end; ++it) {
    repn->variables.push_back(*it);
    }
}

Objective Model::get_objective(unsigned int i)
{
if (i > repn->objectives.size())
    throw std::out_of_range("Objective index " + std::to_string(i) + " is too large: " + std::to_string(repn->objectives.size()) + "       objectives available.");
return repn->objectives[i];
}

Constraint Model::get_constraint(unsigned int i)
{
if (i > repn->constraints.size())
    throw std::out_of_range("Constraint index " + std::to_string(i) + " is too large: " + std::to_string(repn->constraints.size()) + "      constraints available.");
return repn->constraints[i];
}

void Model::set_suffix(const std::string& name, Variable& var, double value)
{ repn->vsuffix[name][var.id()] = value; }

void Model::set_suffix(const std::string& name, Constraint& con, double value)
{ repn->csuffix[name][con.id()] = value; }

void Model::set_suffix(const std::string& name, Objective& obj, double value)
{ repn->osuffix[name][obj.id()] = value; }

void Model::set_suffix(const std::string& name, double value)
{ repn->msuffix[name] = value; }

double Model::get_suffix(const std::string& name, Variable& var)
{ return repn->vsuffix[name][var.id()]; }

double Model::get_suffix(const std::string& name, Constraint& con)
{ return repn->csuffix[name][con.id()]; }

double Model::get_suffix(const std::string& name, Objective& obj)
{ return repn->csuffix[name][obj.id()]; }

double Model::get_suffix(const std::string& name)
{ return repn->msuffix[name]; }

static bool endsWith(const std::string& str, const std::string& suffix)
{
    return str.size() >= suffix.size() && 0 == str.compare(str.size()-suffix.size(), suffix.size(), suffix);
}

void write_lp_problem(Model& model, std::ostream& ostr, std::map<int,int>& varmap, std::map<int,int>& conmap);
void write_lp_problem(CompactModel& model, std::ostream& ostr, std::map<int,int>& varmap, std::map<int,int>& conmap);
void write_nl_problem(Model& model, std::ostream& ostr, std::map<int,int>& varmap, std::map<int,int>& conmap);


void Model::write(std::string fname)
{
std::map<int,int> varmap;
std::map<int,int> conmap;
write(fname, varmap, conmap);
}

void Model::write(std::string fname, std::map<int,int>& varmap, std::map<int,int>& conmap)
{
if (endsWith(fname, ".lp")) {
    std::ofstream ofstr(fname);
    write_lp_problem(*this, ofstr, varmap, conmap);
    ofstr.close();
    return;
    }

else if (endsWith(fname, ".nl")) {
    std::ofstream ofstr(fname);
    write_nl_problem(*this, ofstr, varmap, conmap);
    ofstr.close();
    return;
    }

throw std::runtime_error("Unknown problem type");
}


//
// CompactModel
//

Objective CompactModel::add_objective(const Expression& expr, bool _sense)
{
Objective obj(expr, _sense);
objectives.push_back( obj );
return obj;
}

void CompactModel::add_objective(const ExpressionSequence& seq, bool _sense)
{
/*
objectives.push_back( seq );
sense.push_back( _sense );
*/
}

Constraint CompactModel::add_constraint(const Constraint& expr)
{
constraints.push_back(expr);
return expr;
}

void CompactModel::add_constraint(const ConstraintSequence& seq)
{
constraints.push_back(seq);
}

Variable CompactModel::add_variable(double lb, double ub, double value, bool binary, bool integer)
{
Variable tmp(lb,ub,value,binary,integer);
variables.push_back(tmp);
return variables.back();
}

Variable CompactModel::add_variable(const std::string& name, double lb, double ub, double value, bool binary, bool integer)
{
Variable tmp(name,lb,ub,value,binary,integer);
variables.push_back(tmp);
return variables.back();
}

Variable CompactModel::add_variable(Variable& var)
{
variables.push_back(var);
return var;
}

void CompactModel::add_variable(VariableArray& varray)
{
for (auto it=varray.variables.begin(); it != varray.variables.end(); it++) {
    variables.push_back(*it);
    }
}

void CompactModel::add_variable(ConcreteIndexedVariable& vars)
{
auto end = vars.end();
for (auto it=vars.begin(); it != end; ++it) {
    variables.push_back(*it);
    }
}

Model CompactModel::expand()
{
Model model;
model.repn->variables = variables;

int i=0;
for (auto it=objectives.begin(); it != objectives.end(); ++it) {
    /*
    * WEH - TODO rework this to use Objective objects

    auto& val = *it;
    bool osense = sense[i++];
    if (auto eval = std::get_if<Expression>(&val)) {
        Expression e = eval->expand();
        model.add(e, osense); //repn->objectives.push_back(e);
        //model.repn->sense.push_back(osense);
        }
    else {
#if 0
        auto& seq = std::get<ExpressionSequence>(val);
        for (auto jt=seq.begin(); jt != seq.end(); ++jt) {
            //model.repn->objectives.push_back(*jt);
            model.add( *jt, osense); //repn->objectives.push_back(*jt);
            //model.repn->sense.push_back(osense);
            }
#endif
        }
    */
    }

i=0;
for (auto it=constraints.begin(); it != constraints.end(); ++it) {
    auto& val = *it;
    if (auto cval = std::get_if<Constraint>(&val)) {
        Constraint c = cval->expand();
        model.repn->constraints.push_back(c);
        }
    else {
        auto& seq = std::get<ConstraintSequence>(val);
        for (auto jt=seq.begin(); jt != seq.end(); ++jt) {
            model.repn->constraints.push_back(*jt);
            }
        }
    }
return model;
}

void CompactModel::write(std::string fname)
{
std::map<int,int> varmap;
std::map<int,int> conmap;
write(fname, varmap, conmap);
}

void CompactModel::write(std::string fname, std::map<int,int>& varmap, std::map<int,int>& conmap)
{
if (endsWith(fname, ".lp")) {
    std::ofstream ofstr(fname);
    write_lp_problem(*this, ofstr, varmap, conmap);
    ofstr.close();
    return;
    }

Model model = expand();
model.write(fname, varmap, conmap);
}


//
// NLPModel
//

NLPModel::NLPModel()
{}

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

void NLPModel::reset()
{
if (repn == 0)
    return;
repn->reset();
}

size_t NLPModel::num_variables() const
{
if (repn == 0)
    return 0;
return repn->num_variables();
}

size_t NLPModel::num_objectives() const
{
if (repn == 0)
    return 0;
return repn->num_objectives();
}

size_t NLPModel::num_constraints() const
{
if (repn == 0)
    return 0;
return repn->num_constraints();
}

size_t NLPModel::num_nonzeros_Jacobian() const
{
if (repn == 0)
    return 0;
return repn->num_nonzeros_Jacobian();
}

size_t NLPModel::num_nonzeros_Hessian_Lagrangian() const
{
if (repn == 0)
    return 0;
return repn->num_nonzeros_Hessian_Lagrangian();
}

Variable NLPModel::get_variable(size_t i)
{
if (repn == 0)
    throw std::runtime_error("Calling get_variables() for uninitialized NLPModel.");
return repn->get_variable(i);
}

void NLPModel::set_variable(size_t i, const Variable& v)
{
if (repn == 0)
    throw std::runtime_error("Calling set_variables() for uninitialized NLPModel.");
repn->set_variable(i, v.repn);
}

void NLPModel::set_variables(std::vector<double>& x)
{
if (repn == 0)
    throw std::runtime_error("Calling set_variables() for uninitialized NLPModel.");
repn->set_variables(x);
}

void NLPModel::set_variables(const double* x, size_t n)
{
if (repn == 0)
    throw std::runtime_error("Calling set_variables() for uninitialized NLPModel.");
repn->set_variables(x, n);
}

Constraint NLPModel::get_constraint(size_t i)
{
if (repn == 0)
    throw std::runtime_error("Calling get_constraint() for uninitialized NLPModel.");
return repn->get_constraint(i);
}

void NLPModel::get_J_nonzeros(std::vector<size_t>& jrow, std::vector<size_t>& jcol)
{
if (repn == 0)
    throw std::runtime_error("Calling get_J_nonzeros() for uninitialized NLPModel.");
repn->get_J_nonzeros(jrow, jcol);
}

void NLPModel::get_H_nonzeros(std::vector<size_t>& hrow, std::vector<size_t>& hcol)
{
if (repn == 0)
    throw std::runtime_error("Calling get_H_nonzeros() for uninitialized NLPModel.");
repn->get_H_nonzeros(hrow, hcol);
}

double NLPModel::compute_f(unsigned int i)
{
if (repn == 0)
    throw std::runtime_error("Calling compute_f() for uninitialized NLPModel.");
return repn->compute_f(i);
}

void NLPModel::compute_df(double& f, std::vector<double>& df, unsigned int i)
{
if (repn == 0)
    throw std::runtime_error("Calling compute_df() for uninitialized NLPModel.");
repn->compute_df(f, df, i);
}

void NLPModel::compute_H(std::vector<double>& w, std::vector<double>& H)
{
if (repn == 0)
    throw std::runtime_error("Calling compute_H() for uninitialized NLPModel.");
repn->compute_H(w, H);
}

void NLPModel::compute_c(std::vector<double>& c)
{
if (repn == 0)
    throw std::runtime_error("Calling compute_c() for uninitialized NLPModel.");
repn->compute_c(c);
}

void NLPModel::compute_dc(std::vector<double>& dc, unsigned int i)
{
if (repn == 0)
    throw std::runtime_error("Calling compute_dc() for uninitialized NLPModel.");
repn->compute_dc(dc, i);
}

void NLPModel::compute_J(std::vector<double>& J)
{
if (repn == 0)
    throw std::runtime_error("Calling compute_J() for uninitialized NLPModel.");
repn->compute_J(J);
}

void NLPModel::write(std::string fname)
{
if (repn == 0)
    throw std::runtime_error("Calling write() for uninitialized NLPModel.");
std::map<int,int> varmap;
std::map<int,int> conmap;
repn->model.write(fname, varmap, conmap);
}

void NLPModel::write(std::string fname, std::map<int,int>& varmap, std::map<int,int>& conmap)
{
if (repn == 0)
    throw std::runtime_error("Calling write() for uninitialized NLPModel.");
repn->model.write(fname, varmap, conmap);
}

void NLPModel::print_equations() const
{ print_equations(std::cout); }

void NLPModel::print_equations(std::ostream& ostr) const
{
if (repn == 0)
    throw std::runtime_error("Calling print_equations() for uninitialized NLPModel.");
repn->print_equations(ostr);
}

void NLPModel::print_values() const
{ print_values(std::cout); }

void NLPModel::print_values(std::ostream& ostr) const
{
if (repn == 0)
    throw std::runtime_error("Calling print_values() for uninitialized NLPModel.");
repn->print_values(ostr);
}

std::ostream& operator<<(std::ostream& ostr, const NLPModel& arg)
{
arg.print_equations(ostr);
return ostr;
}


//
// Solver
//

void Solver::initialize(std::string name)
{
std::shared_ptr<SolverRepn> tmp(create_solver(name));
repn = tmp;
}

int Solver::solve(Model& model)
{ return repn->solve(model); }

int Solver::solve(CompactModel& model)
{ return repn->solve(model); }

void Solver::load(Model& model)
{ repn->load(model); }

void Solver::load(CompactModel& model)
{ repn->load(model); }

int Solver::resolve()
{ return repn->resolve(); }

void Solver::reset()
{ repn->reset(); }

bool Solver::get_option(const std::string& option, int& value) const
{ return repn->get_option(option, value); }
bool Solver::get_option(const std::string& option, double& value) const
{ return repn->get_option(option, value); }
bool Solver::get_option(const std::string& option, std::string& value) const
{ return repn->get_option(option, value); }
bool Solver::get_option(int option, int& value) const
{ return repn->get_option(option, value); }
bool Solver::get_option(int option, double& value) const
{ return repn->get_option(option, value); }
bool Solver::get_option(int option, std::string& value) const
{ return repn->get_option(option, value); }

void Solver::set_option(const std::string& option, int value)
{ repn->set_option(option, value); }
void Solver::set_option(const std::string& option, double value)
{ repn->set_option(option, value); }
void Solver::set_option(const std::string& option, const std::string value)
{ repn->set_option(option, value); }
void Solver::set_option(int option, int value)
{ repn->set_option(option, value); }
void Solver::set_option(int option, double value)
{ repn->set_option(option, value); }
void Solver::set_option(int option, const std::string value)
{ repn->set_option(option, value); }

//
// NLPSolver
//

void NLPSolver::initialize(std::string name)
{
std::shared_ptr<NLPSolverRepn> tmp(create_nlpsolver(name));
repn = tmp;
}

int NLPSolver::solve(NLPModel& model)
{ return repn->solve(model); }

void NLPSolver::load(NLPModel& model)
{ repn->load(model); }

int NLPSolver::resolve()
{ return repn->resolve(); }

void NLPSolver::reset()
{ repn->reset(); }

bool NLPSolver::get_option(const std::string& option, int& value) const
{ return repn->get_option(option, value); }
bool NLPSolver::get_option(const std::string& option, double& value) const
{ return repn->get_option(option, value); }
bool NLPSolver::get_option(const std::string& option, std::string& value) const
{ return repn->get_option(option, value); }
bool NLPSolver::get_option(int option, int& value) const
{ return repn->get_option(option, value); }
bool NLPSolver::get_option(int option, double& value) const
{ return repn->get_option(option, value); }
bool NLPSolver::get_option(int option, std::string& value) const
{ return repn->get_option(option, value); }

void NLPSolver::set_option(const std::string& option, int value)
{ repn->set_option(option, value); }
void NLPSolver::set_option(const std::string& option, double value)
{ repn->set_option(option, value); }
void NLPSolver::set_option(const std::string& option, const std::string value)
{ repn->set_option(option, value); }
void NLPSolver::set_option(int option, int value)
{ repn->set_option(option, value); }
void NLPSolver::set_option(int option, double value)
{ repn->set_option(option, value); }
void NLPSolver::set_option(int option, const std::string value)
{ repn->set_option(option, value); }


void check_that_expression_variables_are_declared(Model& model, const std::set<unsigned int>& var_ids)
{
std::set<unsigned int> model_ids;

auto end = model.repn->variables.end();
for (auto it=model.repn->variables.begin(); it != end; ++it)
    model_ids.insert( (*it).id() );

// TODO - Make this faster because both sets are ordered
for (auto it=var_ids.begin(); it != var_ids.end(); it++) {
    auto tmp = model_ids.find(*it);
    if (tmp == model_ids.end()) {
        throw std::runtime_error("Model expressions contain variables that are not declared in the model.");
        }
    }

// TODO - More diagnostic analysis here
}

}
