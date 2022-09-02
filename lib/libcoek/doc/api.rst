Coek API
========

Design Notes
------------

There are a variety of alternative API designs that would be reasonable
for specifying COEK variables, objectives and constraints.  The following
notes highlight different alternatives to help refine the final design
of COEK's API.

Consistency with POEK
~~~~~~~~~~~~~~~~~~~~~

Although it's desirable to have the COEK and POEK APIs be very similar,
Python is a more flexible language that allows for the following features:

* Named keyword arguments
* Non-typed arguments

Hence, Python can rely on introspecting and argument naming to determine
the semantics of the command-line arguments.  This is particularly
flexible in a user-oriented API, so the COEK C++ API will necessarily
be more constrained.

Getter/Setter
~~~~~~~~~~~~~

Currently, COEK relies on getter/setter methods to manage data in the API.
For example, COEK relies on methods with the following names:

* `void foo(arg value)` used for setting foo
* `value foo()` used for getting foo

.. note::

    These methods are less explicit than more traditional get_foo() and
    set_foo(), but perhaps better for function chaining (see examples
    below).

Modeling Components
-------------------

There are three classes of modeling components in COEK:

* Elementary Components - These are individual variables, parameters,
  constraints and objectives in the model.

* Indexed Components - These are groups of variables, parameters,
  constraints and objectives that can be referenced with a tuple of
  integers.  There are several different indexing strategies:

  * Multi-dimensional Array - The indices are members of
    a multi-dimensional array, where each dimension is 0-indexed.
    The size of the multi-dimensional array is specified with a tuple
    of per-dimension sizes.

  * Set or Product Set - The indices are members of a set or product
    set that is provided when declaring the indexed component.

* Component Sequence - A generator object creates a sequence of
  components, where an index tuple is associated with each component.  These
  components rely on a compact expression of COEK expressions and constraints.

The APIs of these classes of modeling components are successively more
complex, so we successively describe each of these classes of modeling
components:

.. toctree::
   :maxdepth: 1

   components_elementary.rst
   components_indexed.rst
   components_sequence.rst

Sets, Models and Solvers
------------------------

.. toctree::
   :maxdepth: 1

   api_models.rst
   api_sets.rst

