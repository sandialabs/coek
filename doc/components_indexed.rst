Indexed Components
==================

Overview
--------

Dense vs Sparse Indexing
~~~~~~~~~~~~~~~~~~~~~~~~

Indexed components generalize the specification of elementary components
to declare multi-dimensional arrays and indexed associative arrays
of components.  There is fundamental distinction between indexing
strategies used for variable and parameter components, and indexing used
for constraints and objectives.  The key difference is that variable
and parameter components represent mutable values.  When declaring a
multi-dimensional array or associative array for variables or parameters,
it is intuitive to treat these as values defined for all indices.
Properties of the variable, like initial values and bounds can be readily
applied across all indices.

By contrast, declarations of constraints and objectives do not simply
have a value.  Thus, it is intuitive to declare an indexed component for
which the indices may be used sparsely.  That is, it is natural to only
use a subset of the declared indices in the indexed component.

Index Sets
~~~~~~~~~~

Associative arrays in COEK are declared using an index set, and COEK
includes several ways of declaring index sets.  The ``SetOf()`` function
converts integer data in an STL vector or initializer list into a COEK
set object:

.. code::

    std::vector<int> v = {1,5,3,7};
    auto s = coek::SetOf( v );

    auto t = coek::SetOf( {1,5,3,7} );

Similarly, the ``RangeSet()`` function declares a sequence of integer
values with a specified start, stop and step value:

.. code::

    auto s = coek::RangeSet(1, 10);     // 1, 2, 3, ..., 10
    auto t = coek::RangeSet(1, 10, 2);  // 1, 3, ..., 9

A variety of standard set operations are supported in COEK, include set
products and set difference:

.. code::

    auto s = coek::RangeSet(1, 10);     // 1, 2, 3, ..., 10
    auto t = coek::RangeSet(1, 10, 2);  // 1, 3, ..., 9
    auto u = s - t;                     // 2, 4, ..., 10
    auto v = s * t;                     // (1, 1), (2, 1), (3, 1), ..., (10, 9)

See :ref:`api-sets` for further details.


Variables
---------

A minimal variable specification includes a name and/or indexing
information.  The following are basic examples:

.. code::

    // A single continuous variable
    auto x = model.add( );
    auto y = model.add( coek::variable("y") );

    // An array of continuous variables of length 'n'
    size_t n=100;
    auto x = model.add( coek::variable(n) );
    auto y = model.add( coek::variable("y", n) );

    // A multi-dimensional array of continuous variables:  R^{2 x 3 x 5}
    auto x = model.add( coek::variable({2,3,4}) );
    auto y = model.add( coek::variable("y", {2,3,4}) );

    // A tensor of continuous variables indexed by COEK set objects
    auto A = coek::RangeSet(1,10);
    auto B = coek::RangeSet(11,20);
    auto x = model.add( coek::variable(A*B) );
    auto y = model.add( coek::variable("y", A*B) );

Variable declarations require the specification of various information:

* Lower bound values
* Upper bound values
* Initial values
* Variable type (continuous, binary, integer, etc)

Additionally, variable store information about whether they are fixed,
and it may make sense to declare variables as fixed.

The following syntax, using function chaining, provides an explicit
annotation of a variable's information:

.. code::

    auto x = model.add( coek::variable("x", A*B) ).
                    lower(2).
                    upper(10).
                    value(3).
                    within(coek::Integers);

Similarly, the ``Variable::bounds()`` function can be used instead of ``Variable::lower()`` and ``Variable::upper()``:

.. admonition:: WEH

    Specifying name and dimension of variables seems fundamental and
    something that would be done commonly, so I'm inclined to keep those
    arguments as part of the function:

    For example, the indexing option determines the type of variable
    object returned, so I think we need to include this and not treat it
    as something that is returned later.  (Yes, we could have a unified
    variable object ... but it's API would be much less clean IMHO.)

Variables declared over sets can be indexed using the ``()`` operator in a natural manner.  For example:

.. code::

    // An array of continuous variables of length 'n'
    size_t n=100;
    auto x = model.add( coek::variable(n) );
    // Value of the 4th element of the array
    auto v = x(3).value();

    // A tensor of continuous variables:  R^{2 x 3 x 5}
    auto x = model.add( coek::variable({2,3,5}) );
    // Value of the variable indexed by (0,2,1)
    auto v = x(0,2,1).value();

    // A tensor of continuous variables indexed by COEK set objects
    auto A = coek::RangeSet(1,10);
    auto B = coek::RangeSet(11,20);
    auto x = model.add( coek::variable(A*B) );
    // Value of the variable indexed by (1,11)
    auto v = x(1,11).value();

.. note::

    For historical reasons, it would be preferable to use the [] operator.
    However, this operator cannot be overloaded with C++ while allowing
    multiple subscripts.  This will change with C++23, but for now we
    restrict COEK to the use of operator() logic.


Parameters
----------

Indexed parameters are declared in a similar manner to indexed variables:

.. code::

    // A single parameter
    auto p = coek::parameter();
    auto q = coek::parameter("q");

    // An array of parameters of length 'n'
    size_t n=100;
    auto x = coek::parameter(n);
    auto q = coek::parameter("q", n);

    // A tensor of parameters:  R^{2 x 3 x 5}
    std::vector<size_t> dim = {2,3,5};
    auto x = coek::parameter(dim);
    auto q = coek::parameter("q", dim);

    // A tensor of parameters indexed by COEK set objects
    auto A = coek::RangeSet(1,10);
    auto B = coek::RangeSet(11,20);
    auto p = coek::parameter(A*B);
    auto q = coek::parameter("q", A*B);

Note that parameter are always continuous, and their value defaults
to zero.  Initializing parameters can be similarly executed using
function chaining:

.. code::

    // A single parameter initialized to 1.0
    auto q = coek::parameter("q").value(1.0);

    // An array of parameter of length 'n' initialized to 1.0
    size_t n=100;
    auto q = coek::parameter(n).value(1.0);

    // A tensor of parameters:  R^{2 x 3 x 5}, initialized to 1.0
    std::vector<size_t> dim = {2,3,5};
    auto q = coek::parameter("q", dim).value(1.0);

    // A tensor of parameters indexed by COEK set objects, initialized to 1.0
    auto A = coek::RangeSet(1,10);
    auto B = coek::RangeSet(11,20);
    auto q = coek::parameter("q", A*B).value(1.0);


Objectives
----------

Indexed objectives are not currently supported in COEK.

.. admonition:: WEH

    Although not often used, we could also support various ways to declare
    groups of objectives:

    .. code::

        // A single objective
        auto a = model.add( coek::objective(2*x) );
        auto b = model.add( coek::objective("b", 2*x) );

        // An array of objectives
        size_t n=100;
        auto a = model.add( coek::objective(n) );
        auto b = model.add( coek::objective("y", n) );

        // A tensor of objectives:  R^{2 x 3 x 5}
        std::vector<size_t> dim = {2,3,5};
        auto a = model.add( coek::objective(dim) );
        auto b = model.add( coek::objective("b", dim) );

        // A tensor of objectives indexed by COEK set objects
        auto A = coek::RangeSet(1,10);
        auto B = coek::RangeSet(11,20);
        auto a = model.add( coek::objective(A*B) );
        auto b = model.add( coek::objective("b", A*B) );


Constraints
-----------

Indexed constraints are declared in a similar manner to indexed variables:

.. code::

    // A single constraint
    auto a = model.add( coek::constraint(2*x == 0) );
    auto b = model.add( coek::constraint("b", 2*x == 0) );

    // An array of constraints
    size_t n=100;
    auto a = model.add( coek::constraint(n) );
    auto b = model.add( coek::constraint("b", n) );

    // A tensor of constraints:  R^{2 x 3 x 5}
    std::vector<size_t> dim = {2,3,5};
    auto a = model.add( coek::constraint(dim) );
    auto b = model.add( coek::constraint("b", dim) );

    // A tensor of constraints indexed by COEK set objects
    auto A = coek::RangeSet(1,10);
    auto B = coek::RangeSet(11,20);
    auto a = model.add( coek::constraint(A*B) );
    auto b = model.add( coek::constraint("b", A*B) );

A declaration of an indexed constraint indicates the space of possible
indices associated with the constraint, but only elementary constraints
have a specific value.  The ``()`` operator can be used to index
constraint objects and specify the constraint value:

.. code::

    auto x = model.add( coek::variable(10) );

    auto c = coek::constraint("c", 10);
    for (int i=0; i<10; i++) {
        c(i) = (i+1)*x(i) <= i;
        }
    model.add(c);

As noted earlier, not all indices need to be added to an indexed constraint:

.. code::

    auto x = model.add( coek::variable(10) );

    auto c = coek::constraint("c", {10,10});
    for (int i=0; i<10; i++) {
        c(i,i) = (i+1)*x(i) <= i;
        }
    model.add(c);
