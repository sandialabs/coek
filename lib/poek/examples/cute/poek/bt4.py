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

#   Source: a variant of problem 4 in
#   P.T. Boggs and J.W. Tolle,
#   "A strategy for global convergence in a sequential
#    quadratic programming algorithm",
#   SINUM 26(3), pp. 600-623, 1989.

#   The original problem seems to be unbounded.  The contribution of
#   x3 in the first constraint has been squared instead of cubed.

#   The problem is not convex.

#   SIF input: Ph. Toint, June 1993.

#   classification QQR2-AN-3-2

import poek as pk


model = pk.model()

x = model.add_variable(index=[1, 2, 3])
x[1].value = 4.0382
x[2].value = -2.9470
x[3].value = -0.09115

model.add_objective(x[1] - x[2] + x[2] ** 3)

model.add_constraint(-25 + x[1] ** 2 + x[2] ** 2 + x[3] ** 2 == 0)

model.add_constraint(x[1] + x[2] + x[3] - 1 == 0)
