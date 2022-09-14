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

#   Source:  The first problem given by
#   R. Fletcher,
#   "An optimal positive definite update for sparse Hessian matrices"
#   Numerical Analysis report NA/145, University of Dundee, 1992.

#   N.B. This formulation is incorrect. See FLETCBV2.SIF for
#        the correct version.

#   SIF input: Nick Gould, Oct 1992.

#   classification OUR2-AN-V-0

import poek as pk

cos = pk.cos


model = pk.model()

n = 10
kappa = 1
objscale = 1.0e0
h = 1 / (n + 1.0)
p = 1 / (objscale)

S = list(range(1, n + 1))
SS = list(range(1, n))

x = model.add_variable(index=S)
for i in S:
    x[i].value = i * h

exp1 = 0.5 * p * (x[1]) ** 2
exp2 = 0.5 * p * (x[n]) ** 2
sum1 = sum(0.5 * p * (x[i] - x[i + 1]) ** 2 for i in SS)
sum2 = sum((p * (-1 - 2 / h**2) * x[i]) for i in S)
sum3 = sum((-kappa * p * cos(x[i]) / h**2) for i in S)
model.add_objective(exp1 + sum1 + exp2 + sum2 + sum3)
