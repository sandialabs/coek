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

#   Source:  problem 214 (p. 68) in
#   A.R. Buckley,
#   "Test functions for unconstrained minimization",
#   TR 1989CS-3, Mathematics, statistics and computing centre,
#   Dalhousie University, Halifax (CDN), 1989.

#   SIF input: Ph. Toint, Dec 1989.

#   classification NQR2-AN-2-2

import poek as pk


model = pk.model()

N = [1, 2]
x = model.add_variable(index=N)
x[1].value = 0.0
x[2].value = 1.0

e = pk.expression(0)
model.add_objective(e)

model.add_constraint(x[1] * x[2] - 1.0 == 0)
model.add_constraint(x[1] ** 2 + x[2] ** 2 - 4.0 == 0)
