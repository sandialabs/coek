Examples
========

The following examples illustrate the functionality of Coek.

.. warning::
    These examples are not automatically tested with Coek, but they reflect
    use cases that are known to work.

A Simple Linear Program
-----------------------

.. code-block::

    import poek as pk
    
    #
    # Create the model
    #
    m = pk.model()

    x = m.add_variable(name="x", lb=0)
    y = m.add_variable(name="y", lb=0)

    m.add_objective( 50*x + 40*y, False )
    m.add_constraint( 2*x + 3*y <= 1500 )
    m.add_constraint( 2*x + y <= 1000 )

    #
    # Optimize the model
    #
    opt = pk.solver('gurobi')
    if opt.available:
        print("SOLVING")
        opt.solve(m)
    else:
        print("NOT SOLVING")

    print("Value of x: {}".format(x.value))
    print("Value of y: {}".format(y.value))

A Simple Quadratic Program
--------------------------

.. code-block::

    import sys
    import poek as pk

    #
    # Mutable parameters define the maximum of an inverted quadratic
    #
    p = []
    for i in range(5):
        p.append( pk.parameter(0.5) )


    #
    # Create the model
    #
    m = pk.model()

    x = []
    for i in range(5):
        x.append( m.add_variable(lb=-10, ub=10, value=0) )

    e = pk.expression(0)
    for i in range(5):
        e += (x[i]-p[i])*(x[i]-p[i])
    m.add_objective( -e )

    nlp = pk.nlp_model(m, "cppad")
    opt = pk.solver("ipopt")

    #
    # Optimize the model
    #
    if opt.available:
        print("SOLVING")
        opt.set_option("print_level", 0)
        opt.solve(nlp)
    else:
        print("NOT SOLVING")

    # x^*_i = -10
    for i in range(5):
        print("Value of x[{}]: {}".format(i, x[i].value))

