Coek API Design Notes
=====================

... WEH::

    Add notes in block using your initials, like this.

Overview
--------

There are a variety of alternative API designs that would be reasonable
for specifying COEK variables, objectives and constraints.  The following
notes highlight different alternatives to help refine the final design
of COEK's API.

Consistency with POEK
~~~~~~~~~~~~~~~~~~~~~

Although it's desirable to have the COEK and POEK APIs be very similar, Python is a more flexible
language that allows for the following features:

* Named keyword arguments
* Non-typed arguments

Hence, Python can rely on introspecting and argument naming to determine the semantics of the
command-line arguments.  This is particularly flexible in a user-oriented API, so the COEK
C++ API will necessarily be more constrained.

Getter/Setter
~~~~~~~~~~~~~

Currently, COEK relies on methods get_foo and set_foo to manage data
in the API.  By contrast, POEK can rely on properties, which are more
intuitive for this sort of thing.

Alternatively, COEK could rely on methods with the following names:

* `void foo(arg value)` used for setting foo
* `value foo()` used for getting foo

These methods are less explicit, but perhaps better for function chaining (see examples below).


Variables
---------

In COEK and POEK, variables are not owned by a model.  However, it is
necessary to associate a variable with a model to facilitate efficient
processing of models.  Thus, the following are equivalent:

... code::

    auto x = coek::Variable("x");
    model.add_variable(x);

and

... code::

    auto x = model.add_variable("x");

We probably need different class types for singleton and
indexed variables.  Currently, COEK uses `coek::Variable` and
`coek::IndexedVariable`.  Thus, it's also convenient to have a function
that creates variables:

... code::

    auto x = coek::variable("x");

It's reasonable to require that the `coek::variable` function and `model.add_variable` method have the same API.

... question::

    Should these have the same name?  Would it be more intuitive for users to call `coek::variable` and `model.variable`?

    Note that just using 'add' is problematic, because a model needs to add variables, objectives and constraints.


Minimal Specification
~~~~~~~~~~~~~~~~~~~~~

A minimal variable specification includes a name and/or indexing information.  The following are basic examples:

... code::

    // A single continuous variable
    auto x = model.add_variable();
    auto y = model.add_variable("y");


    // An array of continuous variables of length 'n'
    size_t n=100;
    auto x = model.add_variable(n);
    auto y = model.add_variable("y", n);


    // A tensor of continuous random variables:  R^{2 x 3 x 5}
    std::tuple<size_t> dim = {2,3,5};
    auto x = model.add_variable(dim);
    auto y = model.add_variable("y", dim);

    // A tensor of continuous random variables indexed by COEK set objects
    auto A = coek::RangeSet(1,10);
    auto B = coek::RangeSet(11,20);
    auto x = model.add_variable(A*B);
    auto y = model.add_variable("y", A*B);


Beyond A Minimal Specification
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Variable declarations require the specification of various information:

* Lower bound values
* Upper bound values
* Initial values
* Variable type (continuous, binary, integer, etc)

Additionally, variable store information about whether they are fixed, and it may make sense to declare variables as fixed.

Currently, COEK specifies these arguments in order.  For example, an integer variable can be declared as:

... code::

    // lower=2
    // upper=10
    // initial=3
    // binary=false
    // integer=true
    auto x = model.add_variable("x", 2, 10, 3, false, true);

This is a nice, compact syntax.  However, it does not explicitly indicate
what the arguments mean.  Also, it can only specify continuous, binary
and integer variables.  The following syntax, using function chaining, is more explicit:

... code::

    auto x = model.add_variable().
                    set_name("x").
                    set_lower(2).
                    set_upper(10).
                    set_initial(3).
                    set_integer(true);

However, the use of `set_*` methods seems verbose, so the following seems preferable:

... code::

    auto x = model.add_variable("x").
                    name("x").
                    lower(2).
                    upper(10).
                    initial(3).
                    integer(true);

Additionally, a more general specification of variable feasible sets can be defined with an enumeration type:

... code::

    auto x = model.add_variable().
                    name("x").
                    lower(2).
                    upper(10).
                    initial(3).
                    within(coek::Integers);

... question::

    I think it's reasonable to limit the specification for 'within'
    to enumeration types.  We could follow a Pyomo model of specifying
    class instances here, but I worry that will complicate the interface
    between COEK and POEK.

    Maybe these types (or class instances) should be defined within a
    separate namespace?  Something like 'coek::types::Integers'?


... WEH::

    Specifying name and dimension of variables seems fundamental and
    something that would be done commonly, so I'm inclined to keep those
    arguments as part of the function:

    auto x = model.add_variable("x", A*B).
                    lower(2).
                    upper(10).
                    initial(3).
                    within(coek::Integers);

    For example, the indexing option determines the type of variable
    object returned, so I think we need to include this and not treat it
    as something that is returned later.  (Yes, we could have a unified
    variable object ... but it's API would be much less clean IMHO.)


Specifications with Set Indices
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

When specifying a tensor of variables, it is sometimes desirable to
specify variable parameters that depend on the indices of the variable.
For example:

.. math::

    x \in R^{n \cross m}\\
    0 \leq x_{ij} \leq i*j\\
    x_{ij} = i+j

We can decelare `x` using set indices that are used in expressions
defining the values of lower- and upper-bounds, and the initial values.
For example:

.. code::

    auto i = set_index("i");
    auto j = set_index("j");

    auto M = coek::RangeSet(1,m);
    auto N = coek::RangeSet(1,n);
    auto x = model.add_variable("x", Forall(i,j).In(M*N)).
                lower(0).
                upper(i*j).
                initial(i+j);

.. WEH::

    The expressions used for the lower, upper and initial values are the
    same as those used to specify COEK models.  It may be desirable to
    allow a more flexible specification, for example using C++ lambda
    functions that are passed into the lower, upper and initial methods.
    However, it's unclear whether lambda functions are strictly superior.
    And this explicit specification allows for a direct translation of
    similar POEK logic into COEK.

.. WEH::

    There's an obvious complaint to be made here about the scoping of the
    values of i and j here.  I'm not sure how to resolve that.  The call
    to Forall() associates i and j with the set M*N, and that association
    is used when generating the values for lower, upper and initial.
    But, you could also associate i and j within other calls to Forall().
    I don't see how we can dynamically create those references here and
    scope them relative to the variable declaration.

    Here's a possible syntax that would limit the scope of i and j, by making their
    values directly tied to the indexing set:

    auto M = coek::RangeSet(1,m);
    auto N = coek::RangeSet(1,n);
    auto x_index = M*N;

    auto i = x_index.index("i");
    auto j = x_index.index("j");
    auto x = model.add_variable("x", x_index).
                lower(0).
                upper(i*(j+p)).
                initial(i+j);

    This seems less intuitively clear, IMHO.

Note that this syntax can be extended to allow for the use of mutable
parameters as well:

.. code::

    auto i = set_index("i");
    auto j = set_index("j");
    auto p = parameter("p", 1.0);

    auto M = coek::RangeSet(1,m);
    auto N = coek::RangeSet(1,n);
    auto x = model.add_variable("x", Forall(i,j).In(M*N)).
                lower(0).
                upper(i*(j+p)).
                initial(i+j);

Here, the value of the upper-bound depends on `p`, which may be changed
after the variable is declared.  COEK uses the expression logic to appropriately
account for that change to the model.



