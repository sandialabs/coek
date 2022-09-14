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

#   Source:
#   J. Nocedal,
#   "Solving large nonlinear systems of equations arising in mechanics",
#   Proceedings of the Cocoyoc Numerical Analysis Conference, Mexico,
#   pp. 132-141, 1981.

#   SIF input: Ph. Toint, Dec 1989.

#   classification OXR2-MN-V-0

import poek as pk


model = pk.model()

p = 71
wght = -0.1
disw = wght / (p - 1)
hp2 = 0.5 * p**2

x = model.add_variable(pk.RangeSet(1, p) * pk.RangeSet(1, p), value=0.0)

model.add_objective(
    sum(
        0.5 * (x[i, j] - x[i, j - 1]) ** 2
        + 0.5 * (x[i, j] - x[i - 1, j]) ** 2
        + hp2 * (x[i, j] - x[i, j - 1]) ** 4
        + hp2 * (x[i, j] - x[i - 1, j]) ** 4
        for i in pk.seq(2, p)
        for j in pk.seq(2, p)
    )
    + sum(wght * x[p, j] for j in pk.seq(1, p))
)

for j in pk.seq(1, p):
    x[1, j].value = 0.0
    x[1, j].fixed = True
