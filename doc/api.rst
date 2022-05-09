Coek API Design Notes
=====================

Overview
--------

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

The `Model::add()` method creates a variable object to the model, and this
object is returned.  This supports a concise declaration of variables,
and it enables the user to directly reference the variable object.
Further, this enables functional chaining to configure the variable.

.. question::

    Should we include an `Model::add_variable()` method that replicates
    the API of the `coek::variable()` method?  This would be redundant
    in the API, but it would allow for shorter end-user code.


Minimal Specification
~~~~~~~~~~~~~~~~~~~~~

A minimal variable specification includes a name and/or indexing
information.  The following are basic examples:

.. code::

    // A single continuous variable
    auto y = model.add( coek::variable("y") );

    // An array of continuous variables of length 'n'
    size_t n=100;
    auto y = model.add( coek::variable("y", n) );

    // A multi-dimensional array of continuous variables:  R^{2 x 3 x 5}
    auto y = model.add( coek::variable("y", {2,3,4}) );

    // A tensor of continuous variables indexed by COEK set objects
    auto A = coek::RangeSet(1,10);
    auto B = coek::RangeSet(11,20);
    auto y = model.add( coek::variable("y", A*B) );


Beyond A Minimal Specification
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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
                    initial(3).
                    within(coek::Integers);

.. WEH::

    This function chaining requires methods where the set- and
    get-semantics are dependent on the method used.  For example:

        x.value(10);

    sets the value of x, while

        auto val = x.value();

    returns the value of x.  The use of `set_*` methods seems desirable,
    but that leads to a verbose syntax that clutters the specification
    of variable properties.

.. question::

    I think it's reasonable to limit the specification for 'within'
    to enumeration types.  We could follow a Pyomo model of specifying
    class instances here, but I worry that will complicate the interface
    between COEK and POEK.

    Maybe these types (or class instances) should be defined within a
    separate namespace?  Something like 'coek::types::Integers'?

.. WEH::

    Specifying name and dimension of variables seems fundamental and
    something that would be done commonly, so I'm inclined to keep those
    arguments as part of the function:

    auto x = model.add( coek::variable("x", A*B) ).
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
    auto x = model.add( coek::variable("x") ).
                lower(0).
                upper(i*j).
                initial(i+j).
                index(Forall(i,j).In(M*N));

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

    Here's a possible syntax that would limit the scope of i and j,
    by making their values directly tied to the indexing set:

    auto M = coek::RangeSet(1,m);
    auto N = coek::RangeSet(1,n);
    auto x_index = M*N;

    auto i = x_index.index("i");
    auto j = x_index.index("j");
    auto x = model.add( coek::variable("x") ).
                lower(0).
                upper(i*(j+p)).
                initial(i+j).
                index(x_index);

    This seems less intuitively clear, IMHO.

Note that this syntax can be extended to allow for the use of mutable
parameters as well:

.. code::

    auto i = set_index("i");
    auto j = set_index("j");
    auto p = parameter("p", 1.0);

    auto M = coek::RangeSet(1,m);
    auto N = coek::RangeSet(1,n);
    auto x = model.add( coek::variable("x") ).
                lower(0).
                upper(i*(j+p)).
                initial(i+j).
                index(Forall(i,j).In(M*N));

Here, the value of the upper-bound depends on `p`, which may be changed
after the variable is declared.  COEK uses the expression logic to
appropriately account for that change to the model.

Finally, note that in these examples the order of indices in the index
set is implicitly defined by the nesting of the calls to `Forall()`.
However, it may be necessary to explicitly denote the order of indices.
For example:

.. code::

    auto i = set_index("i");
    auto j = set_index("j");

    auto M = coek::RangeSet(1,m);
    std::vector<coek::ConcreteSet> N(m);
    auto x = model.add( coek::variable("x") ).
                lower(0).
                upper(i*j).
                initial(i+j).
                index( (j,i), Forall(i).In(M).Forall(j).In(N[i]) );


Indexing Variables
~~~~~~~~~~~~~~~~~~

Variables declared over sets can be indexed using the `()` operator in a natural manner.  For example:

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

Note that variables can be indexed by expressions, but the evaluation
of those expressions is deferred.  For example:

.. code::

    auto p = parameter().value(1.0);
    auto i = set_index();

    // An array of continuous variables of length 'n'
    size_t n=100;
    auto x = model.add( coek::variable(n) );

    // Create a reference to the variable
    auto x0 = x(3+p);
    // Evaluate the reference to the variable, resolving the parameter value
    auto v = x0.value();

    // Create a reference to the variable
    auto x0 = x(3+i);
    // Evaluate the reference to the variable, resolving the parameter value
    auto v = x0.value();

.. note::

    COEK confirms that expressions used to index variables do not
    contain a variable unless it is fixed.  Thus, the following creates
    a runtime error:

    auto x = coek::variable(100);
    auto y = coek::variable();
    auto v = x(y+3).value();

    Similarly, if a set index used in an indexing expression is not being
    processed by a context, then COEK will create an error at runtime.


Parameters
----------

Mutable parameters can be declared in a similar manner to variables:

.. code::

    // A single parameter
    auto p = coek::parameter();
    auto q = coek::parameter("q");

    // An array of parameter of length 'n'
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

    // A tensor of parameters indexed by COEK set objects, initialized to i+j
    auto i = set_index("i");
    auto j = set_index("j");
    auto Q = coek::parameter("Q", Forall(i,j).In(A*B)).value(i+j);

.. WEH::

    Note that this syntax is different from what is currently implemented in COEK:

        coek::Parameter p("p", 1.0);

    The use of a numeric value to specify the array length precludes
    the use of the second argument to specify the parameter value.
    Hence, support for arrays of parameters seems to preclude the simple
    specification of parameter values.

.. question::

    Do we forsee a need for non-double parameters?  I could imagine
    doing the following?

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

    auto x = coek::variable("x");
    auto e = sin(3*x+1);
    auto E = coek::expression("E").value(e);

    This would imply an annotation of the expression tree where the string
    "E" is associated with a sub-expression.

    Support for named expressions would naturally involve support for
    arrays of named expressions.  Hence, 

        auto E = coek::expression("E", 10);

    would refer to an array of named expressions.

.. question::


    If we did this, would the user need to add the named expression
    explicitly to the model to track it there?  I think so.  Thus,
    the following would also make sense:

    auto E = model.add( coek::expression("E") );


Objectives
----------

In COEK and POEK, objectives are not owned by a model, but they are
typically associated with a model.  Thus, the following are equivalent:

.. code::

    auto x = coek::variable("x");
    auto o = coek::objective("o", 2*x).sense(coek::Model::maximize);

and

.. code::

    auto x = coek::variable("x");
    auto o = model.add( coek::objective("o", 2*x).sense(coek::Model::maximize) );

The `expr` method is used to set and get the objective expression, and
the `sense` method is used to get and set the objective sense (which
defaults to minimization).  For example:

.. code::

    auto x = coek::variable("x");
    auto o = model.add( coek::objective("o").
                            expr(2*x).
                            sense(coek::Model::minimize) );

.. WEH::

    Although not often used, we could also support various ways to declare
    groups of objectives:

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

    Finally, objectives can be declared using set indices:

    auto x = model.add( coek::variable("x", M*N) );
    auto o = model.add( coek::objective("o", Forall(i,j).In(M*N)).
                            expr( i*j*x(i,j)) );
    auto O = model.add( coek::objective("O", Forall(i).In(M)).
                            expr( i*Sum(x(i,j), Forall(j).In(M))) );


Constraints
-----------

In COEK and POEK, constraints are not owned by a model, but they are
typically associated with a model.  Thus, the following are equivalent:

.. code::

    auto x = coek::variable("x");
    auto c = coek::constraint("c", 2*x == 0);

and

.. code::

    auto x = coek::variable("x");
    auto o = model.add( coek::constraint("o", 2*x == 0) );

Further, we can declare groups of constraints:

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

The `expr` method is used to set and get the constraint expression.
For example:

.. code::

    auto c = model.add( coek::constraint("c").
                            expr(2*x) );

Finally, constraints can be declared using set indices:

.. code::

    auto x = model.add( coek::variable("x", M*N) );
    auto c = model.add( coek::constraint("c", Forall(i,j).In(M*N)).
                    expr( i*j*x(i,j) == 0 ) );
    auto C = model.add_constraint("C", Forall(i).In(M)).
                    expr( i*Sum(x(i,j), Forall(j).In(M)) == 0 )

Constraint Expressions
~~~~~~~~~~~~~~~~~~~~~~

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


Optimization Models
-------------------

The `Model` Class
~~~~~~~~~~~~~~~~~

The `Model` class provides is used to specify an optimization model.  This class includes the following:

1. Methods for adding and getting variables, objectives and constraints.  This includes methods that summarize the number
of these modeling components, as well as specialized methods to get just the names of these components.

2. Methods to set and get suffix data, which is used to annotate models.  The different components in a model can be annotated,
as well as the model itself.

3. Methods for writing models in standard forms, and for printing model data to output streams.

The `NLPModel` Class
~~~~~~~~~~~~~~~~~~~~

The `NLPModel` class is used to wrap a `Model` in a manner that
is suitable for solving continuous nonlinear programming problems.
The `NLPModel` class provides methods that encapsulate the evaluation
of objectives, gradients, constraints, Jacobians and the Hessian of
the Lagrangian.  These are key kernels required by nonlinear programming
solvers, and this class facilitates optimization with these methods.

The `CompactModel` Class
~~~~~~~~~~~~~~~~~~~~~~~~

The `CompactModel` class is used to generate an instance of `Model` using a compact specification of 
objectives and constraints.  The 




