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

#   Source: Problem 36 in
#   A.R. Buckley,
#   "Test functions for unconstrained minimization",
#   TR 1989CS-3, Mathematics, statistics and computing centre,
#   Dalhousie University, Halifax (CDN), 1989.

#   SIF input: Ph. Toint, Dec 1989.

#   classification NLR2-AN-2-2

import poek as pk


model = pk.model()

Sx = [1, 2]
x = model.add_variable(index=Sx)

model.add_objective(pk.expression(0))
model.add_constraint(0 == x[1] + 2 * x[2] - 7)
model.add_constraint(0 == 2 * x[1] + x[2] - 5)
