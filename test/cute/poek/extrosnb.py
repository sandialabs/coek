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

#   Source: problem 10 in
#   Ph.L. Toint,
#   "Test problems for partially separable optimization and results
#   for the routine PSPMIN",
#   Report 83/4, Department of Mathematics, FUNDP (Namur, B), 1983.

#   See also Buckley#116.  Note that MGH#21 is the separable version.
#   SIF input: Ph. Toint, Dec 1989.

#   classification SUR2-AN-V-0

import poek as pk


model = pk.model()

N = 10
S = list(range(1, N + 1))
x = model.add_variable(index=S, value=1)

Sf = list(range(2, N + 1))

sumexp = sum(100 * (x[i] - x[i - 1] ** 2) ** 2 for i in Sf)
model.add_objective((x[1] - 1) ** 2 + sumexp)
