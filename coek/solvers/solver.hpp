#pragma once

#include <coek/model/model.hpp>
#include <coek/model/nlp_model.hpp>
#include <coek/model/compact_model.hpp>


namespace coek {

class SolverRepn;
class NLPSolverRepn;


/**
  * An optimization solver object that is initialized using the optimizer string name.
  *
  * These solvers can be used to optimize coek::Model objects.
  */
class Solver
{
public:

    std::shared_ptr<SolverRepn> repn;

public:

    /** Creates an empty solver */
    Solver(void)
        {}
    /** Creates a solver that is initialized using the solver name */
    Solver(std::string solver)
        {initialize(solver);}

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

    // Solver Options

    /** Get the value of an integer option
      *
      * The option value is returned by reference if it has
      * a value.
      *
      * \param option  the option name
      * \param value   an integer value that is passed by reference
      *
      * \returns \c true if the option is found
      */
    bool get_option(const std::string& option, int& value) const;
    /** Get the value of a double option
      *
      * The option value is returned by reference if it has
      * a value.
      *
      * \param option  the option name
      * \param value   a double value that is passed by reference
      *
      * \returns \c true if the option is found
      */
    bool get_option(const std::string& option, double& value) const;
    /** Get the value of a string option
      *
      * The option value is returned by reference if it has
      * a value.
      *
      * \param option  the option name
      * \param value   a string value that is passed by reference
      *
      * \returns \c true if the option is found
      */
    bool get_option(const std::string& option, std::string& value) const;
    /** Get the value of an integer option
      *
      * The option value is returned by reference if it has
      * a value.
      *
      * \param option  the option integer id
      * \param value   an integer value that is passed by reference
      *
      * \returns \c true if the option is found
      */
    bool get_option(int option, int& value) const;
    /** Get the value of a double option
      *
      * The option value is returned by reference if it has
      * a value.
      *
      * \param option  the option integer id
      * \param value   a double value that is passed by reference
      *
      * \returns \c true if the option is found
      */
    bool get_option(int option, double& value) const;
    /** Get the value of a string option
      *
      * The option value is returned by reference if it has
      * a value.
      *
      * \param option  the option integer id
      * \param value   a string value that is passed by reference
      *
      * \returns \c true if the option is found
      */
    bool get_option(int option, std::string& value) const;

    /** Set an integer option
      *
      * \param option  the option name
      * \param value   the integer value
      */
    void set_option(const std::string& option, int value);
    /** Set a double option
      *
      * \param option  the option name
      * \param value   the double value
      */
    void set_option(const std::string& option, double value);
    /** Set a string option
      *
      * \param option  the option name
      * \param value   the string value
      */
    void set_option(const std::string& option, const std::string value);
    /** Set an integer option
      *
      * \param option  the option integer id
      * \param value   the integer value
      */
    void set_option(int option, int value);
    /** Set a double option
      *
      * \param option  the option integer id
      * \param value   the double value
      */
    void set_option(int option, double value);
    /** Set a string option
      *
      * \param option  the option integer id
      * \param value   the string value
      */
    void set_option(int option, const std::string value);

};


/**
  * An optimization solver object that is initialized using the optimizer string name.
  *
  * These solvers can be used to optimize coek::NLPModel objects.
  */
class NLPSolver
{
public:

    std::shared_ptr<NLPSolverRepn> repn;

public:

    /** Creates an empty solver */
    NLPSolver(void)
        {}
    /** Creates a solver that is initialized using the solver name */
    NLPSolver(std::string solver)
        {initialize(solver);}

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
    int resolve();
    /** Resets the state of the optimizer */
    void reset();
    
    /** Get the value of an integer option
      *
      * The option value is returned by reference if it has
      * a value.
      *
      * \param option  the option name
      * \param value   an integer value that is passed by reference
      *
      * \returns \c true if the option is found
      */
    bool get_option(const std::string& option, int& value) const;
    /** Get the value of a double option
      *
      * The option value is returned by reference if it has
      * a value.
      *
      * \param option  the option name
      * \param value   a double value that is passed by reference
      *
      * \returns \c true if the option is found
      */
    bool get_option(const std::string& option, double& value) const;
    /** Get the value of a string option
      *
      * The option value is returned by reference if it has
      * a value.
      *
      * \param option  the option name
      * \param value   a string value that is passed by reference
      *
      * \returns \c true if the option is found
      */
    bool get_option(const std::string& option, std::string& value) const;
    /** Get the value of an integer option
      *
      * The option value is returned by reference if it has
      * a value.
      *
      * \param option  the option integer id
      * \param value   an integer value that is passed by reference
      *
      * \returns \c true if the option is found
      */
    bool get_option(int option, int& value) const;
    /** Get the value of a double option
      *
      * The option value is returned by reference if it has
      * a value.
      *
      * \param option  the option integer id
      * \param value   a double value that is passed by reference
      *
      * \returns \c true if the option is found
      */
    bool get_option(int option, double& value) const;
    /** Get the value of a string option
      *
      * The option value is returned by reference if it has
      * a value.
      *
      * \param option  the option integer id
      * \param value   a string value that is passed by reference
      *
      * \returns \c true if the option is found
      */
    bool get_option(int option, std::string& value) const;

    /** Set an integer option
      *
      * \param option  the option name
      * \param value   the integer value
      */
    void set_option(const std::string& option, int value);
    /** Set a double option
      *
      * \param option  the option name
      * \param value   the double value
      */
    void set_option(const std::string& option, double value);
    /** Set a string option
      *
      * \param option  the option name
      * \param value   the string value
      */
    void set_option(const std::string& option, const std::string value);
    /** Set an integer option
      *
      * \param option  the option integer id
      * \param value   the integer value
      */
    void set_option(int option, int value);
    /** Set a double option
      *
      * \param option  the option integer id
      * \param value   the double value
      */
    void set_option(int option, double value);
    /** Set a string option
      *
      * \param option  the option integer id
      * \param value   the string value
      */
    void set_option(int option, const std::string value);

};


}
