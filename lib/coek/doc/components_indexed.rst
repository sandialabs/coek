Indexed Components
==================

Overview
--------

Dense vs Sparse Indexing
~~~~~~~~~~~~~~~~~~~~~~~~

Indexed components generalize the declaration of elementary components
to declare multi-dimensional arrays and indexed associative arrays
of components.  The syntax for declaring indexed components is the
same across modeling components, but there is a fundamental distinction
between indexing strategies used for variable and parameter components,
and indexing used for constraints and objectives.  Variable and parameter
components represent mutable values, and properties, like variable
initial values and bounds can be readily applied across all indices.
When declaring a multi-dimensional array or associative array for
variables or parameters, it is intuitive to treat these as values defined
for all indices.  Thus, these are dense declarations, where components
are defined for all elements in the index set.

By contrast, declarations of constraints and objectives do not simply
have a value.  Thus, it is intuitive to declare an indexed component for
which the indices may be used sparsely.  That is, it is natural to only
use a subset of the declared indices in the indexed component.

Index Sets
~~~~~~~~~~

Associative arrays in Coek are declared using an index set, and Coek
includes several ways of declaring index sets.  The ``SetOf()`` function
converts integer data in an STL vector or initializer list into a Coek
set object:

.. code:: C++

    std::vector<int> v = {1,5,3,7};
    auto s = coek::SetOf( v );

    auto t = coek::SetOf( {1,5,3,7} );

Similarly, the ``RangeSet()`` function declares a sequence of integer
values with a specified start, stop and step value:

.. code:: C++

    auto s = coek::RangeSet(1, 10);     // 1, 2, 3, ..., 10
    auto t = coek::RangeSet(1, 10, 2);  // 1, 3, ..., 9

A variety of standard set operations are supported in Coek, include set
products and set difference:

.. code:: C++

    auto s = coek::RangeSet(1, 10);     // 1, 2, 3, ..., 10
    auto t = coek::RangeSet(1, 10, 2);  // 1, 3, ..., 9
    auto u = s - t;                     // 2, 4, ..., 10
    auto v = s * t;                     // (1, 1), (2, 1), (3, 1), ..., (10, 9)

See :ref:`api-sets` for further details.

.. warning::

    Associative arrays are currently only defined with configuring Coek
    using the ``with_compact`` build option.  This is likely to be the
    default build mode in the future, so these capabilities are not
    documented separately.

Variables
---------

A minimal variable declaration includes a name and/or indexing
information.  The following are basic examples:

.. code:: C++

    // A single continuous variable
    auto x = coek::variable();
    auto y = coek::variable("y");

    // An array of continuous variables of length 'n'
    size_t n=100;
    auto x = coek::variable(n);
    auto y = coek::variable("y", n);

    // A multi-dimensional array of continuous variables:  R^{2 x 3 x 5}
    auto x = coek::variable({2,3,4});
    auto y = coek::variable("y", {2,3,4});

    // A tensor of continuous variables indexed by Coek set objects
    auto A = coek::RangeSet(1,10);
    auto B = coek::RangeSet(11,20);
    auto x = coek::variable(A*B);
    auto y = coek::variable("y", A*B);

Variable declarations require the specification of various information:

* Lower bound values
* Upper bound values
* Initial values
* Variable type (continuous, binary, integer, etc)

Indexed variable declarations support function chaining for these specifications, which are applied to all
variables in the indexed component:

.. code:: C++

    auto x = coek::variable("x", A*B).
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

.. note::

    The use of function chaining for indexed variables simplifies the
    specification of common values across an indexed variable.  However,
    these values are set for each of the indexed variables, and the
    values of each indexed variable can be separately specified.  Thus,
    this notation does not imply that indexed variables are required to
    have consistent values for all indices.

Variables declared over sets can be indexed using the ``()`` operator in a natural manner.  For example:

.. code:: C++

    // An array of continuous variables of length 'n'
    size_t n=100;
    auto x = coek::variable(n);
    // Value of the 4th element of the array
    auto v = x(3).value();

    // A tensor of continuous variables:  R^{2 x 3 x 5}
    auto x = coek::variable({2,3,5});
    // Value of the variable indexed by (0,2,1)
    auto v = x(0,2,1).value();

    // A tensor of continuous variables indexed by Coek set objects
    auto A = coek::RangeSet(1,10);
    auto B = coek::RangeSet(11,20);
    auto x = coek::variable(A*B);
    // Value of the variable indexed by (1,11)
    auto v = x(1,11).value();

.. note::

    For historical reasons, it would be preferable to use the [] operator.
    However, this operator cannot be overloaded with C++ while allowing
    multiple subscripts.  This will change with C++23, but for now we
    restrict Coek to the use of operator() logic.

Note that arguments of the ``()`` operator may be constant expressions with mutable values.  For example, the
following are valid expressions:

.. code:: C++

    auto x = coek::variable(10);

    auto p = coek::parameter().value(1);
    x(p+1).value();         // The value of the x(2) 

    auto i = coek::set_element();
    x(i+1);                 // A reference to x(i+1), which is resolved in a quantified expression

The ``variable()`` function provides a uniform interface for declaring
both multi-dimensional arrays and associative arrays of variables.
The ``variable_array()`` and ``variable_map()`` functions can be used to
more explicitly declare these two types of indexed variables, but there
is no practical advantage for using these functions.  When iterating
over indices, there may be slight computational advantages for using
multi-dimensional arrays, which are stored compactly and thus are more
cache-efficient data structures for iteration.


Parameters
----------

Indexed parameters are declared in a similar manner to indexed variables:

.. code:: C++

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

    // A tensor of parameters indexed by Coek set objects
    auto A = coek::RangeSet(1,10);
    auto B = coek::RangeSet(11,20);
    auto p = coek::parameter(A*B);
    auto q = coek::parameter("q", A*B);

Note that parameter are always continuous, and their value defaults
to zero.  Initializing parameters can be similarly executed using
function chaining:

.. code:: C++

    // A single parameter initialized to 1.0
    auto q = coek::parameter("q").value(1.0);

    // An array of parameter of length 'n' initialized to 1.0
    size_t n=100;
    auto q = coek::parameter(n).value(1.0);

    // A tensor of parameters:  R^{2 x 3 x 5}, initialized to 1.0
    std::vector<size_t> dim = {2,3,5};
    auto q = coek::parameter("q", dim).value(1.0);

    // A tensor of parameters indexed by Coek set objects, initialized to 1.0
    auto A = coek::RangeSet(1,10);
    auto B = coek::RangeSet(11,20);
    auto q = coek::parameter("q", A*B).value(1.0);

The ``()`` operator also has the same behavior as for variable components.


Objectives
----------

Indexed objectives are not currently supported in Coek.

.. admonition:: WEH

    Although not often used, we could also support various ways to declare
    groups of objectives:

    .. code:: C++

        // A single objective
        auto a = coek::objective(2*x);
        auto b = coek::objective("b", 2*x);

        // An array of objectives
        size_t n=100;
        auto a = coek::objective(n);
        auto b = coek::objective("y", n);

        // A tensor of objectives:  R^{2 x 3 x 5}
        std::vector<size_t> dim = {2,3,5};
        auto a = coek::objective(dim);
        auto b = coek::objective("b", dim);

        // A tensor of objectives indexed by Coek set objects
        auto A = coek::RangeSet(1,10);
        auto B = coek::RangeSet(11,20);
        auto a = coek::objective(A*B);
        auto b = coek::objective("b", A*B);


Constraints
-----------

Indexed constraints are declared in a similar manner to indexed variables:

.. code:: C++

    // A single constraint
    auto a = coek::constraint(2*x == 0);
    auto b = coek::constraint("b", 2*x == 0);

    // An array of constraints
    size_t n=100;
    auto a = coek::constraint(n);
    auto b = coek::constraint("b", n);

    // A tensor of constraints:  R^{2 x 3 x 5}
    std::vector<size_t> dim = {2,3,5};
    auto a = coek::constraint(dim);
    auto b = coek::constraint("b", dim);

    // A tensor of constraints indexed by Coek set objects
    auto A = coek::RangeSet(1,10);
    auto B = coek::RangeSet(11,20);
    auto a = coek::constraint(A*B);
    auto b = coek::constraint("b", A*B);

A declaration of an indexed constraint indicates the space of possible
indices associated with the constraint, but only elementary constraints
have a specific value.  The ``()`` operator can be used to index
constraint objects and specify the constraint value:

.. code:: C++

    auto x = coek::variable(10);

    auto c = coek::constraint("c", 10);
    for (int i=0; i<10; i++)
        c(i) = (i+1)*x(i) <= i;
    model.add(c);

As noted earlier, not all indices need to be added to an indexed constraint:

.. code:: C++

    auto x = coek::variable(10);

    auto c = coek::constraint("c", {10,10});
    for (int i=0; i<10; i++)
        c(i,i) = (i+1)*x(i) <= i;
    model.add(c);


