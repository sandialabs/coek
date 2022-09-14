# TODO
# Taken from cute suite. Formulated in Pyomo by Logan Barnes.

import poek as pk

exp = pk.exp


model = pk.model()

N = list(range(1, 4))
x = model.add_variable(index=N)
x[1].value = 0
x[2].value = 1.05
x[3].value = 2.9

model.add_objective(0.2 * x[3] - 0.8 * x[1])

model.add_constraint(x[2] - exp(x[1]) >= 0)
model.add_constraint(x[3] - exp(x[2]) >= 0)
model.add_constraint(pk.inequality(0, x[1], 100))
model.add_constraint(pk.inequality(0, x[2], 100))
model.add_constraint(pk.inequality(0, x[3], 10))
