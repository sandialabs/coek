#pragma once

#include <memory>
#include <vector>
#include <variant>
#include "coek_expr.hpp"
#include "coek_sets.hpp"
#include "coek_indexed.hpp"

namespace coek {

class VariableArray;
class ModelRepn;
class NLPModelRepn;
class SolverRepn;
class NLPSolverRepn;
class ExpressionSeqIteratorRepn;
class ConstraintSeqIteratorRepn;


class ExpressionSeqIterator
{
public:

    std::shared_ptr<ExpressionSeqIteratorRepn> repn;

    typedef Expression* pointer;
    typedef const Expression* const_pointer;
    typedef Expression& reference;
    typedef const Expression& const_reference;

public:

    ExpressionSeqIterator();
    ExpressionSeqIterator(ExpressionSequenceRepn* seq, bool end=false);

    ExpressionSeqIterator& operator++();

    bool operator==(const ExpressionSeqIterator& other) const;
    bool operator!=(const ExpressionSeqIterator& other) const;

    reference operator*();
    const_reference operator*() const;
    pointer operator->();
    const_pointer operator->() const;
};


class ExpressionSequence
{
public:

  std::shared_ptr<ExpressionSequenceRepn> repn;

public:

    ExpressionSequence(const std::shared_ptr<ExpressionSequenceRepn>& _repn);

    template <typename... TYPES>
    ExpressionSequenceAux forall(const TYPES&... args)
        {
        std::vector<IndexParameter> arg;
        collect_args(args..., arg);
        return forall(arg);
        }
    
    ExpressionSequenceAux forall(const std::vector<IndexParameter>& params);
    ExpressionSequence st(const Constraint& con);
    ExpressionSequence where(const Constraint& con);

    ExpressionSeqIterator begin();
    ExpressionSeqIterator end();

protected:
    
    void collect_args(const IndexParameter& arg, std::vector<IndexParameter>& _arg)
        {
        _arg.emplace_back(arg);
        }

    template <typename... TYPES>
    void collect_args(const IndexParameter& arg, const TYPES&... args, std::vector<IndexParameter>&  _arg)
        {
        _arg.emplace_back(arg);
        collect_args(args..., _arg);
        }
};


class ConstraintSeqIterator
{
public:

    std::shared_ptr<ConstraintSeqIteratorRepn> repn;

    typedef Constraint* pointer;
    typedef const Constraint* const_pointer;
    typedef Constraint& reference;
    typedef const Constraint& const_reference;

public:

    ConstraintSeqIterator();
    ConstraintSeqIterator(ConstraintSequenceRepn* seq, bool end=false);

    ConstraintSeqIterator& operator++();

    bool operator==(const ConstraintSeqIterator& other) const;
    bool operator!=(const ConstraintSeqIterator& other) const;

    reference operator*();
    const_reference operator*() const;
    pointer operator->();
    const_pointer operator->() const;
};


class ConstraintSequence
{
public:

  std::shared_ptr<ConstraintSequenceRepn> repn;

public:

    ConstraintSequence(const std::shared_ptr<ConstraintSequenceRepn>& _repn);

    template <typename... TYPES>
    ConstraintSequenceAux forall(const TYPES&... args)
        {
        std::vector<IndexParameter> arg;
        collect_args(args..., arg);
        return forall(arg);
        }
    
    ConstraintSequenceAux forall(const std::vector<IndexParameter>& params);
    ConstraintSequence st(const Constraint& con);
    ConstraintSequence where(const Constraint& con);

    ConstraintSeqIterator begin();
    ConstraintSeqIterator end();

protected:
    
    void collect_args(const IndexParameter& arg, std::vector<IndexParameter>& _arg)
        {
        _arg.emplace_back(arg);
        }

    template <typename... TYPES>
    void collect_args(const IndexParameter& arg, const TYPES&... args, std::vector<IndexParameter>&  _arg)
        {
        _arg.emplace_back(arg);
        collect_args(args..., _arg);
        }
};


// BAD: This function doesn't logically belong in this header
Expression Sum(const ExpressionSequence& seq);


// TODO - Move to *.cpp file
class ModelRepn
{
public:

    std::vector<bool> sense;
    std::vector<Expression> objectives;
    std::vector<Constraint> constraints;
    std::vector<Variable> variables;
};

//
// Coek Model
//
// TODO - Define SimpeModel and AbstractModel.  Operations like get_constraint(i) may not make sense for
// an abstract model, and even when they do, these are probably more expensive operations.
//
class Model
{
public:

    static bool minimize;
    static bool maximize;
    static double inf;

    std::shared_ptr<ModelRepn> repn;

public:

    Model();
    Model(const Model& other);
    ~Model();
    Model& operator=(const Model&);

    void add(const Expression& expr, bool _sense=Model::minimize);
    void add(const Constraint& expr);

    Variable& getVariable(double lb, double ub, const std::string& name);
    Variable& getVariable(double lb=-COEK_INFINITY, double ub=COEK_INFINITY, double value=COEK_NAN);
    Variable& getVariable(double lb, double ub, double value, const std::string& name);
    Variable& getVariable(double lb, double ub, double value, bool binary, bool integer);
    Variable& getVariable(double lb, double ub, double value, bool binary, bool integer, const std::string& name);
    void addVariable(Variable& var);
    void addVariable(VariableArray& var);
    void addVariable(ConcreteIndexedVariable& var);

    Expression get_objective(unsigned int i=0);
    Constraint get_constraint(unsigned int i);

    void write(std::string ostr);

    friend std::ostream& operator<<(std::ostream& ostr, const Model& arg);
};


class CompactModel
{
public:

    std::vector<bool> sense;
    std::vector<std::variant<Expression, ExpressionSequence>> objectives;
    std::vector<std::variant<Constraint, ConstraintSequence>> constraints;
    std::vector<Variable> variables;

public:

    void add(const Expression& expr, bool _sense=Model::minimize);
    void add(const ExpressionSequence& seq, bool _sense=Model::minimize);
    void add(const Constraint& expr);
    void add(const ConstraintSequence& seq);

    Variable& getVariable(double lb, double ub, const std::string& name);
    Variable& getVariable(double lb=-COEK_INFINITY, double ub=COEK_INFINITY, double value=COEK_NAN);
    Variable& getVariable(double lb, double ub, double value, const std::string& name);
    Variable& getVariable(double lb, double ub, double value, bool binary, bool integer);
    Variable& getVariable(double lb, double ub, double value, bool binary, bool integer, const std::string& name);

    void addVariable(Variable& var);
    void addVariable(VariableArray& var);
    void addVariable(ConcreteIndexedVariable& var);

    Model expand();

    void write(std::string ostr);
};


class Solver
{
public:

    std::shared_ptr<SolverRepn> repn;

public:

    Solver(void)
        {}
    Solver(std::string solver)
        {initialize(solver);}

    void initialize(std::string solver);

    int solve(Model& model);
    int solve(CompactModel& model);

    void load(Model& model);
    void load(CompactModel& model);
    int resolve(bool reset_initial_point=true);
    void reset();

    // Solver Options

    bool get_option(const std::string& option, int& value) const;
    bool get_option(const std::string& option, double& value) const;
    bool get_option(const std::string& option, std::string& value) const;
    bool get_option(int option, int& value) const;
    bool get_option(int option, double& value) const;
    bool get_option(int option, std::string& value) const;

    void set_option(const std::string& option, int value);
    void set_option(const std::string& option, double value);
    void set_option(const std::string& option, const std::string value);
    void set_option(int option, int value);
    void set_option(int option, double value);
    void set_option(int option, const std::string value);

};


// Coek NLPModel
class NLPModel
{
public:

    std::shared_ptr<NLPModelRepn> repn;

public:

    NLPModel();
    NLPModel(Model& model, std::string type, bool sparse_JH=true);

    void initialize(Model& model, std::string type, bool sparse_JH=true);
    void reset();   // TODO - maybe this should be called 'update'?

    size_t num_variables() const;
    size_t num_objectives() const;
    size_t num_constraints() const;
    size_t num_nonzeros_Jacobian() const;
    size_t num_nonzeros_Hessian_Lagrangian() const;

    Variable get_variable(size_t i);
    void set_variable(size_t i, const Variable& v);

    void set_variables(std::vector<double>& x);
    void set_variables(const double* x, size_t n);

    void get_J_nonzeros(std::vector<size_t>& jrow, std::vector<size_t>& jcol);
    void get_H_nonzeros(std::vector<size_t>& hrow, std::vector<size_t>& hcol);

    void write(std::ostream& ostr) const;

    friend std::ostream& operator<<(std::ostream& ostr, const NLPModel& arg);

public:

    /// COMPUTE Value of Specific Objective Function

    double compute_f(unsigned int i=0);

    double compute_f(std::vector<double>& x)
        {
        set_variables(x);
        return compute_f(0);
        }

    double compute_f(std::vector<double>& x, unsigned int i)
        {
        set_variables(x);
        return compute_f(i);
        }

    /// COMPUTE Gradient of Specified Objective Function

    void compute_df(double& f, std::vector<double>& df, unsigned int i);

    void compute_df(std::vector<double>& x, std::vector<double>& df)
        {
        set_variables(x);
        double f;
        compute_df(f, df, 0);
        }

    void compute_df(std::vector<double>& x, std::vector<double>& df, unsigned int i)
        {
        set_variables(x);
        double f;
        compute_df(f, df, i);
        }

    void compute_df(double& f, std::vector<double>& df)
        { compute_df(f, df, 0); }

    void compute_df(std::vector<double>& df)
        { double f; compute_df(f, df, 0); }

    void compute_df(std::vector<double>& df, unsigned int i)
        { double f; compute_df(f, df, i); }

    /// COMPUTE Hessian of Lagrangian

    void compute_H(std::vector<double>& w, std::vector<double>& H);

    void compute_H(std::vector<double>& x, std::vector<double>& w, std::vector<double>& H)
        {
        set_variables(x);
        compute_H(w, H);
        }

    /// COMPUTE Constraint Values

    void compute_c(std::vector<double>& c);

    void compute_c(std::vector<double>& x, std::vector<double>& c)
        {
        set_variables(x);
        compute_c(c);
        }

    /// COMPUTE Gradient of Specific Constraint

    void compute_dc(std::vector<double>& dc, unsigned int i);

    void compute_dc(std::vector<double>& x, std::vector<double>& dc, unsigned int i)
        {
        set_variables(x);
        compute_dc(dc, i);
        }

    /// COMPUTE Jacobian

    void compute_J(std::vector<double>& J);

    void compute_J(std::vector<double>& x, std::vector<double>& J)
        {
        set_variables(x);
        compute_J(J);
        }
}; 


// Coek NLPSolver
class NLPSolver
{
public:

    std::shared_ptr<NLPSolverRepn> repn;

public:

    NLPSolver(void)
        {}
    NLPSolver(std::string solver)
        {initialize(solver);}

    void initialize(std::string solver);

    int solve(NLPModel& model);

    void load(NLPModel& model);
    int resolve(bool reset_initial_point=true);
    void reset();
    
    // Solver Options

    bool get_option(const std::string& option, int& value) const;
    bool get_option(const std::string& option, double& value) const;
    bool get_option(const std::string& option, std::string& value) const;
    bool get_option(int option, int& value) const;
    bool get_option(int option, double& value) const;
    bool get_option(int option, std::string& value) const;

    void set_option(const std::string& option, int value);
    void set_option(const std::string& option, double value);
    void set_option(const std::string& option, const std::string value);
    void set_option(int option, int value);
    void set_option(int option, double value);
    void set_option(int option, const std::string value);

};


//
// operator<<
//
std::ostream& operator<<(std::ostream& ostr, const Model& arg);
std::ostream& operator<<(std::ostream& ostr, const NLPModel& arg);

}
