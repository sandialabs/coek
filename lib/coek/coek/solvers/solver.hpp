#pragma once

#include <coek/util/option_cache.hpp>
#include <coek/model/compact_model.hpp>
#include <coek/model/model.hpp>
#include <coek/model/nlp_model.hpp>

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

   public:
    /** Creates an empty solver */
    Solver(void) {}
    /** Creates a solver that is initialized using the solver name */
    Solver(std::string solver) { initialize(solver); }

    /** Initialize the solver with the specified name */
    void initialize(std::string solver);

    /** \returns \c true if the solver is available */
    bool available() const;

    /** Optimize the specified model
     *
     * \returns an error code that is nonzero if an error occurs */
    int solve(Model& model);
    /** Load a model */
    void load(Model& model);

#ifdef COEK_WITH_COMPACT_MODEL
    int solve(CompactModel& model);
    void load(CompactModel& model);
#endif

    /* Resolve a model that has been loaded
     *
     * \returns an error code that is nonzero if an error occurs */
    int resolve();
    /** Resets the state of the optimizer */
    void reset();

    /** Returns \c true if an error occurred */
    bool error_status() const;
    /** Returns the integer error code */
    int error_code() const;
    /** Returns a string error message*/
    std::string error_message() const;
};

/**
 * An optimization solver object that is initialized using the optimizer string name.
 *
 * These solvers can be used to optimize coek::NLPModel objects.
 */
class NLPSolver : public OptionCache {
   public:
    std::shared_ptr<NLPSolverRepn> repn;

   public:
    /** Creates an empty solver */
    NLPSolver(void) {}
    /** Creates a solver that is initialized using the solver name */
    NLPSolver(std::string solver) { initialize(solver); }

    /** Initialize the solver with the specified name */
    void initialize(std::string solver);

    /** \returns \c true if the solver is available */
    bool available() const;

    /** Optimize the specified model
     *
     * \returns an error code that is nonzero if an error occurs */
    int solve(NLPModel& model);

    /** Load a model */
    void load(NLPModel& model);
    /* Resolve a model that has been loaded
     *
     * \returns an error code that is nonzero if an error occurs */
    int resolve(bool reset_nlpmodel = true);
    /** Resets the state of the optimizer */
    void reset();

    /** Returns \c true if an error occurred */
    bool error_status() const;
    /** Returns the integer error code */
    int error_code() const;
    /** Returns a string error message*/
    std::string error_message() const;
};

}  // namespace coek
