Examples
========

The following examples illustrate the functionality of Coek.

.. warning::
    These examples are not automatically tested with Coek, but they reflect
    use cases that are known to work.

A Simple Linear Program
-----------------------

.. code-block:: c

    //
    // Create the model
    //
    coek::Model m;

    auto x = m.add_variable("x", 0, m.inf);
    auto y = m.add_variable("y", 0, m.inf);

    m.add_objective( 50*x + 40*y, coek::Model::maximize );
    m.add_constraint( 2*x + 3*y <= 1500 );
    m.add_constraint( 2*x + y <= 1000 );

    //
    // Optimize the model
    //
    coek::Solver solver('gurobi');
    auto status = solver.solve(m);
    
    std::cout << "Value of x: " << x.get_value() << std::endl;
    std::cout << "Value of y: " << y.get_value() << std::endl;


A Simple Quadratic Problem
--------------------------

.. code-block:: c

    //
    // Mutable parameters define the maximum of an inverted quadratic
    //
    std::vector<coek::Parameter> p(5);
    for (auto it=p.begin(); it != p.end(); ++it)
        it->set_value(0.5);

    //
    // Create the model
    //
    coek::Model m;

    std::vector<coek::Variable> x(p.size());
    for (auto it=x.begin(); it != x.end(); ++it) {
        *it = coek::Variable(-10, 10, 0);
        m.add_variable(*it);
        }

    coek::Expression e;
    for (size_t i=0; i<x.size(); i++)
        e += (x[i]-p[i])*(x[i]-p[i]);
    m.add_objective( -e );

    coek::NLPModel nlp(m, "cppad");
    coek::NLPSolver solver("ipopt");

    //
    // Optimize the model
    //
    solver.set_option("print_level", 0);
    solver.solve(nlp);

    // x^*_i = -10
    for (size_t i=0; i<x.size(); i++)
        std::cout << "Value of x[" << i << "]: " << x[i].get_value() << std::endl;

    // Set p_i = -0.5 and resolve
    for (auto it=p.begin(); it != p.end(); ++it)
        it->set_value(-0.5);
    solver.resolve();

    // x^*_i = -10
    for (size_t i=0; i<x.size(); i++)
        std::cout << "Value of x[" << i << "]: " << x[i].get_value() << std::endl;

    // Set x_i = 0 and resolve
    for (size_t i=0; i<nlp.num_variables(); i++)
        nlp.get_variable(i).set_value(0);
    solver.resolve();

    // x^*_i = 10
    for (size_t i=0; i<x.size(); i++)
        std::cout << "Value of x[" << i << "]: " << x[i].get_value() << std::endl;

