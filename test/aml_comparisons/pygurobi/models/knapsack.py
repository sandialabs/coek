import random
import sys

from gurobipy import Model


def knapsack(Narg):
    random.seed(1000)

    N = Narg * 1000
    W = N / 10.0

    model = Model("knapsack")

    # Weights
    w = {i: random.uniform(0.0, 1.0) for i in range(N)}
    # Costs
    v = {i: random.uniform(0.0, 1.0) for i in range(N)}

    # Add vars with objective coefficients
    x = model.addVars(v.keys(), lb=0.0, ub=1.0, vtype="C", obj=v.values())
    # Capacity
    model.addConstr(x.prod(w) <= W)

    return model
