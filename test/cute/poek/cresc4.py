# TODO
# Formulated in Pyomo by Juan Lopez
# Taken from:

# AMPL Model by Hande Y. Benson
#
# Copyright (C) 2001 Princeton University
# All Rights Reserved
#
# Permission to use, copy, modify, and distribute this software and
# its documentation for any purpose and without fee is hereby
# granted, provided that the above copyright notice appear in all
# copies and that the copyright notice and this
# permission notice appear in all supporting documentation.

#   classification OOR2-MY-6-8

import poek as pk

acos = pk.acos
cos = pk.cos
sin = pk.sin


model = pk.model()

np = 4
x = {1: 1.0, 2: 0.0, 3: 0.0, 4: 0.5}
y = {1: 0.0, 2: 1.0, 3: -1.0, 4: 0.0}

v1 = model.add_variable(value=-40.0)
w1 = model.add_variable(value=5.0)
d = model.add_variable(lb=1e-8, value=1.0)
a = model.add_variable(lb=1.0, value=2.0)
t = model.add_variable(lb=0.0, ub=6.2831852, value=1.5)
r = model.add_variable(lb=0.39, value=0.75)

model.add_objective(
    (d + r) ** 2 * acos(-((a * d) ** 2 - (a * d + r) ** 2 + (d + r) ** 2) / (2 * (d + r) * a * d))
    - (a * d + r) ** 2
    * acos(((a * d) ** 2 + (a * d + r) ** 2 - (d + r) ** 2) / (2 * (a * d + r) * a * d))
    + (d + r)
    * a
    * d
    * sin(acos(-((a * d) ** 2 - (a * d + r) ** 2 + (d + r) ** 2) / (2 * (d + r) * a * d)))
)

for i in range(1, np + 1):
    model.add_constraint(
        (v1 + a * d * cos(t) - x[i]) ** 2 + (w1 + a * d * sin(t) - y[i]) ** 2 - (d + r) ** 2 <= 0.0
    )

for i in range(1, np + 1):
    model.add_constraint((v1 - x[i]) ** 2 + (w1 - y[i]) ** 2 - (a * d + r) ** 2 >= 0.0)
