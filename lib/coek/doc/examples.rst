Examples
========

The following examples illustrate the functionality of Coek.

Model Syntax
------------

Rosenbrock
~~~~~~~~~~

.. literalinclude:: ../examples/rosenbr.cpp
    :language: C++
    :linenos:

The function ``rosenbr`` constructs a ``coek::Model`` object and returns
it.  In Lines 7 and 8, two decision variables are defined, initialized
and added to the model.  On Line 10, an expression is constructed for
the optimization objective, and this expression is added to the model.

Multidimentional Rosenbrock
~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. literalinclude:: ../examples/srosenbr_vector.cpp
    :language: C++
    :linenos:

The function ``rosenbr_vector`` constructs a ``coek::Model`` object
and returns it.  In Lines 7-14, a ``std::vector`` of Coek variables
is declared, initialized, and added to the model.  In Lines 16-19,
an expression is constructed for the optimization objective, and this
expression is added to the model.

.. literalinclude:: ../examples/srosenbr_array.cpp
    :language: C++
    :linenos:

The function ``rosenbr_array`` constructs a ``coek::Model`` object
using the more compact array syntax supported by Coek.  Line 8 declares
an array of variables and adds it to the model.  Lines 9-14 initialize
the variables, using the ``coek::range`` function to simplify the loop.
Similarly, Lines 16-19, construct an expression for the objective using
the ``coek::range`` function, which is then added to the model.


Applying Optimizers
-------------------

Solving a Linear Program
~~~~~~~~~~~~~~~~~~~~~~~~

.. literalinclude:: ../examples/simplelp1_solve.cpp

The function ``simplelp1_solve`` constructs a simple linear program that is 
solved using the ipopt nonlinear optimization solver.


Solving a Quadratic Problem
~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. literalinclude:: ../examples/invquad_array_solve.cpp

The function ``invquad_array_solve`` constructs an inverse quadratic
problem that is optimized using the ipopt nonlinear optimization solver.  The optimal solution
is in the opposite corner of the feasible space from the constant values passed into this function.

.. literalinclude:: ../examples/invquad_array_resolve.cpp

The function ``invquad_array_resolve`` constructs an inverse quadratic
problem that is successively re-optimized using the ipopt nonlinear optimization solver.  
The initial optimal solution is [-10, -10, -10, -10, -10], which is maximally far away from the initial 
parameter values.  Resolving after changing the parameter values does not move the solution, since the resolve uses an initial value matching the last solution.
