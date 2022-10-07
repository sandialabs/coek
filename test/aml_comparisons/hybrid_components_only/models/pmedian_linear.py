import pyomo_coek.components_only as pe


def pmedian_linear(N, P):
    ones = [1] * N

    model = pe.ConcreteModel()

    model.N = N

    model.P = P

    model.M = N

    model.Locations = pe.RangeSet(1, model.N)

    model.Customers = pe.RangeSet(1, model.M)

    model.d = pe.Param(
        model.Locations,
        model.Customers,
        initialize=lambda model, n, m: 1.0 + 1.0 / (n + m + 1),
        within=pe.Reals,
    )

    model.x = pe.Var(model.Locations, model.Customers, bounds=(0.0, 1.0), initialize=0.0)

    model.y = pe.Var(model.Locations, bounds=(0.0, 1.0), initialize=0.0)

    def rule(model):
        return pe.construct_linear_expression(
            linear_coefs=[model.d[n, m] for n in model.Locations for m in model.Customers],
            linear_vars=[model.x[n, m] for n in model.Locations for m in model.Customers],
        )

    model.obj = pe.Objective(rule=rule)

    def rule(model, m):
        return (
            pe.construct_linear_expression(
                linear_coefs=ones, linear_vars=[model.x[n, m] for n in model.Locations]
            ),
            1.0,
        )

    model.single_x = pe.Constraint(model.Customers, rule=rule)

    def rule(model, n, m):
        return (
            None,
            pe.construct_linear_expression(
                linear_coefs=[1, -1], linear_vars=[model.x[n, m], model.y[n]]
            ),
            0.0,
        )

    model.bound_y = pe.Constraint(model.Locations, model.Customers, rule=rule)

    def rule(model):
        return (
            pe.construct_linear_expression(
                linear_coefs=ones, linear_vars=[model.y[n] for n in model.Locations]
            )
            - model.P,
            0.0,
        )

    model.num_facilities = pe.Constraint(rule=rule)

    return model
