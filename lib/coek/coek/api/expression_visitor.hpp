#pragma once

#include <iostream>
#include <vector>

namespace coek {

class VariableTerm;
class Expression;
class Objective;
class Constraint;

class QuadraticExpr {
   public:
    double constval;

    std::vector<VariableTerm*> linear_vars;
    std::vector<double> linear_coefs;

    std::vector<VariableTerm*> quadratic_lvars;
    std::vector<VariableTerm*> quadratic_rvars;
    std::vector<double> quadratic_coefs;

    QuadraticExpr() : constval(0.0) {}

    void reset();

    void collect_terms(const Expression& expr);
    void collect_terms(const Objective& expr);
    void collect_terms(const Constraint& expr);

    bool is_constant() const { return (linear_coefs.size() == 0) && (quadratic_coefs.size() == 0); }
    bool is_linear() const { return (linear_coefs.size() > 0) && (quadratic_coefs.size() == 0); }
    bool is_quadratic() const { return quadratic_coefs.size() > 0; }
};

class MutableNLPExpr {
   public:
    struct varterm_compare {
        bool operator()(const VariableTerm*, const VariableTerm*) const;
    };

   public:
    Expression constval;

    std::vector<VariableTerm*> linear_vars;
    std::vector<Expression> linear_coefs;

    std::vector<VariableTerm*> quadratic_lvars;
    std::vector<VariableTerm*> quadratic_rvars;
    std::vector<Expression> quadratic_coefs;

    std::set<VariableTerm*, varterm_compare> nonlinear_vars;
    Expression nonlinear;

    // std::unordered_set<coek::VariableTerm*> fixed_vars;
    // std::unordered_set<coek::ParameterTerm*> params;

    bool mutable_values;

    MutableNLPExpr();
    ~MutableNLPExpr();

    void collect_terms(Expression& expr);
    void collect_terms(Objective& expr);
    void collect_terms(Constraint& expr);

    bool is_mutable() { return mutable_values; }
};

std::ostream& operator<<(std::ostream& ostr, const QuadraticExpr& arg);

}  // namespace coek
