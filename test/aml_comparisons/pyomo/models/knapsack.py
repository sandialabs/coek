import sys
import random
import pyomo.environ as pe


def knapsack(N):
    random.seed(1000)

    N = N * 1000
    W = N / 10.0

    model = pe.ConcreteModel()

    model.INDEX = pe.RangeSet(1, N)

    model.w = pe.Param(
        model.INDEX,
        initialize=lambda model, i: random.uniform(0.0, 1.0),
        within=pe.Reals,
    )

    model.v = pe.Param(
        model.INDEX,
        initialize=lambda model, i: random.uniform(0.0, 1.0),
        within=pe.Reals,
    )

    model.x = pe.Var(model.INDEX, bounds=(0.0, 1.0))

    model.o = pe.Objective(expr=sum(model.v[i] * model.x[i] for i in model.INDEX))

    model.c = pe.Constraint(expr=sum(model.w[i] * model.x[i] for i in model.INDEX) <= W)

    return model
