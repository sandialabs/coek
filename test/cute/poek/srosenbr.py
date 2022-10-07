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

#   Source:  problem 21 in
#   J.J. More', B.S. Garbow and K.E. Hillstrom,
#   "Testing Unconstrained Optimization Software",
#   ACM Transactions on Mathematical Software, vol. 7(1), pp. 17-41, 1981.

#   SIF input: Ph. Toint, Dec 1989.

#   classification SUR2-AN-V-0

import poek as pk


model = pk.model()

N = 10000

R1 = list(range(1, N + 1))
R2 = list(range(1, 1 + N // 2))

x = model.add_variable(index=R1)
for i in R1:
    x[i].value = -1.2 if i % 2 == 1 else 1

model.add_objective(
    sum(100 * (x[2 * i] - x[2 * i - 1] ** 2) ** 2 + (x[2 * i - 1] - 1) ** 2 for i in R2)
)
