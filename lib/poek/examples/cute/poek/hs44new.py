# Adapted from Pyomo model by William E. Hart
# Formulated in pyomo by Logan Barnes. Taken from:

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

#   Source: problem 44 in
#   W. Hock and K. Schittkowski,
#   "Test examples for nonlinear programming codes",
#   Lectures Notes in Economics and Mathematical Systems 187, Springer
#   Verlag, Heidelberg, 1981.

#   SIF input: Ph.L. Toint, October 1990.

#   classification QLR2-AN-4-6

import poek as pk


model = pk.model()
N = 4

M = [1, 2, 3, 4]

x = model.add_variable(index=M, lb=0.0, value=1.0)

model.add_objective(
    x[1] - x[2] - x[3] - x[1] * x[3] + x[1] * x[4] + x[2] * x[3] - x[2] * x[4]
)

model.add_constraint(-x[1] - x[2] + 8 >= 0)
model.add_constraint(-4 * x[1] - x[2] + 12 >= 0)
model.add_constraint(-3 * x[1] - 4 * x[2] + 12 >= 0)
model.add_constraint(-2 * x[3] - x[4] + 8 >= 0)
model.add_constraint(-x[3] - 2 * x[4] + 8 >= 0)
model.add_constraint(-x[3] - x[4] + 5 >= 0)
