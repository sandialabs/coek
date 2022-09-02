Optimization Models
===================

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




