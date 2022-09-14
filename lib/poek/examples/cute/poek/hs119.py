# TODO
# Formulated in pyomo by Logan Barnes and Gabe Hackebeil.
#
# Taken from CUTE models
#

import poek as pk


model = pk.model()

N = list(range(1, 17))
M = list(range(1, 9))

data = pk.util.load_data("hs119.json")
a, b, c = data.unpack("a", "b", "c", default=0)

x = model.add_variable(index=N, lb=0, ub=5, value=10.0)


model.add_objective(
    sum(a[i, j] * (x[i] ** 2 + x[i] + 1) * (x[j] ** 2 + x[j] + 1) for i in N for j in N)
)

for i in M:
    model.add_constraint(sum(b[i, j] * x[j] for j in N) == c[i])
