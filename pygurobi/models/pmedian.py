import sys
import random
from gurobipy import Model


def pmedian(N, P):
    random.seed(1000)


    #N Locations
    M = N  # Customers
    #P Facilities

    d = {(n, m): 1.0+1.0/(n+m+1) for n in range(N) for m in range(M)}


    model = Model("pmedian")

    x = model.addVars(d.keys(), lb=0.0, ub=1.0, vtype='C')

    y = model.addVars(N, lb=0.0, ub=1.0, vtype='C')

    # obj
    model.setObjective(x.prod(d))

    # single_x
    model.addConstrs((x.sum('*', m) == 1 for m in range(M)))

    # bound_y
    model.addConstrs((x[n, m] - y[n] <= 0 for n in range(N) for m in range(M)))

    # num_facilities
    model.addConstr(y.sum('*') == P)

    return model

