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

#   Source:  problem 8 in
#   A.R.Conn,N.I.M.Gould and Ph.L.Toint,
#   "Testing a class of methods for solving minimization
#   problems with simple bounds on their variables,
#   Mathematics of Computation 50, pp 399-430, 1988.

#   SIF input: Nick Gould and Ph. Toint, Dec 1995.

#   classification SUR2-AN-V-0

import poek as pk


model = pk.model()

ns = 499
n = 2 * ns + 2

x0 = {1: -3, 2: -1, 3: -3, 4: -1}

x = model.add_variable(index=range(1, n + 1))
for i in x:
    x[i].value = x0.get(i, -2)

model.add_objective(
    1.0
    + sum(
        100 * (x[2 * i] - x[2 * i - 1] ** 2) ** 2
        + (1.0 - x[2 * i - 1]) ** 2
        + 90 * (x[2 * i + 2] - x[2 * i + 1] ** 2) ** 2
        + (1.0 - x[2 * i + 1]) ** 2
        + 10 * (x[2 * i] + x[2 * i + 2] - 2.0) ** 2
        + (x[2 * i] - x[2 * i + 2]) ** 2 / 10
        for i in range(1, ns + 1)
    )
)
