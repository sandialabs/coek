# TODO
# Taken from cute suite. Formulated in pyomo by Logan Barnes.

import poek as pk


model = pk.model()

N = list(range(1, 6))
M = list(range(1, 13))

data = pk.util.load_data("hs083.json")
a, l, u = data.unpack("a", "l", "u")

x_init = {}
x_init[1] = 78.0
x_init[2] = 33.0
x_init[3] = 27.0
x_init[4] = 27.0
x_init[5] = 27.0

x = model.add_variable(index=N)
for i in N:
    x[i].value = x_init[i]
    x[i].lb = l[i]
    x[i].ub = u[i]

model.add_objective(5.3578547 * x[3] ** 2 + 0.8356891 * x[1] * x[5] + 37.293239 * x[1] - 40792.141)

model.add_constraint(
    pk.inequality(0, a[1] + a[2] * x[2] * x[5] + a[3] * x[1] * x[4] - a[4] * x[3] * x[5], 92)
)

model.add_constraint(
    pk.inequality(0, a[5] + a[6] * x[2] * x[5] + a[7] * x[1] * x[2] + a[8] * x[3] ** 2 - 90, 20)
)

model.add_constraint(
    pk.inequality(
        0,
        a[9] + a[10] * x[3] * x[5] + a[11] * x[1] * x[3] + a[12] * x[3] * x[4] - 20,
        5,
    )
)
