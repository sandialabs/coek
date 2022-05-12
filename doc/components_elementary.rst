Elementary Components
=====================

Variables
---------

In COEK and POEK, variables are not owned by a model.  However, it is
necessary to associate a variable with a model to facilitate efficient
processing of models.  Thus, the following are equivalent:

.. code::

    auto x = coek::variable("x");
    model.add(x);

and

.. code::

    auto x = model.add( coek::variable("x") );

The ``Model::add()`` method adds a variable object to the model, and the variable
object is returned.  This supports a concise declaration of variables,
and it enables the user to directly reference the variable object.
Further, this enables functional chaining to configure the variable.

.. admonition:: Question

    Should we include an `Model::add_variable()` method that replicates
    the API of the `coek::variable()` method?  This would be redundant
    in the API, but it would allow for shorter end-user code.


A minimal variable specification may include the name:

.. code::

    // A single continuous variable
    auto y = model.add( coek::variable() );
    auto y = model.add( coek::variable("y") );

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

    auto x = model.add( coek::variable("x") ).
                    lower(2).
                    upper(10).
                    value(3).
                    within(coek::Integers);

Similarly, the ``Variable::bounds()`` function can be used instead of ``Variable::lower()`` and ``Variable::upper()``:

.. code::

    auto x = model.add( coek::variable("x") ).
                    bounds(2, 10).
                    value(3).
                    within(coek::Integers);

.. admonition:: WEH

    This function chaining requires methods where the set- and
    get-semantics are dependent on the method used.  For example:

        x.value(10);

    sets the value of x, while

        auto val = x.value();

    returns the value of x.  The use of `set_*` methods seems desirable,
    but that leads to a verbose syntax that clutters the specification
    of variable properties.

.. admonition:: Question

    I think it's reasonable to limit the specification for 'within'
    to enumeration types.  We could follow a Pyomo model of specifying
    class instances here, but I worry that will complicate the interface
    between COEK and POEK.

    Maybe these types (or class instances) should be defined within a
    separate namespace?  Something like 'coek::types::Integers'?

.. note::

    COEK confirms that expressions used to index variables do not
    contain a variable unless it is fixed.  Thus, the following creates
    a runtime error:

    .. code::

        auto x = coek::variable(100);
        auto y = coek::variable();
        auto v = x(y+3).value();


Parameters
----------

Mutable parameters can be declared in a similar manner to variables:

.. code::

    // A single parameter
    auto p = coek::parameter();
    auto q = coek::parameter("q");

Note that parameter are always continuous, and their value defaults
to zero.  Initializing parameters can be similarly executed using
function chaining:

.. code::

    // A single parameter initialized to 1.0
    auto q = coek::parameter("q").value(1.0);

.. admonition:: WEH

    Note that this syntax is different from what is currently implemented in COEK:

    .. code::

        coek::Parameter p("p", 1.0);

    This is motivated by the syntax needed for indexed parameters, where the 
    second argument may specify the array length.

.. admonition:: Question

    Do we forsee a need for non-double parameters?  I could imagine
    doing the following?

    .. code::

        auto qi = coek::parameter<int>("q");

    Gravity allows for this type of typing of values.

.. note::

    These are still 'concrete' parameters.  They are assumed to have
    values that can be used immediately.  In that sense, they differ from
    the abstract approach used in Pyomo.  But the expression management
    is the same;  the parameters are included in the expression tree
    and not pulled out a constant values.


Expressions
-----------

A COEK expression is formed by performing arithmetic operations on
COEK variables, parameters and set indices, including operations with
constant values.  For example:

.. code::

    auto x = coek::variable("x");
    auto e = sin(3*x+1);
    auto v = e.value();

Note that these fundamental types are not owned by a COEK model, so such
an expression can be used and re-used within multiple expressions and
within multiple COEK models.

.. note::

    COEK does not have support for first-order named expressions right
    now.  The re-use described here is part of what a named expression
    provides.  I think more fundamentally a named expression allows users
    to interact with expressions that reflect fundamental values in their
    model, hence it is still worth considering how we would support them.

    Maybe something like the following is sufficient:

    .. code::

        auto x = coek::variable("x");
        auto e = sin(3*x+1);
        auto E = coek::expression("E").value(e);

    This would imply an annotation of the expression tree where the string
    "E" is associated with a sub-expression.

.. admonition:: Question

    If we did this, would the user need to add the named expression
    explicitly to the model to track it there?  I think so.  Thus,
    the following would also make sense:

    .. code::

        auto E = model.add( coek::expression("E") );


Objectives
----------

In COEK and POEK, objectives are not owned by a model, but they are
typically associated with a model.  Thus, the following are equivalent:

.. code::

    auto x = coek::variable("x");
    auto o = model.add_objective("o", 2*x).sense(coek::Model::maximize);
    model.add( o );

and

.. code::

    auto x = coek::variable("x");
    auto o = model.add( coek::objective("o", 2*x).sense(coek::Model::maximize) );

The ``expr()`` method is used to set and get the objective expression, and
the ``sense()`` method is used to get and set the objective sense (which
defaults to minimization).  For example:

.. code::

    auto x = coek::variable("x");
    auto o = model.add( coek::objective("o").
                            expr(2*x).
                            sense(coek::Model::minimize) );

.. admonition:: Question

   Do we want to support the ``add_objective()`` method, or simply use the ``add()`` method?

.. admonition:: WEH

   I think we may need to change the API for ``objective()`` to disallow the specification of expressions here.
   There will be contexts where we want to pass-in an an expression using parameters for arrays of objectives.

.. note::

    This API supports the declaration of multiple objectives, though COEK solvers
    do not currently support multi-objective optimization:

    .. code::

        // A single objective
        auto a = model.add( coek::objective(2*x) );
        auto b = model.add( coek::objective("b", 2*x) );


Constraints
-----------

In COEK and POEK, constraints are not owned by a model, but they are
typically associated with a model.  Thus, the following are equivalent:

.. code::

    auto x = coek::variable("x");
    auto c = model.add_constraint("c", 2*x == 0);

and

.. code::

    auto x = coek::variable("x");
    auto c = model.add( coek::constraint("c", 2*x == 0) );

Further, we can declare multiple constraints be sequentially adding constraint objects:

.. code::

    // A single constraint
    auto a = model.add( coek::constraint(2*x == 0) );
    auto b = model.add( coek::constraint("b", 2*x == 0) );

The `expr` method is used to set and get the constraint expression.
For example:

.. code::

    auto c = model.add( coek::constraint("c").
                            expr(2*x) );

.. admonition:: Question

   Do we want to support the ``add_constraint()`` method, or simply use the ``add()`` method?

There are several forms of constraint expressions supported by COEK:
inequalities, equalities and ranges.  For example:

.. code::

    auto x = coek::variable();
    auto y = coek::variable();

    // Inequalities
    auto c1 = x >= y;
    auto c2 = x > y;
    auto c3 = x <= y;
    auto c4 = x < y;
    // Equality
    auto c5 = x == y;
    // Ranged
    auto c6 = coek::inequality( 0, x + y, 1);

