Coek API
========

.. note::
    The API for Coek continues to evolve.  Thus, this documentation contains many references to possible design
    changes and additional features.

Modeling Components
-------------------

There are three classes of modeling components in Coek:

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
  components rely on a compact expression of Coek expressions and constraints.

The APIs of these classes of modeling components are successively more
complex, so we successively describe each of these classes of modeling
components:

.. toctree::
   :maxdepth: 1

   components_elementary.rst
   components_indexed.rst

Sets, Models and Solvers
------------------------

.. toctree::
   :maxdepth: 1

   api_models.rst

