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

#   Source:
#   C. Gigola and S. Gomez,
#   "A Regularization Method for Solving the Finite Convex Min-Max Problem",
#   SINUM 27(6), pp. 1621-1634, 1990.

#   SIF input: Ph. Toint, August 1993.

#   classification LQR2-AN-3-3

import poek as pk


model = pk.model()

x = model.add_variable(index=[1, 2], value=2.0)
z = model.add_variable(value=2.0)

model.add_objective(z)

model.add_constraint(z + 5.0 * x[1] - x[2] >= 0)
model.add_constraint(z - 4.0 * x[2] - x[1] ** 2 - x[2] ** 2 >= 0)
model.add_constraint(z - 5.0 * x[1] - x[2] >= 0)
