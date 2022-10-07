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

#   Source: problem 6 in
#   P.T. Boggs and J.W. Tolle,
#   "A strategy for global convergence in a sequential
#    quadratic programming algorithm",
#   SINUM 26(3), pp. 600-623, 1989.

#   The problem is not convex.

#   SIF input: Ph. Toint, June 1993.

#   classification OQR2-AN-5-3

import poek as pk


model = pk.model()

S = [1, 2, 3, 4, 5]

x = model.add_variable(index=S, value=1)
x[1].value = -2


model.add_objective(100 * (x[2] - x[1] ** 2) ** 2 + (x[1] - 1.0) ** 2)

model.add_constraint(x[1] * x[2] - x[3] ** 2 == 1.0)

model.add_constraint(x[2] ** 2 - x[4] ** 2 + x[1] == 0.0)

model.add_constraint(x[5] ** 2 + x[1] == 0.5)
