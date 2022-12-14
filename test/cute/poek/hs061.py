# TODO
# Taken from cute suite. Formulated in Pyomo by Logan Barnes.

import poek as pk


model = pk.model()

N = list(range(1, 4))

x = model.add_variable(index=N, value=0.0)

model.add_objective(
    4 * x[1] ** 2 + 2 * x[2] ** 2 + 2 * x[3] ** 2 - 33 * x[1] + 16 * x[2] - 24 * x[3]
)

model.add_constraint(3 * x[1] - 2 * x[2] ** 2 == 7)
model.add_constraint(4 * x[1] - x[3] ** 2 == 11)
