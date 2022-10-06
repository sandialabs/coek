# TODO
# Formulated in Pyomo by Carl D. Laird, Daniel P. Word, and Brandon C. Barrera
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

#   classification OOR2-MY-6-200

import poek as pk

sin = pk.sin
cos = pk.cos
acos = pk.acos


model = pk.model()

np = 100
S = list(range(1, np + 1))

data = pk.util.load_data("cresc100.json")
x = data.unpack("x", index=("i"))
y = data.unpack("y", index=("i"))

v1 = model.add_variable(value=-40.0)
w1 = model.add_variable(value=5.0)
d = model.add_variable(lb=0.00000001, value=1.0)
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

for i in S:
    exp1 = (v1 + a * d * cos(t) - x[i]) ** 2 + (w1 + a * d * sin(t) - y[i]) ** 2 - (d + r) ** 2
    model.add_constraint(exp1 <= 0.0)

for i in S:
    exp2 = (v1 - x[i]) ** 2 + (w1 - y[i]) ** 2 - (a * d + r) ** 2
    model.add_constraint(exp2 >= 0.0)
