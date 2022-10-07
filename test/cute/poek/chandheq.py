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

#   Source: problem 4 in
#   J.J. More',
#   "A collection of nonlinear model problems"
#   Proceedings of the AMS-SIAM Summer seminar on the Computational
#   Solution of Nonlinear Systems of Equations, Colorado, 1988.
#   Argonne National Laboratory MCS-P60-0289, 1989.

#   SIF input: Ph. Toint, Dec 1989.

#   classification NOR2-RN-V-V

import poek as pk


model = pk.model()

n = 100
c = 1.0

x = {i: 1 / n for i in range(1, n + 1)}
w = {i: 1 / n for i in range(1, n + 1)}

h = model.add_variable(index=range(1, n + 1), value=1.0, lb=0)

model.add_objective(pk.expression(0))

for i in range(1, n + 1):
    model.add_constraint(
        sum(-0.5 * c * w[j] * x[i] / (x[i] + x[j]) * h[i] * h[j] for j in range(1, n + 1)) + h[i]
        == 1.0
    )
