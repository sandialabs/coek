# TODO
# Taken from H&S. Formulated in pyomo by Logan Barnes and Gabe Hackebeil.

import poek as pk


model = pk.model()

N = list(range(1, 8))

x = model.add_variable(index=N)
x[1].value = 1.0
x[2].value = 2.0
x[3].value = 0.0
x[4].value = 4.0
x[5].value = 0.0
x[6].value = 1.0
x[7].value = 1.0

model.add_objective(
    (x[1] - 10) ** 2
    + 5.0 * (x[2] - 12) ** 2
    + x[3] ** 4
    + 3.0 * (x[4] - 11) ** 2
    + 10.0 * x[5] ** 6
    + 7.0 * x[6] ** 2
    + x[7] ** 4
    - 4.0 * x[6] * x[7]
    - 10.0 * x[6]
    - 8.0 * x[7]
)

model.add_constraint(2 * x[1] ** 2 + 3 * x[2] ** 4 + x[3] + 4 * x[4] ** 2 + 5 * x[5] <= 127.0)
model.add_constraint(7 * x[1] + 3 * x[2] + 10 * x[3] ** 2 + x[4] - x[5] <= 282.0)
model.add_constraint(23 * x[1] + x[2] ** 2 + 6 * x[6] ** 2 - 8 * x[7] <= 196.0)
model.add_constraint(
    -4 * x[1] ** 2 - x[2] ** 2 + 3 * x[1] * x[2] - 2 * x[3] ** 2 - 5 * x[6] + 11 * x[7] >= 0
)
