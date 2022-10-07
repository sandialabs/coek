# TODO
# Formulated in Pyomo by Carl D. Laird, Daniel P. Word, and Brandon C. Barrera
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
#   P.T. Boggs and J.W. Tolle,
#   "A strategy for global convergence in a sequential
#    quadratic programming algorithm",
#   SINUM 26(3), pp. 600-623, 1989.

#   The problem is convex.

#   SIF input: Ph. Toint, June 1993.

#   classification SLR2-AY-5-3

import poek as pk


model = pk.model()

S = [1, 2, 3, 4, 5]

x = model.add_variable(index=S, value=20.0)

model.add_objective((x[1] - x[2]) ** 2 + (x[2] + x[3] - 2) ** 2 + (x[4] - 1) ** 2 + (x[5] - 1) ** 2)

model.add_constraint(x[1] + 3 * x[2] == 0)
model.add_constraint(x[3] + x[4] - 2 * x[5] == 0)
model.add_constraint(x[2] - x[5] == 0)
