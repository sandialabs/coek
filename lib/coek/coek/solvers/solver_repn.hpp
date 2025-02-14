#pragma once

#include <map>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <memory>

#include <coek/util/option_cache.hpp>
#include "coek/api/expression_visitor.hpp"
#include "coek/model/model.hpp"
#include "coek/model/compact_model.hpp"
#include "coek/solvers/solver_results.hpp"

namespace coek {

class NLPModel;

class SolverCache {
   public:
    std::unordered_map<VariableTerm*, double> vcache;
    std::unordered_map<ParameterTerm*, double> pcache;

    std::unordered_set<VariableTerm*> vupdates;
    std::unordered_set<ParameterTerm*> pupdates;

    //
    double tolerance;

   public:
    SolverCache(void) : tolerance(1e-12), initial(true) {}

    virtual void find_updated_values();

    virtual void reset_cache();

   protected:
    //
    bool initial;
};

//
// The base solver class that defines the API used by Python
//
class SolverRepn : public SolverCache, public OptionCache {
   public:
    Model model;

    std::vector<MutableNLPExpr> repn;

    std::unordered_map<VariableTerm*, std::set<size_t> > vconstvals;
    std::unordered_map<ParameterTerm*, std::set<size_t> > pconstvals;

    std::unordered_map<VariableTerm*, std::set<std::tuple<size_t, size_t> > > vlinvals;
    std::unordered_map<ParameterTerm*, std::set<std::tuple<size_t, size_t> > > plinvals;

    std::unordered_map<VariableTerm*, std::set<std::tuple<size_t, size_t> > > vquadvals;
    std::unordered_map<ParameterTerm*, std::set<std::tuple<size_t, size_t> > > pquadvals;

    std::unordered_map<VariableTerm*, std::set<size_t> > vnonlvals;
    std::unordered_map<ParameterTerm*, std::set<size_t> > pnonlvals;

    std::set<std::tuple<size_t, size_t, size_t> > updated_coefs;

   public:
    SolverRepn(void) : SolverCache() {}
    virtual ~SolverRepn() {}

    void set_options(OptionCache& _options) { options = _options.options; }

    virtual std::shared_ptr<SolverResults> solve(Model& model) = 0;
    virtual void load(Model& model);

#ifdef COEK_WITH_COMPACT_MODEL
    virtual std::shared_ptr<SolverResults> solve(CompactModel& model);
    virtual void load(CompactModel& model);
#endif

    virtual void reset();
    bool initial_solve();
    virtual std::shared_ptr<SolverResults> resolve() = 0;

    void find_updated_coefs();

    virtual bool available() { return true; }
};

SolverRepn* create_solver(std::string& name, OptionCache& options);

class NLPSolverRepn : public SolverCache, public OptionCache {
   public:
    NLPModel* model;

   public:
    NLPSolverRepn() : SolverCache(), model(0) {}
    virtual ~NLPSolverRepn() {}

    void set_options(OptionCache& _options) { options = _options.options; }

    virtual void load(NLPModel& _model) { model = &_model; }

    virtual void reset() {}

    virtual bool initial_solve()
    {
        if (initial) {
            initial = false;
            return true;
        }
        else
            return false;
    }
    virtual std::shared_ptr<SolverResults> solve(NLPModel& model) = 0;
    std::shared_ptr<SolverResults> resolve(bool reset_nlpmodel);
    virtual std::shared_ptr<SolverResults> resolve_exec() = 0;

    virtual bool available() { return true; }
};

NLPSolverRepn* create_nlpsolver(std::string& name, OptionCache& options);

}  // namespace coek
