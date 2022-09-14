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

#   Source: problem 12 in
#   P.T. Boggs and J.W. Tolle,
#   "A strategy for global convergence in a sequential
#    quadratic programming algorithm",
#   SINUM 26(3), pp. 600-623, 1989.

#   The problem is not convex.

#   SIF input: Ph. Toint, June 1993.

#   classification QQR2-AN-5-3

import poek as pk


model = pk.model()

S = [1, 2, 3, 4, 5]

x = model.add_variable(index=S)
x[1].value = 15.811
x[2].value = 1.5811
x[3].value = 0.0
x[4].value = 15.083
x[5].value = 3.7164

model.add_objective(0.01 * x[1] ** 2 + x[2] ** 2)

model.add_constraint(x[1] + x[2] - x[3] ** 2 == 25.0)
model.add_constraint(x[1] ** 2 + x[2] ** 2 - x[4] ** 2 == 25.0)
model.add_constraint(x[1] - x[5] ** 2 == 2.0)
