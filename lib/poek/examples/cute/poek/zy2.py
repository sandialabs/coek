# Adapted from Pyomo model by William E. Hart
# Formulated in Pyomo by Carl D. Laird, Daniel P. Word, Brandon C. Barrera and Saumyajyoti Chaudhuri
#
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
#   Zhengua Lina and Yong Li,
#   "A Modified Scaling Newton-Type Method for Nonlinear Programming"
#   Department of Mathematics, Jilin University, Changchun, China, 1994.

#   SIF input: Ph. Toint, January 1994.

#   classification OQR2-AN-3-2

import poek as pk


model = pk.model()

x = {}
xinit = {1: 0.1, 2: 0.1, 3: 3.0}
for i in xinit:
    x[i] = model.add_variable(lb=0, value=xinit[i])

model.add_objective(x[1] ** 3 - 6 * x[1] ** 2 + 11 * x[1] + x[2] + x[3])

model.add_constraint(pk.inequality(4, x[1] ** 2 + x[2] ** 2 + x[3] ** 2, 10))
model.add_constraint(x[3] <= 5)
