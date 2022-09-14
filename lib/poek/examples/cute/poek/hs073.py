# TODO
# Taken from cute suite. Formulated in Pyomo by Logan Barnes.

import poek as pk

sqrt = pk.sqrt


model = pk.model()

N = list(range(1, 5))

x = model.add_variable(index=N, lb=0, value=1.0)

model.add_objective(24.55 * x[1] + 26.75 * x[2] + 39 * x[3] + 40.50 * x[4])

model.add_constraint(2.3 * x[1] + 5.6 * x[2] + 11.1 * x[3] + 1.3 * x[4] >= 5)
model.add_constraint(
    -(
        21.0
        + 1.645
        * sqrt(
            0.28 * x[1] ** 2 + 0.19 * x[2] ** 2 + 20.5 * x[3] ** 2 + 0.62 * x[4] ** 2
        )
    )
    + (12.0 * x[1] + 11.9 * x[2] + 41.8 * x[3] + 52.1 * x[4])
    >= 0.0
)
model.add_constraint(sum(x[j] for j in N) == 1)
