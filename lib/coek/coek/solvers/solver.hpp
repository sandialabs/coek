#pragma once

#include <memory>
#include <coek/util/option_cache.hpp>
#include <coek/model/compact_model.hpp>
#include <coek/model/model.hpp>
#include <coek/model/nlp_model.hpp>
#include <coek/solvers/solver_results.hpp>

namespace coek {

class SolverRepn;
class NLPSolverRepn;

/**
 * An optimization solver object that is initialized using the optimizer string name.
 *
 * These solvers can be used to optimize coek::Model objects.
 */
class Solver : public OptionCache {
   public:
    std::shared_ptr<SolverRepn> repn;
    std::string name;

   public:
    /** Creates an empty solver */
    Solver(void) {}
    /** Creates a solver that is initialized using the solver name */
    Solver(std::string solver) { initialize(solver); }

    /** Initialize the solver with the specified name */
    void initialize(std::string solver);

    /** \returns \c true if the solver is available */
    virtual bool available();

    /** Optimize the specified model
     *
     * \returns an error code that is nonzero if an error occurs */
    std::shared_ptr<SolverResults> solve(Model& model);
    /** Load a model */
    void load(Model& model);

#ifdef COEK_WITH_COMPACT_MODEL
    std::shared_ptr<SolverResults> solve(CompactModel& model);
    void load(CompactModel& model);
#endif

    /* Resolve a model that has been loaded
     *
     * \returns an error code that is nonzero if an error occurs */
    std::shared_ptr<SolverResults> resolve();
    /** Resets the state of the optimizer */
    void reset();
};

/**
 * An optimization solver object that is initialized using the optimizer string name.
 *
 * These solvers can be used to optimize coek::NLPModel objects.
 */
class NLPSolver : public OptionCache {
   public:
    std::shared_ptr<NLPSolverRepn> repn;
    std::string name;

   public:
    /** Creates an empty solver */
    NLPSolver(void) {}
    /** Creates a solver that is initialized using the solver name */
    NLPSolver(std::string solver) { initialize(solver); }

    /** Initialize the solver with the specified name */
    void initialize(std::string solver);

    /** \returns \c true if the solver is available */
    bool available();

    /** Optimize the specified model
     *
     * \returns an error code that is nonzero if an error occurs */
    std::shared_ptr<SolverResults> solve(NLPModel& model);

    /** Load a model */
    void load(NLPModel& model);
    /* Resolve a model that has been loaded */
    std::shared_ptr<SolverResults> resolve(bool reset_nlpmodel = true);
    /** Resets the state of the optimizer */
    void reset();
};

}  // namespace coek
