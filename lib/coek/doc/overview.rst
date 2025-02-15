Coek Overview
=============

Motivation
----------

Coek is a C++ library for expressing and solving optimization problems.
Coek enables users to express optimization models in C++, interface
directly with optimization solvers, and interact with solutions
generated by solvers.  The design of Coek was broadly inspired by the
object-oriented design in [Gurobi]_, but Coek is designed to support
the expression of mixed-integer nonlinear programs.

Coek is closely developed with the [Poek]_ library, and the goal of
these libraries is to support fast optimization modeling capabilities
in both C++ and Python.  For example:

* Coek can express large optimization problems in C++ nearly as fast [Gurobi]_,
* Coek integrates [CppAD]_ to compute derivatives for nonlinear problems,
* Coek and [Poek]_ include mutable parameters that allow model constants to be modified in place, and
* Poek can express large optimization problems in Python with modest overhead.

Coek development is being driven by targetted experiments and
demonstrations, often with runtime performance as a major driver.  Thus,
Coek is similar to [Gravity]_. Perhaps the biggest difference is that
Coek does not focus on vectorized representations. Poek is much simpler than [Pyomo]_, but
it can build models faster.

Although the Coek API has stabilized, it continues to evolve as new
capabilities are demonstrated.  Consequently, the following documentation
does not provide a detailed specification of the Coek API.  Rather,
we focus on examples that illustrate Coek capabilities.


Capabilities
------------

* Solvers

  * Ipopt - Coek can dynamically load the Ipopt C interface.
  * Gurobi - Coek compiles with the Gurobi C++ interface
  * Highs - Coek compiles with the Highs C++ interface

* Automatic Differentiation

  * CppAD and ASL are currently supported
  * There are general hooks in Coek for alternative AD packages

* PyCoek

  * Python wrappers are supported using pybind11 and cppyy
  * The [Poek]_ library simplifies the use of PyCoek for end-users

* Documentation

  * Sphinx, Doxygen and Breathe are used to generate documentation

