#pragma once

#include <iostream>
#include <vector>

namespace coek {

class ParameterTerm;
class IndexParameterTerm;
class VariableTerm;
class BaseExpressionTerm;
class SubExpressionTerm;

typedef std::shared_ptr<ParameterTerm> ParameterRepn;
typedef std::shared_ptr<IndexParameterTerm> IndexParameterRepn;
typedef std::shared_ptr<VariableTerm> VariableRepn;
typedef std::shared_ptr<BaseExpressionTerm> ExpressionRepn;
typedef std::shared_ptr<SubExpressionTerm> SubExpressionRepn;

class VariableTerm;
class Expression;
class Objective;
class Constraint;

class QuadraticExpr {
   public:
    double constval;

    std::vector<VariableRepn> linear_vars;
    std::vector<double> linear_coefs;

    std::vector<VariableRepn> quadratic_lvars;
    std::vector<VariableRepn> quadratic_rvars;
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
        bool operator()(const VariableRepn&, const VariableRepn&) const;
    };

   public:
    ExpressionRepn constval;

    std::vector<VariableRepn> linear_vars;
    std::vector<ExpressionRepn> linear_coefs;

    std::vector<VariableRepn> quadratic_lvars;
    std::vector<VariableRepn> quadratic_rvars;
    std::vector<ExpressionRepn> quadratic_coefs;

    std::set<VariableRepn, varterm_compare> nonlinear_vars;
    ExpressionRepn nonlinear;

    bool mutable_values;

    MutableNLPExpr();

    void collect_terms(Expression& expr);
    void collect_terms(Objective& expr);
    void collect_terms(Constraint& expr);

    bool is_mutable() { return mutable_values; }
};

}  // namespace coek

namespace std {

std::ostream& operator<<(std::ostream& ostr, const coek::QuadraticExpr& arg);
std::ostream& operator<<(std::ostream& ostr, const coek::MutableNLPExpr& arg);

}  // namespace std
