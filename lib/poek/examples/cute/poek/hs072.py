# TODO
# Taken from cute suite. Formulated in Pyomo by Logan Barnes.

import poek as pk


model = pk.model()

N = list(range(1, 3))
M = list(range(1, 5))

a = {}
a[1, 1] = 4
a[2, 1] = 0.16
a[1, 2] = 2.25
a[2, 2] = 0.36
a[1, 3] = 1
a[2, 3] = 0.64
a[1, 4] = 0.25
a[2, 4] = 0.64
b = {}
b[1] = 0.0401
b[2] = 0.010085

x = model.add_variable(index=M, lb=0.001, value=1.0)

model.add_objective(1 + sum(x[j] for j in M))

for i in N:
    model.add_constraint(sum((a[i, j] / x[j]) for j in M) <= b[i])

for j in M:
    model.add_constraint(x[j] <= (5 - j) * 1.0e5)
