# TODO
# Taken from cute suite. Formulated in pyomo by Logan Barnes.

import poek as pk

sin = pk.sin
from math import asin, sqrt


model = pk.model()

N = list(range(1, 8))

x = model.add_variable(index=N, lb=0)

x[1].value = 1
x[2].value = 1
x[3].value = 1
x[4].value = asin(sqrt(1 / 4.2))
x[5].value = asin(sqrt(1 / 4.2))
x[6].value = asin(sqrt(1 / 4.2))
x[7].value = asin(sqrt(5 / 7.2))

model.add_objective(-x[1] * x[2] * x[3])

model.add_constraint(x[1] - 4.2 * sin(x[4]) ** 2 == 0)
model.add_constraint(x[2] - 4.2 * sin(x[5]) ** 2 == 0)
model.add_constraint(x[3] - 4.2 * sin(x[6]) ** 2 == 0)
model.add_constraint(x[1] + 2 * x[2] + 2 * x[3] - 7.2 * sin(x[7]) ** 2 == 0)
