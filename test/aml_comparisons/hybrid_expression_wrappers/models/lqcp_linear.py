import sys
import pyomo_coek.full_integration as pe


def lqcp_linear(n):
    model = pe.ConcreteModel()

    model.n = n
    model.m = model.n
    model.dx = 1.0 / model.n
    model.T = 1.58
    model.dt = model.T / model.n
    model.h2 = model.dx**2
    model.a = 0.001

    model.ns = pe.RangeSet(0, model.n)
    model.ms = pe.RangeSet(0, model.m)

    model.y = pe.Var(model.ms, model.ns, bounds=(0.0, 1.0))
    model.u = pe.Var(model.ms, bounds=(-1.0, 1.0))

    def yt(j, dx):
        return 0.5 * (1 - (j * dx) * (j * dx))

    def rule(model):
        return 0.25 * model.dx * (
            (model.y[model.m, 0] - yt(0, model.dx)) ** 2
            + 2
            * sum(
                (model.y[model.m, j] - yt(j, model.dx)) ** 2 for j in range(1, model.n)
            )
            + (model.y[model.m, model.n] - yt(model.n, model.dx)) ** 2
        ) + 0.25 * model.a * model.dt * (
            2 * sum(model.u[i] ** 2 for i in range(1, model.m)) + model.u[model.m] ** 2
        )

    model.obj = pe.Objective(rule=rule)

    pde_coef = model.T * 0.5 * model.n  # == dt*0.5/h2

    def pde_rule(m, i, j):
        variables = [
            m.y[i + 1, j],
            m.y[i, j],
            m.y[i, j - 1],
            m.y[i, j + 1],
            m.y[i + 1, j - 1],
            m.y[i + 1, j + 1],
        ]
        coefficients = [
            1 + 2 * pde_coef,
            -1 + 2 * pde_coef,
            -pde_coef,
            -pde_coef,
            -pde_coef,
            -pde_coef,
        ]
        return (
            pe.LinearExpression(
                constant=0, linear_coefs=coefficients, linear_vars=variables
            )
            == 0
        )

    model.pde = pe.Constraint(
        pe.RangeSet(0, model.n - 1), pe.RangeSet(1, model.n - 1), rule=pde_rule
    )

    def ic_rule(model, j):
        return model.y[0, j] == 0

    model.ic = pe.Constraint(model.ms, rule=ic_rule)

    def bc1_rule(model, i):
        return model.y[i, 2] - 4 * model.y[i, 1] + 3 * model.y[i, 0] == 0

    model.bc1 = pe.Constraint(pe.RangeSet(1, model.n), rule=bc1_rule)

    def bc2_rule(model, i):
        return model.y[i, model.n - 2] - 4 * model.y[i, model.n - 1] + 3 * model.y[
            i, model.n - 0
        ] == (2 * model.dx) * (model.u[i] - model.y[i, model.n - 0])

    model.bc2 = pe.Constraint(pe.RangeSet(1, model.n), rule=bc2_rule)

    return model
