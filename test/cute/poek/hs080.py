# TODO
# Taken from cute suite. Formulated in pyomo by Logan Barnes.

import poek as pk

exp = pk.exp


model = pk.model()

N = list(range(1, 6))
a = 2.3
b = 3.2

x = model.add_variable(index=N)
x[1].value = -2.0
x[2].value = 2.0
x[3].value = 2.0
x[4].value = -1.0
x[5].value = -1.0
x[1].ub = a
x[1].lb = -a
x[2].ub = a
x[2].lb = -a
x[3].ub = b
x[3].lb = -b
x[4].ub = b
x[4].lb = -b
x[5].ub = b
x[5].lb = -b

expr = 1.0
for j in N:
    expr *= x[j]
model.add_objective(exp(expr))

model.add_constraint(sum(x[j] ** 2 for j in N) == 10)
model.add_constraint(x[2] * x[3] - 5 * x[4] * x[5] == 0)
model.add_constraint(x[1] ** 3 + x[2] ** 3 == -1)
