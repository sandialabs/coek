# TODO
# Taken from cute suite. Formulated in Pyomo by Logan Barnes

import poek as pk


model = pk.model()

N = list(range(1, 4))

x = model.add_variable(index=N, value=1.0, lb=1.0e-5)

model.add_objective(
    5 * x[1] + 50000.0 / x[1] + 20 * x[2] + 72000.0 / x[2] + 10 * x[3] + 144000.0 / x[3]
)

model.add_constraint(4.0 / x[1] + 32.0 / x[2] + 120.0 / x[3] <= 1)
