# TODO
# Taken from cute suite. Formulated in pyomo by Logan Barnes.

import poek as pk

model = pk.model()

N = list(range(1, 6))

x = model.add_variable(index=N)
x[1].value = -2
x[2].value = 1.5
x[3].value = 2
x[4].value = -1
x[5].value = -1

expr = 1.0
for i in N:
    expr *= x[i]
model.add_objective(expr)

model.add_constraint(sum(x[j] ** 2 for j in N) == 10)
model.add_constraint(x[2] * x[3] - 5 * x[4] * x[5] == 0)
model.add_constraint(x[1] ** 3 + x[2] ** 3 == -1)
