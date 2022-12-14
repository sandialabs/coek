# TODO
# Formulated in pyomo by Logan Barnes and Gabe Hackebeil.
#
# Take from CUTE test models

import poek as pk
from math import atan, sqrt


model = pk.model()

PI = 4 * atan(1)
I = list(range(1, 236))
N = list(range(1, 9))
M = list(range(6, 9))

x_init = {}
x_init[1] = 0.1
x_init[2] = 0.2
x_init[3] = 100.0
x_init[4] = 125.0
x_init[5] = 175.0
x_init[6] = 11.2
x_init[7] = 13.2
x_init[8] = 15.8

data = pk.util.load_data("hs105.json")
y = data.y

x = model.add_variable(index=N)
for i in N:
    x[i].value = x_init[i]

a = {}
for i in I:
    a[i] = (x[1] / x[6]) * pk.exp(-((y[i] - x[3]) ** 2) / (2 * x[6] ** 2))

b = {}
for i in I:
    b[i] = (x[2] / x[7]) * pk.exp(-((y[i] - x[4]) ** 2) / (2 * x[7] ** 2))

c = {}
for i in I:
    c[i] = ((1 - x[2] - x[1]) / x[8]) * pk.exp(-((y[i] - x[5]) ** 2) / (2 * x[8] ** 2))

model.add_objective(-sum(pk.log((a[i] + b[i] + c[i]) / sqrt(2 * PI)) for i in I))

model.add_constraint(1 - x[1] - x[2] >= 0)
model.add_constraint(pk.inequality(0.001, x[1], 0.499))
model.add_constraint(pk.inequality(0.001, x[2], 0.449))
model.add_constraint(pk.inequality(100, x[3], 180))
model.add_constraint(pk.inequality(130, x[4], 210))
model.add_constraint(pk.inequality(170, x[5], 240))
model.add_constraint(pk.inequality(5, x[6], 25))
model.add_constraint(pk.inequality(5, x[7], 25))
model.add_constraint(pk.inequality(5, x[8], 25))
