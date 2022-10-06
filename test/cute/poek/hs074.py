# TODO
# Taken from cute suite. Formulated in Pyomo by Logan Barnes.

import poek as pk

sin = pk.sin

model = pk.model()

a = 0.55
N = list(range(1, 5))

x = model.add_variable(index=N, value=0.0)
x[1].ub = 1200
x[1].lb = 0
x[2].ub = 1200
x[2].lb = 0
x[3].ub = a
x[3].lb = -a
x[4].ub = a
x[4].lb = -a

model.add_objective(3.0 * x[1] + 1.0e-6 * x[1] ** 3 + 2 * x[2] + 2.0e-6 * x[2] ** 3 / 3)

model.add_constraint(pk.inequality(-a, x[4] - x[3], a))
model.add_constraint(x[1] == 1000 * sin(-x[3] - 0.25) + 1000 * sin(-x[4] - 0.25) + 894.8)
model.add_constraint(x[2] == 1000 * sin(x[3] - 0.25) + 1000 * sin(x[3] - x[4] - 0.25) + 894.8)
model.add_constraint(0.0 == 1000 * sin(x[4] - 0.25) + 1000 * sin(x[4] - x[3] - 0.25) + 1294.8)
