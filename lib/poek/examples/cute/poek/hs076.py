# TODO
# Taken from cute suite. Formulated in Pyomo by Logan Barnes.

import poek as pk


model = pk.model()

N = list(range(1, 5))

x = model.add_variable(index=N, lb=0, value=0.5)

model.add_objective(
    x[1] ** 2
    + 0.5 * x[2] ** 2
    + x[3] ** 2
    + 0.5 * x[4] ** 2
    - x[1] * x[3]
    + x[3] * x[4]
    - x[1]
    - 3 * x[2]
    + x[3]
    - x[4]
)

model.add_constraint(x[1] + 2.0 * x[2] + x[3] + x[4] <= 5.0)
model.add_constraint(3.0 * x[1] + x[2] + 2.0 * x[3] - x[4] <= 4.0)
model.add_constraint(x[2] + 4 * x[3] >= 1.5)
