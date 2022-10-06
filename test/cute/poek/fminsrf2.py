# TODO
# Formulated in Pyomo by Gabe Hackebeil, Juan Lopez
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

#   Source: setting the boundary free in
#   A Griewank and Ph. Toint,
#   "Partitioned variable metric updates for large structured
#   optimization problems",
#   Numerische Mathematik 39:429-448, 1982.

#   SIF input: Ph. Toint, November 1991.

#   classification OUR2-MY-V-0

import poek as pk


model = pk.model()

p = 32

h00 = 1.0
slopej = 4.0
slopei = 8.0

scale = (p - 1) ** 2

ston = slopei / (p - 1)
wtoe = slopej / (p - 1)
h01 = h00 + slopej
h10 = h00 + slopei
mid = p / 2

S = [(i, j) for i in range(1, p + 1) for j in range(1, p + 1)]
x = model.add_variable(index=S)

model.add_objective(
    sum(
        (
            (
                0.5
                * (p - 1) ** 2
                * ((x[i, j] - x[i + 1, j + 1]) ** 2 + (x[i + 1, j] - x[i, j + 1]) ** 2)
                + 1.0
            )
        )
        ** 0.5
        for i in range(1, p)
        for j in range(1, p)
    )
    / scale
    + (x[mid, mid]) ** 2 / p**2
)

for j in range(1, p + 1):
    x[1, j] = (j - 1) * wtoe + h00

for j in range(1, p + 1):
    x[p, j] = (j - 1) * wtoe + h10

for i in range(2, p):
    x[i, p] = (i - 1) * ston + h00

for i in range(2, p):
    x[i, 1] = (i - 1) * ston + h01

for i in range(2, p):
    for j in range(2, p):
        x[i, j] = 0.0
