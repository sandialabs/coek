Poek Documentation
==================

Overview
--------

Poek is a Python library for expressing and solving optimization problems.
Poek enables users to express optimization models in Python, interface
directly with optimization solvers, and interact with solutions generated
by solvers.  The design of Poek was broadly inspired by the design of
[Pyomo]_ and the Python interface for [Gurobi]_, though Poek is designed
to support the expression of mixed-integer nonlinear programs.

Poek is closely developed with the [Coek]_ library, and the goal of
these libraries is to support fast optimization modeling capabilities
in both C++ and Python.  For example:

* Poek interfaces with [Coek]_ and integrates [CppAD]_ to compute derivatives for nonlinear problems,
* Poek and [Coek]_ include mutable parameters that allow model constants to be  modified in place, and
* Poek can express large optimization problems in Python with modest overhead.

Poek and [Coek]_ do not currently include high-level modeling abstractions
supported by [Pyomo]_. However, this this is not a fundamental limitation
of the design of these libraries.  For example, preliminary methods for
expressing models with compact syntax have been developed.

Poek development is being driven by targetted experiments and
demonstrations, often with runtime performance as a major driver.
Although the Poek API has stabilized, it continues to evolve as new
capabilities are demonstrated.  Consequently, the following documentation
does not provide a detailed specification of the Poek API.  Rather,
we focus on examples that illustrate Poek capabilities.


.. toctree::
   :maxdepth: 2

   examples.rst
   reference.rst
   bibliography.rst


Indices and Tables
------------------

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`
