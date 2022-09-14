# TODO
# Formulated in Pyomo by Juan Lopez and Gabe Hackebeil
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

#   classification QLR2-AN-8-10

import poek as pk


model = pk.model()

S = list(range(1, 9))

data = pk.util.load_data("avgasa.json")
a, b, c = data.unpack("a", "b", "c")

x = model.add_variable(index=range(1, 9), lb=0.0, ub=1.0, integer=True, value=1)

model.add_objective(
    sum(a[j] * x[j] ** 2 for j in range(1, 9))
    + sum(b[j] * x[j] * x[j + 1] for j in range(1, 8))
    + sum(c[j] * x[j] for j in range(2, 9))
)

for j in range(1, 5):
    model.add_constraint(x[2 * j - 1] + x[2 * j] <= 1.0)

for j in range(0, 2):
    model.add_constraint(sum(x[2 * i - j] for i in range(1, 5)) <= 2.0)

model.add_constraint(2.0 * x[1] + x[3] - x[7] >= 0)

model.add_constraint(5.0 * x[1] + 3.0 * x[3] - 3.0 * x[5] - x[7] >= 0)

model.add_constraint(x[2] - x[4] - 3.0 * x[6] - 5.0 * x[8] >= 0)

model.add_constraint(x[2] - 3.0 * x[6] - 2.0 * x[8] >= 0)
