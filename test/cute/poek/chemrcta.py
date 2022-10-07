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

#   Source: Problem 8, eqs (8.6)--(8.9) in
#   J.J. More',
#   "A collection of nonlinear model problems"
#   Proceedings of the AMS-SIAM Summer seminar on the Computational
#   Solution of Nonlinear Systems of Equations, Colorado, 1988.
#   Argonne National Laboratory MCS-P60-0289, 1989.

#   SIF input: Ph. Toint, Dec 1989.
#              minor correction by Ph. Shott, Jan 1995 and F Ruediger, Mar 1997.

#   classification NOR2-MN-V-V

import poek as pk

exp = pk.exp


model = pk.model()

n = 2500
pem = 1.0
peh = 5.0
d = 0.135
b = 0.5
beta = 2.0
gamma = 25.0
h = 1 / (n - 1)
cu1 = -h * pem
cui1 = 1 / (h**2 * pem) + 1 / h
cui = -1 / h - 2 / (h**2 * pem)
ct1 = -h * peh
cti1 = 1 / (h**2 * peh) + 1 / h
cti = -beta - 1 / h - 2 / (h**2 * peh)

t = model.add_variable(index=range(1, n + 1), value=1.0, lb=0.0000001)
u = model.add_variable(index=range(1, n + 1), value=1.0, lb=0.0)


model.add_objective(pk.expression(0))

model.add_constraint((cu1 * u[2] - u[1] + h * pem) == 0)

model.add_constraint((ct1 * t[2] - t[1] + h * peh) == 0)

for i in range(2, n):
    model.add_constraint(
        (
            -d * u[i] * exp(gamma - gamma / t[i])
            + (cui1) * u[i - 1]
            + cui * u[i]
            + u[i + 1] / (h**2 * pem)
        )
        == 0
    )

for i in range(2, n):
    model.add_constraint(
        (
            b * d * u[i] * exp(gamma - gamma / t[i])
            + (cti1) * t[i - 1]
            + cti * t[i]
            + t[i + 1] / (h**2 * peh)
        )
        == 0
    )

model.add_constraint((u[n] - u[n - 1]) == 0)

model.add_constraint((t[n] - t[n - 1]) == 0)
