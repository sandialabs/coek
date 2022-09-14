import sys
import random
import itertools
import poek as pk


def knapsack(N):
    random.seed(1000)

    N = N * 1000
    W = N / 10.0

    model = pk.model()

    w = {}
    v = {}
    for i in range(N):
        w[i] = random.uniform(0.0, 1.0)
        v[i] = random.uniform(0.0, 1.0)

    x = model.add_variable(N, lb=0.0, ub=1.0)

    model.add_objective(pk.quicksum(v[i] * x[i] for i in range(N)))
    model.add_constraint(pk.quicksum(w[i] * x[i] for i in range(N)) <= W)

    return model
