import sys
from gurobipy import Model, quicksum


def nqueens(N):
    model = Model("nqueens")

    x = model.addVars(N, N, lb=0.0, ub=1.0, vtype='C')

    # obj
    model.setObjective(x.sum())

    # one per row
    model.addConstrs((x.sum(i, '*') == 1 for i in range(N)))

    # one per column
    model.addConstrs((x.sum('*', j) == 1 for j in range(N)))

    # \diagonals_col
    model.addConstrs((x[0, i] + quicksum(x[j, i + j]
                                         for j in range(1, N - i)) <= 1
                      for i in range(N - 1)))
    # \diagonals_row
    model.addConstrs((x[i, 0] + quicksum(x[i + j, j]
                                         for j in range(1, N - i)) <= 1
                      for i in range(1, N - 1)))
    # /diagonals_col
    model.addConstrs((x[0, i] + quicksum(x[j, i - j]
                                         for j in range(1, i + 1)) <= 1
                      for i in range(1, N)))
    # /diagonals_row
    model.addConstrs((x[i, N - 1] + quicksum(x[i + j, N - 1 - j]
                                             for j in range(1, N - i)) <= 1
                      for i in range(1, N)))

    return model

