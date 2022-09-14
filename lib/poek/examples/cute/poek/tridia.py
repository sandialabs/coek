# TODO
# Formulated in Pyomo by Carl D. Laird, Daniel P. Word, Brandon C. Barrera and Saumyajyoti Chaudhuri
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

#   Source: problem 8 in
#   Ph.L. Toint,
#   "Test problems for partially separable optimization and results
#   for the routine PSPMIN",
#   Report 83/4, Department of Mathematics, FUNDP (Namur, B), 1983.

#   See also Buckley#40 (p.96)

#   SIF input: Ph. Toint, Dec 1989.

#   classification QUR2-AN-V-0

import poek as pk


model = pk.model()

N = 50000
alpha = 2.0
beta = 1.0
gamma = 1.0
delta = 1.0

x = model.add_variable(index=range(1, N + 1), value=1.0)


model.add_objective(
    gamma * (x[1] * delta - 1.0) ** 2
    + sum(i * (-beta * x[i - 1] + alpha * x[i]) ** 2 for i in range(2, N + 1))
)
