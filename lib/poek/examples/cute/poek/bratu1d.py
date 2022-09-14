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

#   Source: Problem 121 (p. 99) in
#   A.R. Buckley,
#   "Test functions for unconstrained minimization",
#   TR 1989CS-3, Mathematics, statistics and computing centre,
#   Dalhousie University, Halifax (CDN), 1989.

#   SIF input: Ph. Toint, Dec 1989.

#   classification OXR2-MN-V-0

import poek as pk

exp = pk.exp


model = pk.model()

N = 1001
L = -3.4
h = 1.0 / (N + 1.0)
S = list(range(0, N + 2))

x = model.add_variable(index=S)
for i in S:
    x[i].value = 0 if ((i == 0) or (i == N + 1)) else -0.1 * h * (i**2)

model.add_objective(
    2 * L * h * (exp(x[1]) - exp(x[0])) / (x[1] - x[0])
    + sum(2.0 * x[i] ** 2 / h for i in range(1, N + 1))
    - sum(2.0 * x[i] * x[i - 1] / h for i in range(1, N + 1))
    + sum(
        2.0 * L * h * (exp(x[i + 1]) - exp(x[i])) / (x[i + 1] - x[i])
        for i in range(1, N + 1)
    )
)

x[0].value = 0.0
x[0].fixed = True
x[N + 1].value = 0.0
x[N + 1].fixed = True
