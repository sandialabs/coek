# TODO
# Taken from H&S. Formulated in pyomo by Logan Barnes.

import poek as pk


model = pk.model()

N = list(range(1, 7))
u = {}
u[1] = 0.31
u[2] = 0.046
u[3] = 0.068
u[4] = 0.042
u[5] = 0.028
u[6] = 0.0134

x = model.add_variable(index=N, lb=0, value=0)
for i in N:
    x[i].ub = u[i]

model.add_objective(4.3 * x[1] + 31.8 * x[2] + 63.3 * x[3] + 15.8 * x[4] + 68.5 * x[5] + 4.7 * x[6])

model.add_constraint(
    17.1 * x[1]
    + 38.2 * x[2]
    + 204.2 * x[3]
    + 212.3 * x[4]
    + 623.4 * x[5]
    + 1495.5 * x[6]
    - 169.0 * x[1] * x[3]
    - 3580.0 * x[3] * x[5]
    - 3810.0 * x[4] * x[5]
    - 18500.0 * x[4] * x[6]
    - 24300.0 * x[5] * x[6]
    >= 32.97
)
model.add_constraint(
    17.9 * x[1]
    + 36.8 * x[2]
    + 113.9 * x[3]
    + 169.7 * x[4]
    + 337.8 * x[5]
    + 1385.2 * x[6]
    - 139.0 * x[1] * x[3]
    - 2450.0 * x[4] * x[5]
    - 16600.0 * x[4] * x[6]
    - 17200.0 * x[5] * x[6]
    >= 25.12
)
model.add_constraint(-273.0 * x[2] - 70.0 * x[4] - 819.0 * x[5] + 26000.0 * x[4] * x[5] >= -124.08)
model.add_constraint(
    159.9 * x[1]
    - 311.0 * x[2]
    + 587.0 * x[4]
    + 391.0 * x[5]
    + 2198.0 * x[6]
    - 14000.0 * x[1] * x[6]
    >= -173.02
)
