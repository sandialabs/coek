# TODO
# Formulated in Pyomo by Gabe Hackebeil
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

#   Source: problem 3 in
#   J.J. More',
#   "A collection of nonlinear model problems"
#   Proceedings of the AMS-SIAM Summer seminar on the Computational
#   Solution of Nonlinear Systems of Equations, Colorado, 1988.
#   Argonne National Laboratory MCS-P60-0289, 1989.

#   SIF input: Ph. Toint, Dec 1989.

#   classification NOR2-MN-V-V

import poek as pk

exp = pk.exp
cos = pk.cos
sin = pk.sin


model = pk.model()

p = 23
l = 5.0
h = 1.0 / (p - 1.0)
c = h**2 / l
S = [(i, j) for i in range(1, p + 1) for j in range(1, p + 1)]

u = model.add_variable(index=S, value=0.0)
x = model.add_variable(index=S, value=0.0)

model.add_objective(pk.expression(0))

for i in range(2, p):
    for j in range(2, p):
        model.add_constraint(
            (
                4 * u[i, j]
                - u[i + 1, j]
                - u[i - 1, j]
                - u[i, j + 1]
                - u[i, j - 1]
                - c * exp(u[i, j]) * cos(x[i, j])
            )
            == 0
        )

for i in range(2, p):
    for j in range(2, p):
        model.add_constraint(
            (
                4 * x[i, j]
                - x[i + 1, j]
                - x[i - 1, j]
                - x[i, j + 1]
                - x[i, j - 1]
                - c * exp(u[i, j]) * sin(x[i, j])
            )
            == 0
        )

for j in range(1, p + 1):
    u[1, j].fixed = True
    u[p, j].fixed = True
    x[1, j].fixed = True
    x[p, j].fixed = True

for i in range(2, p):
    u[i, p].fixed = True
    u[i, 1].fixed = True
    x[i, p].fixed = True
    x[i, 1].fixed = True
