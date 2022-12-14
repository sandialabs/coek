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

#   Source: Problem 11 in
#   A.R. Buckley,
#   "Test functions for unconstrained minimization",
#   TR 1989CS-3, Mathematics, statistics and computing centre,
#   Dalhousie University, Halifax (CDN), 1989.

#   SIF input: Ph. Toint, Dec 1989.

#   classification SXR2-AN-3-0

import poek as pk
from math import exp


model = pk.model()

M = 10
S = [1, 2, 3]

x = model.add_variable(index=S)
x[1].value = 0.0
x[2].value = 10.0
x[3].value = 1.0

t = {}
for i in range(1, M + 1):
    t[i] = 0.1 * i

model.add_objective(
    sum(
        (pk.exp(-t[i] * x[1]) - pk.exp(-t[i] * x[2]) - x[3] * exp(-t[i]) + x[3] * exp(-i)) ** 2
        for i in range(1, M + 1)
    )
)

model.add_constraint(x[3] == 1.0)
